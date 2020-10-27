# coding:utf-8

# 定义消息基类，和业务无关，业务相关消息由业务在消息基类派生后使用

import uuid
from socket import *
import struct

from libs.utils.debug import *
import config

mac = uuid.UUID(int = uuid.getnode()).hex[-12:]
mac_addr = ":".join([mac[e:e+2] for e in range(0,11,2)])


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
# 	destAddr: 消息目的地IP地址
# 	destPort: 消息目的地端口，4字节，未使用
# 	serialNo：消息序号，指本消息的重发次数，从0开始，4字节
# 	macAddr ：发送模块的MAC地址，字符串，通过mac地址或模块编码识别客户端，客户端地址可能不固定，此参数未使用
# 	srcModuleCode: 消息源模块编号
# 	srcModuleType: 消息源模块类型， 4字节
# 	destModuleCode: 消息目的地模块编号
# 	destModuleType: 消息目的地模块类型， 4字节
# 	msgLeng ：消息体数据长度，4字节
# 	消息体
# 	endFlag ：消息结束标志，4字节
# 	
# 	字符串编码方式：首先加入字符串长度，然后再是字符串本身，在解码时首先取得字符串长度，再根据长度取得字符串内容
# 	
class CBaseMessage() :
	def __init__(self) :

		# 消息头部、尾部标识
		self.headFlag = MSG_START_FLAG 		
		self.endFlag  = MSG_END_FLAG

		# 本机地址
		self.localAddr 	= gethostbyname(gethostname())

		# 发送接收方IP地址，为字符串
		self.srcAddr 	= ""
		self.destAddr 	= ""

		# 消息类型，由业务定义
		self.msgType 	= 0

		# 消息序列号，对发送的每一条消息，代表发送次数，初始值为0
		self.serialNo 	= 0

		# 暂不使用mac地址及编号属性
		# # MAC地址或模块编号表示ID，根据mac地址或模块编码来确定模块
		# self.mac_addr 	=  mac_addr

		self.srcModuleCode 	= ""
		self.srcModuleType  = MSG_INVALID_MODULE_TYPE

		self.destModuleCode = ""
		self.destModuleType = MSG_INVALID_MODULE_TYPE

		# 消息数据，为编解码后的数据
		self.msgBuffer 	= None

		# 编解码起始位置，初始值为0
		self.pos 		= 0
		self.msgLength	= 0

	# 设置消息的目的地址，用于检验消息是否接收正确
	def setDestAddr(self, addr, port):
		self.destAddr 	= addr
		self.destPort 	= port
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
	def encodeWord(self, param):
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

	# 消息体数据编码，需要加入数据长度
	def encodeData(self, msgData):

		msgDataLen = len(msgData)
		self.encodeUINT(msgDataLen)

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

		# print("decode char: ", value, self.pos)
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
	def decodeWord(self):
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

		# if self.srcAddr == None:
		# 	ipAddr = 0
		# 	logI("Warning: Msg Type: %s, source IP address is NONE"%self.getMsgTypeStr())
		# else:
		# 	ipAddr =  ntohl(struct.unpack("I", inet_aton(self.srcAddr))[0])
		
		# if self.encodeUINT(ipAddr) == False:
		# 	return False

		# if self.destAddr == None:
		# 	ipAddr = 0
		# 	logI("Warning: Msg Type: %s, destination IP address is NONE"%self.getMsgTypeStr())
		# else:
		# 	ipAddr =  ntohl(struct.unpack("I", inet_aton(self.destAddr))[0])
		
		# if self.encodeUINT(ipAddr) == False:
		# 	return False

		if self.encodeUINT(self.msgType) == False:
			return False

		if self.encodeString(self.srcAddr) == False:
			return False
		if self.encodeUINT(self.srcPort) == False:
			return False

		if self.encodeString(self.destAddr) == False:
			return False
		if self.encodeUINT(self.destPort) == False:
			return False

		if self.encodeUINT(self.serialNo) == False:
			return False

		if self.encodeString(self.srcModuleCode) == False:
			return False

		if self.encodeUINT(self.srcModuleType) == False:
			return False

		if self.encodeString(self.destModuleCode) == False:
			return False

		if self.encodeUINT(self.destModuleType) == False:
			return False

		# 消息体数据长度由消息本身编码及解码
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

		# ipAddr  		= self.decodeUINT()
		# self.srcAddr 	= inet_ntoa(struct.pack('I', htonl(ipAddr)))

		# ipAddr  		= self.decodeUINT()
		# self.destAddr 	= inet_ntoa(struct.pack('I', htonl(ipAddr)))

		self.msgType  		= self.decodeUINT()

		self.srcAddr 		= self.decodeString()
		self.srcPort 		= self.decodeUINT()
		self.destAddr 		= self.decodeString()
		self.destPort 		= self.decodeUINT()

		self.serialNo 		= self.decodeUINT()

		# 这四个参数未使用
		self.srcModuleCode 	= self.decodeString()
		self.srcModuleType 	= self.decodeUINT()
		self.destModuleCode = self.decodeString()
		self.destModuleType = self.decodeUINT()

		# 消息体长度，没有使用此参数
		msgDataLen 		= self.decodeUINT()

		return True

	def decodeEnd(self):
		self.endFlag = self.decodeUINT()
		if self.endFlag != MSG_END_FLAG:
			logI("receive a meesage %d with error end flag: %d"%(self.msgType, self.endFlag))
			return False

		return True

	def __repr__ (self):
		return "messageBase"

