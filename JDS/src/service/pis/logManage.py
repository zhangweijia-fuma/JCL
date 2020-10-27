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
from libs.utils.debug import *

from pis import controllerManage

class logManage(dbMysql.CURD) :
	def __init__(self,db) :
		if sys.version > '3':
			# python 3.0 +
			super().__init__(db,'pis.controller_log',False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(logManage,self).__init__(db,'pis.controller_log',False)



class Restful(WebRequestHandler):


	# 获取日志信息
	# 参数：
	# 	lid: 运营线路ID
	# 	sid：车站ID
	# 	cid：控制器ID
	# 	tid：列车ID
	# 	st：开始时间，格式为YYYY-MM-DD HH:MM:SS
	# 	et：结束时间
	# 返回：日志列表
	@operator_except
	def get(self):
		
		op 		= self.get_argument('op',  default = "data")
		lid   	= int(self.get_argument('lid',  default = '0'))
		sid 	= int(self.get_argument('sid', default = '0'))
		cid 	= int(self.get_argument('cid', default = '0'))
		tid		= int(self.get_argument('tid',  default = '0'))
		st 		= self.get_argument('st',  default = "")
		et		= self.get_argument('et',  default = "")

		offset 	= int(self.get_argument('o',  default = '1'))
		rowlimit= int(self.get_argument('r',  default = '20'))

		offset = ( offset - 1 ) * rowlimit
		sql_limit = " limit " + str(rowlimit) + " offset " + str(offset)

		sql  = " select log.id, log.create_time, log.update_time, log.create_id, log.update_id, log.controller, log.date, "
		sql += " log.time, log.type, log.content, ct.code, log.log_id, log.level, log.title"
		sql += " from pis.controller_log log"

		sql_inner  = " inner join pis.controller ct on ct.id = log.controller"
		sql += sql_inner

		sql_where = ""
		if cid > 0:
			sql_where = " where log.controller = %d"%cid
		else:
			if sid > 0:
				sql_where = " where ct.install_id = %d and ct.install_type >= '2'"%sid

			elif tid > 0:
				sql_where  = " where ct.install_id = %d and ct.install_type <= '1' "%tid

			if lid > 0:
				if sql_where == "":
					sql_where = " where ct.line_id = %d "%lid
				else:
					sql_where += " and ct.line_id = %d"%lid

		if st != "" and et != "":
			if sql_where != "":			
				sql_where += " and ( ( log.date || ' ' || log.time ) >= '%s:00' ) and ( (log.date || ' ' || log.time) <= '%s:00') "%(st, et)
			else:
				sql_where = " where ( ( log.date || ' ' || log.time ) >= '%s:00' ) and ( (log.date || ' ' || log.time) <= '%s:00') "%(st, et)
	
		sql += sql_where
		sql += " order by log.date DESC, log.time DESC"

		if op == "data":
			sql += sql_limit

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		if op == "excel":
			self.exportToExcel( rows )

		rowdata = {}

		logList = {}
		logList['rows'] = rows
		logList['struct']  = "id, create_time, update_time, create_id, update_id, controller,date, time, type, content, "
		logList['struct'] += "controller_code, log_id, level, title" 

		sql  = "select count(log.id) from pis.controller_log log"
		sql += sql_inner
		sql += sql_where

		cur.execute(sql)
		row = cur.fetchone() 
		logList['count'] = row[0]

		self.response( logList )


	# 输出日志到excel文件
	def exportToExcel(self, rowdata):

		key  = ['id', 'create_time', 'update_time', 'create_id', 'update_id', 'controller', 'date', 'time', 'type', 'content', 
		'controller_code', 'log_id', 'level', 'title', ]
        
		logList = []
		for i, item in enumerate(rowdata):
			logInfo = dict(zip(key, item))
			logList.append(logInfo)

		cur = self.db.getCursor()
		utils.getDataDict(cur, logList, "type", "CTRL_LOG_TYPE", "type_name")
		utils.getDataDict(cur, logList, "level", "CTRL_LOG_LEVEL", "level_name")
		
		data = []
		sn = 1
		for logInfo in logList:
			
			if 'type_name' not in logInfo:
				logInfo['type_name'] = ""

			if 'level_name' not in logInfo:
				logInfo['level_name'] = ""

			itemData = []
			itemData.append( sn )
			sn = sn + 1
			itemData.append( logInfo['controller_code'] )					# 控制器编号
			itemData.append( logInfo['date'] + ' ' + logInfo['time'] )		# 日志日期
			itemData.append( logInfo['level_name'])						# 日志级别
			itemData.append( logInfo['type_name'] )						# 日志类型
			itemData.append( logInfo['title'] )							# 日志标题
			itemData.append( logInfo['content'] )						# 日志内容
			
			data.append(itemData) 

		struct = "SN, 控制器编号, 时间, 级别, 类型, 标题, 内容"

		path = {}
		path = excel.createTempFile("xls")
		

		excel.saveExcel(path['path'], struct, data )
		
		logInfo = " %s 下载了控制器日志清单 "%self.objUserInfo['name']
		operation_log(self.db).addLog(self.GetUserInfo(), "logManage", logInfo, 0)

		self.response(path)


	# 获取下拉列表信息
	# 参数：
	# 	lid: 运行线路
	# 	sid：车站
	# 	tid: 列车

	@operator_except
	def patch(self):

		paramData = self.getRequestData()

		lid = int(self.getParam(paramData, 'lid', '0'))
		sid = int(self.getParam(paramData, 'sid', '0'))
		tid = int(self.getParam(paramData, 'tid', '0'))

		# 返回的数据
		data = {}

		# 运营线路列表
		sql  = "select ol.id, ol.name "
		sql += " from pis.operation_line ol"
		sql += " order by ol.code"

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		lineList = {}
		lineList['struct']	= "id, name"
		lineList['rows'] 	= [(0, "选择全部", )] + rows

		data['lineList'] 	= lineList

		# 列车列表
		sql  = "select t.id, t.name, t.code "
		sql += " from pis.train t"
		sql += " inner join pis.operation_line ol on ol.id = t.line_id"
		if lid > 0:
			sql += " and t.line_id = %d"%lid

		sql += " order by t.code"

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		trainList = {}
		trainList['struct']	= "id, name"
		trainList['rows'] 	= [(0, "选择全部", )] + rows

		data['trainList'] 	= trainList

		# 车站列表
		sql  = "select st.id, st.name, st.code "
		sql += " from pis.station st"
		if lid > 0:
			sql += " inner join pis.station_line sl on sl.station_id = st.id and sl.line_id = %d"%lid
		if lid > 0:
			sql += " order by sl.sort"
		else:
			sql += " order by st.code"

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		stationList = {}
		stationList['struct']	= "id, name, code"
		stationList['rows'] 	= [(0, "选择全部", "" )] + rows

		data['stationList'] 	= stationList

		# 控制器列表
		sql_where = ""
		sql  = "select ct.id, ct.name, ct.code "
		sql += " from pis.controller ct"
		if tid > 0:
			# 安装在列车上的控制器
			sql += " inner join pis.train t on t.id = ct.install_id and ct.install_id = %d"%tid
			sql += " where ct.install_type <= '1' "
		elif sid > 0:
			# 安装在车站的控制器
			sql += " inner join pis.station st on st.id = ct.install_id and st.id = %d"%sid
			if lid > 0:
				sql += " inner join pis.station_line sl on sl.station_id = st.id and sl.line_id = %d"%lid
			sql += " where ct.install_type >= '2' "

		sql += " order by ct.code"

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		controllerList = {}
		controllerList['struct']	= "id, name, code"
		controllerList['rows'] 	= [(0, "选择全部", "" )] + rows

		data['controllerList'] 	= controllerList


		# 发送给控制器的消息类型列表
		sql  = "select cv.code, cv.name "
		sql += " from system.code_value cv"
		sql += " where cv.type_code = 'MSG_TYPE_CONTROLLER' "
		sql += " order by cv.sort"

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		msgTypeList = {}
		msgTypeList['struct']	= "code, name"
		msgTypeList['rows'] 	= rows

		data['msgTypeList'] 	= msgTypeList

		self.response(data)
