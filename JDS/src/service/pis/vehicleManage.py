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

from pis import lineManage

class vehicleManage(dbMysql.CURD) :
	def __init__(self,db) :
		if sys.version > '3':
			# python 3.0 +
			super().__init__(db,'pis.vehicle',False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(vehicleManage,self).__init__(db,'pis.vehicle',False)



class Restful(WebRequestHandler):


	# 获取地列车信息
	# 参数：
	# 	无
	# 返回：运营线路列表，以及运营线路所拥有的车站、车站控制器列表
	@operator_except
	def get(self):

		self.response(0)

	# 创建车辆
	@operator_except
	def post(self):

		alldata 	= self.getRequestData()

		s = vehicleManage(self.db)
		
		lsData = {
			'name'			: 'name',
			'code' 			: 'code', 
			'serial_no'		: 'serial_no',
			'type'			: 'type',
			'default_count'	: 'default_count',
			'train_id'		: 'train_id',
			'description'	: 'description',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass
		
		# 检查车辆编号是否重复
		db = self.getDB("pis.vehicle")
		result = db.findByCond("code", "code = '%s' "%data['code'])
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "车辆编号 %s 已经存在！"%data['code'])

		# 检查序列号是否重复
		result = db.findByCond("serial_no", "serial_no = '%s' "%data['serial_no'])
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "车辆序列号 %s 已经存在！"%data['serial_no'])

		# 新建车站

		data['create_id'] 	= self.objUserInfo['id']
		data['create_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		
		# 创建车站记录
		vid = s.save(data, table = 'pis.vehicle')

		# 创建车辆成功，记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "vehicleManage", 
			"创建车辆信息：名称：%s，编号：%s"%(data['name'], data['code']), vid)

		self.response(vid)

	# 更新车辆信息
	# 参数：
	#   id: 待更新车站的ID，必选
	#   其他参数可选
	@operator_except
	def put(self):
		
		alldata = self.getRequestData()
		if self.getParam( alldata, "id", 0) == 0:
			raise BaseError(801, "必须指定待更新信息的车辆ID！")

		if self.getParam( alldata, "code", 0) == 0:
			raise BaseError(801, "必须指定待更新信息的车辆编码！")

		s = vehicleManage(self.db)
		
		lsData = {
			'id' 			: 'id',
			'name'			: 'name',
			'code' 			: 'code', 
			'serial_no'		: 'serial_no',
			'type'			: 'type',
			'default_count'	: 'default_count',
			'train_id'		: 'train_id',
			'description'	: 'description',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass

		# 检查车站名称是否重复
		db = self.getDB("pis.vehicle")
		result = db.findByCond("serial_no", "serial_no = '%s' and id <> %s "%(data['serial_no'], data['id']) )
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "车辆序列号 %s 重复！"%data['serial_no'])

		# 检查车辆编号是否重复
		result = db.findByCond("code", "code = '%s' and id <> %s "%( data['code'], data['id'] ))
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "车辆编号 %s 已经存在！"% data['code'])

		data['update_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		data['update_id'] = self.objUserInfo['id']
		id = s.save(data, data['id'], table = 'pis.vehicle')

		logInfo = "更新车辆信息：名称：%s，编号：%s"%(data['name'], data['code'])

		operation_log(self.db).addLog(self.GetUserInfo(), "vehicleManage", logInfo, id)
		self.response(id)


	# 删除指定车辆
	# 参数：
	#  	id: 待删除的车辆ID，必选 
	#  
	#  返回值：重新更新后的线路车站列表，以便更新树结构
	@operator_except
	def delete(self):
		alldata = self.getRequestData()
		vid = self.getParam( alldata, "vid", 0)
		if vid == 0:
			raise BaseError(801, "必须指定待更删除的车辆ID！")

		sql  = " select v.serial_no, v.code from pis.vechicle v "
		sql += " where v.id = %s"%vid

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()
		logInfo = "删除车辆：序列号：%s, 编号：%s"%(rows[0][0], rows[0][1])

		# 删除车辆记录
		s.remove( vid, table = "pis.vechicle", key = "id", delete = True)

		# 删除车站安装的控制器信息

		# 记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "vehicleManage", logInfo, sid)

		# 重新获取线路的车站列表，以更新站点树
		# lineNode = lineManage.getStationList(lid)

		self.response(vid)


	# 获取车辆类型列表
	# 参数：
	# 无
	@operator_except
	def patch(self):

		# 返回的数据
		data = {}

		# 车辆类型列表
		sql  = "select cv.code, cv.name "
		sql += " from system.code_value cv"
		sql += " where cv.type_code='VEHICLE_TYPE' "
		sql += " order by cv.sort"

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		vehicleTypeList = {}
		vehicleTypeList['struct']	= "code, name"
		vehicleTypeList['rows'] 	= rows

		data['vehicleTypeList'] 	= vehicleTypeList

		self.response(data)

