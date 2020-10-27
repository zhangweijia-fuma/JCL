#coding:utf-8
import sys, struct, time, threading, random, datetime
from utils.utils import *
from utils.debug import *
from .dataBuf import *
from .consts import *

from libs.db.dbHelper import *
from libs.utils.debug import *


# 消息类型定义
HeartBeatMsg  			= 0
HeartBeatAck			= 1
SendProgramListMsg  	= 2
SendProgramListAck		= 3
SendNewsMsg				= 4	
SendNewsAck				= 5
ClearNewsMsg			= 6  
ClearNewsAck			= 7
SendEmergenceMsg		= 8
SendEmergenceAck		= 9
ClearEmergenceMsg	  	= 10
ClearEmergenceAck	  	= 11
SendLineInfoMsg			= 12
SendLineInfoAck		  	= 13
SendStationTimeMsg	  	= 14
SendStationTimeAck		= 15
SendFormatFileMsg		= 16
SendFormatFileAck	  	= 17
SystemRebootMsg			= 18
SystemRebootAck		  	= 19
RunStatusMsg			= 20
RunStatusAck			= 21
RunLogMsg				= 22	
RunLogAck				= 23  
SendUpdateMsg			= 24
SendUpdateAck			= 25
InitialRequestMsg		= 26
InitialRequestAck	  	= 27
UpdateWorkTimeMsg		= 28
UpdateWorkTimeAck	  	= 29
ViewControllerVideoMsg 	= 30
ViewControllerVideoAck  = 31
SendSafeVideoMsg		= 32
SendSafeVideoAck		= 33

# 控制器在下载视频文件完成后，需要向服务器报告下载已经完成
DownloadCompleteMsg 	= 34
DownloadCompleteAck 	= 35


# 需要接收下载完成消息的消息列表
msgForDownloadList = [ SendProgramListMsg, SendSafeVideoAck ]


MSG_TYPES = {}

def msgtype(_type, tcp=False, ack=False):
	def register(f):
		# logD("Register msg type:0x%02x tcp:%s" % (_type, str(tcp)))
		# print(f.__name__)
		MSG_TYPES[_type] = f
		f.msgtype = _type
		f.tcp = tcp
		f.ack = ack
		return f
	return register

class BaseMsg(DataBuf) :
	def __init__(self) :
		# logD("BaseMsg init")
		DataBuf.init(self)

		self.sid 		= None 											# 消息序号
		self.tx 		= True											# 发送请求消息标识
		self.next 		= None
		self.timeout 	= 0
		self.callback 	= 0
		self.addr 		= "" 											# 发送方地址
		self.oid 		= None

		t 				= type(self)
		self.tcp 		= t.tcp
		self.ack 		= t.ack
		self.type 		= t.msgtype

	def isTcp(self) :
		return self.tcp

	def isAck(self) :
		return self.ack

	def setTx(self, tx):
		self.tx = tx

	def initAck(self, resent = False):
		if resent:
			self.resent += 1
		else:
			self.resent = 0

		self.timeout = time.time() + ACK_TIMEOUT

	def setCallback(self, callback):
		self.callback = callback

	def setAckMsg(self, ackMsg):
		self.ackMsg = ackMsg
		if self.callback:
			result = True if ackMsg else False
			self.callback(self, result)

	def isAckTimeout(self):
		return time.time() >= self.timeout

	def setAddr(self, addr):
		self.addr = addr

	def setController(self, c):
		self.controller = c
		self.addr = c.data["ip_address"]

	#decapsulation, decap
	def decode(self, data):
		self.data = data

	#encapsulation, encap
	def encode(self):
		return b""

	def getMsgType(self):
		return self.type

	def getAckType(self):
		if self.ack:
			logE("no ack type for a ack msg")
			return 0
		return self.type + 1

	def getAckedType(self):
		if not self.ack:
			logE("no acked type for a data msg")
			return 0
		return self.type - 1

	def getMsgTypeStr(self):
		dbHelper = DbHelper(True)

		try:
			cur = dbHelper.getCursor()

			sql = "select cv.description"
			sql += " from system.code_value cv where cv.type_code = 'MESSAGE_TYPE' and cv.code = '%s' and cv.status = '1' "%self.type
			cur.execute(sql)
			rows = cur.fetchall()
			dbHelper.closeDB()
			if len(rows) == 0:
				return "unknown message type: %s"%self.type
			else:
				return rows[0][0]
		except Exception as e:
			dbHelper.closeDB()
			onException(e)
			return "ERROR"

	def setOid(self, oid, oid1 = None, oid2 = None):
		self.oid 	= oid
		self.oid1 	= oid1
		self.oid2 	= oid2

	def getTypeName(self):
		return type(self).__name__

	def toString(self):
		return "n/a"

	def setController(self, controller):
		self.controller = controller
		self.addr = controller["ip_address"]

