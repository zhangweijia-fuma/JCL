#coding:utf-8

import re
import time
import datetime
import sys
import os
import _thread

from db import dbMysql
from web.baseException import errorDic,BaseError
from web.Base  import WebRequestHandler,BaseError,operator_except

import config
from config import *
from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE
from libs.utils import utils
from libs.utils.debug import *

from pis.controller.message import *
import xlrd, xlwt
from public.excel import excel

from pis.controller import controller as Controller
from pis.PISMessage.PISMsgServer import *

class messageManage(dbMysql.CURD) :
	def __init__(self,db) :
		if sys.version > '3':
			# python 3.0 +
			super().__init__(db,'pis.message_log',False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(messageManage,self).__init__(db,'pis.message_log',False)


# 线程函数，在发送消息给控制器时通过线程发送，以避免因同时向多个控制器发消息时造成前台响应缓慢
def SendMessage(controllerList, msgData):
	MsgServer.sendMessage(controllerList, msgData)
	return

def SendMsgToController(self, paramData):

	if config.PisConfig['stationServer'] == True:
		raise BaseError(801, "服务器运行模式：车站服务器！")

	# 发送消息

	s = messageManage(self.db)
	cur = self.db.getCursor()

	msgType = self.getParam( paramData, "msgType", "")
	if msgType == "":
		raise BaseError(801, "参数错误：缺少消息类别")
	
	controllerList = self.getParam( paramData, "controllerList", [])
	if len(controllerList) == 0:
		raise BaseError(801, "参数错误：无控制器列表!")

	logInfo = ""
	data = {}
	msgData = []
	if msgType == "list":
		# 发送节目单
		lid = int(self.getParam(paramData, "id", "0"))
		
		if lid == 0:
			raise BaseError(801, "参数错误：无节目单ID！")

		sql  = " select pl.name from pis.program_list pl where pl.id = %d"%lid
		cur.execute(sql)
		rows = cur.fetchall()
		if len(rows) == 0:
			raise BaseError(801, "参数错误：节目单ID错误，找不到数据！")

		# 生成节目单文件
		file_name = self.createProgramListFile(lid)

		msgData.append(file_name)

		data['operation_id1'] = lid
		data['operation_id2'] = 0

		data['message_type'] = SendProgramListMsg
		data['content'] = '发送节目单 - %s '%rows[0][0]

		logInfo = "%s 发送节目单：%s"%(self.objUserInfo['name'], data['content'])

	elif msgType == "format_file":
		# 更新版式文件发送状态，版本文件可以多次发送
		formatFile = self.getParam( paramData, "file", {})
		if "id" not in formatFile.keys():
			raise BaseError(801, "参数错误：无版式文件名！")

		data['content'] 		= "发送版式文件：%s, operation_id1: %d"%(formatFile['file_name'], formatFile['id'])
		data['operation_id1'] 	= formatFile['id']
		data['operation_id2'] 	= 0

		msgData.append(formatFile['file_path'])
		data['message_type'] = SendFormatFileMsg

		logInfo = "%s 发送版式文件：%s"%(self.objUserInfo['name'], data['content'])

	elif msgType == "news":
		data['message_type'] = SendNewsMsg
		data['content'] = self.getParam( paramData, "content", "")
		if data['content'] == "":
			raise BaseError(801, "参数错误：无新闻正文！")

		lifeTime = int(self.getParam( paramData, "LifeTime", "0")) 
		msgData.append(lifeTime)

		data['operation_id1'] = 0
		data['operation_id2'] = 0

		logInfo = "%s 发送新闻：%s"%(self.objUserInfo['name'], data['content'])

	elif msgType == "emergnce":
		data['message_type'] = SendEmergenceMsg

		emgcContent = self.getParam( paramData, "content", " ")
		if emgcContent == " ":
			raise BaseError(801, "参数错误：无消息正文！")

		data['content'] = emgcContent

		mode = int(self.getParam( paramData, "mode", ""))
		msgData.append(mode)
		lifeTime = int(self.getParam( paramData, "LifeTime", "0")) 
		msgData.append(lifeTime)

		data['operation_id1'] 	= 0 
		data['operation_id2'] 	= 0
		
		logInfo = "%s 发送紧急消息：%s"%(self.objUserInfo['name'], data['content'])

	elif msgType == "reboot":
		data['message_type'] = SystemRebootMsg
		rebootMode = int(self.getParam(paramData, "rebootMode", '0'))
		msgData.append(rebootMode)

		rebootDelayTime = int(self.getParam(paramData, "rebootDelayTime", '0'))
		msgData.append(rebootDelayTime)
		logI("mode: %d, time: %d"%(rebootMode, rebootDelayTime))

		data['content'] = "发送控制器重启命令"
		data['operation_id1'] = 0
		data['operation_id2'] = 0

		logInfo = "%s %s"%(self.objUserInfo['name'], data['content'])

	elif msgType == "update":
		data['message_type'] = SendUpdateMsg
		file_name = self.getParam( paramData, "file_name", "")
		if file_name == "":
			raise BaseError(801, "参数错误：无版本文件名称！")

		file_path = self.getParam( paramData, "file_path", "")
		if file_path == "":
			raise BaseError(801, "参数错误：无版本文件路径！")

		msgData.append( os.path.basename(file_path))

		data['content'] = '发送版本更新消息：%s'%file_name 
		data['operation_id1'] = 0
		data['operation_id2'] = 0

		logInfo = "%s %s"%(self.objUserInfo['name'], data['content'])

	elif msgType == 'work_time':
		data['message_type'] = UpdateWorkTimeMsg
		open_time = self.getParam( paramData, "open_time", "")
		close_time = self.getParam( paramData, "close_time", "")
		msgData.append(open_time)
		msgData.append(close_time)

		data['operation_id1'] = 0
		data['operation_id2'] = 0
		data['content']  = "开机时间：%s，休眠时间：%s" % (open_time, close_time)

		# 更新控制器开机及休眠时间
		for controller in controllerList:
			cTimeData = {
				'id' : controller['id'],
				'open_time' : open_time,
				'close_time' : close_time
			}
			s.save( cTimeData, cTimeData['id'], table = 'pis.controller' )

		logInfo = "%s %s"%(self.objUserInfo['name'], data['content'])

	elif msgType == "videoSurv":
		# 在使用视频监播时，消息发给控制器开始或取消视频监播，由控制器发送视频给流媒体服务器，由流媒体服务器根据控制器编号生成URL。
		# 浏览器根据控制器编号生成URL到流媒体服务器上获取视频流
		data['message_type'] = ViewControllerVideoMsg
		start_end_flag = int(self.getParam( paramData, "mode", "0"))

		if start_end_flag != 0 and start_end_flag != 1:
			raise BaseError(801, "参数错误：无查看视频开始结束标志！")

		msgData.append(start_end_flag)
		data['content'] = '发送查看实时视频消息：' 
		data['operation_id1'] = 0
		data['operation_id2'] = 0

		logInfo = "%s %s"%(self.objUserInfo['name'], data['content'])

	elif msgType == "safe_file":
		data['message_type'] = SendSafeVideoMsg
		safeVideoFile = self.getParam( paramData, "file", {})
		
		msgData.append(safeVideoFile)
		data['content'] = "发送安全垫片：%s"%safeVideoFile['file_name'] 
		data['operation_id1'] = 0
		data['operation_id2'] = 0

		logInfo = "%s %s"%(self.objUserInfo['name'], data['content'])

	elif msgType == "line_info":
		data['message_type'] = SendLineInfoMsg
		lineInfo = paramData['lineInfo']
		msgData.append(lineInfo)
		data['content'] 		= "发送支线信息"
		data['operation_id1'] 	= 0
		data['operation_id2'] 	= 0
		logInfo = "%s %s"%(self.objUserInfo['name'], data['content'])

	else:
		raise BaseError(801, "参数错误：消息类型值 %s 错误！"%msgType)

	# 新增发送记录
	data['create_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
	data['create_id'] 	= self.objUserInfo['id']

	data['sender_id'] 	= self.objUserInfo['id']
	data['sender_name'] = self.objUserInfo['name']
	data['send_date'] 	= datetime.datetime.now().strftime('%Y-%m-%d')
	data['send_time'] 	= datetime.datetime.now().strftime('%H:%M:%S')

	data['send_status'] = '2' # 下发状态为待接收，待控制器发送响应消息后再更新状态

	id = s.save( data, table = "pis.message_list")
	data['id'] = id
	data['data'] = msgData

	_thread.start_new_thread(SendMessage, (controllerList, data, ))

	# 操作日志
	operation_log(self.db).addLog(self.GetUserInfo(), "messageManage", logInfo, id)

	return

# Message Body:
# create_id: 发送方ID
# create_time：发送时间
# sender_id：发送方ID
# sender_name：发送方姓名
# send_date：发送日期
# send_time：发送时间
# send_status：发送状态，初始为2
# message_type：消息类型
# content：消息内容
# operation_id1：附加操作ID1
# operation_id2：附加操作ID2
# id：消息记录ID
# data: 附加消息数据


class Restful(WebRequestHandler):

	# 获取消息列表
	# 参数：
	# 	op: data: 数据查询，excel：下载到excel
	# 	
	# 	控制器消息历史时使用：
	# 	cid: 只根据接收方或发送方的ID查询，
	# 	
	# 	节目单或版式文件查询发送记录时使用：
	# 	oi：针对message_list中的operation_id1 或 operation_id2进行查询
	# 	optype: list: 节目单数据，format：版式文件数据，
	# 	
	# 	消息查询时使用：
	# 	sid：发送方id
	# 	rid：接收方id
	# 	sd：发送日期
	# 	mid: 消息记录ID
	# 	ss : 发送状态，all：所有，success：只查询发送成功消息，failed：只查询发送失败消息
	# 	
	# 	oi 或 mid 必有一个出现

	# 	dt: current: 当前数据记录，history：历史数据记录
	# 	ds: 数据来源，internal，内部消息数据，external：外部数据
	@operator_except
	def get(self):

		op 		= self.get_argument('op', default = "data")
		offset 	= int(self.get_argument('o', default = '1'))
		rowlimit= int(self.get_argument('r', default = '20'))

		offset = ( offset - 1 ) * rowlimit
		sql_limit = " limit " + str(rowlimit) + " offset " + str(offset)

		sid		= int(self.get_argument('sid', default = '0'))
		rid		= int(self.get_argument('rid', default = '0'))
		sd 		= self.get_argument('sd', default = "")

		cid 	= self.get_argument('cid', default = "")

		oi 		= int(self.get_argument('oi', default = "0"))
		optype 	= self.get_argument('optype', default = '')

		mid 	= int(self.get_argument('mid', default = '0'))

		sendStatus 	= self.get_argument('ss', default = 'all')

		# 默认使用内部消息
		dataSource = self.get_argument('ds', default = 'internal')

		# 查询历史记录还是当前记录，默认采用当前数据记录
		dataType = self.get_argument('dt', default = 'current')

		if dataType == "current":
			tableName = "pis.message_log"
		elif dataType == "history":
			tableName = "pis.message_log_history"
		else:
			raise BaseError(801, "参数错误：查询来源类型错误！") 
		
		if cid != "":
			# 根据接收方或发送方ID进行查询

			sql = " select log.id, log.sender_name, log.sender_id, log.send_date, log.send_time, log.receiver_id, log.receiver_name, log.receiver_code, log.receive_date, "
			sql += " log.receive_time, log.message_type, log.message_sn, log.msg_list_id, log.bin_content, log.text_content, log.send_status "
			sql += " from %s log "%tableName
			
			sql_subquery = ""
			# sql_subquery = " ,(select msg_list_id, receiver_id, sender_id, id from pis.message_log where ( sender_id = %s or receiver_id = %s ) b"%( cid, cid)
			# sql_subquery += " where log.id = b.id and log.msg_list_id = b.msg_list_id and log.receiver_id = b.receiver_id "
			sql_where = "where ( sender_id = %s or receiver_id = %s ) and log.message_sn = 1 "%(cid, cid)

			sql += sql_subquery

			sql += sql_where

			# id 是随着时间逐渐增加的，倒叙排序就是按照最新时间进行排序
			sql += " order by log.id DESC"
			sql += sql_limit

			cur = self.db.getCursor()
			cur.execute( sql  )
			rows = cur.fetchall()

			rowdata = {}

			messageList = {}
			messageList['rows'] = rows
			messageList['struct']  = "id, sender_name, sender_id, send_date, send_time, receiver_id, receiver_name, receiver_code, receive_date, receive_time, "
			messageList['struct'] += " message_type,message_sn, msg_list_id, bin_content, text_content, send_status"

			cur.execute("select count(log.id) from " + tableName + " log " + sql_subquery + sql_where )
			row = cur.fetchone() 
			messageList['count'] = row[0]

		else:

			sql  = "select log.id, log.create_time, log.update_time, log.create_id, log.update_id, log.msg_list_id,"
			sql += " log.sender_type, log.sender_name, log.sender_code, log.send_date, log.send_time, log.sender_addr, "
			sql += " log.receiver_type, log.receiver_name, log.receiver_code, log.receive_date, log.receive_time, log.receiver_addr, "
			sql += " log.message_type, log.message_sn, log.content_type, log.bin_content, log.text_content, log.send_status "
			sql += " from %s log"%tableName
			
			sql_where = ""
			if oi > 0:
				mt = ""
				if optype == 'list':
					mt = '2'
				elif optype == 'format':
					mt = '16'

				sql_where = " where message_sn = 1 and message_type = '%s' and ( operation_id1 = %d or operation_id2 = %d )"%(mt, oi, oi)

			elif mid > 0:
				sql_where = " where message_sn = 1 and  msg_list_id = %d "%mid
			else:
				raise BaseError(801, "参数错误！")

			if sid > 0:
				sql_where += " and log.sender_id = %d "%sid

			if rid > 0:
				sql_where += " and log.receiver_id = %d "%rid

			if sd != "":
				sql_where += " and log.send_date = '%s' "%sd

			sql_status = sql_where
			if sendStatus != 'all':
				if sendStatus == 'success':
					sql_where += " and send_status = '1' "
				elif sendStatus == 'failed':
					sql_where += " and send_status <> '1' "

			sql += sql_where
			sql += " order by log.id DESC"

			if op == "data":
				sql += sql_limit

			cur = self.db.getCursor()
			cur.execute(sql)
			rows = cur.fetchall()

			if op == "excel":
				self.exportToExcel( rows )

			rowdata = {}

			messageList = {}
			messageList['rows'] = rows
			messageList['struct']  = "id, create_time, update_time, create_id, update_id, msg_list_id, sender_type, sender_name, sender_code, send_date, send_time, sender_addr, "
			messageList['struct'] += " receiver_type, receiver_name, receiver_code, receive_date, receive_time, receiver_addr,"
			messageList['struct'] += " message_type, message_sn, content_type, bin_content, text_content, send_status"

			sql  = "select count(log.id) from %s log "%tableName
			sql += sql_where
			
			cur.execute(sql)
			row = cur.fetchone() 
			messageList['count'] = row[0]

			# 查询发送成功、失败的消息数
			sql = "select count(*) from %s log "%tableName + sql_status + " and send_status = '1' "
			cur.execute(sql)
			row = cur.fetchone() 
			messageList['msgSuccess'] = row[0]

			sql = "select count(*) from %s log "%tableName + sql_status + " and send_status <> '1' "
			cur.execute(sql)
			row = cur.fetchone() 
			messageList['msgFailed'] = row[0]

		self.response( messageList )

	# 输出记录到excel文件
	def exportToExcel(self, rowdata):

		key  = ['id', 'create_time', 'update_time', 'create_id', 'update_id', 'msg_list_id', 'sender_type', 'sender_name', 'sender_code', 'send_date', 'send_time', 'sender_addr', 
		 'receiver_type', 'receiver_name', 'receiver_code', 'receive_date', 'receive_time', 'receiver_addr',
		 'message_type', 'message_sn', 'content_type', 'bin_content', 'text_content', 'send_status']

		messageList = []
		for i, item in enumerate(rowdata):
			messageInfo = dict(zip(key, item))
			messageList.append(messageInfo)

		cur = self.db.getCursor()
		utils.getDataDict(cur, messageList, "send_status", "PROGRAM_SEND_STATUS", "send_status_name")
		utils.getDataDict(cur, messageList, "message_type", "MSG_TYPE_CONTROLLER", "message_type_name")
		
		data = []
		sn = 1
		for item in messageList:

			if 'send_status_name' not in item:
				item['send_status_name'] = "未发送"

			if 'apply_status_name' not in item:
				item['apply_status_name'] = "未提交"

			itemData = []
			itemData.append( sn )
			sn = sn + 1
			
			itemData.append( messageInfo['sender_name'] )
			itemData.append( messageInfo['send_date'] )
			itemData.append( messageInfo['send_time'])	
			itemData.append( messageInfo['receiver_name'] )
			itemData.append( messageInfo['receive_date'] )
			itemData.append( messageInfo['receive_time'] )
			itemData.append( messageInfo['message_type'] )
			itemData.append( messageInfo['message_sn'] )
			itemData.append( messageInfo['text_content'] )
			
			data.append(itemData) 

		struct = "SN, 发送方名称, 发送日期, 发送时间, 接收方名称, 接收日期, 接收时间, 消息类别, 消息序号, 消息正文"

		path = {}
		path = excel.createTempFile("xls")

		excel.saveExcel(path['path'], struct, data )

		logInfo = " %s 下载了消息发送详单 "%self.objUserInfo['name']
		operation_log(self.db).addLog(self.GetUserInfo(), "messageManage", logInfo, 0)

		self.response(path)


	# 发送消息给控制器
	# 参数：
	# 	op: list 或 data，list：获取下拉列表，data：发送消息
	# 	msgType：消息类别，发送节目单、新闻、软件更新、控制器重启；
	# 	controllerList：接收消息的控制器列表，每个控制器含有控制器ID、编号及IP地址
	# 	其他消息参数

	@operator_except
	def patch(self):

		s = messageManage(self.db)

		paramData = self.getRequestData()

		op = self.getParam( paramData, "op", "")
		if op == "":
			raise BaseError(801, "参数错误：缺少操作类别")
		
		cur = self.db.getCursor()
		if op == "list":
			# 获取下拉列表

			data = {}

			# 发送方列表
			sql = "select distinct(sender_name), id, sender_code from pis.message_log where sender_name is not null order by sender_code"
			cur.execute(sql)
			rows = cur.fetchall()

			senderList = {
				'rows' : [("选择全部", 0, "")] + rows,
				'struct' : 'name, id, code',
			}

			data['senderList'] = senderList

			# 接收方列表
			sql = "select distinct(receiver_name), id, receiver_code from pis.message_log where receiver_name is not null order by receiver_code"
			cur.execute(sql)
			rows = cur.fetchall()

			receiverList = {
				'rows' : [("选择全部", 0, "")] + rows,
				'struct' : 'name, id, code',
			}
			data['receiverList'] = receiverList

			self.response(data)

		elif op == "data":
			# 发送消息

			SendMsgToController(self, paramData)
			self.response(0)

		elif op == "resend":
			# 重发机制还需要再测试
			logI("重发机制还需要再测试")
			self.response(0)

			# data 为mesage_log记录，重发时只针对单个控制器
			
			# 重新获取message_log 记录
			id = self.getParam(paramData, "id", 0)
			if id == 0:
				raise BaseError(801, "参数错误：无消息记录ID！")

			sql  = "select log.id, log.msg_list_id, log.operation_id2, log.operation_id2, log.sender_type, log.sender_name, log.sender_code, log.sender_addr, "
			sql += " log.receiver_type, log.receiver_name, log.receiver_code, log.receiver_addr, log.message_type, log.message_sn, "
			sql += " log.send_date, log.send_time, log.receive_date, log.receive_time, log.content_type, log.bin_content, log.text_content, "
			sql += " log.send_status"
			sql += " from pis.message_log log "
			sql += " where log.id = %d"%id

			cur.execute(sql)
			rows = cur.fetchall()
			if len(rows) == 0:
				raise BaseError(801, "数据错误：找不到消息记录！")

			key  = ['id', 'msg_list_id', 'operation_id1', 'operation_id2', 'sender_type', 'sender_name', 'sender_code', 'sender_addr', 
			'receiver_type', 'receiver_name', 'receiver_code', 'receiver_addr', 'message_type', 'message_sn', 
			'send_date', 'send_time', 'receive_date', 'receive_time', 'content_type', 'bin_content', 'text_content'
			'send_status']

			msg = dict(zip(key, rows[0]) )

			# 获取控制器列表，只有一个记录
			controllerlist = []

			sql  = " select ct.id, ct.ip_address, ct.install_type, ct.install_id, ct.code "
			sql += " from pis.controller ct "
			sql += " where ct.code = '%s'"%msg['receiver_code']

			cur = self.db.getCursor()
			cur.execute(sql)
			rows = cur.fetchall()
			if len(rows) == 0:
				raise BaseError(801, "参数错误：控制器编号错误！")

			key  = ['id', 'ip_address', 'install_type', 'install_id', 'code']

			for item in rows:
				controllerlist.append( dict(zip(key, item)) )

			# 重新构建 message_list 记录，控制器列表只有一个控制器数据
			sql = "select ml.id, ml.message_type, ml.operation_id1, ml.operation_id2, ml.content from pis.message_list ml where ml.id=%d"%msg['msg_list_id']
			cur.execute(sql)
			rows=cur.fetchall()
			if len(rows) == 0:
				raise BaseError(801, "数据错误：找不到消息记录！")

			key  = ['id', 'message_type', 'operation_id1', 'operation_id2', 'content']
			data = dict(zip(key, rows[0])) 

			if data['message_type'] == SendProgramListMsg:
				# 节目单消息，重新构建节目单文件
				file_path = self.createProgramListFile( data['operation_id1'])
				msgData = []
				msgData.append(file_path)
				data['data'] = msgData

			# elif data['message_type'] == SendFormatFileMsg:
			# 	# 版式文件，
			# 	sql = "select ff.file_path from pis.format_file ff where ff.id in (%s)"%data['operation_id1']
			# 	cur.execute(sql)
			# 	rows = cur.fetchall()
			# 	msgData = []
			# 	for row in rows:
			# 		msgData.append( row[0] )

			# 	data['data'] = msgData

			self.sendMessage( controllerlist, data, True )
			self.response(0)

		else:
			raise BaseError(801, "参数错误：操作类别值 %s 错误！"%op)

	# 构建节目单文件
	def createProgramListFile(self, lid):

		# 节目单文件存放目录, root/list/filename
		sql  = "select pl.valid_date, pl.video_type from pis.program_list pl where pl.id = %d"%lid
		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()
		if len(rows) == 0:
			raise BaseError(801, "参数错误：节目单ID错误，找不到数据！")

		valid_date 	= rows[0][0] 
		month 		= valid_date[0:7]
		video_type 	= rows[0][1]

		file_path = "%s/list"%(config.PisConfig['PISFileRoot'])
		if not os.path.exists(file_path):
			# 如果目录不存在，则创建文件夹
			os.makedirs(file_path) 

		file_name = "program_list_%d.txt"%lid
		file_path = "%s/list/program_list_%d.txt"%(config.PisConfig['PISFileRoot'], lid)

		content = "[PROGRAM]\r\n"
		content += "VALID_DATE=%s\r\n"%valid_date
		content += "ORDER=%d\r\n"%lid
		content += "LIVE_FLAG=%s\r\n"%video_type

		if video_type == '0':
			content += "LIVE_URL=\r\n"

			sql = "select pu.start_time, pu.end_time, pu.id from pis.program_unit pu where pu.list_id = %d order by pu.sort"%lid
			cur.execute(sql)
			rows = cur.fetchall()
			if len(rows) == 0:
				raise BaseError(801, "系统错误：节目栏数量为0！")

			content += "TOTAL_UNIT=%d\r\n"%len(rows)

			sn = 1
			for row in rows:
				content += "[UNIT%d]\r\n"%sn
				content += "START_TIME=%s\r\n"%row[0]
				content += "END_TIME=%s\r\n"%row[1]

				sql = "select pi.file_path from pis.program_info pi where pi.unit_id = %d order by pi.sort"%row[2]
				cur.execute(sql)
				fileList = cur.fetchall()
				content += "TOTAL_PROGRAM=%d\r\n"%len(fileList)
				sn2 = 1
				for file in fileList:
					content += "PROGRAM%d=%s\r\n"%(sn2, os.path.basename(file[0]))
					sn2 += 1

				sn += 1
		else:
			content += "LIVE_URL=%s\r\n"%pisConfig["LiveVideoURL"]
			content += "TOTAL_UNIT=0\r\n"

		file = open(file_path, "w", encoding="utf-8")
		file.write(content)
		file.close

		return file_name

	def sendMessage(self, controllerList, message, reSendFlag ):

		#调用接口发送消息
		MsgServer.sendMessage(controllerList, message)
		return

		# 仅用于测试
		for controller in controllerList:
			data = {}
			data['create_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
			data['create_id'] 	= self.objUserInfo['id']

			data['sender_name'] = "中心服务器"
			data['sender_type'] = "10"
			data['sender_code'] = "CCCCCC"
			data['sender_id'] 	= 0

			sql = ""
			if controller['install_type'] <= '1':
				sql  = "select ol.name || '-' || t.name || '-' || ct.name "
				sql += " from pis.controller ct"
				sql += " inner join pis.operation_line ol on ol.id = ct.line_id"
				sql += " inner join pis.train t on t.id = ct.install_id"
			else:
				sql  = "select ol.name || '-' || st.name || '-' || ct.name "
				sql += " from pis.controller ct"
				sql += " inner join pis.operation_line ol on ol.id = ct.line_id"
				sql += " inner join pis.station st on st.id = ct.install_id"

			sql += " where ct.id = %d"%controller['id']
			cur = self.db.getCursor()
			cur.execute(sql)
			rows = cur.fetchall()
			name = rows[0][0]

			data['receiver_id']   = controller['id']
			data['receiver_type'] = controller['install_type']
			data['receiver_name'] = name
			data['receiver_code'] = controller['code']
			data['receiver_addr'] = controller['ip_address']

			data['message_type']  = message['message_type']
			data['operation_id1'] = message['operation_id1']
			data['operation_id2'] = message['operation_id2']

			if reSendFlag:
				data['message_sn'] = 0
			else:
				data['message_sn'] = 1

			data['send_date'] 	= datetime.datetime.now().strftime('%Y-%m-%d')
			data['send_time'] 	= datetime.datetime.now().strftime('%H:%M:%S')

			data['bin_content'] = message['content']
			data['text_content'] = message['content']

			data['send_status'] = '2'
			data['msg_list_id'] = message['id']

			s = messageManage(self.db)
			s.save(data, table = "pis.message_log")
