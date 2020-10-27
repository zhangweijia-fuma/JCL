#coding:utf-8

import sys, time, threading,datetime
from utils.utils import *
from utils.debug import *
from config import PisConfig

from libs.db.dbHelper import *

from .consts import *
from .message import *
from .msgHandler import *
from .resultListener import *
from .modbusServer import *
from libs.utils.debug import *

dbHelper = None

# ctrIdMap = {}
# ctrAddrMap = {}

# class Controller() :

# 	def __init__(self, data) :
# 		self.data = data
# 		ctrAddrMap[data["ip_address"]] = self

# def initControllers():
# 	db = dbHelper.getDB("pis.controller")
# 	result = db.find(None, {})
# 	items = dbResultToDict(result)

# 	ctrIdMap = {}
# 	ctrAddrMap = {}

# 	for item in items:
# 		c = Controller(item)

def getDbHelper():
	return DbHelper()

ControllerNames = {}
def getControllerName(cur, controller):
	cid = controller['id']
	if haskey(ControllerNames, cid):
		return ControllerNames[cid]
	sql = ""
	if controller['install_type'] <= '1':
		sql  = "select ol.name || '-' || t.name || '-' || ct.name "
		sql += " from pis.controller ct"
		sql += " inner join pis.operation_line ol on ol.id = ct.line_id"
		sql += " inner join pis.train t on t.id = ct.install_id"
	else:
		sql  = "select ol.name || '-' || st.name || '-' || ct.name "
		sql += " from pis.controller ct"
		sql += " inner join pis.operation_line ol on ol.id = ct.line_id"
		sql += " inner join pis.station st on st.id = ct.install_id"

	sql += " where ct.id = %d" % cid
	cur.execute(sql)
	rows = cur.fetchall()
	if not len(rows):
		return "UNKOWN"

	name = rows[0][0]
	ControllerNames[cid] = name
	return name

def getController(dbh, ipAddr, extra=""):
	db = dbh.getDB("pis.controller")
	if extra:
		extra = "," + extra
	result = db.findByCond("id,ip_address,code,install_type,name" + extra, "ip_address='%s'" % ipAddr, "id desc")
	if not result["rows"]:
		return None

	keys = result["struct"].split(",")
	item = dict(zip(keys, result["rows"][0]))

	item["name"] = getControllerName(dbh.getCursor(), item)
	# logD(str(item))
	return item

def onRunStatusMsg(msg):
	dbh = getDbHelper()
	try:
		c = getController(dbh, msg.addr)
		if not c:
			logE("not found controller @ onRunStatusMsg")
			dbh.closeDB()
			return False

		currentDate = datetime.datetime.now().strftime('%Y-%m-%d')
		currentTime = datetime.datetime.now().strftime('%H:%M:%S')
		fullTime = currentDate + " " + currentTime
		data = {
			"create_time" 	: fullTime,
			"controller" 	: c["id"],
			"ip_address" 	: msg.addr,
			"cpu" 			: msg.cpu,
			"memory" 		: msg.memory,
			"harddisk" 		: msg.harddisk,
			"status" 		: msg.status,
			'date' 			: currentDate,
			'time' 			: currentTime
		}

		db = dbh.getDB("pis.controller_status")
		db.add(data)

		# status = msg.status
		# if status != 0:

		data = {}
		data['create_time'] = fullTime
		data["status"] = msg.status
		data["status_update_time"] = fullTime
		db = dbh.getDB("pis.controller")
		db.save(data, c["id"])
		
	except Exception as e:
		onException(e)
	dbh.closeDB()

	return True

def onLogMsg(msg):
	dbh = getDbHelper()
	try:
		c = getController(dbh, msg.addr)
		if not c:
			E("not found controller @ onRunStatusMsg")
			dbh.closeDB()
			return False

		ds = "%04d-%02d-%02d" % (msg.year, msg.month + 1, msg.day)
		data = {
			"controller" 	: c["id"],
			"date" 			: ds,
			"time" 			: "%02d:%02d:%02d" % (msg.hour, msg.minute, msg.second),
			"log_id" 		: msg.id,
			"level" 		: msg.level,
			"type" 			: msg._type,
			"title" 		: msg.title,
			"content" 		: msg.desc,
			"create_time" 	: datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
		}
		
		db = dbh.getDB("pis.controller_log")
		db.deleteByCond("log_id=%s and date='%s'" % (msg.id, ds))
		db.add(data)
	except Exception as e:
		onException(e)
	dbh.closeDB()
	return True

# 接收到发送节目单响应消息
def OnSendProgramListAck(msg):
	pass

