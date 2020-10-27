#coding:utf-8

import sys, struct, time, threading, random, datetime
from utils.utils import *

from config import PisConfig
from libs.db.dbHelper import *
from libs.utils.debug import *
from libs.JCL.JCLCommon import *

from pis.PISMessage.PISMessageBase import *

# 消息类型定义, 消息定义规则：响应消息的ID为请求消息ID + 1
HeartBeatMsg  			= 0 											# 心跳消息
HeartBeatAck			= 1 											# 心跳响应消息
SendProgramListMsg  	= 2 											# 发送节目单
SendProgramListAck		= 3 											# 发送节目单响应
SendNewsMsg				= 4	 											# 发送新闻
SendNewsAck				= 5 											# 发送新闻响应
# ClearNewsMsg			= 6   											# 清除新闻 此消息被废弃
# ClearNewsAck			= 7 											# 清除新闻响应
SendEmergenceMsg		= 8 											# 发送紧急通知
SendEmergenceAck		= 9 											# 发送紧急通知响应
# ClearEmergenceMsg	  	= 10 											# 清除紧急通知，此消息被废弃
# ClearEmergenceAck	  	= 11 											# 清除紧急通知响应
SendLineInfoMsg			= 12 											# 发送运营线路基本信息
SendLineInfoAck		  	= 13 											# 发送运营线路基本信息响应
SendStationTimeMsg	  	= 14 											# 发送到站信息
SendStationTimeAck		= 15 											# 发送到站信息响应
SendFormatFileMsg		= 16 											# 发送版式文件
SendFormatFileAck	  	= 17 											# 发送版式文件响应
SystemRebootMsg			= 18 											# 系统重启
SystemRebootAck		  	= 19 											# 系统重启响应
RunStatusMsg			= 20 											# 控制器运行状态报告
RunStatusAck			= 21 											# 运行报告响应
RunLogMsg				= 22	 										# 控制器日志报告
RunLogAck				= 23 											# 控制器日志报告响应
SendUpdateMsg			= 24 											# 控制器软件更新
SendUpdateAck			= 25 											# 控制器软件更新响应
InitialRequestMsg		= 26 											# 控制器初始化请求
InitialRequestAck	  	= 27 											# 控制器初始化请求响应
UpdateWorkTimeMsg		= 28 											# 更新控制器工作时间
UpdateWorkTimeAck	  	= 29 											# 更新控制器工作时间响应
ViewControllerVideoMsg 	= 30 											# 查看控制器视频
ViewControllerVideoAck  = 31 											# 查看控制器视频响应
SendSafeVideoMsg		= 32 											# 发送安全垫片
SendSafeVideoAck		= 33 											# 发送安全垫片响应
DownloadCompleteMsg 	= 34 											# 控制器在下载视频文件完成后，需要向服务器报告下载已经完成
DownloadCompleteAck 	= 35 											# 下载完成响应

UnAckedMsg 				= 0xFF 											# 未使用
ModbusMsg 				= 0x100 										# Modbus 消息

# 以下5条消息为内部消息，不会发送给控制器
ModbusStationTimeMsg 	= 0x101 										# 仅用于消息定义
ModbusDownTimeMsg 		= 0x102 										# Modbus 下行到站时间消息
ModbusDownNextTimeMsg	= 0x103 										# Modbus 下行下次列车到站时间消息
ModbusUpTimeMsg 		= 0x104 										# Modbus 上行到站时间消息
ModbusUpNextTimeMsg 	= 0x105 										# Modbus 上行下次到站到站时间消息
ModbusEmergenceMsg 		= 0x106 										# Modbus 紧急通知消息

UnknownPISMsg			= 0x200 										# 

# PIS 消息列表
PISMessageTypeList = [
	HeartBeatMsg,
	HeartBeatAck,
	SendProgramListMsg,
	SendProgramListAck,
	SendNewsMsg,	
	SendNewsAck,
	# ClearNewsMsg,  
	# ClearNewsAck,
	SendEmergenceMsg,
	SendEmergenceAck,
	# ClearEmergenceMsg,
	# ClearEmergenceAck,
	SendLineInfoMsg,
	SendLineInfoAck,
	SendStationTimeMsg,
	SendStationTimeAck,
	SendFormatFileMsg,
	SendFormatFileAck,
	SystemRebootMsg,
	SystemRebootAck,
	RunStatusMsg,
	RunStatusAck,
	RunLogMsg,	
	RunLogAck,  
	SendUpdateMsg,
	SendUpdateAck,
	InitialRequestMsg,
	InitialRequestAck,
	UpdateWorkTimeMsg,
	UpdateWorkTimeAck,
	ViewControllerVideoMsg,
	ViewControllerVideoAck,
	SendSafeVideoMsg,
	SendSafeVideoAck,

	# 控制器在下载视频文件完成后，需要向服务器报告下载已经完成
	DownloadCompleteMsg,
	DownloadCompleteAck,

	UnAckedMsg,
	ModbusMsg,

	ModbusDownTimeMsg,
	ModbusUpTimeMsg,
	ModbusEmergenceMsg,
]

# 需要接收下载完成消息的消息列表
msgForDownloadList = [ 
	SendProgramListMsg, 
	SendSafeVideoMsg, 
	SendUpdateMsg, 
	SendFormatFileMsg 
]

# 需要在控制器初始化后重新发送的失败消息列表
failedMsgForResendList = [
	SendProgramListMsg, 
	SendFormatFileMsg, 
	SendSafeVideoMsg, 
	SendNewsMsg
]

# 不需要显示收发记录的消息
noShowMsgList = [
	HeartBeatMsg,
	HeartBeatAck,
	RunStatusMsg,
	RunStatusAck,
	RunLogMsg,
	RunLogAck,
]


MsgTypeList = {}

def setMsgType(msgType, isTCPMsg = False, isAckMsg = False):
	def register(msg):
		MsgTypeList[msgType] = msg
		msg.msgType 		= msgType
		msg.isTCPMsg 		= isTCPMsg
		msg.isAckMsg 		= isAckMsg
		return msg

	return register