class BASE_ACK(BaseMsg) :
	def __init__(self) :
		BaseMsg.__init__(self)
		# logD("ACK init")
		self.ack_type 	= 1
		self.status  	= 2

	def setParams(self, ack_type, status):
		self.ack_type 	= ack_type
		self.status 	= status

	def encode(self):
		self.setBuf(bytearray(8))
		self.writeUint32(self.ack_type)
		self.writeUint32(self.status)
		return self.getBuf()

	def decode(self, data) :
		self.setBuf(data)
		self.ack_type 	= self.readUint32()
		self.status 	= self.readUint32()

	def toString(self):
		return "msg type: %s, ack_type : %s， status : %s" % ( self.getMsgTypeStr(), self.ack_type, self.status)

@msgtype(0x00)
class HEARTBEAT(BaseMsg) :
	def __init__(self) :
		BaseMsg.__init__(self)
		# logD("HEARTBEAT init")

@msgtype(0x01, ack=True)
class HEARTBEAT_ACK(BaseMsg) :
	def __init__(self) :
		BaseMsg.__init__(self)
		# logD("HEARTBEAT_ACK init")

@msgtype(0x02)
class SEND_PROGRAM_LIST(BaseMsg) :
	def __init__(self) :
		BaseMsg.__init__(self)
		# logD("SEND_PROGRAM_LIST init")
		self.programe_file = ""
		# self.format_file1 = ""
		# self.format_file2 = ""
		# self.format_switch_time = 0

	def setParams(self, programe_file):
		self.programe_file = programe_file
		# self.format_file1 = format_file1
		# self.format_file2 = format_file2
		# self.format_switch_time = format_switch_time

	def encode(self):
		self.setBuf(bytearray(128))
		self.writeStr(self.programe_file, 128)
		# self.writeStr(self.format_file1, 128)
		# self.writeStr(self.format_file2, 128)
		# self.writeUint32(self.format_switch_time)
		return self.getBuf()

	def toString(self):
		return "发送节目单: %s" % (self.programe_file)

@msgtype(0x03, ack=True)
class SEND_PROGRAM_LIST_ACK(BASE_ACK) :
	pass

"""
1	LIFE_TIME	U32	4	生命期，单位：秒	等于0表示一直显示。否则生命期结束此消息被清掉。该参数默认为0。
3	CONTENT	STR	1024	最大为1024字节	
"""

@msgtype(0x04)
class SEND_NEWS(BaseMsg) :
	def __init__(self) :
		BaseMsg.__init__(self)
		# logD("SEND_NEWS init")
		self.content 	= ""
		self.life_time 	= 0

	def setParams(self, life_time, content):
		self.life_time 	= life_time
		self.content 	= content

	def encode(self):
		self.setBuf(bytearray(4 + 2048))
		self.writeUint32(self.life_time)
		self.writeStr(self.content, 2048)
		return self.getBuf()

	def toString(self):
		return "新闻内容: %s，有效时长: %s" % (self.content, self.life_time)

