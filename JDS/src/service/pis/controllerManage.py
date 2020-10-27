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
from config import *
from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE
from libs.utils.debug import *

from .scheduler import *
from pis.controller import controller as Controller

class controllerManage(dbMysql.CURD) :
	def __init__(self,db) :
		if sys.version > '3':
			# python 3.0 +
			super().__init__(db,'pis.controller',False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(controllerManage,self).__init__(db,'pis.controller',False)


# 获取控制器信息
# 	id : 控制器id
# 	install_type: 控制器安装地点类型，
# 		0：列车
# 		1：车站
# 	lid：线路ID
def getControllerList(self, cur, id, device_type, lid ):
	if id == 0 :
		return []

	# 获取控制器
	sql  = "select lc.id, lc.serial_no, lc.code, lc.name, lc.device_type, lc.install_type, lc.install_id, lc.ip_address,"
	sql += " lc.open_time, lc.close_time, lc.sort, cv.name as device_type_name, cv2.name as install_type_name, lc.line_id, lc.status_update_time, lc.status"
	sql += " from pis.controller lc"
	sql += " inner join system.code_value cv on cv.type_code = 'CONTROLLER_DEVICE_TYPE' and cv.code = lc.device_type "
	sql += " inner join system.code_value cv2 on cv2.type_code = 'CONTROLLER_INSTALL_TYPE' and cv2.code = lc.install_type "
	
	if device_type == '0':
		sql += " where ( lc.install_type = '0' or lc.install_type = '1' ) and lc.install_id = %s"%id
	else:
		sql += " where ( lc.install_type = '2' or lc.install_type = '3' or lc.install_type = '4' ) and lc.install_id = %s"%id

	sql += " and lc.line_id = %d"%lid
	
	sql += " order by lc.code"

	cur.execute(sql)
	rows = cur.fetchall()

	controllerList = []
	nameSort = 1
	for row in rows:
		obj = {}
		obj['id']	   			= row[0]
		obj['serial_no']		= row[1]
		obj['code']	 			= row[2]
		obj['name']	 			= row[3]
		obj['device_type'] 		= row[4]
		obj['install_type'] 	= row[5]
		obj['install_id'] 		= row[6]
		obj['ip_address']		= row[7]
		obj['open_time']		= row[8]
		obj['close_time'] 		= row[9]
		obj['sort']	 			= row[10]
		obj['device_type_name'] = row[11]
		obj['install_type_name']= row[12]
		obj['line_id'] 			= row[13]
		obj['status_update_time']= row[14]
		obj['status'] 			= row[15]
		obj['show_name'] 		= row[3]
		obj['nodeType'] 		= 'controller'
		obj['children'] 		= []
		obj['chkDisabled'] 		= False
		obj['open']	 			= True
		obj['checked']  		= False
		obj['isItem']   		= False

		# 获取安装地点名称
		if obj['install_type'] == '0' or obj['install_type'] == '1':
			# 车辆控制器
			sql  = " select t.name from pis.train t"
			sql += " where t.id = %d"%obj['install_id']
		else:
			# 车站控制器
			sql  = " select st.name from pis.station st"
			sql += " where st.id = %d"%obj['install_id']

		cur.execute(sql)
		rows = cur.fetchall()
		if len(rows) > 0 :
			obj['install_name'] = rows[0][0]
		else:
			obj['install_name'] = 'Unknown'
			
		controllerList.append(obj)
		
	return controllerList


# 检查指定的IP地址是否重复
# 需要检查controller表和station表
# 返回值：存在，True，不存在：False
def isIPAddrExist(self, ip_address):

	db = self.getDB("pis.controller")
	result = db.findByCond("ip_address", "ip_address = '%s'"%ip_address )
	if len( result['rows'] ) > 0 :
		return True

	db = self.getDB("pis.station")
	result = db.findByCond("ip_address", "ip_address = '%s' "%ip_address)
	if len( result['rows'] ) > 0 :
		return True

	return False

# 删除控制器
# 参数：
# 	lid：线路ID
# 	sid：车站ID
# 	tid：列车ID
# 	cid：控制器ID
def deleteController(self, lid, sid, tid, cid):
	cur = self.db.getCursor()
	s = controllerManage(self.db)
	
	rows = []
	if cid > 0:
		# 删除单个控制器
		sql  = " select ct.name, ct.code, ct.ip_address from pis.controller ct "
		sql += " where ct.id = %s"%cid

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

	elif sid > 0 or tid > 0:
		if sid > 0:
			# 删除车站控制器
			sql = "select ct.id from pis.controller ct where ct.install_id = %d and ct.install_type >= '2' "%sid
			cur.execute(sql)
			rows = cur.fetchall()
		
		if tid > 0:
			# 删除列车控制器
			sql = "select ct.id from pis.controller ct where ct.install_id = %d and ct.install_type < '2' "%tid
			cur.execute(sql)
			rows = cur.fetchall()
				
	elif lid > 0:
		# 删除线路所有控制器
		sql = "select ct.id from pis.controller ct where ct.line_id = %d"%lid
		cur.execute(sql)
		rows = cur.fetchall()
	
	for item in rows:
		# 删除记录，将数据移动到历史表中
		MoveDeleteRecord("pis", "controller", item[0], self.GetUserID())

	return


class Restful(WebRequestHandler):

	# 获取控制器列表
	# 参数：
	# 	cid : 控制器ID
	# 	id：控制器安装站点的ID
	# 	type：站点类型，车站、列车
	# 返回：
	# 	控制器列表，带控制器状态
	@operator_except
	def get(self):

		cid = int(self.get_argument('cid',  default = "0"))

		sql  = "select ct.id, ct.code, ct.status, ct.name, ct.install_type, ct.status_update_time"
		sql += " from pis.controller ct"

		# 根据控制器ID查询
		if cid == 0:
			id = int(self.get_argument('id',  default = "0"))
			if id == 0:
				raise BaseError(801, "参数错误：缺少安装站点ID！")

			type = self.get_argument('type',  default = "station")
			if type != "station" and type != "train":
				raise BaseError(801, "参数错误：站点类型%s错误！"%type)

			if type == "station":
				# 车站控制器
				sql += " where ct.install_type >= '2' and ct.install_id = %d"%id
			else:
				sql += " where ct.install_type <= '1' and ct.install_id = %d"%id

			sql += "order by ct.install_type"
		else:
			sql += " where ct.id = %d"%cid

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		data = {}
		data['rows'] = rows
		data['struct'] = "id, code, status, name, install_type, status_update_time"

		self.response(data)

	# 创建控制器
	@operator_except
	def post(self):

		alldata 	= self.getRequestData()

		s = controllerManage(self.db)
		
		lsData = {
			'install_type' 	: 'install_type',
			'install_id' 	: 'install_id',
			'line_id' 		: 'line_id',
			'code' 			: 'code',
			'ip_address' 	: 'ip_address',
			'open_time'		: 'open_time',
			'close_time'	: 'close_time',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass
		
		# logI("控制器信息：", data)


		sql  = "select cv.name from system.code_value cv"
		sql += " where cv.type_code = 'CONTROLLER_INSTALL_TYPE' and cv.code = '%s'"%data['install_type']

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()
		if len(rows) == 0:
			raise BaseError(801, "系统错误：缺少数据字典 CONTROLLER_INSTALL_TYPE，请联系系统管理员！")

		data['name'] = rows[0][0]

		# 检查IP地址是否已经存在
		# result = db.findByCond("ip_address", "ip_address = '%s'"%data['ip_address'] )
		# if len( result['rows'] ) > 0 :
		# 	raise BaseError(801, "设备IP地址 %s 已经存在！"%data['ip_address'] )
		if isIPAddrExist(self, data['ip_address']):
			raise BaseError(801, "设备IP地址 %s 已经存在！"%data['ip_address'] )

		db = self.getDB("pis.controller")
		
		# 检查安装地点是否重复
		result = db.findByCond("install_type", "install_id =%d and install_type = '%s' and line_id = %s "%(data['install_id'], data['install_type'], data['line_id'] ) )
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "控制器安装地点重复！" )

		# 检查编号是否重复
		# 如果编号是系统自动产生则不需要检查
		result = db.findByCond("code", "code ='%s' "%data['code'] )
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "控制器编号重复！" )
	

		# 新建控制器

		# 默认为常规控制器
		data['device_type'] = '1'

		data['create_id'] 	= self.objUserInfo['id']
		data['create_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		
		# 创建控制器记录
		cid = s.save(data, table = 'pis.controller')


		# 根据安装地点设置其编号
		
		# 控制器编号格式为： X-AAAAAA-T
		# X：为T、S或其他，T:安装在列车上的控制器，S:安装在车站的控制器，A: 安装在其他地方的控制器
		# AAAAAA：为序号，列车序号或车站序号。每新编组一辆列车，其序号为一新增值，不能是以前曾经使用过的值。车站序号也是如此。并且车站、列车序号一旦产生就不可修改。
		# T：表示类别，车辆控制器值为0或1，表示车头或车车尾；车站控制器值为2，3，4，表示站厅控制器、上行控制器及下行控制器，5：其他控制器
		
		if data['install_type'] == '0' or data['install_type'] == '1' :  
			# 列车控制器
			# data['code'] = "T-%06d-%s"%( cid, data['install_type'])
			sql = "select t.sort from pis.train t where t.id = %d"%data['install_id']
		elif data['install_type'] >= '2' and data['install_type'] <= '4':
			# 车站控制器
			# data['code'] = "S-%06d-%s"%( cid, data['install_type'])
			sql  = "select sl.sort from pis.station st "
			sql += " inner join pis.station_line sl on sl.station_id = st.id and sl.line_id = %d"%data['line_id']
			sql += " where st.id = %d"%data['install_id']
		# elif data['install_type'] == '5':
		# 	# 其他控制器
		# 	data['code'] = "A-%06d-%s"%( cid, data['install_type'])
		else:
			raise BaseError(801, "控制器安装类型 %s 错误！"%data['install_type'] )

		cur.execute(sql)
		rows = cur.fetchall()
		if len(rows) == 0:
			raise BaseError(801, "参数错误：找不到安装站点！")

		# 设置控制器序号，为车站或列车序号值*10+控制器安装类型值，该值用于控制器排序
		data['sort'] = rows[0][0] * 10 + int(data['install_type'])

		s.save(data, cid, table="pis.controller")
		
		# 创建控制器成功，记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "controllerManage", 
			"创建控制器信息：名称：%s，编号：%s，地址：%s"%(data['name'], data['code'], data['ip_address']), cid)

		self.response(cid)

	# 更新控制器信息
	# 参数：
	#   id: 待更新控制器的ID，必选
	#   其他参数可选
	@operator_except
	def put(self):
		
		alldata = self.getRequestData()
		if self.getParam( alldata, "id", 0) == 0:
			raise BaseError(801, "必须指定待更新信息的控制器ID！")

		s = controllerManage(self.db)
		
		# 只能更新IP地址，开机时间，休眠时间
		lsData = {
			'id' 			: 'id',
			'ip_address' 	: 'ip_address',
			'open_time'		: 'open_time',
			'close_time'	: 'close_time',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass

		db = self.getDB("pis.controller")

		# 检查IP地址是否已经存在
		result = db.findByCond("ip_address", "ip_address = '%s' and id <> %d"%(data['ip_address'], data['id']) )
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "设备IP地址 %s 已经存在！"%data['ip_address'] )

		sql = " select ct.open_time, ct.close_time from pis.controller ct where ct.id = %d"%data['id']
		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()
		if len(rows) == 0:
			raise BaseError(801, "控制器ID %d 不存在！"%data['id'] )

		oldOpenTime  = rows[0][0]
		oldCloseTime = rows[0][1]

		# 保存数据
		data['update_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		data['update_id'] = self.objUserInfo['id']

		id = s.save(data, data['id'], table = 'pis.controller')

		operation_log(self.db).addLog(self.GetUserInfo(), "controllerManage", 
			"更新控制器信息：名称：%s，编号：%s，地址：%s"%(alldata['name'], alldata['code'], data['ip_address']), data['id'])

		# 检查开机及休眠时间是否有变化
		if data['open_time'] != oldOpenTime or data['close_time'] != oldCloseTime:
			# 发送消息给控制器，更新其时间
			self.sendUpdateControllerTimeMsg( data, oldOpenTime, oldCloseTime )

		self.response(id)

	# 在更新控制器参数后，向控制器发送更新休眠时间消息
	# 参数：
	# 	timeData：字典，含控制器ID、open_time、close_time
	# 	open_time、close_time为具体的时间
	def sendUpdateControllerTimeMsg(self, timeData, oldOpenTime, oldCloseTime ):
		
		ctrlID 		= timeData['id']
		open_time 	= timeData['open_time']
		close_time 	= timeData['close_time']

		data 	= {}
		msgData = []

		data['message_type'] = "28"

		#data['content'] = '发送更新控制器休眠时间消息' 
		data['operation_id1'] = 0
		data['operation_id2'] = 0

		data['content']  = "更新开机时间：%s，休眠时间：%s" % (open_time, close_time)

		# 新增发送记录
		data['create_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		data['create_id'] 	= self.objUserInfo['id']

		data['sender_id'] 	= self.objUserInfo['id']
		data['sender_name'] = self.objUserInfo['name']
		data['send_date'] 	= datetime.datetime.now().strftime('%Y-%m-%d')
		data['send_time'] 	= datetime.datetime.now().strftime('%H:%M:%S')

		data['send_status'] = '2' # 下发状态为待接收，待控制器发送响应消息后再更新状态

		s = controllerManage(self.db)
		id = s.save( data, table = "pis.message_list")

		# 获取控制器的编号、安装类型、IP地址
		sql  = "select ct.code, ct.install_type, ip_address from pis.controller ct where ct.id = %d"%ctrlID
		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()
		if len(rows) == 0:
			raise BaseError(801, "参数错误：控制器ID错误！")
		
		ctrlInfo = {}
		ctrlInfo['id'] 				= ctrlID
		ctrlInfo['code'] 			= rows[0][0]
		ctrlInfo['install_type'] 	= rows[0][1]
		ctrlInfo['ip_address'] 		= rows[0][2]

		controllerList = []
		controllerList.append(ctrlInfo)

		data['id'] = id
		data['data'] = msgData

		# 计算新旧时间的差值
		ss = open_time.split(":")
		newOpenTime = datetime.datetime(2000, 1, 1, int(ss[0]), int(ss[1]))

		ss = close_time.split(":")
		newCloseTime = datetime.datetime(2000, 1, 1, int(ss[0]), int(ss[1]))

		ss = oldOpenTime.split(":")
		oldOpenTime = datetime.datetime(2000, 1, 1, int(ss[0]), int(ss[1]))

		ss = oldCloseTime.split(":")
		oldCloseTime = datetime.datetime(2000, 1, 1, int(ss[0]), int(ss[1]))

		# logI("休眠时间：", int((newOpenTime - oldOpenTime).total_seconds() / 60) , int((newCloseTime - oldCloseTime).total_seconds() / 60 ) )

		data['delta'] = "%d:%d"%( int((newOpenTime - oldOpenTime).total_seconds() / 60) , int((newCloseTime - oldCloseTime).total_seconds() / 60 ) )


		#调用接口发送消息
		Controller.sendMessage(controllerList, data)

	# 删除指定控制器
	# 参数：
	#  	id: 待删除的控制器ID，必选 
	#  
	@operator_except
	def delete(self):
		alldata = self.getRequestData()
		cid = self.getParam( alldata, "cid", 0)
		if cid == 0:
			raise BaseError(801, "必须指定待更删除的控制器ID！")

		sql  = " select ct.name, ct.code, ct.ip_address from pis.controller ct "
		sql += " where ct.id = %s"%cid

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()
		logInfo = "删除控制器：名称：%s, 编号：%s，IP地址：%s"%(rows[0][0], rows[0][1], rows[0][2])

		# 删除记录，将数据移动到历史表中
		MoveDeleteRecord("pis", "controller", cid, self.GetUserID())

		# 记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "controllerManage", logInfo, cid)

		self.response(cid)


	# 获取控制器最新的监播图片或状态列表
	# 参数：
	# 	op：video或status
	# 	list： 控制器列表
	# 	无
	@operator_except
	def patch(self):

		paramData = self.getRequestData()
		op = paramData['op']
		controllerList = paramData['list']

		if len(controllerList) == 0:
			self.response(controllerList)

		# 返回的图片列表数据或状态列表
		list = []
		if op == 'video':

			# logI("控制器列表", controllerList)

			# 查询每个控制器最新的监播图片
			for item in controllerList:

				data = item
				data['file_path'] = ""
				
				# 仅在测试时使用
				data['video_url'] = "%s/0047201837.mp4"%(PisConfig["StreamServer"])
				logI("测试时使用：视频监播文件：%s"%data['video_url'])
				list.append(data)

				# 查询安装节点的名称
				if data['install_type'] <= '1':
					# 列车控制器
					sql  = "select t.name from pis.train t where t.id = %d"%data['install_id']
				else:
					sql = "select st.name from pis.station st where st.id = %d"%data['install_id'] 

				cur = self.db.getCursor()
				cur.execute(sql)
				rows = cur.fetchall()
				if len(rows) == 0:
					raise BaseError(801, "参数错误：控制器ID错误！")

				data['name'] = "%s - %s"%(rows[0][0], data['name']) 

				# 取当天控制器的监播画面，路径为: /根/screen/YYYY-mm-DD/控制器编号
				date = datetime.datetime.now().strftime('%Y-%m-%d')
				url = "/screen/%s/%s/"%( date, data['code'] )

				filePath = "%s%s"%(config.PisConfig['PISFileRoot'], url )

				# 判断路径是否存在
				if os.path.isdir(filePath) == False:
					# continue
					
					# 测试时使用
					filePath = "%s/screen/"%config.PisConfig['PISFileRoot']
					url = "/screen/"
					logI("使用监播截屏测试图片 - " + filePath)

				# 查找控制器目录下最新的文件
				
				filelist = []
				for file in os.listdir(filePath):
					url2 = "%s%s"%(url, file)
					file_path = os.path.join(filePath, file)
					if os.path.isdir(file_path):
						continue
					else:
						filelist.append( (file_path, os.path.getmtime(file_path), url2) )

				if len(filelist) > 0:
					newrestFile = filelist[0]
					for i in range(len(filelist)):
						if i < (len(filelist) - 1 ) and newrestFile[1] < filelist[ i + 1 ][1]:
							newrestFile = filelist[ i + 1 ]
						else:
							continue

					data['file_path'] = newrestFile[2]

			self.response( list )
			# logI("监播", list)
		
		# 获取控制器状态列表 
		elif op == 'status':
			for item in controllerList:

				cid = item['id']
				sql  = "select ct.status, ct.status_update_time"
				sql += " from pis.controller ct"
				sql += " where ct.id = %d"%cid
				cur = self.db.getCursor()
				cur.execute(sql)
				rows = cur.fetchall()
				if len(rows) == 0:
					raise BaseError(801, "参数错误：控制器ID错误！")

				data = {
					'id' : cid,
					'status': rows[0][0],
					'status_update_time' : rows[0][1],
				}
				list.append(data)

			self.response( list )

		else:
			raise BaseError(801, '参数错误：OP类型错误！')

		


