#coding:utf-8

import re
import time
import datetime
import sys
import os

from db import dbMysql
from web.baseException import errorDic,BaseError
from web.Base  import WebRequestHandler,BaseError,operator_except

import xlrd, xlwt
from public.excel import excel

from libs.utils.debug import *

from pis import stationManage

class elecMap(dbMysql.CURD) :
	def __init__(self,db) :
		if sys.version > '3':
			# python 3.0 +
			super().__init__(db,'pis.controller_status',False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(elecMap,self).__init__(db,'pis.controller_status',False)


class Restful(WebRequestHandler):

	# 获取电子地图播放文件清单
	# 文件路径： /home/controller/elecMap
	# 文件类型：.mp4
	@operator_except
	def get(self):
		
		path = "../app/resource/video"
		fileList = os.listdir( path )
		videoList = []
		for file in fileList:
			if file.find(".mp4") == -1 and file.find(".MP4"):
				continue

			videoList.append( "resource/video/" + file)

		# 获取指定线路的车站列表
		
		lid 	= int(self.get_argument('lid', default = '0'))
		if lid == 0:
			raise BaseError(801, "参数错误：缺少线路ID！")
			
		sql  = " select st.id, st.name, st.name_en, st.code, st.type, st.ip_address, st.description"
		sql += " from pis.station st"
		sql += " inner join pis.station_line sl on sl.station_id = st.id and sl.line_id =%d"%lid
		sql += " where st.type = 'N'"
		sql += " order by sl.sort"

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		rowdata = {}

		stationList = {}
		stationList['rows'] = rows
		stationList['struct']  = "id, name, name_en, code, type, ip_address, description"

		sql  = "select count(st.id) from pis.station st "
		sql += " inner join pis.station_line sl on sl.station_id = st.id and sl.line_id =%d"%lid

		cur.execute(sql)
		row = cur.fetchone() 
		stationList['count'] = row[0]

		dataList = {}
		dataList = { 'videoList': videoList, 'stationList' : stationList }

		self.response( dataList )

	# 获取当前到站信息
	@operator_except
	def patch(self):

		self.response(0)
