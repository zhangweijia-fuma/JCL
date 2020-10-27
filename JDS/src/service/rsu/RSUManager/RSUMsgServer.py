# coding:utf-8

# ------------------------------------------------------------------------------
# JCCL - Component :: RSU Message Server
# Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
# Author: Zhang Weijia
# ------------------------------------------------------------------------------
# Name:    RSUMsgServer.py
# Purpose: RSU Message Processs
#  *----------------------------------------------------------------------------

# RSU 消息处理

import os, sys, time, threading, datetime

import urllib.request
from socket import *

from libs.utils.debug import *
from libs.utils.utils import *
from libs.db.dbHelper import *

from libs.JCL.MsgSocket import *
from libs.JCL.JCLCommon import *
from libs.JCL.JCLScheduler import *
from libs.JCL.JCLDBTools import *

from rsu.RSUManager.RSUConfig import *
from rsu.RSUManager.RSUMessage import *

def RSUDatabaseCleanTask():

	logI("RSUDatabaseCleanTask Starting ... ")

	# 删除设备状态数据
	
	now 	= datetime.datetime.now()
	days 	= config.BackupAfterDays
	btime 	= now - datetime.timedelta(days=days)
	btimeStr= btime.strftime("%Y-%m-%d %H:%M:%S")

	# 设置删除条件

	system_user_id = GetSystemUserID("rsu")
	sql = """
		delete from public.device_status 
		where device_id in (select device_id from rsu.rsu_list where system_user_id = %d ) and create_time < '%s' and system_user_id = %d 
	"""%(system_user_id, btimeStr, system_user_id)

	dbh = DbHelper(auto=False)
	try :
		db = dbh.openDB(table)
		cur = dbh.getCursor()
		cur.execute(sql)
	except Exception as e:
		onException(e)
	dbh.closeDB()

	return


