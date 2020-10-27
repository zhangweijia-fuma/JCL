#coding:utf-8

import sys, struct, time, threading, random, datetime
from utils.utils import *

from config import PisConfig
from libs.db.dbHelper import *
from libs.utils.debug import *

from libs.JCL.JCLMsgBase import *
from libs.JCL.JCLCommon import *


RSU_MSG_TYPE_BASE       	= 0x0300

RSU_MSG_InitialRequest 		= RSU_MSG_TYPE_BASE + 1
RSU_MSG_InitialRequestAck 	= RSU_MSG_TYPE_BASE + 2
RSU_MSG_UpdateSoftware 		= RSU_MSG_TYPE_BASE + 3
RSU_MSG_UpdateSoftwareAck	= RSU_MSG_TYPE_BASE + 4
RSU_MSG_Reboot 				= RSU_MSG_TYPE_BASE + 5
RSU_MSG_RebootAck			= RSU_MSG_TYPE_BASE + 6
RSU_MSG_SetParam 			= RSU_MSG_TYPE_BASE + 7
RSU_MSG_SetParamAck 		= RSU_MSG_TYPE_BASE + 8
RSU_MSG_GetParam 			= RSU_MSG_TYPE_BASE + 9
RSU_MSG_GetParamAck 		= RSU_MSG_TYPE_BASE + 10
RSU_MSG_ReportStatus 		= RSU_MSG_TYPE_BASE + 11
RSU_MSG_ReportStatusAck		= RSU_MSG_TYPE_BASE + 12


# 消息列表
RSUMessageTypeList = [
	HeartBeatMsg,
	HeartBeatAckMsg,

	RSU_MSG_InitialRequest,
	RSU_MSG_InitialRequestAck,
	RSU_MSG_UpdateSoftware,
	RSU_MSG_UpdateSoftwareAck,
	RSU_MSG_Reboot,
	RSU_MSG_RebootAck,
	RSU_MSG_SetParam,
	RSU_MSG_SetParamAck,
	RSU_MSG_GetParam,
	RSU_MSG_GetParamAck,

	RSU_MSG_ReportStatus,
	RSU_MSG_ReportStatusAck,
]

# 需要接收下载完成消息的消息列表
RSUMsgForDownloadList = [ 

]

# 不需要显示收发记录的消息
noShowMsgList = [
	HeartBeatMsg,
	HeartBeatAckMsg,
	
	RSU_MSG_ReportStatus,
	RSU_MSG_ReportStatusAck,
]


MAX_RSU_MSG_PARAM 			= 120                             			# RSU可设置的最多参数数量

# 参数Tag列表
Tag_RSUID 					= 0
Tag_longtitude				= 1
Tag_Latitude 				= 2
Tag_Temperature 			= 3
Tag_Humidity 				= 4
TAG_Invalid 				= MAX_RSU_MSG_PARAM

# 更新模式
RSU_Update_NextStart 		= 0                                         # 下次重启时更新
RSU_Update_Immediately 		= 1                                     	# 立即重启

# 重启模式
Reboot_Application 			= 0                                         # 重启应用
Reboot_System 				= 1                                         # 重启硬件


RPT_String 					= 0                                         # 字符串类型参数
RPT_UINT 					= 1
RPT_INT 					= 2
RPT_SHORT 					= 3
RPT_USHORT 					= 4
RPT_BYTE 					= 5
RPT_Invalid					= 6


class CRSUMsgParam():
	def __init__(self) :
		self.nParamTag 		= TAG_Invalid                               # 参数的Tag
		self.ParamData 		= None
		self.ParamType 		= RPT_Invalid


	def GetParamTag(self):
		return self.nParamTag

	def GetDataType(self):
		return self.RSUParamType

	def GetParamData(self):
		return m_nIntData

	def SetParamData(self, nTag, ParamType, ParamData):
		self.nParamTag = nTag
		self.ParamData = ParamData
		self.ParamType = ParamType
