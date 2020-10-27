#coding:utf-8

import time, datetime
from libs.utils.utils import *
from libs.db.dbHelper import *


import config

from public.scheduler.baseScheduler import scheduler_func
from public.deviceUtils import *

@scheduler_func(hour = "1", day_of_week = "1")
def RSUWeeklyScheduler():

	now 	= datetime.datetime.now()
	days 	= config.BackupAfterDays
	btime 	= now - datetime.timedelta(days=days)

	btimeStr= btime.strftime("%Y-%m-%d %H:%M:%S")

	deleteTableData("public.device_status", btimeStr)


# 每天晚上1点定时清除文件
@scheduler_func(hour = "1", minute = "0", second = "0")
def clearRSUFile():
	pass
