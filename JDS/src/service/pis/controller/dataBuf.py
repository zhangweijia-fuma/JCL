#coding:utf-8

import sys, struct
from io import BytesIO
from utils.utils import *

class DataBuf(object) :

	def init(self):
		self.length = 0
		self.bbuf = None
		self.pos = 0

	def truncateBuf(self, pos = None):
		if not pos :
			pos = self.pos
		if not pos:
			return
		if (pos) >= len(self.bbuf) or not self.bbuf:
			self.resetBuf()
			return
		self.bbuf = self.bbuf[pos:len(self.bbuf)]
		self.pos = 0
		self.length = len(self.bbuf)

	def readUint8(self):
		v = self.bbuf[self.pos]
		self.pos += 1
		return v

	def readUint16(self):
		bbuf = self.bbuf
		pos = self.pos
		v = ((bbuf[pos + 0] << 8 )
			+ (bbuf[pos + 1] << 0))
		self.pos += 2
		return v

	def readInt16(self):
		bbuf = self.bbuf
		pos = self.pos
		v = ((bbuf[pos + 0] << 8 )
			+ (bbuf[pos + 1] << 0))

		if v & 0x8000:
			v = 0 - (v & ~0x8000)
		self.pos += 2
		return v

	def readUint32(self):
		bbuf = self.bbuf
		pos = self.pos
		v = ((bbuf[pos + 0] << 24 )
			+ (bbuf[pos + 1] << 16)
			+ (bbuf[pos + 2] << 8)
			+ (bbuf[pos + 3] << 0))
		self.pos += 4
		return v

	def readBcdStr(self, dlen):
		bbuf = self.bbuf
		pos = self.pos
		s = ""
		for i in range(dlen):
			v = bbuf[pos + i]
			s += str((v & 0xF0) >> 4)
			s += str((v & 0x0F) >> 0)
		self.pos += dlen
		return s

	def readStr(self, dlen):
		bbuf = self.bbuf
		pos = self.pos
		end = 0
		while end < dlen:
			c = bbuf[pos + end]
			if c == 0x00:
				break
			end += 1
		d = bbuf[pos:pos+end]
		s = d.decode(encoding="utf-8")
		self.pos += dlen
		return s

	def readData(self, dlen):
		bbuf = self.bbuf
		pos = self.pos
		d = bbuf[pos:pos+dlen]
		self.pos += dlen
		return d

	def writeUint8(self, v):
		self.bbuf[self.pos] = v
		self.pos += 1

	def writeUint16(self, v):
		bbuf = self.bbuf
		pos = self.pos
		bbuf[pos + 0] = ((v >> 8) & 0xFF)
		bbuf[pos + 1] = ((v >> 0) & 0xFF)
		self.pos += 2

	def writeInt16(self, v):
		bbuf = self.bbuf
		pos = self.pos
		f = False
		if v < 0:
			f = True
			v = 0 - v
		bbuf[pos + 0] = ((v >> 8) & 0xFF)
		bbuf[pos + 1] = ((v >> 0) & 0xFF)
		if f :
			bbuf[pos + 0] |= 0x80
		self.pos += 2

	def writeUint32(self, v):
		bbuf = self.bbuf
		pos = self.pos
		bbuf[pos + 0] = ((v >> 24) & 0xFF)
		bbuf[pos + 1] = ((v >> 16) & 0xFF)
		bbuf[pos + 2] = ((v >>  8) & 0xFF)
		bbuf[pos + 3] = ((v >>  0) & 0xFF)
		self.pos += 4

	def writeBcdStr(self, s):
		bbuf = self.bbuf
		pos = self.pos
		length = len(s)
		ofs = 0
		i = 0
		while i + 2 <= length:
			v1 = int(s[i])
			v2 = int(s[i + 1])
			v = (v1 << 4) + v2;
			bbuf[pos + ofs] = v
			ofs += 1
			i += 2
		self.pos += ofs

	def writeData(self, data):
		bbuf = self.bbuf
		pos = self.pos
		length = len(data)
		i = 0
		while i < length:
			bbuf[pos + i] = data[i]
			i += 1
		self.pos += length

	def writeStr(self, s, dlen=None):
		data = s.encode(encoding="utf-8")
		blen = len(data)

		if dlen is not None:
			if blen > dlen:
				data = data[:dlen]
				plen = 0
			else:
				plen = dlen - blen
		else:
			plen = 0

		self.writeData(data)

		#padding
		if plen:
			self.writeData(bytearray(plen))

	def left(self):
		return self.length - self.pos

	def resetBuf(self):
		# D("resetBuf");
		self.pos	= 0
		self.length	= 0
		self.bbuf	= None

	def setBuf(self, data):
		# D("setBuf")
		self.pos	= 0
		self.length	= len(data)
		self.bbuf	= data

	def getBuf(self):
		return self.bbuf

	def seek(self, offset, type=1):
		if type == 0:
			self.pos = offset
		elif type == 1:
			self.pos += offset

	def calcChksum(self, pos, len):
		v = 0
		for i in range(pos, pos + len):
			v += self.bbuf[i]
		return v

	def appendData(self, data):
		data = bytearray(data)
		if self.bbuf :
			self.bbuf += data
			self.length += len(data)
		else :
			self.bbuf = data
			self.length = len(data)

	def save(self):
		self.bk_pos = self.pos
		self.bk_length = self.length
		self.bk_bbuf = self.bbuf
		self.saved = True

	def restore(self):
		self.pos = self.bk_pos
		self.length = self.bk_length
		self.bbuf = self.bk_bbuf
		self.saved = False