# PIS 控制器消息基类
class CBaseMsg(CBaseMessage) :
	def __init__(self) :
		CBaseMessage.__init__(self)

		self.isTCPMsg 		= False 									# 是否为TCP消息
		self.isAckMsg 		= False 									# 是否为响应消息
		self.isSendMsg 		= False 									# 准备发送给控制器的消息标识

		self.MsgListID 		= 0 										# 消息的MsgID值
		self.MsgLogID 		= 0 										# 消息的logID值
		self.OperationID1 	= 0 										# OprationID1 为节目单记录ID，或版式文件1记录ID
		self.OperationID2 	= 0 										# 版式文件2记录ID

		self.ResendFlag 	= False 									# 重发标识
		self.SendTimes 		= 0 										# 发送次数
		self.Controller 	= None 										# 发送或接收的控制器

		msg 				= type(self)
		self.msgType 		= msg.msgType
		self.isTCPMsg 		= msg.isTCPMsg
		self.isAckMsg 		= msg.isAckMsg

	def isTcp(self) :
		return self.isTCPMsg

	def isAck(self) :
		return self.isAckMsg


	def getAckMsgType(self):
		if self.isAckMsg:
			return UnknownPISMsg
		return self.msgType + 1

	def getRequestMsgType(self): 										# 消息定义规则：响应消息的ID为请求消息ID + 1
		if not self.isAckMsg:
			# logE("no request message type for a request message")
			return UnknownPISMsg
		return self.msgType - 1

	def getMsgTypeStr(self):
		return type(self).__name__

		dbHelper = DbHelper(True)

		try:
			cur = dbHelper.getCursor()

			sql = "select cv.description"
			sql += " from system.code_value cv where cv.type_code = 'MESSAGE_TYPE' and cv.code = '%s' and cv.status = '1' "%self.msgType
			cur.execute(sql)
			rows = cur.fetchall()
			dbHelper.closeDB()
			if len(rows) == 0:
				return "unknown message type: %s"%self.msgType
			else:
				return rows[0][0]

		except Exception as e:
			dbHelper.closeDB()
			onException(e)
			return "ERROR"

	def setOperationID(self, msgID, OperationID1 = 0, OperationID2 = 0):
		self.MsgListID 		= msgID
		self.OperationID1 	= OperationID1
		self.OperationID2 	= OperationID2

		return

	# 无参数消息编码时将调用基类函数
	# 有参数消息编码时调用派生类函数
	def encode(self):
		return b""

	# 无参数消息解码时将调用基类函数
	def decode(self, dataBuf):
		self.setMsgBuffer(dataBuf)
		self.decodeHead()
		self.decodeEnd()
		return True

	# 对消息进行编码，
	# 返回：编码后的数据
	def encodeMsg(self, msg):
		msg.pos = 0
		msg.msgBuffer = None

		# 首先将消息的消息体部分进行编码
		msgData = msg.encode()

		# 对消息头部、消息体、消息尾部进行编码
		msg.encodeHead()
		
		msg.encodeData(msgData)
		msg.encodeEnd()

		return msg.getMsgBuffer()

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
			logE("not supported type " + str(msgType))
			return None

		# 根据对象类型，生成一个对象
		msg = decodeMsgType()
		msg.decode(dataBuf)
		return msg

	def toString(self):
		return "n/a"

class CBaseAckMsg(CBaseMsg):
	def __init__(self) :
		CBaseMsg.__init__(self)

		self.ack_type 	= 1
		self.status  	= 2
		self.isAckMsg 	= True

	def setParams(self, ack_type, status):
		self.ack_type 	= ack_type
		self.status 	= status

	def encode(self):
		self.encodeUINT(self.ack_type)
		self.encodeUINT(self.status)
		return self.getMsgBuffer()

	def decode(self, dataBuf) :
		self.setMsgBuffer(dataBuf)
		self.decodeHead()
		self.ack_type 	= self.decodeUINT()
		self.status 	= self.decodeUINT()
		self.decodeEnd()
		return True

	def toString(self):
		return "msg type: %d, %s, ack_type : %d， status : %d" % (self.msgType, self.getMsgTypeStr(), self.ack_type, self.status)


@setMsgType(HeartBeatMsg)
class CHeartBeatMsg(CBaseMsg):
	def __init__(self) :
		CBaseMsg.__init__(self)
		self.msgType 	= HeartBeatMsg

@setMsgType(HeartBeatAck, isAckMsg = True)
class CHeartBeatAckMsg(CBaseMsg):
	def __init__(self) :
		CBaseMsg.__init__(self)

	def toString(self):
		return "HeartBeatAckMsg"

@setMsgType(SendProgramListMsg)
class CSendProgramListMsg(CBaseMsg):
	def __init__(self) :
		CBaseMsg.__init__(self)
		self.programe_file 	= "" 										# 节目单文件名称，含路径

	def setParams(self, programe_file):
		self.programe_file = programe_file

	def encode(self):
		self.encodeString(self.programe_file)
		return self.getMsgBuffer()

	def decode(self, dataBuf):
		self.setMsgBuffer(dataBuf)
		self.decodeHead()
		self.programe_file = self.decodeString()
		self.decodeEnd()

	def toString(self):
		return "发送节目单: %s" % (self.programe_file)

@setMsgType(SendProgramListAck, isAckMsg = True)
class CSendProgramListAck(CBaseAckMsg):
	def __init__(self) :
		CBaseAckMsg.__init__(self)

		# 无参数

	def toString(self):
		return "发送节目单响应消息"

@setMsgType(SendNewsMsg)
class CSendNewsMsg(CBaseMsg):
	def __init__(self) :
		CBaseMsg.__init__(self)

		self.content 	= "" 											# 新闻内容，内容为空时为清除新闻
		self.life_time 	= 0 											# 显示时间，单位：秒，0 为一直显示，否则时间到就停止显示内容

	def setParams(self, life_time, content):
		self.life_time 	= life_time
		self.content 	= content

	def encode(self):
		self.encodeUINT(self.life_time)
		self.encodeString(self.content)
		return self.getMsgBuffer()

	def decode(self, dataBuf):
		self.setMsgBuffer(dataBuf)
		self.decodeHead()
		self.life_time 	= self.decodeUINT()
		self.content 	= self.decodeString()
		self.decodeEnd()

	def toString(self):
		return "新闻内容: %s，有效时长: %s" % (self.content, self.life_time)