@msgtype(0x05, ack=True)
class SEND_NEWS_ACK(BASE_ACK) :
	pass

@msgtype(0x06)
class CLEAR_NEWS(BaseMsg) :
	def __init__(self) :
		BaseMsg.__init__(self)
		# logD("CLEAR_NEWS init")

	def toString(self):
		return "清除新闻内容"

@msgtype(0x07, ack=True)
class CLEAR_NEWS_ACK(BASE_ACK) :
	pass

@msgtype(0x08)
class SEND_EMERGENCE(BaseMsg) :
	def __init__(self) :
		BaseMsg.__init__(self)
		# logD("SEND_NEWS init")
		self.content 	= ""
		self.life_time 	= 0
		self.mode 		= 0

	def setParams(self, life_time, mode, content):
		self.mode 		= mode
		self.life_time 	= life_time
		self.content 	= content

	def encode(self):
		self.setBuf(bytearray(4 + 4 + 1024))
		self.writeUint32(self.life_time)
		self.writeUint32(self.mode)
		self.writeStr(self.content, 1024)
		return self.getBuf()

	def toString(self):
		if self.mode == 0:
			modeName = "滚动"
		else:
			modeName = "全屏"
		return "模式: %s，通知内容: %s，有效时长: %s" % ( modeName, self.content, self.life_time)

@msgtype(0x09, ack=True)
class SEND_EMERGENCE_ACK(BASE_ACK) :
	pass

@msgtype(0x0A)
class CLEAR_EMERGENCE(BaseMsg) :
	def __init__(self) :
		BaseMsg.__init__(self)
		# logD("CLEAR_NEWS init")

	def toString(self):
		return "清除紧急通知内容"

@msgtype(0x0B, ack=True)
class CLEAR_EMERGENCE_ACK(BASE_ACK) :
	pass

"""
1	FIRST_STATION	STR	256B		始发站中文名称
2	FIIRST_STATION_EN	STR	256B		始发站英文名称
3	END_STATION	STR	256B		终点站中文名称
4	END_STATION_EN	STR	256B		终点站英文名称
5	CURRENT_STATION	STR	256B		当前站中文名称
6	CURRENT_STATION_EN	STR	256B		当前站英文名称
7	NEXT_STATION	STR	256B		下一站中文名称
8	NEXT_STATION_EN	STR	256B		下一站英文名称
9	START_TIME_HH	U8	1B	时	首班车车时间
10	START_TIME_MM	U8	1B	分	首班车车时间
11	END_TIME_HH	U8	1B	时	末班车时间
12	END_TIME_MM	U8	1B	分	末班车时间
"""
@msgtype(0x0C)
class SEND_LINE_INFO(BaseMsg) :
	def __init__(self) :
		BaseMsg.__init__(self)
		# logD("SEND_LINE_INFO init")

	def setParams(self, lineInfo):

		self.first_station 		= lineinfo["first_station"]
		self.first_station_en 	= lineinfo["first_station_en"]
		self.end_station 		= lineinfo["end_station"]
		self.end_station_en 	= lineinfo["end_station_en"]
		self.current_station 	= lineinfo["current_station"]
		self.current_station_en = lineinfo["current_station_en"]
		self.next_station 		= lineinfo["next_station"]
		self.next_station_en 	= lineinfo["next_station_en"]

		stime = lineinfo["start_time"]
		ss = stime.split(":")
		self.start_time_hh 		= parseInt(ss[0])
		self.start_time_mm 		= parseInt(ss[1])

		etime = lineinfo["end_time"]
		ss = etime.split(":")
		self.end_time_hh 		= parseInt(ss[0])
		self.end_time_mm 		= parseInt(ss[1])

	def encode(self):
		self.setBuf(bytearray(8 * 256 + 4))
		self.writeStr(self.first_station, 256)
		self.writeStr(self.first_station_en, 256)
		self.writeStr(self.end_station, 256)
		self.writeStr(self.end_station_en, 256)
		self.writeStr(self.current_station, 256)
		self.writeStr(self.current_station_en, 256)
		self.writeStr(self.next_station, 256)
		self.writeStr(self.next_station_en, 256)
		self.writeUint8(self.start_time_hh)
		self.writeUint8(self.start_time_mm)
		self.writeUint8(self.end_time_hh)
		self.writeUint8(self.end_time_mm)
		return self.getBuf()

	def toString(self):
		return "first_station: %s，first_station_en: %s，end_station: %s，end_station_en: %s，current_station: %s，current_station_en: %s，next_station: %s，next_station_en: %s，start_time: %02d:%02d，end_time: %02d:%02d" % (self.first_station, self.first_station_en, self.end_station, self.end_station_en, self.current_station, self.current_station_en, self.next_station, self.next_station_en, self.start_time_hh, self.start_time_mm, self.end_time_hh, self.end_time_mm)

