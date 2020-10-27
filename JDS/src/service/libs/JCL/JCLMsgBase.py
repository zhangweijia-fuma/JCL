# coding:utf-8
# ------------------------------------------------------------------------------
# JCCL - Component :: Base Message
# Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
# Author: Zhang Weijia
# ------------------------------------------------------------------------------
# Name:    JCLMsgBase.py
# Purpose: JCCL module base message definition
#  *----------------------------------------------------------------------------

# 定义消息基类，和业务无关，业务相关消息由业务在消息基类派生后使用


from libs.utils.debug import *

from JCL.JCLCommon import *

MSG_START_FLAG			= 0xA5A5A5A5
MSG_END_FLAG			= 0xDEDEDEDE

MESSAGE_MAX_LEN			= 1024 * 1024

MSG_INVALID_MODULE_TYPE = 0xFF


#心跳消息上报时间间隔，单位为秒
HEARTBEAT_INTERVAL 		= 6

# 消息最大超时次数
MSG_EXPIRE_TIMES 		= 3

# 消息重发时间间隔
MSG_RESEND_INTERVAL 	= 5


# 消息基类，完成消息头部及基本的编解码功能
# 
# 消息定义：
# 	headFlag: 消息开始标志，4字节
# 	msgType ：消息ID，4字节
# 	srcAddr ：消息源IP地址
# 	srcPort : 消息源端口，4字节，此参数未使用
# 	srcModuleCode: 消息源模块编号
# 	srcModuleType: 消息源模块类型， 4字节
# 	macAddr ：发送模块的MAC地址，字符串，通过mac地址或模块编码识别客户端，客户端地址可能不固定，此参数未使用
# 	destAddr: 消息目的地IP地址
# 	destPort: 消息目的地端口，4字节，未使用
# 	destModuleCode: 消息目的地模块编号
# 	destModuleType: 消息目的地模块类型， 4字节
# 	serialNo：消息序号，指本消息的重发次数，从0开始，4字节
# 	msgLen ：消息体数据长度，4字节
# 	消息体
# 	endFlag ：消息结束标志，4字节
# 	
# 	字符串编码方式：首先加入字符串长度，然后再是字符串本身，在解码时首先取得字符串长度，再根据长度取得字符串内容

# 消息类型定义, 消息定义规则：响应消息的ID为请求消息ID + 1
HeartBeatMsg  				= 0 										# 心跳消息
HeartBeatAckMsg				= 1 										# 心跳响应消息


MsgTypeList = {}

def setMsgType(msgType):
	def register(msg):
		MsgTypeList[msgType] = msg
		msg.msgType 		= msgType
		return msg

	return register

