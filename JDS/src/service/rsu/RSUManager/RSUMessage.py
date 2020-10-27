#coding:utf-8

import sys, struct, time, threading, random, datetime
from utils.utils import *

from config import PisConfig
from libs.db.dbHelper import *
from libs.utils.debug import *

from libs.JCL.JCLMsgBase import *
from libs.JCL.JCLCommon import *

from rsu.RSUManager.RSUMsgBase import *

@setMsgType(RSU_MSG_InitialRequest)
class CRSUInitialRequestMsg(CJCLMsgBase):
	def __init__(self) :
		CJCLMsgBase.__init__(self)

		self.msgType 		= RSU_MSG_InitialRequest

		self.longtitude 	= 0
		self.latitude 		= 0

	def encode(self):
		self.pos = 0

		if self.encodeInt(self.longtitude) == False:
			return False

		if self.encodeInt(self.latitude) == False:
			return False

		return True

	def decode(self, dataBuf):
		self.setMsgBuffer(dataBuf)
		
		if self.decodeHead() == False:
			return False

		self.longtitude = self.decodeInt()
		self.latitude 	= self.decodeInt()
		
		if self.decodeEnd() == False:
			return False

		return True

	def toString(self):
		return "RSU InitialRequestMsg"

@setMsgType(RSU_MSG_InitialRequestAck)
class CRSUInitialRequestAckMsg(CJCLBaseAckMsg):
	def __init__(self) :
		CJCLBaseAckMsg.__init__(self)
		self.msgType 	= RSU_MSG_InitialRequestAck

	def toString(self):
		return "RSU InitialRequestAckMsg"

@setMsgType(RSU_MSG_UpdateSoftware)
class CRSUUpdateSoftwareMsg(CJCLMsgBase):
	def __init__(self) :
		CJCLMsgBase.__init__(self)

		self.msgType 		= RSU_MSG_UpdateSoftware

		self.strFileName 	= ""
		self.nUpdateMode 	= RSU_Update_Immediately

	def setParams(self, strFileName, nUpdateMode):
		self.strFileName 	= strFileName
		self.nUpdateMode 	= nUpdateMode

	def encode(self):
		self.pos = 0

		if self.encodeString(self.strFileName) == False:
			return False

		if self.encodeByte(self.nUpdateMode) == False:
			return False

		return True

	def decode(self, dataBuf):
		
		self.setMsgBuffer(dataBuf)
		
		if self.decodeHead() == False:
			return False

		self.strFileName = self.decodeString()
		self.nUpdateMode = self.decodeByte()

		if self.decodeEnd() == False:
			return False

		return True

	def toString(self):
		return "RSU UpdateSoftwareMsg"

@setMsgType(RSU_MSG_UpdateSoftwareAck)
class CRSUUpdateSoftwareAckMsg(CJCLBaseAckMsg):
	def __init__(self) :
		CJCLBaseAckMsg.__init__(self)
		
		self.msgType 		= RSU_MSG_UpdateSoftwareAck

		# 无参数

	def toString(self):
		return "RSU UpdateSoftwareAckMsg"

@setMsgType(RSU_MSG_Reboot)
class CRSURebootMsg(CJCLMsgBase):
	def __init__(self) :
		CJCLMsgBase.__init__(self)

		self.msgType 		= RSU_MSG_Reboot

		self.nDelaySecond 	= 0
		self.nRebootMode 	= 0

	def setParams(self, nDelaySecond, nRebootMode):
		self.nDelaySecond 	= nDelaySecond
		self.nRebootMode 	= nRebootMode

	def encode(self):

		if self.encodeUINT(self.nDelaySecond) == False:
			return False

		if self.encodeByte(self.nRebootMode) == False:
			return False

		return True

	def decode(self, dataBuf):

		self.setMsgBuffer(dataBuf)
		
		if self.decodeHead() == False:
			return False

		self.nDelaySecond 	= self.decodeUINT()
		self.nRebootMode	= self.decodeByte()

		if self.decodeEnd() == False:
			return False

		return True

	def toString(self):
		return "RSU RebootMsg"

@setMsgType(RSU_MSG_RebootAck)
class CRSURebootAckMsg(CJCLBaseAckMsg):
	def __init__(self) :
		CJCLBaseAckMsg.__init__(self)

		self.msgType 		= RSU_MSG_RebootAck

		# 无参数

	def toString(self):
		return "RSU RebootAckMsg"