@msgtype(0x0D, ack=True)
class SEND_LINE_INFO_ACK(BASE_ACK) :
	pass

"""
1	CURRENT_LEFT_TIME_MM	U8	1	0~99	到站时间，分
2	CURRENT_LEFT_TIME_SS	U8	1	0~59	到站时间，秒
3	NEXT_LEFT_TIME_MM	U8	1	0~99	下一班到站时间，分
4	NEXT_LEFT_TIME_SS	U8	1	0~59	下一班到站时间，秒
"""
@msgtype(0x0E)
class SEND_STATION_TIME(BaseMsg) :
	def __init__(self) :
		BaseMsg.__init__(self)
		# logD("SEND_STATION_TIME init")

	def setParams(self, stationTime):
		print(stationTime)

		self.current_dest 			= stationTime["s1Dest"]
		self.current_left_time_hh 	= stationTime["s1TimeHH"]
		self.current_left_time_mm 	= stationTime["s1TimeMM"]
		self.current_left_time_ss 	= 0

		self.next_dest = stationTime["s2Dest"]
		self.next_left_time_hh 		= stationTime["s2TimeHH"]
		self.next_left_time_mm 		= stationTime["s2TimeMM"]
		self.next_left_time_ss 		= 0

	def encode(self):
		self.setBuf(bytearray(7 * 2))
		self.writeUint8(self.current_left_time_hh)
		self.writeUint8(self.current_left_time_mm)
		self.writeUint8(self.current_left_time_ss)
		self.writeUint32(self.current_dest)

		self.writeUint8(self.next_left_time_hh)
		self.writeUint8(self.next_left_time_mm)
		self.writeUint8(self.next_left_time_ss)
		self.writeUint32(self.next_dest)
		return self.getBuf()

	def toString(self):
		return "current_left_time : %02d:%02d:%02d， next_left_time : %02d:%02d:%02d" % (
			self.current_left_time_hh, self.current_left_time_mm, self.current_left_time_ss, 
			self.next_left_time_hh, self.next_left_time_mm, self.next_left_time_ss)

@msgtype(0x0F, ack=True)
class SEND_STATION_TIME_ACK(BASE_ACK) :
	pass

@msgtype(0x10)
class SEND_FORMAT_FILE(BaseMsg) :
	def __init__(self) :
		BaseMsg.__init__(self)
		# logD("SEND_FORMAT_FILE init")
		self.format_file1 = ""
		self.format_file2 = ""
		self.format_switch_time = 0

	def setParams(self, format_file1, format_file2, format_switch_time):
		self.format_file1 		= format_file1
		self.format_file2 		= format_file2
		self.format_switch_time = format_switch_time

	def encode(self):
		self.setBuf(bytearray(128 * 2 + 4))
		self.writeStr(self.format_file1, 128)
		self.writeStr(self.format_file2, 128)
		self.writeUint32(self.format_switch_time)
		return self.getBuf()

	def toString(self):
		return "版式文件1: %s，版式文件2: %s，切换时间: %s" % (self.format_file1, self.format_file2, self.format_switch_time)