@setMsgType(SendNewsAck, isAckMsg = True)
class CSendNewsAck(CBaseAckMsg):
	def __init__(self) :
		CBaseAckMsg.__init__(self)
		# 无参数

	def toString(self):
		return "发送新闻响应消息"

# @setMsgType(ClearNewsMsg)
# class CClearNewsMsg(CBaseMsg): 											# 清除新闻内容
# 	def __init__(self) :
# 		CBaseMsg.__init__(self)
# 		# 无参数

# 	def toString(self):
# 		return "清除新闻内容"

# @setMsgType(ClearNewsAck, isAckMsg = True)
# class CClearNewsAck(CBaseAckMsg):
# 	def __init__(self) :
# 		CBaseAckMsg.__init__(self)
# 		# 无参数

# 	def toString(self):
# 		return "清除新闻响应消息"

@setMsgType(SendEmergenceMsg)
class CSendEmergenceMsg(CBaseMsg): 										# 发送紧急通知消息
	def __init__(self) :
		CBaseMsg.__init__(self)

		self.content 	= "" 											# 紧急通知内容
		self.life_time 	= 0 											# 显示时间，单位：秒，0 为一直显示，否则时间到就停止显示内容
		self.mode 		= 0 											# 显示方式，0：滚动，1：全屏

	def setParams(self, life_time, mode, content):
		self.mode 		= mode
		self.life_time 	= life_time
		self.content 	= content

	def encode(self):
		self.encodeUINT(self.life_time)
		self.encodeUINT(self.mode)
		self.encodeString(self.content)
		return self.getMsgBuffer()

	def toString(self):
		if self.mode == 0:
			modeName = "滚动"
		else:
			modeName = "全屏"
		return "模式: %s，通知内容: %s，有效时长: %s" % ( modeName, self.content, self.life_time)

@setMsgType(SendEmergenceAck, isAckMsg = True)
class CSendEmergenceAck(CBaseAckMsg):
	def __init__(self) :
		CBaseAckMsg.__init__(self)
		# 无参数

	def toString(self):
		return "发送紧急消息响应消息"

# @setMsgType(ClearEmergenceMsg)
# class CClearEmergenceMsg(CBaseMsg):
# 	def __init__(self) :
# 		CBaseMsg.__init__(self)

# 	def toString(self):
# 		return "清除紧急通知内容"

# @setMsgType(ClearEmergenceAck, isAckMsg = True)
# class CClearEmergenceAck(CBaseAckMsg):
# 	def __init__(self) :
# 		CBaseAckMsg.__init__(self)
# 		# 无参数

# 	def toString(self):
# 		return "清除紧急消息响应消息"


@setMsgType(SendLineInfoMsg)  											# 该消息未使用
class CSendLineInfoMsg(CBaseMsg):
	def __init__(self) :
		CBaseMsg.__init__(self)

		self.SubLineNum 			= 1 		 		 				# 默认运行线路只有一条支线，即主线
		self.SubLineList = []

	# lineInfo: 支线信息数组
	def setParams(self, subLineInfo):
		self.SubLineList = []
		for item in subLineInfo:
			SubLineInfo = {
				'UpStartStationNameCN'  : item['up_start_station_name'],
				'UpStartStationNameEN'  : item['up_start_station_name_en'],
				'UpEndStationNameCN' 	: item['up_end_station_name'],
				'UpEndStationNameEN' 	: item['up_end_station_name_en'],
				'DownStartStationNameCN': item['down_start_station_name'],
				'DownStartStationNameEN': item['down_start_station_name_en'],
				'DownEndStationNameCN' 	: item['down_end_station_name'],
				'DownEndStationNameEN' 	: item['down_end_station_name_en'],
				'UpLineStartTime'  		: item['up_start_time'],
				'UpLineEndTime'  		: item['up_end_time'],
				'DownLineStartTime' 	: item['down_start_time'],
				'DownLineEndTime' 		: item['down_end_time'],
			}
			self.SubLineList.append(SubLineInfo)

		self.SubLineNum = len(self.SubLineList)

	def encode(self):
		self.encodeChar(self.SubLineNum)
		for SubLineInfo in self.SubLineList:
			self.encodeString(SubLineInfo['UpStartStationNameCN'])
			self.encodeString(SubLineInfo['UpStartStationNameEN'])
			self.encodeString(SubLineInfo['UpEndStationNameCN'])
			self.encodeString(SubLineInfo['UpEndStationNameEN'])
			self.encodeString(SubLineInfo['DownStartStationNameCN'])
			self.encodeString(SubLineInfo['DownStartStationNameEN'])
			self.encodeString(SubLineInfo['DownEndStationNameCN'])
			self.encodeString(SubLineInfo['DownEndStationNameEN'])

			self.encodeString(SubLineInfo['UpLineStartTime'])
			self.encodeString(SubLineInfo['UpLineEndTime'])
			self.encodeString(SubLineInfo['DownLineStartTime'])
			self.encodeString(SubLineInfo['DownLineEndTime'])

		return self.getMsgBuffer()

	def toString(self):
		return "发送线路信息消息" # "UP Line Start Time: %s，UP Line End Time: %s，Down Line Start Time: %s，Down Line Close Time: %s"%(self.UPLineStartTime, self.UPLineEndTime, self.DownLineStartTime, self.DownLineEndTime)

@setMsgType(SendLineInfoAck, isAckMsg = True)
class CSendLineInfoAck(CBaseAckMsg):
	def __init__(self) :
		CBaseAckMsg.__init__(self)
		# 无参数

	def toString(self):
		return "发送线路信息响应消息"


