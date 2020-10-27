#coding:utf-8

import sys, time, threading
from config import PisConfig
from utils.utils import *
from utils.debug import *
from .consts import *
from .message import *
from .tcpServer import *  
from .msgLogger import *


def _printHex(data):
	logI(" ".join(["%02x" % d for d in data]))

class ModbusMsg(DataBuf) :

	def __init__(self) :
		DataBuf.init(self)
		self.busId = 0
		self.proId = 0
		self._length = 0
		self.unitId = 0
		self.fcode = 0
		self.saddr = 0
		self.regNum = 0
		self.byteNum = 0
		self.data = []
		self.addr = None
		self.ack = False
		self.saved = False

	def parse(self, addr):
		if self.saved:
			self.restore()

		dlen = self.left()
		if not dlen:
			return False
		if dlen < 8:
			logE("ModbusMsg too short : %s" % dlen)
			return False

		#header
		self.busId = self.readUint16()
		self.proId = self.readUint16()
		self._length = self.readUint16()
		self.unitId = self.readUint8()

		if addr:
			self.addr = addr[0]
		else:
			self.addr = None

		#func code
		self.fcode = self.readUint8()

		if self.fcode == 0x03:
			if self._length != 6:
				logE("ModbusMsg 03H'length != 6")
				return False

			if dlen < 12:
				logE("ModbusMsg 03H' total length < 12")
				return False

			self.saddr = self.readUint16()
			self.regNum = self.readUint16()

		elif self.fcode == 0x10:
			if dlen < 13:
				logE("ModbusMsg 10H' total length < 13")
				return False

			if dlen < self._length + 6:
				logE("ModbusMsg 10H' total length < length + 6")
				return False

			self.saddr = self.readUint16()
			self.regNum = self.readUint16()
			self.byteNum = self.readUint8()

			if self.byteNum < self._length - 7:
				logE("ModbusMsg 10H' byte length < length - 7")
				return False

			self.data = self.readData(self.byteNum)
		else:
			logE("ModbusMsg invalid fcode : %02x" % self.fcode)
			return False

		self.packet = self.bbuf[self.pos - (self._length + 6):self.pos]

		if self.left() > 0:
			logE("ModbusMsg save since left(%s)>0" % self.left())
			self.save()

		return True

	def toString(self):
		if self.ack:
			s = ("busId : %04x, proId : %04x, length : %04x, unitId : %02x, fcode : %02x, saddr : %04x, regNum : %04x, byteNum : %02x, data : [%s]"
					% (self.busId, self.proId, self._length, self.unitId, self.fcode, self.saddr, self.regNum, self.byteNum, " ".join(["%02x" % d for d in self.data])))
		else:
			s = ("busId : %04x, proId : %04x, length : %04x, unitId : %02x, fcode : %02x, saddr : %04x, regNum : %04x, byteNum : %02x, data : [%s]"
					% (self.busId, self.proId, self._length, self.unitId, self.fcode, self.saddr, self.regNum, self.byteNum, " ".join(["%02x" % d for d in self.data])))
		return s

	def setData(self, data):
		self.data = data
		self.byteNum = len(data)

	def response(self, error = False):
		if error:
			rcode = self.fcode | 0x80
			length = 3
		else:
			rcode = self.fcode
			length = 0

		if error or self.fcode == 0x03:
			if not error:
				length = self.byteNum + 3

			self.setBuf(bytearray(length + 6))
			self.writeUint16(self.busId)
			self.writeUint16(self.proId)
			self.writeUint16(length)
			self.writeUint8(self.unitId)
			self.writeUint8(rcode)
			if not error:
				self.writeUint8(self.byteNum)
				self.writeData(self.data)
			else:
				self.writeUint8(0x01)
		elif self.fcode == 0x10:
			if not error:
				length = 6
			self.setBuf(bytearray(length + 6))
			self.writeUint16(self.busId)
			self.writeUint16(self.proId)
			self.writeUint16(length)
			self.writeUint8(self.unitId)
			self.writeUint8(rcode)
			if not error:
				self.writeUint16(self.saddr)
				self.writeUint16(self.regNum)
			else:
				self.writeUint8(0x01)
		else:
			return None

		self.ack = True
		self._length = length
		self.rcode = rcode

		self.packet = self.getBuf()
		return self.packet

	def logMsg(self):
		msg = MSG_MODBUS()
		msg.setMbMsg(self)
		msgLogger.logMsg(msg)

