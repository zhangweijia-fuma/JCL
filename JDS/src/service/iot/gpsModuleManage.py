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
from utils.debug import *

class gpsModuleManage(dbMysql.CURD) :
	def __init__(self,db) :
		if sys.version > '3':
			# python 3.0 +
			super().__init__(db,'iot.gps_module', False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(gpsModuleManage,self).__init__(db,'iot.gps_module', False)

class Restful(WebRequestHandler):

	# 获取定位模块信息
	# 参数：
	# 	mid: 模块ID
	# 	batch：安装批次
	# 	it：安装类型
	# 	cl：定位位置，暂不使用
	@operator_except
	def get(self):
		
		op 		= self.get_argument('op',  default = "data")
		mid  	= int(self.get_argument('mid',  default = '0'))
		batch  	= self.get_argument('batch',  default = '')
		it 		= self.get_argument('it', default = '')
		cl 		= self.get_argument('cl', default = '')

		offset 	= int(self.get_argument('o',  default = '1'))
		rowlimit= int(self.get_argument('r',  default = '20'))

		# logI("gpsModuleManage 参数：%s"%it)

		offset = ( offset - 1 ) * rowlimit
		sql_limit = " limit " + str(rowlimit) + " offset " + str(offset)

		sql  = " select gm.id, gm.name, gm.code, gm.serial_no, gm.device_type, gm.vendor, gm.mac_addr,  "
		sql += " gm.install_date, gm.install_batch, gm.install_type, gm.install_id, gm.status"
		sql += " from iot.gps_module gm"

		sql_where = ""
		if mid != 0:
			sql_where = " where gm.id = %s"%mid

		if it != "":
			if sql_where == "":
				sql_where = " where gm.install_type = '%s'"%it
			else:
				sql_where += " and gm.install_type = '%s'"%it

		if batch != "":
			if sql_where == "":
				sql_where = " where gm.install_batch = '%s'"%batch
			else:
				sql_where += " and gm.install_batch = '%s'"%batch
	
		sql += sql_where
		sql += " order by gm.id"

		if op == "data":
			sql += sql_limit

		# logI("定位模块查询SQL", sql )

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		# 获取每个模块的最新地址，不一定是当前地址
		moduleData = []
		for row in rows:
			sql  = "select gmt.id, gmt.addr from iot.gps_module_trace gmt where gmt.module_id = %d order by gmt.id DESC limit 1"%row[0]
			cur.execute(sql)
			data = cur.fetchall()

			addr = ()
			if len(data) > 0:
				addr = (data[0][1], )
			else:
				addr = ("",)
			
			moduleData.append(row + addr)

		if op == "excel":
			path = self.exportToExcel( moduleData )
			self.response(path)
		else:
			rowdata = {}

			# logI("定位模块查询Data", rows)

			moduleList = {}
			moduleList['rows'] = moduleData
			moduleList['struct']  = "id, name, code, serial_no, device_type, vendor, mac_addr, install_date, install_batch, install_type, install_id, status, addr"

			sql  = "select count(gm.id) from iot.gps_module gm"
			sql += sql_where

			cur.execute(sql)
			row = cur.fetchone() 
			moduleList['count'] = row[0]

			self.response( moduleList )


	# 输出日志到excel文件
	def exportToExcel(self, rowdata):

		key  = ['id', 'name', 'code', 'serial_no', 'device_type', 'vendor', 'mac_addr', 'install_date', 'install_batch', 'install_type', 'install_id', 'status', 'addr']
		
		moduleList = []
		for i, item in enumerate(rowdata):
			logInfo = dict(zip(key, item))
			moduleList.append(logInfo)

		cur = self.db.getCursor()
		utils.getDataDict(cur, moduleList, "device_type", "GPS_MODULE_DEVICE_TYPE", "device_type_name")
		utils.getDataDict(cur, moduleList, "install_type", "GPS_MODULE_INSTALL_TYPE", "install_type_name")
		utils.getDataDict(cur, moduleList, "status", "GPS_MODULE_STATUS", "status_name")

		data = []
		sn = 1
		for module in moduleList:

			itemData = []
			itemData.append( sn )
			sn = sn + 1
			itemData.append( module['name'] )
			itemData.append( module['code'] )
			itemData.append( module['serial_no'] )
			itemData.append( module['device_type_name'] )
			itemData.append( module['vendor'] )
			itemData.append( module['mac_addr'] )
			itemData.append( module['install_date'])
			itemData.append( module['install_batch'] )
			itemData.append( module['install_type_name'] )
			itemData.append( module['status_name'] )
			itemData.append( module['addr'] )

			data.append(itemData) 

		struct = "SN, 名称, 编号, 序列号, 设备类型, 厂家, MAC地址, 安装日期, 安装批次, 安装类型, 状态, 最新位置"

		path = {}
		path = excel.createTempFile("xls")

		excel.saveExcel(path['path'], struct, data )
		
		logInfo = " %s 下载了定位模块清单 "%self.objUserInfo['name']
		operation_log(self.db).addLog(self.GetUserInfo(), "gpsModuleManage", logInfo, 0)

		return path

	@operator_except
	def post(self):

		alldata = self.getRequestData()

		s = gpsModuleManage(self.db)
		
		lsData = {
			'name'			: 'name',
			'code' 			: 'code', 
			'serial_no' 	: 'serial_no',
			'device_type' 	: 'device_type',
			'vendor'		: 'vendor', 
			'mac_addr' 		: 'mac_addr',
			'install_date' 	: 'install_date',

			'install_batch' : 'install_batch',
			'install_type'	: 'install_type', 
			'install_id' 	: 'install_id',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass

		# LogI("定位模块信息：", data)

		db = self.getDB("iot.gps_module")

		# 检查定位模块名称是否重复
		result = db.findByCond("name", "name = '%s'"%(data['name']))
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：定位模块名称 %s 已经存在！"%data['name'])

		# 检查定位模块编号是否重复
		result = db.findByCond("code", "code = '%s'"%(data['code']))
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：定位模块编号 %s 已经存在！"%data['code'])

		# 检查定位模块MAC地址是否重复
		result = db.findByCond("mac_addr", "mac_addr = '%s'"%(data['mac_addr']))
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：定位模块MAC地址 %s 已经存在！"%data['mac_addr'])

		data['create_id'] 	= self.objUserInfo['id']
		data['create_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		
		id = s.save(data, table = 'iot.gps_module')

		# 创建定位模块成功，记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "gpsModuleManage", 
			"%s 创建定位模块：名称：%s，编号：%s，MAC地址：%s"%(self.objUserInfo['name'], data['name'], data['code'], data['mac_addr']), id)

		self.response(id)

	# 获取下拉列表信息
	# 参数：

	@operator_except
	def patch(self):

		paramData = self.getRequestData()

		op = self.getParam(paramData, "op", "")
		if op == "":
			raise BaseError(801, "参数错误：缺少操作类型！")

		if op == "data":

			batch = self.getParam( paramData, "batch", "")
			type  = self.getParam( paramData, "type", "")
			
			# 返回的数据
			data = {}

			# 定位模块编号
			sql  = "select gm.id, gm.code from iot.gps_module gm " 
			sql_where = "" 
			if batch != "":
				sql_where = " where gm.install_batch = '%s'"%batch

			if type != "":
				if sql_where == "":
					sql_where = " where gm.install_type = '%s' "%type
				else:
					sql_where += " and gm.install_type = '%s' "%type

			sql += sql_where
			sql += "order by gm.code "

			cur = self.db.getCursor()
			cur.execute(sql)
			rows = cur.fetchall()

			codeList = {}
			codeList['struct']	= "id, code"
			codeList['rows'] 	= [(0, "选择全部", )] + rows

			data['codeList'] 	= codeList

			# 安装批次
			sql  = "select distinct(gm.install_batch) from iot.gps_module gm order by gm.install_batch"

			cur.execute(sql)
			rows = cur.fetchall()

			batchList = {}
			batchList['struct']	= "batch"
			batchList['rows'] 	= [("选择全部", )] + rows

			data['batchList'] 	= batchList

			# 安装类型
			sql  = "select cv.code, cv.name, cv.sort from system.code_value cv where cv.type_code = 'GPS_MODULE_INSTALL_TYPE' order by cv.sort "

			cur.execute(sql)
			rows = cur.fetchall()

			installTypeList = {}
			installTypeList['struct']	= "code, name, sort"
			installTypeList['rows'] 	= [("", "选择全部", "" )] + rows

			data['installTypeList'] 	= installTypeList

			# 定位模块设备类型
			sql  = "select cv.code, cv.name, cv.sort from system.code_value cv where cv.type_code = 'GPS_MODULE_DEVICE_TYPE' order by cv.sort "

			cur.execute(sql)
			rows = cur.fetchall()

			deviceTypeList = {}
			deviceTypeList['struct']= "code, name, sort"
			deviceTypeList['rows'] 	= rows

			data['deviceTypeList'] 	= deviceTypeList

			# 当前位置

			cur.execute(sql)
			rows = cur.fetchall()

			currentLocationList = {}
			currentLocationList['struct']	= "id, name, code"
			currentLocationList['rows'] 	= [(0, "选择全部", "" )]

			data['currentLocationList'] 	= currentLocationList

			self.response(data)
		
		elif op == "upload":
			# 批量上传定位模块
			fid = int(self.getParam(paramData, "fid", "0"))
			if fid == 0:
				raise BaseError(801, "参数错误：缺少上传文件ID！")


			# 获取文件
			sql = "select a.store_path from public.file a where a.id = %s "%fid

			cur.execute(sql)
			rows = cur.fetchall()
			if len(rows) == 0:
				raise BaseError(801, "参数错误：文件ID错误")

			filename = rows[0][0]

			excel = xlrd.open_workbook( filename )
			table = excel.sheet_by_index(0)
			nrows = table.nrows	#行总数  
			ncols = table.ncols	#列总数
			if ncols != 2 :
				msg = "数据中只需要包含列参数，第一行为标题。"
				logE("数据中只需要包含卡号、打卡时间两列参数：" )
				raise BaseError(801, msg)

			# 文件格式待定
			
			self.response(0)
			
