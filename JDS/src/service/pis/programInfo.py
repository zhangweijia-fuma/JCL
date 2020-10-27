#coding:utf-8

import re
import time
import datetime
import sys
import os
import shutil

import config
from db import dbMysql
from web.baseException import errorDic,BaseError
from web.Base  import WebRequestHandler,BaseError,operator_except

from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE
from libs.utils.debug import *


class programInfo(dbMysql.CURD) :
	def __init__(self,db) :
		if sys.version > '3':
			# python 3.0 +
			super().__init__(db,'pis.program_info',False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(programInfo,self).__init__(db,'pis.program_info',False)



# 删除节目记录，不删除节目文件
# 参数：
# 	line_id: 归属的节目单ID
# 	unit_id：归属的节目栏ID
# 	file_id: 节目记录ID，默认为0。如果为0，表示删除节目栏的所有节目
# 	alldata: 节目记录或节目栏记录参数
# 返回值：日志信息
def deleteProgramFile(self, list_id, unit_id , fileID, alldata):

	cur = self.db.getCursor()
	s = programInfo(self.db)
		
	if fileID > 0:
		# 删除单个节目记录
		logInfo = "%s 删除节目：名称：%s"%(self.objUserInfo['name'], alldata['name'])

		s.remove( fileID, table = "pis.program_info", key = "id", delete = True)

		# 更新SORT
		sql  = " select pi.id, pi.sort from pis.program_info pi "
		sql += " where pi.unit_id = %s and pi.sort > %d"%(alldata['unit_id'], alldata['sort'])
		sql += " order by pi.sort"

		cur.execute(sql)
		rows = cur.fetchall()
		for row in rows:
			data = {
				'id' : row[0],
				'sort' : row[1] - 1,
			}

			s.save(data, row[0], table="pis.program_info")

	elif unit_id > 0:

		# 删除节目栏所有节目文件
		sql = "select pl.create_time from pis.program_list pl where pl.id = %d"%list_id
		cur.execute(sql)
		rows = cur.fetchall()
		month = rows[0][0].strftime('%Y-%m')

		# 删除节目栏所有节目记录
		s.remove( unit_id, table = "pis.program_info", key = "unit_id", delete = True)

		sql = "select pu.start_time, pu.end_time from pis.program_unit pu where pu.id=%d"%unit_id
		cur.execute(sql)
		rows=cur.fetchall()
		logInfo = "%s 删除节目栏 %s - %s 所有节目"%(self.objUserInfo['name'], rows[0][0], rows[0][1])

	elif list_id > 0:
		# 删除节目单时把节目单的所有文件都需要删除
		sql = "select pl.create_time from pis.program_list pl where pl.id = %d"%list_id
		cur.execute(sql)
		rows = cur.fetchall()

		if len(rows) > 0:
			month = rows[0][0].strftime('%Y-%m')
			filePath = "%s/list/%s/%d"%(config.PisConfig['PISFileRoot'], month, list_id)
			try:
				shutil.rmtree(filePath)
			except:
				pass

			logInfo = ""

	else:
		raise BaseError(801, "参数错误：必须指定待更删除的节目ID或节目栏ID！")

	return logInfo


class Restful(WebRequestHandler):


	# 获取节目信息
	# 参数：
	# 	uid: 归属的节目栏ID
	# 返回：
	@operator_except
	def get(self):
		uid  = int(self.get_argument('uid',  default = "0"))
		cur  = self.db.getCursor()

		sql  = "select pi.id, pi.name, pi.code, pi.unit_id, pi.list_id, pi.program_type, pi.duration, "
		sql += " pi.file_name, pi.file_path, pi.program_source, pi.sort"
		sql += " from pis.program_info pi"
		sql += " where pi.unit_id = %d"%uid
		sql += " order by pi.sort"

		cur.execute(sql)
		rows = cur.fetchall()
		programInfoList = {}
		programInfoList['rows'] = rows
		programInfoList['struct']  = "id, name, code, unit_id, list_id, program_type, duration, file_name, file_path, "
		programInfoList['struct'] += "program_source, sort"

		self.response(programInfoList)

	# 更新节目信息
	# 参数：
	#   id: 待更新节目的ID，必选
	#   其他参数可选
	@operator_except
	def put(self):
		
		alldata = self.getRequestData()
		s = programInfo(self.db)
		
		lsData = {
			'id' 			: 'id',
			'name'			: 'name',
			'code' 			: 'code', 
			'list_id'		: 'list_id',
			'unit_id'		: 'unit_id',
			'program_type'	: 'program_type',
			'duration'		: 'duration',
			'file_name' 	: 'file_name',
			'file_path'		: 'file_path',
			'program_source': 'program_source',
			'sort'			: 'sort',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass

		data['update_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		data['update_id'] = self.objUserInfo['id']

		id = data['id']

		direction = int( self.getParam(alldata, 'direction', "0"))
		if direction == 0:
			# 仅仅更新信息
			id = s.save(data, data['id'], table = 'pis.program_info')

			logInfo = "%s 更新节目信息：名称：%s"%(self.objUserInfo['name'], data['name'])
		else:
			# 调整节目顺序
			if direction == -1:
				# 上升一个位置
				sql  = " select pi.id, pi.sort from pis.program_info pi "
				sql += " where pi.unit_id = %s and pi.sort = %d"%(data['unit_id'], data['sort'] - 1 )
				cur = self.db.getCursor()
				cur.execute(sql)
				rows = cur.fetchall()
				if len(rows) > 0:
					dataOld = {
						'id' : rows[0][0],
						'sort' : data['sort'],
					}

					s.save(dataOld, dataOld['id'], table="pis.program_info")
					data['sort'] = data['sort'] - 1
					s.save(data, data['id'], table="pis.program_info")
			else:
				# 下降一个位置
				sql  = " select pi.id, pi.sort from pis.program_info pi "
				sql += " where pi.unit_id = %s and pi.sort = %d"%(data['unit_id'], data['sort'] + 1 )
				cur = self.db.getCursor()
				cur.execute(sql)
				rows = cur.fetchall()
				if len(rows) > 0:
					dataOld = {
						'id' : rows[0][0],
						'sort' : data['sort'],
					}

					s.save(dataOld, dataOld['id'], table="pis.program_info")
					data['sort'] = data['sort'] + 1
					s.save(data, data['id'], table="pis.program_info")

			logInfo = "%s 更新节目信息顺序：名称：%s"%(self.objUserInfo['name'], data['name'])

		operation_log(self.db).addLog(self.GetUserInfo(), "programInfo", logInfo, id)
		self.response(id)


	# 删除指定节目
	# 参数：
	#  	id: 待删除的节目ID，必选。如果ID=0，表示删除归属节目栏的所有节目
	#   unit_id: 节目归属的节目栏ID
	#  返回值：
	@operator_except
	def delete(self):

		alldata = self.getRequestData()
		id = self.getParam( alldata, "id", 0)
		unit_id = self.getParam( alldata, "unit_id", 0)

		if id == 0 and unit_id == 0:
			raise BaseError(801, "参数错误：必须指定待更删除的节目ID！")

		if id > 0:
			# 删除单个节目记录，不删除文件
			logInfo = deleteProgramFile(self, alldata['list_id'], alldata['unit_id'], id, alldata )

		elif unit_id > 0:
			# 删除节目记录
			logInfo = deleteProgramFile(self, alldata['list_id'], alldata['unit_id'], 0, alldata )

		else:
			raise BaseError(801, "参数错误：必须指定待更删除的节目ID或节目栏ID！")

		# 记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "programInfo", logInfo, id)

		self.response(id)