class CJCLMsgBase() :
	def __init__(self) :

		# 消息头部、尾部标识
		self.headFlag 		= MSG_START_FLAG 		


		# 消息类型，由业务定义
		self.msgType 		= 0

		# 发送接收方IP地址，为字符串
		self.srcAddr 		= getLocalIPAddr()
		self.srcPort 		= 0
		self.srcModuleCode 	= ""
		self.srcModuleType  = MSG_INVALID_MODULE_TYPE

		# 暂不使用mac地址及编号属性
		# # MAC地址或模块编号表示ID，根据mac地址或模块编码来确定模块
		self.srcMacAddr		=  LocalMacAddr

		self.destAddr 		= ""
		self.destport 		= 0
		self.destModuleCode = ""
		self.destModuleType = MSG_INVALID_MODULE_TYPE

		# 消息序列号，对发送的每一条消息，代表发送次数，初始值为0
		self.serialNo 		= 0
		self.msgLen 		= 0

		self.endFlag  		= MSG_END_FLAG


		# ##########################################################
		# 消息数据，为编解码后的数据
		self.msgBuffer 		= None

		# 编解码起始位置，初始值为0
		self.pos 			= 0
		self.msgLength		= 0


	# 设置消息的目的地址，用于检验消息是否接收正确
	def setDestAddr(self, addr, port):
		self.destAddr 		= addr
		self.destPort 		= port
		return

	def getMsgType(self):
		return self.msgType

	def getTypeName(self):
		return type(self).__name__

	def getMsgBuffer(self):
		return self.msgBuffer

	def setMsgBuffer(self, data):
		self.pos = 0
		self.msgBuffer = data


	# 对基本数据类型进行编解码
	
	# 4字节无符号整数 
	def encodeUINT(self, param ):
		if param == None:
			return False

		# 主机顺序转换为网络字节顺序
		# param = htonl(param)

		data = bytearray(4)
		data[0] = (( param >> 24) & 0xFF)
		data[1] = (( param >> 16) & 0xFF)
		data[2] = (( param >>  8) & 0xFF)
		data[3] = (( param >>  0) & 0xFF)

		if self.msgBuffer == None:
			self.msgBuffer = data
		else:
			self.msgBuffer += data

		self.pos += 4

		return True

	# 4字节有符号整数，转换为无符号整数再进行编码
	def encodeInt(self, param):
		if param == None:
			return False

		if param < 0:
			param = param & 0xFFFFFFFF

		return self.encodeUINT(param)

	# 有符号单字节编码，转换后再编码
	def encodeChar(self, param):
		if param == None:
			return False

		if param < 0:
			param = param & 0xFF

		return self.encodeByte(param)

	# 无符号单字节编码
	def encodeByte(self, param):
		if param == None:
			return False

		data = bytearray(1)
		data[0] = param

		if self.msgBuffer == None:
			self.msgBuffer = data
		else:
			self.msgBuffer += data

		self.pos += 1

		return True

	# 有符号双字节编码，转换后再编码
	def encodeShort(self, param):
		if param == None:
			return False

		if param < 0:
			param = param & 0xFFFF

		return self.encodeWord(param)

	# 无符号双字节编码
	def encodeUShort(self, param):
		if param == None:
			return False

		data = bytearray(2)
		data[0] = (( param >> 8) & 0xFF)
		data[1] = (( param >> 0) & 0xFF)

		if self.msgBuffer == None:
			self.msgBuffer = data
		else:
			self.msgBuffer += data
		
		self.pos += 2

		return True

	# 字符串，首先将字符串长度写入到消息数据中，然后再把字符串加入到消息中
	# 如果编码后的字符串长度小于指定长度，则需要补齐位数
	# 如果编码后的长度大于指定长度，则不用再处理
	def encodeString(self, param, strMaxLen = None):
		data = param.encode(encoding="utf-8")
		length = len(data)

		# 字符串长度
		if strMaxLen is not None and length > strMaxLen:
			length = strMaxLen
			data = data[0:strMaxLen]

		self.encodeUINT(length)

		if self.msgBuffer == None:
			self.msgBuffer = data
		else:
			self.msgBuffer += data

		return True

	# 消息体数据编码
	def encodeData(self, msgData):

		if msgData == None:
			return True
			
		if self.msgBuffer == None:
			self.msgBuffer = msgData
		else:
			self.msgBuffer += msgData

		self.pos += len(msgData)
		return True

	# 4字节无符号整数
	def decodeUINT(self):
		pos = self.pos
		msgBuffer = self.msgBuffer

		if ( pos + 3) > len(msgBuffer):
			# 数据异常
			logE("Message decode UINT error: pos: %d, len: %d"%(pos, len(msgBuffer)) )
			return 0
		
		value = ((msgBuffer[pos + 0] << 24 )
			+ (msgBuffer[pos + 1] << 16)
			+ (msgBuffer[pos + 2] << 8)
			+ (msgBuffer[pos + 3] << 0))

		self.pos += 4
		
		# 网络字节顺序转换为主机顺序
		
		# value2 = ntohl(value)
		# logI("INT value: %d, %d"%(value, value2))
		return value

	# 4字节有符号整数
	def decodeInt(self):
		value = self.decodeUINT()
		if value > 0x7FFFFFFF and value < 0x100000000:
			value = value - 0x100000000

		return value

	# 有符号单字节解码
	def decodeChar(self):
		value = self.decodeByte()

		if value > 0x7F and value < 0x100:
			value = value - 0x100

		return value

	# 无符号单字节解码
	def decodeByte(self):
		pos = self.pos
		msgBuffer = self.msgBuffer

		if ( pos + 1) > len(msgBuffer):
			# 数据异常
			logE("Message decode BYTE error: pos: %d, len: %d"%(pos, len(msgBuffer)) )
			return 0
		
		value = msgBuffer[pos]
		self.pos += 1

		# print("decode byte: ", value, self.pos)
		return value

	# 有符号双字节解码
	def decodeShort(self):
		value = self.decodeWord()

		if value > 0x7FFF and value < 0x10000:
			value = value - 0x10000

		return value

	# 无符号双字节解码
	def decodeUShort(self):
		pos = self.pos
		msgBuffer = self.msgBuffer

		if ( pos + 1) > len(msgBuffer):
			# 数据异常
			logD("error", pos, len(msgBuffer))
			return 0
		
		value = ((msgBuffer[pos + 0] << 8 )
			+ (msgBuffer[pos + 1] << 0))

		self.pos += 2
		
		return value

	# Modubus消息使用
	def decodeData(self, dataLength):
		pos = self.pos
		msgBuffer = self.msgBuffer

		if ( pos + dataLength) > len(msgBuffer):
			# 数据异常
			logD("error", pos, len(msgBuffer))
			return 0

		value = msgBuffer[ pos : pos + dataLength]

		self.pos += dataLength
		
		return value

	# 解码字符串，首先解析长度
	def decodeString(self):
		msgBuffer = self.msgBuffer

		length = self.decodeUINT()
		if length == 0:
			return ""

		pos = self.pos
		data = msgBuffer[pos:pos + length]

		value = data.decode(encoding="utf-8")
		self.pos += length

		return value

	# 对消息进行编码，形成消息体 
	def encodeHead(self):
		self.pos = 0
		self.msgBuffer 	= None

		if self.encodeUINT(self.headFlag) == False:
			return False

		if self.encodeUINT(self.msgType) == False:
			return False

		if self.encodeString(self.srcAddr) == False:
			return False

		if self.encodeUINT(self.srcPort) == False:
			return False

		if self.encodeString(self.srcModuleCode) == False:
			return False

		if self.encodeUINT(self.srcModuleType) == False:
			return False

		if self.encodeString(self.srcMacAddr) == False:
			return False

		if self.encodeString(self.destAddr) == False:
			return False

		if self.encodeUINT(self.destPort) == False:
			return False

		if self.encodeString(self.destModuleCode) == False:
			return False

		if self.encodeUINT(self.destModuleType) == False:
			return False

		if self.encodeUINT(self.serialNo) == False:
			return False

		return True

	def encodeEnd(self):
		return self.encodeUINT(self.endFlag)

	# 解析消息头部
	def decodeHead(self):

		self.pos = 0
		self.headFlag = self.decodeUINT()
		if self.headFlag != MSG_START_FLAG:
			logI("receive message with error Head Flag: %d"%self.headFlag)
			return False

		self.msgType  		= self.decodeUINT()

		self.srcAddr 		= self.decodeString()
		self.srcPort 		= self.decodeUINT()

		self.srcModuleCode 	= self.decodeString()
		self.srcModuleType 	= self.decodeUINT()

		self.srcMacAddr 	= self.decodeString()

		self.destAddr 		= self.decodeString()
		self.destPort 		= self.decodeUINT()

		self.destModuleCode = self.decodeString()
		self.destModuleType = self.decodeUINT()

		self.serialNo 		= self.decodeUINT()

		return True

	def decodeEnd(self):
		self.endFlag = self.decodeUINT()
		if self.endFlag != MSG_END_FLAG:
			logI("receive a meesage %s with error end flag: %X"%(self.toString(), self.endFlag))
			return False

		return True

	# 对消息进行编码，
	# 返回：编码后的数据
	def encodeMsg(self, msg):
		msg.pos = 0
		msg.msgBuffer = None

		# 首先将消息的消息体部分进行编码
		if msg.encode() == False:
			return False

		msgData = msg.getMsgBuffer()

		# 对消息头部、消息体、消息尾部进行编码
		if msg.encodeHead() == False:
			return False

		if msg.encodeData(msgData) == False:
			return False

		if msg.encodeEnd() == False:
			return False

		return True

	# 对消息数据进行解码
	# 返回：解码后的消息
	def decodeMsg(self, dataBuf):
		self.setMsgBuffer(dataBuf)
		if self.decodeHead() == False:
			logE("Error: decode message head error!")
			return None

		msgType = self.getMsgType()

		# 获得消息的对象类型
		decodeMsgType = MsgTypeList.get(msgType)
		if not decodeMsgType:
			logE("not supported type %x"%msgType)
			return None

		# 根据对象类型，生成一个对象
		msg = decodeMsgType()
		if msg.decode(dataBuf) == False:
			return None

		return msg

	# 无参数消息编码时将调用基类函数
	# 有参数消息编码时调用派生类函数
	def encode(self):
		# if self.encodeHead() == False:
		# 	return False
		# if self.encodeEnd() == False:
		# 	return False

		return True

	# 无参数消息解码时将调用基类函数
	def decode(self, dataBuf):
		self.setMsgBuffer(dataBuf)
		
		if self.decodeHead() == False:
			return False
		if self.decodeEnd() == False:
			return False

		return True

	def __repr__ (self):
		return "James Common Lib Message Base"

