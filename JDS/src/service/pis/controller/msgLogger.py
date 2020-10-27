#coding:utf-8

import sys, time, threading, socket
from multiprocessing import Queue
from queue import Empty
from utils.utils import *
from utils.debug import *
from config import PisConfig
from .consts import *
from .message import *
from libs.db.dbHelper import *

from libs.JCL.JCLCommon import *


class MsgLogger(threading.Thread) :

	def __init__(self, callback = None, queueSize = 10000) :
		threading.Thread.__init__(self)
		self.msgQueue = Queue(queueSize)
		self.dbHelper = None
		self.myName = ""
		try :
			self.myAddr = getLocalIPAddr()
		except Exception as e:
			self.myAddr = "127.0.0.1"
			onException(e)

		station = PisConfig["stationServer"]
		if station:
			info = {
				"id" : 0,
				"code" : "BBBBBB",
				"install_type" : "10"
			}
		else:
			info = {
				"id" : 0,
				"code" : "CCCCCC",
				"install_type" : "8"
			}

		info["name"] = self.getMyName()
		info["ip_address"] = self.myAddr
		self.info = info

		self.mutex = threading.Lock()
		self.msgCache = {}

	def getMyName(self):
		if self.myName:
			return self.myName

		if not PisConfig["stationServer"]:
			self.myName = "中心服务器"
			return self.myName

		code = PisConfig["stationCode"]
		try :
			db = self.getDB("pis.station")
			result = db.findByCond("name", "code='%s'" % code)
			if len(result["rows"]):
				self.myName = result["rows"][0][0] + "车站服务器"
			return self.myName
		except Exception as e:
			onException(e)
			self.closeDB()
			return "xxx"

	def getMyInfo(self):
		self.info["name"] = self.getMyName()
		return self.info

	def getControllerName(self, item):
		from pis.controller.controller import getControllerName
		try:
			if not haskey(item, "fix_name"):
				item["name"] = getControllerName(self.getCursor(), item)
		except Exception as e:
			onException(e)
			self.closeDB()
			if not haskey(item, "name"):
				item["name"] = "ERR"

	def getController(self, addr):
		db = self.getDB("pis.controller")
		result = db.findByCond("id,ip_address,code,install_type,name", "ip_address='%s'" % addr, "id desc")
		if not result["rows"]:
			return None
		keys = result["struct"].split(",")
		item = dict(zip(keys, result["rows"][0]))
		self.getControllerName(item)
		# print(item)
		return item

	def putMsg(self, msg):
		self.mutex.acquire()
		key = id(msg)
		try :
			self.msgCache[key] = msg
		except Exception as e:
			onException(e)
		self.mutex.release()
		return key

	def getMsg(self, key):
		if not key:
			return None
		self.mutex.acquire()
		msg = None
		try :
			if haskey(self.msgCache, key):
				msg = self.msgCache[key]
				del self.msgCache[key]
		except Exception as e:
			onException(e)
		self.mutex.release()
		return msg

	def logMsg(self, msg):
		try :
			key = self.putMsg(msg)
			self.msgQueue.put(key, block=True)
			return True
		except Exception as e:
			onException(e)
			logE("TxMsgQueue.put failed")
			return False

	def getDB(self, table):
		if not self.dbHelper:
			self.dbHelper = DbHelper(auto=True)
		return self.dbHelper.getDB(table)

	def getCursor(self):
		if not self.dbHelper:
			self.dbHelper = DbHelper(True)
		return self.dbHelper.getCursor()

	def closeDB(self):
		try :
			if self.dbHelper:
				self.dbHelper.closeDB()
				self.dbHelper = None
		except Exception as e:
			onException(e)

	"""

	sender_type : SELECT * FROM system.code_value where type_code='ENTITY_TYPE';
	"0";"车头控制器"
	"1";"车尾控制器"
	"2";"上行控制器"
	"3";"下行控制器"
	"4";"站厅控制器"
	"5";"其他控制器"
	"6";"车站服务器"
	"7";"流媒体服务器"
	"8";"综合监控系统"
	"9";"ATS"
	"10";"中心服务器"

	p_send_status : SELECT * FROM system.code_value where type_code='PROGRAM_SEND_STATUS'

	send_status : SELECT * FROM system.code_value where type_code='CTRL_RECEIVE_STATUS'
	"1";"成功"
	"2";"待接收"
	"0";"失败"

	"""

	def countLog(self, oid):
		cur = self.getCursor()
		sql = """ select count(id) as all_count, sum(case send_status when '1' then 1 else 0 end) as success_count, sum(case send_status when '0' then 1 else 0 end) as fail_count
			from pis.message_log where id in (select a.id from (select max(id) as id from pis.message_log where msg_list_id=%s group by receiver_addr) a)
			""" % oid
		# logD(sql)
		cur.execute(sql)
		result = cur.fetchone()
		# logD(result)
		return result[0], result[1], result[2]

	def getMsgSn(self, oid, receiver_id):

		# db = self.getDB("pis.message_log")
		# result = db.findByCond("max(message_sn)", "msg_list_id=%s and receiver_id=%s" % (oid, receiver_id))
		# if not result["rows"]:
		# 	return 1
		# elif result["rows"][0][0]:
		# 	return result["rows"][0][0] + 1
		# else:
		# 	return 1

		if not oid:
			return 1
		cur = self.getCursor()
		sql = "update pis.message_log set message_sn=0 where msg_list_id=%s and receiver_id=%s" % (oid, receiver_id)
		cur.execute(sql)
		return 1

	def updateStatus(self, ackedMsg, oid, status):
		# if isinstance(ackedMsg, SEND_PROGRAM_LIST):
		# 	data = {
		# 		"send_status" : status
		# 	}
		# 	db = self.getDB("pis.program_list")
		# 	db.save(data, oid)
		# elif (isinstance(ackedMsg, SEND_NEWS) or isinstance(ackedMsg, CLEAR_NEWS)
		# 	or isinstance(ackedMsg, SEND_EMERGENCE) or isinstance(ackedMsg, CLEAR_EMERGENCE)
		# 	or isinstance(ackedMsg, SYSTEM_REBOOT) or isinstance(ackedMsg, SEND_UPDATE)):

		data = {
			"send_status" : status
		}
		db = self.getDB("pis.message_list")
		db.save(data, oid)

	def handle(self, msg):
		if hasattr(msg, "controller"):
			c = msg.controller
			self.getControllerName(c)
		else:
			c = self.getController(msg.addr)
			if c:
				msg.setController(c)
		if not c:
			logE("MsgLogger not found controller")
			return

		ntime = datetime.datetime.now()
		cdate = ntime.strftime('%Y-%m-%d')
		ctime = ntime.strftime('%H:%M:%S')
		myInfo = self.getMyInfo()

		if msg.tx:
			packetStr = " ".join(["%02x" % d for d in msg.packet])
			data = {
				"sender_id"   : myInfo["id"],
				"sender_type" : myInfo["install_type"],
				"sender_name" : myInfo["name"],
				"sender_code" : myInfo["code"],
				"sender_addr" : myInfo["ip_address"],

				"receiver_id"   : c["id"],
				"receiver_type" : c["install_type"],
				"receiver_name" : c["name"],
				"receiver_code" : c["code"],
				"receiver_addr" : c["ip_address"],

				"message_type" : msg.type,
				# "message_sn" : msg.sid,
				"message_sn" : self.getMsgSn(msg.oid, c["id"]),
				"msg_list_id" : msg.oid,
				"operation_id1" : msg.oid1,
				"operation_id2" : msg.oid2,
				"send_date" : cdate,
				"send_time" : ctime,
				"receive_date" : None,
				"receive_time" : None,
				"content_type" : "0",
				"bin_content" : packetStr,
				"text_content" : msg.toString(),
				"send_status" : "2",
				"create_time" : cdate + " " + ctime,
			}
			if msg.ack:
				data["send_status"] = 1

			# 心跳消息及响应消息不记录到数据库
			if msg.type != 0 and msg.type != 1 and not msg.ack:
				db = self.getDB("pis.message_log")
				msg.msgLogId = db.add(data)

		elif isinstance(msg, MSG_UNACKED):
			oldMsg = msg.oldMsg
			if hasattr(oldMsg, "msgLogId"):
				data = {
					"send_status" : 0
				}
				db = self.getDB("pis.message_log")
				db.save(data, oldMsg.msgLogId)

				oid = oldMsg.oid
				if oid :
					count, success, fail = self.countLog(oid)
					if count == fail:
						status = "5"
						self.updateStatus(oldMsg, oid, status)
			else:
				logE("MsgLogger not found oldMsg's msgLogId")

		else:

			# 接收到控制器发的请求消息后，在message_list表中新增一条记录
			# 发送时间和接收时间设置为相同值
			# 心跳消息不记录
			if not msg.ack and msg.type != 0:
				data = {
					'create_time' 	: datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
					'create_id' 	: 0,
					'message_type' 	: msg.type,
					'operation_id1' : None,
					'operation_id2' : None,
					'content' 		: msg.toString(),
					'sender_id' 	: c["id"],
					'sender_name' 	: c["name"],
					'send_date'		: cdate,
					'send_time' 	: ctime,
					'send_status' 	: 3,
				}

				db = self.getDB('pis.message_list')
				msgListId = db.add(data)
			else:
				msgListId = None

			packetStr = " ".join(["%02x" % d for d in msg.packet])
			data = {
				"sender_id" 		: c["id"],
				"sender_type" 		: c["install_type"],
				"sender_name" 		: c["name"],
				"sender_code" 		: c["code"],
				"sender_addr" 		: c["ip_address"],

				"receiver_id"   	: myInfo["id"],
				"receiver_type" 	: myInfo["install_type"],
				"receiver_name" 	: myInfo["name"],
				"receiver_code" 	: myInfo["code"],
				"receiver_addr" 	: myInfo["ip_address"],

				"message_type" 		: msg.type,
				# "message_sn" 		: msg.sid,
				"message_sn" 		: 1,
				"msg_list_id" 		: msgListId,
				"send_date" 		: cdate,
				"send_time" 		: ctime,
				"receive_date" 		: cdate,
				"receive_time" 		: ctime,
				"content_type" 		: "0",
				"bin_content" 		: packetStr,
				"text_content" 		: msg.toString(),
				"send_status" 		: "1",
				"create_time" 		: cdate + " " + ctime,
			}

			if msg.ack:
				if hasattr(msg, "ackedMsg"):
					ackedMsg = msg.ackedMsg
					if hasattr(ackedMsg, "msgLogId"):
						# 更新message_log记录的发送状态
						status = 1

						# 节目单及安全垫片消息需要确认视频文件是否下载完成
						if ackedMsg.getMsgType() in msgForDownloadList:
							status = 3

						temp = {
							"send_status" 	: status,
							"receive_date" 	: ntime.strftime('%Y-%m-%d'),
							"receive_time" 	: ntime.strftime('%H:%M:%S'),
						}

						db = self.getDB("pis.message_log")
						db.save(temp, ackedMsg.msgLogId)

						oid = ackedMsg.oid
						if oid:
							count, success, fail = self.countLog(oid)
							#logD("%s %s %s" % (count, success, fail))
							if count == success:
								status = "3"
							else:
								status = "4"
							self.updateStatus(ackedMsg, oid, status)
					else:
						logE("MsgLogger not found ackedMsg's msgLogId")
				else:
					logE("MsgLogger not ackedMsg of a ackMsg")

			# 心跳消息及响应消息不记录到数据库
			if msg.type != 0 and msg.type != 1 and not msg.ack:
				db = self.getDB("pis.message_log")
				db.add(data)

	def run(self):
		# logD("TxMsgQueue start!!!")
		while True:
			msg = None
			try :
				msg = self.msgQueue.get(timeout = 10000)
			except Empty as e:
				pass
			except Exception as e:
				onException(e)

			# print(self.msgCache)
			msg = self.getMsg(msg)
			if msg:
				# logD("msgLogger, got msg xxxxx")
				try:
					self.handle(msg)
				except Exception as e:
					logE("TxMsgQueue handle failed")
					onException(e)
					self.closeDB()

msgLogger = MsgLogger()
if USING_MSGER:
	msgLogger.start()
