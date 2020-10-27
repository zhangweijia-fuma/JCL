###############################################################################
# JamesSoft JDS 路由定义
#coding:utf-8

import os
import re
import web.Base
import config

###############################################################################
# System 部分
import system.login
import system.menu
import system.dataDict
import system.accountManage
import system.operationLog
import system.userGroup

from system.login import login, heartbeat, logout	   # 登录/注销
from system.menu import menuManage, menuItemManage, getMenu

from system.accountManage import account
from system.dataDict import valueList, nameList #数据字典
from system.operationLog import operationLog

from system.userGroup import userGroupManage
from system.userGroup import userGroupMenuManage
from system import systemUserManage
###############################################################################
# Public 部分

# 文件上传
import public.upload
from public.upload import callBack, avatar
import public.organizationManage
import public.areaCodeManage
import public.vendorManage
import public.deviceTypeManage
import public.componentManage
import public.propertyManage
import public.deviceStatus
from public.deviceUtils import *

###############################################################################
# 加载配置的业务
for sys in config.SystemModule:
	if sys == 'PIS':

		###############################################################################
		# PIS 部分
		import pis.lineManage
		import pis.stationManage
		import pis.controllerManage
		import pis.trainManage
		import pis.vehicleManage
		import pis.logManage
		import pis.programList
		import pis.programUnit
		import pis.programInfo
		import pis.programFile
		import pis.formatFileList
		import pis.controllerStatus
		import pis.messageManage
		import pis.messageListManage
		import pis.subLineInfo

	elif sys == 'IOT':
		###############################################################################
		# IOT 部分
		import iot.gpsModuleManage
		import iot.gpsModuleTrace

	elif sys == 'elecMap':
		import elecMap.elecMapManage

	elif sys == 'iMRO':
		pass

	elif sys == 'oa':
		import oa.oaBulletinBoardManage

	elif sys =='stock':
		import stock.stockData
		import stock.stockList
		import stock.favorityList

	elif sys == 'rsu':
		import rsu.rsuProjectManage
		import rsu.rsuDeviceManage
		
handlers = [

	#######################################################################
	# System 部分

	(r"/jds/logout", system.login.logout.Handle),
	(r"/jds/login/(.*)/(.*)", system.login.login.Handle),
	(r"/jds/login/heartbeat", system.login.heartbeat.Handle),

	# 菜单管理
	(r"/jds/menuManage", system.menu.menuManage.Restful),
	(r"/jds/menuItemManage", system.menu.menuItemManage.Restful),
	(r"/jds/getMenu", system.menu.getMenu.Restful),

	# 系统用户类型管理
	(r"/jds/systemUserManage", system.systemUserManage.Restful),

	# 数据字典
	(r"/jds/dataDictionaryType", system.dataDict.nameList.Restful),
	(r"/jds/dataDictionaryValue", system.dataDict.valueList.Restful),

	# 操作日志
	(r"/jds/operationLog", system.operationLog.operationLog.Restful),

	# 账户管理
	(r"/jds/accountManage", system.accountManage.account.Restful),

	# 用户组管理
	(r"/jds/userGroupManage", system.userGroup.userGroupManage.Restful),
	(r"/jds/userGroupMenuManage", system.userGroup.userGroupMenuManage.Restful),

	######################################################################
	# public 部分

	(r"/upload", public.upload.callBack.Restful), 						# 文件上传回调地址
	(r"/jds/organizationManage", public.organizationManage.Restful),
	(r"/jds/vendorManage", public.vendorManage.Restful),
	(r"/jds/propertyManage", public.propertyManage.Restful),
	(r"/jds/deviceTypeManage", public.deviceTypeManage.Restful),
	(r"/jds/componentManage", public.componentManage.Restful),
	(r"/jds/deviceStatusManage", public.deviceStatus.Restful),

	# 头像裁剪后端地址
	(r"/jds/avatarUpload", public.upload.avatar.Restful),

	# 区域代码管理
	(r"/jds/areaCodeManage", public.areaCodeManage.Restful),

]

