# coding:utf-8

# IOT 消息处理

import os, sys
import datetime, time
import threading
from socket import *

from sys import path

sysDir = "/JamesSoft/JDS/src/service"

path.append( sysDir )
path.append( sysDir + "/libs" )
path.append( sysDir + "/libs/db" )
path.append( sysDir + "/libs/utils" )

import config
from libs.socketMsg.messageSocket import *
from libs.socketMsg.messageConfig import *
from libs.socketMsg.messageBase import *
from libs.utils.debug import *

from iot import iotConfig
from iot.iotMessage import *
from systemUsage import *


# IOT 业务消息处理
class iotClient(threading.Thread):
	def __init__(self):
		threading.Thread.__init__(self)
		
		self.clientSock = None

		self.srvAddr = iotConfig.serverAddr
		self.srvPort = iotConfig.serverPort

		# 定时器
		self.timers = {}

		self.runFlag 		= True

		# 设置初始值
		self.reportInterval = IOT_REPORT_INTERVAL

		# 初始化时先获取一次CPU占有率
		getCPUUsage()

		self.toInitial = True

		self.clientVer = config.clientVer

		return

	def __del__(self):
		self.close()

		return

	def close(self):
		self.closeTimer()

		self.runFlag = False
		if self.clientSock:
			self.clientSock.close()
			self.clientSock = None


	def closeTimer(self):
		for item in self.timers:
			if self.timers[item] != None:
				self.timers[item].cancel()
				self.timers[item] = None

	# 启动
	def init(self):
		logI("initial client...")

		# 设置待重发的消息，心跳消息不需要重发
		self.resendMsgList = {
			'iotModuleInitialMsg' : None,
			'iotLocationReportMsg': None,
		}

		self.heartbeatSendTimes = 0

		# 连接到服务端
		self.clientSock = socketClient(iotConfig.SOCKET_TYPE, self.onReceiveMsg )
		
		# 连接到服务端，连接异常由connect函数处理
		self.clientSock.connect(self.srvAddr, self.srvPort)

		# 设置定时器，定时发送心跳消息和位置上报消息
		# self.timers['heartbeatMsg'] = threading.Timer(IOT_HEARTBEAT_INTERVAL, self.sendHeartbeatMsg)
		# self.timers['heartbeatMsg'].start()

		self.timers['locationReportMsg'] = threading.Timer(IOT_REPORT_INTERVAL, self.sendReportMsg)
		self.timers['locationReportMsg'].start()

		self.toInitial = False

		# 等待heartbeat消息响应后设置为True
		self.isConnected = False
		
		#立即发送心跳消息
		self.sendHeartbeatMsg()

		return

	def run(self):

		logI("IOT client Starting...")

		while self.runFlag:
			if self.toInitial:
				self.init()
				time.sleep(1)

				# 发送模块初始化消息
				self.sendModuleInitialMsg()
			
			if self.clientSock == None:
				continue

			# logI("waiting for data...")
			data = self.clientSock.receiveMessage()

		logI("IOT client exit...")

		return

	# 发送模块初始化消息
	def sendModuleInitialMsg(self):

		# 发送初始化消息
		msg = iotModuleInitialMsg()
		msg.setDestAddr(self.srvAddr)

		if self.isConnected == False:
			self.resendMsgList['iotModuleInitialMsg'] = msg
			self.timers['resendModuleInitialMsg'] = threading.Timer(MSG_RESEND_INTERVAL, self.resendModuleInitialMsg)
			self.timers['resendModuleInitialMsg'].start()

			return

		logI("send message to server: %s"%msg.__repr__())
		
		if self.sendMessage(msg):
			# 设置定时器，准备重发该消息
			self.resendMsgList['iotModuleInitialMsg'] = msg
			self.timers['resendModuleInitialMsg'] = threading.Timer(MSG_RESEND_INTERVAL, self.resendModuleInitialMsg)
			self.timers['resendModuleInitialMsg'].start()

		return

	# 重发初始化消息
	def resendModuleInitialMsg(self):
		msg = self.resendMsgList['iotModuleInitialMsg']
		if msg == None:
			self.timers['resendModuleInitialMsg'] = None
			return

		msg.serialNo += 1
		if msg.serialNo > MSG_EXPIRE_TIMES:
			# 重发次数超过上限，停止重发
			self.resendMsgList['iotModuleInitialMsg'] = None
			logE("resend Module Initial Message exceed max times:%d"%msg.serialNo)

			# 准备重新初始化
			self.toInitial = True
			self.closeTimer()
			return

		self.resendMsgList['iotModuleInitialMsg'] = msg
		
		# 系统未连接，设置定时器，等待重发
		if self.isConnected == False:
			self.resendMsgList['iotModuleInitialMsg'] = msg
			self.timers['resendModuleInitialMsg'] = threading.Timer(MSG_RESEND_INTERVAL, self.resendModuleInitialMsg)
			self.timers['resendModuleInitialMsg'].start()

			return

		logI("resend message to server: %s"%msg.__repr__())
		
		if self.sendMessage(msg):
			# 设置定时器，准备重发该消息
			self.resendMsgList['iotModuleInitialMsg'] = msg
			self.timers['resendModuleInitialMsg'] = threading.Timer(MSG_RESEND_INTERVAL, self.resendModuleInitialMsg)
			self.timers['resendModuleInitialMsg'].start()

		return

	# 心跳消息，不需要设置重发，但需要检查是否接收到响应消息以确保连接正常
	def sendHeartbeatMsg(self):

		self.heartbeatSendTimes += 1

		if self.heartbeatSendTimes > MSG_EXPIRE_TIMES:
			logE("not receive heartbeat ack message, system connection broken. re-connect to server...")

			# 发送三次都没有接收到消息消息就认为和服务端连接中断，需要重新进行连接
			# 准备重新初始化
			self.toInitial = True
			self.clientSock.close()
			self.clientSock = None

			self.closeTimer()
			return

		msg = heartbeatMsg()
		msg.setDestAddr(self.srvAddr)

		logI("send heartbeat to server: %s"%msg.__repr__())

		self.sendMessage(msg)
		
		# 重新设置定时器
		self.timers['heartbeatMsg'] = threading.Timer(HEARTBEAT_INTERVAL, self.sendHeartbeatMsg)
		self.timers['heartbeatMsg'].start()

		return

	# 发送位置上报消息
	def sendReportMsg(self):

		msg = iotLocationReportMsg()
		msg.setDestAddr(self.srvAddr)

		# 获取当前位置的经纬度
		msg.lng 		= "121.81502"
		msg.lat 		= "31.157386"

		msg.time 		= datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		msg.ip_addr 	= msg.localAddr

		# 获取当前系统的运行数据
		msg.cpu 		= getCPUUsage()
		msg.memory		= getMemoryUsage()
		msg.harddisk	= getDiskUsage()
		
		logI("send location report message to server: %s"%msg.__repr__())

		if self.sendMessage(msg):
			# 设置定时器，以重发该消息
			self.resendMsgList['iotLocationReportMsg'] = msg
			self.timers['resendLocationReportMsg'] = threading.Timer(MSG_RESEND_INTERVAL, self.resendLocationReportMsg)
			self.timers['resendLocationReportMsg'].start()

		# 继续设置定时器，定期发送位置上报消息
		self.timers['locationReportMsg'] = threading.Timer( self.reportInterval, self.sendReportMsg)
		self.timers['locationReportMsg'].start()

		return

	# 重发位置上报消息
	def resendLocationReportMsg(self):
		msg = self.resendMsgList['iotLocationReportMsg']
		if msg == None:
			return

		msg.serialNo += 1
		if msg.serialNo > MSG_EXPIRE_TIMES:
			# 重发次数超过上限，停止重发
			self.resendMsgList['iotLocationReportMsg'] = None
			logE("resend Location Report Message exceed max times:%d"%msg.serialNo)

			# 准备重新初始化
			self.toInitial = True

			return

		self.resendMsgList['iotLocationReportMsg'] = msg
		logI("resend Location Report Message to server")
		
		if self.sendMessage(msg):
			# 继续设置定时器
			self.timers['resendLocationReportMsg'] = threading.Timer(MSG_RESEND_INTERVAL, self.resendLocationReportMsg)
			self.timers['resendLocationReportMsg'].start()

		return


	# 发送消息给服务端，如果消息发送失败，则准备重新连接到服务端
	# 返回值：True or False
	def sendMessage(self, msg):

		if self.isConnected == False and msg.getMsgType() != HEARTBEAT_MSG:
			# 系统还没有连接，不发送除心跳消息外的任何消息
			return False

		msg.encode()

		addr = (self.srvAddr, self.srvPort)

		if self.clientSock == None:
			return

		ret = self.clientSock.sendMessage( msg.getMsgBuffer(), addr )
		if ret == False:
			# 发送消息失败，可能是连接中断
			logE("send message failed......")

		return ret


	# 消息处理
	def onReceiveMsg(self, data):
		
		# logI("receive message from server...")

		msg = messageBase()
		msg.setMsgBuffer(data)
		if msg.decodeHead() == False:
			return

		msgType = msg.getMsgType()

		# 心跳响应消息
		if msgType == HEARTBEAT_ACK_MSG:
			msg = heartbeatAckMsg()
			msg.setMsgBuffer(data)
			if msg.decode() == False:
				return

			self.processHeartbeatAckMsg(msg)

		# 模块初始化响应消息
		elif msgType == IOT_MODULE_INITIAL_ACK:
			msg = iotModuleInitialAckMsg()
			msg.setMsgBuffer(data)
			if msg.decode() == False:
				return

			self.processModuleInitialAckMsg(msg)

		# 位置上报响应消息
		elif msgType == IOT_LOCATION_REPORT_ACK:
			msg = iotLocationReportAckMsg()
			msg.setMsgBuffer(data)
			if msg.decode() == False:
				return

			self.processLocationReportAckMsg(msg)

		else:
			logE("receive an error message from server: ", msgType)

		return

	# 处理心跳响应消息
	def processHeartbeatAckMsg(self, msg):
		
		logI("receive message from server: %s"%msg.__repr__())

		# 每次收到响应消息都把超时次数重置为0
		self.heartbeatSendTimes = 0

		# 比较客户端版本号，以确定是否升级
		if self.clientVer != msg.clientVer:
			self.updateClientVer(msg.verPath)

		# 设置系统连接正常标志
		self.isConnected = True

		return

	# 处理模块初始化响应消息
	def processModuleInitialAckMsg(self, msg):
		logI("receive message from server: %s"%msg.__repr__())
		if msg.reportInterval != IOT_REPORT_INTERVAL:
			if msg.reportInterval >= IOT_MIN_INTERVAL and msg.reportInterval <= IOT_MAX_INTERVAL:
				
				logI("change report interval to: %d"%msg.reportInterval)

				if self.reportTimer:
					self.reportTimer.cancel()

				# 重新设置位置上报时间
				self.reportTimer = threading.Timer( msg.reportInterval, self.sendReportMsg)
				self.reportTimer.start()

		# 不需要再重发
		self.resendMsgList['iotModuleInitialMsg'] = None
		if self.timers['resendModuleInitialMsg'] != None:
			self.timers['resendModuleInitialMsg'].cancel()
			self.timers['resendModuleInitialMsg'] = None

		return

	# 定位模块位置上报响应消息
	def processLocationReportAckMsg(self, msg):
		
		logI("receive message from server: %s"%msg.__repr__())

		# 不需要再重发
		self.resendMsgList['iotLocationReportMsg'] = None
		if self.timers['resendLocationReportMsg'] != None:
			self.timers['resendLocationReportMsg'].cancel()
			self.timers['resendLocationReportMsg'] = None
			
		return


	# 客户端版本更新，启动外部程序下载版本后重新启动客户端
	def updateClientVer(self, verPath ):
		logI("exit client and update client version...")
		
		return

