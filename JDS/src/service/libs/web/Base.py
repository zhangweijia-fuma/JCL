#coding:utf-8
# _version_ = "0.2.0"
# 增加   returnErrorInfo 方法替换旧的 gotoErrorPage方法，新的returnErrorInfo可以返回具体的原始错误信息
# 将异常处理由 @operator_except 来统一捕获

import sys,traceback
import tornado.web
import tornado.gen
import db
import config
from libs.web.baseException import errorDic,BaseError
import decimal,time,datetime,json,ujson
import inspect
from libs.utils import user
from libs.utils.utils import *
from libs.utils.debug import *

import hashlib 

# 屏幕非法链接
class Base404Handler(tornado.web.RequestHandler):
	def head(self,*args,**kwargs):
		self.set_status(404)

	def get(self,*args,**kwargs):
		self.set_status(404)

	def post(self,*args,**kwargs):
		self.set_status(404)

	def put(self,*args,**kwargs):
		self.set_status(404)

	def delete(self,*args,**kwargs):
		self.set_status(404)

	def patch(self,*args,**kwargs):
		self.set_status(404)

class WebRequestHandler(tornado.web.RequestHandler):

	# 批量创建用户时，创建单个用户密码
	def createPassword( self, strAccount ):
		m2 = hashlib.md5()
		m2.update(strAccount)
		return m2.hexdigest()

	#历史数据的时间条件语句
	def getSqlTime(self,tb,attr="create_time"):
		today = datetime.date.today()
		year=today.year
		month=today.month
		tyear=config.HistoryMonth/12
		tmonth=config.HistoryMonth%12
		if tmonth>month:
			year=year-1
			month=12-(tmonth-month)
		elif tmonth==month:
			year=year-1
			month=12
		else:
			month=month-tmonth
		year=year-tyear

		timeStr='%d-%02d-01'%(year, month)
		sql=" and "
		if not tb:
			sql+=attr
		else:
			sql+=tb+"."+attr
		sql+=">='%s'"%(timeStr)
		#print(sql)
		return sql

	# Http Options
	def options(self,*args,**kwargs):
		self.set_header('Access-Control-Allow-Origin','*')
		self.set_header('Access-Control-Allow-Methods','GET,POST,PUT,DELETE,PATCH')
		self.set_header('Access-Control-Allow-Headers', 'app-key,app-secret,authorization,Content-type,X_Requested_With')

	def dump(self, data, desc='Object'):
		strJson=json.dumps(data)
		# print('=====> DUMP ' + desc + ' : ' + strJson)

	# Http Response
	def response(self,data=None,status=200,angluar=True,callback=None,async2=False):

		if status==200 : # 自动识别状态码
			callfun=inspect.stack()[1][3]
			if ('get' in callfun ) or ('put' in callfun and data) : status=200  # GET及PUT有返回时200
			elif 'post' in callfun								 : status =201  # POST成功时返回时 - 201
			elif ('put' in callfun or  'delete' in callfun or 'patch' in callfun) and data is None  :
				status = 204													  # DELETE,PUT,PATCH无返回时 - 204

		self.set_status(status)
		self.options()

		if data is not None :
			if angluar : self.write(")]}',\n")
			strJson=json.dumps(data,cls=DecimalAndDateTimeEncoder,ensure_ascii=False)
			if callback :
				self.set_header("Content-Type", "application/x-javascript")
				#self._write_buffer=[escape.utf8(callback),"(",strJson,")"]
				self.write(callback+"("+strJson+")")
			else :
				self.set_header("Content-Type", "application/json; charset=UTF-8")
				self.write(strJson)
		if (not async2) or (data is None) : self.finish()

	def responseFile(self, file, mineType=None):
		buf_size = 4096
		if not mineType:
			import re
			m = re.compile(".*\.([0-9a-z]+)$", re.IGNORECASE).match(file)
			if m:
				ext = m.group(1)
				if ext == "jpg":
					self.set_header("Content-Type", "image/jpg")
				elif ext == "png":
					self.set_header("Content-Type", "image/png")
				elif ext == "bmp":
					self.set_header("Content-Type", "image/bmp")
				elif ext == "pdf":
					self.set_header("Content-Type", "application/pdf")
				elif ext == "txt":
					self.set_header("Content-Type", "text/plain")

		with open(file, 'rb') as f:
			while True:
				data = f.read(buf_size)
				if not data:
					break
				self.write(data)
		self.response()

	# Http Response
	def redirect(self, url):
		self.set_header("location", url)
		self.set_status(301)
		self.finish()

	def getRequestHeader(self,header):
		return self.request.headers.get(header)

	def init(self):
		self.__init()
		#pass
	def checkUnauthAccess(self):
		#return True

		if hasattr(self, "publicHandler") :
			return self.publicHandler
		return False

	def __init(self,*args,**kwargs):
		self._db_ = None

		self.objUserInfo = None
		self._now_time_ = None
		self._now_date_ = None

		# if not hasattr(self, "useHisDb") :
		# 	self.useHisDb = False

		self.objUserInfo = self.getUserToObjct()
		self.user = self.objUserInfo

		if not self.objUserInfo:
			if self.checkUnauthAccess() :
				return
			raise BaseError(602) # 未登录


	def head(self,*args,**kwargs):
		self.__init()

	def get(self,*args,**kwargs):
		self.__init(*args,**kwargs)

	def post(self,*args,**kwargs):
		self.__init()

	def delete(self,*args,**kwargs):
		self.__init()

	def put(self,*args,**kwargs):
		self.__init()

	def patch(self,*args,**kwargs):
		self.__init()

	def now_time(self) :
		if self._now_time_ :
			return self._now_time_
		self._now_time_ = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		return self._now_time_;

	def now_date(self) :
		if self._now_date_ :
			return self._now_date_
		self._now_date_ = datetime.datetime.now().strftime('%Y-%m-%d')
		return self._now_date_;


	def gotoErrorPage(self,error_code,error_string='',help=False) :
		#	_version_=0.1.0
		#在错误处理中如果数据库连接是打开发，应回滚并关闭,回滚前须提交的数据必须提交
		try :
			if self._db_ is not None :
				self._db_.rollback()
				self.closeDB()
		except:
			pass
		eid=int(error_code)
		try:
			error=errorDic[eid]
		except:
			error=errorDic[900]

		# 如果存在自定义的错误消息者，替换原消息体
		if error_string !='' :
			error['message'] = error_string

		#info=sys.exc_info()
		#error['help_document']=info[0]+":"+info[1]
		info=traceback.print_exc()

		if help is False :
			try :
				del error['help_document']
			except :
				pass

		self.response(error,error['status'])


	def returnErrorInfo(self,e,help=False) :
		#在错误处理中如果数据库连接是打开发，应回滚并关闭,回滚前须提交的数据必须提交
		#_version_=0.2.0
		error_code=e.code
		error_string=e.message
		try :
			if self._db_ is not None :
				self._db_.rollback()
				self.closeDB()
		except:
			pass
		eid=int(error_code)
		try:
			error=errorDic[eid]
		except:
			error=errorDic[900]

		# 如果存在自定义的错误消息者，替换原消息体
		if error_string :
			error['original_message']=error_string

		if help is False :
			try :
				del error['help_document']
			except :
				pass
		else :
			traceback.print_exc()  # 将错误信息在控制台输出

		self.response(error,error['status'])

	def unexpected(self, e) :
		try :
			if self._db_ is not None :
				self._db_.rollback()
				self.closeDB()
		except:
			pass

		traceback.print_exc()  # 将错误信息在控制台输出

		self.response({
			"status" : 500,
			"message" : str(e)
		}, status=500)

	def checkAppKey(self):
		if self.request.headers.get('app-key')!=config.App_Key :
			raise BaseError(601)

	def getUserToObjct(self):
		token=self.request.headers.get('Authorization')
		if not token :
			return None
		return self._tokenToUser(token)

	def _tokenToUser(self,token):
		u=user.user()
		objUerInfo=u.get(token)
		return objUerInfo

	def getTokenToUser(self):
		token=self.request.headers.get('Authorization')
		return self._tokenToUser(token)
	'''
		if token is not None  :
			myToken=Token(config.RedisConfig)
			try :
				user=myToken.getUser(token).decode('utf-8')
			except:
				raise BaseError(602) #未登录授权的应用
		else :
			raise BaseError(602)
		return user
	'''

	def getRequestData(self):
		if hasattr(self, "objRequestBody"):
			return self.objRequestBody
		try :
			objRequestBody=ujson.loads(self.request.body.decode('utf-8'))
			#objRequestBody=json.loads(self.request.body.decode('utf-8'))
		except:
			raise BaseError(801) # 参数错误

		if objRequestBody==None :
			raise BaseError(801) # 参数错误
		else :
			self.objRequestBody = objRequestBody
			return objRequestBody


	def getRequestArguments(self):
		try :
			data=self.request.arguments
			if data=={} :
				return {}

			#objRequestBody=json.loads(self.request.arguments.decode('utf-8'))
			objData={}
			for (k,v) in  data.items():
				objData[k]=v[0].decode('utf-8')
			return objData
		except:
			raise BaseError(801) # 参数错误

	# 获取字典中的指定参数
	def getParam(self, param, key, default):
		if key in param:
			return param[key]
		else:
			return default

	def openDB(self):
		db = self.application.db.clone()
		self._db_ = db
		db.open() # open 时 autocommit=False
		return db

	def closeDB(self):
		if self._db_ is not None :
			self._db_.close()
		self._db_ = None

	def switchDB(self):
		self.db = self._db_
		if hasattr(self, "dbItem") :
			delattr(self, "dbItem")

	def resetToken(self):
		token=self.request.headers.get('Authorization')
		if token is not None  :
			try :
				myToken=Token(config.RedisConfig)
				user=myToken.getUser(token).decode('utf-8')
				myToken.saveToRedis(token,user)
				time=36000
			except:
				time=0
		else :
			time=0
		return time

	def limitArgv(self):
		all = self.get_argument('a',default=None)
		if all:
			return None
		offset   = int(self.get_argument('o',default='1'))
		rowcount = int(self.get_argument('r',default='100'))
		offset = (offset - 1) * rowcount
		#return (offset,rowcount)
		return '%s,%s' % (offset,rowcount)

	def searchCond0(self, fileds, stext) :
		s = " like '%%" + (stext.replace("'", "\\'")) + "%%' or "
		cond = s.join(fileds)
		cond += s
		return cond

	def searchCond(self, fileds, stext) :
		s0 = " like '%%" + (stext.replace("'", "\\'")) + "%%' "
		s1 = s0 + "or "
		cond = fileds.replace(",", s1)
		cond += s0
		return cond

	def getIds(self, results):
		return ",".join(str(row[0]) for row in results["rows"])

	def getDB(self, table):
		if hasattr(self, "dbItem") :
			self.dbItem.select(table)
			return self.dbItem
		self.dbItem = db.dbMysql.CURD(self.db, table, False)
		return self.dbItem

	def doGet(self, table, select = "*", where = None, order = None, limit = None,
			sfields = None, stext = None, res = True, nolimit = False, timeField = "create_time") :

		if not order :
			order = "id desc"

		db = self.getDB(table)
		cond = {
			"select"	: select,
			"order"		: order
		}

		wheres = []

		if not nolimit:
			if not limit :
				limit = self.limitArgv()
			if limit:
				cond["limit"] = limit

		if sfields :
			if not stext :
				stext = self.get_argument("s",default=None)
			if stext :
				scond = self.searchCond(sfields, stext)
				wheres.append(scond)
				#if where :
				#	where = "%s and (%s)" % (where, scond)
				#else :
				#	where = scond

		_id = self.get_argument("id",default=None)
		if not nolimit and _id :
			wheres.append("id=%s" % (_id))
			#if where :
			#	where = "id=%s and (%s)" % (_id, where)
			#else :
			#	where = "id=%s" % (_id)

		stime = self.get_argument("stime",default=None)
		if stime :
			wheres.append("%s>='%s'" % (timeField, stime))

		etime = self.get_argument("etime",default=None)
		if etime :
			wheres.append("%s<='%s'" % (timeField, etime))

		if where :
			wheres.append(where)
			where = None

		sz = len(wheres)
		if sz :
			if sz > 1:
				where = "(" + ")and(".join(wheres) + ")"
			else :
				where = wheres[0]

		if where :
			cond["where"] = where

		#D(wheres)
		#D(where)

		results = db.find(cond, isRaise=False)
		if res :
			self.response(results)
		else :
			return results

	def _getReq(self, objData, fileds, optional = None) :
		return getKeys(objData, fileds, optional);

	def getReq(self, fileds, optional = None) :
		objData = self.getRequestData()
		return getKeys(objData, fileds, optional)

	def doAdd(self, table, fileds, optional, preAdd = None, postAdd = None ) :
		data = self.getReq(fileds, optional)
		if not data :
			raise BaseError(801)
		data["create_id"] = self.user["id"]
		# if hoscode:
		# 	data["create_hoscode"] = self.user["hospital_code"]

		#pre add callback
		if not preAdd and hasattr(self, "preAdd"):
			preAdd = self.preAdd
		if preAdd and preAdd(data) == False:
			raise BaseError(801)

		db = self.getDB(table)
		D("doAdd====>" + table)
		results = db.add(data)

		#post add callback
		if not postAdd and hasattr(self, "postAdd"):
			postAdd = self.postAdd
		if postAdd and postAdd(results, data) == False:
			raise BaseError(801)

		#D(results)
		self.response(results)

	def doAddRst(self, table, fileds, optional, preAdd = None, postAdd = None) :
		data = self.getReq(fileds, optional)
		if not data :
			raise BaseError(801)
		data["create_id"] = self.user["id"]
		data["create_time"] = self.now_time()
		# data["hospital_code"] = self.user["hospital_code"]

		if preAdd and not preAdd(data) :
			raise BaseError(801)

		db = self.getDB(table)
		# D("doAdd====>" + table)
		results = db.add(data)

		if postAdd and not postAdd(results, data) :
			raise BaseError(801)

		# D(results)
		self.response(results)

	def setUpdateInfo(self, data):
		data["update_id"] = self.user["id"]
		data["update_time"] = self.now_time()

		# if data.__contains__("create_hoscode"):
		# 	del data["create_hoscode"]

		if data.__contains__("create_id"):
			del data["create_id"]

	def doUpdate(self, table, fileds, optional, cfields, preUpdate=None, postUpdate = None) :
		data = self.getReq(fileds, optional)
		if not data :
			raise BaseError(801)

		db = self.getDB(table)
		_id = data["id"]

		#pre update callback
		if not preUpdate and hasattr(self, "preUpdate"):
			preUpdate = self.preUpdate
		if preUpdate :
			obj = db.findById(cfields, _id)
			if not obj:
				raise BaseError(802)
			if False == preUpdate(obj, data) :
				raise BaseError(801)
			#select table again after check
			db.select(table)

		self.setUpdateInfo(data)

		#D("doUpdate====>" + table)
		#D(data)
		result = db.update(data, ids=_id)
		# D(result)

		#post update callback
		if not postUpdate and hasattr(self, "postUpdate"):
			postUpdate = self.postUpdate
		if postUpdate :
			if False == postUpdate(result, data):
				raise BaseError(801)

		self.response(result)

	def doDelete(self, table, fields = ["id"], cfields="id,create_id", res = True) :
		data = self.getReq(fields, None)
		if not data:
			raise BaseError(801)
		db = self.getDB(table)

		#pre delete callback
		if hasattr(self, "preDelete"):
			preDelete = self.preDelete
			results = db.findByAttr(cfields, data)
			if False == preDelete(results["rows"], data):
				raise BaseError(801)
			db.select(table)
		ret = db.deleteByAttr(data)

		#post update callback
		if hasattr(self, "postDelete"):
			postDelete = self.postDelete
			if False == postDelete(ret, data):
				raise BaseError(801)

		r = {"result" : ret}
		if res :
			self.response(r)
		return r

	def doDeleteMine(self, table, fields = ["id"], cfields="id,create_id", res = True) :
		data = self.getReq(fields, None)
		data["create_id"] = self.user["id"]
		db = self.getDB(table)
		#pre delete callback
		if hasattr(self, "preDelete"):
			preDelete = self.preDelete
			results = db.findByAttr(cfields, data)
			if False == preDelete(results["rows"], data):
				raise BaseError(801)
		ret = db.deleteByAttr(data)
		data["result"] = ret
		if res :
			self.response(data)
		return data

	def doAction(self) :
		data = self.getReq("action", None)
		if not data:
			raise BaseError(801)

		action = str(data["action"])
		if not hasattr(self, action):
			raise BaseError(801)

		actionFunc = getattr(self, action)
		if isAction(actionFunc):
			actionFunc()
		else:
			raise BaseError(801)

	def deleteFile(self, fileIds) :
		db = db.getDB("public.file")
		where = "where id in (" + fileIds + ")";
		r = db.findByCond("store_path", where)
		db.deleteByCond(where)
		for f in r["rows"]:
			deleteFile(f[0])

	def onError(self, code) :
		raise BaseError(code)

	def isAdmin(self) :
		user = self.user
		if not user:
			return False
		if user["user_name"] == "admin":
			return True
		return False

	def assertAdmin(self) :
		user = self.user
		if not user:
			self.onError(604)
		if user["user_name"] == "admin":
			return
		self.onError(604)

	def checkRole(self, role) :
		user = self.user
		if not user or not user.__contains__("roles"):
			return False

		roles = user["roles"]
		for r in roles:
			if r[1] == role:
				# D("checkRole %s ok" % (role))
				return True

		# D("checkRole %s fail" % (role))
		return False

	# 获取当前用户的信息
	def GetUserInfo(self):
		userID = self.objUserInfo['id']
		db = self.getDB("system.account")
		fields = db.getFields()
		keys = fields.split(",")

		result = db.findByCond(fields, "id=%d" %userID)
		if len(result["rows"]):
			userInfo = dict(zip(keys, result["rows"][0]))
			return userInfo

		return None

	# 获取当前用户的系统用户ID
	def GetSystemUserID(self):
		return self.objUserInfo['system_user_id']

	# 获取当前用户的ID
	def GetUserID(self):
		return self.objUserInfo['id']
		