import sys as SYS
# re.compile('.*\.py$', 0).match("aa.bb.py")
PyFileRe = re.compile("(.*)\.pyc*$", 0)
PyCacheRe = re.compile(".*__pycache__$", 0)

def _importAll(path, url, rets, namespace, level):
	if level <= 0 :
		return
	subPath = None
	subNs = None
	curNs = None
	for parent, dirnames, filenames in os.walk(path):
		if PyCacheRe.match(parent):
			continue
		imported = False
		for filename in filenames:
			m = PyFileRe.match(filename)
			if not m:
				continue;
			if not imported:
				imported = True
				subPath = parent.replace(path + "/", "")
				subNs = subPath.replace("/", ".")
				curNs = namespace + "." + subNs
				importStr = "import " + curNs
				exec(importStr)
			name = m.group(1)
			importStr = "from %s import %s" % (curNs, name)
			exec(importStr)
			modname = curNs + "." + name
			module = SYS.modules[modname]
			if hasattr(module, "Restful"):
				cls = getattr(module, "Restful")
				if subPath:
					u = url + "/" + subPath + "/" + name
				else:
					u = url + "/" + name
				print("Add route : %s -> %s" % (u, modname))
				rets.append((u, cls))

def importAll(dir, url, rets):
	curDir = os.path.dirname(__file__)
	path = os.path.join(curDir, dir)
	path = os.path.abspath(path)
	namespace = dir.replace("/", ".")
	_importAll(path, url, rets, namespace, 3)

def getRoute():
	for sys in config.SystemModule:
		if sys == 'PIS':
			# ######################################################################
			# PIS 部分
			handlers.append((r"/jds/lineManage", pis.lineManage.Restful))
			handlers.append((r"/jds/stationManage", pis.stationManage.Restful))
			handlers.append((r"/jds/controllerManage", pis.controllerManage.Restful))
			handlers.append((r"/jds/controllerStatus", pis.controllerStatus.Restful))
			handlers.append((r"/jds/trainManage", pis.trainManage.Restful))
			handlers.append((r"/jds/vehicleManage", pis.vehicleManage.Restful))
			handlers.append((r"/jds/logManage", pis.logManage.Restful))
			handlers.append((r"/jds/programList", pis.programList.Restful))
			handlers.append((r"/jds/programUnit", pis.programUnit.Restful))
			handlers.append((r"/jds/programInfo", pis.programInfo.Restful))
			handlers.append((r"/jds/programFile", pis.programFile.Restful))
			handlers.append((r"/jds/formatFileList", pis.formatFileList.Restful))
			handlers.append((r"/jds/messageManage", pis.messageManage.Restful))
			handlers.append((r"/jds/messageList", pis.messageListManage.Restful))
			handlers.append((r"/jds/subLineInfo", pis.subLineInfo.Restful))
			
		elif sys == "IOT":
			# ######################################################################
			# IOT 部分
			handlers.append((r"/jds/gpsModuleManage", iot.gpsModuleManage.Restful))
			handlers.append((r"/jds/gpsModuleTrace", iot.gpsModuleTrace.Restful))

		elif sys == 'elecMap':
			handlers.append((r"/jds/elecMap", elecMap.elecMapManage.Restful))

		# elif sys == 'iMRO':

		elif sys == 'oa':
			handlers.append((r"/jds/BulletinBoardManage", oa.oaBulletinBoardManage.Restful))

		elif sys == 'stock':
			handlers.append((r"/jds/stockData", stock.stockData.Restful))
			handlers.append((r"/jds/stockList", stock.stockList.Restful))

		elif sys == 'rsu':
			handlers.append((r"/jds/rsuProjectManage", rsu.rsuProjectManage.Restful))
			handlers.append((r"/jds/rsuDeviceManage", rsu.rsuDeviceManage.Restful))

	# 最后配置异常处理
	handlers.append((r".*", web.Base.Base404Handler));
	return handlers

