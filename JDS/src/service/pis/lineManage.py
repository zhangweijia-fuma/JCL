#coding:utf-8

import re
import time
import datetime
import sys

from db import dbMysql
from web.baseException import errorDic,BaseError
from web.Base  import WebRequestHandler,BaseError,operator_except

from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE
from libs.utils.debug import *

import xlrd, xlwt
from public.excel import excel

from libs.db.dbHelper import *

from pis import controllerManage
from pis import trainManage
from pis import stationManage

class lineManage(dbMysql.CURD) :
	def __init__(self,db) :
		if sys.version > '3':
			# python 3.0 +
			super().__init__(db,'pis.operation_line',False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(lineManage,self).__init__(db,'pis.operation_line',False)



class Restful(WebRequestHandler):


	# 获取地铁运营线路信息
	# 参数：
	# 	无
	# 返回：运营线路列表
	@operator_except
	def get(self):

		lineName = self.get_argument('name', default = '')
		if lineName != '':
			sql_where = " where ol.name = '%s'"%lineName
		else:
			sql_where = "";

		rowdata = {}
		cur = self.db.getCursor()

		sql  = "select ol.id, ol.create_time, ol.update_time, ol.create_id, ol.update_id, "
		sql += " ol.name, ol.name_en, ol.code, ol.memo "
		sql += " from pis.operation_line ol "
		sql += sql_where
		sql += " order by ol.code"

		cur.execute(sql)
		rows = cur.fetchall()

		rowdata['struct']  = "id, create_time, update_time, create_id, update_id, name, name_en, code, memo"
		rowdata['rows']	= rows

		sql  = "select count(*) from pis.operation_line ol  " + sql_where

		cur.execute(sql)
		row = cur.fetchone() 
		rowdata['count'] = row[0]

		lineInfo = {}
		lineInfo['lineList'] = rowdata

		self.response( lineInfo )

	# 创建一条运营线路
	@operator_except
	def post(self):

		alldata = self.getRequestData()

		s = lineManage(self.db)
		
		lsData = {
			'name'			: 'name',
			'name_en'		: 'name_en',
			'code' 			: 'code', 
			'memo' 			: 'memo',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass

		# 检查线路名称是否重复
		db = self.getDB("pis.operation_line")
		result = db.findByCond("name", "name = '%s' or name_en = '%s' "%(data['name'], data['name_en']))
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：运营线路名称已经存在！")

		# 检查线路编号是否重复
		db = self.getDB("pis.operation_line")

		result = db.findByCond("code", "code = '%s' "%data['code'])
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：运营线路编号已经存在！")

		data['create_id'] 	= self.objUserInfo['id']
		data['create_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		
		id = s.save(data, table = 'pis.operation_line')

		# 创建线路成功，记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "lineManage", 
			"创建运营线路：线路名称：%s，线路编号：%s"%(data['name'], data['code']), id)

		self.response(id)

	# 更新线路信息
	# 参数：
	#   id: 待更新线路的ID，必选
	#   其他参数可选
	@operator_except
	def put(self):

		alldata = self.getRequestData()
		if self.getParam( alldata, "id", 0) == 0:
			raise BaseError(801, "参数错误：缺少待更新信息的线路ID！")

		line_name = self.getParam( alldata, "name", "")
		if line_name == "":
			raise BaseError(801, "参数错误，无线路中文名称！")

		line_name_en = self.getParam( alldata, "name_en", "")
		if line_name_en == "":
			raise BaseError(801, "参数错误，无线路英文名称！")

		line_code = self.getParam( alldata, "code", "")
		if line_code == "":
			raise BaseError(801, "参数错误，无线路编号！")

		s = lineManage(self.db)
		
		lsData = {
			'id' 			: 'id',
			'name'			: 'name',
			'name_en'		: 'name_en',
			'code' 			: 'code', 
			'memo' 			: 'memo',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass

		# 检查线路名称是否重复
		db = self.getDB("pis.operation_line")
		result = db.findByCond("name", "( name = '%s' or name_en = '%s' ) and id <> %s "%(data['name'], data['name_en'], data['id']) )
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：线路名称重复！")

		# 检查线路编号是否重复
		result = db.findByCond("code", "code = '%s' and id <> %s "%( data['code'], data['id'] ))
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：运营线路编号 %s 已经存在！"% data['code'])

		data['update_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		data['update_id'] 	= self.objUserInfo['id']

		id = s.save(data, data['id'], table = 'pis.operation_line')

		logInfo = "更新运营线路信息：线路名称：%s，线路编号：%s"%(data['name'], data['code'])

		operation_log(self.db).addLog(self.GetUserInfo(), "lineManage", logInfo, id)

		self.response(id)


	# 删除指定线路
	# 参数：
	#  	id: 待删除的线路ID，必选 
	@operator_except
	def delete(self):
		alldata = self.getRequestData()
		lid = self.getParam( alldata, "id", 0)
		if lid == 0:
			raise BaseError(801, "参数错误：缺少待更删除的线路ID！")

		sql  = " select ol.name, ol.code from pis.operation_line ol "
		sql += " where ol.id = %s"%lid

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()
		logInfo = "删除运营线路：名称：%s, 编号：%s"%(rows[0][0], rows[0][1])

		# 删除线路所有车站、列车的控制器
		controllerManage.deleteController(self, lid, 0, 0, 0)

		# 删除线路所在的车站记录

		# 换乘站不删除
		sql  = "delete from pis.station st " 
		sql += " where ( id in "
		sql += " ( select sl.station_id from pis.station_line sl, (select count(*) as _count,station_id from pis.station_line group by station_id ) sl_count "
		sql += " where sl.line_id = %d and sl.station_id = sl_count.station_id and sl_count._count = 1))"%lid
		cur.execute(sql)
		
		s = lineManage(self.db)
		s.remove( lid, table = "pis.station_line", key = "line_id", delete = True)

		# 删除列车记录
		s.remove( lid, table = "pis.train", key = "line_id", delete = True)

		# 删除所有支线记录
		s.remove( lid, table = "pis.sub_line_info", key = "line_id", delete = True)
		
		# 删除线路记录
		s.remove( lid, table = "pis.operation_line", key = "id", delete = True)

		# 记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "lineManage", logInfo, lid)
		self.response(lid)


	# 获取运营线路信息
	# 参数：
	# 	op  : line 或者是data，操作方式
	# 	line: 只获取线路列表
	# 	data: 获取指定线路的车站、控制器信息
	# 	lid : 线路ID 

	@operator_except
	def patch(self):

		alldata = self.getRequestData()
		op = self.getParam( alldata, "op", "")
		if op == "":
			raise BaseError(801, "必须指定操作模式！")

		if op == "line":
			# 运营线路列表
			 
			# 返回的数据
			data = {}

			sql  = "select ol.id, ol.name"
			sql += " from pis.operation_line ol"
			sql += " order by ol.code"

			cur = self.db.getCursor()
			cur.execute(sql)
			rows = cur.fetchall()

			# 获取每条线路车站的最大序号
			linedata = []
			for row in rows:
				sql  = " select max(sl.sort)"
				sql += " from pis.station_line sl"
				sql += " where sl.line_id = %d"%row[0]
				cur.execute(sql)
				sorts = cur.fetchall()
				if  len(sorts) == 0:
					sort = row[0] * 200
				else:
					sort = sorts[0][0]

				linedata.append( (row[0], row[1], sort))

			lineList = {}
			lineList['struct']	= "id, name, max_sort"
			lineList['rows'] 	= linedata

			data['lineList'] 	= lineList

			self.response(data)

		elif op == "data":
			# 获取指定线路的车站、控制器信息
			
			ut = self.getParam( alldata, "ut", "")
			if ut == "":
				raise BaseError(801, "参数错误：缺少类型参数UT！")

			lid = int(self.getParam( alldata, "lid", "0"))
			if lid == 0:
				raise BaseError(801, "参数错误：缺少运营线路ID！")

			sql  = " select ol.name from pis.operation_line ol where ol.id =%d"%lid
			cur = self.db.getCursor()
			cur.execute(sql)
			rows = cur.fetchall()
			line_name = rows[0][0]

			lineNode = {}
			lineNode['name'] 		= line_name
			lineNode['id'] 			= lid
			lineNode['chkDisabled'] = True
			lineNode['open']	 	= True
			lineNode['checked']  	= False
			lineNode['isItem']   	= False
			
			# logI("信息：", lineNode)
			
			if ut == "station":
				lineNode['nodeType'] = 'stationRoot'
				lineNode['children'] = stationManage.getStationList(self, lid)
			elif ut == "train":
				lineNode['nodeType'] = 'trainRoot'
				lineNode['children'] = trainManage.getTrainList(self, lid)
			else:
				raise BaseError(801, "参数错误：指定的类型参数UT错误！")

			self.response(lineNode)

		elif op == "station":
			lid = int(self.getParam( alldata, 'lid', '0'))
			if lid == 0 :
				raise BaseError(801, "参数错误：指定的运营线路ID错误！")

			self.exportStationInfo(lid)

		elif op == "train":
			lid = int(self.getParam( alldata, 'lid', '0'))
			if lid == 0 :
				raise BaseError(801, "参数错误：指定的运营线路ID错误！")

			self.exportTrainInfo(lid)

		else:
			raise BaseError(801, "参数错误：指定的操作类型OP错误！")

	# 导出车站信息到Excel文件
	def exportStationInfo(self, lid):


		sql  = "select st.id, st.code, st.name, st.ip_address"
		sql += " from pis.station st"
		sql += " inner join pis.station_line sl on sl.station_id = st.id"
		sql += " where sl.line_id = %s "%lid
		sql += " order by st.code"

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		key = ['id', 'code', 'name', 'ip_address']

		data = []
		sn = 1

		for item in rows:
			station = dict(zip(key, item))

			data.append( [sn, station['code'], station['name'], station['ip_address'] ] )
			sn = sn + 1

			sql  = " select lc.code, lc.name, lc.ip_address"
			sql += " from pis.controller lc"
			sql += " where lc.install_id = %s and lc.install_type >= '2' and lc.install_type <= '4' "%station['id']
			sql += " order by lc.code "

			cur.execute(sql)
			rowsdata = cur.fetchall()

			key2 = ['code', 'name', 'ip_address']

			for iteminfo in rowsdata:
				controller = dict(zip(key2, iteminfo))
				data.append( [ sn, '', '', '', controller['code'], controller['name'], controller['ip_address'] ] )
				sn = sn + 1

		struct = "SN, 车站编号, 车站名称, 服务器地址, 控制器编号, 控制器类型, 控制器地址"

		path = {}
		path = excel.createTempFile("xls")
		
		excel.saveExcel(path['path'], struct, data )

		logInfo = " %s 下载了车站信息 "%self.objUserInfo['name']
		operation_log(self.db).addLog(self.GetUserInfo(), "listManage", logInfo, 0)

		self.response(path)

	# 导出列车控制器信息到excel文件
	def exportTrainInfo(self, lid):

		sql  = "select t.id, t.code, t.name"
		sql += " from pis.train t"
		sql += " where t.line_id = %s "%lid
		sql += " order by t.sort"

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		key = ['id', 'code', 'name']

		data = []
		sn = 1

		for item in rows:
			train = dict(zip(key, item))

			
			sql  = " select lc.code, lc.ip_address"
			sql += " from pis.controller lc"
			sql += " where lc.install_id = %s and lc.install_type >= '0' and lc.install_type <= '1' "%train['id']
			sql += " order by lc.code "

			cur.execute(sql)
			rowsdata = cur.fetchall()

			data.append( [ sn, train['code'], train['name'], rowsdata[0][0], rowsdata[0][1], rowsdata[1][0], rowsdata[1][1] ] )
			sn = sn + 1

		struct = "SN, 列车编号, 列车名称, 车头控制器编号, 车头控制器地址, 车尾控制器编号, 车尾控制器地址"

		path = {}
		path = excel.createTempFile("xls")
		

		excel.saveExcel(path['path'], struct, data )

		logInfo = " %s 下载了列车控制器信息 "%self.objUserInfo['name']
		operation_log(self.db).addLog(self.GetUserInfo(), "listManage", logInfo, 0)

		self.response(path)

