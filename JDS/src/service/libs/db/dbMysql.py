#coding:utf-8
# _version_ = "Unicorn 0.2.0"
import config

if config.DbConfig['datatype']=='MYSQL' :
	import mysql.connector
	import mysql.connector.pooling
	import mysql.connector.cursor
	from mysql.connector import errorcode
elif config.DbConfig['datatype']=='ORACLE' :
	import cx_Oracle
elif config.DbConfig['datatype']=='POSTGRESQL' :
	import psycopg2
	from psycopg2.pool import ThreadedConnectionPool
else :
	import mysql.connector
	import mysql.connector.pooling
	import mysql.connector.cursor
	from mysql.connector import errorcode
	import cx_Oracle

import os,datetime
from web.Base  import BaseError
from config import DEBUG, Debug_Level

import logging,config
from utils.debug import *

TableColums = {}

# Version 0.2.0 2015-01-21
# 主要更改：
# 1. Open时 - 事物处理直接打开
# 2. Close时 - 事物处理提交，错误时回滚
# 3. 新增CURD类，完成基础CURD
# 4. SQL操作异常处理时返回了原始错误信息，方便调试

class CURD(object) :

	def __init__(self,db=None,tableName='',tb=True) :
		self.table = ('tb' if tb else '') + tableName
		self.view  = ('vw' if tb else '') + tableName
		self.db	= db

	def select(self,tableName,tb=True):
		#self.table = ('tb' if tb else '') + tableName
		#self.view  = ('vw' if tb else '') + tableName
		self.table = tableName
		return self

	#postgres only
	def getFields(self, name=None, table=None):
		global TableColums

		if not table: 
			table = self.table

		if name:
			keyName = table + "." + name
		else :
			keyName = table

		if TableColums.__contains__(keyName):
			return TableColums[keyName]

		if self.db.DataName == "POSTGRESQL":
			cond = {
				"select" : "column_name",
				"where" : "table_schema || '.' || table_name='%s'" % table
			}
			r = self.find(cond, isRaise=False, table="information_schema.columns")
			if not r or not r["rows"]:
				colums = "*"
				TableColums[keyName] = colums
				return colums

			if name:
				joinStr = "," + name + "."
			else :
				joinStr = ","

			colums = joinStr.join(i[0] for i in r["rows"])
			if name : 
				colums = name + "." + colums;
			TableColums[keyName] = colums
			return colums
		elif self.db.DataName == "ORALCE":
			return "*"
		else:
			return "*"

	def findById(self, fields, _id) :
		cond = {
			"select" : fields,
			"where" : "id=%s" % _id
		}
		r = self.find(cond, isRaise=False)

		if not r or not r["rows"] or len(r["rows"]) != 1:
			return None
		return r["rows"][0]

	def findByCond(self, fields, _cond, order = None, limit=None) :
		cond = {
			"select" : fields,
			"where" : _cond
		}
		if order:
			cond["order"] = order
		if limit:
			cond["limit"] = limit
		return self.find(cond, isRaise=False)

	def find1ById(self, fields, _id) :
		cond = {
			"select" : fields,
			"where" : "id=%s" % (_id)
		}
		r = self.find(cond, isRaise=False)
		#print(r)
		if not r or not r["rows"] or len(r["rows"]) != 1:
			return None
		return r["rows"][0]

	def find1ByCond(self, fields, _cond) :
		cond = {
			"select" : fields,
			"where" : _cond
		}
		r = self.find(cond, isRaise=False)
		#print(r)
		if not r or not r["rows"] or len(r["rows"]) != 1:
			return None
		return r["rows"][0]

	def findByAttr(self, fields, params, order = None, join = 'AND') :
		join = " " + join + " "
		where = join.join(k + '=\'' + str(v) + '\'' for k,v in params.items())
		cond = {
			"select" : fields,
			"where" : where
		}
		if order:
			cond["order"] = order
		return self.find(cond, isRaise=False)

	def deleteByAttr(self, params, join='AND'):
		return self.db.deleteByAttr(self.table, params, join)

	def deleteByCond(self, cond):
		return self.db.deleteByCond(self.table, cond)

	def find(self,conditions,table=None,isRaise=True,encode=''):
		# isRaise 出现错误是否立即抛出异常
		# 0.2.x 版本： if not table : table=self.view
		# 0.3.x 版本后，直接调用table而不是视图
		if not table : table=self.table

		#1. 查询总数
		#1.1 检查 conditions 是否包含 limit 分页条件
		lstKeys=list(conditions.keys())
		isLimit=False
		for key in lstKeys :
			if 'LIMIT'==key.upper() :
				isLimit=True
				break

		#1.2 存在分页，统计复合条件的总记录数
		if isLimit :
			try :
				where = { '':'{{'+conditions['where']+'}}'}
			except:
				where = { "{{1}}" : 1 }
			intCount=self.db.count(table,where)

			if intCount==0 and isRaise :
				raise BaseError(802) # 没有找到数据

		#2 得到复合条件的记录
		lstRowsData = self.db.getAllToList(table,conditions)  # 查询结果以List的方式返回

		#2. 错误处理
		if (not lstRowsData) and  isRaise :
			raise BaseError(802) # 未找到数据

		#strcut = delXXX(conditions['select'])

		rows = {
			#'struct': strcut,
			'struct':conditions['select'],
			'rows' : lstRowsData
		}
		if isLimit : rows['count']=intCount

		if encode.lower()=='gbk' :
			rows=self.decodeToGBK(rows)

		return rows

	def decodeToGBK(self,rowData):
		for i in range(len(rowData['rows'])):
			row=rowData['rows'][i]
			arr=[]
			for ii in range(len(row)):
				try :
					f=row[ii].decode('gbk')
				except:
					f=row[ii]
				arr.append(f)
				rowData['rows'][i] = tuple(arr)
		return rowData

	def save(self,data,ids='',table=None,key='id',_commit=False):
		if ids :
			return self.update(data,ids,table,key,_commit)
		else :
			return self.add(data, table, _commit)

	def remove(self,ids,table=None,key='id',_commit=False,delete=False) :
		rw=self.delete(ids,table,key,_commit,delete)
		if rw<0 : raise BaseError(705) # SQL执行错误
		return rw

	def add(self,data,table=None,_commit=False):
		if not table : table=self.table
		 #加入到数据库
		rid=self.db.insert(table,data,commit=_commit)
		if rid<1 : raise BaseError(703) # SQL执行错误
		return rid


	def update(self,data,ids,table=None,key='id',_commit=False):
		if not table : table=self.table
		# 根据 ids 数据更新到数据库,多个 id
		#rw=self.db.updateByPk(table,data,rid,commit=_commit)
		try :
			if isinstance(ids, str) and  (',' in ids) :
				#ids="'"+"'".join(ids)+"'"
				ids_list=ids.split(',')
				for i,v in enumerate(ids_list):
					ids_list[i]="'%s'" % v
				ids=','.join(ids_list)
			else :
				ids = "'%s'" % ids
		except :
			pass
		rw=self.db.updateByPk(table,data,"{{ in (%s)}}"%(ids),pk=key,commit=_commit)
		if rw<0 : raise BaseError(705) # SQL执行错误
		return rw

	def delete(self,ids,table=None,key='id',_commit=False,delete=True):
		# 根据 ids 删除数据库表的数据, 多个id
		if not table : table=self.table
		try :
			if isinstance(ids, str) and  (',' in ids) :
				#ids="'"+"'".join(ids)+"'"
				ids_list=ids.split(',')
				for i,v in enumerate(ids_list):
					ids_list[i]="'%s'" % v
				ids=','.join(ids_list)
			else :
				ids = "'%s'" % ids
		except :
			pass

		if delete :
			rw=self.db.deleteByPk(table,"{{ in (%s)}}"%(ids),pk=key,commit=_commit)
		else :
			if self.db.DataName=='MYSQL' :
				rw=self.db.updateByPk(table,{'isDelete':'Y','updateTime':'{{now()}}'},"{{ in (%s)}}"%(ids),pk=key,commit=_commit)
			elif self.db.DataName=='ORALCE':
				rw=self.db.updateByPk(table,{'iS_DELETE':'Y','update_date_Time':'{{sysdate}}'},"{{ in (%s)}}"%(ids),pk=key,commit=_commit)
			elif self.db.DataName=='POSTGRESQL':
				rw=self.db.updateByPk(table,{'is_Delete':'Y','update_date_Time':'{{now()}}'},"{{ in (%s)}}"%(ids),pk=key,commit=_commit)

		#rw=db.updateByPk(self.table,{'isDelete':'Y','updateTime':'{{now()}}'},id=rid)
		if rw<0 : raise BaseError(705) # SQL执行错误
		return rw


	def map_get(self,objData):
		return Null

	def map_post(self,objData):
		return Null

	def map_put(self,objData):
		return Null

	def map_patch(self,objData):
		return Null

	def map_delete(self,objData):
		return Null


