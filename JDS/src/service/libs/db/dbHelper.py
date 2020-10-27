# coding:utf-8
# 使用完数据库时，需要及时关闭到数据库的连接

import sys, time, threading
from utils.utils import *
import db.dbMysql, config

class DbHelper() :

	def __init__(self, auto=False, debug=None) :
		self.db 	= None
		self._db_ 	= None
		self.auto 	= auto
		self.debug 	= debug

	def __del__(self):
		self.closeDB()
		return

	def getDB(self, table):
		if not self.db:
			return self.openDB(table)

		if self.db:
			self.db.select(table)
			return self.db
		
		return None

	def openDB(self, table = ""):
		_db_ = db.dbMysql.DB(config.DbConfig)
		dbItem = db.dbMysql.CURD(_db_, table, False)
		try :
			if self.debug != None:
				_db_.setDebug(self.debug)
		
			_db_.open(self.auto)
		except:
			return None
		
		self._db_ 	= _db_
		self.db 	= dbItem
		
		return dbItem

	def rollbackDB(self):
		if self._db_:
			try :
				self._db_.rollback()
			except:
				pass

	def closeDB(self):
		if self._db_:
			try :
				self._db_.close()
			except:
				pass
		
		self._db_ 	= None
		self.db 	= None

	def getCursor(self):
		if not self._db_:
			if not self.openDB(""):
				return None
		
		return self._db_.getCursor()