@setMsgType(SendStationTimeMsg)
class CSendStationTimeMsg(CBaseMsg): 									# 到站信息，由车站服务器接收综合监控消息后发送给控制器
	def __init__(self) :
		CBaseMsg.__init__(self)

	def setParams(self, stationTime):
		
		self.endStationID 	= stationTime["EndStationID"] 				# 到站ID
		self.arriveTimeH 	= stationTime["ArriveTimeH"] 				# 到达时间小时值
		self.arriveTimeM 	= stationTime["ArriveTimeM"] 				# 到达时间分钟值

		self.nextEndStationID 	= stationTime["NextEndStationID"] 		# 下次列车到站ID
		self.nextArriveTimeH	= stationTime["NextArriveTimeH"] 		# 下次列车到达时间小时值
		self.nextArriveTimeM	= stationTime["NextArriveTimeM"] 		# 下次列车到达时间分钟值

	def encode(self):
		self.encodeWord(self.arriveTimeH)
		self.encodeWord(self.arriveTimeM)
		self.encodeWord(self.endStationID)

		self.encodeWord(self.nextArriveTimeH)
		self.encodeWord(self.nextArriveTimeM)
		self.encodeWord(self.nextEndStationID)

		return self.getMsgBuffer()

	def toString(self):
		return "ArriveTime : %d hour %d minute，End Station ID: %d, NextArriveTime: %d hour %d minute, NextEndStationID: %d" % (
			self.arriveTimeH, self.arriveTimeM, self.endStationID, self.nextArriveTimeH, self.nextArriveTimeM, self.nextEndStationID)

@setMsgType(SendStationTimeAck, isAckMsg = True)
class CSendStationTimeAck(CBaseAckMsg):
	def __init__(self) :
		CBaseAckMsg.__init__(self)

		# 无参数

	def toString(self):
		return "发送到站消息响应消息"

@setMsgType(SendFormatFileMsg)
class CSendFormatFileMsg(CBaseMsg): 									# 发送版式文件
	def __init__(self) :
		CBaseMsg.__init__(self)

		self.format_file 		= "" 									# 版式文件1文件名，含路径

	def setParams(self, format_file):
		self.format_file = format_file

	def encode(self):
		self.encodeString(self.format_file)
		return self.getMsgBuffer()

	def toString(self):
		return "版式文件: %s"%(self.format_file)

@setMsgType(SendFormatFileAck, isAckMsg = True)
class CSendFormatFileAck(CBaseAckMsg):
	def __init__(self) :
		CBaseAckMsg.__init__(self)

		# 无参数

	def toString(self):
		return "发送版式文件响应消息"

@setMsgType(SystemRebootMsg, isTCPMsg = False)
class CSystemRebootMsg(CBaseMsg): 										# 系统重启命令
	def __init__(self) :
		CBaseMsg.__init__(self)
		self.delay_second 	= 0 										# 延时时间，单位：秒，默认：0，立即重启
		self.rebootMode 	= 0 										# 重启模式，0：只重启应用，1：重启服务器，暂未启用

	def setParams(self, delay_second, rebootMode = 1 ): 				# 默认重启服务器
		self.delay_second 	= delay_second
		self.rebootMode 	= rebootMode

	def encode(self):
		self.encodeUINT(self.delay_second)
		self.encodeChar(self.rebootMode)

		return self.getMsgBuffer()

	def toString(self):
		return "重启延迟时间: %s，重启模式：%s" %(self.delay_second, "重启应用" if self.rebootMode == 0 else "重启服务器")

@setMsgType(SystemRebootAck, isAckMsg = True, isTCPMsg = False)
class CSystemRebootAck(CBaseAckMsg):
	def __init__(self) :
		CBaseAckMsg.__init__(self)

		# 无参数

	def toString(self):
		return "发送系统重启响应消息"

"""
1	CPU			U32	4	控制器CPU占有率	百分比值 * 100
2	MEMORY		U32	4	内存占有率	百分比值 * 100
3	HARDDISK	U32	4	硬盘占有率	百分比值 * 100
4	STATUS		U32	4	0-正常；其他为故障编码	
"""

@setMsgType(RunStatusMsg, isTCPMsg = False)
class CRunStatusMsg(CBaseMsg): 											# 运营状态报告
	def __init__(self) :
		CBaseMsg.__init__(self)

		self.cpu 		= 0 											# CPU占有率，值小于10000，为百分比值*100
		self.memory 	= 0 											# 内存使用率，值小于10000，为百分比值*100
		self.harddisk 	= 0 											# 硬盘使用率，值小于10000，为百分比值*100
		self.status 	= 0 											# 状态值，暂未启用

	def setParams(self, cpu, memory, harddisk, status):
		self.cpu 		= cpu
		self.memory 	= memory
		self.harddisk 	= harddisk
		self.status 	= status

	def encode(self):
		self.encodeUINT(self.cpu)
		self.encodeUINT(self.memory)
		self.encodeUINT(self.harddisk)
		self.encodeUINT(self.status)
		return self.getMsgBuffer()

	def decode(self, data):
		self.setMsgBuffer(data)
		self.decodeHead()
		self.cpu 		= self.decodeUINT()
		self.memory 	= self.decodeUINT()
		self.harddisk 	= self.decodeUINT()
		self.status 	= self.decodeUINT()
		self.decodeEnd()

	def toString(self):
		return "CPU: %0.2f%%, 内存: %0.2f%%, 硬盘: %0.2f%%" % (self.cpu/100, self.memory/100, self.harddisk/100)

@setMsgType(RunStatusAck, isAckMsg = True, isTCPMsg = False)
class CRunStatusAck(CBaseAckMsg):
	def __init__(self) :
		CBaseAckMsg.__init__(self)
		# 无参数

	def toString(self):
		return "运行状态响应消息"

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

@setMsgType(RunLogMsg, isTCPMsg = False)
class CRunLogMsg(CBaseMsg): 											# 运行日志报告
	def __init__(self):
		CBaseMsg.__init__(self)

		self.logID 	 	= 0 											# 日志ID，每天从1开始
		self.logLevel 	= 0 											# 日志级别
		self.logType 	= 0 											# 日志类型
		self.logTitle 	= "" 											# 日志标题
		self.logDesc 	= "" 											# 日志内容
		self.logDate 	= "" 											# 日志日期
		self.logTime 	= "" 											# 日志时间

	def encode(self):
		self.encodeUINT(self.logID)
		self.encodeWord(self.logLevel)
		self.encodeWord(self.logType)

		self.encodeString(self.logTitle)
		self.encodeString(self.logDesc)

		self.encodeString(self.logDate)
		self.encodeString(self.logTime)

		return self.getMsgBuffer()

	def decode(self, data) :
		self.setMsgBuffer(data)
		self.decodeHead()
		self.logID 	 	= self.decodeUINT()
		self.logLevel 	= self.decodeWord()
		self.logType 	= self.decodeWord()
		self.logTitle 	= self.decodeString()
		self.logDesc 	= self.decodeString()
		self.logDate 	= self.decodeString()
		self.logTime 	= self.decodeString()
		# self.logDesc = self.logDesc.decode("gbk")
		self.logDesc 	= " "

		self.decodeEnd()

	def toString(self):
		return "日志级别: %d，类型: %d，日期: %s，时间: %s，标题: %s，描述: %s"%(self.logLevel, self.logType, self.logDate, self.logTime, self.logTitle, self.logDesc)