@msgtype(0x11, ack=True)
class SEND_FORMAT_FILE_ACK(BASE_ACK) :
	pass

@msgtype(0x12, tcp=False)
class SYSTEM_REBOOT(BaseMsg) :
	def __init__(self) :
		BaseMsg.__init__(self)
		# logD("SYSTEM_REBOOT init")
		self.delay_second = 0

	def setParams(self, delay_second):
		self.delay_second = delay_second

	def encode(self):
		self.setBuf(bytearray(4))
		self.writeUint32(self.delay_second)
		return self.getBuf()

	def toString(self):
		return "重启延迟时间: %s" % (self.delay_second)

@msgtype(0x13, ack=True, tcp=False)
class SYSTEM_REBOOT_ACK(BASE_ACK) :
	pass

"""
1	CPU			U32	4	控制器CPU占有率	百分比值 * 100
2	MEMORY		U32	4	内存占有率	百分比值 * 100
3	HARDDISK	U32	4	硬盘占有率	百分比值 * 100
4	STATUS		U32	4	0-正常；其他为故障编码	
"""

@msgtype(0x14, tcp=False)
class RUN_STATUS(BaseMsg) :
	def __init__(self) :
		BaseMsg.__init__(self)
		# logD("RUN_STATUS init")

	def setParams(self, cpu, memory, harddisk, status):
		self.cpu 		= cpu
		self.memory 	= memory
		self.harddisk 	= harddisk
		self.status 	= status

	def encode(self):
		self.setBuf(bytearray(4 * 4))
		self.writeUint32(self.cpu)
		self.writeUint32(self.memory)
		self.writeUint32(self.harddisk)
		self.writeUint32(self.status)
		return self.getBuf()

	def decode(self, data) :
		self.setBuf(data)
		self.cpu 		= self.readUint32()
		self.memory 	= self.readUint32()
		self.harddisk 	= self.readUint32()
		self.status 	= self.readUint32()

	def toString(self):
		return "CPU: %0.2f%%, 内存: %0.2f%%, 硬盘: %0.2f%%" % (self.cpu/100, self.memory/100, self.harddisk/100)

@msgtype(0x15, ack=True, tcp=False)
class RUN_STATUS_ACK(BASE_ACK) :
	pass

"""
1	ID	U32	4		日志ID，每天的日志从1开始递增
2	LEVEL	U16	2		日志级别
3	TYPE	U16	2		日志类型
4	TITLE	STR	64		日志标题
5	DESC	STR	512		日志描述
6	YEAR	U16	2B		日志时间：年
7	MONTH	U8	1B		日志时间：月
8	DAY		U8	1B		日志时间：日
9	HOUR	U8	1B		日志时间：时
10	MINUTE	U8	1B		日志时间：分
11	SECOND	U8	1B		日志时间：秒
12	RESERVED	U8	1B		保留
"""

testLogId = 0