class DB(object):
	connections = 0
	def __init__(self, config):
		self.debug = True if (DEBUG and Debug_Level >= 3) else False
		if isinstance(config, DB) :
			self.DataName = config.DataName
			self.pool = config.pool
			self.cnx = self.cur = None
			return
		try :
			self.DataName=config['datatype']
			del config['datatype']
		except:
			self.DataName='POSTGRESQL'
		if self.DataName == 'MYSQL' :
			try:
				self.pool = mysql.connector.pooling.MySQLConnectionPool(**config)
				self.cnx=self.cur=None
			except mysql.connector.Error as err:
				# 这里需要记录操作日志
				logging.debug(err.msg)
				self.cnx=None
				raise BaseError(701) # 与数据库连接异常
		elif self.DataName == 'POSTGRESQL' :
			try :
				self.pool = ThreadedConnectionPool(**config)
			except:
				raise BaseError(701) # 与数据库连接异常
		elif self.DataName == 'ORACLE' :
			try :
				if config['NLS_LANG'] :
					os.environ['NLS_LANG']=config['NLS_LANG']
				del config['NLS_LANG']
			except:
				pass
			try :
				self.pool = cx_Oracle.SessionPool(**config)
			except :
				raise BaseError(701) # 与数据库连接异常
		#恢复删除的配置
		config['datatype'] = self.DataName

	def setDebug(self, debug):
		self.debug = debug

	def clone(self):
		db = DB(self)
		return db

	def open(self, auto=False):
		try :

			DB.connections += 1
			# print("===================================db.open, " + str(DB.connections))

			if self.DataName=='ORACLE' :
				self.__conn = self.pool.acquire()
				self.__cursor = self.__conn.cursor()
			elif self.DataName=='POSTGRESQL' :
				self.__conn = self.pool.getconn()
				self.__cursor = self.__conn.cursor()
			else :  # 默认为Mysql
				self.__conn   = self.pool.get_connection()
				self.__cursor = self.__conn.cursor(buffered=True)

			#self.__conn.autocommit=True
			self.__conn.autocommit=auto
			self.cnx=self.__conn
			self.cur=self.__cursor
		except :
			raise BaseError(702) # 无法获得连接池

	def close(self):

		DB.connections -= 1
		# print("===================================db.close, " + str(DB.connections))

		#关闭游标和数据库连接
		self.__conn.commit()
		if self.__cursor is not None:
			self.__cursor.close()

		if self.DataName == 'POSTGRESQL' :
			self.pool.putconn(self.__conn)	#将数据库连接放回连接池中
		else :
			self.__conn.close()

		# print("===================================db.close end ")

	def begin(self):
		self.__conn.autocommit=False

	def commit(self):
		self.__conn.commit()

	def rollback(self):
		self.__conn.rollback()