@setMsgType(RunLogAck, isAckMsg = True, isTCPMsg = False)
class CRunLogAck(CBaseAckMsg):
	def __init__(self) :
		CBaseAckMsg.__init__(self)
		# 无参数

	def toString(self):
		return "运行日志响应消息"

"""
1	FILE_PATH	STR	128		升级文件路径
"""
@setMsgType(SendUpdateMsg)
class CSendUpdateMsg(CBaseMsg): 										# 更新控制器软件
	def __init__(self) :
		CBaseMsg.__init__(self)

		self.file_path 	= "" 											# 软件包路径，含路径
		self.updateMode = 0 											# 0：下次重启时更新，1：立即更新，默认为重启时更新

	def setParams(self, file_path, updateMode = 0):
		self.file_path  = file_path
		self.updateMode = updateMode

	def encode(self):
		self.encodeString(self.file_path)
		self.encodeByte(self.updateMode)
		return self.getMsgBuffer()

	def toString(self):
		return "控制器版本文件名称: %s, 升级模式：%s"%(self.file_path, "重启时升级" if self.updateMode == 0 else "立刻重启")

@setMsgType(SendUpdateAck, isAckMsg = True)
class CSendUpdateAck(CBaseAckMsg):
	def __init__(self) :
		CBaseAckMsg.__init__(self)
		# 无参数

	def toString(self):
		return "发送系控制器升级响应消息"

"""
1	SCREEN_PATH			STR	128		抓屏根路径
2	LOG_PATH			STR	128		日志根路径
3	DEVICE_NO			STR	16		设备编号
4	SNATCH_SCREEN_INTERVAL	U32	4		抓屏间隔，秒
"""
@setMsgType(InitialRequestMsg)
class CInitialRequestMsg(CBaseMsg): 									# 控制器初始化请求消息
	def __init__(self) :
		CBaseMsg.__init__(self)

	def toString(self):
		return "控制器初始化请求"

# 站台控制器才区分首站、末站等和方向相关的参数，站厅和车载控制器不需要处理
@setMsgType(InitialRequestAck, isAckMsg = True)
class CInitialRequestAck(CBaseMsg): 									# 初始化请求响应消息
	def __init__(self) :
		CBaseAckMsg.__init__(self)

		self.snatch_screen_interval = PisConfig["snatchScreenTime"] 	# 截屏时间间隔，单位：秒
		self.screen_path 			= os.path.join(PisConfig['PISFileRoot'], "screen") 	# 截屏文件存放路径

		self.log_path 				= os.path.join(PisConfig['PISFileRoot'], "log") 	# 日志文件上传路径
		self.device_no 				= "" 								# 设备编号

		self.current_station 		= "" 								# 本站中文名称
		self.current_station_en 	= "" 								# 本站英文名称
		self.next_station 			= "" 								# 下一站中文名称
		self.next_station_en 		= "" 								# 下一站英文名称
	
		self.train_name 			= ""								# 车辆中文名称
		self.train_name_en 			= "" 								# 车辆英文名称

		self.controller_type 		= '' 								# 控制器类型: 0: 车头，1：车尾，2：上行，3：下行，4：站厅

		self.openTime 				= "" 								# 控制器开机时间
		self.closeTime 				= "" 								# 控制器休眠时间

		self.UPStationStartTime 	= "" 								# 上行本站首班时间
		self.UPStationEndTime 		= "" 								# 上行本站末班时间
		self.DownStationStartTime 	= "" 								# 下行本站首班时间
		self.DownStationEndTime 	= "" 								# 下行本站末班时间

		self.SubLineNum 			= 0 								# 支线数量
		self.subLineList 			= [] 								# 支线信息数组

	def setParams(self, device_no, lineInfo):

		self.device_no 				= device_no

		self.controller_type 		= lineInfo['install_type']
		if self.controller_type >= '2': 								# 控制器为站厅或上下行站台控制器
			self.current_station 	= lineInfo["station_name"]
			self.current_station_en = lineInfo["station_name_en"]

		# 控制器休眠唤醒时间
		self.openTime 				= lineInfo['open_time']
		self.closeTime 				= lineInfo['close_time']

		self.UPStationStartTime 	= lineInfo['up_start_time']
		self.UPStationEndTime 		= lineInfo['up_end_time']
		self.DownStationStartTime 	= lineInfo['down_start_time']
		self.DownStationEndTime 	= lineInfo['down_end_time']

		self.subLineList 			= lineInfo['subLineList']
		self.SubLineNum 			= len(self.subLineList)

	def encode(self):
		logI("screen_path: %s"%self.screen_path)
		self.encodeString(self.screen_path)
		logI("log_path: %s"%self.log_path)
		self.encodeString(self.log_path)
		self.encodeString(self.device_no)
		self.encodeUINT(self.snatch_screen_interval)
		self.encodeString(self.current_station)
		self.encodeString(self.current_station_en)

		# controller_type
		self.encodeChar( int(self.controller_type))

		self.encodeString(self.openTime)
		self.encodeString(self.closeTime)

		self.encodeString(self.UPStationStartTime)
		self.encodeString(self.UPStationEndTime)
		self.encodeString(self.DownStationStartTime)
		self.encodeString(self.DownStationEndTime)

		self.encodeChar(self.SubLineNum)
		for SubLineInfo in self.subLineList:
			self.encodeString(SubLineInfo['up_start_station_name'])
			self.encodeString(SubLineInfo['up_start_station_name_en'])
			self.encodeString(SubLineInfo['up_end_station_name'])
			self.encodeString(SubLineInfo['up_end_station_name_en'])
			self.encodeString(SubLineInfo['down_start_station_name'])
			self.encodeString(SubLineInfo['down_start_station_name_en'])
			self.encodeString(SubLineInfo['down_end_station_name'])
			self.encodeString(SubLineInfo['down_end_station_name_en'])

			self.encodeString(SubLineInfo['up_start_time'])
			self.encodeString(SubLineInfo['up_end_time'])
			self.encodeString(SubLineInfo['down_start_time'])
			self.encodeString(SubLineInfo['down_end_time'])

		return self.getMsgBuffer()

	def toString(self):
		return "截屏路径: %s，截屏间隔: %d，日志路径: %s，设备编号: %s，当前站: %s，%s" % (self.screen_path, self.snatch_screen_interval, self.log_path, self.device_no, self.current_station, self.current_station_en)

