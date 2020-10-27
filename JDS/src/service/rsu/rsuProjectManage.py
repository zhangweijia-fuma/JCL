#coding:utf-8

import re
import time
import datetime
import sys

from db import dbMysql
from web.baseException import errorDic,BaseError
from web.Base  import WebRequestHandler,BaseError,operator_except

import xlrd, xlwt
from public.excel import excel

from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE
from libs.utils import utils
from utils.debug import *
from libs.JCL.JCLDBTools import *


class rsuProjectManage(dbMysql.CURD) :
	def __init__(self,db) :
		table_name = 'rsu.rsu_project'

		if sys.version > '3':
			# python 3.0 +
			super().__init__(db, table_name, False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(rsuProjectManage, self).__init__(db, table_name, False)

class Restful(WebRequestHandler):

	# 获取RSU项目信息
	# 参数：
	# 	pid:项目ID
	# 	pa: 甲方ID
	# 	pb: 乙方ID
	# 	rs: 记录状态，默认为2，正常
	@operator_except
	def get(self):
		
		op 			= self.get_argument('op',  default = "data")
		projectID  	= int(self.get_argument('pid',  default = '0'))
		partAID 	= int(self.get_argument('pa',  default = '0'))
		partBID 	= int(self.get_argument('pb', default = '0'))

		offset 		= int(self.get_argument('o',  default = '1'))
		rowlimit	= int(self.get_argument('r',  default = '20'))

		offset 		= ( offset - 1 ) * rowlimit
		sql_limit 	= " limit " + str(rowlimit) + " offset " + str(offset)

		sql = """
				select rp.id, rp.project_name, rp.project_amount, rp.sign_date, rp.sign_addr,
				rp.parta_id, rp.parta_code, rp.parta_man_name, rp.parta_title,
				rp.partb_id, rp.partb_code, rp.partb_man_name, rp.partb_title, 
				oga.name, ogb.name
				from rsu.rsu_project rp
				inner join public.organization oga on oga.id = rp.parta_id
				inner join public.organization ogb on ogb.id = rp.partb_id
		"""

		# 默认查找正常记录
		sql_where = " where rp.system_user_id = %d "%(self.GetSystemUserID())

		if projectID != 0:
			sql_where += " and rp.id = %d"%projectID

		if partAID != 0:
			sql_where += " and rp.parta_id = %d"%partAID

		if partBID != 0:
			sql_where += " and rp.partb_id = %d"%partBID
	
		sql += sql_where
		sql += " order by rp.project_name"

		if op == "data":
			sql += sql_limit

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		if op == "excel":
			path = self.exportToExcel( rows )
			self.response(path)
		else:
			rowdata = {}

			rowdata = {}
			rowdata['rows'] = rows
			rowdata['struct']  = "id, project_name, project_amount, sign_date, sign_addr, parta_id, parta_code, parta_man_name, parta_title, partb_id, partb_code, partb_man_name, partb_title, parta_name, partb_name"

			sql  = "select count(rp.id) from rsu.rsu_project rp"
			sql += sql_where

			cur.execute(sql)
			row = cur.fetchone() 
			rowdata['count'] = row[0]

			self.response( rowdata )


	# 输出excel文件
	def exportToExcel(self, rowdata):

		key  = ['id', 'project_name', 'project_amount', 'sign_date', 'sign_addr',
				'parta_id', 'parta_code', 'parta_man_name', 'parta_title',
				'partb_id', 'partb_code', 'partb_man_name', 'partb_title', 
				'parta_name', 'partb_name']

		rsuList = []
		for i, item in enumerate(rowdata):
			rsuInfo = dict(zip(key, item))
			rsuList.append(rsuInfo)

		data = []
		sn = 1
		for module in moduleList:

			itemData = []
			itemData.append( sn )
			sn = sn + 1
			itemData.append( module['project_name'] )
			itemData.append( module['project_amount'] )
			itemData.append( module['sign_date'] )
			itemData.append( module['sign_addr'] )
			itemData.append( module['parta_name'] )
			itemData.append( module['parta_code'] )
			itemData.append( module['parta_man_name'])
			itemData.append( module['parta_title'] )
			itemData.append( module['partb_name'] )
			itemData.append( module['partb_code'] )
			itemData.append( module['partb_man_name'])
			itemData.append( module['partb_title'] )

			data.append(itemData) 

		struct = "SN, 项目名称, 项目金额, 签订日期, 签订地点, 甲方名称, 甲方合同号, 甲方签署人, 甲方签署人职务, 乙方名称, 乙方合同号, 乙方签署人, 乙方签署人职务"

		path = {}
		path = excel.createTempFile("xls")

		excel.saveExcel(path['path'], struct, data )
		
		logInfo = " %s 下载了RSU项目清单 "%self.objUserInfo['name']
		operation_log(self.db).addLog(self.GetUserInfo(), "rsuProjectManage", logInfo, 0)

		return path

	@operator_except
	def post(self):

		alldata = self.getRequestData()

		s = rsuProjectManage(self.db)
		
		lsData = {
			'project_name'	: 'project_name',
			'project_amount': 'project_amount', 
			'sign_date' 	: 'sign_date',
			'sign_addr' 	: 'sign_addr',
			'parta_id'		: 'parta_id', 
			'parta_code' 	: 'parta_code',
			'parta_man_name': 'parta_man_name',
			'parta_title' 	: 'parta_title',
			'partb_id'		: 'partb_id', 
			'partb_code' 	: 'partb_code',
			'partb_man_name': 'partb_man_name',
			'partb_title' 	: 'partb_title',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass

		# LogI("RSU 项目信息：", data)
		system_user_id = self.GetSystemUserID()

		db = self.getDB("rsu.rsu_project")

		# 只对比记录状态为正常的记录

		# 检查合同名称是否重复
		result = db.findByCond("project_name", "project_name = '%s' "%(data['project_name']))
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：合同 %s 已经存在！"%data['project_name'])

		# 检查合同编号是否重复
		result = db.findByCond("parta_code", "parta_code = '%s' and system_user_id = %d"%(data['parta_code'], system_user_id))
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：甲方合同编号 %s 已经存在！"%data['parta_code'])

		result = db.findByCond("partb_code", "partb_code = '%s' and system_user_id = %d"%(data['partb_code'], system_user_id))
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：乙方合同编号 %s 已经存在！"%data['partb_code'])

		data['create_id'] 		= self.objUserInfo['id']
		data['create_time'] 	= datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		data['system_user_id'] 	= self.GetSystemUserID()

		id = s.save(data, table = 'rsu.rsu_project')

		# 创建项目成功，记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "rsuProjectManage", 
			"%s 创建 RSU 项目：%s"%(self.objUserInfo['name'], data['project_name']), id)

		self.response(id)


	@operator_except
	def put(self):
		alldata = self.getRequestData()

		s = rsuProjectManage(self.db)
		
		lsData = {
			'id' 			: 'id',
			'project_name'	: 'project_name',
			'project_amount': 'project_amount', 
			'sign_date' 	: 'sign_date',
			'sign_addr' 	: 'sign_addr',
			'parta_id'		: 'parta_id', 
			'parta_code' 	: 'parta_code',
			'parta_man_name': 'parta_man_name',
			'parta_title' 	: 'parta_title',
			'partb_id'		: 'partb_id', 
			'partb_code' 	: 'partb_code',
			'partb_man_name': 'partb_man_name',
			'partb_title' 	: 'partb_title',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass

		system_user_id = self.GetSystemUserID()

		db = self.getDB("rsu.rsu_project")

		# 检查合同名称是否重复
		result = db.findByCond("project_name", "project_name = '%s' and id <> %d"%(data['project_name'], data['id']))
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：合同 %s 已经存在！"%data['project_name'])

		# 检查合同编号是否重复
		result = db.findByCond("parta_code", "parta_code = '%s' and system_user_id = %d and id <> %d"%(data['parta_code'], system_user_id, data['id']))
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：甲方合同编号 %s 已经存在！"%data['parta_code'])

		result = db.findByCond("partb_code", "partb_code = '%s' and system_user_id = %d and id <> %d"%(data['partb_code'], system_user_id, data['id']))
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：乙方合同编号 %s 已经存在！"%data['partb_code'])

		data['update_id'] 		= self.objUserInfo['id']
		data['update_time'] 	= datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		
		id = s.save(data, data['id'], table = 'rsu.rsu_project')

		# 更新项目成功，记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "rsuProjectManage", 
			"%s 更新 RSU 项目：%s"%(self.objUserInfo['name'], data['project_name']), id)

		self.response(id)

	@operator_except
	def delete(self):

		alldata = self.getRequestData()
		
		sql  = "select rp.project_name from rsu.rsu_project rp where rp.id = %d"%alldata['id']
		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()
		logInfo = "删除项目：%s"%(rows[0][0])

		# 删除记录，将数据移动到历史表中
		MoveDeleteRecord("rsu", "rsu_project", alldata['id'], self.GetUserID())

		# 更新项目成功，记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "rsuProjectManage", 
			"%s 删除 RSU 项目：%s"%(self.objUserInfo['name'], alldata['project_name']), alldata['id'])

		self.response(alldata['id'])


	# 获取下拉列表信息
	# 参数：

	@operator_except
	def patch(self):

		paramData = self.getRequestData()

		op = self.getParam(paramData, "op", "")
		if op == "":
			raise BaseError(801, "参数错误：缺少操作类型！")

		if op == "data":

			# 返回的数据
			data = {}

			# 合同列表
			sql  = "select rp.id, rp.project_name from rsu.rsu_project rp where rp.system_user_id = %d order by rp.project_name "%self.GetSystemUserID()
			cur = self.db.getCursor()
			cur.execute(sql)
			rows = cur.fetchall()

			projectList = {}
			projectList['struct']	= "id, name"
			projectList['rows'] 	= [(0, "选择全部", )] + rows

			data['projectList'] 	= projectList

			# 甲方名称
			sql  = """
				select distinct(og.id), og.name 
				from public.organization og 
				inner join rsu.rsu_project rp on rp.parta_id = og.id
				where og.system_user_id = %d
				order by og.name
			"""%self.GetSystemUserID()

			cur.execute(sql)
			rows = cur.fetchall()

			partAList = {}
			partAList['struct']	= "id, name"
			partAList['rows'] 	= [(0, "选择全部", )] + rows

			data['partAList'] 	= partAList

			# 乙方列表
			sql  = """
				select distinct(og.id), og.name 
				from public.organization og 
				inner join rsu.rsu_project rp on rp.partb_id = og.id
				where og.system_user_id = %d
				order by og.name
			"""%self.GetSystemUserID()

			cur.execute(sql)
			rows = cur.fetchall()

			partBList = {}
			partBList['struct']	= "id, name"
			partBList['rows'] 	= [(0, "选择全部", )] + rows

			data['partBList'] 	= partBList

			# 组织列表
			sql  = """
				select og.id, og.name 
				from public.organization og
				where og.system_user_id = %d 
				order by og.name
			"""%self.GetSystemUserID()

			cur.execute(sql)
			rows = cur.fetchall()

			organizationList = {}
			organizationList['struct']	= "id, name"
			organizationList['rows'] 	= rows

			data['organizationList'] 	= organizationList

			self.response(data)

			
