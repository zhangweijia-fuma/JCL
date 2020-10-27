################################################################################################
#coding:utf-8

import os, sys
import datetime
import time
import shutil
import config
from config import *

# 打印调试输出信息
# 参数： msg：字符串，调试信息，可以为空""，data：具体数据列表，可以为空
# debug_level: 
# 	0: 关闭输出
# 	1: 告警或错误信息
# 	2：普通信息
# 	3：调试信息
# 	4：调试信息及数据
DEBUG_CLOSE 		= 0
DEBUG_ERROR 		= 1
DEBUG_NORMAL 		= 2
DEBUG_DEBUG 		= 3
DEBUG_DEBUG_DATA 	= 4

bSysIsInitial       = False                                             # 每次系统启动的时候会重新创建新的日志文件

def printLog( debug_level, msg, data ):
	if config.DEBUG == False:
		return

	if config.Debug_Level == DEBUG_CLOSE:
		return

	if debug_level > config.Debug_Level:
		return 

	logLine = ""
	if len(msg) == 0:
		print("\n")
	else:
		curTime = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		logLine = curTime + ' - ' + str(msg)
		print(logLine)

	if data:
		# for item in data:
		print(data)

	# if os.environ['PISMode'] == 'Station':
	# 	filename = os.environ['SRVDIR'] + '/src/service/' + config.PisConfig['StationLogFile'] + '.txt'
	# else:
	# 	filename = os.environ['SRVDIR'] + '/src/service/' + config.PisConfig['ServerLogFile'] + '.txt'

	# if not filename:
	# 	return

	# global bSysIsInitial
	# if os.path.exists(filename): 										# 日志文件是否已经存在
	# 	if bSysIsInitial == False or os.path.getsize(filename) > config.debugFileSize: 			# 如果文件大小超过最大大小，则将现有日志文件更名，同时打开新文件
	# 		curTime = datetime.datetime.now().strftime('%Y-%m-%d-%H%M%S')
	# 		if os.environ['PISMode'] == 'Station':
	# 			backfile = os.environ['SRVDIR'] + '/src/service/' + config.PisConfig['StationLogFile'] + '.' + curTime + '.txt'
	# 		else:
	# 			backfile = os.environ['SRVDIR'] + '/src/service/' + config.PisConfig['ServerLogFile'] + '.' + curTime + '.txt'

	# 		shutil.move(filename, backfile)
	# bSysIsInitial = True


	filename = os.environ['SRVDIR'] + '/src/service/JDS.log'

	logLine += '\n'
	try:
		logFile = open(filename, "a")
		logFile.write(logLine)
	finally:
		logFile.close()

def logE(msg, data = None):
	printLog(DEBUG_ERROR, msg, data)

def logW(msg, data = None):
	printLog(DEBUG_ERROR, msg, data)

def logI(msg, data = None):
	printLog(DEBUG_NORMAL, msg, data)

def logD(msg, data = None):
	printLog(DEBUG_DEBUG, msg, data)

def logDD(msg, data = None):
	printLog(DEBUG_DEBUG_DATA, msg, data)