"""
1	open_time
2 	close_time
"""
@setMsgType(UpdateWorkTimeMsg)
class CUpdateWorkTimeMsg(CBaseMsg): 									# 更新控制器休眠唤醒时间
	def __init__(self) :
		CBaseMsg.__init__(self)

		self.openTime 		= "" 										# 唤醒时间
		self.closeTime 		= "" 										# 休眠时间

	def setParams(self, openTime, closeTime):
		self.openTime 	= openTime
		self.closeTime 	= closeTime

	def encode(self):
		self.encodeString(self.openTime)
		self.encodeString(self.closeTime)
		return self.getMsgBuffer()

	def toString(self):
		return "开机时间: %s，休眠时间: %s" % (self.openTime, self.closeTime)

@setMsgType(UpdateWorkTimeAck, isAckMsg = True)
class CUpdateWorkTimeAck(CBaseAckMsg):
	def __init__(self) :
		CBaseAckMsg.__init__(self)

		self.msgType 	= UpdateWorkTimeAck
		# 无参数

	def toString(self):
		return "更新控制器工作时间响应消息"

@setMsgType(ViewControllerVideoMsg)
class CViewControllerVideoMsg(CBaseMsg): 								# 查看控制器视频
	def __init__(self) :
		CBaseMsg.__init__(self)

		self.start_end_flag = 0 										# 开始结束标识，0：结束，1：开始

	def setParams(self, start_end_flag):
		self.start_end_flag = start_end_flag

	def encode(self):
		self.encodeChar(self.start_end_flag)
		return self.getMsgBuffer()

	def toString(self):
		return "查看控制器实时视频：%s" % ( "开始查看" if (self.start_end_flag == 1) else "结束查看" )


@setMsgType(ViewControllerVideoAck, isAckMsg = True)
class CViewControllerVideoAck(CBaseAckMsg):
	def __init__(self) :
		CBaseAckMsg.__init__(self)

		self.msgType 	= ViewControllerVideoAck
		# 无参数

	def toString(self):
		return "查看控制器实时视频响应消息"

@setMsgType(SendSafeVideoMsg)
class CSendSafeVideoMsg(CBaseMsg): 										# 发送安全垫片
	def __init__(self) :
		CBaseMsg.__init__(self)
		
		self.file_path 	= "" 											# 安全垫片文件名，含路径

	def setParams(self, filePath):
		self.file_path = filePath

	def encode(self):
		self.encodeString(self.file_path)
		return self.getMsgBuffer()

	def toString(self):
		return self.file_path


@setMsgType(SendSafeVideoAck, isAckMsg = True)
class CSendSafeVideoAck(CBaseAckMsg):
	def __init__(self) :
		CBaseAckMsg.__init__(self)

		# 无参数

	def toString(self):
		return "发送安全垫片响应消息"

@setMsgType(DownloadCompleteMsg)
class CDownloadCompleteMsg(CBaseMsg): 									# 下载完成通知消息
	def __init__(self) :
		CBaseMsg.__init__(self)
		self.requestMsgType = None 										# 下载完成的请求类别，为：节目单、安全垫片或节目单

	def setParams(self, requestMsgType):
		self.requestMsgType = requestMsgType

	def encode(self):
		self.encodeUINT(self.requestMsgType)
		return self.getMsgBuffer()

	def decode(self, data) :
		self.setMsgBuffer(data)
		self.decodeHead()
		self.requestMsgType = self.decodeUINT()
		self.decodeEnd()

	def encode(self):
		self.encodeUINT(self.requestMsgType)
		return self.getMsgBuffer()

	def toString(self):
		return "下载完成消息，请求消息类型: %d"%self.requestMsgType

@setMsgType(DownloadCompleteAck, isAckMsg = True)
class CDownloadCompleteAck(CBaseAckMsg):
	def __init__(self) :
		CBaseAckMsg.__init__(self)
		# 无参数

	def toString(self):
		return "下载完成响应消息"


@setMsgType(UnAckedMsg, isAckMsg = True)
class CUnAckedMsg(CBaseMsg):
	def __init__(self) :
		CBaseMsg.__init__(self)

		self.isSendMsg 	= False


INSTALL_TYPE_HEAD       = "0"
INSTALL_TYPE_TAIL       = "1"
INSTALL_TYPE_UP         = "2"
INSTALL_TYPE_DOWN       = "3"
INSTALL_TYPE_HALL       = "4"
INSTALL_TYPE_OTHER      = "5"

# Modbus Register Address, 地址码，也可视为消息类别
# H： 站厅控制器, U：上行控制器, D: 下行控制器
MB_ADDR_STATUS_HC       = 0x01 											# 站厅控制器状态
MB_ADDR_STATUS_UC       = 0x02 											# 上行控制器状态
MB_ADDR_STATUS_DC       = 0x03 											# 下行控制器状态

# 紧急消息
MB_ADDR_CTRL_HC         = 0x0B 											# 站厅控制器
MB_ADDR_CTRL_UC         = 0x0C 											# 上行控制器
MB_ADDR_CTRL_DC         = 0x0D 											# 下行控制器
MB_ADDR_MSG_SEL_ALL     = 0x15 											# 所有控制器
MB_ADDR_MSG_MODE        = 0x16 											# 消息类型，0：滚动，1：全屏
MB_ADDR_MSG_CONTENT     = 0x33 											# 消息内容

# 到站信息
MB_ADDR_D1_DEST         = 0xC9 											# 下行到站ID
MB_ADDR_D1_TIME_HH      = 0xCD 											# 下行到站时间小时值
MB_ADDR_D1_TIME_MM      = 0xCC 											# 下行到站时间分钟值