@msgtype(0x16, tcp=False)
class RUN_LOG(BaseMsg) :
	def __init__(self) :
		BaseMsg.__init__(self)
		# logD("RUN_LOG init")

	def random(self):
		global testLogId
		testLogId = testLogId + 1
		time = datetime.datetime.now()

		self.id 	 = testLogId
		self.level 	 = random.randint(1, 4)
		self._type 	 = 1 #random.randint(0, 10)
		self.title 	 = randomStr(20)
		self.desc 	 = randomStr(50)
		self.year	 = time.year
		self.month	 = time.month
		self.day	 = time.day
		self.hour	 = time.hour
		self.minute	 = time.minute
		self.second	 = time.second
		self.reserved = 0

	def encode(self):
		self.setBuf(bytearray(64 + 512 + 4 + 2 * 3 + 6))
		self.writeUint32(self.id)
		self.writeUint16(self.level)
		self.writeUint16(self._type)

		self.writeStr(self.title, 64)
		self.writeStr(self.desc, 512)

		self.writeUint16(self.year)
		self.writeUint8(self.month)
		self.writeUint8(self.day)
		self.writeUint8(self.hour)
		self.writeUint8(self.minute)
		self.writeUint8(self.second)
		self.writeUint8(self.reserved)

		return self.getBuf()

	def decode(self, data) :
		self.setBuf(data)
		self.id 	 = self.readUint32()
		self.level 	 = self.readUint16()
		self._type 	 = self.readUint16()
		self.title 	 = self.readStr(64)
		self.desc 	 = self.readStr(512)
		self.year	 = self.readUint16()
		self.month	 = self.readUint8()
		self.day	 = self.readUint8()
		self.hour	 = self.readUint8()
		self.minute	 = self.readUint8()
		self.second	 = self.readUint8()
		self.reserved = self.readUint8()

	def toString(self):
		return "级别: %s，类型: %s，日期: %d.%02d.%02d，时间: %02d:%02d:%02d，标题: %s，描述: %s" % (self.level, self._type, self.year, self.month, self.day, self.hour, self.minute, self.second, self.title, self.desc)

@msgtype(0x17, ack=True, tcp=False)
class RUN_LOG_ACK(BASE_ACK) :
	pass

"""
1	FILE_PATH	STR	128		升级文件路径
"""
@msgtype(0x18)
class SEND_UPDATE(BaseMsg) :
	def __init__(self) :
		BaseMsg.__init__(self)
		# logD("SEND_UPDATE init")
		self.file_path = ""

	def setParams(self, file_path):
		self.file_path = file_path

	def encode(self):
		self.setBuf(bytearray(128))
		self.writeStr(self.file_path, 128)
		return self.getBuf()

	def toString(self):
		return "版本文件路径: %s" % (self.file_path)

@msgtype(0x19, ack=True)
class SEND_UPDATE_ACK(BASE_ACK) :
	pass


"""
1	SCREEN_PATH			STR	128		抓屏根路径
2	LOG_PATH			STR	128		日志根路径
3	DEVICE_NO			STR	16		设备编号
4	SNATCH_SCREEN_INTERVAL	U32	4		抓屏间隔，秒
"""
@msgtype(0x1A)
class INITIAL_REQUEST(BaseMsg) :
	def __init__(self) :
		BaseMsg.__init__(self)
		# logD("INITIAL_REQUEST init")

	def toString(self):
		return "控制器初始化请求"