import os
os.system("clear")

def main():

	client = iotClient()
	client.start()

	return

# 运行测试客户端
main()

# 消息编解码测试
# class testMsg(messageBase):
# 	def __init__(self):
# 		messageBase.__init__(self)

# 		self.msgType   = 1
# 		self.intv = -1
# 		self.intc = -1
# 		self.ints = -1
# 		self.str = "abcde"
	
# 	def encode(self):
# 		if self.encodeHead() == False:
# 			return False

# 		if self.encodeString(self.str, 30) == False:
# 			return False

# 		if self.encodeChar(self.intc) == False:
# 			return False

# 		if self.encodeShort(self.ints) == False:
# 			return False

# 		if self.encodeInt(self.intv) == False:
# 			return False

# 		if self.encodeEnd() == False:
# 			return False

# 	def decode(self):
# 		if self.decodeHead() == False:
# 			return False
			
# 		self.str  = self.decodeString(30)

# 		self.intc = self.decodeChar()
# 		self.ints = self.decodeShort()
# 		self.intv = self.decodeInt()

# 		if self.decodeEnd() == False:
# 			return False

# 		return True

# 	def __repr__ (self):
# 		return "test Message, char:%d, short:%d, int:%s, str:%s"%(self.intc, self.ints, self.intv, self.str)

# def testCoding():
	# msg = testMsg()
	# msg.intv = -22
	# msg.intc = -13
	# msg.ints = -1232
	# msg.str = 'andwjjskjsass'
	# msg.encode()

	# data = msg.getMsgBuffer()
	# print(data)

	# msg2 = testMsg()
	# msg2.setMsgBuffer(data)

	# msg2.decode()
	# print(msg2.__repr__())
	# return

# testCoding()

