# coding: utf-8

# IOT 消息定义

from libs.socketMsg.messageConfig import *
from libs.socketMsg.messageBase import *
from iot.iotConfig import *

iotIndex = getModuleIndex("IOT")
if iotIndex == None:
	iotIndex = 1

IOT_MSG_BASE = MSG_TYPE_BASE + MAX_MSG_COUNT * iotIndex

# 定位模块消息类型定义

IOT_MODULE_INITIAL		= IOT_MSG_BASE + 1
IOT_MODULE_INITIAL_ACK	= IOT_MSG_BASE + 2

IOT_LOCATION_REPORT		= IOT_MSG_BASE + 3
IOT_LOCATION_REPORT_ACK	= IOT_MSG_BASE + 4


# 位置上报时间间隔，单位为秒
IOT_REPORT_INTERVAL 	= 5 * 60
IOT_MIN_INTERVAL 		= 60
IOT_MAX_INTERVAL 		= 15 * 60


# 模块初始化消息
class iotModuleInitialMsg(messageBase):
	def __init__(self):
		messageBase.__init__(self)

		self.msgType = IOT_MODULE_INITIAL

		return

	def encode(self):
		self.encodeHead()
		self.encodeEnd()

	def decode(self):
		if self.decodeHead() == False:
			return False

		if self.decodeEnd() == False:
			return False

		return True

	def __repr__ (self):
		return "IOT Module Initial Message, srcAddr: %s, destAddr: %s"%(self.srcAddr, self.destAddr)

# 模块初始化应答消息
class iotModuleInitialAckMsg(messageBase):
	def __init__(self):
		messageBase.__init__(self)

		self.msgType = IOT_MODULE_INITIAL_ACK

		# 位置上报时间间隔
		self.reportInterval = IOT_REPORT_INTERVAL
	
	def encode(self):
		self.encodeHead()
		self.encodeUINT(self.reportInterval)
		self.encodeEnd()

	def decode(self):
		if self.decodeHead() == False:
			return False

		self.reportInterval = self.decodeUINT()
		# 如果上报时间超出了允许的范围，则使用默认的时间间隔
		if self.reportInterval < IOT_MIN_INTERVAL or self.reportInterval > IOT_MAX_INTERVAL:
			self.reportInterval = IOT_REPORT_INTERVAL

		if self.decodeEnd() == False:
			return False

		return True

	def __repr__ (self):
		return "IOT Module Initial ACK Message, srcAddr: %s, destAddr: %s, report interval: %d s. "%(self.srcAddr, self.destAddr, self.reportInterval)

# 定位模块位置上报消息
class iotLocationReportMsg(messageBase):
	def __init__(self):
		messageBase.__init__(self)
	
		self.msgType = IOT_LOCATION_REPORT

		# 消息参数：经纬度、上报时间、IP地址、CPU、内存、硬盘使用率
		self.lng 		= ""
		self.lat 		= ""
		self.time 		= ""
		self.ip_addr 	= ""

		self.cpu 		= 0
		self.memory		= 0
		self.harddisk	= 0

	def encode(self):
		self.encodeHead()

		self.encodeString( self.lng )
		self.encodeString( self.lat )
		self.encodeString( self.time )
		self.encodeString( self.ip_addr )
		self.encodeString( self.mac_addr )
		self.encodeUINT(self.cpu)
		self.encodeUINT(self.memory)
		self.encodeUINT(self.harddisk)
		
		self.encodeEnd()

		return

	def decode(self):
		if self.decodeHead() == False:
			return False

		self.lng 		= self.decodeString()
		self.lat 		= self.decodeString()
		self.time 		= self.decodeString()
		self.ip_addr 	= self.decodeString()
		self.mac_addr 	= self.decodeString()

		self.cpu 		= self.decodeUINT()
		self.memory 	= self.decodeUINT()
		self.harddisk	= self.decodeUINT()

		if self.decodeEnd() == False:
			return False

		return

	def __repr__ (self):
		desc  =  "IOT Location Report Message, srcAddr: %s, destAddr: %s"%(self.srcAddr, self.destAddr)
		desc += ", lng: %s, lat: %s, time: %s, ip_addr: %s, mac_addr: %s "%(self.lng, self.lat, self.time, self.ip_addr, self.mac_addr) 
		desc += ", cpu: %d, memory: %d, disk_usage: %d"%(self.cpu, self.memory, self.harddisk)

		return desc

# 定位模块位置上报应答消息
class iotLocationReportAckMsg(messageBase):
	def __init__(self):
		messageBase.__init__(self)
		self.msgType = IOT_LOCATION_REPORT_ACK

		# 无参数
	
	def encode(self):
		self.encodeHead()
		self.encodeEnd()
		return

	def decode(self):
		if self.decodeHead() == False:
			return False

		if self.decodeEnd() == False:
			return False

		return True

	def __repr__ (self):
		return "IOT Location Report ACK Message, srcAddr: %s, destAddr: %s"%(self.srcAddr, self.destAddr)


