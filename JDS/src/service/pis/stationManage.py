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

from pis import controllerManage

class stationManage(dbMysql.CURD) :
	def __init__(self,db) :
		if sys.version > '3':
			# python 3.0 +
			super().__init__(db,'pis.station',False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(stationManage,self).__init__(db,'pis.station',False)



class Restful(WebRequestHandler):

	# 获取指定运营线路的车站列表
	# 参数：
	# 	lid: 运营线路ID
	# 	
	# 返回：车站列表
	@operator_except
	def get(self):
		
		lid 	= int(self.get_argument('lid', default = '0'))
		if lid == 0:
			raise BaseError(801, "参数错误：缺少线路ID！")
		

		sql  = " select st.id, st.name, st.name_en, st.code, st.type, st.ip_address, st.description, st.status"
		sql += " from pis.station st"
		sql += " inner join pis.station_line sl on sl.station_id = st.id and sl.line_id =%d"%lid
		sid 	= int(self.get_argument('sid', default = '0'))
		if sid != 0:
			sql += " where st.id = %d"%sid
		sql += " order by sl.sort"

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		rowdata = {}

		stationList = {}
		stationList['rows'] = rows
		stationList['struct']  = "id, name, name_en, code, type, ip_address, description, status"

		sql  = "select count(st.id) from pis.station st "
		sql += " inner join pis.station_line sl on sl.station_id = st.id and sl.line_id =%d"%lid

		cur.execute(sql)
		row = cur.fetchone() 
		stationList['count'] = row[0]

		self.response( stationList )

	# 创建车站，如果车站已经存在，则检查是否是在归属于新的运营线路
	@operator_except
	def post(self):

		alldata 	= self.getRequestData()
		lineInfo 	= self.getParam( alldata, "lineInfo", None)

		if lineInfo == None or len(lineInfo) == None:
			raise BaseError(801, "参数错误，缺少车站归属线路信息！")

		s = stationManage(self.db)
		
		lsData = {
			'name'			: 'name',
			'name_en' 		: 'name_en',
			'code'			: 'code',
			'type'			: 'type',
			'ip_address'	: 'ip_address',
			'description'	: 'description',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass
		
		db = self.getDB("pis.station")

		# 检查IP地址是否重复
		if controllerManage.isIPAddrExist(self, data['ip_address']):
			raise BaseError(801, "设备IP地址 %s 已经存在！"%data['ip_address'] )

		# 检查序号是否已经存在
		db = self.getDB("pis.station_line")
		for line in lineInfo:
			result = db.findByCond("sort", "line_id = %s and sort = %s "%(line['line_id'], line['sort']) )
			if len( result['rows'] ) > 0 :
				raise BaseError(801, "运营线路 %s 的序号%s已经存在！"%(line['name'], line['sort']) )

		# 检查车站名称是否重复
		db = self.getDB("pis.station")
		result = db.findByCond("code, id", "name = '%s' or name_en = '%s' "%(data['name'], data['name_en']))
		if len( result['rows'] ) > 0 :
			# 再检查是否有新的运营线路
			db = self.getDB("pis.station_line")
			
			# 现有车站信息的ID及编号
			scode 	= result['rows'][0][0]
			sid 	= result['rows'][0][1]

			data['code'] = scode

			# 检查每条运营线路是否有现有车站
			for line in lineInfo:
				result = db.findByCond("line_id", "station_id=%d and line_id = %s"%(sid, line['line_id']) )
				if len(result['rows'] ) > 0:
					raise BaseError(801, "车站名称 %s 已经存在！"%data['name'])

			# 没有找到记录，则为把现有车站加入到新的运营线路
			# 不需要更新车站信息
		else:
			# 新建车站

			data['create_id'] 	= self.objUserInfo['id']
			data['create_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
			
			# 创建车站记录
			sid = s.save(data, table = 'pis.station')

			# 车站编号，由运营商自行编号
			# 若无运营商编号，则内部编号规则为：
			# 规则：T- AAAAAA，AAAAAA为记录的ID值，T为站点类型，运营站点为N，特殊站点为S。
			if 'code' not in data:
				data['code'] = "%s-%06d"%(data['type'], sid)

			s.save(data, sid, table = 'pis.station')

		# 更新车站的线路信息
		for line in lineInfo:
			lsData = {
				'station_id' 	: 'station_id', 
				'line_id' 		: 'line_id',
				'up_start_time' : 'up_start_time',
				'up_end_time'	: 'up_end_time',
				'down_start_time': 'down_start_time',
				'down_end_time' : 'down_end_time',
				'sort' 			: 'sort',
			}

			linedata = { }
			for (k, v) in lsData.items():
				try:
					linedata[k] = line[v]
				except:
					pass

			linedata['station_id']  = sid
			linedata['create_id'] 	= self.objUserInfo['id']
			linedata['create_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
			
			s.save(linedata, table = 'pis.station_line')

		# 创建车站成功，记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "stationManage", 
			"创建车站信息：名称：%s，编号：%s"%(data['name'], data['code']), sid)

		self.response(sid)

	# 更新车站信息
	# 参数：
	#   id: 待更新车站的ID，必选
	#   其他参数可选
	@operator_except
	def put(self):
		
		alldata = self.getRequestData()
		if self.getParam( alldata, "id", 0) == 0:
			raise BaseError(801, "必须指定待更新信息的车站ID！")

		if self.getParam( alldata, "name", 0) == 0:
			raise BaseError(801, "必须指定待更新信息的车站中文名称！")
		
		if self.getParam( alldata, "name_en", 0) == 0:
			raise BaseError(801, "必须指定待更新信息的车站英文名称！")
		
		if self.getParam( alldata, "code", 0) == 0:
			raise BaseError(801, "必须指定待更新信息的车站编码！")

		lineInfo = self.getParam( alldata, "lineInfo", None)

		if lineInfo == None or len(lineInfo) == None:
			raise BaseError(801, "参数错误，缺少车站归属线路信息！")

		s = stationManage(self.db)
		
		lsData = {
			'id' 			: 'id',
			'name'			: 'name',
			'name_en'		: 'name_en',
			'code'			: 'code',
			'type'			: 'type',
			'ip_address'	: 'ip_address',
			'description'	: 'description',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass

		# 检查车站名称是否重复
		db = self.getDB("pis.station")
		result = db.findByCond("name", "(name = '%s' or name_en = '%s' ) and id <> %s "%(data['name'], data['name_en'], data['id']) )
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "车站名称 %s, %s 重复！"%(data['name'], data['name_en']))

		# 检查编号是否重复
		result = db.findByCond("code", "code = '%s' and id <> %s "%(data['code'], data['id']))
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "车站编号 %s 重复！"%data['code'])

		# 检查IP地址是否重复
		result = db.findByCond("ip_address", "ip_address = '%s' and id <> %s "%(data['ip_address'], data['id']))
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "车站地址 %s 已经存在！"%data['ip_address'])


		data['update_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		data['update_id'] = self.objUserInfo['id']
		id = s.save(data, data['id'], table = 'pis.station')

		logInfo = "更新车站信息：名称：%s，%s，编号：%s"%(data['name'], data['name_en'], data['code'])

		# 更新station_line 信息
		
		for line in lineInfo:
			lsData = {
				'id' 			: 'id',
				'station_id' 	: 'station_id', 
				'line_id' 		: 'line_id',
				'up_start_time' : 'up_start_time',
				'up_end_time'	: 'up_end_time',
				'down_start_time': 'down_start_time',
				'down_end_time' : 'down_end_time',				
				'sort' 			: 'sort',
			}

			linedata = { }
			for (k, v) in lsData.items():
				try:
					linedata[k] = line[v]
				except:
					pass

			# 检查序号是否重复
			db = self.getDB("pis.station_line")
			result = db.findByCond("sort", "sort = %s and line_id = %s and station_id <> %s "%(linedata['sort'], linedata['line_id'], linedata['station_id']))
			if len( result['rows'] ) > 0 :
				raise BaseError(801, "车站序号 %s 已经存在！"%linedata['sort'])

			linedata['update_id'] 	= self.objUserInfo['id']
			linedata['update_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
			
			s.save(linedata, linedata['id'], table = 'pis.station_line')

		operation_log(self.db).addLog(self.GetUserInfo(), "stationManage", logInfo, id)
		self.response(id)


	# 删除指定车站
	# 参数：
	#  	id: 待删除的车站ID，必选 
	#  
	#  返回值：
	@operator_except
	def delete(self):
		alldata = self.getRequestData()
		sid = self.getParam( alldata, "sid", 0)
		if sid == 0:
			raise BaseError(801, "必须指定待更删除的车站ID！")

		lid = self.getParam( alldata, "lid", 0)
		if lid == 0:
			raise BaseError(801, "必须指定待更删除车站归属的运营线路ID！")

		sql  = " select st.name, st.name_en, st.code from pis.station st "
		sql += " where st.id = %s"%sid

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()
		logInfo = "删除车站：名称：%s，%s，编号：%s"%(rows[0][0], rows[0][1], rows[0][2])

		s = stationManage(self.db)

		# 删除车站记录
		s.remove( sid, table = "pis.station", key = "id", delete = True)

		# 删除station_line表中的记录
		s.remove( sid, table = "pis.station_line", key = "station_id", delete = True)

		# 删除车站安装的控制器信息
		sql = "delete from pis.controller ct where ct.install_id = %d and ct.install_type >= '2' "%sid
		cur.execute(sql)

		# 记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "stationManage", logInfo, sid)

		# 重新获取线路的车站列表，以更新站点树
		# lineNode = lineManage.getStationList(lid)

		self.response(sid)


	# 获取车站归属的运营线路列表
	# 参数：
	# 	sid : 车站ID，必选 
	# 	op  : line，data或both，操作方式
	# 	data: 获取指定车站的开行信息
	# 	both：同时获取线路列表和开行信息
	# 	line: 只获取线路列表

	@operator_except
	def patch(self):

		alldata = self.getRequestData()
		stationID = self.getParam( alldata, "sid", 0)
		if stationID  == 0:
			raise BaseError(801, "参数错误：缺少车站ID！")

		op = self.getParam( alldata, "op", "")
		if op == "":
			raise BaseError(801, "必须指定操作模式！")

		# 返回的数据
		data = {}

		if op == "line" or op == "both":
		
			# 运营线路列表
			sql  = "select ol.id, ol.name "
			sql += " from pis.operation_line ol"
			sql += " inner join pis.station_line st on st.station_id = %d and st.line_id = ol.id"%stationID
			sql += " order by ol.code"

			cur = self.db.getCursor()
			cur.execute(sql)
			rows = cur.fetchall()

			lineList = {}
			lineList['struct']	= "id, name"
			lineList['rows'] 	= rows

			data['lineList'] 	= lineList

		if op == "data" or op == "both":
			# 获取指定车站开行信息

			cur  = self.db.getCursor()

			sql  = "select sl.id, sl.line_id, sl.station_id, sl.sort, sl.up_start_time, sl.up_end_time, "
			sql += " sl.down_start_time, sl.down_end_time, ol.name "
			sql += " from pis.station_line sl"
			sql += " inner join pis.operation_line ol on ol.id = sl.line_id"
			sql += " where sl.station_id = %s"%stationID
			sql += " order by sl.sort"

			cur.execute(sql)
			rows = cur.fetchall()

			selectedLineList = {}
			selectedLineList['struct']	= "id, line_id, station_id, sort, up_start_time, up_end_time, "
			selectedLineList['struct'] += "down_start_time, down_end_time, name"
			selectedLineList['rows'] 	= rows

			data['selectedLineList'] 	= selectedLineList

		self.response(data)



# 获取指定线路的站点信息
# 参数：
# 	lid： 线路ID
# 	
# 返回值：以运营线路名称为根节点的树结构，一级子节点为车站，二级子节点为控制器
def getStationList(self, lid):

	cur  = self.db.getCursor()

	sql  = "select st.id, st.name, st.code, st.type, st.description, cv.name as station_type_name,  "
	sql += " sl.sort, sl.up_start_time, sl.up_end_time, sl.down_start_time, sl.down_end_time, ol.name, st.ip_address, st.name_en, st.status"
	sql += " from pis.station st"
	sql += " inner join pis.station_line sl on sl.line_id = %d and sl.station_id = st.id"%lid
	sql += " inner join pis.operation_line ol on ol.id = %d"%lid
	sql += " inner join system.code_value cv on cv.type_code = 'STATION_TYPE' and cv.code = ST.type "
	sql += " order by sl.sort"

	cur.execute(sql)
	rows = cur.fetchall()

	#将所有元组转为对象,因为前台tree组件无法加载元组类型
	stationList = []
	nameSort  = 1
	for row in rows:
		obj = {}
		obj['id']	   			= row[0]
		obj['name']	 			= "%03d"%nameSort + ' - ' + row[1]
		obj['show_name'] 		= row[1]
		obj['code']	 			= row[2]
		obj['type']	 			= row[3]
		obj['description'] 		= row[4]
		obj['type_name'] 		= row[5]
		obj['sort']	 			= row[6]
		obj['up_start_time']	= row[7]
		obj['up_end_time']	 	= row[8]
		obj['down_start_time'] 	= row[9]
		obj['down_end_time'] 	= row[10]
		obj['line_name'] 		= row[11]
		obj['ip_address']		= row[12]
		obj['name_en'] 			= row[13]
		obj['status'] 			= row[14]
		
		obj['nodeType'] 		= 'station'
		obj['children'] 		= []
		obj['chkDisabled'] 		= True
		obj['open']	 			= True
		obj['checked']  		= False
		obj['isItem']   		= False

		nameSort += 1
		obj['children'] = controllerManage.getControllerList(self, cur, obj['id'], '1', lid)

		stationList.append(obj)

	return stationList
