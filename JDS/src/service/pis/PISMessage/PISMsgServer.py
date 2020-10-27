# coding:utf-8

# PIS 控制器消息处理

import os, sys, time, threading, datetime

import urllib.request
from socket import *

from libs.utils import debug
from libs.utils.debug import *
from libs.utils.utils import *
from libs.db.dbHelper import *

from libs.JCL.MsgSocket import *

from config import *
from config import PisConfig

from pis.PISMessage.PISMessageBase import *
from pis.PISMessage.PISMessage import *
from pis.PISMessage.ModbusServer import *

class CPISMsgServer(threading.Thread):
	def __init__(self):
		threading.Thread.__init__(self)

		self.TCPServer 			= None 									# TCP Server Socket，控制器 TCP 通信
		self.UDPServer 			= None 									# UDP Server socket，控制器 UDP 通信
		self.ModbusServer 		= None 									# Modbus Server，Modbus 通信
		self.StationMsgServer 	= None

		self.onlineControllers 	= [] 									# 在线控制器列表，车站服务器只有站厅和上下行控制器
		self.sentMsgList 		= [] 									# 已发送消息列表
		self.resendMsgTimer 	= None 									# 重发消息定时器
		self.heartbeatTimer 	= None 									# 心跳消息定时器，中心服务器做检查控制器状态，车站服务器做发送心跳消息

		try :
			self.LocalAddr = gethostbyname(gethostname())
		except Exception as e:
			self.LocalAddr = "127.0.0.1"
			onException(e)

		# if debug.bIsInitial == False:
		# 	print("bIsInitial == False")

		# # 根据IP地址查找是否为车站服务器还是中心服务器
		# # 开发期间这样使用，正式运行时需要根据配置参数来确定
		# if self.GetStation(self.LocalAddr) != None:
		# 	self.isStation = True
		# 	os.environ['PISMode'] = "Station"
		# 	logI("\n\n\nPIS Station Server starting...\n\n\n")
		# else:
		# 	self.isStation = False
		# 	os.environ['PISMode'] = "CenterServer"
		# 	logI("\n\n\nPIS Center Server starting...\n\n\n")

		self.isStation 			= PisConfig["stationServer"] 			# 车站服务器标识，True：车站服务器，False：中心服务器
		if self.isStation: 												# 车站服务器
			os.environ['PISMode'] = "Station"

			self.LocalInfo = {
				"id" 			: 0,
				"code" 			: PisConfig["stationCode"],
				"name" 			: "车站服务器",
				"install_type" 	: '10',
				"ip_address" 	: self.LocalAddr,
			}

			self.GetStationInfo()

		else: 															# 中心服务器
			os.environ['PISMode'] = "CenterServer"
			self.downloadMsgList 	= [] 								# 需要等待下载完成消息的消息列表
			self.onlineStation 		= [] 								# 在线车站服务器列表

			self.LocalInfo = {
				"id" 			: 0,
				"code" 			: "CCCCCC",
				"install_type" 	: "8",
				"ip_address" 	: self.LocalAddr,
				"name" 			: "中心服务器",
			}

			# 中心服务器启动时，更新所有控制器、车站服务器状态为离线状态
			dbHelper = DbHelper(auto=True)
			try:
				now = datetime.datetime.now()
				timeStr = now.strftime("%Y-%m-%d %H:%M:%S")
				db = dbHelper.openDB("pis.controller")
				cur = dbHelper.getCursor()
				sql = "update pis.controller set status = '5', status_update_time = '%s'"%timeStr
				result = cur.execute(sql)

				sql = "update pis.station set status = '5'"
				result = cur.execute(sql)

			except Exception as e:
				onException(e)

			dbHelper.closeDB()

		return

	# 获取车站服务器信息
	def GetStationInfo(self):

		code = PisConfig["stationCode"]
		try :
			dbHelper = DbHelper()
			db = dbHelper.getDB("pis.station")
			result = db.findByCond("id,name", "code='%s'" % code)
			if len(result["rows"]):
				self.LocalInfo['id'] 	= result["rows"][0][0]
				self.LocalInfo['name'] 	= result["rows"][0][1] + "车站服务器";
			dbHelper.closeDB()
			return

		except Exception as e:
			onException(e)
			return


	def __del__(self):
		self.close()

		return

	# 启动
	def run(self):

		# 是否配置了PIS业务
		if "PIS" not in config.SystemModule:
			logE("Error: System not config PIS module")
			return

		# 车站服务器不需要检查控制器的心跳消息
		if self.isStation == False:
			# 中心服务器，设置定时器，定期检查心跳消息接收状态
			self.heartbeatTimer = threading.Timer(HEARTBEAT_INTERVAL, self.CheckOnlineStatus)
			self.heartbeatTimer.start()

			# 中心服务器需要单独启动一个线程来处理车站服务器的状态消息
			self.StationMsgServer = CSocketServer( SOCK_DGRAM, PisConfig["StationServerPort"], self.StationMessageProcess )
			self.StationMsgServer.start()

			timer = threading.Timer(HEARTBEAT_INTERVAL, self.CheckControlerStatus)
			timer.start()
		else:
			# 车站服务器，定时发送心跳消息给中心服务器
			self.MsgSerialNo 	= 1 													# 消息序号每天从1开始
			self.CurrentDate 	= datetime.datetime.now().strftime("%Y-%m-%d") 			# 当期日期

			self.UDPClient = CSocketClient( SOCK_DGRAM, PisConfig["PISServerAddr"],  PisConfig["StationServerPort"], self.ProcessServerMsg )
			self.UDPClient.start()

			self.heartbeatTimer = threading.Timer(HEARTBEAT_INTERVAL, self.SendHeartbeatMsgToServer)
			self.heartbeatTimer.start()
			self.SendHeartbeatMsgToServer() 											# 立即发送心跳消息给中心服务器


		# 以下为车站服务器及中心服务器共有部分

		# 设置定时器，重发消息
		self.resendMsgTimer = threading.Timer(MSG_RESEND_INTERVAL, self.CheckSentMsg)
		self.resendMsgTimer.start()

		# 启动Modbus TCP服务端，开始准备接收Modbus消息
		self.ModbusServer = CModbusServer(self)
		self.ModbusServer.start()

		# 启动TCP服务端，开始准备接收TCP消息
		self.TCPServer = CSocketServer( SOCK_STREAM, PisConfig["TCPServerPort"], self.ControllerMessageProcess )
		self.TCPServer.start()

		# 启动UDP服务端，开始准备接收消息
		self.UDPServer = CSocketServer( SOCK_DGRAM, PisConfig["UDPServerPort"], self.ControllerMessageProcess )
		self.UDPServer.start()

		logI("PIS Server Starting...")

		# 进入线程主循环，等待发送或接收消息
		while (1):
			time.sleep(1)


	def close(self):
		if self.TCPServer: 
			self.TCPServer.close()

		if self.UDPServer:
			self.UDPServer.close()

		if self.heartbeatTimer:
			self.heartbeatTimer.cancel()

		if self.resendMsgTimer:
			self.resendMsgTimer.cancel()

		if self.ModbusServer:
			self.ModbusServer.close()

		if self.isStation == False and self.StationMsgServer:
			self.StationMsgServer.close()

		return

	# ##################################################################
	# 车站服务器和中心服务器之间的消息处理部分，只处理心跳消息

	# 车站服务器，发送心跳消息给中心服务器
	def SendHeartbeatMsgToServer(self):
		currentDate 	= datetime.datetime.now().strftime("%Y-%m-%d")
		if self.CurrentDate != currentDate:
			self.CurrentDate = currentDate
			self.MsgSerialNo = 1
		else:
			self.MsgSerialNo += 1

		msg = CHeartBeatMsg()
		msg.serialNo = self.MsgSerialNo
		msg.srcAddr  = self.LocalAddr
		msg.destAddr = PisConfig['PISServerAddr']


		basemsg 		= CHeartBeatMsg() 								# 使用消息基类的编码功能
		msgData 		= basemsg.encodeMsg(msg)
		serverAddr 		= (PisConfig['PISServerAddr'], PisConfig['StationServerPort'])
		
		logDD("Send HeartBeatMsg to PIS Sevrer(%s:%d)"%(PisConfig['PISServerAddr'], PisConfig['StationServerPort']))
		self.UDPClient.sendMessage( msgData, serverAddr )

		self.heartbeatTimer = threading.Timer(HEARTBEAT_INTERVAL, self.SendHeartbeatMsgToServer)
		self.heartbeatTimer.start()

	# 处理从中心服务器来的消息，只有心跳应答消息，对消息不做任何处理
	def ProcessServerMsg(self, data, serverAddr):
		basemsg = CHeartBeatMsg() 										# 使用消息基类的解码功能
		msg = basemsg.decodeMsg(data)
		
		logDD("Receive data from server(%s:%d): AAAAAA%s"%(serverAddr[0], serverAddr[1], msg.toString()))

		if msg == None:
			logE("Error: Decode message failed, Data: %s"%ConvertToHex(data))
			return

		msgType = msg.getMsgType()

		if msgType != HeartBeatAck:
			logE("Error: Receive an unsupport message %d from PIS Sevrer"%(msgType))
			return

		logDD("Receive %s from PIS Server"%msg.toString())
		return

	# ##################################################################
	# 中心服务器处理部分

	# 更新控制器状态， 0：Online， 5： Offline
	def UpdateControllerState(self, controller, status):
		
		# 更新控制器状态
		now = datetime.datetime.now()
		data = { 
			'status' : status, 											# 状态：0；Online，5：Offline 
			'status_update_time' : now.strftime('%Y-%m-%d %H:%M:%S'),
		}

		dbHelper = DbHelper()
		db = dbHelper.getDB("pis.controller")
		db.save(data, controller['id'])
		dbHelper.closeDB()

		logI("%s(%s:%d) %s"%(controller['name'], controller['ip_address'], controller['port'], "Online" if status == 0 else "Offline"))

		return

	# 更新车站服务器状态， 0：Online， 5： Offline
	def UpdateStationState(self, station, status):
		
		# 更新车站服务器状态
		now = datetime.datetime.now()
		data = { 
			'status' : status, 											# 状态：0；Online，5：Offline 
		}

		dbHelper = DbHelper()
		db = dbHelper.getDB("pis.station")
		db.save(data, station['id'])
		dbHelper.closeDB()

		logI("%s站车站服务器(%s:%d) %s"%(station['name'], station['ip_address'], station['port'], "Online" if status == 0 else "Offline"))

		return


	# 临时使用
	def CheckControlerStatus(self):
		return
		dbHelper = DbHelper()
		db = dbHelper.getDB("pis.controller")
		result = db.findByCond("id,name,ip_address,status,status_update_time", "id=1")
		dbHelper.closeDB()

		timer = threading.Timer(HEARTBEAT_INTERVAL, self.CheckControlerStatus)
		timer.start()

		if len(result["rows"]) == 0:
			return None

		keys = result["struct"].split(",")
		item = dict(zip(keys, result["rows"][0]))
		if item['status'] != '0':
			logI("Controller %s lose connection: %s"%(item['ip_address'], item['status_update_time']))
		else:
			logI("Controller %s keep connection: %s"%(item['ip_address'], item['status_update_time']))
		

	# 根据IP地址获取车站服务器信息
	def GetStation(self, ipAddr):
		
		dbHelper = DbHelper()
		db = dbHelper.getDB("pis.station")
		result = db.findByCond("id,name,name_en,code,type,description,ip_address,status", "ip_address='%s'" % ipAddr)
		dbHelper.closeDB()

		if len(result["rows"]) == 0:
			return None

		keys = result["struct"].split(",")
		item = dict(zip(keys, result["rows"][0]))
		return item


	# 查找车站服务器是否在线
	def FindOnlineStation(self, station):
		for item in self.onlineStation:
			if item['id'] == station['id']:
				return item

		return None

	# 处理车站服务器的消息，车站服务器和中心服务器之间只有心跳消息
	def StationMessageProcess(self, data, clientSocket, clientAddr):
		basemsg = CHeartBeatMsg() 										# 使用消息基类的解码功能
		msg = basemsg.decodeMsg(data)

		if msg == None:
			logE("Error: Decode message failed, Data: %s"%ConvertToHex(data))
			return

		msg.isSendMsg = False

		clientIPAddr 	= clientAddr[0]
		clientPort 		= clientAddr[1]
		station 		= None
		msgType 		= msg.getMsgType()

		if msgType != HeartBeatMsg:
			logE("Error: Receive an unsupport message %d from station %s"%(msgType, clientIPAddr))
			return

		station = self.GetStation(clientIPAddr)
		if station == None:
			logE("Error: Receive message from a invalid station: %s"%(clientIPAddr) )
			return

		sendStation = self.FindOnlineStation(station) 				# 中心服务器需要检查车站服务器是否已经在列表中，不在则添加进队列
		if sendStation == None:
			station['socket'] 		= clientSocket
			station['port'] 		= clientPort
			station['expire_times'] = 0
			self.onlineStation.append(station)

			self.UpdateStationState(station, 0)						# 更新车站服务器状态为在线
		else:
			sendStation['expire_times'] = 0

		# 返回心跳应答消息
		ackMsg = CHeartBeatAckMsg()
		ackMsg.serialNo = msg.serialNo
		ackMsg.srcAddr 	= self.LocalAddr
		ackMsg.destAddr = msg.srcAddr
		ackMsg.setOperationID(None)

		basemsg 		= CHeartBeatMsg() 								# 使用消息基类的编码功能
		msgData 		= basemsg.encodeMsg(ackMsg)
		logDD("Send %s to %s:%d, data: %s"%(ackMsg.toString(), clientAddr[0], clientAddr[1], ConvertToHex(msgData)))
		clientSocket.sendMessage( msgData, clientAddr )

		return

	# 定时检查控制器、车站服务器状态，以更新控制器连接状态
	def CheckOnlineStatus(self):

		for module in self.onlineControllers:
			if module['expire_times'] == MSG_EXPIRE_TIMES:
				self.UpdateControllerState(module, 5) 					# 3次没有收到心跳消息，则更新状态为未连接

				# 将该控制器从在线控制器列表中移除
				# logE("Error: %s lost connection"%module['name'])
				self.onlineControllers.remove(module)
			else:
				module['expire_times'] += 1

		for module in self.onlineStation:
			if module['expire_times'] == MSG_EXPIRE_TIMES:
				self.UpdateStationState(module, 5) 					# 3次没有收到心跳消息，则更新状态为未连接

				# 将该控制器从在线控制器列表中移除
				logE("Error: %s站车站服务器 lost connection"%module['name'])
				self.onlineStation.remove(module)
			else:
				module['expire_times'] += 1

		# 重新设置定时器
		self.heartbeatTimer = threading.Timer(HEARTBEAT_INTERVAL, self.CheckOnlineStatus)
		self.heartbeatTimer.start()

		return

	# 检查已发送消息队列，超时重发消息
	def CheckSentMsg(self):

		for msg in self.sentMsgList:
			controller = msg.controller
			if msg.SendTimes >= MSG_EXPIRE_TIMES:
				logE("Send %s to %s times exceed MAX times. Msg will be discard. Parameters: %s"%(msg.getMsgTypeStr(), controller['name'], msg.toString()))
				self.sentMsgList.remove(msg)
			else:
				# 重发消息
				msg.SendTimes += 1
				self.SendMsgToController(controller, msg, resendFlag = True)

		# 设置定时器，重发消息
		self.resendMsgTimer = threading.Timer(MSG_RESEND_INTERVAL, self.CheckSentMsg)
		self.resendMsgTimer.start()


	# 获取控制器名称
	def getControllerName(self, controller):
		dbHelper = DbHelper()
		db = dbHelper.getDB("pis.controller")
		cur = dbHelper.getCursor()

		cid = controller['id']

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
		dbHelper.closeDB()

		if not len(rows):
			return "UNKOWN"

		name = rows[0][0]
		return name

	# 根据IP地址获取控制器信息
	def getController(self, ipAddr):
		dbHelper = DbHelper()
		db = dbHelper.getDB("pis.controller")
		result = db.findByCond("id,ip_address,code,install_type,name" , "ip_address='%s'" % ipAddr, "id desc")
		dbHelper.closeDB()

		if not result["rows"]:
			return None

		keys = result["struct"].split(",")
		item = dict(zip(keys, result["rows"][0]))

		item["name"] = self.getControllerName(item)
		
		return item


	# 控制器接收消息处理
	# 中心服务器只要接收到控制器的消息，则认为控制器已经在线，更新其状态为在线
	# 车站服务器只接收到站时间响应消息
	def ControllerMessageProcess(self, data, clientSocket, clientAddr):

		logDD("Receive message from client %s: Hex Data:%s"%(clientAddr[0], ConvertToHex(data)))

		basemsg = CHeartBeatMsg() 										# 使用消息基类的解码功能
		msg = basemsg.decodeMsg(data)

		if msg == None:
			logE("Error: Decode message failed, Data: %s"%ConvertToHex(data))
			return

		msg.isSendMsg = False

		clientIPAddr 	= clientAddr[0]
		clientPort 		= clientAddr[1]
		controller 		= None
		msgType 		= msg.getMsgType()

		if msgType not in PISMessageTypeList:
			logE("Error: Receive an error message %d from client %s"%(msgType, clientIPAddr))
			return

		if msgType != ModbusMsg: 										# 如果不是Modbus消息，则检查是否是合法的控制器
			controller = self.getController(clientIPAddr)
			if controller == None:
				logE("Error: Receive message from a invalid controller: %s, %s"%(clientIPAddr, msg.getMsgTypeStr()) )
				return

		if self.isStation == False:										# 车站服务器不需要检查控制器是否在线
			
			sendController = self.FindOnlineController(controller) 		# 中心服务器需要检查控制器是否已经在列表中，不在则添加进队列
			if sendController == None:
				controller['socket'] 		= clientSocket
				controller['port'] 			= clientPort
				controller['expire_times'] 	= 0
				self.onlineControllers.append(controller)
				
				self.UpdateControllerState(controller, 0)				# 更新控制器状态为在线
		else:
			# 车站服务器，只接收到站时间响应消息
			if msgType != SendStationTimeAck:
				logE("Error: Station Server  receive a wrong message %s from controller %s"%(msg.getMsgTypeStr(), clientIPAddr))
				return

		if msgType not in noShowMsgList: 								# 心跳消息、状态报告、日志消息数量太多，一般不显示
			logI("Receive %s from %s %s"%(msg.getMsgTypeStr(), controller['name'], clientIPAddr))
		else:
			logD("Receive %s from %s %s"%(msg.getMsgTypeStr(), controller['name'], clientIPAddr))

		self.LogMsg(controller, msg) 									# 记录消息日志

		if msgType == HeartBeatMsg: 									# 心跳消息
			self.processHeartbeatMsg(clientSocket, controller, msg)

		elif msgType == SendProgramListAck:
			self.processSendProgramListAck(clientSocket, controller, msg)

		elif msgType == SendNewsAck:
			self.processSendNewsAck(clientSocket, controller, msg)

		# elif msgType == ClearNewsAck:
		# 	self.processClearNewsAck(clientSocket, controller, msg)

		elif msgType == SendEmergenceAck:
			self.processSendEmergenceAck(clientSocket, controller, msg)

		# elif msgType == ClearEmergenceAck:
		# 	self.processClearEmergenceAck(clientSocket, controller, msg)

		elif msgType == SendLineInfoAck:
			self.processSendLineInfoAck(clientSocket, controller, msg)

		elif msgType == SendStationTimeAck:
			self.processSendStationTimeAck(clientSocket, controller, msg)

		elif msgType == SendFormatFileAck:
			self.processSendFormatFileAck(clientSocket, controller, msg)

		elif msgType == SystemRebootAck:
			self.processSystemRebootAck(clientSocket, controller, msg)

		elif msgType == RunStatusMsg:
			self.processRunStatusMsg(clientSocket, controller, msg)

		elif msgType == RunLogMsg:
			self.processRunLogMsg(clientSocket, controller, msg)

		elif msgType == SendUpdateAck:
			self.processSendUpdateAck(clientSocket, controller, msg)

		# 模块初始化消息
		elif msgType == InitialRequestMsg:
			self.processInitialRequestMsg(clientSocket, controller, msg)

		elif msgType == UpdateWorkTimeAck:
			self.processUpdateWorkTimeAck(clientSocket, controller, msg)

		elif msgType == ViewControllerVideoAck:
			self.processSendViewVideoAck(clientSocket, controller, msg)

		elif msgType == SendSafeVideoAck:
			self.processSendSafeVideoAck(clientSocket, controller, msg)

		elif msgType == DownloadCompleteMsg:
			self.processDownloadCompleteMsg(clientSocket, controller, msg)

		else:
			logE("Error: Receive an unsupport message %d from client %s"%(msgType, clientIPAddr))

		return

	# 处理心跳消息，更新状态
	def processHeartbeatMsg(self, clientSocket, controller, msg):

		ackMsg = CHeartBeatAckMsg()
		ackMsg.serialNo = msg.serialNo
		ackMsg.srcAddr 	= self.LocalAddr
		ackMsg.destAddr = msg.srcAddr

		ackMsg.setOperationID(None)
		self.SendMsgToController(controller, ackMsg)

		if self.isStation == False:
			# 查找在线控制器，更新其超时次数为0
			item = self.FindOnlineController(controller)
			if item:
				item['expire_times'] = 0

		return

	# 发送节目单给控制器
	def sendProgramList(self, controllerList, msgInfo, resend = False):
		data 		= msgInfo["data"]
		MsgListID 	= msgInfo["id"]

		for item in controllerList:
			msg = CSendProgramListMsg()
			msg.setParams(data[0])
			msg.setOperationID(MsgListID, msgInfo["operation_id1"], msgInfo["operation_id2"])
			self.SendMsgToController(item, msg)

		return None

	# 接收节目单响应消息
	def processSendProgramListAck(self, clientSocket, controller, msg):
		# 不需要处理该响应消息
		pass	

	# 发送新闻给控制器
	def sendNews(self, controllerList, msgInfo, resend = False):
		news = msgInfo["content"].strip()
		if not news:
			# return self.sendClearNewsMsg(controllerList, msgInfo, resend)
			news = ""

		lifeTime = msgInfo['data'][0]

		MsgListID = msgInfo["id"]
		for item in controllerList:
			msg = CSendNewsMsg()
			msg.setParams(lifeTime, news)
			msg.setOperationID( MsgListID, msgInfo["operation_id1"], msgInfo["operation_id2"])
			self.SendMsgToController(item, msg)

	def processSendNewsAck(self, clientSocket, controller, msg):
		pass

	# # 发送清除新闻消息
	# def sendClearNewsMsg(self, controllerList, msgInfo, resend = False):
	# 	MsgListID= msgInfo["id"]
	# 	msg = CClearNewsMsg()
	# 	msg.setOperationID( MsgListID, msgInfo["operation_id1"], msgInfo["operation_id2"])

	# 	for item in controllerList:
	# 		self.SendMsgToController(item, msg)

	# def processClearNewsAck(self, clientSocket, controller, msg):
	# 	pass

	# 发送紧急消息
	def sendEmergence(self, controllerList, msgInfo, resend = False):
		strEmgc = msgInfo["content"].strip()
		if not strEmgc:
			# return self.sendClearEmergenceMsg(controllerList, msgInfo, resend)
			strEmgc = ""

		mode = msgInfo['data'][0]
		lifeTime = msgInfo['data'][1]

		MsgListID = msgInfo["id"]

		for item in controllerList:
			msg = CSendEmergenceMsg()
			msg.setParams(lifeTime, mode, strEmgc)
			msg.setOperationID(MsgListID)
			self.SendMsgToController(item, msg)

		return

	def processSendEmergenceAck(self, clientSocket, controller, msg):
		pass

	# # 发送清除紧急消息内容消息
	# def sendClearEmergenceMsg(self, controllerList, msgInfo, resend = False):
	# 	MsgListID = msgInfo["id"]
	# 	msg = CClearEmergenceMsg()
	# 	msg.setOperationID(MsgListID)

	# 	for item in controllerList:
	# 		self.SendMsgToController(item, msg)

	# 	return

	# def processClearEmergenceAck(self, clientSocket, controller, msg):
	# 	pass

	# 发送线路首末班时间信息
	def sendLineInfo(self, controllerList, msgInfo, resend = False):
		MsgListID 	= msgInfo["id"]

		msgData = msgInfo['data']
		if len(msgData) == 0:
			raise BaseError(801, "更新运营线路首末班时间信息：缺少参数")

		for item in controllerList:
			msg = CSendLineInfoMsg()
			msg.setParams(msgData[0])
			msg.setOperationID(MsgListID)
			self.SendMsgToController(item, msg, resend)

		return None

	def processSendLineInfoAck(self, clientSocket, controller, msg):
		pass

	# 发送到站信息
	def sendStationTime(self, controllerList, msgInfo, resend = False):
		MsgListID 	= msgInfo["id"]
		stationTime = msgInfo['stationTime']

		msg = CSendStationTimeMsg()
		msg.setParams(stationTime)
		msg.setOperationID(MsgListID)
		
		for item in controllerList:
			self.SendMsgToController(item, msg)

		return

	def processSendStationTimeAck(self, clientSocket, controller, msg):
		pass

	# 发送版式文件
	def sendFormatFile(self, controllerList, msgInfo, resend = False):
		MsgListID 	= msgInfo["id"]
		formatFile 	= msgInfo["data"]
		if len(formatFile) == 0:
			raise BaseError(801, "参数错误：无版式文件名！")

		msg = CSendFormatFileMsg()
		msg.setParams(formatFile[0])
		msg.setOperationID(MsgListID, msgInfo["operation_id1"], 0)

		for item in controllerList:
			self.SendMsgToController(item, msg)

		return

	def processSendFormatFileAck(self, clientSocket, controller, msg):
		pass

	# 发送控制器重启消息
	def sendSystemReboot(self, controllerList, msgInfo, resend = False):
		MsgListID = msgInfo["id"]
		msgData = msgInfo["data"]
		if len(msgData) != 2:
			raise BaseError(801, "参数错误：参数输入个数错误！")

		rebootMode 	= msgData[0]
		delayTime 	= msgData[1]

		for item in controllerList:
			msg = CSystemRebootMsg()
			msg.setParams(delayTime, rebootMode)
			msg.setOperationID(MsgListID, 0, 0)

			self.SendMsgToController(item, msg)

		return

	def processSystemRebootAck(self, clientSocket, controller, msg):
		pass

	# 接收运行状态消息
	def processRunStatusMsg(self, clientSocket, controller, msg):
		
		dbHelper = DbHelper()

		currentDate = datetime.datetime.now().strftime('%Y-%m-%d')
		currentTime = datetime.datetime.now().strftime('%H:%M:%S')
		fullTime = currentDate + " " + currentTime
		data = {
			"create_time" 	: fullTime,
			"controller" 	: controller["id"],
			"ip_address" 	: msg.srcAddr,
			"cpu" 			: msg.cpu,
			"memory" 		: msg.memory,
			"harddisk" 		: msg.harddisk,
			"status" 		: msg.status,
			'date' 			: currentDate,
			'time' 			: currentTime
		}

		db = dbHelper.getDB("pis.controller_status")
		db.add(data)

		# 发送响应消息
		ackMsg = CRunStatusAck()
		ackMsg.serialNo = msg.serialNo
		self.SendMsgToController(controller, ackMsg)

		return

	# 接收运行日志消息
	def processRunLogMsg(self, clientSocket, controller, msg):
		dbHelper = DbHelper()

		currentDate = msg.logDate
		data = {
			"controller" 	: controller["id"],
			"date" 			: msg.logDate,
			"time" 			: msg.logTime,
			"log_id" 		: msg.logID,
			"level" 		: msg.logLevel,
			"type" 			: msg.logType,
			"title" 		: msg.logTitle,
			"content" 		: msg.logDesc,
			"create_time" 	: datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
		}

		db = dbHelper.getDB("pis.controller_log")
		db.deleteByCond("log_id=%s and date='%s'" % (msg.logID, currentDate))
		db.add(data)

		# 发送响应消息
		ackMsg = CRunLogAck()
		ackMsg.serialNo = msg.serialNo
		self.SendMsgToController(controller, ackMsg)

		return

	# 发送更新系统消息
	def sendUpdate(self, controllerList, msgInfo, resend = False):
		MsgListID 	= msgInfo["id"]
		fileInfo 	= msgInfo["data"]
		if len(fileInfo) == 0:
			raise BaseError(801, "发送版本更新错误：无版本文件名称！")

		for item in controllerList:
			msg = CSendUpdateMsg()
			msg.setParams(fileInfo[0])
			msg.setOperationID( MsgListID, 0, 0)

			self.SendMsgToController(item, msg)

		return

	def processSendUpdateAck(self, clientSocket, controller, msg):
		pass

	# 根据控制器IP地址获取控制器及所属线路基本信息
	# 包括：
	# 	1. 控制器类型
	# 	2. 控制器所在车站或列车的中英文名称
	# 	3. 控制器的开机休眠时间
	# 	4. 控制器所属的车站本站上下行首末班时间，如果是车载控制器则为空
	# 	5. 控制器归属运营线路的支线信息
	def GetLineInfo(self, controller):
		dbHelper = DbHelper()
		cursor = dbHelper.getCursor()

		ipAddr = controller['ip_address']
		data = {}

		sql = """
			select ct.line_id, ct.install_type, ct.open_time, ct.close_time, sl.up_start_time, sl.up_end_time, sl.down_start_time, sl.down_end_time, st.name, st.name_en, t.name, t.name_en 
			from pis.controller ct
			left join pis.operation_line ol on ol.id = ct.line_id
			left join pis.station st on st.id = ct.install_id and ct.install_type in ('2', '3', '4')
			left join pis.station_line sl on sl.line_id = ct.line_id and sl.station_id = ct.install_id and ct.install_type in ('2', '3', '4')
			left join pis.train t on t.id = ct.install_id and ct.install_type in ('0', '1')
			where ct.ip_address = '%s'
			""" %ipAddr

		cursor.execute(sql)
		result = cursor.fetchall()
		if len(result) != 1:
			return {}

		key = ['line_id', 'install_type',  'open_time', 'close_time', 'up_start_time', 'up_end_time', 'down_start_time', 'down_end_time', 'station_name', 'station_name_en', 'train_name', 'train_name_en']
		controllerInfo = dict(zip(key, result[0]))		

		# 获取支线信息
		sql  = " select sli.id, sli.line_id, sli.sub_line_name, sli.sub_line_name_en, sli.up_start_station, stus.name, stus.name_en, sli.up_end_station, stue.name, stue.name_en, "
		sql += " sli.down_start_station, stds.name, stds.name_en, sli.down_end_station, stde.name, stde.name_en, sli.up_start_time, sli.up_end_time, "
		sql += " sli.down_start_time, sli.down_end_time from pis.sub_line_info sli"
		sql += " inner join pis.station stus on stus.id = sli.up_start_station"
		sql += " inner join pis.station stue on stue.id = sli.up_end_station"
		sql += " inner join pis.station stds on stds.id = sli.down_start_station"
		sql += " inner join pis.station stde on stde.id = sli.down_end_station"
		
		sql += " where sli.line_id = %d"%controllerInfo['line_id']

		cursor.execute(sql)
		rows = cursor.fetchall()

		key = ['id', 'line_id', 'sub_line_name', 'sub_line_name_en', 'up_start_station', 'up_start_station_name', 'up_start_station_name_en', 'up_end_station', 'up_end_station_name',
		 'up_end_station_name_en', 'down_start_station', 'down_start_station_name', 'down_start_station_name_en', 'down_end_station', 'down_end_station_name', 'down_end_station_name_en',
		 'up_start_time', 'up_end_time', 'down_start_time', 'down_end_time']

		subLineList = []
		for item in rows:
			subLineInfo = dict(zip(key, item))		
			subLineList.append(subLineInfo)

		controllerInfo['subLineList'] = subLineList
		return controllerInfo

	# 检查是否有接收失败的消息，若有则重新发送给控制器
	def ResendFailedMsgToController(self, controller):
		dbHelper = DbHelper()

		db = dbHelper.getDB("pis.message_log")
		cur = dbHelper.getCursor()

		sql  = "select ml.id, ml.msg_list_id, ml.message_type, ml.operation_id1, ml.operation_id2, ml.send_status from pis.message_log ml where ml.id in (" 
		sql += "select max(log.id) id from pis.message_log log "
		
		# 指定控制器的发送失败的消息
		sql += " where log.receiver_id = %d"%controller['id']

		# 发送时间在指定天数内
		nowTime 	= datetime.datetime.now()
		delta 		= datetime.timedelta(days = PisConfig['failedDaysMsg'])
		checkDay 	= nowTime - delta
		sql += " and log.send_date >= '%s'"%checkDay.strftime('%Y-%m-%d')

		# 查找指定类型的消息
		sql_where = " and ("
		for i, msgType in enumerate(failedMsgForResendList):
			sql_where += "log.message_type = '%d' "%msgType
			if i < len(failedMsgForResendList) - 1:
				sql_where += " or "
		sql_where += ")"
		sql += sql_where
		sql += " group by log.message_type"

		sql += ")"

		cur.execute(sql)
		rows = cur.fetchall()

		key = ['id', "msg_list_id", 'message_type', 'operation_id1', 'operation_id2', 'send_status']
		msgList = []
		for i, item in enumerate(rows):
			msg = dict(zip(key, item))
			msg['message_type'] = int(msg['message_type'])
			msgList.append(msg)

		for msg in msgList:
			if msg['send_status'] == '1':
				# 如果发送成功则忽略
				continue

			if msg['message_type'] == SendProgramListMsg:
				self.ResendSendProgramListMsg(controller, msg)
			elif msg['message_type'] == SendSafeVideoMsg:
				self.ResendSendSafeVideoMsg(controller, msg)
			elif msg['message_type'] == SendNewsMsg:
				self.ResendSendNewsMsg(controller, msg)
			elif msg['message_type'] == SendFormatFileMsg:
				self.ResendSendFormatFileMsg(controller, msg)

		return

	# 控制器初始化后，重新发送最新的发送失败的节目单文件
	def ResendSendProgramListMsg(self, controller, msgInfo):

		# 根据节目单的记录ID生成节目单文件名，节目单ID为operation_id1
		ProgramListID = msgInfo['operation_id1']
		file_name = "program_list_%d.txt"%ProgramListID
		MsgListID = msgInfo['msg_list_id']

		msg = CSendProgramListMsg()
		msg.setParams(file_name)
		msg.setOperationID( MsgListID, ProgramListID, 0)
		msg.MsgLogID = msgInfo['id']
		self.SendMsgToController(controller, msg, resendFlag = True)

	# 控制器初始化后，重新发送最新的发送失败的安全垫片文件
	def ResendSendSafeVideoMsg(self, controller, msgInfo):
		# 安全垫片路径在message_log的content字段中
		dbHelper = DbHelper()
		db  = dbHelper.getDB("pis.message_log")
		cur = dbHelper.getCursor()

		sql = "select ml.text_content from pis.message_log ml where ml.id = %d"%msgInfo['id']
		cur.execute(sql)
		rows = cur.fetchall()

		dbHelper.closeDB()

		if len(rows) == 0:
			logE("Error: Can not find program log info, ID: %d"%msgInfo['id'])
			return

		file_name = rows[0][0]

		MsgListID = msgInfo['msg_list_id']

		msg = CSendSafeVideoMsg()
		msg.setParams(file_name)
		msg.setOperationID(MsgListID)
		msg.MsgLogID = msgInfo['id']

		self.SendMsgToController(controller, msg, resendFlag = True)


	# 控制器初始化后，重新发送最新的发送失败的新闻内容
	def ResendSendNewsMsg(self, controller, msgInfo):
		# 新闻内容在message_list的content字段中
		dbHelper = DbHelper()
		db  = dbHelper.getDB("pis.message_list")
		cur = dbHelper.getCursor()

		sql = "select ml.content from pis.message_list ml where ml.id = %d"%msgInfo['msg_list_id']
		cur.execute(sql)
		rows = cur.fetchall()

		dbHelper.closeDB()

		if len(rows) == 0:
			logE("Error: Can not find program list info, ID: %d"%msgInfo['msg_list_id'])
			return

		newsContent = rows[0][0].strip()
		if not newsContent:
			# 没有新闻内容，则不需要重新发送消息
			return

		# logI("Resend news content: %s"%newsContent)

		MsgListID = msgInfo['msg_list_id']

		msg = CSendNewsMsg()
		msg.setParams(0, newsContent)
		msg.setOperationID(MsgListID)
		msg.MsgLogID = msgInfo['id']

		self.SendMsgToController(controller, msg, resendFlag = True)

	# 控制器初始化后，重新发送最新的发送失败的版式文件
	def ResendSendFormatFileMsg(self, controller, msgInfo):
		# 版式文件ID在message_list的operation_id1中
		dbHelper = DbHelper()
		db  = dbHelper.getDB("pis.message_list")
		cur = dbHelper.getCursor()

		sql = "select ml.operation_id1 from pis.message_list ml where ml.id=%d"%msgInfo['msg_list_id']
		cur.execute(sql)
		rows = cur.fetchall()

		if len(rows) == 0:
			logE("Error: Can not find format file info, ID: %d"%msgInfo['msg_list_id'])
			return

		fileID = rows[0][0]

		sql  = "select ff.file_path from pis.format_file ff where ff.id = %d "%fileID

		cur.execute(sql)
		rows = cur.fetchall()

		dbHelper.closeDB()

		if len(rows) == 0:
			logE("Error: Can not find format file info, ID: %d"%msgInfo['msg_list_id'])
			return

		MsgListID = msgInfo['msg_list_id']
		file = os.path.basename(rows[0][0])

		msg = CSendFormatFileMsg()
		msg.setParams(file)
		msg.setOperationID(MsgListID, fileID, 0)
		msg.MsgLogID = msgInfo['id']

		self.SendMsgToController(controller, msg, resendFlag = True)


	# 处理模块初始化消息
	def processInitialRequestMsg(self, clientSocket, controller, msg):

		# 发送响应消息给控制器

		ackMsg = CInitialRequestAck()
		ackMsg.serialNo = msg.serialNo
		ackMsg.setOperationID(None)
		ackMsg.setParams( controller['code'], self.GetLineInfo(controller))

		# logI("Controller Type: %s"%ackMsg.controller_type)

		self.SendMsgToController(controller, ackMsg)

		# 检查是否有接收失败的消息，若有则重新发送给控制器
		self.ResendFailedMsgToController(controller)
		return

	# 发送控制器工作时间消息
	def sendWorkTime(self, controllerList, msgInfo, resend = False):

		MsgListID 	= msgInfo["id"]
		msgData 	= msgInfo['data']
		if len(msgData) < 2:
			raise BaseError(801, "更新控制器工作时间消息，缺少时间参数")

		openTime 	= msgData[0]
		closeTime 	= msgData[1]

		for item in controllerList:
			msg = CUpdateWorkTimeMsg()
			msg.setParams(openTime, closeTime)
			msg.setOperationID(MsgListID)

			self.SendMsgToController(item, msg)

		return

	def processUpdateWorkTimeAck(self, clientSocket, controller, msg):
		pass

	# 发送查看控制器视频消息
	def sendViewControllerVideo(self, controllerList, msgInfo, resend = False):
		MsgListID = msgInfo["id"]

		for item in controllerList:
			msg = CViewControllerVideoMsg()
			msg.setParams( msgInfo['data'][0])
			msg.setOperationID(MsgListID)

			self.SendMsgToController(item, msg)

		return

	def processSendViewVideoAck(self, clientSocket, controller, msg):
		pass

	# 发送安全垫片消息
	def sendSafeVideoFile(self, controllerList, msgInfo, resend = False):
		MsgListID = msgInfo["id"]

		fileInfo = msgInfo['data']
		if len(fileInfo) == 0:
			raise BaseError(801, "发送安全垫片错误：无视频文件名！")

		filePath = fileInfo[0]['file_path']
		fileName = os.path.basename(filePath)

		for item in controllerList:
			msg = CSendSafeVideoMsg()
			msg.setParams(fileName)
			msg.setOperationID(MsgListID)

			self.SendMsgToController(item, msg)

		return

	def processSendSafeVideoAck(self, clientSocket, controller, msg):
		pass

	# 接收下载完成消息
	def processDownloadCompleteMsg(self, clientSocket, controller, msg):
		# 发送响应消息
		ackMsg = CDownloadCompleteAck()
		ackMsg.serialNo = msg.serialNo
		self.SendMsgToController(controller, ackMsg)

		requestMsgType = msg.requestMsgType
		if requestMsgType not in msgForDownloadList:
			logE("Error: Receive DownloadCompleteMsg but requestMsgType error: %d"%requestMsgType)
			return

		# 根据控制器IP地址和消息类型进行查找
		for item in self.downloadMsgList:
			if item.msgType != requestMsgType and item.destAddr != controller['ip_address']:
				continue

			# 从队列中移除
			self.downloadMsgList.remove(item)
			MsgLogID = item.MsgLogID
			MsgListID = item.MsgListID

			CurrentTime = datetime.datetime.now()
			
			# 更新mesage_list表记录状态
			data = {
				"send_status" 	: 3, 									# 消息接收成功
			}
			dbHelper = DbHelper()
			db = dbHelper.getDB("pis.message_list")
			db.save(data, MsgListID)

			# 更新message_log记录状态
			data = {
				"send_status" 	: 1, 									# 消息接收成功
				"receive_date" 	: CurrentTime.strftime('%Y-%m-%d'),
				"receive_time" 	: CurrentTime.strftime('%H:%M:%S'),
			}
			
			db = dbHelper.getDB("pis.message_log")
			db.save(data, MsgLogID)
			return

	# 查找控制器是否在线
	def FindOnlineController(self, controller):
		for item in self.onlineControllers:
			if controller['id'] == item['id']:
				return item

		return None

	# 记录消息的收发日志
	def LogMsg(self, controller, msg, resendFlag = False):

		# 心跳消息及心跳响应消息不需要记录到消息日志
		if msg.msgType == HeartBeatMsg or msg.msgType == HeartBeatAck:
			return

		dbHelper = DbHelper()

		CurrentTime = datetime.datetime.now()
		cdate = CurrentTime.strftime('%Y-%m-%d')
		ctime = CurrentTime.strftime('%H:%M:%S')

		if msg.isAckMsg == False:
			# 发送或接收请求消息

			if msg.isSendMsg == False:
				# 从控制器接收到的请求消息

				# 先在message_list表中新增一条记录
				data = {
					'create_time' 	: cdate + " " + ctime,
					'create_id' 	: 0,
					'message_type' 	: msg.msgType,
					'operation_id1' : None,
					'operation_id2' : None,
					'sender_id' 	: controller["id"],
					'sender_name' 	: controller["name"],
					'send_date'		: cdate,
					'send_time' 	: ctime,
					'send_status' 	: 3,  								# 下发成功，表示服务器已经接收该消息
					'content' 		: msg.toString(),
				}

				db = dbHelper.getDB('pis.message_list')
				msg.MsgListID = db.add(data)

				senderInfo 	= controller
				receiveInfo = self.LocalInfo

				data = {
					"message_sn" 	: self.getMsgSn(dbHelper.getCursor(), msg.MsgListID, controller["id"]),
					"send_status" 	: 3, 								# 3: 发送成功，表示服务器已经接收该消息					
				}

			else:
				# 发送给控制器的请求消息
				
				# 先在message_list表中新增一条记录
				data = {
					'create_time' 	: cdate + " " + ctime,
					'create_id' 	: 0,
					'message_type' 	: msg.msgType,
					'operation_id1' : msg.operation_id1,
					'operation_id2' : msg.operation_id2,
					'sender_id' 	: self.LocalInfo["id"],
					'sender_name' 	: self.LocalInfo["name"],
					'send_date'		: cdate,
					'send_time' 	: ctime,
					'send_status' 	: 2,  								# 待接收
					'content' 		: msg.toString(),
				}

				db = dbHelper.getDB('pis.message_list')
				msg.MsgListID = db.add(data)

				senderInfo 	= self.LocalInfo
				receiveInfo = controller
				data = {
					"message_sn" 	: 1,
					"send_status" 	: 2, 								# 2: 待接收
				}

			# 在message_log表中新增一条记录

			packetStr = " ".join(["%02x" % d for d in msg.msgBuffer])
			data2 = {
				"sender_id"   	: senderInfo["id"],
				"sender_type" 	: senderInfo["install_type"],
				"sender_name" 	: senderInfo["name"],
				"sender_code" 	: senderInfo["code"],
				"sender_addr" 	: senderInfo["ip_address"],

				"receiver_id"   : receiveInfo["id"],
				"receiver_type" : receiveInfo["install_type"],
				"receiver_name" : receiveInfo["name"],
				"receiver_code" : receiveInfo["code"],
				"receiver_addr" : receiveInfo["ip_address"],

				"message_type" 	: msg.msgType,
				"msg_list_id" 	: msg.MsgListID,
				"operation_id1" : msg.OperationID1,
				"operation_id2" : msg.OperationID2,
				"send_date" 	: cdate,
				"send_time" 	: ctime,
				"receive_date" 	: None,
				"receive_time" 	: None,
				"content_type" 	: "0",
				"bin_content" 	: packetStr,
				"text_content" 	: msg.toString(),
				"create_time" 	: cdate + " " + ctime,
			}

			data.update(data2)

			db = dbHelper.getDB("pis.message_log")
			# 如果是重发消息，则更新原记录
			if resendFlag:
				db.save(data, msg.MsgLogID)
			else:
				msg.MsgLogID = db.add(data)

			# 将发送给控制器的请求消息添加到队列中
			# logI("isSendMsg: %s, SendTimes: %d"%( "True" if msg.isSendMsg else "False", msg.SendTimes ))
			if msg.isSendMsg and msg.SendTimes == 0:
				self.AddSentMsg(controller, msg)

		else:
			# 发送或接收响应消息
			if msg.isSendMsg == True:
				# 发送给控制器的响应消息不需要记录
				return

			# 从控制器接收到响应消息，需要更新message_log中相应的请求消息发送状态
			sentMsg = self.FindSentMsg(controller, msg.getRequestMsgType())
			if sentMsg == None:
				logE("Error: Receive a %s from controller %s:%s but not find request msg"%(msg.getMsgTypeStr(), controller['name'], controller['ip_address']))
				return

			# 将请求消息从列表中删除
			self.sentMsgList.remove(sentMsg)

			# 如果请求消息需要等待下载完成，将请求消息添加到等待队列中
			if sentMsg.msgType in msgForDownloadList:
				self.downloadMsgList.append(sentMsg)
				status = 3 												# 状态：正在下载 
			else:
				status = 1

			data = {
				"send_status" 	: status, 								# 消息接收成功
				"receive_date" 	: CurrentTime.strftime('%Y-%m-%d'),
				"receive_time" 	: CurrentTime.strftime('%H:%M:%S'),
			}

			db = dbHelper.getDB("pis.message_log")
			db.save(data, sentMsg.MsgLogID)

			# 更新message_list的发送状态
			MsgListID = sentMsg.MsgListID
			if MsgListID == 0:
				logE("Error: ")
				return

			# 查找消息发送的消息总数、成功个数和失败个数
			cur = dbHelper.getCursor()
			sql = """ select count(id) as all_count, sum(case send_status when '1' then 1 else 0 end) as success_count, sum(case send_status when '0' then 1 else 0 end) as fail_count
				from pis.message_log where id in (select a.id from (select max(id) as id from pis.message_log where msg_list_id=%s group by receiver_addr) a)
				""" % MsgListID

			cur.execute(sql)
			result 		= cur.fetchone()

			nTotal 		= result[0]
			nSuccess 	= result[1]
			nFailed 	= result[2]

			if nTotal == nSuccess:
				status = "3" 											# 所有控制器接收成功
			else:
				status = "4" 											# 部分控制器接收成功

			db = dbHelper.getDB("pis.message_list")
			data = { "send_status" : status }
			db.save(data, MsgListID)

		dbHelper.closeDB()

	# 将发送给控制器的请求消息添加到队列中
	def AddSentMsg(self, controller, msg):
		# 响应消息及接收到的请求消息不需要保留到队列中
		if msg.isAckMsg or msg.isSendMsg == False:
			return

		sentMsg = self.FindSentMsg(controller, msg.msgType)
		if sentMsg != None:
			# 同一类消息只保留最新的一次消息
			# logI("消息类型：%s"%sentMsg.getMsgTypeStr())
			self.sentMsgList.remove(sentMsg)

		msg.controller 	= controller
		self.sentMsgList.append(msg)
		return

	# 查找已发送的消息
	def FindSentMsg(self, controller, msgType):
		for item in self.sentMsgList:
			if item.destAddr == controller['ip_address'] and item.msgType == msgType:
				return item

		return None

	def getMsgSn(self, cur, MsgListID, receiver_id):

		if not MsgListID:
			return 1

		sql = "update pis.message_log set message_sn=0 where msg_list_id=%s and receiver_id=%s" % (MsgListID, receiver_id)
		cur.execute(sql)
		return 1

	# 从中心服务器页面发送消息给控制器接口
	def sendMessage(self, controllerList, msgInfo, resend = False):

		for item in controllerList:
			item['name'] = self.getControllerName(item)

		msgType = msgInfo["message_type"]
		if msgType == SendProgramListMsg:
			self.sendProgramList(controllerList, msgInfo)

		elif msgType == SendNewsMsg:
			self.sendNews(controllerList, msgInfo)

		elif msgType == SendLineInfoMsg:
			self.sendLineInfo(controllerList, msgInfo)

		elif msgType == SendUpdateMsg:
			self.sendUpdate(controllerList, msgInfo)

		elif msgType == SystemRebootMsg:
			self.sendSystemReboot(controllerList, msgInfo)

		elif msgType == SendFormatFileMsg:
			self.sendFormatFile(controllerList, msgInfo)

		elif msgType == SendEmergenceMsg:
			self.sendEmergence(controllerList, msgInfo)

		elif msgType == UpdateWorkTimeMsg:
			self.sendWorkTime(controllerList, msgInfo)

		elif msgType == ViewControllerVideoMsg:
			self.sendViewControllerVideo(controllerList, msgInfo)

		elif msgType == SendSafeVideoMsg:
			self.sendSafeVideoFile(controllerList, msgInfo)

		else:
			logE("sendMessage, error msgtype: %d"%msgType)

		return None

	# 发送消息给控制器
	def SendMsgToController(self, controller, msg, resendFlag = False):

		msg.isSendMsg 	= True
		msg.srcAddr 	= self.LocalAddr
		msg.destAddr 	= controller['ip_address']

		msg.srcPort 	= PisConfig['UDPServerPort']
		msg.destPort 	= PisConfig['ControllerUDPPort']

		msgType = msg.msgType
		if resendFlag:
			resendStr = "Resend"
		else:
			resendStr = "Send"

		# 不管是否发送给了控制器，消息都要进行记录
		basemsg 		= CHeartBeatMsg() 								# 使用消息基类的编码功能
		msgData 		= basemsg.encodeMsg(msg)

		# 发送的消息记录到消息日志
		self.LogMsg(controller, msg, resendFlag)

		# 发送消息给控制器，如果控制器没有连接，则不发送消息
		recController = self.FindOnlineController(controller)
		if recController == None:
			if msgType not in noShowMsgList:
				logI("%s %s to %s(%s), Parameters: %s"%(resendStr, msg.getMsgTypeStr(), controller['name'], controller['ip_address'], msg.toString()))
			else:
				logD("%s %s to %s"%(resendStr, msg.getMsgTypeStr(), controller['name']))

			return 

		clientSocket 	= recController['socket']
		clientAddr 		= recController['ip_address']
		clientPort 		= recController['port']

		if msgType not in noShowMsgList:
			logI("%s %s to %s(%s:%d), Parameters: %s"%(resendStr, msg.getMsgTypeStr(), recController['name'], clientAddr, clientPort, msg.toString()))
			logDD("Hex Data: %s"%ConvertToHex(msgData))
		else:
			logD("%s %s to %s"%(resendStr, msg.getMsgTypeStr(), recController['name']))

		clientSocket.sendMessage( msgData, (clientAddr, clientPort) )


# 根据配置文件中配置的模块列表，将业务模块的处理实例加入到数组中
if "PIS" in SystemModule:
	info = {
		"name" 	: "rsu",
		"server": CPISMsgServer(),
	}
	SystemModuleList.append( info )
	