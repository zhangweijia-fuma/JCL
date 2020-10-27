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
from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE
from libs.utils import debug

class stockManage(dbMysql.CURD) :
	def __init__(self,db) :
		if sys.version > '3':
			# python 3.0 +
			super().__init__(db,'stock.stock',False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(stockManage,self).__init__(db,'stock.stock',False)

class Restful(WebRequestHandler):

	@operator_except
	def get(self):

		sql  = "select s.id, s.name, s.code, s.full_name, s.name_en, s.list_date, s.regist_addr, s.capitalization, s.circulatingshares "
		sql += " from stock.stock s"
		sql += " order by s.code"
		# sql += " limit 100 "

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		data = {}
		data['rows'] = rows
		data['struct'] = "id, name, code, full_name, name_en, list_date, regist_addr, capitalization, circulatingshares"

		self.response(data)

