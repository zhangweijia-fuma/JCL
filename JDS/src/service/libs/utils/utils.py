#coding:utf-8

import os, sys
import codecs, time, datetime,decimal, json, ujson,random,decimal,threading
import traceback
from socket import *

from libs.utils.debug import *

__utilsTempDir = "/run/mio/"
#__curDir = os.path.dirname(__file__)
__utilsCacheDir = os.path.join(__utilsTempDir, "cache")

try :
	os.makedirs(__utilsTempDir)
except Exception as e:
	pass

try :
	os.makedirs(__utilsCacheDir)
except Exception as e:
	pass

#print("utils.py --> init")

class DecimalAndDateTimeEncoder(json.JSONEncoder):
	def default(self, o):
		if isinstance(o, datetime.datetime):
			return o.strftime('%Y-%m-%d %H:%M:%S')
		elif isinstance(o, datetime.date):
			return o.strftime('%Y-%m-%d')
		elif isinstance(o, datetime.time):
			return o.strftime('%H:%M:%S')
		elif isinstance(o, decimal.Decimal):
			return float(o)
		else :
			return json.JSONEncoder.default(self, o)

class ThreadCallback(threading.Thread) :

	def __init__(self, callback, *data) :
		threading.Thread.__init__(self)
		self.callback = callback
		self.data = data

	def run(self):
		# logD("ThreadCallback run")
		try :
			self.callback(*self.data)
		except Exception as e:
			onException(e)

def threadCallback(callback, *data):
	ThreadCallback(callback, *data).start()

def randomStr(len) :
	return "".join(random.sample("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789",len))

def cachePath() :
	return __utilsCacheDir

def tempFile(parent = None) :
	if not parent:
		parent = __utilsTempDir
	return parent + str(time.time()) + str(random.randint(1, 100000000))

def closeFile(fp) :
	try :
		if fp:
			fp.close()
		return True
	except Exception as e:
		return False

def deleteFile(file) :
	try :
		os.remove(file)
		return True
	except Exception as e:
		return False

def deleteDir(dir) :
	try :
		for root, dirs, files in os.walk(top, topdown=False):
			for name in files:
				print ("rmDir f : " + name)
				if not os.remove(os.path.join(root, name)) :
					return False

	except Exception as e:
		return False

	os.removedirs(dir)
	return True


def firstFile(dir) :
	for parent,dirnames,filenames in os.walk(dir):
		#for dirname in  dirnames:
		for filename in filenames:
			return os.path.join(parent, filename)
	return None

class ObjEncoder(json.JSONEncoder ):
	def default(self, obj):
		try :
			return json.JSONEncoder.default(self, obj)
		except:
			return str(obj)

def toStr(o):
	#return o.__str__()
	return json.dumps(o, cls=ObjEncoder)

def bigFile(dir) :
	maxSize = 0
	maxSizeFile = None
	for parent,dirnames,filenames in os.walk(dir):
		for filename in filenames:
			filename = os.path.join(parent, filename)
			if not maxSizeFile :
				maxSizeFile = filename
				maxSize = os.path.getsize(filename)
			elif os.path.getsize(filename) > maxSize :
				maxSizeFile = filename
				maxSize = os.path.getsize(filename)
	return maxSizeFile

# def _P(tag, msg) :
# 	if type(msg) == str:
# 		print(tag + msg)
# 	else:
# 		print(tag + json.dumps(msg, cls=DecimalAndDateTimeEncoder, ensure_ascii=False))

# def D(msg) :
# 	_P("\033[32m[DEBUG]\033[0m : ", msg)

# def W(msg) :
# 	_P("\033[33m[WARN]\033[0m : ", msg)

# def E(msg) :
# 	_P("\033[31m[ERROR]\033[0m : ", msg)

def readFile(file, codes = "utf-8") :

	fp = None
	rlines = None

	try :
		fp = codecs.open(file, "r", codes)
		rlines = fp.readlines()
		#print(rlines)
	except (Exception, UnicodeDecodeError) as e:
		logE("readFile failed : ========>%s %s " % (file, codes))
		pass

	if fp:
		closeFile(fp)
	return rlines

def tryReadFile(file) :

	rlines = readFile(file, "gbk")

	if not rlines :
		rlines = readFile(file, "utf-8")

	return rlines

def exec(cmd, useTempFile = False):
	logD("exec ==>")
	logD(cmd)

	if useTempFile :
		tempfile = tempFile()
		cmd = cmd + " > " + tempfile + " 2>&1"
		p = os.popen(cmd, "r")
		closeFile(p)
		rlines = tryReadFile(tempfile)
		deleteFile(tempfile);
	else :
		p = os.popen(cmd + " 2>&1", "r")
		rlines = p.readlines()
	return rlines

def invoke(self, funcName) :
	if hasattr(self, funcName):
		logD("invoke start : " + funcName)
		func = getattr(self, funcName)
		func()
		logD("invoke end : " + funcName)
		return True
	else :
		logD("invoke not found : " + funcName)
		return False

def str2list(str):
	pass

def now():
	return datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')

def getDictAttr(obj, fields, optional) :
	r = {}
	if fields :
		for f in fields:
			if not obj.__contains__(f) or obj[f] == None:
				E("field not exist : " + f)
				return None
			r[f] = obj[f]

	if optional :
		for f in optional:
			if obj.__contains__(f) and obj[f] != None:
				r[f] = obj[f]
	return r