class ModbusData() :
	def __init__(self) :

		self.ctrlHState = 0
		self.ctrlUState = 0
		self.ctrlDState = 0

		self.ctrlHError = 0
		self.ctrlUError = 0
		self.ctrlDError = 0

		self.msgHSelect = False
		self.msgUSelect = False
		self.msgDSelect = False

		self.msgASelect = False
		self.msgMode    = 0
		self.msgContent = ""

		self.d1Dest = 0xFF
		self.d1TimeHH = 0xFF
		self.d1TimeMM = 0xFF

		self.d2Dest = 0xFF
		self.d2TimeHH = 0xFF
		self.d2TimeMM = 0xFF

		self.u1Dest = 0xFF
		self.u1TimeHH = 0xFF
		self.u1TimeMM = 0xFF

		self.u2Dest = 0xFF
		self.u2TimeHH = 0xFF
		self.u2TimeMM = 0xFF

class ModbusServer(threading.Thread) :

	def __init__(self, port=MODBUS_SVR_PORT) :
		threading.Thread.__init__(self)
		self.port = port
		self.tcpServer = None
		self.modbusData = ModbusData()

		self.station = None
		self.hallCtrl = None
		self.upCtrl = None
		self.downCtrl = None
		self.dbHelper = None

		self.isStation = PisConfig["stationServer"]

	def onReadData(self, msg):

		addr = msg.saddr
		logD("ModbusServer onReadData addr : %04x" % addr)

		if addr == MB_ADDR_STATUS_HC:
			msg.setData([self.modbusData.ctrlHState | self.modbusData.ctrlHError])
		elif addr == MB_ADDR_STATUS_UC:
			msg.setData([self.modbusData.ctrlUState | self.modbusData.ctrlUError])
		elif addr == MB_ADDR_STATUS_DC:
			msg.setData([self.modbusData.ctrlDState | self.modbusData.ctrlDError])
		else:
			msg.setData([0])
			logE("ModbusServer read, not supported addr : %04x" % addr)

	def parseUint8(self, data):
		if len(data) < 1:
			raise Exception("parseUint8 too short")
		return int(data[0])

	def parseUint16(self, data):
		if len(data) < 2:
			raise Exception("parseUint16 too short")

		return int((data[0] << 8) + data[1])

	def swapUint16(self, data):
		dlen = len(data)
		rdata = bytearray(len(data))
		i = 0
		while i + 2 <= dlen:
			rdata[i] = data[i + 1]
			rdata[i + 1] = data[i]
			i += 2
		if i < dlen:
			rdata[i] = data[i]
		return rdata

	def parseStr(self, data):
		try:
			s = data.decode("utf-8")
			return s
		except Exception as e:
			onException(e)

		try:
			s = data.decode("gbk")
			return s
		except Exception as e:
			onException(e)

		return "Invalid emergency message"

	def log(self, msg):
		logI("[MODBUS] %s" % msg)

	def onWriteData(self, msg):

		addr = msg.saddr
		data = msg.data
		self.log("onWriteData addr : %04x" % addr)
		# printHex(data)

		msg.setData([])

		modbusData = self.modbusData

		if addr == MB_ADDR_CTRL_HC:
			val = self.parseUint16(data)
			modbusData.msgHSelect = (val & 0x04) != 0
			self.log("write msgHSelect : %s" % modbusData.msgHSelect)
			if not modbusData.msgHSelect:
				self.sendEmergence(True)

		elif addr == MB_ADDR_CTRL_UC:
			val = self.parseUint16(data)
			modbusData.msgUSelect = (val & 0x04) != 0
			self.log("write msgUSelect : %s" % modbusData.msgUSelect)
			if not modbusData.msgUSelect:
				self.sendEmergence(True)

		elif addr == MB_ADDR_CTRL_DC:
			val = self.parseUint16(data)
			modbusData.msgDSelect = (val & 0x04) != 0
			self.log("write msgDSelect : %s" % modbusData.msgDSelect)
			if not modbusData.msgDSelect:
				self.sendEmergence(True)

		elif addr == MB_ADDR_MSG_SEL_ALL:
			val = self.parseUint16(data)
			modbusData.msgASelect = (val & 0x01) != 0
			self.log("write msgASelect : %s" % modbusData.msgASelect)
			if modbusData.msgASelect:
				modbusData.msgHSelect = True
				modbusData.msgUSelect = True
				modbusData.msgDSelect = True
			else:
				modbusData.msgHSelect = False
				modbusData.msgUSelect = False
				modbusData.msgDSelect = False
				self.sendEmergence(True)

		elif addr == MB_ADDR_MSG_MODE:
			val = self.parseUint16(data)
			modbusData.msgMode = (val & 0x01)
			self.log("write msgMode : %s" % modbusData.msgMode)

		elif addr == MB_ADDR_MSG_CONTENT:
			data = self.swapUint16(data)
			modbusData.msgContent = self.parseStr(data)
			self.log("write msgContent : %s" % modbusData.msgContent)
			self.sendEmergence()

		elif addr == MB_ADDR_D1_DEST:
			modbusData.d1Dest = self.parseUint16(data)
			self.log("write d1Dest : %s" % modbusData.d1Dest)

		elif addr == MB_ADDR_D1_TIME_HH:
			modbusData.d1TimeHH = self.parseUint16(data)
			self.log("write d1TimeHH : %s" % modbusData.d1TimeHH)

		elif addr == MB_ADDR_D1_TIME_MM:
			modbusData.d1TimeMM = self.parseUint16(data)
			self.log("write d1TimeMM : %s" % modbusData.d1TimeMM)
			self.sendDTime()

		elif addr == MB_ADDR_D2_DEST:
			modbusData.d2Dest = self.parseUint16(data)
			self.log("write d2Dest : %s" % modbusData.d2Dest)

		elif addr == MB_ADDR_D2_TIME_HH:
			modbusData.d2TimeHH = self.parseUint16(data)
			self.log("write d2TimeHH : %s" % modbusData.d2TimeHH)

		elif addr == MB_ADDR_D2_TIME_MM:
			modbusData.d2TimeMM = self.parseUint16(data)
			self.log("write d2TimeMM : %s" % modbusData.d2TimeMM)
			self.sendDTime()

		elif addr == MB_ADDR_U1_DEST:
			modbusData.u1Dest = self.parseUint16(data)
			self.log("write u1Dest : %s" % modbusData.u1Dest)

		elif addr == MB_ADDR_U1_TIME_HH:
			modbusData.u1TimeHH = self.parseUint16(data)
			self.log("write u1TimeHH : %s" % modbusData.u1TimeHH)

		elif addr == MB_ADDR_U1_TIME_MM:
			modbusData.u1TimeMM = self.parseUint16(data)
			self.log("write u1TimeMM : %s" % modbusData.u1TimeMM)
			self.sendUTime()

		elif addr == MB_ADDR_U2_DEST:
			modbusData.u2Dest = self.parseUint16(data)
			self.log("write u2Dest : %s" % modbusData.u2Dest)

		elif addr == MB_ADDR_U2_TIME_HH:
			modbusData.u2TimeHH = self.parseUint16(data)
			self.log("write u2TimeHH : %s" % modbusData.u2TimeHH)

		elif addr == MB_ADDR_U2_TIME_MM:
			modbusData.u2TimeMM = self.parseUint16(data)
			self.log("write u2TimeMM : %s" % modbusData.u2TimeMM)
			self.sendUTime()

		else:
			logW("ModbusServer write, not supported addr : %04x" % addr)

	def handle(self, msg):
		msg.logMsg()

		if msg.fcode == 0x03:
			self.onReadData(msg)
			ret = msg.response()
		elif msg.fcode == 0x10:
			try:
				self.onWriteData(msg)
			except Exception as e:
				onException(e)
			ret = msg.response()
		else:
			ret = msg.response(error=True)

		if ret:
			msg.logMsg()

		return ret

	def send(self, addr, data):
		try:
			self.log("tx data : %s" % (str(addr[0])))
			_printHex(data)
			addr[2].send(data)
		except Exception as e:
			onException(e)

	def onCallback(self, _type, addr, data):

		if _type == "data":
			self.log("rx data : %s" % (str(addr[0])))
			_printHex(data)

			msg = ModbusMsg()
			msg.setBuf(data)
			while True:
				r = msg.parse(addr)
				if not r:
					break
				#msg.toString()
				res = self.handle(msg)
				if res:
					self.send(addr, res)
				else:
					logW("ModbusServer handle res None")

		elif _type == "conn":
			self.log("conn : %s" % (str(addr)))

	def sendDTime(self):
		downCtrl = self.downCtrl
		if not downCtrl:
			return

		mbd = self.modbusData
		stationTime = {
			"s1Dest"   : mbd.d1Dest,
			"s1TimeHH" : mbd.d1TimeHH,
			"s1TimeMM" : mbd.d1TimeMM,
			"s2Dest"   : mbd.d2Dest,
			"s2TimeHH" : mbd.d2TimeHH,
			"s2TimeMM" : mbd.d2TimeMM,
		}

		logI("######### send down line station time:")
		logI(stationTime)

		self.ctrlSendStationTime([downCtrl], stationTime)

	def sendUTime(self):
		upCtrl = self.upCtrl
		if not upCtrl:
			return

		mbd = self.modbusData
		stationTime = {
			"s1Dest"   : mbd.u1Dest,
			"s1TimeHH" : mbd.u1TimeHH,
			"s1TimeMM" : mbd.u1TimeMM,
			"s2Dest"   : mbd.u2Dest,
			"s2TimeHH" : mbd.u2TimeHH,
			"s2TimeMM" : mbd.u2TimeMM,
		}
		logI("######### send up line station time:")
		logI(stationTime)

		self.ctrlSendStationTime([upCtrl], stationTime)

	def sendEmergence(self, clean = False):

		if not clean:
			mbd = self.modbusData
			msgInfo = {
				"content"   : mbd.msgContent,
				"mode" : mbd.msgMode,
				"lifeTime" : 0,
				"id" : None,
				"operation_id1" : mbd.msgMode
			}
			if self.hallCtrl and mbd.msgHSelect:
				logI("######### send hall ctrl emergence: %s" % mbd.msgContent)
				self.ctrlSendEmergence([self.hallCtrl], msgInfo)
			if self.upCtrl and mbd.msgUSelect:
				logI("######### send up ctrl emergence: %s" % mbd.msgContent)
				self.ctrlSendEmergence([self.upCtrl], msgInfo)
			if self.downCtrl and mbd.msgDSelect:
				logI("######### send down ctrl emergence: %s" % mbd.msgContent)
				self.ctrlSendEmergence([self.downCtrl], msgInfo)
		else:
			msgInfo = {
				"content"   : "",
				"id" : None
			}
			if self.hallCtrl and not mbd.msgHSelect:
				logI("######### clear hall ctrl emergence: %s" % mbd.msgContent)
				self.ctrlSendEmergence([self.hallCtrl], msgInfo)
			if self.upCtrl and not mbd.msgUSelect:
				logI("######### clear up ctrl emergence: %s" % mbd.msgContent)
				self.ctrlSendEmergence([self.upCtrl], msgInfo)
			if self.downCtrl and not mbd.msgDSelect:
				logI("######### clear down ctrl emergence: %s" % mbd.msgContent)
				self.ctrlSendEmergence([self.downCtrl], msgInfo)

	def initStation(self):

		if not self.dbHelper:
			self.dbHelper = DbHelper()

		dbh = self.dbHelper
		try:
			db = dbh.getDB("pis.station")
			code = PisConfig["stationCode"]
			result = db.findByCond(db.getFields(), "code='%s'" % code)
			if not len(result["rows"]):
				return

			keys = result["struct"].split(",")
			station = dict(zip(keys, result["rows"][0]))

			stationId = station["id"]
			stationName = station["name"]

			hallCtrl = None
			upCtrl = None
			downCtrl = None

			db = dbh.getDB("pis.controller")
			fields = db.getFields()
			keys = fields.split(",")

			result = db.findByCond(fields, "install_id=%d and install_type='%s'" % (stationId, INSTALL_TYPE_HALL))
			if len(result["rows"]):
				hallCtrl = dict(zip(keys, result["rows"][0]))

			result = db.findByCond(fields, "install_id=%d and install_type='%s'" % (stationId, INSTALL_TYPE_UP))
			if len(result["rows"]):
				upCtrl = dict(zip(keys, result["rows"][0]))

			result = db.findByCond(fields, "install_id=%d and install_type='%s'" % (stationId, INSTALL_TYPE_DOWN))
			if len(result["rows"]):
				downCtrl = dict(zip(keys, result["rows"][0]))

			self.station = station
			self.hallCtrl = hallCtrl
			self.upCtrl = upCtrl
			self.downCtrl = downCtrl

		except Exception as e:
			onException(e)

		dbh.closeDB()

	def run(self):
		# logI("ModbusServer start, port : %s" % self.port)

		from .controller import sendEmergence,sendStationTime

		self.ctrlSendEmergence = sendEmergence
		self.ctrlSendStationTime = sendStationTime

		self.tcpServer = TcpServer(self.onCallback, self.port)
		self.tcpServer.start()

		while True:
			if self.isStation:
				self.initStation()
			time.sleep(MODBUS_SVR_TICK)

if USING_MSGER:
	ModbusServer().start()