def public_handler(func):
	def handler(self,*args, **kwargs):
		self.publicHandler = True
		func(self,*args, **kwargs)
	return handler

# def use_hisdb(func):
# 	def handler(self,*args, **kwargs):
# 		self.useHisDb = True
# 		func(self,*args, **kwargs)
# 	return handler

# action处理函数
def action(func):
	# D("----------action " + getattr(func, "__name__"))
	def gen_status(self, *args, **kwargs):
		func(self, *args, **kwargs)
	gen_status.___action = True
	return gen_status

def isAction(func):
	return hasattr(func, "___action")

# 操作异常处理
def operator_except(func):

	#得到操作状态
	def gen_status(self,*args, **kwargs):
		#result = None, None

		try:
			self.init()
			self.db = self.openDB()
			func(self, *args, **kwargs)
			self.closeDB()
		except Exception as e:
			if isinstance(e, BaseError) :
				# help=True 将显示错误信息
				self.returnErrorInfo(e,help=True)
			else :
				self.returnErrorInfo(BaseError(900),help=True)
		#return result

	return gen_status

# 操作异常处理
def operator_argumentExcept(func):
	#得到操作状态
	def gen_status(self,*args, **kwargs):
		result = None, None
		try:
			result = func(self,*args, **kwargs)
			func(self,*args, **kwargs)
		except :
			raise BaseError(801) # 参数错误
		return result
	return gen_status
