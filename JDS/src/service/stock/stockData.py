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
from libs.utils.debug import *

class stockDataManage(dbMysql.CURD) :
	def __init__(self,db) :
		if sys.version > '3':
			# python 3.0 +
			super().__init__(db,'stock.stock_day_data',False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(stockDataManage,self).__init__(db,'stock.stock_day_data',False)

class Restful(WebRequestHandler):

	@operator_except
	def get(self):

		# 日线数据、5分钟线分年存放，1分钟线按月存放
		
		# 参数：code: 股票代码
		# dt：类型，日线、分钟线或5分钟线
		# startDate: 开始日期，默认为当前日期
		# count: 需要的记录数

		code 		= self.get_argument('code',  default = "")
		dt 	 		= self.get_argument('dt',  default = "")
		startDate 	= self.get_argument('sd',  default = "")
		count 		= int(self.get_argument("count", default = 0 ))

		today = datetime.datetime.now()

		# 开始日期默认从当天开始
		if startDate == '':
			startDate = today.strftime('%Y-%m-%d %H:%M:%S')

		dbCursor 	= self.db.getCursor()

		sql 	= ""
		subSQL 	= ""

		# 日线数据
		if dt == 'day':

			StartDate 	= time.strptime(startDate, '%Y-%m-%d %H:%M:%S')

			nRecCount 	= 0
			recordYear 	= StartDate.tm_year
			while nRecCount < count and recordYear > 1990:
				# 查询数据
				subSQL += "select id, name, code, trade_time, open, close, high, low, amount, vol "
				subSQL += " from stock.stock_day_data_%d where code = '%s' and trade_time <= '%s' "%(recordYear, code, StartDate)

				# 查询指定年份的记录表中的记录是否已经足够
				sql = "select count(*) from stock.stock_day_data_%d where code = '%s' and trade_time <= '%s' "%(recordYear, code, StartDate)
				dbCursor.execute(sql)
				row = dbCursor.fetchone() 
				nRecCount += row[0]

				# 记录数已经足够
				if nRecCount >= count:
					break

				# 记录数不够，继续查询上一年的记录
				recordYear -= 1

				# 已经到了第一年
				if recordYear < 1990:
					break

				subSQL += " union "

 		# 分钟线数据
		elif dt == "m1":

			StartDate 	= time.strptime(startDate, '%Y-%m-%d %H:%M:%S')

			nRecCount 	= 0
			recordYear 	= StartDate.tm_year
			recordMonth = StartDate.tm_mon

			# 分钟线数据从2015年开始
			while nRecCount < count and recordYear > 2015:

				while nRecCount < count and recordMonth > 0:
					
					subSQL += "select id,   name, code, trade_time, open, close, high, low, amount, vol "
					subSQL += " from stock.stock_minute_1_data_%d_%02d where code  = '%s' and trade_time <= '%s' "%(recordYear, recordMonth, code, StartDate)

					# 查询指定年份的记录表中的记录是否已经足够
					sql = "select count(*) from stock.stock_minute_1_data_%d_%02d where code = '%s' and trade_time <= '%s' "%(recordYear, recordMonth, code, StartDate)
					dbCursor.execute(sql)
					row = dbCursor.fetchone() 
					nRecCount += row[0]

					# 记录数已经足够
					if nRecCount >= count:
						break

					# 记录数不够，继续查询上一月的记录
					recordMonth -= 1

					# 已经到了第一年
					if recordMonth == 0:
						break

					subSQL += " union "

				# 记录数已经足够
				if nRecCount >= count:
					break

				# 继续从上一年数据开始查询
				recordYear -= 1
				recordMonth = 12

		# 5分钟线数据
		elif dt == "m5":

			StartDate 	= time.strptime(startDate, '%Y-%m-%d %H:%M:%S')

			nRecCount 	= 0
			recordYear 	= StartDate.tm_year
			# 5分钟线数据从2015年开始
			while nRecCount < count and recordYear > 2015:
				# 查询数据
				subSQL += "select id, name,    code, trade_time, open, close, high, low, amount, vol "
				subSQL += " from stock.stock_minute_5_data_%d where code = '%s' and trade_time <= '%s' "%(recordYear, code, StartDate)

				# 查询指定年份的记录表中的记录是否已经足够
				sql = "select count(*) from stock.stock_minute_5_data_%d where code = '%s' and trade_time <= '%s' "%(recordYear, code, StartDate)
				dbCursor.execute(sql)
				row = dbCursor.fetchone() 
				nRecCount += row[0]

				# 记录数已经足够
				if nRecCount >= count:
					break

				# 记录数不够，继续查询上一年的记录
				recordYear -= 1

				# 已经到了第一年
				if recordYear < 1990:
					break

				subSQL += " union "

		else:
			raise BaseError(801, "交易数据类型参数错误")

		sql = "select * from ( %s ) c order by c.trade_time desc limit %d"%(subSQL, count)

		# logI("stock data SQL: ", sql)

		dbCursor.execute(sql)
		rows = dbCursor.fetchall()

		data = {}
		data['rows'] = rows
		# data['rows'] = []
		
		data['struct'] = "id, name, code, trade_time, open, close, high, low, amount, vol"

		self.response(data)

	@operator_except
	def patch(self):

		self.response(0)
