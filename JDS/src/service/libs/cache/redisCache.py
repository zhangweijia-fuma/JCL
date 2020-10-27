#coding:utf-8
from libs.db import dbRedis,dbMysql
#from Service import Random
import ujson,config,msgpack

TokenPrefix = 'utoken_'
IdPrefix = 'uid_'

DefaultExpireTime = config.DefaultCacheExpireTime

class Cache(object) :

	def __init__(self) :
		self.rds = dbRedis.RedisCache()
		return

	def setItem(self, key, value, expire=None) :
		if expire == None :
			expire = DefaultExpireTime
		self.rds.set_data(key, value)
		self.rds.set_expire(key, expire)

	def getItem(self, key) :
		data = self.rds.get_data(key)
		if not data or data['error'] or not data['result']:
			return None;
		r = data['result'].decode("utf-8")
		
		return r

	def setObject(self, key, obj, expire=None) :
		self.setItem(key, ujson.dumps(obj), expire)

	def getObject(self, key) :
		s = self.getItem(key)
		if not s :
			return None
		return ujson.loads(s)

	def delete(self, key) :
		self.rds.del_data(key)

	def setExpire(self, key, expire) :
		self.rds.set_expire(key, expire)

