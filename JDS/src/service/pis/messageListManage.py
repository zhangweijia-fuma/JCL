#coding:utf-8

import re
import time
import datetime
import sys

from db import dbMysql
from web.baseException import errorDic,BaseError
from web.Base  import WebRequestHandler,BaseError,operator_except

from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE
from libs.utils import utils
from libs.utils.debug import *

import xlrd, xlwt
from public.excel import excel
# from pis.controller import controller

class messageListManage(dbMysql.CURD) :
	def __init__(self,db) :
		if sys.version > '3':
			# python 3.0 +
			super().__init__(db,'pis.message_log',False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(messageListManage,self).__init__(db,'pis.message_log',False)

class Restful(WebRequestHandler):

	# 获取消息列表
	# 参数：
	# 	sid: 发送方ID
	# 	sname : 发送方名称
	# 	sd：发送日期
	# 	mt：消息类型
	# 	ss: 发送状态
	# 	
	@operator_except
	def get(self):

		op 		= self.get_argument('op', default = "data")
		sid		= int(self.get_argument('sid', default = '0'))
		sname 	= self.get_argument('sname', default = "")
		sd 		= self.get_argument('sd', default = "")
		mt		= self.get_argument('mt', default = "")
		ss 		= self.get_argument('ss', default = "")
		offset 	= int(self.get_argument('o', default = '1'))
		rowlimit= int(self.get_argument('r', default = '20'))

		# 默认使用内部消息
		dataSource = self.get_argument('ds', default = 'internal')

		# 查询历史记录还是当前记录，默认采用当前数据记录
		dataType = self.get_argument('dt', default = 'current')

		if dataType == "current":
			tableName = "pis.message_list"
		elif dataType == "history":
			tableName = "pis.message_list_history"
		else:
			raise BaseError(801, "参数错误：查询类型错误！") 

		offset = ( offset - 1 ) * rowlimit
		sql_limit = " limit " + str(rowlimit) + " offset " + str(offset)

		sql  = "select msg.id, msg.operation_id1, msg.operation_id2, msg.message_type, msg.content, msg.sender_id, msg.sender_name, "
		sql += " msg.send_date, msg.send_time, msg.send_status"
		sql += " from %s msg"%tableName

		sql_where = ""

		if sid > 0 and sname != "":
			sql_where = " where msg.sender_id = %d and msg.sender_name = '%s' "%(sid, sname)

		if sd != "":
			if sql_where == "":
				sql_where = " where msg.send_date = '%s' "%sd
			else:
				sql_where += " and msg.send_date = '%s' "%sd

		if mt != "":
			if sql_where == "":
				sql_where = " where msg.message_type = '%s' "%mt
			else:
				sql_where += " and msg.message_type = '%s' "%mt

		if ss != "":
			if sql_where == "":
				sql_where = " where msg.send_status = '%s' "%ss
			else:
				sql_where += " and msg.send_status = '%s' "%ss

		sql += sql_where
		sql += " order by msg.send_date DESC, msg.send_time DESC"

		if op == "data":
			sql += sql_limit

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		if op == "excel":
			self.exportToExcel( rows )
		else:
			rowdata = {}

			messageList = {}
			messageList['rows'] = rows
			messageList['struct']  = "id, operation_id1, operation_id2, message_type, content, sender_id, sender_name, send_date, send_time, send_status"

			sql  = "select count(msg.id) from %s msg "%tableName
			sql += sql_where
			
			cur.execute(sql)
			row = cur.fetchone() 
			messageList['count'] = row[0]

			self.response( messageList )

	# 输出到excel文件
	def exportToExcel(self, rowdata):

		key  = ['id', 'operation_id1', 'operation_id2', 'message_type', 'content', 'sender_id', 'sender_name', 'send_date', 'send_time', 'send_status']

		messageList = []
		for i, item in enumerate(rowdata):
			messageInfo = dict(zip(key, item))
			messageList.append(messageInfo)

		cur = self.db.getCursor()
		utils.getDataDict(cur, messageList, "send_status", "PROGRAM_SEND_STATUS", "send_status_name")
		utils.getDataDict(cur, messageList, "message_type", "MESSAGE_TYPE", "message_type_name")

		data = []
		sn = 1
		for messageInfo in messageList:

			itemData = []
			itemData.append( sn )
			sn = sn + 1
			itemData.append( messageInfo['sender_name'] )
			itemData.append( messageInfo['send_date'] )
			itemData.append( messageInfo['send_time'])	
			itemData.append( messageInfo['message_type_name'] )
			itemData.append( messageInfo['content'] )
			itemData.append( messageInfo['send_status_name'] )
			
			data.append(itemData) 

		struct = "SN, 发送方名称, 发送日期, 发送时间, 消息类别, 消息正文, 发送状态"

		path = {}
		path = excel.createTempFile("xls")
	
		excel.saveExcel(path['path'], struct, data )
		logInfo = " %s 下载了消息清单 "%self.objUserInfo['name']
		operation_log(self.db).addLog(self.GetUserInfo(), "messageListManage", logInfo, 0)

		self.response(path)


	@operator_except
	def patch(self):

		s = messageListManage(self.db)

		paramData = self.getRequestData()
		dataType  = self.getParam( paramData, 'dt', 'current')
		if dataType == "current":
			tableName = "pis.message_list"
		elif dataType == "history":
			tableName = "pis.message_list_history"
		else:
			raise BaseError(801, "参数错误：查询来源类型错误！") 

		data = {}

		cur = self.db.getCursor()

		# 发送方列表
		sql = "select distinct(sender_name), sender_id from %s where sender_name is not null order by sender_name"%tableName
		cur.execute(sql)
		rows = cur.fetchall()

		senderList = {
			'rows' : [("选择全部", 0, "")] + rows,
			'struct' : 'name, id',
		}

		data['senderList'] = senderList

		# 消息类别
		sql = "select cv.name, cv.code from system.code_value cv "
		sql += " where cv.type_code = 'MESSAGE_TYPE' "
		sql += " and to_number(cv.code, '999') < 100 and to_number(cv.code, '999') > 1 "
		sql += " and POSITION( 'ACK' in cv.description ) = 0"
		sql += " order by cv.sort"
		cur.execute(sql)
		rows = cur.fetchall()

		msgTypeList = {
			'rows' : [("选择全部", '0', "")] + rows,
			'struct' : 'name, code',
		}
		data['msgTypeList'] = msgTypeList

		# 消息发送状态
		sql = "select cv.name, cv.code from system.code_value cv where cv.type_code = 'PROGRAM_SEND_STATUS' order by cv.sort "
		cur.execute(sql)
		rows = cur.fetchall()

		sendStatusList = {
			'rows' : [("选择全部", '0', "")] + rows,
			'struct' : 'name, code',
		}
		data['sendStatusList'] = sendStatusList

		self.response(data)