MB_ADDR_D2_DEST         = 0xCF 											# 下行下次列车到站ID
MB_ADDR_D2_TIME_HH      = 0xD3 											# 下行下次列车到站小时值
MB_ADDR_D2_TIME_MM      = 0xD2 											# 下行下次列车到站分钟值

MB_ADDR_U1_DEST         = 0xC8 											# 上行到站ID
MB_ADDR_U1_TIME_HH      = 0xCB 											# 上行列车到站小时值
MB_ADDR_U1_TIME_MM      = 0xCA 											# 上行列车到站分钟值

MB_ADDR_U2_DEST         = 0xCE 											# 上行下次列车到站ID
MB_ADDR_U2_TIME_HH      = 0xD1 											# 上行下次列车到站小时值
MB_ADDR_U2_TIME_MM      = 0xD0 											# 上行下次列车到站分钟值

ModbusMsgType = {} 														# 消息名称列表，用于显示输出

ModbusMsgType[MB_ADDR_STATUS_HC]    = "Modbus Query Hall Controller Status Msg"
ModbusMsgType[MB_ADDR_STATUS_UC]    = "Modbus Query Up Controller Status Msg"
ModbusMsgType[MB_ADDR_STATUS_DC]    = "Modbus Query Down Controller Status Msg"
ModbusMsgType[MB_ADDR_CTRL_HC]      = "Modbus Send Emergence message to Hall Controller Msg"
ModbusMsgType[MB_ADDR_CTRL_UC]      = "Modbus Send Emergence message to Up Controller Msg"
ModbusMsgType[MB_ADDR_CTRL_DC]      = "Modbus Send Emergence message to Down Controller Msg"
ModbusMsgType[MB_ADDR_MSG_SEL_ALL]  = "Modbus Send emergence message to all controller Msg"
ModbusMsgType[MB_ADDR_MSG_MODE]     = "Modbus Emergence message mode Msg"
ModbusMsgType[MB_ADDR_MSG_CONTENT]  = "Modbus Emergence message content Msg"
ModbusMsgType[MB_ADDR_D1_DEST]      = "Modbus Send Down Arriving Train Destination Code Msg"
ModbusMsgType[MB_ADDR_D1_TIME_HH]   = "Modbus Send Down Arriving Train Hour Msg"
ModbusMsgType[MB_ADDR_D1_TIME_MM]   = "Modbus Send Down Arriving Train Minute Msg"

ModbusMsgType[MB_ADDR_D2_DEST]      = "Modbus Send Down Next Arriving Train Destination Code Msg"
ModbusMsgType[MB_ADDR_D2_TIME_HH]   = "Modbus Send Down Next Arriving Train Hour Msg"
ModbusMsgType[MB_ADDR_D2_TIME_MM]   = "Modbus Send Down Next Arriving Train Minute Msg"

ModbusMsgType[MB_ADDR_U1_DEST]      = "Modbus Send Up Arriving Train Destination Code Msg"
ModbusMsgType[MB_ADDR_U1_TIME_HH]   = "Modbus Send Up Arriving Train Hour Msg"
ModbusMsgType[MB_ADDR_U1_TIME_MM]   = "Modbus Send Up Arriving Train Minute Msg"
ModbusMsgType[MB_ADDR_U2_DEST]      = "Modbus Send Up Next Arriving Train Destination Code Msg"
ModbusMsgType[MB_ADDR_U2_TIME_HH]   = "Modbus Send Up Next Arriving Train Hour Msg"
ModbusMsgType[MB_ADDR_U2_TIME_MM]   = "Modbus Send Up Next Arriving Train Minute Msg"

@setMsgType(ModbusMsg)
class CModbusMsg(CBaseMsg): 											# Mobus消息，接收从综合监控的消息
	def __init__(self) :
		CBaseMsg.__init__(self)

		self.isSendMsg	= False
		self.strMsg 	= ""
		self.setOperationID(None)

		# Modbus 消息参数
		self.TransactionID 	= 0 										# 事务ID 
		self.ProtocolID  	= 0 										# 协议ID，默认为0
		self.DataLength 	= 0 										# 后面的数据长度
		self.UnitID 		= 0 										# 单元ID或地址，默认为0
		self.FunctionCode 	= 0 										# 功能码

		self.RegisterAddr 	= 0 										# 寄存器起始地址，也可以视为消息类型
		self.RegisterCount	= 0 										# 寄存器数量
		self.msgDataLen		= 0 										# 消息的数据长度
		self.msgData 		= [] 										# 消息数据

		self.isSaved 		= False

	def left(self): 													# 当前数据中还没有解码的数据长度，因为在一次接收数据里面可能会包含多条消息的内容
		return len(self.msgBuffer) - self.pos

	def decode(self, clientAddr):

		# if self.saved:
		# 	self.restore()

		dataLength = self.left() 										# 剩余数据的长度
		if not dataLength:
			return False

		if dataLength < 8: 												# 数据最小长度
			logE("Error: ModbusMsg too short: Length: %d, Data: %s"%dataLength, ConvertToHex(self.msgBuffer[self.pos:eln(self.msgBuffer)]))
			return False

		# 消息头部
		self.TransactionID 	= self.decodeWord()
		self.ProtocolID 	= self.decodeWord()
		self.DataLength 	= self.decodeWord()
		self.UintID 		= self.decodeChar()

		if clientAddr:
			self.srcAddr 	= clientAddr[0]
		else:
			self.srcAddr 	= None

		self.FunctionCode 	= self.decodeByte()

		if self.FunctionCode == 0x03:
			if self.DataLength != 6:
				logE("Error: ModbusMsg Function Code: 03H, data length(%d) != 6"%self.DataLength)
				return False

			if dataLength < 12:
				logE("Error: ModbusMsg Function Code: 03H, total length(%d) < 12"%dataLength)
				return False

			self.RegisterAddr 	= self.decodeWord()
			self.RegisterCount 	= self.decodeWord()

		elif self.FunctionCode == 0x10:
			if dataLength < 13:
				logE("Error: ModbusMsg Function Code: 10H, total length(%d) < 13"%dataLength)
				return False

			if dataLength < self.DataLength + 6:
				logE("Error: ModbusMsg Function Code: 10H, total length(%d) < length(%d) + 6"%(dataLength, self.DataLength))
				return False

			self.RegisterAddr 	= self.decodeWord()
			self.RegisterCount 	= self.decodeWord()
			self.msgDataLen 	= self.decodeByte()

			if self.msgDataLen < self.DataLength - 7:
				logE("Error: ModbusMsg Function Code: 10H, byte length(%d) < length(%d) - 7"%(self.msgDataLen, self.DataLength))
				return False

			self.msgData = self.decodeData(self.msgDataLen)
		else:
			logE("Error: ModbusMsg invalid Function code : %02X" % self.FunctionCode)
			return False

		# 头部6字节数据
		self.packet = self.msgBuffer[self.pos - (self.DataLength + 6):self.pos]

		# if self.left() > 0:
		# 	logE("ModbusMsg save since left(%s)>0" % self.left())
		# 	self.save()

		return True

	def setModbusMsgData(self, msgData):
		self.msgData = msgData
		self.msgDataLen = len(msgData)

	def toString(self):
		if self.isAckMsg:
			retStr = ("TransactionID : %04x, ProtocolID : %04x, DataLength : %04x, UnitID : %02x, FunctionCode : %02x, RegisterAddr : %04x, RegisterCount : %04x, msgDataLen : %02x, data : [%s]"
					% (self.TransactionID, self.ProtocolID, self.DataLength, self.UnitID, self.FunctionCode, self.RegisterAddr, self.RegisterCount, self.msgDataLen, ConvertToHex(self.msgData)))
		else:
			retStr = ("TransactionID : %04x, ProtocolID : %04x, DataLength : %04x, UnitID : %02x, FunctionCode : %02x, RegisterAddr : %04x, RegisterCount : %04x, msgDataLen : %02x, data : [%s]"
					% (self.TransactionID, self.ProtocolID, self.DataLength, self.UnitID, self.FunctionCode, self.RegisterAddr, self.RegisterCount, self.msgDataLen, ConvertToHex(self.msgData)))
		return retStr

