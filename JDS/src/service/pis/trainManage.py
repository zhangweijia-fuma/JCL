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

class trainManage(dbMysql.CURD) :
	def __init__(self,db) :
		if sys.version > '3':
			# python 3.0 +
			super().__init__(db,'pis.train',False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(trainManage,self).__init__(db,'pis.train',False)



class Restful(WebRequestHandler):


	# 获取地铁列车编组信息
	# 参数：
	# 	lid: 线路ID
	# 返回：列车列表
	@operator_except
	def get(self):

		lid 	= int(self.get_argument('lid', default = '0'))
		if lid == 0:
			raise BaseError(801, "参数错误：缺少线路ID！")
			
		rowdata = {}
		cur = self.db.getCursor()

		sql  = "select t.id, t.create_time, t.update_time, t.create_id, t.update_id, "
		sql += " t.name, t.name_en, t.code, t.description, t.line_id, t.sort, ol.name "
		sql += " from pis.train t "
		sql += " inner join pis.operation_line ol on ol.id = t.line_id"
		sql += " where t.line_id = %d"%lid
		sql += " order by t.code"

		cur.execute(sql)
		rows = cur.fetchall()

		rowdata['struct']  = "id, create_time, update_time, create_id, update_id, name, name_en, code, description, line_id, sort, line_name"
		rowdata['rows']	= rows

		sql  = "select count(*) from pis.train t  "

		cur.execute(sql)
		row = cur.fetchone() 
		rowdata['count'] = row[0]

		trainInfo = {}
		trainInfo['trainList'] = rowdata

		self.response( trainInfo )

	# 创建一条运营线路
	@operator_except
	def post(self):

		alldata = self.getRequestData()

		s = trainManage(self.db)
		
		lsData = {
			'name'			: 'name',
			'name_en'		: 'name_en',
			# 'code' 			: 'code', 
			'description' 	: 'description',
			'line_id' 		: 'line_id',

		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass

		# 检查列车名称是否重复
		db = self.getDB("pis.train")
		result = db.findByCond("name", "name = '%s' or name_en = '%s' "%(data['name'], data['name_en']))
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：列车名称已经存在！")

		data['create_id'] 	= self.objUserInfo['id']
		data['create_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		
		id = s.save(data, table = 'pis.train')

		# 列车的序号: XX-AAAA，其中，XX为线路编号，AAAA为ID值，暂不考虑列车总数超过10000列的情况
		cur = self.db.getCursor();
		sql = "select max(sort) from pis.train where line_id = %d"%data['line_id']
		cur.execute(sql)
		row = cur.fetchone()
		index = data['line_id'] * 10000
		if len(row) > 0:
			index = row[0] + 1
		else:
			index = index + 1

		data['sort'] = index

		# 列车编号：
		# LC-LLL-AAAA，其中，LLL为线路编号，AAAA为序号，不足位数补0。
		data['code'] = 'LC-%03d-%04d'%(data['line_id'], index - data['line_id'] * 10000)

		s.save(data, id, table = 'pis.train')

		# 创建列车成功，记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "trainManage", 
			"创建列车：名称：%s，%s，编号：%s"%(data['name'], data['name_en'], data['code']), id)

		self.response(id)

	# 更新列车信息
	# 参数：
	#   id: 待更新列车的ID，必选
	#   其他参数可选
	@operator_except
	def put(self):

		alldata = self.getRequestData()
		if self.getParam( alldata, "id", 0) == 0:
			raise BaseError(801, "参数错误：缺少待更新信息的列车ID！")

		line_name = self.getParam( alldata, "name", "")
		if line_name == "":
			raise BaseError(801, "参数错误，无列车名称！")

		s = trainManage(self.db)
		
		# 只能更新列车名称及描述
		lsData = {
			'id' 			: 'id',
			'name'			: 'name',
			'name_en'		: 'name_en',
			# 'code'			: 'code',
			'description'	: 'description',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass

		# 检查列车名称是否重复
		db = self.getDB("pis.train")
		result = db.findByCond("name", "(name = '%s' or name_en = '%s') and id <> %s "%(data['name'], data['name_en'], data['id']) )
		if len( result['rows'] ) > 0 :
			raise BaseError(801, "数据错误：列车名称重复！")

		data['update_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		data['update_id'] 	= self.objUserInfo['id']

		id = s.save(data, data['id'], table = 'pis.train')

		logInfo = "更新列车信息：名称：%s，%s，编号：%s"%(data['name'], data['name_en'], data['code'])

		operation_log(self.db).addLog(self.GetUserInfo(), "trainManage", logInfo, id)
		self.response(id)


	# 删除指定列车
	# 参数：
	#  	id: 待删除的列车ID，必选 
	@operator_except
	def delete(self):
		alldata = self.getRequestData()
		tid = self.getParam( alldata, "id", 0)
		if tid == 0:
			raise BaseError(801, "参数错误：缺少待更删除的列车ID！")

		sql  = " select t.name, t.code from pis.train t "
		sql += " where t.id = %s"%tid

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()
		logInfo = "删除列车：名称：%s, 编号：%s"%(rows[0][0], rows[0][1])

		# 删除列车的控制器
		sql = "delete from pis.controller ct where ct.install_id = %d and ct.install_type < '2' "%tid
		cur.execute(sql) 

		s = trainManage(self.db)
		
		# 删除列车记录
		s.remove( tid, table = "pis.train", key = "id", delete = True)

		# 记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "trainManage", logInfo, tid)
		self.response(tid)


	# 获取列车信息
	# 参数：
	# 	op  : train 或者是data，操作方式
	# 	train: 只获取列车列表
	# 	data: 获取指定列车的车辆站、控制器信息
	# 	tid : 列车ID 

	@operator_except
	def patch(self):

		alldata = self.getRequestData()
		op = self.getParam( alldata, "op", "")
		if op == "":
			raise BaseError(801, "必须指定操作模式！")

		if op == "train":
			# 列车列表
			 
			# 返回的数据
			data = {}

			sql  = "select t.id, t.name "
			sql += " from pis.train t"
			sql += " order by t.code"

			cur = self.db.getCursor()
			cur.execute(sql)
			rows = cur.fetchall()

			# logI("列车", sql, rows)

			trainList = {}
			trainList['struct']	= "id, name"
			trainList['rows'] 	= rows

			data['trainList'] 	= trainList

			self.response(data)

		# elif op == "data":
		# 	# 获取所有列车的车辆、控制器信息

		# 	trainList = self.getTrainList()

		# 	treeTreeNode = {}
		# 	treeTreeNode['name'] 		= "列车列表"
		# 	treeTreeNode['children'] 	= trainList
		# 	treeTreeNode['chkDisabled'] = True
		# 	treeTreeNode['open']	 	= True
		# 	treeTreeNode['checked']  	= False
		# 	treeTreeNode['isItem']   	= False
		# 	treeTreeNode['nodeType'] 	= 'root'

		# 	logI("列车：", treeTreeNode)
		# 	self.response(treeTreeNode)

		else:
			raise BaseError(801, "参数错误：指定的操作类型OP错误！")

# 获取所有列车的车辆、控制器信息
def getTrainList(self, lid):

	cur  = self.db.getCursor()

	sql  = "select t.id, t.name, t.code, t.description, t.line_id, t.sort, ol.name, t.name_en "
	sql += " from pis.train t "
	sql += " inner join pis.operation_line ol on ol.id = t.line_id"
	sql += " where t.line_id = %d"%lid
	sql += " order by t.code"
	
	cur.execute(sql)
	rows = cur.fetchall()

	nameSort = 1
	trainList = []
	for row in rows:

		obj = {}
		obj['id']	   			= row[0]
		obj['name']	 			= "%03d"%nameSort + ' - ' + row[1]
		obj['show_name'] 		= row[1]
		obj['code']	 			= row[2]
		obj['description']		= row[3]
		obj['line_id']	 		= row[4]
		obj['sort']				= row[5]
		obj['line_name']		= row[6]
		obj['name_en']			= row[7]
		
		obj['nodeType'] 		= 'train'
		obj['children'] 		= []
		obj['chkDisabled'] 		= True
		obj['open']	 			= True
		obj['checked']  		= False
		obj['isItem']   		= False

		nameSort += 1

		# 获取列车的车辆信息
		obj['children'] = controllerManage.getControllerList(self, cur, obj['id'], '0', lid)
		
		trainList.append(obj)

	return trainList