@msgtype(0x1B, ack=True)
class INITIAL_REQUEST_ACK(BaseMsg) :

	def setParams(self, screen_path, log_path, device_no, snatch_screen_interval, lineInfo):
		self.screen_path 	= screen_path
		self.log_path 		= log_path
		self.device_no 		= str(device_no)
		self.snatch_screen_interval = snatch_screen_interval
		self.setParamsLi(lineInfo)

	def encode(self):
		self.setBuf(bytearray(128 * 2 + 16 + 4) + bytearray(8 * 256 + 4 + 1 + 4))
		self.writeStr(self.screen_path, 128)
		self.writeStr(self.log_path, 128)
		self.writeStr(self.device_no, 16)
		self.writeUint32(self.snatch_screen_interval)
		return self.encodeLi()

	def toString(self):
		return "截屏路径: %s，截屏间隔: %s，日志路径: %s，设备编号: %s" % (self.screen_path, self.snatch_screen_interval, self.log_path, self.device_no ) + ", " + self.toStringLi()

	def setParamsLi(self, lineinfo):
		self.first_station 		= lineinfo["first_station"]
		self.first_station_en 	= lineinfo["first_station_en"]
		self.end_station 		= lineinfo["end_station"]
		self.end_station_en 	= lineinfo["end_station_en"]
		self.current_station 	= lineinfo["current_station"]
		self.current_station_en = lineinfo["current_station_en"]
		self.next_station 		= lineinfo["next_station"]
		self.next_station_en 	= lineinfo["next_station_en"]

		stime = lineinfo["start_time"]
		ss = stime.split(":")
		self.start_time_hh 		= parseInt(ss[0])
		self.start_time_mm 		= parseInt(ss[1])

		etime = lineinfo["end_time"]
		ss = etime.split(":")
		self.end_time_hh 		= parseInt(ss[0])
		self.end_time_mm 		= parseInt(ss[1])

		# controller type: 0: 车头，1：车尾，2：上行，3：下行，4：站厅
		self.controller_type = lineinfo['controller_type']

		# 控制器休眠唤醒时间
		open_time = lineinfo['open_time']
		ss = open_time.split(":")
		self.open_time_hh 		= parseInt(ss[0])
		self.open_time_mm 		= parseInt(ss[1])

		close_time = lineinfo['close_time']
		ss = close_time.split(":")
		self.close_time_hh 		= parseInt(ss[0])
		self.close_time_mm 		= parseInt(ss[1])

	def encodeLi(self):
		#self.setBuf(bytearray(8 * 256 + 4))
		self.writeStr(self.first_station, 256)
		self.writeStr(self.first_station_en, 256)
		self.writeStr(self.end_station, 256)
		self.writeStr(self.end_station_en, 256)
		self.writeStr(self.current_station, 256)
		self.writeStr(self.current_station_en, 256)
		self.writeStr(self.next_station, 256)
		self.writeStr(self.next_station_en, 256)
		self.writeUint8(self.start_time_hh)
		self.writeUint8(self.start_time_mm)
		self.writeUint8(self.end_time_hh)
		self.writeUint8(self.end_time_mm)

		# controller_type
		self.writeUint8( int(self.controller_type))
		
		self.writeUint8(self.open_time_hh)
		self.writeUint8(self.open_time_mm)
		self.writeUint8(self.close_time_hh)
		self.writeUint8(self.close_time_mm)

		return self.getBuf()

	def toStringLi(self):
		return "始发站 : %s，%s，终点站: %s，%s，当前站: %s，%s，下一站: %s，: %s，首班时间: %02d:%02d，末班时间: %02d:%02d" % (self.first_station, self.first_station_en, self.end_station, self.end_station_en, self.current_station, self.current_station_en, self.next_station, self.next_station_en, self.start_time_hh, self.start_time_mm, self.end_time_hh, self.end_time_mm)

"""
1	open_time
2 	close_time
"""
@msgtype(0x1C)
class UPDATE_WORK_TIME(BaseMsg) :
	def __init__(self) :
		BaseMsg.__init__(self)
		# logD("UPDATE_WORK_TIME init")
		self.file_path = ""

	def setParams(self, start_time, end_time):
		ss1 = start_time.split(":")
		ss2 = end_time.split(":")

		self.start_time_hh 	= parseInt(ss1[0])
		self.start_time_mm 	= parseInt(ss1[1])
		self.end_time_hh 	= parseInt(ss2[0])
		self.end_time_mm 	= parseInt(ss2[1])

	def encode(self):
		self.setBuf(bytearray(4))
		self.writeUint8(self.start_time_hh)
		self.writeUint8(self.start_time_mm)
		self.writeUint8(self.end_time_hh)
		self.writeUint8(self.end_time_mm)
		return self.getBuf()

	def toString(self):
		return "开机时间: %02d:%02d，休眠时间: %02d:%02d" % (self.start_time_hh, self.start_time_mm, self.end_time_hh, self.end_time_mm)

@msgtype(0x1D, ack=True)
class UPDATE_WORK_TIME_ACK(BASE_ACK) :
	pass