# 接收到发送安全垫片响应消息
def OnSendSafeFileAck(msg):
	pass

def onMsgCallback(msg):
	# logD("onMsgCallback %s %s %s" % (formatHex(msg.type), type(msg), isinstance(msg, RUN_LOG)))

	if isinstance(msg, HEARTBEAT):
		sendAck(msg, HEARTBEAT_ACK)

	elif isinstance(msg, RUN_STATUS):
		if onRunStatusMsg(msg):
			sendAck(msg, RUN_STATUS_ACK)

	elif isinstance(msg, RUN_LOG):
		# D("Log, title:%s, desc:%s" % (msg.title, msg.desc))
		if onLogMsg(msg):
			sendAck(msg, RUN_LOG_ACK)

	elif isinstance(msg, INITIAL_REQUEST):
		sendInitReqAck(msg)

	elif isinstance(msg, SEND_PROGRAM_LIST_ACK):
		OnSendProgramListAck(msg)

	elif isinstance(msg, SEND_SAFE_FILE_ACK):
		OnSendSafeFileAck(msg)

	elif msg.getMsgType() == DownloadCompleteMsg:
		pass
	else:
		logD("no handled msg!")

# 接收到控制器的消息处理
def onCallback(m):
	t = m["type"]
	# logD("controller onCallback : %s" % t)
	if t == "msg":
		msg = m["msg"]
		onMsgCallback(msg)

def sendAck(ackedMsg, ACK_MSG):
	msg = ACK_MSG()
	msg.sid = ackedMsg.sid
	msg.addr = ackedMsg.addr
	msg.setOid(None)
	msgHandler.sendMsg(msg)

def getLineInfo(dbh, ackedMsg):
	addr = ackedMsg.addr

	sql = """
        select c.sort,b.up_start_time,b.up_end_time,b.down_start_time,b.down_end_time,e.id,e.name,e.name_en,d.id,d.name,a.install_type, a.open_time, a.close_time 
        from pis.controller a
        left join pis.operation_line b on b.id=a.line_id
        left join pis.station_line c on c.line_id = a.line_id 
        left join pis.station d on d.id = a.install_id and a.install_type in ('2', '3', '4')
        left join pis.station e on e.id = c.station_id
        where a.ip_address='%s' and e.type='N'
        order by c.sort asc;
	""" % addr

	cursor = dbh.getCursor()

	cursor.execute(sql)
	result = cursor.fetchall()

	result = {
		"rows" : result,
		"struct" : "sort,up_start_time,up_end_time,down_start_time,down_end_time,station_id,station_name,station_name_en,my_station_id,my_station_name,install_type,open_time,close_time"
	}

	result = dbResultToDict(result)
	i = 0
	for item in result:
		if item["my_station_id"] == item["station_id"]:
			#print(item)
			if item["install_type"] == "3" or item["install_type"] == "4":
				first = result[0]
				last = result[len(result) - 1]
				if i < len(result) - 1:
					_next = result[i + 1]
				else:
					_next = {
						"station_name" : "无",
						"station_name_en" : "NA"
					}
				start_time = item["down_start_time"]
				end_time = item["down_end_time"]
			else:
				first = result[len(result) - 1]
				last = result[0]
				if i > 0:
					_next = result[i - 1]
				else:
					_next = {
						"station_name" : "无",
						"station_name_en" : "NA"
					}
				start_time = item["up_start_time"]
				end_time = item["up_end_time"]

			data = {
				"first_station" 		: first["station_name"],
				"first_station_en" 		: first["station_name_en"],
				"end_station" 			: last["station_name"],
				"end_station_en" 		: last["station_name_en"],
				"current_station" 		: item["station_name"],
				"current_station_en" 	: item["station_name_en"],
				"next_station" 			: _next["station_name"],
				"next_station_en" 		: _next["station_name_en"],
				"start_time" 			: start_time,
				"end_time" 				: end_time,
				"controller_type" 		: item['install_type'],
				"open_time" 			: item['open_time'],
				"close_time" 			: item['close_time']
			}

			return data
		i = i + 1
		
	return {
		"first_station" 		: "first_station",
		"first_station_en" 		: "first_station_en",
		"end_station" 			: "end_station",
		"end_station_en" 		: "end_station_en",
		"current_station" 		: "current_station",
		"current_station_en" 	: "current_station_en",
		"next_station" 			: "next_station",
		"next_station_en" 		: "next_station_en",
		"start_time" 			: "06:00",
		"end_time" 				: "23:00",
		"controller_type" 		: '0',
		"open_time"				: "05:45",
		"close_time" 			: "23:55"
	}

