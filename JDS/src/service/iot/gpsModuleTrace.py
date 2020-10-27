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

from libs.utils import utils
from utils.debug import *

from xml.etree import ElementTree  
import urllib.request

from iot import iotConfig

class gpsModuleTrace(dbMysql.CURD) :
	def __init__(self,db) :
		if sys.version > '3':
			# python 3.0 +
			super().__init__(db,'iot.gps_module_trace', False) # 定义本实例需要操作的表名
		else :
			# python 2.7
			super(gpsModuleTrace,self).__init__(db,'iot.gps_module_trace', False)

class Restful(WebRequestHandler):

	# 获取定位模块位置信息
	# 参数：
	# 	op：操作类型，map：取指定模块的最新位置，trace：指定模块的轨迹
	# 	mid: 模块ID
	# 	batch：安装批次
	# 	it：安装类型
	@operator_except
	def get(self):

		op  	= self.get_argument('op',  default = 'map')
		mid  	= int(self.get_argument('mid',  default = '0'))
		batch  	= self.get_argument('batch',  default = '')
		it 		= self.get_argument('it', default = '')

		# logI("gpsModuleTrace 参数：")

		sql  = " select gmt.id, gmt.module_id, gmt.ip_addr, gmt.time, gmt.longitude, gmt.latitude, gmt.addr, gm.code, gm.name  "
		sql += " from iot.gps_module_trace gmt"
		sql += " inner join iot.gps_module gm on gm.id = gmt.module_id"

		# 在查询轨迹时，不需要取最新值
		if op == "map":
			sql += ",(select max(id) id from iot.gps_module_trace group by module_id ) b "

		sql_where = ""
		if op == "map":		
			sql_where = " where gmt.id = b.id "
			if mid > 0:
				sql_where += " and gm.id = %s"%mid

			if it != "":
				sql_where += " and gm.install_type = '%s'"%it

			if batch != "":
				sql_where += " and gm.install_batch = '%s'"%batch
		else:
		# elif op == "trace":
			# 轨迹跟踪
			sql_where = " where gm.id = %s"%mid

		sql += sql_where
		sql += " order by gmt.id DESC"
		
		# 如果是轨迹跟踪，则取最新的100条记录
		if op == "trace":
			sql += " limit %d"%iotConfig.IOT_TRACE_COUNT

		# logI("定位模块位置查询SQL", sql )

		cur = self.db.getCursor()
		cur.execute(sql)
		rows = cur.fetchall()

		dataList = rows

		# 用于测试时根据经纬度更新地址
		# s = gpsModuleTrace(self.db)
		# for row in rows:
		# 	if row[1] != 1:
		# 		continue

		# 	lng 	= row[4]
		# 	lat 	= row[5]
		# 	id 	 	= row[0]
		# 	addr 	= self.searchAddrByLngLat(lng, lat)
		# 	data 	= {
		# 		'addr' : addr,
		# 	}

		# 	# 不能直接修改元组的数据，将元组更改为list
		# 	rowItem = list(row)
		# 	rowItem[6] = addr
		# 	dataList.append(rowItem)

		# 	s.save(data, id, table = 'iot.gps_module_trace')

		# logI("定位模块位置查询Data", rows)

		moduleList = {}
		moduleList['rows'] = dataList
		moduleList['struct']  = "id, module_id, ip_addr, time, longitude, latitude, addr, code, name"

		sql  = "select count(gmt.id) from iot.gps_module_trace gmt"
		sql += " inner join iot.gps_module gm on gm.id = gmt.module_id"
		if op == "map":
			sql += ",(select max(id) id from iot.gps_module_trace group by module_id ) b "
		
		sql += sql_where

		cur.execute(sql)
		row = cur.fetchone() 
		moduleList['count'] = row[0]

		self.response( moduleList )

	# 
	# 参数：
	@operator_except
	def patch(self):

		paramData = self.getRequestData()

		self.response(0)


	def searchAddrByLngLat(self, lng, lat):

		# logI("根据经纬度获取地址...")
		url = 'http://api.map.baidu.com/geocoder/v2/?callback=renderReverse&location='+ lat + ',' + lng + '&output=xml&pois=1&ak=%s'%iotConfig.appKey

		# logI("URL: %s"%url)

		# XML格式的返回数据
		req = urllib.request.urlopen(url)

		# 将其他编码的字符串解码成unicode
		res = req.read().decode("utf-8")

		# 解析XML时直接将字符串转换为一个Element，解析树的根节点
		root = ElementTree.fromstring(res)

		# find()用于查找属于某个tag的第一个element，这里查找结构化地址
		formatted_address = root.find('result/formatted_address')
		sematic_description = root.find('result/sematic_description')
		address = formatted_address.text + sematic_description.text

		# Province 	= root.find('result/addressComponent/province')
		# city 		= root.find('result/addressComponent/city')
		# district 	= root.find('result/addressComponent/district')
		# name 		= root.find('result/pois/poi/name')
		# direction 	= root.find('result/pois/poi/direction')
		# distance 	= root.find('result/pois/poi/distance')

		# address 	= Province.text + city.text + district.text + name.text + direction.text + distance.text
		return address