def setDictAttr(data, obj, fields):
	for f in fields:
		if data.__contains__(f) and data[f] != None:
			obj[f] = data[f]
	return obj

def copyDictKey(src, dst, fields):
	if isinstance(fields, str):
		fields = fields.split(",")
	for f in fields:
		f = f.strip()
		if src.__contains__(f) and src[f] != None:
			dst[f] = src[f]
	return dst

def getObjAttr(obj, fields, optional):
	data = {}

	for f in fields:
		if hasattr(obj, f):
			data[f] = getattr(obj, f)
		else:
			D(obj)
			E("getObjAttr %s not exist" % (f))
			return None

	if optional :
		for f in optional:
			if hasattr(obj, f):
				data[f] = getattr(obj, f)

	return data

def setObjAttr(data, obj, fields):
	for f in fields:
		obj.setattr(f, data[f])
	return obj

def resultToDict(result):
	return listToDict(result["rows"], result["struct"])

def listToDict(rows, struct):
	keys = struct.split(",")
	for i in range(len(keys)):
		#D(i)
		keys[i] = keys[i].strip()
	#D(keys)

	ret = []

	for i in range(len(rows)):
		obj = {}
		r = rows[i]
		for j in range(len(r)):
			obj[keys[j]] = r[j]
		#D(obj)
		ret.append(obj)

	#D(ret)
	return ret

def getSplitParam(params, deli=','):
	s = params.split(deli)
	r = "','".join(p.strip() for p in s)
	#D(r)
	return "'" + r + "'"


def combineWheres(wheres, join="and"):
	where = None
	sz = len(wheres)
	if sz :
		if sz > 1:
			joinStr = ")" + join + "("
			where = "(" + joinStr.join(wheres) + ")"
		else :
			where = wheres[0]
	return where

def hasKey(d, k):
	return d.__contains__(k)

def haskey(d, k):
	return d.__contains__(k)

def getKeys(objData, fields, optional) :
	r = {}
	if fields :
		if isinstance(fields, str):
			fields = fields.split(",")
		for f in fields:
			f = f.strip()
			if not f:
				continue
			if not objData.__contains__(f) or objData[f] == None:
				E("field not exist : " + f)
				return None
			r[f] = objData[f]

	if optional :
		if isinstance(optional, str):
			optional = optional.split(",")
		for f in optional:
			f = f.strip()
			if objData.__contains__(f) and objData[f] != None:
				r[f] = objData[f]
	return r

def dummyHttpHandler(response):
	if response.error :
		logE("handle_result error")
	else:
		logE("handle_result ok")

def httpRequest(url, method, body):
	http_client = AsyncHTTPClient()
	http_client.fetch(url, dummyHttpHandler, method=method, body=body,
		connect_timeout=5, request_timeout=5, validate_cert=False)

#length <= chars.length
def randomKey(length):
	chars = "abcdefghijkmnpqrstuvwxyzABCDEFGHJKMNPQRSTUVWXYZ123456789abcdefghijkmnpqrstuvwxyzABCDEFGHJKMNPQRSTUVWXYZ123456789"
	keys = "".join(random.sample(chars, length))
	return keys

#length <= chars.length
def randomDigitKey(length):
	chars = "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
	keys = "".join(random.sample(chars, length))
	return keys

def dbResultToDict(result):
	if not result or not haskey(result, "rows") or not haskey(result, "struct"):
		return {}
	fields = result["struct"].split(",")
	n = len(result["rows"])
	rr = []
	for row in result["rows"]:
		j = 0
		r = {}
		for f in fields:
			f = f.strip()
			if not f:
				j += 1
				continue
			r[f] = row[j]
			j += 1
		rr.append(r)
	return rr

def getDbResultValues(result, column):
	if not result or not haskey(result, "rows") or not haskey(result, "struct"):
		return None
	if not result["rows"]:
		return None
	fields = result["struct"].split(",")
	logD(result)
	i = 0
	for f in fields:
		f = f.strip()
		if f and f == column:
			return "'" + "','".join(str(v[i]) for v in result["rows"]) + "'"
		i += 1
	return None

def dumpDict(dict) :
	return ",".join( k + "=" + str(v) for k,v in dict.items())   

def formatHex(v):
	chars = "0123456789ABCDEF"
	v = int(v)
	s = ""
	while v > 0:
		s = chars[v & 0x0F] + s
		v = v >> 4
	while len(s) < 2:
		s = "0" + s
	s = "0x" + s
	return s
	
def onException(e):
	logE("onException : %s" % type(e))
	logE(str(e))
	traceback.print_exc()

def parseInt(s):
	try:
		return int(s)
	except:
		pass
	return 0

def printHex(data):
	logD(" ".join(["%02x" % d for d in data]))


# 在PIS记录输出到excel的时候使用
# cur: 数据库查询游标
# list: 列表
# field: 查询的字段，为字典代码值
# type_code：字典名称
# value: 赋值的字段
def getDataDict(cur, list, field, type_code, value):
	# 获取数据字典
	sql = "select cv.code, cv.name"
	sql += " from system.code_value cv where cv.type_code = '%s' and cv.status = '1' "%type_code

	cur.execute(sql)
	rows = cur.fetchall()

	for item in list:
		for dict in rows:
			if item[field] == dict[0]:
				item[value] = dict[1]
				break

	return
