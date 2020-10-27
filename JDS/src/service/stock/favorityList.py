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

class favorityList(dbMysql.CURD) :
	def __init__(self,db) :
		if sys.version > '3':
			# python 3.0 +
			super().__init__(db,'stock.stock',False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(favorityList,self).__init__(db,'stock.stock',False)

class Restful(WebRequestHandler):

	@operator_except
	def get(self):

		sql  = "select s.id, s.name, s.code, s.create_time "
		sql += " from stock.favority_list s"
		sql += " order by s.code"

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		data = {}
		data['rows'] = rows
		data['struct'] = "id, name, code, create_time"

		self.response(data)


	# 新增自选股
	@operator_except
	def post(self):

		alldata 	= self.getRequestData()

		s = favorityList(self.db)
		
		lsData = {
			'code' 	: 'code',
			'name' 	: 'name',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass
		
		db = self.getDB("stock.favority_list")
		
		sql  = "select fl.code from stock.favority_list fl"
		sql += " where fl.code = '%s'"%data['code']

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()
		if len(rows) > 0:
			self.response(0)

		else:

			# 新增自选股

			data['create_id'] 	= self.objUserInfo['id']
			data['create_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
			
			id = s.save(data, table = 'stock.favority_list')

			# 创建控制器成功，记录日志并退出
			operation_log(self.db).addLog( self.objUserInfo['id'], "stockManage", 
				"新增自选股：名称：%s，编号：%s"%(data['name'], data['code']), id)

			self.response(id)

	# 删除指定自选股
	# 参数：
	#  	id: 待删除的自选股ID，必选 
	#  
	@operator_except
	def delete(self):
		alldata = self.getRequestData()
		id = self.getParam( alldata, "id", 0)
		if id == 0:
			raise BaseError(801, "必须指定待更删除的自选股ID！")

		sql  = " select fl.name, fl.code from stock.favority_list fl "
		sql += " where fl.id = %s"%id

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()
		logInfo = "删除自选股：名称：%s, 编号：%s"%(rows[0][0], rows[0][1])

		# 删除自选股记录
		s = favorityList(self.db)
		s.remove( id, table = "stock.favority_list", key = "id", delete = True)

		# 记录日志并退出
		operation_log(self.db).addLog( self.objUserInfo['id'], "StockManage", logInfo, id)

		self.response(id)
