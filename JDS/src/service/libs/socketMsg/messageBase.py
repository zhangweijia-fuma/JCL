# coding:utf-8

# 定义消息基类，和业务无关，业务相关消息由业务在消息基类派生后使用

import uuid
from socket import *
from .messageConfig import *
from libs.utils.debug import *
import config

mac = uuid.UUID(int = uuid.getnode()).hex[-12:]
mac_addr = ":".join([mac[e:e+2] for e in range(0,11,2)])

# 消息基类，完成消息头部及基本的编解码功能
# 
# 消息定义：
# 	headFlag: 消息开始标志，4字节
# 	srcAddr ：消息源IP地址，字符串
# 	destAddr: 消息目的地IP地址，字符串
# 	msgType ：消息ID，4字节
# 	serialNo：消息序号，指本消息的重发次数，从0开始，4字节
# 	macAddr ：发送模块的MAC地址，字符串，通过mac地址或模块编码识别客户端，客户端地址可能不固定
# 	code 	：发送模块的编码，字符串
# 	消息体
# 	endFlag ：消息结束标志，4字节
# 	
# 	字符串编码方式：首先加入字符串长度，然后再是字符串本身，在解码时首先取得字符串长度，再根据长度取得字符串内容
# 	
class messageBase() :
	def __init__(self) :

		# 消息头部、尾部标识
		self.headFlag = MSG_START_FLAG 		
		self.endFlag  = MSG_END_FLAG

		# 本机地址
		self.localAddr 	= gethostbyname(gethostname())

		# 发送接收方IP地址，为字符串
		self.srcAddr 	= gethostbyname(gethostname())
		self.destAddr 	= ""

		# 消息类型，由业务定义
		self.msgType 	= 0

		# 消息序列号，对发送的每一条消息，代表发送次数，初始值为0
		self.serialNo 	= 0

		# 暂不使用mac地址及编号属性
		# # MAC地址或模块编号表示ID，根据mac地址或模块编码来确定模块
		# self.mac_addr 	=  mac_addr

		# # 模块编码由业务模块设置
		# self.code 	  	= ""

		# 消息数据，为编解码后的数据
		self.msgBuffer 	= None

		# 编解码起始位置，初始值为0
		self.pos 		= 0
		self.msgLength	= 0

	# 设置消息的目的地址，用于检验消息是否接收正确
	def setDestAddr(self, addr):
		self.destAddr 	= addr
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
	def encodeString(self, param, strLen = None):
		data = param.encode( encoding="utf-8")
		length = len(data)

		if strLen is not None:
			if length >= strLen:
				padding = 0
			else:
				padding = strLen - length
		else:
			padding = 0

		self.encodeUINT(length)
		self.msgBuffer += data
		
		self.pos += length

		if padding > 0:
			self.msgBuffer += bytearray(padding)
			self.pos += padding

		return True

	# 消息体数据
	def encodeData(self, msgData):
		if self.msgBuffer == None:
			logE("Error: encoding message data but header part not encoded yet!")
			return False

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
			print("error", pos, len(msgBuffer))
			return 0
		
		value = ((msgBuffer[pos + 0] << 8 )
			+ (msgBuffer[pos + 1] << 0))

		self.pos += 2
		
		return value

	# 解码字符串，首先解析长度
	def decodeString(self, strLen = None):
		msgBuffer = self.msgBuffer

		length = self.decodeUINT()
		if length == 0:
			return ""

		pos = self.pos
		data = msgBuffer[ pos : pos + length]

		value = data.decode(encoding="utf-8")
		self.pos += length

		# 跳过补齐部分
		if strLen is not None and length < strLen:
			self.pos += strLen - length

		return value

	# 对消息进行编码，形成消息体
	def encodeHead(self):
		self.pos = 0
		self.msgBuffer 	= None

		if self.encodeUINT(self.headFlag) == False:
			return False

		if self.encodeString(self.srcAddr) == False:
			return False

		if self.encodeString(self.destAddr) == False:
			return False
			
		if self.encodeUINT(self.msgType) == False:
			return False
			
		if self.encodeUINT(self.serialNo) == False:
			return False
			
		if self.encodeString(self.mac_addr) == False:
			return False
			
		if self.encodeString(self.code) == False:
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

		self.srcAddr  = self.decodeString()
		self.destAddr = self.decodeString()

		# if self.destAddr != self.localAddr:
		# 	logI("receive a message with error destination address: %s"%self.destAddr)
		# 	return False

		self.msgType  = self.decodeUINT()
		if self.msgType < MSG_TYPE_BASE:
			logI("receive a message with error msgType: %d"%self.msgType)
			return False

		self.serialNo = self.decodeUINT()

		self.mac_addr = self.decodeString()
		self.code 	  = self.decodeString()

		return True

	def decodeEnd(self):
		self.endFlag = self.decodeUINT()
		if self.endFlag != MSG_END_FLAG:
			logI("receive a meesage with error end flag: %d"%self.endFlag)
			return False

		return True

	def __repr__ (self):
		return "messageBase"


# 心跳消息，为公用消息，由客户端定时上报
class heartbeatMsg(messageBase):
	def __init__(self):
		messageBase.__init__(self)

		self.msgType  	= HEARTBEAT_MSG

		# 客户端版本号，以检查客户端版本是否有更新
		self.clientVer 	= config.clientVer

		return

	def encode(self):
		
		if self.encodeHead() == False:
			return False

		if self.encodeString(self.clientVer) == False:
			return False

		if self.encodeEnd() == False:
			return False

	def decode(self):
		if self.decodeHead() == False:
			return False

		self.clientVer = self.decodeString()

		if self.decodeEnd() == False:
			return False

		return True

	def __repr__ (self):
		return "Heartbeat Message, srcAddr: %s, destAddr: %s, version: %s"%(self.srcAddr, self.destAddr, self.clientVer)

# 心跳应答消息
class heartbeatAckMsg(messageBase):
	def __init__(self):
		messageBase.__init__(self)

		self.msgType   = HEARTBEAT_ACK_MSG

		# 客户端软件最新版本号
		self.clientVer = config.clientVer
		
		# 升级版本的路径，为服务器路径
		self.verPath   = config.clientVersionPath

	
	def encode(self):
		if self.encodeHead() == False:
			return False

		if self.encodeString(self.clientVer) == False:
			return False

		if self.encodeString(self.verPath) == False:
			return False

		if self.encodeEnd() == False:
			return False

	def decode(self):
		if self.decodeHead() == False:
			return False

		self.clientVer = self.decodeString()
		self.verPath = self.decodeString()

		if self.decodeEnd() == False:
			return False

		return True

	def __repr__ (self):
		return "Heartbeat ACK Message, srcAddr: %s, destAddr: %s, version: %s, path:%s"%(self.srcAddr, self.destAddr, self.clientVer, self.verPath)

# 获取模块序号
def getModuleIndex(moduleName):

	index = 0
	for module in config.SystemModule:
		if module == moduleName:
			# index 至少从1 开始，以避开公共消息
			return index + 1

		index += 1

	return None