#---------------------------------------------------------------------------

	def findBySql(self,sql,params = {},limit = 0,join = 'AND',lock=False):
		"""
			自定义sql语句查找
			limit = 是否需要返回多少行
			params = dict(field=value)
			join = 'AND | OR'
		"""
		try :
			cursor = self.__getCursor()
			sql = self.__joinWhere(sql,params,join)
			cursor.execute(sql,tuple(params.values()))
			rows = cursor.fetchmany(size=limit) if limit > 0 else cursor.fetchall()
			result = [dict(zip(cursor.column_names,row)) for row in rows] if rows else None
			return result
		except:
			raise BaseError(706)


	def countBySql(self,sql,params = {},join = 'AND'):
		# 自定义sql 统计影响行数
		try:
			cursor = self.__getCursor()
			sql = self.__joinWhere(sql,params,join)
			cursor.execute(sql,tuple(params.values()))
			result = cursor.fetchone();
			return result[0] if result else 0
		except:
			raise BaseError(707)

	def deleteByCond(self,table,cond):
		# 删除数据
		try :
			sql = "DELETE FROM %s where %s" % (table, cond)
			cursor = self.__getCursor()
			self.__display_Debug_IO(sql,()) #DEBUG
			cursor.execute(sql)

			#self.__conn.commit() #
			return cursor.rowcount

		#except:
		#	raise BaseError(704)
		except  Exception as err:
			raise BaseError(704,err._full_msg)


	#def updateByPk(self,table,data,id,pk='id'):
	#	# 根据主键更新，默认是id为主键
	#	return self.updateByAttr(table,data,{pk:id})

	def deleteByAttr(self,table,params={},join='AND'):
		# 删除数据
		try :
			fields = ','.join(k+'=%s' for k in params.keys())
			sql = "DELETE FROM %s "% table
			sql = self.__joinWhere(sql,params,join)
			cursor = self.__getCursor()
			self.__display_Debug_IO(sql,tuple(params.values())) #DEBUG
			cursor.execute(sql,tuple(params.values()))

			#self.__conn.commit() #
			return cursor.rowcount

		#except:
		#	raise BaseError(704)
		except  Exception as err:
			raise BaseError(704,err._full_msg)

	def deleteByPk(self,table,id,pk='id'):
		# 根据主键删除，默认是id为主键
		return self.deleteByAttr(table,{pk:id})

	def findByAttr(self,table,criteria = {}):
		# 根据条件查找一条记录
		return self.__query(table,criteria)

	def findByPk(self,table,id,pk='id'):
		return self.findByAttr(table,{'where':pk+'='+str(id)})

	def findAllByAttr(self,table,criteria={}):
		# 根据条件查找记录
		return self.__query(table,criteria,True)



	def exit(self,table,params={},join='AND'):
		# 判断是否存在
		return self.count(table,params,join) > 0

