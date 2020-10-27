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

from libs.utils.debug import *
from pis import controllerManage

class statusManage(dbMysql.CURD) :
	def __init__(self,db) :
		if sys.version > '3':
			# python 3.0 +
			super().__init__(db,'pis.controller_status',False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(statusManage,self).__init__(db,'pis.controller_status',False)


class Restful(WebRequestHandler):

	# 获取控制器状态信息
	# 参数：
	# 	op ：data：数据，excel：下载到excel文件
	# 	cid：控制器ID
	# 返回：状态列表
	@operator_except
	def get(self):
		
		op 		= self.get_argument('op', default = 'data')
		cid 	= int(self.get_argument('cid', default = '0'))
		offset 	= int(self.get_argument('o',  default = '1'))
		rowlimit= int(self.get_argument('r',  default = '20'))

		offset = ( offset - 1 ) * rowlimit
		sql_limit = " limit " + str(rowlimit) + " offset " + str(offset)

		sql  = " select cs.id, cs.controller, cs.ip_address, cs.date, cs.time, cs.cpu, cs.memory, cs.harddisk, cs.status, ct.code"
		sql += " from pis.controller_status cs"

		sql += " inner join pis.controller ct on ct.id = cs.controller"

		sql_where = ""
		if cid > 0:
			sql_where = " where cs.controller = %d"%cid
	
		sql += sql_where
		sql += " order by cs.id desc, cs.date, cs.time"

		if op == "data":
			sql += sql_limit

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		if op == "excel":
			self.exportToExcel( rows )

		rowdata = {}

		statusList = {}
		statusList['rows'] = rows
		statusList['struct']  = "id, controller, ip_address, date, time, cpu, memory, harddisk, status, code"

		sql  = "select count(cs.id) from pis.controller_status cs"
		sql += sql_where

		cur.execute(sql)
		row = cur.fetchone() 
		statusList['count'] = row[0]

		self.response( statusList )

	# 导出到Excel文件
	def exportToExcel(self, data):
		
		self.response(0)
