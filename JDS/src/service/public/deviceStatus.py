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

class deviceStatusManage(dbMysql.CURD) :
	def __init__(self,db) :
		if sys.version > '3':
			# python 3.0 +
			super().__init__(db,'public.device_status',False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(deviceStatusManage,self).__init__(db,'public.device_status',False)


class Restful(WebRequestHandler):

	# 获取设备状态信息
	# 参数：
	# 	op ：data：数据，excel：下载到excel文件
	# 	did：设备ID
	# 返回：状态列表
	@operator_except
	def get(self):
		
		op 		= self.get_argument('op', default = 'data')
		did 	= int(self.get_argument('did', default = '0'))
		offset 	= int(self.get_argument('o',  default = '1'))
		rowlimit= int(self.get_argument('r',  default = '20'))

		offset = ( offset - 1 ) * rowlimit
		sql_limit = " limit " + str(rowlimit) + " offset " + str(offset)

		sql  = " select ds.id, ds.device_id, ds.ip_address, ds.date, ds.time, ds.cpu, ds.memory, ds.harddisk"
		sql += " from public.device_status ds"

		sql_where = ""
		if did > 0:
			sql_where = " where ds.device_id = %d"%did
	
		sql += sql_where

		# 按照ID的倒叙取数据，以取得最新的数据
		sql += " order by ds.id DESC "

		if op == "data":
			sql += sql_limit

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		if op == "excel":
			self.exportToExcel( rows )

		# 把数据从倒叙转换为正序
		
		rows.sort(key = lambda x:x[0])

		rowdata = {}

		statusList = {}
		statusList['rows'] = rows
		statusList['struct']  = "id, device_id, ip_address, date, time, cpu, memory, harddisk"

		sql  = "select count(ds.id) from public.device_status ds"
		sql += sql_where

		cur.execute(sql)
		row = cur.fetchone() 
		statusList['count'] = row[0]

		self.response( statusList )

	# 导出到Excel文件
	def exportToExcel(self, data):
		
		self.response(0)