# 接收到控制器初始化消息后，检查自上次登录后是否有新闻及节目单下发失败的记录，若有，则重新发送最近一次失败的节目单及新闻消息
def ResendFailedMessage(controller):
	dbh = getDbHelper()
	try:
		pass
	except Exception as e:
		onException(e)

	dbh.closeDB()

def sendInitReqAck(ackedMsg):

	dbh = getDbHelper()
	try:
		c = getController(dbh, ackedMsg.addr)
		if not c:
			logE("sendInitReqAck not found controller %s" % ackedMsg.addr)
			dbh.closeDB()
			return

		screen_path = os.path.join(PisConfig['PISFileRoot'], "screen")
		log_path = os.path.join(PisConfig['PISFileRoot'], "log")

		msg = INITIAL_REQUEST_ACK()
		msg.sid = ackedMsg.sid
		msg.setOid(None)
		msg.addr = ackedMsg.addr
		msg.setParams(screen_path, log_path, c['code'], PisConfig["snatchScreenTime"], getLineInfo(dbh, ackedMsg))
		msgHandler.sendMsg(msg)
	except Exception as e:
		onException(e)

	dbh.closeDB()

	ResendFailedMessage(c)
"""
controllers = [{
	"ip_address" : "192.168.1.100"
	...
}, {
	"ip_address" : "192.168.1.200"
	...
}
]

mrl :
mrl.success		: success count
mrl.fail		: fail count
"""
def sendHeartbeat(controllers):
	mrl = MsgResultListener()
	for item in controllers:
		msg = HEARTBEAT()
		msg.setController(item)
		mrl.addMsg(msg)
		msgHandler.sendMsg(msg)
	return mrl

def sendProgramList(controllers, programFile):
	data = programFile["data"]
	oid = programFile["id"]
	mrl = MsgResultListener()
	for item in controllers:
		msg = SEND_PROGRAM_LIST()
		msg.setParams(data[0])
		msg.setController(item)
		msg.setOid(oid, programFile["operation_id1"], programFile["operation_id2"])
		mrl.addMsg(msg)
		msgHandler.sendMsg(msg)
	return mrl

def sendNews(controllers, info):
	news = info["content"].strip()
	if not news:
		return clearNews(controllers, info)

	if haskey(info, "lifeTime"):
		lifeTime = info["lifeTime"]
	else:
		lifeTime = 0

	oid = info["id"]
	mrl = MsgResultListener()
	for item in controllers:
		msg = SEND_NEWS()
		msg.setController(item)
		msg.setParams(lifeTime, news)
		msg.setOid(oid, info["operation_id1"], info["operation_id2"])
		mrl.addMsg(msg)
		msgHandler.sendMsg(msg)

	# logD("sendNews .... done")
	return mrl

def clearNews(controllers, info):
	oid = info["id"]
	mrl = MsgResultListener()
	for item in controllers:
		msg = CLEAR_NEWS()
		msg.setController(item)
		msg.setOid(oid, info["operation_id1"], info["operation_id2"])
		mrl.addMsg(msg)
		msgHandler.sendMsg(msg)
	return mrl

def sendEmergence(controllers, info):
	news = info["content"].strip()
	if not news:
		return clearEmergence(controllers, info)

	if haskey(info, "lifeTime"):
		lifeTime = info["lifeTime"]
	else:
		lifeTime = 0

	# if haskey(info, "mode"):
	# 	mode = info["mode"]
	# else:
	# 	mode = 0
	mode = info["operation_id1"]

	oid = info["id"]
	mrl = MsgResultListener()
	for item in controllers:
		msg = SEND_EMERGENCE()
		msg.setController(item)
		msg.setParams(lifeTime, mode, news)
		msg.setOid(oid)
		mrl.addMsg(msg)
		msgHandler.sendMsg(msg)
	return mrl

def clearEmergence(controllers, info):
	oid = info["id"]
	mrl = MsgResultListener()
	for item in controllers:
		msg = CLEAR_EMERGENCE()
		msg.setController(item)
		msg.setOid(oid)
		mrl.addMsg(msg)
		msgHandler.sendMsg(msg)
	return mrl

def sendLineInfo(controllers, lineInfo):
	mrl = MsgResultListener()
	for item in controllers:
		msg = SEND_LINE_INFO()
		msg.setController(item)
		msg.setParams(lineInfo)
		msg.setOid(None)
		mrl.addMsg(msg)
		msgHandler.sendMsg(msg)
	return mrl

def sendStationTime(controllers, stationTime):
	mrl = MsgResultListener()
	for item in controllers:
		msg = SEND_STATION_TIME()
		msg.setController(item)
		msg.setParams(stationTime)
		msg.setOid(None)
		mrl.addMsg(msg)
		msgHandler.sendMsg(msg)
	return mrl

