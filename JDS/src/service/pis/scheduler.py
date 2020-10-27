#coding:utf-8

# 车站服务器不需要使用以下定时任务

import time, datetime
import config

from public.scheduler.baseScheduler import scheduler_func

from libs.utils.utils import *
from libs.db.dbHelper import *

def moveTableData(table, historyTable, beforeTime):
	dbh = DbHelper(auto=False)
	try :
		db = dbh.openDB(table)
		cur = dbh.getCursor()
		fields = db.getFields()
		sql = "insert into %s (%s) select %s from %s where create_time<'%s'" % (historyTable, fields, fields, table, beforeTime)
		# logI(sql)
		cur.execute(sql)
		sql = "delete from %s where create_time<'%s'" % (table, beforeTime)
		cur.execute(sql)
	except Exception as e:
		onException(e)
	dbh.closeDB()

def deleteTableData(table, beforeTime):
	dbh = DbHelper(auto=False)
	try :
		db = dbh.openDB(table)
		cur = dbh.getCursor()
		sql = "delete from %s where create_time<'%s'" % (table, beforeTime)
		cur.execute(sql)
	except Exception as e:
		onException(e)
	dbh.closeDB()

# @scheduler_func(second = "*/10")
@scheduler_func(hour = "1", day_of_week = "1")
def weeklyScheduler():

	if PisConfig["stationServer"]:
		return

	now = datetime.datetime.now()
	days = PisConfig["backupAfterDays"]
	btime = now - datetime.timedelta(days=days)

	btimeStr = btime.strftime("%Y-%m-%d %H:%M:%S")

	moveTableData("pis.message_list", "pis.message_list_history", btimeStr)
	moveTableData("pis.message_log", "pis.message_log_history", btimeStr)
	moveTableData("pis.controller_log", "pis.controller_log_history", btimeStr)
	moveTableData("pis.controller_status", "pis.controller_status_history", btimeStr)
	# deleteTableData("pis.controller_status", btimeStr)

# # @scheduler_func(minute = "*/1")
# def checkCtrlScheduler():
# 	dbh = DbHelper(auto=True)
# 	try :
# 		now = datetime.datetime.now()
# 		btime = now - datetime.timedelta(minutes=2)
# 		btimeStr = btime.strftime("%Y-%m-%d %H:%M:%S")
# 		db = dbh.openDB("pis.controller")
# 		cur = dbh.getCursor()
# 		sql = """update pis.controller set status='5',status_update_time=now() 
# 			where (status_update_time<'%s' or status_update_time is null) and (status!='5' or status is null)
# 		""" % (btimeStr)
# 		# logI(sql)
# 		result = cur.execute(sql)
# 	except Exception as e:
# 		onException(e)
# 	dbh.closeDB()


# 清理指定目录下的文件
def clearFile(filePath, fileKeepDay):
	now = datetime.datetime.now()
	delta = datetime.timedelta(days=fileKeepDay)
	checkDay = now - delta

	# logI("Delete file before %s from %s"%(checkDay.strftime('%Y-%m-%d %H:%M:%S'), filePath))
	checkTimeStamp = checkDay.timestamp()

	for file in os.listdir(filePath):
		file_path = os.path.join(filePath, file)
		if os.path.isdir(file_path):
			clearFile(file_path, fileKeepDay)
		else:
			fileTime = os.path.getmtime(file_path)
			if ( fileTime < checkTimeStamp):
				logD("删除文件：%s"%file)
				os.remove(file_path)


# 在三十天内的节目单文件中没有使用的节目文件将被删除
def deleteUnusedProgramFile():
	if PisConfig["stationServer"]:
		return

	now = datetime.datetime.now()
	delta = datetime.timedelta(days = config.PisConfig['ProgramFileKeepDay'])
	checkDay = now - delta
	validDate = checkDay.strftime('%Y-%m-%d')

	logI("delete program file not used before %s"%validDate)

	dbh = DbHelper(auto=False)
	try :
		db = dbh.openDB("pis.programe_file")
		cur = dbh.getCursor()

		sql  = "select id from pis.program_file pf "
		sql += " where pf.file_name not in ( "
		sql += " select DISTINCT(pi.file_name) from pis.program_info pi "
		sql += " INNER JOIN pis.program_list pl on pl.id = pi.list_id "
		sql += " where pl.valid_date > '%s' ) "%validDate
		# 只删除节目文件，安全垫片文件不删除
		sql += " and pf.video_type = '1' "

		# logI(sql)
		cur.execute(sql)
		rows = cur.fetchall()
		for row in rows:
			id = row[0]
			sql = "delete from pis.program_file pf where pf.id = %d"%id
			cur.execute(sql)

	except Exception as e:
		onException(e)
	dbh.closeDB()

# 每天晚上1点定时清除控制器截屏文件、日志、软件更新包文件及视频文件
@scheduler_func(hour = "1", minute = "0", second = "0")
def clearPISFile():
	if PisConfig["PIS"] == False:
		return

	if PisConfig["stationServer"]:
		return

	logI("clear all screen file, log and software update file of %d days ago..."%config.PisConfig['fileKeepDay'])

	clearFile(config.PisConfig['PISFileRoot'] + '/screen', config.PisConfig['fileKeepDay'])
	clearFile(config.PisConfig['PISFileRoot'] + '/log', config.PisConfig['fileKeepDay'])
	clearFile(config.PisConfig['PISFileRoot'] + '/update', config.PisConfig['fileKeepDay'])
	deleteUnusedProgramFile()