# 除心跳应答消息外，所有应答消息都有2个参数：
# ASK_TYPE：
# 	0，接收消息应答
# 	1：完成接收应答
# STATUS：
# 	0：未完成
# 	1：部分完成
# 	2：全部完成

ACK_Type_Receive 		= 0
ACK_Type_Send 			= 1
Request_Status_Failed 	= 0
Request_Status_Part 	= 1
Request_Status_Complete = 2

class CJCLBaseAckMsg(CJCLMsgBase):
	def __init__(self) :
		CJCLMsgBase.__init__(self)

		self.ackType 		= ACK_Type_Send
		self.requestStatus  = Request_Status_Complete

	def setParams(self, ack_type, status):
		self.ackType 		= ack_type
		self.requestStatus 	= status

	def getParam(self):
		return self.ackType, self.requestStatus

	def encode(self):
		self.pos = 0

		if self.encodeUINT(self.ackType) == False:
			return False

		if self.encodeUINT(self.requestStatus) == False:
			return False

		return True

	def decode(self, dataBuf) :
		self.setMsgBuffer(dataBuf)
		
		if self.decodeHead() == False:
			return False

		self.ackType 	= self.decodeUINT()
		self.requestStatus 	= self.decodeUINT()
		
		if self.decodeEnd() == False:
			return False

		return True

	def toString(self):
		return "msg type: %04X" % (self.msgType)

@setMsgType(HeartBeatMsg)
class CHeartBeatMsg(CJCLMsgBase):
	def __init__(self) :
		CJCLMsgBase.__init__(self)
		self.msgType 	= HeartBeatMsg


	def toString(self):
		return "HeartBeatMsg"

@setMsgType(HeartBeatAckMsg)
class CHeartBeatAckMsg(CJCLMsgBase):
	def __init__(self) :
		CJCLMsgBase.__init__(self)

		self.msgType 	= HeartBeatAckMsg

	def toString(self):
		return "HeartBeatAckMsg"