# Modbus 到站时间消息，由接收的综合监控多条消息构建，为内部消息
@setMsgType(ModbusStationTimeMsg)
class CModbusStationTimeMsg(CBaseMsg):
	def __init__(self, MsgType) :
		CBaseMsg.__init__(self)

		self.msgType 		= MsgType

		self.InitMsg()

	def InitMsg(self):
		self.stationID 			= 0xFFFF									# 目的站点ID
		self.arriveHour 		= 0xFF 										# 到站时间的小时值
		self.arriveMinute 		= 0xFF 										# 到站时间的分钟值

		self.nextStationID 		= 0xFFFF									# 目的站点ID
		self.nextArriveHour 	= 0xFF 										# 到站时间的小时值
		self.nextArriveMinute 	= 0xFF 										# 到站时间的分钟值

	# 信息是否已经接收完整
	def isMsgComplete(self):
		if self.stationID == 0xFFFF or self.arriveHour == 0xFF or self.arriveMinute == 0xFF or self.nextStationID == 0xFFFF or self.nextArriveHour == 0xFF or self.nextArriveMinute == 0xFF:
			return False

		return True

	def getMsgTypeStr(self):
		if self.msgType == ModbusDownTimeMsg:
			return "Modbus Send Down Arriving Infomation Msg"

		elif self.msgType == ModbusUpTimeMsg:
			return "Modbus Send Up Arriving Infomation Msg"

		else:
			return "Error Modbus Msg"

	def encode(self):
		self.encodeUINT(self.msgType)
		self.encodeWord(self.stationID)
		self.encodeWord(self.arriveHour)
		self.encodeWord(self.arriveMinute)

		self.encodeWord(self.nextStationID)
		self.encodeWord(self.nextArriveHour)
		self.encodeWord(self.nextArriveMinute)

	def toString(self):

		string  = "%s, Arrive Station ID: %d, Arrive Time: %d hour, %d minute"%(self.getMsgTypeStr(), self.stationID, self.arriveHour, self.arriveMinute)
		string2 = "%s, Next Arrive Station ID: %d, Arrive Time: %d hour, %d minute"%(self.getMsgTypeStr(), self.nextStationID, self.nextArriveHour, self.nextArriveMinute)
		return string + string2

# Modbus 发送紧急消息，由接收的综合监控多条消息构建，为内部消息
@setMsgType(ModbusEmergenceMsg)
class CModbusEmergenceMsg(CBaseMsg):
	def __init__(self) :
		CBaseMsg.__init__(self)
		
		self.InitMsg()
		
	def InitMsg(self):

		self.selectedCtrl 	= 0xFF 										# 选择的控制器编号，0x0b:站厅控制器，0x0c：上行控制器，0x0d：下行控制器, 0x15：所有控制器
		self.selectMode 	= False 									# 是选择还是取消
		self.emergenceMode 	= 0xFF 										# 模式，0：滚动，1：全屏
		self.msgContent 	= None										# 消息内容

	# 信息是否已经接收完整
	def isMsgComplete(self):
		if self.selectedCtrl == 0xFF or self.selectMode == 0xFF or self.emergenceMode == 0xFF or self.msgContent == None:
			return False

		return True

	def encode(self):
		self.encodeUINT(self.msgType)
		self.encodeByte(self.selectedCtrl)
		self.encodeByte(self.selectMode)
		self.encodeByte(self.emergenceMode)
		self.encodeString(self.msgContent)

	def toString(self):
		if self.selectedCtrl == MB_ADDR_CTRL_HC:
			ctrlName = "Hall Controller"
		elif self.selectedCtrl == MB_ADDR_CTRL_UC:
			ctrlName = "Up Controller"
		elif self.selectedCtrl == MB_ADDR_CTRL_DC:
			ctrlName = "Down Controller"
		elif self.selectedCtrl == MB_ADDR_MSG_SEL_ALL:
			ctrlName = "All Controller"
		else:
			ctrlName = "Unknown Controller"

		selectMode = "Select" if self.selectMode else "Unselect"
		mode = "scroll" if self.emergenceMode == 0 else "Full screen"

		return "%s, controller: %s, Select Mode: %s, Show Mode: %s, Content: %s"%(self.getMsgTypeStr(), ctrlName, selectMode, mode, self.msgContent)