def systemReboot(controllers, info):
	oid = info["id"]
	if haskey(info, "delayTime"):
		delayTime = info["delayTime"]
	else:
		delayTime = 0
	mrl = MsgResultListener()
	for item in controllers:
		msg = SYSTEM_REBOOT()
		msg.setController(item)
		msg.setParams(delayTime)
		msg.setOid(oid, info["operation_id1"], info["operation_id2"])
		mrl.addMsg(msg)
		msgHandler.sendMsg(msg)
	return mrl

def sendUpdate(controllers, info):
	oid = info["id"]
	filePath = info["data"][0]
	mrl = MsgResultListener()
	for item in controllers:
		msg = SEND_UPDATE()
		msg.setController(item)
		msg.setParams(filePath)
		msg.setOid(oid, info["operation_id1"], info["operation_id2"])
		mrl.addMsg(msg)
		msgHandler.sendMsg(msg)
	return mrl

def sendFormat(controllers, info):
	oid = info["id"]
	formatFile = info["data"]
	formatSwitchTime = PisConfig["formatSwitchTime"]
	mrl = MsgResultListener()
	for item in controllers:
		msg = SEND_FORMAT_FILE()
		msg.setController(item)
		msg.setParams(formatFile[0], formatFile[1], formatSwitchTime)
		msg.setOid(oid, info["operation_id1"], info["operation_id2"])
		mrl.addMsg(msg)
		msgHandler.sendMsg(msg)
	return mrl


def sendWorkTime(controllers, info):
	# print(info)
	# print(controllers)
	mrl = MsgResultListener()
	oid = info["id"]

	dbh = getDbHelper()
	try:
		for item in controllers:
			msg = UPDATE_WORK_TIME()
			msg.setController(item)
			c = getController(dbh, item["ip_address"], "open_time,close_time")
			if not c:
				raise Exception("控制器未找到")

			open_time = info['open_time']
			close_time = info['close_time']
			msg.setParams(open_time, close_time)
			msg.setOid(oid)
			mrl.addMsg(msg)
			msgHandler.sendMsg(msg)
	except Exception as e:
		dbh.closeDB()
		onException(e)
		return

	dbh.closeDB()
	return mrl

def sendViewControllerVideo(controllers, info):
	mrl = MsgResultListener()
	oid = info["id"]

	dbh = getDbHelper()
	try:
		for item in controllers:
			msg = VIEW_CONTROLLER_VIDEO()
			msg.setController(item)
			msg.setParams(info['data'][0])
			msg.setOid(oid)
			mrl.addMsg(msg)
			msgHandler.sendMsg(msg)

	except Exception as e:
		dbh.closeDB()
		onException(e)
		return

	dbh.closeDB()
	return mrl

def sendSafeVideoFile(controllers, info):
	mrl = MsgResultListener()
	oid = info["id"]
	fileInfo = info['data'][0]
	# logI("安全垫片:", fileInfo)

	dbh = getDbHelper()
	try:
		for item in controllers:
			msg = SEND_SAFE_FILE()
			msg.setController(item)
			msg.setParams(fileInfo['file_name'])
			msg.setOid(oid)
			mrl.addMsg(msg)
			msgHandler.sendMsg(msg)

	except Exception as e:
		dbh.closeDB()
		onException(e)
		return

	dbh.closeDB()
	return mrl

def sendMessage(controllers, msgInfo, resend=False):

	# print(controllers)
	# print(msgInfo)
	# print(resend)

	mtype = msgInfo["message_type"]
	if mtype == SendProgramListMsg:
		return sendProgramList(controllers, msgInfo)
	elif mtype == SendNewsMsg:
		return sendNews(controllers, msgInfo)
	elif mtype == SendUpdateMsg:
		return sendUpdate(controllers, msgInfo)
	elif mtype == SystemRebootMsg:
		return systemReboot(controllers, msgInfo)
	elif mtype == SendFormatFileMsg:
		return sendFormat(controllers, msgInfo)
	elif mtype == SendEmergenceMsg:
		return sendEmergence(controllers, msgInfo)
	elif mtype == UpdateWorkTimeMsg:
		return sendWorkTime(controllers, msgInfo)
	elif mtype == ViewControllerVideoMsg:
		return sendViewControllerVideo(controllers, msgInfo)
	elif mtype == SendSafeVideoMsg:
		return sendSafeVideoFile(controllers, msgInfo)

	logE("sendMessage, error msgtype: %d"%mtype)
	return None

def init():
	msgHandler.setCallback(onCallback)

if USING_MSGER:
	init()