# 公共的方法 -------------------------------------------------------------------------------------
	def count(self,table,params={},join='AND'):
		# 根据条件统计行数
		try :
			sql = 'SELECT COUNT(*) FROM %s' % table

			if params :
				where ,whereValues   = self.__contact_where(params)
				sqlWhere= ' WHERE '+where if where else ''
				sql+=sqlWhere

			#sql = self.__joinWhere(sql,params,join)
			cursor = self.__getCursor()

			self.__display_Debug_IO(sql,tuple(whereValues)) #DEBUG

			if self.DataName=='ORACLE':
				cursor.execute(sql % tuple(whereValues))
			else :
				cursor.execute(sql,tuple(whereValues))
			#cursor.execute(sql,tuple(params.values()))
			result = cursor.fetchone();
			return result[0] if result else 0
		#except:
		#	raise BaseError(707)
		except  Exception as err:
			try :
				raise BaseError(707,err._full_msg)
			except :
				raise BaseError(707)


	def getToListByPk(self,table,criteria={},id=None,pk='id'):
		# 根据条件查找记录返回List
		if ('where' not in criteria) and (id is not None) :
			criteria['where']=pk+ "='" + str(id) + "'"
		return self.__query(table,criteria,isDict=False)

	def getAllToList(self,table,criteria={},id=None,pk='id',join='AND'):
		# 根据条件查找记录返回List
		if ('where' not in criteria) and (id is not None) :
			criteria['where']=pk+ "='" + str(id) + "'"
		return self.__query(table,criteria,all=True,isDict=False)

	def getToObjectByPk(self,table,criteria={},id=None,pk='id'):
		# 根据条件查找记录返回Object
		if ('where' not in criteria) and (id is not None) :
			criteria['where']=pk+"='"+str(id)+"'"
		return self.__query(table,criteria)

	def getAllToObject(self,table,criteria={},id=None,pk='id',join='AND'):
		# 根据条件查找记录返回Object
		if ('where' not in criteria) and (id is not None) :
			criteria['where']=pk+"='"+str(id)+"'"
		return self.__query(table,criteria,all=True)


	def insert(self,table,data,commit=True):
		# 新增一条记录
		try :

			'''
				从data中分离含用SQL函数的字字段到funData字典中,
				不含SQL函数的字段到newData
			'''
			funData,newData=self.__split_expression(data)

			funFields='';funValues=''

			# 拼不含SQL函数的字段及值
			fields = ', '.join(k for k in newData.keys())
			values = ', '.join(("%s", ) * len(newData))

			# 拼含SQL函数的字段及值
			if funData :
				funFields = ','.join(k for k in funData.keys())
				funValues =','.join( v for  v in funData.values())

			# 合并所有字段及值
			fields += ', '+funFields if funFields else ''
			values += ', '+funValues if funValues else ''
			sql = 'INSERT INTO %s (%s) VALUES (%s)'%(table,fields,values)
			cursor = self.__getCursor()

			for (k,v) in newData.items() :
				try:
					if  isinstance(v, str) :
						newData[k]="'%s'" % (v,)
					if v==None :
						newData[k]="null"
				except :
					pass


			self.__display_Debug_IO(sql,tuple(newData.values())) #DEBUG
			sql= sql % tuple(newData.values())

			if self.DataName=='POSTGRESQL' :
				sql+=' RETURNING id'

			cursor.execute(sql)

			#if self.DataName=='ORACLE':
				#sql= sql % tuple(newData.values())
				#cursor.execute(sql)
			#else :
				#cursor.execute(sql,tuple(newData.values()))

			if self.DataName=='ORACLE':
				# 1. commit 一定要为假
				# 2. Oracle Sequence 的命名规范为： [用户名.]SEQ_表名_ID
				# 3. 每张主表都应该有ID
				t_list=table.split('.')
				if len(t_list)>1 :
					SEQ_Name= t_list[0]+'.SEQ_'+t_list[1]+'_ID'
				else :
					SEQ_Name='SEQ_'+t_list[0]+'_ID'

				cursor.execute('SELECT %s.CURRVAL FROM dual' % SEQ_Name.upper())

				result = cursor.fetchone()
				insert_id= result[0] if result else 0
				#insert_id=cursor.rowcount
			elif self.DataName=='MYSQL' :
				insert_id = cursor.lastrowid
			elif self.DataName=='POSTGRESQL':
				item = cursor.fetchone()
				insert_id = item[0]

			if commit : self.commit()
			return insert_id

		except  Exception as err:
			try :
				raise BaseError(705,err._full_msg)
			except :
				raise BaseError(705,err.args)

	def update(self,table,data,params={},join='AND',commit=True,lock=True):
		# 更新数据
		try :
			fields,values  = self.__contact_fields(data)
			if params :
				where ,whereValues   = self.__contact_where(params)

			values.extend(whereValues) if whereValues else values

			sqlWhere= ' WHERE '+where if where else ''

			cursor = self.__getCursor()

			if commit : self.begin()

			if lock :
				sqlSelect="SELECT %s From %s %s for update" % (','.join(tuple(list(params.keys()))),table,sqlWhere)
				sqlSelect=sqlSelect % tuple(whereValues)
				cursor.execute(sqlSelect)  # 加行锁
				#cursor.execute(sqlSelect,tuple(whereValues))  # 加行锁

			sqlUpdate = "UPDATE %s SET %s "% (table,fields) + sqlWhere

			for index,val in enumerate(values):
				if isinstance(val,str) :
					values[index]="'"+val+"'"
				if val==None :
					values[index]="null"

			self.__display_Debug_IO(sqlUpdate,tuple(values)) #DEBUG
			sqlUpdate = sqlUpdate % tuple(values)
			cursor.execute(sqlUpdate)

			#cursor.execute(sqlUpdate,tuple(values))

			if commit : self.commit()

			return cursor.rowcount

		except  Exception as err:
			try :
				raise BaseError(705,err._full_msg)
			except :
				raise BaseError(705,err.args)


	def updateByPk(self,table,data,id,pk='id',commit=True,lock=True):
		# 根据主键更新，默认是id为主键
		return self.update(table,data,{pk:id},commit=commit,lock=lock)

	def delete(self,table,params={},join='AND',commit=True,lock=True):
		# 更新数据
		try :
			data={}
			fields,values  = self.__contact_fields(data)
			if params :
				where ,whereValues   = self.__contact_where(params)

			values.extend(whereValues) if whereValues else values

			sqlWhere= ' WHERE '+where if where else ''

			cursor = self.__getCursor()

			if commit : self.begin()

			#if lock :
				#sqlSelect="SELECT %s From %s %s for update" % (','.join(tuple(list(params.keys()))),table,sqlWhere)
				#sqlSelect=sqlSelect % tuple(whereValues)
				#cursor.execute(sqlSelect)  # 加行锁
				##cursor.execute(sqlSelect,tuple(whereValues))  # 加行锁

			sqlDelete = "DELETE FROM %s %s"% (table,sqlWhere)

			for index,val in enumerate(values):
				if isinstance(val,str) :
					values[index]="'"+val+"'"

			self.__display_Debug_IO(sqlDelete,tuple(values)) #DEBUG
			sqlDelete = sqlDelete % tuple(values)
			cursor.execute(sqlDelete)

			#cursor.execute(sqlUpdate,tuple(values))

			if commit : self.commit()

			return cursor.rowcount

		except  Exception as err:
			try :
				raise BaseError(705,err._full_msg)
			except :
				raise BaseError(705,err.args)

	def deleteByPk(self,table,id,pk='id',commit=True,lock=True):
		# 根据主键更新，默认是id为主键
		return self.delete(table,{pk:id},commit=commit,lock=lock)