@msgtype(0x1E)
class VIEW_CONTROLLER_VIDEO(BaseMsg):
	def __init__(self) :
		BaseMsg.__init__(self)
		self.start_end_flag = 0

	def setParams(self, start_end_flag):
		self.start_end_flag = start_end_flag

	def encode(self):
		self.setBuf(bytearray(1))
		self.writeUint8(self.start_end_flag)
		return self.getBuf()

	def toString(self):
		return "查看控制器实时视频：%s" % ( "开始查看" if (self.start_end_flag == 1) else "结束查看" )


@msgtype(0x1F, ack=True)
class VIEW_CONTROLLER_VIDEO_ACK(BASE_ACK) :
	pass

@msgtype(0x20)
class SEND_SAFE_FILE(BaseMsg):
	def __init__(self) :
		BaseMsg.__init__(self)
		self.file_path = ""

	def setParams(self, filePath):
		self.file_path = filePath

	def encode(self):
		self.setBuf(bytearray(128))
		self.writeStr(self.file_path, 128)
		return self.getBuf()

	def toString(self):
		return "发送安全垫片视频：%s"%self.file_path


@msgtype(0x21, ack=True)
class SEND_SAFE_FILE_ACK(BASE_ACK) :
	pass

@msgtype(0x22)
class DOWNLOAD_COMPLETE_MSG(BaseMsg):
	def __init__(self) :
		BaseMsg.__init__(self)
		self.requestMsgType = None

	def setParams(self, requestMsgType):
		self.requestMsgType = requestMsgType

	def encode(self):
		self.setBuf(bytearray(4))
		self.writeUint32(self.requestMsgType, 4)
		return self.getBuf()

	def decode(self, data) :
		self.setBuf(data)
		self.requestMsgType = self.readUint32()

	def encode(self):
		self.setBuf(bytearray(4))
		self.writeUint32(self.requestMsgType, 4)
		return self.getBuf()

	def toString(self):
		return ""

@msgtype(0x23, ack=True)
class DOWNLOAD_COMPLETE_ACK(BASE_ACK) :
	pass



@msgtype(0xFF, ack=True)
class MSG_UNACKED(BaseMsg) :
	def __init__(self) :
		BaseMsg.__init__(self)
		# logD("MSG_UNACKED init")
		self.tx = False

@msgtype(0x100)
class MSG_MODBUS(BaseMsg) :
	def __init__(self) :
		BaseMsg.__init__(self)
		# logD("MSG_MODBUS init")
		self.tx 		= False
		self.strMsg 	= ""
		self.setOid(None)

	def setMbMsg(self, mbMsg):
		self.packet 	= mbMsg.packet
		if mbMsg.ack:
			self.tx 	= True
			self.type 	= 0x100
		else:
			self.tx 	= False
			self.type 	= mbMsg.saddr | 0x100

		self.strMsg 	= mbMsg.toString()

		self.setController({
			"ip_address" : mbMsg.addr,
			"id" : 0,
			"install_type" :  "8",
			"name" : "综合监控系统",
			"code" : "AAAAAA",
			"fix_name" : True
		})

		logI("MSG_MODBUS : %s" % self.strMsg)

	def toString(self):
		return self.strMsg


#####################################################################
#####################################################################

def test() :
	logD("HelloMsg.msgtype   : " + str(HelloMsg.msgtype))
	logD("ReqTimeMsg.msgtype : " + str(ReqTimeMsg.msgtype))
	obj = HelloMsg()
	logD("obj.getCmdType : " + str(obj.getMsgType()))
	obj = ReqTimeMsg()
	logD("obj.getCmdType : " + str(obj.getMsgType()))

@msgtype(0xFF)
class InternalMsg(BaseMsg) :
	def __init__(self, type, data) :
		# logD("InternalMsg init")
		self.type = type
		self.data = data

# test()
