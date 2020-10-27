#coding:utf-8
import time
# import traceback
from datetime import datetime
from apscheduler.schedulers.background import BackgroundScheduler
from apscheduler.triggers.cron import CronTrigger
from apscheduler.events import EVENT_JOB_EXECUTED
from apscheduler.events import EVENT_JOB_ERROR

import db
from libs.utils.utils import *
from libs.utils.debug import *
import config

rootApp = None

def init(app):
	rootApp = app

def getRootApp():
	return rootApp

class BaseSchedulerAgent():
	def __init__(self, app):
		logD("BaseSchedulerAgent __init__")
		self.application = app
		self._db_ = None
		self.db = None
		self.dbWraper = None

	def openDB(self, table):
		_db_ = self.application.db.clone()
		dbItem = db.dbMysql.CURD(_db_, table, False)
		self.db = dbItem
		self._db_ = _db_
		_db_.open()
		return dbItem

	def rollbackDB(self):
		if self._db_:
			try :
				self._db_.rollback()
			except:
				pass

	def closeDB(self):
		_db_ = self._db_
		self._db_ = None
		self.db = None
		if _db_:
			_db_.close()

	def getDB(self, table):
		if self.db:
			self.db.select(table)
			return self.db
		return self.openDB(table)

	def now_time(self) :
		return datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')

	@staticmethod
	def schedulerListener(event):
		# currentTime = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		if event.exception:
			logW('The job crashed')
		# else:
		# 	logW('%s The job worked'% currentTime )

schedulers = {}

def scheduler_listener(event):
	
	# currentTime = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')

	if event.exception:
		logW('The job crashed' )
	# else:
		# logW('%s The job worked'% currentTime )

def scheduler_func(second = "0", minute = "*", hour = "*", day_of_week = "*", name = "default"):
	def handler(func):
		# print(func)
		# D("add scheduler_func %s %s %s %s" % (second, minute, hour, day_of_week))
		if hasKey(schedulers, name) :
			scheduler = schedulers[name]
		else:
			scheduler = BackgroundScheduler()
			scheduler.add_listener(scheduler_listener, EVENT_JOB_EXECUTED | EVENT_JOB_ERROR)
			scheduler.start()
			schedulers[name] = scheduler
		cronTrigger = CronTrigger(second = second, minute = minute, hour = hour, day_of_week = day_of_week)
		scheduler.add_job(func, cronTrigger)
		return func
	return handler

class TestSchedulerAgent(BaseSchedulerAgent):
	def __init__(self, app):
		logD("TestSchedulerAgent __init__")
		super().__init__(app)
	def run(self):
		logD("TestSchedulerAgent run.....")

#@scheduler_func
def testScheduler(minute = "*/1"):
	TestSchedulerAgent(getRootApp()).run()

