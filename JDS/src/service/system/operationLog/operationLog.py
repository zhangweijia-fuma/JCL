#coding:utf-8

import sys
import time
import datetime
import string 

from db import dbMysql
from web.Base  import WebRequestHandler,BaseError,operator_except
from libs.utils import debug
import xlrd, xlwt
from public.excel import excel


class Restful(WebRequestHandler):

	# 查询日志记录
	# 参数：
	# 	uid: 操作人ID
	# 	mc：菜单项编码
	# 	st：开始日期
	# 	et：结束日期
	# 各参数均为可选，如果没有参数则查询全部日志
	@operator_except
	def get(self):

		userInfo 	= self.GetUserInfo()

		op 			= self.get_argument('op',default='data')
		offset   	= int(self.get_argument('o',default='0'))
		rowlimit 	= int(self.get_argument('r',default='10'))
		offset		= (offset - 1) * rowlimit

		uid			= int(self.get_argument('uid', default='0'))
		menu_code	= self.get_argument('mc', default='')
		start_time	= self.get_argument('st', default='')
		end_time	= self.get_argument('et', default='')

		sql_id = ""
		if uid > 0 :
			sql_id = "ac.id = %d"%uid

		sql_menu = ""
		if menu_code != '':
			sql_menu = " menu.code = '%s'"%menu_code

		sql_time = ""
		if start_time and end_time:
			sql_time  = " (( to_timestamp('%s','YYYY-MM-DD HH24:MI:SS')<= ol.create_time "%( start_time + " 00-00-00")
			sql_time += " and ol.create_time<= to_timestamp('%s','YYYY-MM-DD HH24:MI:SS')))"%( end_time + " 23-59-59")

		# sql_where = " where ac.user_name <> 'supervisor' "
		sql_where = " where ol.system_user_id = %d"%userInfo['system_user_id']

		if sql_id != "":
			sql_where += " and " + sql_id

		if sql_menu != "":
			sql_where += " and " + sql_menu

		if sql_time != "":
			sql_where += " and " + sql_time

		sql_limit = " limit " + str(rowlimit) + " offset " + str(offset)

		sql  = "select ol.id, ac.id, menu.code, ol.operation_context, ol.operation_no, ol.create_time, ac.name , menu.name "
		sql += " from system.operation_log ol "
		sql += " left join system.account ac on ol.operator_id = ac.id "
		sql += " left join system.menu menu on ol.menu_code = menu.code "
		sql += sql_where
		sql += " order by ol.create_time DESC"
		if op == "data":
			sql += sql_limit

		cur = self.db.getCursor()
		cur.execute(sql) 
		rows = cur.fetchall()

		if op == "excel":
			self.exportToExcel(rows)

		rowdata = {}
		rowdata['rows'] = rows

		rowdata['struct'] = 'u_id, id, menu_code, operation_context, operation_no, create_time, operator_name, menu_name'

		sql  = "select count(*) from system.operation_log ol "
		sql += "left join system.account ac on ol.operator_id = ac.id "
		sql += "left join system.menu menu on ol.menu_code = menu.code "
		if sql_where != "":
			sql += sql_where

		cur.execute(sql)
		row = cur.fetchone()
		rowdata['count'] = row[0]
		self.response(rowdata)

	# 输出日志到excel文件
	def exportToExcel(self, rowdata):

		data = []
		sn = 1

		key  = ['u_id', 'id', 'menu_code', 'operation_context', 'operation_no', 'create_time', 'operator_name', 'menu_name']
		for i, item in enumerate(rowdata):
			messageInfo = dict(zip(key, item))

			itemData = []
			itemData.append( sn )
			sn = sn + 1
			itemData.append( messageInfo['operator_name'] )
			itemData.append( messageInfo['create_time'].strftime('%Y-%m-%d %H:%M:%S') )
			itemData.append( messageInfo['operation_context'])	
			
			data.append(itemData) 

		struct = "SN, 操作人, 操作时间, 操作内容"

		path = {}
		path = excel.createTempFile("xls")
	
		excel.saveExcel(path['path'], struct, data )

		logInfo = " %s 下载了操作日志清单 "%self.objUserInfo['name']
		operation_log(self.db).addLog(self.GetUserInfo(), "operationLog", logInfo, 0)

		self.response(path)



	# 获取日志页面操作人及操作项下拉列表
	# 参数：
	# 	无
	@operator_except
	def patch(self):
		
		userInfo = self.GetUserInfo()
		cur = self.db.getCursor()

		sql  = "select ac.id, ac.name from system.account ac "
		sql += " where ac.system_user_id = %d"%userInfo['system_user_id']
		sql += " order by ac.name "
		
		cur.execute(sql)
		rows = cur.fetchall()
		rowdata = {}
		rowdata['rows'] = [(0, "选择全部", )] + rows
		rowdata['struct'] = 'id, name'
		alldata = {}
		alldata['operatorList'] = rowdata

		sql  = "select c.code as menu_code, c.name from system.menu c "	
		sql += " where c.tier = 1 and c.id in ( select menu_id from system.system_user_menu sm where sm.system_user_id = %d) "%userInfo['system_user_id'] 
		sql += " order by c.id"
 
		cur.execute(sql)
		rows 	= cur.fetchall()
		rowdata = {}
		rowdata['rows'] 	= [("", "选择全部", )] + rows
		rowdata['struct'] 	= 'menu_code, name'
		alldata['operationMenuCodeList'] 	= rowdata

		self.response(alldata)
