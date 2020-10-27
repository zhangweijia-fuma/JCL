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
from libs.utils.utils import *
from libs.utils.debug import *

from pis import controllerManage
from pis.messageManage import SendMsgToController

class subLineInfoManage(dbMysql.CURD) :
	def __init__(self,db) :
		if sys.version > '3':
			# python 3.0 +
			super().__init__(db,'pis.controller_log',False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(subLineInfoManage,self).__init__(db,'pis.controller_log',False)



class Restful(WebRequestHandler):


	# 获取支线列表
	# 参数：
	# 	lid: 运营线路ID

	@operator_except
	def get(self):
		
		lid   	= int(self.get_argument('lid',  default = '0'))
		if lid == 0:
			raise BaseError(801, "错误：缺省运营线路ID参数！")

		sql  = " select sli.id, sli.line_id, sli.sub_line_name, sli.sub_line_name_en, sli.up_start_station, stus.name, sli.up_end_station, stue.name, "
		sql += " sli.down_start_station, stds.name, sli.down_end_station, stde.name, sli.up_start_time, sli.up_end_time, "
		sql += " sli.down_start_time, sli.down_end_time from pis.sub_line_info sli"
		sql += " inner join pis.station stus on stus.id = sli.up_start_station"
		sql += " inner join pis.station stue on stue.id = sli.up_end_station"
		sql += " inner join pis.station stds on stds.id = sli.down_start_station"
		sql += " inner join pis.station stde on stde.id = sli.down_end_station"
		
		sql += " where sli.line_id = %d"%lid
		sql += " order by sli.id "

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		subLineList = {}
		subLineList['rows'] = rows

		subLineList['struct']  = "id, line_id, sub_line_name, sub_line_name_en, up_start_station, up_start_station_name, up_end_station, up_end_station_name, "
		subLineList['struct'] += "down_start_station, down_start_station_name, down_end_station, down_end_station_name, up_start_time, up_end_time, down_start_time, down_end_time"
		subLineList['count'] = len(rows)

		rowdata = {}
		rowdata['subLineList'] = subLineList

		sql  = "select st.id, st.name from pis.station_line sl"
		sql += " inner join pis.station st on st.id = sl.station_id"
		sql += " where sl.line_id = %d and st.type = 'N' "%lid
		sql += " order by sl.sort"

		cur.execute(sql)
		rows = cur.fetchall() 
		
		stationList = {}
		stationList['rows'] = rows
		stationList['struct'] = "id, name"
		stationList['count'] = len(rows)

		rowdata['stationList'] = stationList

		self.response( rowdata )

	# 创建一条支线
	@operator_except
	def post(self):

		alldata = self.getRequestData()

		s = subLineInfoManage(self.db)
		
		lsData = {
			'line_id' 				: 'line_id',
			'sub_line_name'			: 'sub_line_name',
			'sub_line_name_en'		: 'sub_line_name_en',
			'up_start_station' 		: 'up_start_station',
			'up_end_station' 		: 'up_end_station',
			'down_start_station' 	: 'down_start_station',
			'down_end_station' 		: 'down_end_station',
			 
			'up_start_time' 		: 'up_start_time',
			'up_end_time' 			: 'up_end_time',
			'down_start_time'		: 'down_start_time', 
			'down_end_time' 		: 'down_end_time',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass

		data['create_id'] 	= self.objUserInfo['id']
		data['create_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		
		id = s.save(data, table = 'pis.sub_line_info')

		# 创建支线线路成功，记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "subLineInfoManage", 
			"创建支线线路：线路名称：%s"%(data['sub_line_name']), id)

		# 更新支线信息
		self.UpdateSubLineInfo(data['line_id'])

		self.response(id)

	# 创建一条支线
	@operator_except
	def put(self):

		alldata = self.getRequestData()

		s = subLineInfoManage(self.db)
		
		lsData = {
			'id' 					: 'id',
			'line_id' 				: 'line_id',
			'sub_line_name'			: 'sub_line_name',
			'sub_line_name_en'		: 'sub_line_name_en',
			'up_start_station' 		: 'up_start_station',
			'up_end_station' 		: 'up_end_station',
			'down_start_station' 	: 'down_start_station',
			'down_end_station' 		: 'down_end_station',
			 
			'up_start_time' 		: 'up_start_time',
			'up_end_time' 			: 'up_end_time',
			'down_start_time'		: 'down_start_time', 
			'down_end_time' 		: 'down_end_time',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass

		data['update_id'] 	= self.objUserInfo['id']
		data['update_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		
		id = s.save(data, data['id'], table = 'pis.sub_line_info')

		# 更新支线线路成功，记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "subLineInfoManage", 
			"更新支线线路：线路名称：%s"%(data['sub_line_name']), id)

		# 更新支线信息
		self.UpdateSubLineInfo(data['line_id'])

		self.response(id)

	# 删除指定支线
	# 参数：
	#  	id: 待删除的支线ID 
	@operator_except
	def delete(self):

		alldata = self.getRequestData()
		subLineID = self.getParam( alldata, "id", 0)
		if subLineID == 0:
			raise BaseError(801, "参数错误：缺少待更删除的支线ID！")

		sql  = " select sli.sub_line_name, sli.line_id from pis.sub_line_info sli "
		sql += " where sli.id = %s"%subLineID

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()
		logInfo = "删除运营支线：名称：%s, 所属运营线路ID： %d"%(rows[0][0], rows[0][1])
		
		s = subLineInfoManage(self.db)
		s.remove( subLineID, table = "pis.sub_line_info", key = "id", delete = True)

		# 记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "subLineInfoManage", logInfo, subLineID)

		# 更新支线信息
		self.UpdateSubLineInfo(rows[0][1])

		self.response(subLineID)


	@operator_except
	def patch(self):

		paramData = self.getRequestData()
		self.response(0)

	# 更新控制器的支线信息
	def UpdateSubLineInfo(self, lid):
		if lid == 0:
			raise BaseError(801, "参数错误，无线路编号！")

		# 获取支线信息
		sql = """
			select sli.id, sli.line_id, sli.sub_line_name, sli.sub_line_name_en, sli.up_start_station, stus.name, stus.name_en, sli.up_end_station, stue.name, stue.name_en, 
			sli.down_start_station, stds.name, stds.name_en, sli.down_end_station, stde.name, stde.name_en, sli.up_start_time, sli.up_end_time, 
			sli.down_start_time, sli.down_end_time from pis.sub_line_info sli
			inner join pis.station stus on stus.id = sli.up_start_station
			inner join pis.station stue on stue.id = sli.up_end_station
			inner join pis.station stds on stds.id = sli.down_start_station
			inner join pis.station stde on stde.id = sli.down_end_station
			where sli.line_id = %d
			"""%lid

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		key = ['id', 'line_id', 'sub_line_name', 'sub_line_name_en', 'up_start_station', 'up_start_station_name', 'up_start_station_name_en', 'up_end_station', 'up_end_station_name',
		 'up_end_station_name_en', 'down_start_station', 'down_start_station_name', 'down_start_station_name_en', 'down_end_station', 'down_end_station_name', 'down_end_station_name_en',
		 'up_start_time', 'up_end_time', 'down_start_time', 'down_end_time']

		subLineList = []
		for item in rows:
			subLineInfo = dict(zip(key, item))		
			subLineList.append(subLineInfo)

		msgInfo = {
			'msgType' : 'line_info',
			'lineInfo' : subLineList,
		}

		# 只发送给在线的控制器，非在线控制器在启动时会获取到最新的支线信息
		db = self.getDB("pis.controller")
		result = db.findByCond("id,code,ip_address,install_type", "line_id=%d and status = '0' "%lid)
		items = dbResultToDict(result)

		if len(items) == 0:
			raise BaseError(801, "系统错误：无控制器， %d"%lid)

		msgInfo['controllerList'] = items

		SendMsgToController(self, msgInfo)

		return

