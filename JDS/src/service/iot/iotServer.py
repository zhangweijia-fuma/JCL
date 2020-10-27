# coding:utf-8

# IOT 消息处理

import sys
import datetime
import threading
from xml.etree import ElementTree  
import urllib.request
from socket import *

import libs.db.dbMysql, config

from libs import db
from libs.socketMsg.messageSocket import *
from libs.socketMsg.messageBase import *

from libs.utils.debug import *

from iot import iotConfig
from iot.iotMessage import *

# IOT 业务消息处理
# 不使用线程方式，待后续根据业务量再考虑
class iotServer(threading.Thread):
	def __init__(self, socket_type = SOCK_STREAM):
		threading.Thread.__init__(self)

		self.database 	= None
		self.db 		= None
		self.cursor 	= None

		self.moduleList = []

		self.timer 		= None
		self.serverSock = None

		self.socket_type = socket_type

		return

	def __del__(self):
		self.close()

		return

	# 启动
	def run(self):

		# 是否配置了IOT业务
		if "IOT" not in config.SystemModule:
			logI("没有配置IOT业务")
			return

		# 连接到数据库
		self.openDB()

		# 获取定位模块列表
		self.getModuleList()

		# 设置定时器，定期检查心跳消息接收状态
		self.timer = threading.Timer(HEARTBEAT_INTERVAL, self.checkModuleStatus)
		self.timer.start()

		# 启动服务端，开始准备接收消息
		self.serverSock = socketServer( iotConfig.SOCKET_TYPE, iotConfig.serverPort, self.messageProcess )
		self.serverSock.start()

		logI("IOT Server start....")

		# 进入线程主循环，等待发送或接收消息
		while (1):
			time.sleep(1)

		# return

	def close(self):
		if self.serverSock: 
			self.serverSock.close()

		if self.timer:
			self.timer.cancel()

		if self.database:
			self.closeDB()

		return

	# 连接到数据库
	def openDB(self):
		database 	= db.dbMysql.DB(config.DbConfig)
		dbItem  	= db.dbMysql.CURD(database, "iot.gps_module", False)
		try :
			# if self.debug != None:
			# 	database.setDebug(self.debug)
			database.open(False)
		except:
			return

		self.database = database
		self.cursor   = database.getCursor()
		self.db 	  = dbItem
		return

	def closeDB(self):
		if self.database:
			try :
				self.database.close()
			except:
				pass

		self.database = None
		self.cursor   = None
		self.db 	  = None

		return

	# 获取定位模块列表
	def getModuleList(self):

		sql  = "select gm.id, gm.name, gm.code, gm.serial_no, gm.device_type, gm.vendor, gm.mac_addr, gm.install_date, gm.install_batch, gm.install_type, gm.install_id, gm.status"
		sql += " from iot.gps_module gm"
		if iotConfig.IOT_DEBUG:
			sql += " where gm.id <= 2"
		sql += " order by gm.id"

		self.openDB()
		cur = self.cursor
		cur.execute(sql)
		rows = cur.fetchall()
		self.closeDB()

		key = ['id', 'name', 'code', 'serial_no', 'device_type', 'vendor', 'mac_addr', 'install_date', 'install_batch', 'install_type', 'install_id', 'status']
		for row in rows:
			moduleInfo = dict(zip(key, row))
			moduleInfo['expire_times'] 	= 0
			self.moduleList.append(moduleInfo)

		return

	# 定时检查模块状态，以更新模块连接状态
	def checkModuleStatus(self):

		now = GetSystemTime()
		for module in self.moduleList:
			if module['expire_times'] == MSG_EXPIRE_TIMES:
				# 3次没有收到心跳消息，则更新状态为未连接
				module['status'] = 5
				data = { 'status' : '5', }
				self.openDB()
				self.db.save(data, module['id'], table='iot.gps_module')
				self.closeDB()

				# 重置超时次数为0
				module['expire_times'] = 0
			else:
				module['expire_times'] += 1

		# 重新设置定时器
		self.timer = threading.Timer(HEARTBEAT_INTERVAL, self.checkModuleStatus)
		self.timer.start()

		return

	# 消息处理
	def messageProcess(self, data, clientSock, clientAddr):

		# logI("receive message from client")

		msg = messageBase()
		msg.setMsgBuffer(data)

		if msg.decodeHead() == False:
			return

		module = None

		# 检查是否是合法的定位模块
		code = msg.code
		mac_addr = msg.mac_addr

		for moduleInfo in self.moduleList:
			if moduleInfo['code'] == code or moduleInfo['mac_addr'] == mac_addr:
				module = moduleInfo 

		if module == None:
			logE("receive message from a invalid module: code: %s, mac_addr: %s"%(code, mac_addr) )
			return

		msgType = msg.getMsgType()

		ackMsg = None

		# 心跳消息
		if msgType == HEARTBEAT_MSG:
			msg = heartbeatMsg()
			msg.setMsgBuffer(data)
			if msg.decode() == False:
				return

			ackMsg = self.processHeartbeatMsg(module, msg)

		# 模块初始化消息
		elif msgType == IOT_MODULE_INITIAL:
			msg = iotModuleInitialMsg()
			msg.setMsgBuffer(data)
			if msg.decode() == False:
				return

			ackMsg = self.processModuleInitialMsg(module, msg)

		# 位置上报消息
		elif msgType == IOT_LOCATION_REPORT:
			msg = iotLocationReportMsg()
			msg.setMsgBuffer(data)
			if msg.decode() == False:
				return

			ackMsg = self.processLocationReportMsg(module, msg)

		else:
			logE("receive an error message from client: ", msgType)

		if ackMsg == None:
			return

		ackMsg.setDestAddr(clientAddr[0])
		logI("send message to client: %s"%ackMsg.__repr__() )

		ackMsg.encode()
		clientSock.sendMessage(ackMsg.getMsgBuffer(), clientAddr )

		return

	# 处理心跳消息，更新定位模块状态
	def processHeartbeatMsg(self, module, msg):

		logI("receive message: %s"%msg.__repr__())

		module['status'] 		= '6'
		module['expire_times'] 	= 0
		data = { 'status' : '6', }

		self.openDB()
		self.db.save(data, module['id'], table='iot.gps_module' )
		self.closeDB()

		ackMsg = heartbeatAckMsg()

		return ackMsg

	# 处理模块初始化消息
	def processModuleInitialMsg(self, module, msg):

		logI("receive message: %s"%msg.__repr__())

		ackMsg = iotModuleInitialAckMsg()
		return ackMsg

	# 定位模块位置上报消息
	def processLocationReportMsg(self, module, msg):

		logI("receive message: %s"%msg.__repr__())

		data = {
			'module_id' : module['id'],
			'ip_addr' 	: msg.ip_addr,
			'longitude'	: msg.lng,
			'latitude'	: msg.lat,
			'time'		: msg.time,
			'cpu'		: "%0.2f"%(msg.cpu / 100) ,
			'memory' 	: "%0.2f"%(msg.memory / 100),
			'harddisk'  : "%0.2f"%(msg.harddisk / 100),
		}

		data['addr'] = self.searchAddrByLngLat(msg.lng, msg.lat)

		self.openDB()
		self.db.save(data, table='iot.gps_module_trace' )
		self.closeDB()

		ackMsg = iotLocationReportAckMsg()

		return ackMsg

	# 使用百度API查询经纬度对应的地址
	# 参数：
	# 	lng：经度
	# 	lat：纬度
	# 返回值：对应的地址名称
	def searchAddrByLngLat(self, lng, lat):

		# logI("根据经纬度获取地址...")

		url = 'http://api.map.baidu.com/geocoder/v2/?callback=renderReverse&location='+ lat + ',' + lng + '&output=xml&pois=1&ak=%s'%iotConfig.appKey

		# XML格式的返回数据
		req = urllib.request.urlopen(url)

		# 将其他编码的字符串解码成unicode
		res = req.read().decode("utf-8")

		# 解析XML时直接将字符串转换为一个Element，解析树的根节点
		root = ElementTree.fromstring(res)

		# find()用于查找属于某个tag的第一个element，这里查找结构化地址
		node_find = root.find('result/formatted_address')
		return node_find.text

# def InitialIOTServer():
# 	if "IOT" in config.SystemModule: 
# 		from iot import iotServer
# 		server = iotServer.iotServer()
# 		server.init()

# InitialIOTServer()

if "IOT" in config.SystemModule:
	info = {
		"name" 	: "IOT",
		"server": iotServer(),
	}
	SystemModuleList.append( info )

