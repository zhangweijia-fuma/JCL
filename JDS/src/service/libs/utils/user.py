#coding:utf-8

from libs.db import dbRedis
# from libs.db import dbMysql
import ujson,config,msgpack
from libs.utils.debug import *

TokenPrefix = 'utoken_'
IdPrefix = 'uid_'

class user(object) :

	def __init__(self) :
		self.rds = dbRedis.RedisCache()
		self.timeout =config.UserAuthTimeout
		return

	def setKey(self, key, value) :
		self.rds.set_data(key, value)
		self.rds.set_expire(key, self.timeout)

	def getKey(self, key) :
		data = self.rds.get_data(key)
		if not data or data['error'] or not data['result']:
			return None;
		self.rds.set_expire(key, self.timeout)
		return data

	def deleteOld(self, userId) :
		if not config.UserAuthOneSession:
			return None
		data = self.rds.get_data(IdPrefix + userId)
		if not data or data['error'] or not data['result']:
			return None;
		token = data['result']
		token = token.decode()
		self.rds.del_data(TokenPrefix + token)
		self.rds.del_data(IdPrefix + userId)
		return data

	def setVerificationCode(self, token, userData) :
		if not token:
			return None
		#print('===> user set ' + token)
		data = self.rds.get_data("phone_" + token)
		#print(data)
		if not data or data['error'] or not data['result']:
			pass
		else:
			self.rds.del_data("phone_" + token)
		self.setKey("phone_" + token, userData)

	def getVerificationCode(self, token) :
		if not token:
			return None
		#print('===> user get ' + token)
		data = self.rds.get_data("phone_" + token)
		if not data or data['error'] or not data['result']:
			return None;
		return ujson.loads(data['result']);


	def set(self, token, userData) :
		if not token:
			return None
		#print('===> user set ' + token)
		userId = str(userData['id'])
		self.deleteOld(userId);
		data = ujson.dumps(userData)
		self.setKey(TokenPrefix + token, data)
		self.setKey(IdPrefix + userId, str(token));

	def get(self, token) :
		if not token:
			return None
		# print('===> user get ' + token)
		data = self.getKey(TokenPrefix + token)
		if not data or data['error'] or not data['result']:
			return None

		userData = ujson.loads(data['result']);
		self.rds.set_expire(IdPrefix + str(userData['id']), self.timeout)
		return userData

	def delete(self, token) :
		if not token:
			return None
		#print('===> user del ' + token)
		userData = self.get(token)
		if not userData:
			return None
		self.rds.del_data(TokenPrefix + token)
		self.rds.del_data(IdPrefix + str(userData['id']))