# 内部私有的方法 -------------------------------------------------------------------------------------

	def __display_Debug_IO(self,sql,params) :
		# 不输出SQL语句，以减少屏显时间
		return
		if self.debug :
			debug_now_time = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
			print('[S '+debug_now_time+' SQL:] '+(sql % params) if params else sql)

	def __get_connection(self):
		return self.pool.get_connection()

	def __getCursor(self):
		"""获取游标"""
		if self.__cursor is None:
			self.__cursor = self.__conn.cursor()
		return self.__cursor
	def getCursor(self):
		"""获取游标"""
		if self.__cursor is None:
			self.__cursor = self.__conn.cursor()
		return self.__cursor

	def __joinWhere(self,sql,params,join):
		# 转换params为where连接语句
		if params:

			funParams={};newParams={};newWhere='';funWhere=''

			# 从params中分离含用SQL函数的字字段到Params字典中
			for (k,v) in params.items():
				if 'str' in str(type(v)) and '{{' == v[:2] and '}}'==v[-2:]  :
					funParams[k]=v[2:-2]
				else:
					newParams[k]=v

			# 拼 newParams 条件
			keys,_keys = self.__tParams(newParams)
			newWhere = ' AND '.join(k+'='+_k for k,_k in zip(keys,_keys)) if join == 'AND' else ' OR '.join(k+'='+_k for k,_k in zip(keys,_keys))

			# 拼 funParams 条件
			if funParams :
				funWhere = ' AND '.join(k+'='+v for k,v in funParams.items()) if join == 'AND' else ' OR '.join(k+'='+v for k,v in funParams.items())

			# 拼最终的 where
			where=((newWhere+' AND ' if newWhere else '')+funWhere if funWhere else newWhere) if join=='AND' else ((newWhere+' OR ' if newWhere else '')+funWhere if funWhere else newWhere)

			#--------------------------------------
			#keys,_keys = self.__tParams(params)
			#where = ' AND '.join(k+'='+_k for k,_k in zip(keys,_keys)) if join == 'AND' else ' OR '.join(k+'='+_k for k,_k in zip(keys,_keys))
			sql+=' WHERE ' + where
		return sql

	def __tParams(self,params):
		keys = [k  if k[:2]!='{{' else k[2:-2] for k in params.keys()]
		_keys = ['%s' for k in params.keys()]
		return keys,_keys

	def __query(self,table,criteria,all=False,isDict=True,join='AND'):
		'''
		   table	: 表名
		   criteria : 查询条件dict
		   all	  : 是否返回所有数据，默认为False只返回一条数据,当为真是返回所有数据
		   isDict   : 返回格式是否为字典，默认为True ，即字典否则为数组
		'''
		try :
			if all is not True:
				criteria['limit'] = 1  # 只输出一条
			sql,params = self.__contact_sql(table,criteria,join) #拼sql及params
			'''
			# 当Where为多个查询条件时，拼查询条件 key 的 valuse 值
			if 'where' in criteria and 'dict' in str(type(criteria['where'])) :
				params = criteria['where']
				#params = tuple(params.values())
				where ,whereValues   = self.__contact_where(params)
				sql+= ' WHERE '+where if where else ''
				params=tuple(whereValues)
			else :
				params = None
			'''
			#__contact_where(params,join='AND')
			cursor = self.__getCursor()

			self.__display_Debug_IO(sql,params) #DEBUG

			#if self.DataName=="ORACLE":
				#sql="select * from(select * from(select t.*,row_number() over(order by %s) as rownumber from(%s) t) p where p.rownumber>%s) where rownum<=%s" % ()
				#pass


			cursor.execute(sql,params if params else ())

			rows = cursor.fetchall() if all else cursor.fetchone()

			if isDict :
				result = [dict(zip(cursor.column_names,row)) for row in rows] if all else dict(zip(cursor.column_names,rows)) if rows else {}
			else :
				result = [row for row in rows] if all else rows if rows else []
			return result
		except  Exception as err:
			try :
				raise BaseError(706,err._full_msg)
			except :
				raise BaseError(706)


	def __contact_sql(self,table,criteria,join='AND'):
		sql = 'SELECT '
		if criteria and type(criteria) is dict:
			#select fields
			if 'select' in criteria:
				fields = criteria['select'].split(',')
				sql+= ','.join(field.strip()[2:-2] if '{{' == field.strip()[:2] and '}}'==field.strip()[-2:] else field.strip() for field in fields)
			else:
				sql+=' * '
			#table
			sql+=' FROM %s' % table

			#where
			whereValues=None
			if 'where' in criteria:
				if 'str' in str(type(criteria['where'])) :   # 当值为String时，即单一Key时
					sql+=' WHERE '+ criteria['where']
				else :									   # 当值为dict时，即一组key时
					params=criteria['where']
					#sql+= self.__joinWhere('',params,join)
					#sql+=self.__contact_where(params,join)
					where ,whereValues   = self.__contact_where(params)
					sql+= ' WHERE '+where if where else ''
					#sql=sql % tuple(whereValues)

			#group by
			if 'group' in criteria:
				sql+=' GROUP BY '+ criteria['group']
			#having
			if 'having' in criteria:
				sql+=' HAVING '+ criteria['having']

			if self.DataName=='MYSQL' :
				#order by
				if 'order' in criteria:
					sql+=' ORDER BY '+ criteria['order']
				#limit
				if 'limit' in criteria:
					sql+=' LIMIT '+ str(criteria['limit'])
				#offset
				if 'offset' in criteria:
					sql+=' OFFSET '+ str(criteria['offset'])
			elif (self.DataName=='POSTGRESQL') :
				#order by
				if 'order' in criteria:
					sql+=' ORDER BY '+ criteria['order']
				if  'limit' in criteria :
					# 取 offset,rowcount
					arrLimit=(str(criteria['limit']).split('limit ').pop()).split(',')
					strOffset = arrLimit[0]
					try :
						strRowcount  = arrLimit[1]
					except :
						strOffset	= '0'
						strRowcount  = '1'
					sql+='  LIMIT %s OFFSET %s' %(strRowcount,strOffset)

			elif (self.DataName=='ORACLE') and ('limit' in criteria) :
				# 取 offset,rowcount
				arrLimit=(str(criteria['limit']).split('limit ').pop()).split(',')
				strOffset = arrLimit[0]
				try :
					strRowcount  = arrLimit[1]
				except :
					strOffset	= '0'
					strRowcount  = '1'

				# 处理 order by
				if 'order' in criteria :
					strOrder = criteria['order']
				else :
					strOrder = 'ROWNUM'
				# 以下Sql是针对 Oracle 的大数据查询效率
				sql="select * from(select * from(select t.*,row_number() over(order by %s) as rownumber from(%s) t) p where p.rownumber>%s) where rownum<=%s" % (strOrder,sql,strOffset,strRowcount)
			elif (self.DataName=='ORACLE') and ('order' in criteria) :
				sql+=' ORDER BY '+ criteria['order']

		else:
			sql+=' * FROM %s' % table

		return sql,whereValues

	# 将字符串和表达式分离
	def __split_expression(self,data) :
		funData={};newData={};funFields=''

		# 从data中移出含用SQL函数的字字段到funData字典中
		for (k,v) in data.items():
			if 'str' in str(type(v)) and '{{' == v[:2] and '}}'==v[-2:] :
				funData[k]=v[2:-2]
			else : newData[k]=v

		return (funData,newData)


	# 拼Update字段
	def __contact_fields(self,data) :

		funData,newData=self.__split_expression(data)
		if funData :
			funFields = ','.join(k+'=%s'  % (v) for k,v in funData.items())
		fields = ','.join(k+'=%s' for k in newData.keys())


		# fields 与 funFields 合并
		if funData :
			fields = ','.join([fields,funFields]) if fields else funFields

		values = list(newData.values())

		return (fields,values)

	def __hasKeyword(self,key) :
		if '{{}}' in key : return True
		if 'in ('  in key : return True
		if 'like ' in key : return True
		if '>' in key : return True
		if '<' in key : return True
		return False

	# 拼Where条件
	def __contact_where(self,params,join='AND') :
		funParams,newParams=self.__split_expression(params)

		# 拼 newParams 条件
		keys,_keys = self.__tParams(newParams)
		newWhere = ' AND '.join(k+'='+_k for k,_k in zip(keys,_keys)) if join == 'AND' else ' OR '.join(k+'='+_k for k,_k in zip(keys,_keys))
		values = list(newParams.values())

		# 拼 funParams 条件
		#funWhere = ' AND '.join(('`' if k else '') +k+('`' if k else '')+ (' ' if self.__hasKeyword(v) else '=') +v for k,v in funParams.items()) if join == 'AND' else ' OR '.join('`'+k+'`'+(' ' if self.__hasKeyword(v) else '=')+v for k,v in funParams.items())


		funWhere = ' AND '.join(k+ (' ' if self.__hasKeyword(v) else '=' if k else '') +v for k,v in funParams.items()) if join == 'AND' else ' OR '.join(k+(' ' if self.__hasKeyword(v) else '=' if k else '')+v for k,v in funParams.items())

		# 拼最终的 where
		where=((newWhere+' AND ' if newWhere else '')+funWhere if funWhere else newWhere) if join=='AND' else ((newWhere+' OR ' if newWhere else '')+funWhere if funWhere else newWhere)
		return (where,values)


	def get_ids(self,list): #从getAllToList返回中提取id
		try:
			test=list[0][0]
			dimension=2
		except:
			dimension=1

		ids=[]
		if dimension>1 :
			for i in range(len(list)) : ids.append(str(list[i][0]))
		else :
			for i in range(len(list)) : ids.append(str(list[i]))

		return ','.join(ids)