class CRSUMsgServer(threading.Thread):
	def __init__(self):
		threading.Thread.__init__(self)

		self.UDPServer 			= None 									# UDP Server socket，控制器 UDP 通信

		self.onlineRSU 			= [] 									# 在线RSU列表
		self.heartbeatTimer 	= None 									# 心跳消息定时器
		self.system_user_id 	= 0

		self.nModuleType 		= MODULE_RSU_MANAGER
		self.strModuleCode 		= "RSU_MANAGER_001"

		self.LocalAddr 			= getLocalIPAddr()

		return

	def __del__(self):
		self.close()

		return

	# 启动
	def run(self):

		# 是否配置了业务
		if "rsu" not in config.SystemModule:
			logE("Error: System not config RSU module")
			return

		self.system_user_id = GetSystemUserID("rsu")
		if self.system_user_id == 0:
			return

		# 重置所有设备未未连接状态
		self.ResetDeviceStatus()

		# 设置定时器，定期检查心跳消息接收状态
		self.heartbeatTimer = threading.Timer(HEARTBEAT_INTERVAL, self.CheckOnlineStatus)
		self.heartbeatTimer.start()

		# 启动UDP服务端，开始准备接收消息
		self.UDPServer = CSocketServer( SOCK_DGRAM, RSU_SERVER_PORT, self.MessageProcess )
		self.UDPServer.start()

		# 创建定时任务，定时清理数据库数据
		# 每周日凌晨2点启动任务
		CreateScheduleTask(RSUDatabaseCleanTask, second="0", minute = "0", hour="2", day_of_week="0")

		logI("RSU Manager Starting...")

		# 进入线程主循环，等待发送或接收消息
		while (1):
			time.sleep(1)

	def close(self):
		if self.UDPServer:
			self.UDPServer.close()

		if self.heartbeatTimer:
			self.heartbeatTimer.cancel()

		return

	# 重置所有设备未为连接状态
	def ResetDeviceStatus(self):

		dbHelper = DbHelper()
		cursor = dbHelper.getCursor()
		sql = """
			update public.device_list set device_status = '5' 
			where system_user_id = %d and id in 
			( select rl.device_id from rsu.rsu_list rl where rl.system_user_id = %d )
		"""%(self.system_user_id, self.system_user_id)

		cursor.execute(sql)
		dbHelper.closeDB()

		logI("Reset all RSU Device Status to offline.")
		return


	# 更新RSU状态， 1：Online， 5： Offline
	def UpdateDeviceState(self, rsuInfo, status):
		# 更新状态
		now = datetime.datetime.now()
		data = { 
			'device_status' : status, 									# 状态：0；Online，5：Offline 
			'status_update_time' : now.strftime('%Y-%m-%d %H:%M:%S'),
		}

		dbHelper = DbHelper()
		db = dbHelper.getDB("public.device_list")
		db.save(data, rsuInfo['device_id'])

		cursor = dbHelper.getCursor()
		sql = "select cv.name from system.code_value cv where cv.type_code = 'DEVICE_STATUS_TYPE' and cv.code = '%s'"%status

		cursor.execute(sql)
		rows = cursor.fetchall()
		dbHelper.closeDB()

		if len(rows) == 0:
			logE("Error: Can not find device status: %s"%staus)
			return

		# logI("Update %s %s status: %s"%(rsuInfo['device_name'], rsuInfo['device_code'],  rows[0][0]))

		return

	# 定时检查RSU状态，以更新连接状态
	def CheckOnlineStatus(self):

		for rsuInfo in self.onlineRSU:
			if rsuInfo['expire_times'] == MSG_EXPIRE_TIMES:
				self.UpdateDeviceState(rsuInfo, '5') 					# 3次没有收到心跳消息，则更新状态为未连接

				# 将该设备从在线列表中移除
				logE("Error: %s offline"%rsuInfo['device_code'])
				self.onlineRSU.remove(rsuInfo)
			else:
				rsuInfo['expire_times'] += 1

		# 重新设置定时器
		self.heartbeatTimer = threading.Timer(HEARTBEAT_INTERVAL, self.CheckOnlineStatus)
		self.heartbeatTimer.start()

		return

	# 根据IP地址查找RSU设备信息
	def FindDeviceByAddr(self, strAddr):

		dbHelper = DbHelper()
		cursor = dbHelper.getCursor()
		sql = """
			select rl.id, rl.device_id, rl.ip_address1, rl.ip_address2, rl.longtitude, rl.latitude,
			rl.project_id, rl.install_location, rl.map_location, rl.install_date,
			dl.device_code, dl.device_name 
			from rsu.rsu_list rl
			inner join public.device_list dl on dl.id = rl.device_id
			where (rl.ip_address1 = '%s' or rl.ip_address2 = '%s') and rl.system_user_id = %d 
		"""%(strAddr, strAddr, self.system_user_id)

		cursor.execute(sql)
		rows = cursor.fetchall()
		if len(rows) != 1:
			return None

		key  = ['id', 'device_id', 'ip_address1', 'ip_address2', 'longtitude', 'latitude',
			'project_id', 'install_location', 'map_location', 'install_date', 
			'device_code', 'device_name']

		rsuInfo = dict(zip(key, rows[0]))

		dbHelper.closeDB()

		return rsuInfo

	# ##################################################################
	# 消息处理部分

	# 接收消息处理
	# 只要接收到控制器的消息，则认为设备已经在线，更新其状态为在线
	def MessageProcess(self, data, clientSocket, clientAddr):

		clientIPAddr 	= clientAddr[0]
		clientPort 		= clientAddr[1]

		logD("Receive message from client %s:%d "%(clientIPAddr, clientPort))
		logD( ConvertToHex(data) )


		# 根据IP地址查找设备是否为已登记设备
		rsuInfo = self.FindDeviceByAddr(clientIPAddr)
		if rsuInfo == None:
			logE("Error: Can not find RSU infrormation by address: %s"%clientIPAddr)
			return

		rsuInfo['ip_address'] 	= clientIPAddr
		rsuInfo['port'] 		= clientPort
		rsuInfo['expire_times'] = 0

		basemsg = CHeartBeatMsg() 										# 使用消息基类的解码功能
		msg = basemsg.decodeMsg(data)

		if msg == None:
			logE("Error: Decode message failed, Data: %s"%ConvertToHex(data))
			return

		msgType = msg.getMsgType()

		if msgType not in RSUMessageTypeList:
			logE("Error: Receive an error message %s from client %s"%(msg.toString(), clientIPAddr))
			return

		recRSUInfo = self.FindOnlineDevice(rsuInfo)
		if recRSUInfo == None: 						# 需要检查设备是否已经在列表中，不在则添加进队列
			self.onlineRSU.append(rsuInfo) 								# 第一次接收到RSU的消息
			logI("%s online"%rsuInfo['device_code'])
			# 更新设备状态
			self.UpdateDeviceState(rsuInfo, '1')
		else:
			rsuInfo = recRSUInfo

		if msgType not in noShowMsgList: 								# 心跳消息、状态报告、日志消息数量太多，一般不显示
			logI("Receive %s from %s %s"%(msg.toString(), rsuInfo['device_code'], clientIPAddr))
		else:
			logD("Receive %s from %s %s"%(msg.toString(), rsuInfo['device_code'], clientIPAddr))

		if msgType == HeartBeatMsg: 									# 心跳消息
			self.processHeartbeatMsg(rsuInfo, msg)

		elif msgType == RSU_MSG_InitialRequest:
			self.processInitialRequestMsg(rsuInfo, msg)

		elif msgType == RSU_MSG_UpdateSoftwareAck:
			self.processUpdateSoftwareAckMsg(rsuInfo, msg)

		elif msgType == RSU_MSG_RebootAck:
			self.processRebootAckMsg(rsuInfo, msg)

		elif msgType == RSU_MSG_ReportStatus:
			self.processReportStatusMsg(rsuInfo, msg)

		# elif msgType == DownloadCompleteMsg:
		# 	self.processDownloadCompleteMsg(rsuInfo, msg)

		else:
			logE("Error: Receive an unsupport message %d from client %s"%(msgType, clientIPAddr))

		return

	# 处理心跳消息，更新状态
	def processHeartbeatMsg(self, rsuInfo, msg):

		rsuInfo['expire_times'] = 0

		ackMsg 			= CHeartBeatAckMsg()
		ackMsg.serialNo = msg.serialNo

		self.SendMsgToDevice(rsuInfo, ackMsg)

		return


	# 处理模块初始化消息
	# 消息中的经纬度值，是实际值的1000000倍，需要进行转换
	def processInitialRequestMsg(self, rsuInfo, msg):

		# logI("processInitialRequestMsg")

		# 更新其经纬度信息
		data = {
			'id' 		: rsuInfo['id'],
			'longtitude': msg.longtitude / RSU_LNG_LAT_FACTOR,
			'latitude' 	: msg.latitude / RSU_LNG_LAT_FACTOR,
 		}

		data['map_location'] = getAddrByLngLat(data['longtitude'], data['latitude'])
		dbHelper = DbHelper()
		db = dbHelper.getDB("rsu.rsu_list")
		db.save(data, data['id'])
		dbHelper.closeDB()

		# logI("update longtitude latitude")

		# 发送响应消息给控制器

		ackMsg = CRSUInitialRequestAckMsg()
		ackMsg.serialNo = msg.serialNo

		# logI("send ack msg")
		self.SendMsgToDevice(rsuInfo, ackMsg)
		# logI("ack msg sent")
		
		return


	# 发送更新系统消息
	def SendUpdateMsgToDevice(self, deviceList, msgInfo, resend = False):
		
		file_name 	= msgInfo["file_name"]
		file_path 	= msgInfo["file_path"]
		if file_name == "" or file_name == None:
			raise BaseError(801, "发送版本更新错误：无版本文件名称！")

		for item in deviceList:
			msg = CRSUUpdateSoftwareMsg()
			msg.setParams(msgInfo['file_name'], msgInfo['updateMode'])

			self.SendMsgToDevice(item, msg)

		return

	def processUpdateSoftwareAckMsg(self, deviceInfo, msg):
		pass


	# 发送重启设备消息
	def SendRebootMsgToDevice(self, deviceList, msgInfo):
		delay_second 	= msgInfo["delay_second"]
		reboot_mode 	= msgInfo["reboot_mode"]

		# 如果没有此参数，则默认为0，立即重启
		if delay_second == None:
			delay_second = 0

		if reboot_mode == None:
			reboot_mode = Reboot_Application

		for item in deviceList:
			msg = CRSURebootMsg()
			msg.setParams(delay_second, reboot_mode)

			self.SendMsgToDevice(item, msg)

		return

	# 处理设备来的重启响应消息
	def processRebootAckMsg(self, deviceInfo, msg):
		# 不需要处理
		pass

	# 设备上报运行状态
	def processReportStatusMsg(self, deviceInfo, msg):
		
		dbHelper = DbHelper()

		currentDate = datetime.datetime.now().strftime('%Y-%m-%d')
		currentTime = datetime.datetime.now().strftime('%H:%M:%S')
		fullTime = currentDate + " " + currentTime
		data = {
			"create_time" 	: fullTime,
			"device_id" 	: deviceInfo["device_id"],
			"ip_address" 	: msg.srcAddr,
			"cpu" 			: msg.cpu,
			"memory" 		: msg.memory,
			"harddisk" 		: msg.harddisk,
			'date' 			: currentDate,
			'time' 			: currentTime,
			'system_user_id': self.system_user_id,
		}

		db = dbHelper.getDB("public.device_status")
		db.add(data)

		# 发送响应消息
		ackMsg = CRSUReportStatusAckMsg()
		ackMsg.serialNo = msg.serialNo
		self.SendMsgToDevice(deviceInfo, ackMsg)

		return

	# 查找设备是否在线
	def FindOnlineDevice(self, rsuInfo):
		for item in self.onlineRSU:
			if item['device_id'] == rsuInfo['device_id']:
				return item

		return None


	# 从服务器页面发送消息给设备
	def sendMessage(self, deviceList, msgInfo):

		msgType = msgInfo["message_type"]

		if msgType == RSU_MSG_UpdateSoftware:
			self.SendUpdateMsgToDevice(deviceList, msgInfo)

		elif msgType == RSU_MSG_Reboot:
			self.sendRebootMsgToDevice(deviceList, msgInfo)
			
		else:
			logE("RSU Manager: sendMessage, error msgtype: %d"%msgType)

		return

	# 发送消息给设备
	def SendMsgToDevice(self, deviceInfo, msg):

		# 发送消息给设备，如果设备没有连接，则不发送消息
		msgType = msg.msgType
		if self.FindOnlineDevice(deviceInfo) == None:
			
			if msgType not in noShowMsgList:
				logI("Device %s is offline, message %s will be discard"%(deviceInfo['device_code'], msg.toString()))
			else:
				logD("Device %s is offline, message %s will be discard"%(deviceInfo['device_code'], msg.toString()))

			return
		else:
			deviceInfo = self.FindOnlineDevice(deviceInfo)

		msg.srcAddr 		= self.LocalAddr
		msg.srcPort 		= RSU_SERVER_PORT
		msg.srcModuleType 	= MODULE_RSU_MANAGER
		msg.srcModuleCode 	= self.strModuleCode

		msg.destAddr 		= deviceInfo['ip_address']
		msg.destPort 		= RSU_CLIENT_PORT
		msg.destModuleType 	= MODULE_RSU
		msg.destModuleCode 	= deviceInfo['device_code']

		basemsg = CHeartBeatMsg() 										# 使用消息基类的编码功能
		if basemsg.encodeMsg(msg) == False:
			logE("Error: Encode %s failed."%msg.toString())
			return

		msgData = msg.getMsgBuffer()


		# # 发送的消息记录到消息日志
		# self.LogMsg(deviceInfo, msg)

		clientAddr 	= deviceInfo['ip_address']
		clientPort 	= RSU_CLIENT_PORT

		if msgType not in noShowMsgList:
			logI("Send %s to %s(%s:%d)"%(msg.toString(), deviceInfo['device_code'], clientAddr, clientPort))
			logD("Hex Data: %s"%ConvertToHex(msgData))
		else:
			logD("Send %s to %s(%s:%d)"%(msg.toString(), deviceInfo['device_code'], clientAddr, clientPort))
			logD("Hex Data: %s"%ConvertToHex(msgData))

		self.UDPServer.sendMessage( msgData, (clientAddr, clientPort) )


# 根据配置文件中配置的模块列表，将业务模块的处理实例加入到数组中
if "rsu" in SystemModule:
	info = {
		"name" 	: "rsu",
		"server": CRSUMsgServer(),
	}
	SystemModuleList.append( info )

