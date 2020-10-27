#coding:utf-8

import re
import time
import datetime
import sys
import os

import xml.etree.ElementTree as ET

from db import dbMysql
from web.baseException import errorDic,BaseError
from web.Base  import WebRequestHandler,BaseError,operator_except

import xlrd, xlwt
from public.excel import excel

from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE
from libs.utils import debug, utils
from utils.debug import *

class formatFileList(dbMysql.CURD) :
	def __init__(self,db) :
		if sys.version > '3':
			# python 3.0 +
			super().__init__(db,'pis.format_file',False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(formatFileList,self).__init__(db,'pis.format_file',False)



class Restful(WebRequestHandler):


	# 获取节目单信息
	# 参数：
	# 	op: list or file 是下载到文件还是数据清单
	# 	aid: 申请者ID
	# 	valid_date：生效日期
	# 	apply_status: 申请状态
	# 	send_status: 发送状态
	# 	dt: dataType，list：仅版式文件清单本身

	@operator_except
	def get(self):
		
		op 			= self.get_argument('op',  default = "list")
		file_id		= int(self.get_argument('fid',  default = '0'))
		st 			= self.get_argument('st',  default = "")
		et			= self.get_argument('et',  default = "")

		dt			= self.get_argument('dt',  default = "list")

		offset 	= int(self.get_argument('o',  default = '1'))
		rowlimit= int(self.get_argument('r',  default = '20'))

		# logI("Get Format File参数：ff_id:%d, StartTime:%s, EndTime:%s"%(file_id, st, et))

		offset = ( offset - 1 ) * rowlimit
		sql_limit = " limit " + str(rowlimit) + " offset " + str(offset)


		sql_file_id = ""
		if file_id != 0:
			sql_file_id = "ff.id = %s "%(file_id)

		sql_time = ""
		if st != "" and et != "":
			sql_time = "ff.upload_date >= '%s 00:00:00' and ff.upload_date <= '%s 23:59:59'"%(st, et)

		sql  = " select ff.id, ff.create_time, ff.update_time, ff.create_id, ff.update_id, ff.code, ff.name, "
		sql += " ff.uploader_id, ff.uploader_name, ff.upload_date, ff.file_path, ff.file_name "
		if dt != "list":
			sql += " , ml.sender_id, ml.sender_name, ml.send_date, ml.send_time, ml.send_status"
		sql += " from pis.format_file ff"

		sql_inner  = ""
		sql_inner += " left join pis.message_list ml on ml.message_type = '16' "
		sql_inner += " and ( ff.id = ml.operation_id1 or ff.id = ml.operation_id2 )"

		if dt != "list":
			sql += sql_inner

		sql_where = ""
		if sql_file_id != "":
			sql_where = " where %s"%sql_file_id
	
		if sql_time != "":
			if sql_where == "":
				sql_where = " where %s"%sql_time
			else:
				sql_where += " and %s"%sql_time

		sql += sql_where
		sql += " order by ff.id DESC"

		# logI("版式文件信息", sql)
		if op == "list":
			sql += sql_limit

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		if op == "excel":
			self.exportToExcel( rows, ( True if dt == 'list' else False) )
		else:
			rowdata = {}

			# logI "版式文件信息", sql, rows)

			formatFileList = {}
			formatFileList['rows'] = rows
			formatFileList['struct'] = "id, create_time, update_time, create_id, update_id, code, name, uploader_id, uploader_name,"
			formatFileList['struct'] += "upload_date, file_path, file_name" 
			if dt != "list":
				formatFileList['struct'] += "sender_id, sender_name, send_date, send_time, send_status"

			sql  = "select count(ff.id) from pis.format_file ff"
			if dt != "list":
				sql += sql_inner

			sql += sql_where

			cur.execute(sql)
			row = cur.fetchone() 
			formatFileList['count'] = row[0]

			self.response( formatFileList )

	# 输出版式文件列表到excel文件
	def exportToExcel(self, rowdata, flag):

		if flag:
			key = ['id', 'create_time', 'update_time', 'create_id', 'update_id', 'code', 'name', 'uploader_id', 'uploader_name',
				'upload_date', 'file_path', 'file_name', 'sender_id', 'sender_name', 'send_date', 'send_time', 'send_status']
		else:
			key = ['id', 'create_time', 'update_time', 'create_id', 'update_id', 'code', 'name', 'uploader_id', 'uploader_name',
				'upload_date', 'file_path', 'file_name']

		messageList = []
		for item in rowdata:
			messageInfo = dict(zip(key, item))
			messageList.append(messageInfo)

		if flag == False:
			cur = self.db.getCursor()
			utils.getDataDict(cur, messageList, "send_status", "PROGRAM_SEND_STATUS", "send_status_name")
		
		data = []
		sn = 1
		for item in messageList:
			if 'send_status_name' not in item:
				item['send_status_name'] = "未发送"

			itemData = []
			itemData.append( sn )
			sn = sn + 1
			itemData.append( item['name'] )		
			itemData.append( item['uploader_name'] )		
			itemData.append( item['upload_date'])
			if flag == False:
				itemData.append( item['sender_name'] )
				itemData.append( item['send_date'] )
				itemData.append( item['send_time'] )
				itemData.append( item['send_status_name'] )

			data.append(itemData) 

		struct = "SN, 文件名称, 上传人姓名, 上传日期 "
		if flag == False:
			struct += ", 下发人姓名, 发送日期, 发送时间, 发送状态"

		path = {}
		path = excel.createTempFile("xlsx")
		
		# logI("exportToExcel path:", path)
		
		excel.saveExcel(path['path'], struct, data )

		logInfo = " %s 下载了版式文件清单 "%self.objUserInfo['name']
		operation_log(self.db).addLog(self.GetUserInfo(), "listManage", logInfo, 0)

		self.response(path)

	# 删除指定版式文件
	# 参数：
	#  	fileInfo: 待删除的版式文件记录，必选 
	@operator_except
	def delete(self):
		
		fileInfo = self.getRequestData()
		
		s = formatFileList(self.db)

		# logI("待删除版式文件信息：", fileInfo)

		id = self.getParam( fileInfo, "id", 0)
		if id == 0:
			raise BaseError(801, "参数错误：缺少待更删除的版式文件ID！")

		logInfo = "%s 删除版式文件：名称：%s"%(self.objUserInfo['name'], fileInfo['name'])

		# 删除记录，将数据移动到历史表中
		MoveDeleteRecord("pis", "format_file", id, self.GetUserID())

		# 记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "formatFileList", logInfo, id)
		self.response(id)


	# 获取下拉列表信息或节目单附属的节目栏、节目信息
	# 参数：
	# 	op: list，获取下拉列表
	# 	

	@operator_except
	def patch(self):

		paramData = self.getRequestData()
		op = self.getParam( paramData, "op", "")
		if op == "":
			raise BaseError(801, "参数错误：缺少操作模式参数OP")

		cur = self.db.getCursor()

		if op == "list":
			# 返回的数据
			data = {}

			# 版式文件列表
			sql  = "select ff.id, ff.name "
			sql += " from pis.format_file ff"
			sql += " order by ff.name"

			
			cur.execute(sql)
			rows = cur.fetchall()

			nameList = {}
			nameList['struct']	= "id, name"
			nameList['rows'] 	= [(0, "选择全部", )] + rows

			data['nameList'] 	= nameList

			# 发送状态列表
			sql  = "select cv.code, cv.name "
			sql += " from system.code_value cv"
			sql += " where cv.type_code = 'PROGRAM_SEND_STATUS' "
			sql += " order by cv.code"

			cur = self.db.getCursor()
			cur.execute(sql)
			rows = cur.fetchall()

			sendStatusList = {}
			sendStatusList['struct']	= "code, name"
			sendStatusList['rows'] 	= [('0', "选择全部", )] + rows

			data['sendStatusList'] 	= sendStatusList

			self.response(data)

		elif op == "content":
			# 获取版式文件内容并进行解析
			id = self.getParam( paramData, "id", 0 )
			if id == 0:
				raise BaseError(801, "参数错误：缺少版式文件ID！")
			
			sql  = "select ff.file_path from pis.format_file ff "
			sql += " where ff.id = %d"%id

			cur.execute(sql)
			rows = cur.fetchall()

			if len(rows) == 0:
				raise BaseError(801, "参数错误：版式文件ID错误，找不到数据！")

			file_path = rows[0][0]

			fileParam = self.getFileParam( file_path )
			
			self.response( fileParam )

		else:
			raise BaseError(801, "参数错误：操作类型参数OP值错误！")

	# 解析版式文件
	# 版式文件格式参见接口文档
	# 返回：版式文件各项参数
	def getFileParam(self, file_path):

		fileParam = {}
		logI("版式文件路径：", file_path)
		xmlFilePath = os.path.abspath(file_path)
		
		try:
			tree = ET.parse(xmlFilePath)
			# print ("tree type:", type(tree))
		
			# 获得根节点
			root = tree.getroot()
		except Exception as e:  #捕获除与程序退出sys.exit()相关之外的所有异常
			logI("parse %s fail!"%file_path)
			raise BaseError(801, "参数错误：版式文件ID错误，找不到数据！")

		# 背景图片
		Background = root.findall("Background")
		if len(Background) == 0:
			raise BaseError(801, "版式文件格式错误：无背景图片定义！")

		item = Background[0]
		fileParam['background'] = item.attrib['imagepath']

		# 各区域参数
		fileParam['vedio'] = self.getParamUnit(root, 'Vedio', flag = False)
		fileParam['message'] = self.getParamUnit(root, 'Message')
		fileParam['emergency'] = self.getParamUnit(root, 'Emergency')
		fileParam['date'] = self.getParamUnit(root, 'Date')
		fileParam['week_ch'] = self.getParamUnit(root, 'Week_ch')
		fileParam['week_en'] = self.getParamUnit(root, 'Week_en')
		fileParam['time'] = self.getParamUnit(root, 'Time')
		fileParam['first_Up_Station_ch'] = self.getParamUnit(root, 'First_Up_Station_ch')

		fileParam['last_Up_Station_ch'] = self.getParamUnit(root, 'Last_Up_Station_ch')
		fileParam['first_Up_Time'] = self.getParamUnit(root, 'First_Up_Time')
		fileParam['last_Up_Time'] = self.getParamUnit(root, 'Last_Up_Time')
		fileParam['first_Down_Time'] = self.getParamUnit(root, 'First_Down_Time')
		fileParam['last_Down_Time'] = self.getParamUnit(root, 'Last_Down_Time')


		#logI("版式文件参数:", fileParam)
		return fileParam

	# 获取每个单元的参数
	# 返回值：
	def getParamUnit(self, root, key, flag = True ):
		unit = {}

		if flag == None:
			flag == True

		section = root.findall(key)
		if len(section) == 0:
			raise BaseError(801, "版式文件格式错误：无%s定义！"%key)

		item = section[0]

		unit['x'] = item.attrib['x']
		unit['y'] = item.attrib['y']
		unit['w'] = item.attrib['width']
		unit['h'] = item.attrib['height']
		if flag:
			unit['fontsize'] = item.attrib['x']
			unit['color'] = item.attrib['color']
			unit['bgcolor'] = item.attrib['bgcolor']


		return unit


# 上传版式文件成功后，增加相应的节目记录
# 参数：
# 	fileData：版式文件信息
def uploadFormatFile(self, fileData):

	# logI("上传文件信息：", fileData)

	cur  = self.db.getCursor()

	# 检查文件名是否存在，如果已经存在，则不做操作，此时新的节目文件已经覆盖了就的视频文件
	sql  = "select ff.id from pis.format_file ff "
	sql += " where ff.file_name = '%s'"%(fileData['file_name'])

	cur.execute(sql)
	rows = cur.fetchall()
	if len(rows) > 0:
		return 

	fileInfo = {}
	fileInfo['name'] 			= fileData['file_name'].split(".")[0]
	fileInfo['file_name'] 		= fileData['file_name']
	fileInfo['file_path'] 		= fileData['file_new_path']
	fileInfo['uploader_id'] 	= self.objUserInfo['id']
	fileInfo['uploader_name'] 	= self.objUserInfo['name']
	fileInfo['upload_date'] 	= datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
	fileInfo['update_time'] 	= datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
	fileInfo['update_id'] 		= self.objUserInfo['id']

	s = formatFileList(self.db)
	id = s.save( fileInfo, table = 'pis.format_file')
	fileInfo['code'] = id
	s.save(fileInfo, id, table = 'pis.format_file')

	# 记录日志
	logInfo = "%s 上传了版式文件：名称：%s"%(self.objUserInfo['name'], fileInfo['file_name'])
	operation_log(self.db).addLog(self.GetUserInfo(), "formatFileList", logInfo, id)

