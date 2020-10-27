#coding:utf-8

import sys, struct
from config import PisConfig
from utils.utils import *
from utils.debug import *
from .consts import *
from .message import *


"""
1	HEAD_FLAG	U32	4	0xA5A5A5A5	固定值，表示消息开始
2	SRC_ADDR	U32	4		发送者地址IP，保留字段
3	DEST_ADDR	U32	4		接收者地址IP，保留字段
4	MSG_TYPE	U16	2		消息类别
5	MSG_SID	U16	2		消息序号
6	MSG_LEN	U32	4		消息长度
7	MSG_DATA	U8 * N	MSG_LEN		消息数据
7	TAIL_FLAG	INT	4	0xDEDEDEDE	固定值，表示消息结束
"""


def _D(s):
	logD(s)
	pass

class MsgParser(DataBuf) :

	def __init__(self, callback = None) :
		DataBuf.init(self)
		self.callback = callback
		self.state	= STATE_READ_START_FLAG

	def setCallback(self, callback):
		self.callback = callback

	def readStartFlag(self):
		# _D("readStartFlag ==> " + str(self.pos))
		if not self.bbuf:
			return False

		# print(self.bbuf)
		if self.length < MIN_PKT_LEN:
			return False

		while self.left() >= MIN_PKT_LEN:
			m = self.readUint32()
			if m == START_FLAG:
				self.state = STATE_READ_HAEHER
				return True

			#back 1 byte
			self.seek(-3)

		self.truncateBuf()
		return False

	"""
	1	HEAD_FLAG	U32	4	0xA5A5A5A5	固定值，表示消息开始
	2	SRC_ADDR	U32	4		发送者地址IP，保留字段
	3	DEST_ADDR	U32	4		接收者地址IP，保留字段
	4	MSG_TYPE	U16	2		消息类别
	5	MSG_SID		U16	2		消息序号
	6	MSG_LEN		U32	4		消息长度
	7	MSG_DATA	U8 * N	MSG_LEN		消息数据
	7	TAIL_FLAG	INT	4	0xDEDEDEDE	固定值，表示消息结束
	"""

	def readHeader(self):
		# _D("readHeader ... @ %s" % now());
		if self.left() < HEADER_LEN:
			return False
		#self.header = self.bbuf[self.pos:(self.pos + 11)]

		self.header = {
			"src"	: self.readUint32(),
			"dest"	: self.readUint32(),
			"type"	: self.readUint16(),
			"sid"	: self.readUint16(),
			"len"	: self.readUint32(),
		}
		if self.header["len"] > MAX_PKT_LEN:
			self.resetBuf()
			self.state = STATE_READ_START_FLAG
			return
		self.header["rxTypeHex"] = formatHex(self.header["type"])
		# logD(self.header)
		self.state = STATE_READ_DATA
		return True

	def readData(self):
		# _D("readBody ==>");
		dlen = self.header["len"]
		if self.left() < dlen :
			D(self.left())
			return False
		self.data = self.bbuf[self.pos:(self.pos + dlen)]
		self.pos += dlen
		self.state = STATE_READ_END_FLAG
		return True

	def readEndFlag(self):
		if self.left() < END_FLAG_LEN:
			return False
		# _D("readEndFlag ==>");
		ef = self.readUint32()
		if ef != END_FLAG:
			logE("END_FLAG error 0x%04x vs 0x%04x" % (ef, END_FLAG))
			self.resetBuf()
			self.state = STATE_READ_START_FLAG
			return

		spos = self.pos - self.header["len"] - MIN_PKT_LEN
		self.packet = self.bbuf[spos : self.pos]

		if self.parseData():
			try :
				if self.callback:
					self.callback(self.msg)
			except Exception as e:
				onException(e)

		self.truncateBuf()
		self.state = STATE_READ_START_FLAG
		return True


	def parseData(self):
		# _D("parseData....")
		header = self.header
		t = header["type"]
		cls = MSG_TYPES.get(t)
		if not cls :
			logE("not supported type " + str(t))
			self.msg  = ErrorMsg()
			return False

		try :
			self.msg = cls()
			self.msg.packet = self.packet
			self.msg.sid = self.header["sid"]
			if self.host:
				self.msg.addr = self.host[0]
			self.msg.decode(self.data)
			return True
		except Exception as e:
			logE("parseData failed !!!!!!")
			onException(e)
			self.msg  = ErrorMsg()
			return False

	def handleData(self):
		r = False

		state = self.state
		if STATE_READ_START_FLAG == state:
			r = self.readStartFlag()
		elif STATE_READ_HAEHER == state:
			r = self.readHeader()
		elif STATE_READ_DATA == state:
			r = self.readData()
		elif STATE_READ_END_FLAG == state:
			r = self.readEndFlag()
		else:
			self.resetBuf()
			self.state = STATE_READ_START_FLAG

		return r

	def input(self, data):
		self.appendData(data)
		while self.handleData():
			pass

		return self.msg

	def parse(self, callback):
		while self.handleData():
			pass

	def setHost(self, host):
		self.host = host

class MsgEncoder(DataBuf) :
	def __init__(self) :
		DataBuf.init(self)
		self.txSids = {}

	def initSid(self, msg):
		if msg.sid != None:
			return msg.sid
		addr = msg.addr
		txSids = self.txSids
		if haskey(txSids, addr):
			sid = txSids[addr]
		else:
			sid = 1
		if sid > 0xFFFF:
			sid = 0
		txSids[addr] = sid + 1
		msg.sid = sid
		return sid

	def encode(self, msg):

		msgData = msg.encode()
		dataLen = len(msgData)

		data = bytearray(dataLen + PKT_EXTRA_LEN)
		self.setBuf(data)

		t = msg.getMsgType()

		self.writeUint32(START_FLAG)		#HEAD_FLAG
		self.writeUint32(0)					#SRC_ADDR
		self.writeUint32(0)					#DEST_ADDR
		self.writeUint16(t)					#MSG_TYPE
		self.writeUint16(self.initSid(msg))	#MSG_SID
		self.writeUint32(dataLen)			#MSG_LEN
		self.writeData(msgData)				#MSG_DATA
		self.writeUint32(END_FLAG)			#TAIL_FLAG

		b = bytes(self.bbuf)
		
		# logD("Encoded packet, type : %s, len : %s" % (formatHex(msg.getMsgType()), formatHex(len(b))))
		if PACKET_DEBUG:
			printHex(b)
		return b