@setMsgType(RSU_MSG_SetParam)
class CRSUSetParamMsg(CJCLMsgBase):
	def __init__(self) :
		CJCLMsgBase.__init__(self)

		self.msgType 		= RSU_MSG_SetParam

		self.ParamList 		= []


	def setParams(self, Param):
		self.ParamList.append(Param)

	def encode(self):
		
		if self.encodeByte(self.ParamList.length) == False:
			return False

		for item in self.ParamList:
			if self.encodeByte(item['tag']) == False:
				return False

			dataType = item['dataType']
			if self.encodeByte(dataType) == False:
				return False

			if dataType == RPT_String:
				if self.encodeString(item['data']) == False:
					return False
			if dataType == RPT_UINT:
				if self.encodeUINT(item['data']) == False:
					return False
			if dataType == RPT_INT:
				if self.encodeInt(item['data']) == False:
					return False
			if dataType == RPT_SHORT:
				if self.encodeShort(item['data']) == False:
					return False
			if dataType == RPT_USHORT:
				if self.encodeUShort(item['data']) == False:
					return False
			if dataType == RPT_BYTE:
				if self.encodeByte(item['data']) == False:
					return False

		return

	def decode(self, dataBuf):
		self.setMsgBuffer(dataBuf)
		
		if self.decodeHead() == False:
			return False

		nParamCount	= self.decodeByte()
		if nParamCount > 0 :
			for i in range(0, nParamCount):
				nTag = self.decodeByte()
				dataType = self.decodeByte()
				if dataType == RPT_String:
					data = self.decodeString()
				elif dataType == RPT_UINT:
					data = self.encodeUINT()
				elif dataType == RPT_INT:
					data = self.decodeInt()
				elif dataType == RPT_SHORT:
					data = self.decodeShort()
				elif dataType == RPT_USHORT:
					data = self.decodeUShort()
				elif dataType == RPT_BYTE:
					data = self.decodeByte()
				else:
					return False

				item = { 
					'tag' 		: nTag,
					'dataType' 	: dataType,
					'data' 		: data,
				}

				self.ParamList.append(item)

		if self.decodeEnd() == False:
			return False

		return True

	def toString(self):
		return "RSU SetParamMsg"

@setMsgType(RSU_MSG_SetParamAck)
class CRSUSetParamAckMsg(CJCLBaseAckMsg):
	def __init__(self) :
		CJCLBaseAckMsg.__init__(self)

		self.msgType 		= RSU_MSG_SetParamAck

		# 无参数

	def toString(self):
		return "RSU SetParamAckMsg"

@setMsgType(RSU_MSG_GetParam)
class CRSUGetParamMsg(CJCLMsgBase):
	def __init__(self) :
		CJCLMsgBase.__init__(self)

		self.msgType 		= RSU_MSG_GetParam

		self.ParamTagList 	= []

	def setParams(self, ParamTag):
		self.ParamTagList.append(ParamTag)

	def encode(self):
		
		if self.encodeByte(self.ParamList.length) == False:
			return False

		for item in self.ParamTagList:
			if self.encodeByte(item) == False:
				return False

		return True

	def decode(self, dataBuf):
		self.setMsgBuffer(dataBuf)
		
		if self.decodeHead() == False:
			return False

		nParamCount = self.decodeByte()
		if nParamCount < 0 and nParamCount >= MAX_RSU_MSG_PARAM:
			logE("Error: nParamCount out of range[0, %d]"%MAX_RSU_MSG_PARAM)
			return False

		for i in range(0, nParamCount):
			nTag = self.decodeByte()
			self.ParamTagList.append(nTag)

		if self.decodeEnd() == False:
			return False

		return True

	def toString(self):
		return "RSU SetParamMsg"

