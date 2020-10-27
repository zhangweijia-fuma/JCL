#coding:utf-8

import re
import time
import datetime
import sys
import os

from db import dbMysql
from web.baseException import errorDic,BaseError
from web.Base  import WebRequestHandler,BaseError,operator_except

import config
import xlrd, xlwt
from public.excel import excel
from public.upload import callBack

from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE
from libs.utils.debug import *
from libs.utils import utils

from pis import programUnit
from pis import programInfo

class programList(dbMysql.CURD) :
	def __init__(self,db) :
		if sys.version > '3':
			# python 3.0 +
			super().__init__(db,'pis.program_list',False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(programList,self).__init__(db,'pis.program_list',False)



class Restful(WebRequestHandler):


	# 获取节目单信息
	# 参数：
	# 	op: list or file 是下载到文件还是数据清单
	# 	id: 节目单ID
	# 	aid: 申请者ID
	# 	valid_date：生效日期
	# 	apply_status: 申请状态
	# 	send_status: 发送状态

	@operator_except
	def get(self):
		
		op 				= self.get_argument('op',  default = "list")
		id 				= int(self.get_argument('id',  default = '0'))
		apply_id   		= int(self.get_argument('aid',  default = '0'))
		valid_date 		= self.get_argument('vd', default = '')
		apply_status 	= self.get_argument('as', default = '')
		send_status 	= self.get_argument('ss', default = '')
		
		offset 	= int(self.get_argument('o',  default = '1'))
		rowlimit= int(self.get_argument('r',  default = '20'))

		offset = ( offset - 1 ) * rowlimit
		sql_limit = " limit " + str(rowlimit) + " offset " + str(offset)

		sql  = " select pl.id, pl.create_time, pl.update_time, pl.create_id, pl.update_id, pl.code, pl.name, pl.valid_date, "
		sql += " pl.apply_id, pl.apply_name, pl.apply_date, pl.checker_id, pl.checker_name, pl.check_date, pl.apply_status, pl.video_type, "
		sql += " ml.sender_id, ml.sender_name, ml.send_date, ml.send_time, ml.send_status"

		sql += " from pis.program_list pl"

		sql_inner  = ""
		sql_inner += " left join pis.message_list ml on ml.message_type = '2' and ml.operation_id1 = pl.id "
		sql += sql_inner

		sql_where = ""
		if id > 0:
			# 如果有节目单ID，则其他条件忽略
			sql_where = " where pl.id = %d"%id
		else:
			if valid_date != "":
				sql_where = " where pl.valid_date = '%s' "%(valid_date)
		
			if apply_id > 0:
				if sql_where == "":
					sql_where = " where pl.apply_id = %d"%apply_id
				else:
					sql_where += " and pl.apply_id = %d"%apply_id

			if apply_status != "":
				if sql_where == "":
					sql_where = " where pl.apply_status = '%s'"%apply_status
				else:
					sql_where += " and pl.apply_status = '%s'"%apply_status

			if send_status != "":
				if send_status != "1":
					if sql_where == "":
						sql_where = " where ml.send_status = '%s' "%send_status
					else:
						sql_where += " and ( ml.send_status = '%s'  )"%send_status
				else:
					if sql_where == "":
						sql_where = " where ml.send_status is null "
					else:
						sql_where += " and ml.send_status is null "

		sql += sql_where
		sql += " order by pl.id DESC"

		if op == "list":
			sql += sql_limit

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		if op == "excel":
			self.exportToExcel( rows )
		else:
			rowdata = {}

			programList = {}
			programList['rows'] = rows
			programList['struct'] = "id, create_time, update_time, create_id, update_id, code, name, valid_date, apply_id, apply_name,"
			programList['struct'] += "apply_date, checker_id, checker_name, check_date, apply_status, video_type, "
			programList['struct'] += "sender_id, sender_name, send_date, send_time, send_status"

			sql  = "select count(pl.id) from pis.program_list pl"
			sql += sql_inner
			sql += sql_where

			cur.execute(sql)
			row = cur.fetchone() 
			programList['count'] = row[0]

			self.response( programList )

	# 输出日志到excel文件
	def exportToExcel(self, rowdata):

		key = ['id', 'create_time', 'update_time', 'create_id', 'update_id', 'code', 'name', 'valid_date', 'apply_id', 'apply_name', 
				'apply_date', 'checker_id', 'checker_name', 'check_date', 'apply_status', 'video_type',
				 'sender_id', 'sender_name', 'send_date', 'send_time', 'send_status']
		
		messageList = []
		for i, item in enumerate(rowdata):
			messageInfo = dict(zip(key, item))
			messageList.append(messageInfo)

		cur = self.db.getCursor()
		utils.getDataDict(cur, messageList, "send_status", "PROGRAM_SEND_STATUS", "send_status_name")
		utils.getDataDict(cur, messageList, "apply_status", "PROGRAM_APPLY_STATUS", "apply_status_name")
		
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
			itemData.append( item['name'] )				# 节目单名称
			itemData.append( item['valid_date'] )		# 生效日期
			itemData.append( item['apply_name'])		# 申请人姓名
			itemData.append( item['apply_date'] )		# 申请日期
			itemData.append( item['checker_name'] )
			itemData.append( item['check_date'] )		# 审核日期
			itemData.append( item['apply_status_name'])	# 申请状态
			itemData.append( item['sender_name'] )		# 发送人姓名
			itemData.append( item['send_date'] )		# 发送日期
			itemData.append( item['send_time'] )		# 发送日期
			itemData.append( item['send_status_name'] )	# 发送状态

			data.append(itemData) 

		struct = "SN, 节目单名称, 生效日期, 申请人姓名, 申请日期, 审核人姓名, 审核日期, 申请状态, 发送者姓名, 发送日期, 发送时间, 发送状态"

		path = {}
		path = excel.createTempFile("xls")
		
		excel.saveExcel(path['path'], struct, data )

		logInfo = " %s 下载了节目单清单 "%self.objUserInfo['name']
		operation_log(self.db).addLog(self.GetUserInfo(), "listManage", logInfo, 0)

		self.response(path)


	# 创建节目单，此时只有节目单名称和生效日期参数
	# 节目单编号自动生成，等同于ID值，同时自动创建从5点到24点的节目栏，节目栏按小时划分
	@operator_except
	def post(self):

		alldata = self.getRequestData()

		s = programList(self.db)
		
		lsData = {
			'name'			: 'name',
			'valid_date' 	: 'valid_date',
			'video_type' 	: 'video_type',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass

		# 检查名称是否重复
		db = self.getDB("pis.program_list")
		result = db.findByCond("name", "name = '%s' "%data['name'])
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：节目单名称已经存在！")

		data['create_id'] 	= self.objUserInfo['id']
		data['create_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		
		# 节目单初始值
		data['apply_status'] = '1'		# 未提交审核

		id = s.save(data, table = 'pis.program_list')

		data['code'] = id
		s.save(data, id, table = 'pis.program_list')

		# 创建节目栏
		hour = 0
		while hour < 24:
			unit = {
				'list_id' : id,
				'start_time' : "%02d:00:00"%hour,
				'end_time'   : "%02d:59:59"%hour,
				'sort' 		 : hour - 4,
			}
			hour += 1
			uid = s.save( unit, table = "pis.program_unit")
			unit['code'] = uid
			s.save(unit, uid, table = "pis.program_unit")

		# 创建节目单成功，记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "listManage", 
			"%s 创建节目单：名称：%s，生效日期：%s"%(self.objUserInfo['name'], data['name'], data['valid_date']), id)

		self.response(id)


	# 更新节目单信息
	# 参数：
	#   id: 待更新节目单的ID，必选
	#   op: 操作类型，
	#     modify: 修改，更新节目单名称及生效日期
	#     submit：提交申请，无参数
	#     check：审核，审核意见：result
	#     send：发送，更新发送状态为2：已发送，controllerList：待接收的控制器列表
	@operator_except
	def put(self):

		paramData = self.getRequestData()
		lid = self.getParam( paramData, "id", 0)
		if lid == 0:
			raise BaseError(801, "参数错误：缺少待更新的节目单ID！")

		op = self.getParam( paramData, "op", "")
		if op == "":
			raise BaseError(801, "参数错误：缺少数据操作模式参数OP！")

		s = programList(self.db)
		
		controllerList = []

		data = {}
		data['update_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		data['update_id'] = self.objUserInfo['id']
		
		sql = " select pl.name from pis.program_list pl where pl.id = %d"%lid
		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()
		if len(rows) == 0:
			raise BaseError(801, "参数错误：缺少待更新的节目单ID！")

		list_name = rows[0][0]
		
		logInfo = ""
		if op == "modify":
			# 只是更新节目单基本信息
			lsData = {
				"id" 		: "id",
				'name'		: 'name',
				'valid_date': 'valid_date',
				'video_type': 'video_type',
			}

			for (k, v) in lsData.items():
				try:
					data[k] = paramData[v]
				except:
					pass
			
			logInfo = "%s 更新了节目单：名称：%s， 生效日期：%s"%(self.objUserInfo['name'], list_name, data['valid_date'] )

		elif op == "submit":
			# 提交审核，更新申请人信息
			data['apply_id'] 		= self.objUserInfo['id']
			data['video_type'] 		= self.getParam( paramData, "video_type", '0')

			data['apply_name'] 		= self.objUserInfo['name']
			data['apply_date'] 		= datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
			data['apply_status'] 	= '2'

			logInfo = "%s 将节目单 %s 提交审核，时间：%s"%(data['apply_name'], list_name, data['apply_date'] )

		elif op == "check":
			# 完成审核，修改审核人信息
			data['checker_id'] 		= self.objUserInfo['id']
			data['checker_name'] 	= self.objUserInfo['name']
			data['check_date'] 		= datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')

			result  = self.getParam( paramData, "result", "")
			opinion = self.getParam( paramData, "opinion", "")

			sql  = " select cv.name from system.code_value cv "
			sql += " where cv.type_code = 'PROGRAM_APPLY_STATUS' and cv.code = '%s'"%result
			cur.execute(sql)
			rows = cur.fetchall()
			if len(rows) == 0:
				raise BaseError(801, "参数错误：审核结果参数 %s 错误！"%result)

			data['apply_status'] 	= result
			resultName = rows[0][0]

			# 新增审批记录
			checkData = {
				'create_id'		: self.objUserInfo['id'],
				'create_time'	: datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
				'list_id' 		: lid,
				'checker_id' 	: data['checker_id'],
				'checker_name' 	: data['checker_name'],
				'check_date' 	: data['check_date'],
				'check_result' 	: result,
				'check_opinion' : opinion,
			}
			s.save(checkData, table="pis.program_check_history")

			logInfo = "%s 审批了节目单 %s，审批结果：%s, 审批意见：%s， 时间：%s"%(data['checker_name'], list_name, resultName, opinion, data['check_date'] )

		else:
			raise BaseError(801, "参数错误：数据操作模式参数 OP 值 %s 错误！"%op)

		id = s.save(data, lid, table = 'pis.program_list')

		# 更新节目单成功，记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "listManage", logInfo, id)

		self.response(id)

	# 删除指定节目单
	# 参数：
	#  	listInfo: 待删除的节目单记录，必选 
	@operator_except
	def delete(self):
		
		listInfo = self.getRequestData()
		
		# logI("待删除节目单信息：", listInfo)

		id = self.getParam( listInfo, "id", 0)
		if id == 0:
			raise BaseError(801, "参数错误：缺少待更删除的节目单ID！")

		
		logInfo = "%s 删除节目单及相关节目栏、节目信息：节目单名称：%s, 生效效期：%s"%(self.objUserInfo['name'], listInfo['name'], listInfo['valid_date'])

		# 不删除节目文件
		
		# 删除节目单所有的节目信息
		s = programList(self.db)
		s.remove( id, table = "pis.program_info", key = "list_id", delete = True)

		# 删除节目栏记录
		s.remove( id, table = "pis.program_unit", key = "list_id", delete = True)

		# 删除节目单记录
		s.remove( id, table = "pis.program_list", key = "id", delete = True)

		# 记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "programList", logInfo, id)
		self.response(id)


	# 获取下拉列表信息或节目单附属的节目栏、节目信息
	# 参数：
	# 	op: list 或tree，获取下拉列表或树形结构
	# 	

	@operator_except
	def patch(self):

		paramData = self.getRequestData()
		op = self.getParam( paramData, "op", "")
		if op == "":
			raise BaseError(801, "参数错误：缺少操作模式参数OP")

		if op == "list":
			# 返回的数据
			data = {}

			# 申请者列表
			sql  = "select distinct(pl.apply_id), pl.apply_name "
			sql += " from pis.program_list pl"
			sql += " order by pl.apply_name"

			cur = self.db.getCursor()
			cur.execute(sql)
			rows = cur.fetchall()

			applyerList = {}
			applyerList['struct']	= "id, name"
			applyerList['rows'] 	= [(0, "选择全部", )] + rows

			data['applyerList'] 	= applyerList

			# 申请状态列表
			sql  = "select cv.code, cv.name "
			sql += " from system.code_value cv"
			sql += " where cv.type_code = 'PROGRAM_APPLY_STATUS' "
			sql += " order by cv.code"

			cur = self.db.getCursor()
			cur.execute(sql)
			rows = cur.fetchall()

			applyStatusList = {}
			applyStatusList['struct']	= "code, name"
			applyStatusList['rows'] 	= [('0', "选择全部", )] + rows

			data['applyStatusList'] 	= applyStatusList

			# 发送状态列表
			sql  = "select cv.code, cv.name "
			sql += " from system.code_value cv"
			sql += " where cv.type_code = 'PROGRAM_SEND_STATUS' "
			sql += " order by cv.code"

			cur = self.db.getCursor()
			cur.execute(sql)
			rows = cur.fetchall()

			sendStatusList = {}
			sendStatusList['struct']	= "code, name"
			sendStatusList['rows'] 	= [('0', "选择全部", )] + rows

			data['sendStatusList'] 	= sendStatusList


			self.response(data)

		elif op == "tree":
			pid = int(self.getParam(paramData, "pid", 0))
			if pid == 0:
				raise BaseError(801, "参数错误：无节目单ID！")

			sql  = " select pl.name from pis.program_list pl where pl.id =%d"%pid
			cur = self.db.getCursor()
			cur.execute(sql)
			rows = cur.fetchall()
			list_name = rows[0][0]

			listNode = {}
			listNode['name'] 		= list_name
			listNode['chkDisabled'] = True
			listNode['open']	 	= True
			listNode['checked']  	= False
			listNode['isItem']   	= False
			listNode['nodeType'] 	= 'root'

			listNode['children'] = programUnit.getProgramUnitList(self, pid)

			self.response(listNode)

		elif op == "history":
			# 获取节目单审批记录
			
			pid = int(self.getParam( paramData, "pid", "0"))
			if pid == 0:
				raise BaseError(801, "参数错误：无节目单ID！")

			data = {}

			sql  = "select pch.list_id, pch.checker_id, pch.checker_name, pch.check_date, pch.check_result, pch.check_opinion"
			sql += " from pis.program_check_history pch"
			sql += " where pch.list_id = %d"%pid

			cur = self.db.getCursor()
			cur.execute(sql)
			rows = cur.fetchall()

			data['struct']	= "list_id, checker_id, checker_name, check_date, check_result, check_opinion"
			data['rows'] 	= rows
			self.response(data)

		elif op == "send":
			# 获取节目单发送记录
			pid = int(self.getParam( paramData, "pid", "0"))
			if pid == 0:
				raise BaseError(801, "参数错误：无节目单ID！")

			offset 	= int(self.getParam( paramData, 'o', '1'))
			rowlimit= int(self.getParam( paramData, 'r',  '20'))
			offset  = ( offset - 1 ) * rowlimit

			data = {}

			sql  = "select ml.id, ml.operation_id1, ml.operation_id2, ml.sender_id, ml.sender_name, ml.send_date, ct.id,"
			sql += " ml.controller_code, ct.name, ct.controller_type, ct.controller_addr, log.receive_status, log.receive_date"
			sql += " from pis.message_list ml"
			sql_inner = " inner join pis.message_log log on log.msg_list_id = ml.id "
			sql_inner += " inner join pis.controller ct on ct.id = log.receiver_id"
			# sql_inner += " inner join system.code_value cv on cv.code = ml.receive_status and cv.type_code = 'CTRL_RECEIVE_STATUS' "
			
			sql_where = " where ml.operation_id1 = %d and ml.message_type = '2' "%pid
			
			sql += sql_inner + sql_where + " limit " + str(rowlimit) + " offset " + str(offset)

			cur = self.db.getCursor()
			cur.execute(sql)
			rows = cur.fetchall()

			data['struct']	= "id, operation_id1, operation_id2, sender_id, sender_name, send_date, controller_id, controller_code, controller_name, controller_type, "
			data['struct'] += " controller_addr, receive_status, receive_date"
			data['rows'] 	= rows

			sql  = "select count(ml.id) from pis.message_list ml"
			sql += sql_inner + sql_where

			cur.execute(sql)
			row = cur.fetchone() 
			data['count'] = row[0]

			self.response(data)

		elif op == "copy":
			# 复制节目单
			self.copyProgramList(paramData)

		else:
			raise BaseError(801, "参数错误：操作类型参数OP值错误！")

	# 复制节目单
	def copyProgramList(self, paramData):

		oldID = self.getParam(paramData, "id", 0)
		if oldID == 0:
			raise BaseError(801, "参数错误：无待复制的节目单ID")

		data = {
			'name' 		 : paramData['name'],
			'valid_date' : paramData['valid_date'],
			'video_type' : paramData['video_type'],
		}

		s = programList(self.db)

		# 检查名称是否重复
		db = self.getDB("pis.program_list")
		result = db.findByCond("name", "name = '%s' "%data['name'])
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "参数错误：节目单名称已经存在！")

		data['create_id'] 	= self.objUserInfo['id']
		data['create_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		
		# 节目单初始值
		data['apply_status'] = '1'		# 未提交审核

		newID = s.save(data, table = 'pis.program_list')

		data['code'] = newID
		s.save(data, newID, table = 'pis.program_list')

		sql = "select pu.id, pu.start_time, pu.end_time, pu.sort from pis.program_unit pu where pu.list_id = %d"%oldID
		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		# 复制节目栏
		for row in rows:
			puID = row[0]

			unit = {
				'start_time' : row[1],
				'end_time' 	 : row[2],
				'sort'		 : row[3],
				'list_id'	 : newID,
			}

			uid = s.save( unit, table = "pis.program_unit")
			unit['code'] = uid
			s.save(unit, uid, table = "pis.program_unit")

			# 复制节目
			sql  = "select pi.name, pi.program_type, pi.duration, pi.file_name, pi.file_path, pi.program_source, pi.sort, pi.uploader_id, pi.uploader_name, pi.upload_date"
			sql += " from pis.program_info pi where pi.unit_id = %d"%puID
			cur.execute(sql)
			infoList = cur.fetchall()

			key = ['name', 'program_type', 'duration', 'file_name', 'file_path', 'program_source', 'sort', 'uploader_id', 'uploader_name', 'upload_date']

			for info in infoList:
				programData = dict(zip(key, info))
				programData['list_id'] = newID
				programData['unit_id'] = uid

				# 复制节目文件
				month = data['valid_date'][0:7]

				filePath = "%s/list/%s/%d/"%(config.PisConfig['PISFileRoot'], month, newID)
				filename = programData['file_path'].split("/")[-1]
				file_new_path = callBack.copyFile( programData['file_path'], filePath, filename )

				programData['file_path'] = file_new_path

				pid = s.save( programData, table = "pis.program_info")
				programData['code'] = pid
				s.save(programData, pid, table = "pis.program_info")


		# 复制节目单成功，记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "listManage", 
			"%s 复制了节目单：原名称： %s, 原生效日期： %s, 新名称：%s，新生效日期：%s"%(self.objUserInfo['name'], paramData['original_name'], paramData['original_valid_date'], data['name'], data['valid_date']), newID)

		self.response(newID)
