#coding:utf-8

import re
import time
import datetime
import sys

import _thread

from db import dbMysql
from web.baseException import errorDic,BaseError
from web.Base  import WebRequestHandler,BaseError,operator_except

import xlrd, xlwt
import openpyxl
from public.excel import excel

from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE
from libs.utils.utils import *
from utils.debug import *

from public.deviceUtils import *

from rsu.RSUManager.RSUMsgServer import *

class rsuDeviceManage(dbMysql.CURD) :
	def __init__(self,db) :
		if sys.version > '3':
			# python 3.0 +
			super().__init__(db,'iot.gps_module', False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(rsuDeviceManage,self).__init__(db,'iot.gps_module', False)

class Restful(WebRequestHandler):


	# 根据条件查询设备信息
	def getDeviceData(self, op, projectID, statusCode, vendorID, offset, rowlimit):

		if offset >= 0 and rowlimit > 0:
			offset 		= ( offset - 1 ) * rowlimit
			sql_limit 	= " limit " + str(rowlimit) + " offset " + str(offset)
		else:
			sql_limit = ""

		sql = """
				select rl.id, rl.device_id, rl.ip_address1, rl.ip_address2, rl.longtitude, rl.latitude,
				rl.project_id, rl.install_location, rl.map_location, rl.install_date, 
				rp.project_name, cv.name, og.name, dl.device_name, dl.device_name_en, dl.device_code, dl.device_sn,
				dl.device_desc, dl.device_type_id, dl.vendor_id, dl.produce_date, dl.produce_location, dl.produce_batch,
				dl.qr_code, dl.start_used_date, dl.end_used_date, dl.device_status  
				from rsu.rsu_list rl
				inner join rsu.rsu_project rp on rp.id = rl.project_id
				inner join public.device_list dl on dl.id = rl.device_id
				inner join system.code_value cv on cv.code = dl.device_status and cv.type_code = 'DEVICE_STATUS_TYPE'
				inner join public.organization og on og.id = dl.vendor_id
		"""

		sql_where = " where rl.system_user_id = %d "%(self.GetSystemUserID())
		if projectID != 0:
			sql_where += " and rl.project_id = %d"%projectID

		if statusCode != "0":
			sql_where += " and dl.device_status = '%s'"%statusCode

		if vendorID != 0:
			sql_where += " and og.id = %d"%vendorID
	
		sql += sql_where
		sql += " order by rl.id"

		if op == "data":
			sql += sql_limit

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		rowdata = {}
		rowdata['rows'] = rows

		structs = """
			id, device_id, ip_address1, ip_address2, longtitude, latitude,
			project_id, install_location, map_location, install_date, project_name, 
			status_name, vendor_name, device_name, device_name_en, device_code, device_sn,
			device_desc, device_type_id, vendor_id, produce_date, produce_location, produce_batch,
			qr_code, start_used_date, end_used_date, device_status 
		"""
		rowdata['struct']  = structs
		
		sql  = "select count(rl.id) from rsu.rsu_list rl inner join public.device_list dl on dl.id = rl.device_id inner join public.organization og on og.id = dl.vendor_id"
		sql += sql_where

		cur.execute(sql)
		row = cur.fetchone() 
		rowdata['count'] = row[0]

		return rowdata

	# 获取RSU信息
	# 参数：
	# 	pid:项目ID
	# 	sc: 设备状态ID
	# 	vi: 生产厂家ID
	@operator_except
	def get(self):
		
		op 			= self.get_argument('op',  default = "data")
		projectID  	= int(self.get_argument('pid',  default = '0'))

		# 默认设备状态为正常
		statusCode 	= self.get_argument('sc',  default = '0')
		vendorID 	= int(self.get_argument('vid', default = '0'))

		offset 		= int(self.get_argument('o',  default = '-1'))
		rowlimit	= int(self.get_argument('r',  default = '0'))

		rowdata = self.getDeviceData(op, projectID, statusCode, vendorID, offset, rowlimit)

		if op == "excel":
			path = self.exportToExcel(rowdata)
			self.response(path)
		else:
			self.response( rowdata )


	# 输出excel文件
	def exportToExcel(self, rowdata):

		rsuList = []
		rsuList = ConvertDataToTuple(rowdata['struct'], rowdata['rows'])

		data = []
		sn = 1
		for rsu in rsuList:

			itemData = []
			itemData.append( sn )
			sn = sn + 1

			itemData.append( rsu['project_name'] )
			itemData.append( rsu['vendor_name'] )
			itemData.append( rsu['device_name'] )
			itemData.append( rsu['device_name_en'] )
			itemData.append( rsu['device_code'] )
			itemData.append( rsu['device_sn'] )
			itemData.append( rsu['device_desc'] )
			itemData.append( rsu['device_type_id'] )
			itemData.append( rsu['produce_date'] )
			itemData.append( rsu['produce_location'] )
			itemData.append( rsu['produce_batch'] )
			itemData.append( rsu['qr_code'] )
			itemData.append( rsu['start_used_date'] )
			itemData.append( rsu['end_used_date'] )
			itemData.append( rsu['device_status'] )
			itemData.append( rsu['install_location'] )
			itemData.append( rsu['install_date'] )
			itemData.append( rsu['ip_address1'] )
			itemData.append( rsu['ip_address2'] )
			itemData.append( rsu['longtitude'] )
			itemData.append( rsu['latitude'] )
			itemData.append( rsu['map_location'] )

			data.append(itemData) 

		struct  = "SN, 项目名称, 生产厂家, 设备型号, 设备型号(英文), 设备编号, 设备序列号, 设备描述, 设备类型ID, 生产日期, 生产地点, 生产批次,"
		struct += "二维码, 启用日期, 停用日期, 设备状态, 安装地点, 安装日期, IP地址1, IP地址2, 经度, 纬度, 地图地点"

		path = {}
		path = excel.CreateExcelFile(struct, data)

		logInfo = " %s 下载了RSU清单 "%self.objUserInfo['name']
		operation_log(self.db).addLog(self.GetUserInfo(), "rsuDeviceManage", logInfo, 0)

		return path

	@operator_except
	def post(self):

		alldata = self.getRequestData()

		s = rsuDeviceManage(self.db)
		
		# 分别向 device_list 和 rsu_list 插入数据
		lsData = {
			'device_name'		: 'device_name',
			'device_name_en' 	: 'device_name_en',
			'device_code' 		: 'device_code', 
			'device_sn' 		: 'device_sn',
			'device_desc' 		: 'device_desc',
			'device_type_id' 	: 'device_type_id',
			'vendor_id'			: 'vendor_id', 
			'produce_date' 		: 'produce_date',
			'produce_batch' 	: 'produce_batch',
			'produce_location' 	: 'produce_location',
			'qr_code' 			: 'qr_code',
			'start_used_date' 	: 'start_used_date',
			'end_used_date'		: 'end_used_date', 
			# 'device_status' 	: 'device_status',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass

		device_id = self.insertRSUDevice(data)

		lsData = {
			'ip_address1'	: 'ip_address1',
			'ip_address2' 	: 'ip_address2',
			'longtitude' 	: 'longtitude',
			'latitude' 		: 'latitude',
			'project_id' 	: 'project_id',
			'install_location' : 'install_location', 
			'map_location' 	: 'map_location',
			'install_date' 	: 'install_date',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass

		rsu_id = self.insertRSUData(data)

		# 创建RSU成功，记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "rsuDeviceManage", 
			"%s 创建RSU设备：名称：%s"%(self.objUserInfo['name'], alldata['device_name']), rsu_id)

		self.response(rsu_id)

	@operator_except
	def put(self):

		alldata = self.getRequestData()

		s = rsuDeviceManage(self.db)
		
		# 分别向 device_list 和 rsu_list 插入数据
		lsData = {
			'id' 				: 'device_id',
			'device_name'		: 'device_name',
			'device_name_en' 	: 'device_name_en',
			'device_code' 		: 'device_code', 
			'device_sn' 		: 'device_sn',
			'device_desc' 		: 'device_desc',
			'device_type_id' 	: 'device_type_id',
			'vendor_id'			: 'vendor_id', 
			'produce_date' 		: 'produce_date',
			'produce_batch' 	: 'produce_batch',
			'produce_location' 	: 'produce_location',
			'qr_code' 			: 'qr_code',
			'start_used_date' 	: 'start_used_date',
			'end_used_date'		: 'end_used_date', 
			'device_status' 	: 'device_status',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass

		db = self.getDB("public.device_list")

		systemUserID = self.GetSystemUserID()

		# 检查序列号是否重复
		result = db.findByCond("device_sn", "device_sn = '%s' and id <> %d and system_user_id = %d "%(data['device_sn'], data['id'], systemUserID))
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：序列号 %s 已经存在！"%data['device_sn'])

		# 检查设备编号是否重复
		result = db.findByCond("device_code", "device_code = '%s' and id <> %d and system_user_id = %d "%(data['device_code'], data['id'], systemUserID))
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：设备编号 %s 已经存在！"%data['device_code'])

		data['update_id'] 		= self.objUserInfo['id']
		data['update_time'] 	= datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')

		device_id = s.save(data, data['id'], table = 'public.device_list')

		lsData = {
			'id' 			: 'id',
			'device_id' 	: 'device_id',
			'ip_address1'	: 'ip_address1',
			'ip_address2' 	: 'ip_address2',
			'longtitude' 	: 'longtitude',
			'latitude' 		: 'latitude',
			'project_id' 	: 'project_id',
			'install_location' : 'install_location', 
			'map_location' 	: 'map_location',
			'install_date' 	: 'install_date',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass

		# 检查设备是否是否重复
		db = self.getDB("rsu.rsu_list")
		result = db.findByCond("device_id", "device_id = '%s' and id <> %d and system_user_id = %d "%(data['device_id'], data['id'], systemUserID))
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：该设备已经存在！")

		# 检查IP地址是否重复
		condition = """
			(ip_address1 = '%s' or ip_address1 = '%s' or ip_address2 = '%s' or ip_address2 = '%s' ) 
			and id <> %d and system_user_id = %d  
			"""%(data['ip_address1'], data['ip_address1'], data['ip_address1'], data['ip_address2'], data['id'], systemUserID)

		result = db.findByCond("ip_address1", condition) 
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：IP 地址 %s, %s 已经存在！"%(data['ip_address1'], data['ip_address2']))


		data['create_id'] 		= self.objUserInfo['id']
		data['create_time'] 	= datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')

		rsu_id = s.save(data, data['id'], table = 'rsu.rsu_list')

		# 更新RSU成功，记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "rsuDeviceManage", 
			"%s 更新RSU设备：名称：%s, Code: %s"%(self.objUserInfo['name'], alldata['device_name'], alldata['device_code']), rsu_id)

		self.response(rsu_id)


	@operator_except
	def delete(self):

		paramData = self.getRequestData()
		device_id = self.getParam(paramData, "device_id", 0)

		sql  = "select dl.device_code, dl.device_name from public.device_list dl where dl.id = %d"%device_id

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()
		if len(rows) == 0:
			self.response(0)

		logInfo = "%s 删除设备：%s(%s)"%(self.objUserInfo['name'], rows[0][1], rows[0][0])

		# 删除记录，将数据移动到历史表中
		MoveDeleteRecord("public", "device_list", device_id, self.GetUserID())
		rsuid = int(self.getParam(paramData, "id", 0))
		MoveDeleteRecord("rsu", "rsu_list", rsuid, self.GetUserID())

		# 更新成功，记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "rsuDeviceManage", 
			logInfo, device_id)

		self.response(device_id)

	# 获取下拉列表信息
	# 参数：

	@operator_except
	def patch(self):

		paramData = self.getRequestData()
		cur = self.db.getCursor()

		op = self.getParam(paramData, "op", "")
		if op == "":
			raise BaseError(801, "参数错误：缺少操作类型！")

		if op == "data":
			# 下拉列表数据
			batch = self.getParam( paramData, "batch", "")
			type  = self.getParam( paramData, "type", "")
			
			# 返回的数据
			data = {}

			# 合同列表
			sql  = "select rp.id, rp.project_name from rsu.rsu_project rp where rp.system_user_id = %d order by rp.project_name "%self.GetSystemUserID()
			cur = self.db.getCursor()
			cur.execute(sql)
			rows = cur.fetchall()

			projectList = {}
			projectList['struct']	= "id, name"
			projectList['rows'] 	= [(0, "选择全部", )] + rows

			data['projectList'] 	= projectList


			# 设备状态
			sql  = """
				select cv.code, cv.name from system.code_value cv 
				where cv.type_code = 'DEVICE_STATUS_TYPE' order by cv.sort
			"""

			cur.execute(sql)
			rows = cur.fetchall()

			statusList = {}
			statusList['struct']	= "code, name"
			statusList['rows'] 	= [("0", "选择全部")] + rows

			data['statusList'] 	= statusList

			# 供应商列表
			sql  = """
				select  og.id, og.name from public.vendor_list vl 
				inner join public.organization og on og.id = vl.vendor_id and og.system_user_id = vl.system_user_id
				where vl.system_user_id = %d 
			"""%self.GetSystemUserID()

			cur.execute(sql)
			rows = cur.fetchall()

			vendorList = {}
			vendorList['struct']	= "id, name"
			vendorList['rows'] 	= [(0, "选择全部")] + rows

			data['vendorList'] 	= vendorList

			# 组织列表
			sql  = """
				select og.id, og.name 
				from public.organization og
				where og.system_user_id = %d 
				order by og.name
			"""%self.GetSystemUserID()

			cur.execute(sql)
			rows = cur.fetchall()

			organizationList = {}
			organizationList['struct']	= "id, name"
			organizationList['rows'] 	= rows

			data['organizationList'] 	= organizationList

			# 设备类型列表
			sql  = """
				select dt.id, dt.name from public.device_type dt
				where dt.system_user_id = %d 
				order by dt.name
			"""%self.GetSystemUserID()

			cur.execute(sql)
			rows = cur.fetchall()

			deviceTypeList = {}
			deviceTypeList['struct']	= "id, name"
			deviceTypeList['rows'] 	= rows

			data['deviceTypeList'] 	= deviceTypeList

			self.response(data)
		
		elif op == "upload":

			# 批量上传RSU设备
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

			# 判断文件类别是 xls 还是 xlsx
			name, filetype = os.path.splitext(filename)

			if filetype == ".xls":
				msg = "暂时不支持 xls文件，请转换为xlsx文件格式"
				raise BaseError(801, msg)

			elif filetype == ".xlsx":
				wb = openpyxl.load_workbook(filename)
				sheet = wb.active
				nRows = sheet.max_row                                       # 行总数
				if nRows < 2:
				    raise BaseError(801, "文件无数据！")

				rowIndex = 2
				while rowIndex <= nRows:
					project_name = sheet["B%d"%rowIndex].value
					vendor_name  = sheet["C%d"%rowIndex].value
					rsuDevice = {
						'device_name'		: sheet["D%d"%rowIndex].value,
						'device_name_en' 	: sheet["E%d"%rowIndex].value,
						'device_code' 		: sheet["F%d"%rowIndex].value, 
						'device_sn' 		: sheet["G%d"%rowIndex].value,
						'device_desc' 		: sheet["H%d"%rowIndex].value,
						'device_type_id' 	: sheet["I%d"%rowIndex].value,
						'produce_date' 		: ("" if sheet["J%d"%rowIndex].value == None else sheet["J%d"%rowIndex].value),
						'produce_location' 	: sheet["K%d"%rowIndex].value,
						'produce_batch' 	: sheet["L%d"%rowIndex].value,
						'qr_code' 			: sheet["M%d"%rowIndex].value,
						'start_used_date' 	: ("" if sheet["N%d"%rowIndex].value == None else sheet["N%d"%rowIndex].value),
						'end_used_date'		: ("" if sheet["O%d"%rowIndex].value == None else sheet["O%d"%rowIndex].value), 
						'device_status' 	: sheet["P%d"%rowIndex].value,
					}

					sql = "select id from public.organization og where og.name = '%s'"%vendor_name
					cur.execute(sql)
					rows = cur.fetchall()
					if len(rows) == 0:
						logI("供应商查询SQL；" + sql)
						raise BaseError(801, "参数错误：无法找到供应商 %s 数据"%vendor_name)

					rsuDevice['vendor_id'] = rows[0][0]

					device_id = self.insertRSUDevice(rsuDevice)

					rsuData = {

						'install_location' 	: sheet["Q%d"%rowIndex].value,
						'install_date' 		: ("" if sheet["R%d"%rowIndex].value == None else sheet["R%d"%rowIndex].value),
						'ip_address1'		: sheet["S%d"%rowIndex].value,
						'ip_address2' 		: sheet["T%d"%rowIndex].value,
						'longtitude' 		: sheet["U%d"%rowIndex].value,
						'latitude' 			: sheet["V%d"%rowIndex].value,
						'device_id' 		: device_id,
					}

					sql = "select id from rsu.rsu_project rp where rp.project_name = '%s'"%project_name
					cur.execute(sql)
					rows = cur.fetchall()
					if len(rows) == 0:
						raise BaseError(801, "参数错误：无法找到项目 %s 数据"%project_name)

					rsuData['project_id'] = rows[0][0]
					map_location = getAddrByLngLat(rsuData['longtitude'], rsuData['latitude'])
					if map_location == None:
						raise BaseError(801, "参数错误：无法根据经纬度查询 %s 安装位置, Lng: %s, Lat: %s！"%(rsuDevice['device_code'], str(rsuData['longtitude']), str(rsuData['latitude'])))

					rsuData['map_location'] = map_location

					rsu_id = self.insertRSUData(rsuData)

					# 创建RSU成功，记录日志并退出
					operation_log(self.db).addLog(self.GetUserInfo(), "rsuDeviceManage", 
						"%s 批量创建RSU设备：编号：%s"%(self.objUserInfo['name'], rsuDevice['device_code']), rsu_id)

					rowIndex += 1

			self.response(0)

		elif op == "treeData":

			self.getDeviceTree(paramData)

		elif op == "msg":
			self.sendMessageToDevice(paramData)

	# 新增RSU设备
	def insertRSUDevice(self, data):

		s = rsuDeviceManage(self.db)
		db = self.getDB("public.device_list")

		systemUserID = self.GetSystemUserID()

		# 检查序列号是否重复
		result = db.findByCond("device_sn", "device_sn = '%s' and system_user_id = %d "%(data['device_sn'], systemUserID))
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：序列号 %s 已经存在！"%data['device_sn'])

		# 检查设备编号是否重复
		result = db.findByCond("device_code", "device_code = '%s' and system_user_id = %d "%(data['device_code'], systemUserID))
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：设备编号 %s 已经存在！"%data['device_code'])

		# 创建设备时默认为未连接状态
		data['device_status'] 	= '5'

		data['create_id'] 		= self.objUserInfo['id']
		data['create_time'] 	= datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		data['system_user_id'] 	= systemUserID

		device_id = s.save(data, table = 'public.device_list')

		return device_id

	def insertRSUData(self, data):

		s = rsuDeviceManage(self.db)
		systemUserID = self.GetSystemUserID()

		# 检查设备是否是否重复
		db = self.getDB("rsu.rsu_list")
		result = db.findByCond("device_id", "device_id = '%s' and system_user_id = %d "%(data['device_id'], systemUserID))
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：该设备已经存在！")

		# 检查IP地址是否重复
		condition = "(ip_address1 = '%s' or ip_address1 = '%s' or ip_address2 = '%s' or ip_address2 = '%s' ) and system_user_id = %d "%(data['ip_address1'],
			data['ip_address1'], data['ip_address1'], data['ip_address2'], systemUserID)
		result = db.findByCond("ip_address1", condition)
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：IP 地址 %s, %s 已经存在！"%(data['ip_address1'], data['ip_address2']))

		data['create_id'] 		= self.objUserInfo['id']
		data['create_time'] 	= datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		data['system_user_id'] 	= systemUserID

		rsu_id = s.save(data, table = 'rsu.rsu_list')

		return rsu_id

	# 获取设备树数据
	def getDeviceTree(self, paramData):

		projectID  	= int(self.getParam(paramData, 'pid', '0'))

		# 默认设备状态为正常
		statusCode 	= self.getParam(paramData, 'sc', '0')
		vendorID 	= int(self.getParam(paramData, 'vid', '0'))

		# 设备树根据项目进行分级

		# 查询项目清单
		
		sql = """
			select pl.id, pl.project_name from rsu.rsu_project pl 
			where pl.system_user_id = %d 
		"""%(self.GetSystemUserID())

		if projectID > 0 :
			sql += " and pl.id = %d"%projectID
		sql += "order by pl.project_name "

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()
		
		rootNode = {}
		rootNode['id'] 			= 0
		rootNode['name'] 		= "RSU 设备"

		rootNode['chkDisabled'] = False
		rootNode['open']	 	= True
		rootNode['checked']  	= False
		rootNode['isItem']   	= False

		rootNode['node_type'] 	= "root"

		projectNode = []

		for item in rows:
			node = {}
			node['id'] 			= item[0]
			node['name'] 		= item[1]

			node['chkDisabled'] = False
			node['open']	 	= True
			node['checked']  	= False
			node['isItem']   	= False

			node['node_type'] 	= "project"

			node['children'] 	= self.getProjectDevice(item[0], vendorID, statusCode)
			projectNode.append(node)

		rootNode['children'] 	= projectNode
		self.response(rootNode)

	# 获取指定项目的设备数据
	def getProjectDevice(self, projectID, vendorID, statusCode):

		sql = """
			select rl.device_id, dl.device_code, dl.device_status, rl.id 
			from public.device_list dl
			inner join rsu.rsu_list rl on rl.device_id = dl.id
			where rl.project_id = %d and rl.system_user_id = %d
		"""%(projectID, self.GetSystemUserID())

		if vendorID > 0:
			sql += " and dl.vendor_id = %d"%vendorID

		if statusCode != '0':
			sql += " and dl.device_status = '%s'"%statusCode

		sql += "order by dl.device_code"
		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		rootNode = []
		for item in rows:
			node = {}
			node['device_id'] 		= item[0]
			node['device_code'] 	= item[1]
			node['name'] 			= item[1]
			node['device_status'] 	= item[2]
			node['id'] 				= item[3]

			node['children'] 		= []

			# 设备未在线时不能被选中
			if node['device_status'] == '5':
				node['chkDisabled'] = True
			else:
				node['chkDisabled'] = False
			node['open']	 		= True
			node['checked']  		= False
			node['isItem']   		= False

			node['node_type'] 		= 'device'

			rootNode.append(node)

		return rootNode

	# 发送消息给设备
	def sendMessageToDevice(self, paramData):

		msgType = self.getParam( paramData, "msgType", "")
		if msgType == "":
			raise BaseError(801, "参数错误：缺少消息类别")
		
		deviceList = self.getParam( paramData, "deviceList", [])
		if len(deviceList) == 0:
			raise BaseError(801, "参数错误：无设备列表!")

		deviceList = self.getDeviceList(deviceList)

		logInfo = ""
		data = {}
		if msgType == "update":
			data['message_type'] = RSU_MSG_UpdateSoftware
			file_name = self.getParam( paramData, "file_name", "")
			if file_name == "":
				raise BaseError(801, "参数错误：无版本文件名称！")

			file_path = self.getParam( paramData, "file_path", "")
			if file_path == "":
				raise BaseError(801, "参数错误：无版本文件路径！")

			data['file_name'] 	= file_name
			data['file_path'] 	= file_path
			data['updateMode'] 	= self.getParam( paramData, "UpdateMode", 0)

			logInfo = "%s Send RSU_MSG_UpdateSoftware to device, filename: %s"%(self.objUserInfo['name'], file_name)

		elif msgType == "reboot":
			data['message_type'] = RSU_MSG_Reboot
			data['delay_second'] = self.getParam( paramData, "delay_second", 0)
			data['reboot_mode']  = self.getParam( paramData, "rebootMode", 1)
			
			if data['reboot_mode'] == 0:
				modeStr = "Reboot Application"
			else:
				modeStr = "Reboot System"
			logInfo = "%s Send RSU_MSG_Reboot to device, RebootMode: %s, DelayTime: %d"%(self.objUserInfo['name'], modeStr, data['delay_second'])

		# 操作日志
		operation_log(self.db).addLog(self.GetUserInfo(), "rsuDeviceManage", logInfo, 0)

		_thread.start_new_thread(self.SendMessage, (deviceList, data, ))
		self.response(0)

	# 获取设备列表
	def getDeviceList(self, deviceList):

		sql = """
				select rl.id, rl.device_id, rl.ip_address1, rl.ip_address2, dl.device_name, dl.device_code, 
				dl.device_status
				from rsu.rsu_list rl
				inner join public.device_list dl on dl.id = rl.device_id
		"""
		sql_where = " where rl.system_user_id = %d "%(self.GetSystemUserID())
		sql_where += " and rl.device_id in ("
		for item in deviceList:
			sql_where += "%d, "%item['device_id']

		sql_where += "0)"
		sql += sql_where

		cursor = self.db.getCursor()
		cursor.execute(sql)
		rows = cursor.fetchall()

		key  = ['id', 'device_id', 'ip_address1', 'ip_address2', 'device_name', 'device_code', 'device_status']
		rsuList = []
		for i, item in enumerate(rows):
			rsuInfo = dict(zip(key, item))
			rsuList.append(rsuInfo)

		return rsuList

	# 线程函数，在发送消息给控制器时通过线程发送，以避免因同时向多个控制器发消息时造成前台响应缓慢
	def SendMessage(self, deviceList, msgData):
		rsuMsgServer = None
		for item in SystemModuleList:
			if item['name'] == 'rsu':
				rsuMsgServer = item['server']

		if rsuMsgServer == None:	
			raise BaseError(801, "系统错误：未加载RSU模块，请检查配置文件！")

		rsuMsgServer.sendMessage(deviceList, msgData)