@setMsgType(RSU_MSG_GetParamAck)
class CRSUGetParamAckMsg(CJCLBaseAckMsg):
	def __init__(self) :
		CJCLBaseAckMsg.__init__(self)

		self.msgType 		= RSU_MSG_GetParamAck

		# 无参数

		self.ParamList 		= []


	def setParams(self, Param):
		self.ParamList.append(Param)

	def encode(self):
		
		if self.encodeByte(self.ParamList.length) == False:
			return False

		for item in self.ParamList:
			if self.encodeByte(item['tag']) == False:
				return False

			dataType = item['dataType']
			if self.encodeByte(dataType) == False:
				return False

			if dataType == RPT_String:
				if self.encodeString(item['data']) == False:
					return False
			if dataType == RPT_UINT:
				if self.encodeUINT(item['data']) == False:
					return False
			if dataType == RPT_INT:
				if self.encodeInt(item['data']) == False:
					return False
			if dataType == RPT_SHORT:
				if self.encodeShort(item['data']) == False:
					return False
			if dataType == RPT_USHORT:
				if self.encodeUShort(item['data']) == False:
					return False
			if dataType == RPT_BYTE:
				if self.encodeByte(item['data']) == False:
					return False

		return

	def decode(self, dataBuf):
		self.setMsgBuffer(dataBuf)
		
		if self.decodeHead() == False:
			return False

		nParamCount	= self.decodeByte()
		if nParamCount > 0 :
			for i in range(0, nParamCount):
				nTag = self.decodeByte()
				dataType = self.decodeByte()
				if dataType == RPT_String:
					data = self.decodeString()
				elif dataType == RPT_UINT:
					data = self.encodeUINT()
				elif dataType == RPT_INT:
					data = self.decodeInt()
				elif dataType == RPT_SHORT:
					data = self.decodeShort()
				elif dataType == RPT_USHORT:
					data = self.decodeUShort()
				elif dataType == RPT_BYTE:
					data = self.decodeByte()
				else:
					return False

				item = { 
					'tag' 		: nTag,
					'dataType' 	: dataType,
					'data' 		: data,
				}

				self.ParamList.append(item)

		if self.decodeEnd() == False:
			return False

		return True

	def toString(self):
		return "RSU GetParamMsg"


@setMsgType(RSU_MSG_ReportStatus)
class CRSUReportStatusMsg(CJCLMsgBase): 											# 运营状态报告
	def __init__(self) :
		CJCLMsgBase.__init__(self)

		self.cpu 		= 0 											# CPU占有率，值小于10000，为百分比值*100
		self.memory 	= 0 											# 内存使用率，值小于10000，为百分比值*100
		self.harddisk 	= 0 											# 硬盘使用率，值小于10000，为百分比值*100

	def setParams(self, cpu, memory, harddisk):
		self.cpu 		= cpu
		self.memory 	= memory
		self.harddisk 	= harddisk

	def encode(self):
		self.encodeUINT(self.cpu)
		self.encodeUINT(self.memory)
		self.encodeUINT(self.harddisk)
		return True

	def decode(self, data):
		self.setMsgBuffer(data)

		if self.decodeHead() == False:
			return False
		
		self.cpu 		= self.decodeUINT()
		self.memory 	= self.decodeUINT()
		self.harddisk 	= self.decodeUINT()
		if self.decodeEnd() == False:
			return False

		return True

	def toParamString(self):
		return "CPU: %0.2f%%, 内存: %0.2f%%, 硬盘: %0.2f%%" % (self.cpu/100, self.memory/100, self.harddisk/100)

	def toString(self):
		return "RSU ReportStatusMsg"

@setMsgType(RSU_MSG_ReportStatusAck)
class CRSUReportStatusAckMsg(CJCLBaseAckMsg):
	def __init__(self) :
		CJCLBaseAckMsg.__init__(self)

		self.msgType 		= RSU_MSG_ReportStatusAck
		
		# 无参数

	def toString(self):
		return "RSU ReportStatusAckMsg"


# @setMsgType(DownloadCompleteMsg)
# class CDownloadCompleteMsg(CJCLMsgBase): 									# 下载完成通知消息
# 	def __init__(self) :
# 		CJCLMsgBase.__init__(self)
# 		self.requestMsgType = None 										# 下载完成的请求类别，为：节目单、安全垫片或节目单

# 	def setParams(self, requestMsgType):
# 		self.requestMsgType = requestMsgType

# 	def encode(self):
# 		self.encodeUINT(self.requestMsgType)
# 		return self.getMsgBuffer()

# 	def decode(self, data) :
# 		self.setMsgBuffer(data)
# 		self.decodeHead()
# 		self.requestMsgType = self.decodeUINT()
# 		self.decodeEnd()

# 	def encode(self):
# 		self.encodeUINT(self.requestMsgType)
# 		return self.getMsgBuffer()

# 	def toString(self):
# 		return "下载完成消息，请求消息类型: %d"%self.requestMsgType

# @setMsgType(DownloadCompleteAck, isAckMsg = True)
# class CDownloadCompleteAck(CJCLBaseAckMsg):
# 	def __init__(self) :
# 		CJCLBaseAckMsg.__init__(self)
# 		# 无参数

# 	def toString(self):
# 		return "下载完成响应消息"


