#coding:utf-8

import re
import time
import datetime
import sys

from db import dbMysql
from web.baseException import errorDic,BaseError
from web.Base  import WebRequestHandler,BaseError,operator_except

from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE
from libs.utils import debug
from libs.utils.debug import *
from public.areaCodeManage import *

class accountManage(dbMysql.CURD) :
    def __init__(self,db) :
        if sys.version > '3':
            # python 3.0 +
            super().__init__(db,'system.account',False) # 定义本实例需要操作的表名
        else :
            # python 2.7
            super(accountManage,self).__init__(db,'system.account',False)

def getWhereClause(systemUserID, companyName, deptName, groupID, userID, userName):
	
	sql_company	 = ""
	if companyName != "选择全部" and companyName != None and companyName != "":
		sql_company = " ac.company = '%s' "%companyName

	sql_dept	 = ""
	if deptName != "选择全部" and deptName != None and deptName != "":
		sql_dept = " ac.dept_name = '%s' "%deptName

	sql_name = ""
	if userName != "" and userName != None:
		sql_name += " ac.name like '%s"%userName
		sql_name += "%'"

	sql_group = ""
	if groupID > 0:
		sql_group += " ( ug.name != 'supervisors' and ug.id = %d)"%groupID

	if userID == 0:
		sql_where = " where ac.system_user_id = %d"%systemUserID
	else:
		sql_where = " where ac.id = %d "%userID

	if sql_company != "":
		sql_where += sql_company

	if sql_dept != "":
		sql_where += " and " + sql_dept

	if sql_name != "":
		sql_where += " and " + sql_name

	if sql_group != "":
		sql_where += " and " + sql_group

	return sql_where


# 获取账户信息，唯一获取用户账户信息的入口
# 参数：
# 	uid：查询用户ID，可选，如果没有则查询所有用户的信息
# 	uname: 查询用户姓名
# 	gid：查询指定用户组用户信息，可选，如果没有则查询所有用户的信息；
# 	company：公司名称；
# 	dept：部门名称
# 	offset：页数；
# 	rowlimit：每页数据行数；
# 	userid 和 groupid 只能同时用一个参数，默认优先使用userid;
# 返回值：
#   1、accountList, 用户账户信息
#   
def getAccountInfo(self, uid = 0, uname = "", gid = 0, company = "", dept = "", offset = 1, rowlimit = 20 ):

	if uid == None:
		uid = 0

	if gid == None:
		gid = 0

	if company == None:
		company = "选择全部"

	if dept == None:
		dept = "选择全部"

	if offset == None:
		offset = 1

	if rowlimit == None:
		rowlimit = 20

	offset = ( offset - 1 ) * rowlimit

	userInfo = self.GetUserInfo()

	rowdata = {}
	cur = self.db.getCursor()

	sql  = "select distinct(ac.id), ac.create_time, ac.update_time, ac.create_id, ac.update_id, ac.user_name, ac.pass, "
	sql += " ac.user_type, ac.status, ac.name, ac.id_no, ac.sex, ac.birthday, ac.country, ac.company, ac.dept_name, ac.mailbox, ac.description, ac.mobile_phone1, "
	sql += " ac.mobile_phone2, ac.address1, ac.address2, ac.photo_path, ac.default_language, ac.system_user_id, "
	sql += " b.name user_type_name, d.name status_name, gn.groupname, cv.name "
	sql += " from system.account ac "
	sql += " left join system.code_value b on ac.user_type = b.code and b.type_code = 'ACCOUNT_TYPE' and b.status= '1' "
	sql += " left join system.code_value d on ac.status = d.code and d.type_code = 'ACCOUNT_STATUS' and d.status= '1' "
	sql += " left join system.code_value cv on cv.code = ac.sex and cv.type_code = 'SEX'"
	if gid > 0:
		sql += " inner join system.user_group_member ugm on ugm.user_id = ac.id "
		sql += " inner join system.user_group ug on ug.id = ugm.group_id "
	
	sql += " left join (select r.user_id, array_to_string(array(select unnest(array_agg(s.c_name))),'，') as groupname from system.user_group_member r left join system.user_group s on s.id=r.group_id where s.name is not null group by r.user_id) gn on gn.user_id=ac.id "

	sql_where = getWhereClause(userInfo['system_user_id'], company, dept, gid, uid, uname)
	sql += sql_where
	sql += " order by ac.name"

	sql_limit = ""
	if uid == 0:
		sql_limit = " limit " + str(rowlimit) + " offset " + str(offset)
	sql += sql_limit

	# logI("Account SQL:", sql)
	cur.execute(sql)
	rows = cur.fetchall()

	# logI("获取用户信息：%s"%sql, rows)
	
	rowdata['struct'] =  "id, create_time, update_time, create_id, update_id, user_name, pass, "
	rowdata['struct'] += " user_type, status, name, id_no, sex, birthday, country, company, dept_name, mailbox, description, mobile_phone1, "
	rowdata['struct'] += " mobile_phone2, address1, address2, photo_path, default_language, system_user_id, "
	rowdata['struct'] += " user_type_name, status_name, group_name_list, sex_name"
	rowdata['rows'] = rows

	sql  = "select count(distinct(ac.id)) from system.account ac  "
	if gid > 0 :
		sql += " inner join system.user_group_member ugm on ugm.user_id = ac.id "
		sql += " inner join system.user_group ug on ug.id = ugm.group_id "

	sql += sql_where

	cur.execute(sql)
	row = cur.fetchone() 
	rowdata['count'] = row[0]

	return rowdata


class Restful(WebRequestHandler):

	# 查询账户基础数据,
	# 查询单个用户的详细信息，查询所有用户详细信息
	# 参数：
	# 	1、id：操作用户ID，必选
	# 	2、uid：查询用户ID，可选，如果没有则查询所有用户的信息
	# 	3、gid：查询指定用户组用户信息，可选，如果没有则查询所有用户的信息；
	# 	4、cn：公司名称；
	# 	5、dn：部门名称
	# 	6、o：页数；
	# 	7、r：每页数据行数；
	# 	userid 和 groupid 只能同时用一个参数，默认优先使用userid;
	# 返回值：
	#   1、accountList, 用户账户信息
	#   2、menuList，指定用户或用户组的菜单组;
	@operator_except
	def get(self):
		# 获取账户信息

		opUserID   		= int(self.get_argument('id',  default = '0'))
		queryUserID 	= int(self.get_argument('uid', default = '0'))
		queryGroupID 	= int(self.get_argument('gid', default = '0'))
		companyName		= self.get_argument('cn',  default = '选择全部')
		deptName		= self.get_argument('dn',  default = '选择全部')

		offset 			= int(self.get_argument('o',  default = '1'))
		rowlimit 		= int(self.get_argument('r',  default = '20'))
		uname 			= self.get_argument('un',  default = "")

		accountInfo = getAccountInfo(self, queryUserID, uname, queryGroupID, companyName, deptName, offset, rowlimit )

		self.response( accountInfo )

	# 创建一个账户记录
	@operator_except
	def post(self):

		alldata 	= self.getRequestData()
		groupIDList = self.getParam( alldata, "groupList", None)
		if groupIDList == None:
			raise BaseError(801, "新建用户必须至少加入一个用户组！")

		s = accountManage(self.db)
		
		lsData = {
			'user_name'		: 'user_name',
			'pass' 			: 'pass', 
			'user_type' 	: 'user_type',
			'status' 		: 'status',
			'name' 			: 'name', 
			'country' 		: 'country',
			'company' 		: 'company',
			'dept_name' 	: 'dept_name',
			'description' 	: 'description',
			'mobile_phone1' : 'mobile_phone1',
			'mobile_phone2'	: 'mobile_phone2', 
			'mailbox' 		: 'mailbox',
			'id_no' 		: 'id_no',
			'address1' 		: 'address1',
			'address2' 		: 'address2',
			'photo_path' 	: 'photo_path',
			'sex'			: 'sex',
			'default_language' : 'default_language',
		}

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass
		
		# 检查用户名是否重复
		db = self.getDB("system.account")
		result = db.findByCond("user_name", "user_name = '%s' "%data['user_name'])
		if len( result['rows'] ) > 0 :
			raise BaseError(910)

		# 创建的新用户和创建者属于同一业务类型
		userInfo = self.GetUserInfo()
		data['system_user_id'] 	= userInfo['system_user_id']
		data['create_id'] 		= self.objUserInfo['id']
		data['create_time'] 	= datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		
		# 账户类型：子账户
		data['user_type']		= "2"
		# 账户状态：正常
		data['status']			= "2"
		data['photo_path']		= "./resource/images/default.png"

		uid = s.save(data, table = 'system.account')

		# 添加用户组成员
		ugmData = {
			'create_id' 	: self.objUserInfo['id'],
			'create_time' 	: datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
			'user_id' 		: uid,
			'system_user_id': self.GetSystemUserID(),
		}

		for gid in groupIDList:
			ugmData['group_id'] = gid
			s.save( ugmData, table = 'system.user_group_member')

		# 创建用户成功，记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "accountManage", 
			"创建账户：用户名：%s，用户姓名：%s"%(data['user_name'], data['name']), uid)
		self.response(uid)

	# 更新用户信息
	# 参数：
	#   id: 待更新用户的ID，必选
	#   其他参数可选
	#   groupIDList: 用户所在用户组的ID列表
	@operator_except
	def put(self):
		# 取POST Form提交的数据
		alldata = self.getRequestData()
		if self.getParam( alldata, "id", 0) == 0:
			raise BaseError(801, "必须指定待更新信息的账户ID！")

		groupIDList = self.getParam( alldata, "groupList", None)
		if groupIDList == None:
			raise BaseError(801, "必须为用户指定至少一个用户组！")

		s = accountManage(self.db)
		
		lsData={
			'id' 			: 'id',
			'user_name'		: 'user_name',
			'pass' 			: 'pass', 
			'user_type' 	: 'user_type',
			'status' 		: 'status',
			'name' 			: 'name', 
			'country' 		: 'country',
			'company' 		: 'company',
			'dept_name' 	: 'dept_name',
			'description' 	: 'description',
			'mobile_phone1' : 'mobile_phone1',
			'mobile_phone2'	: 'mobile_phone2', 
			'mailbox' 		: 'mailbox',
			'id_no' 		: 'id_no',
			'address1' 		: 'address1',
			'address2' 		: 'address2',
			'photo_path' 	: 'photo_path',
			'sex'			: 'sex',
			'default_language' : 'default_language',
        }

		data = {}
		for (k, v) in lsData.items():
			try:
				data[k] = alldata[v]
			except:
				pass

		data['update_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		data['update_id'] = self.objUserInfo['id']
		uid = data['id']
		id = s.save(data, uid, table = 'system.account')

		# 更新用户组信息
		cur = self.db.getCursor()

		db = self.getDB("system.user_group_member")
		
		# 将该用户从用户组中删除
		sql = " delete from system.user_group_member where user_id = %s and group_id not in (%s)"%(data['id'], ','.join(str(i) for i in groupIDList))
		cur.execute(sql)

		# 将用户添加进入指定的用户组
		for gid in groupIDList:
			result = db.findByCond('id', 'group_id = %s and user_id = %s'%(gid, data['id']))
			if len(result['rows']) > 0 :
				# 已经是用户组成员
				continue

			ugmData = {
				'create_id' 	: self.objUserInfo['id'],
				'create_time' 	: datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
				'group_id' 		: gid,
				'user_id' 		: uid,
				'system_user_id': self.GetSystemUserID(),
			}
			s.save( ugmData, table = "system.user_group_member")

		logInfo = "更新账户 "
		if 'user_name' in data:
			logInfo += "，用户名：" + data['user_name']

		if 'name' in data:
			logInfo += "， 用户姓名：" + data['name']
		operation_log(self.db).addLog(self.GetUserInfo(), "accountManage", logInfo, id)
		self.response(id)


	# 删除指定账户
	# 参数：
	#  	id: 待删除的账户ID，必选 
	@operator_except
	def delete(self):
		alldata = self.getRequestData()
		if 'id' not in alldata or 'name' not in alldata:
			raise BaseError(801, "必须指定待更删除的账户ID！")

		uid = self.getParam( alldata, "id", 0)

		# 删除用户所在的用户组成员记录
		s = accountManage(self.db)
		s.remove( uid, table = "system.user_group_member",   key = "user_id", delete = True)

		# 删除用户账户记录
		s.remove( uid, table = "system.account",   key = "id", delete = True)

		# 记录日志并退出
		operation_log(self.db).addLog(self.GetUserInfo(), "accountManage", "删除账户记录 %s"%alldata['name'], uid)
		self.response(uid)


	# 获取账户相关数据，包括公司列表，部门列表，用户组列表，及用户名称列表
	# 参数：
	# 	1、id：操作用户ID，必选
	# 	2、cn: 公司名称，可选；
	# 	3、dn：部门名称，可选；
	# 	4、gn：用户组名称，可选
	# 	5、uid：用户id，可选；
	# 	根据四个参数进行筛选，如果都没有则返回所有
	@operator_except
	def patch(self):
		paramData = self.getRequestData()

		op = self.getParam(paramData, 'op', '')

		if op == "list":

			companyName = self.getParam(paramData, 'cn', '选择全部') 
			deptName 	= self.getParam(paramData, 'dn', '选择全部') 
			groupID 	= self.getParam(paramData, 'gid', 0) 
			userID 		= self.getParam(paramData, 'uid', 0) 

			# logI("Account Patch Params: ", paramData )

			userInfo 	= self.GetUserInfo()
			sql_where 	= getWhereClause(userInfo['system_user_id'], companyName, deptName, groupID, userID, "")

			cur 		= self.db.getCursor() 

			# 返回的数据
			data = {}

			# 公司名称列表
			sql  = "select distinct(ac.company) from system.account ac"
			sql += " inner join system.user_group_member ugm on ugm.user_id = ac.id "
			sql += " inner join system.user_group ug on ug.id = ugm.group_id "
			sql += sql_where
			sql += " order by ac.company "
			# logI("Account Patch SQL1:", sql)
			cur.execute(sql)
			rows = cur.fetchall()

			# logI("公司", sql, rows)

			companyList = {}
			companyList['struct']	= "name"
			companyList['rows'] 	= [("选择全部", )] + rows

			data['companyList'] 	= companyList

			# 部门列表
			sql  = "select distinct(ac.dept_name) from system.account ac "
			sql += " inner join system.user_group_member ugm on ugm.user_id = ac.id "
			sql += " inner join system.user_group ug on ug.id = ugm.group_id "	
			sql += sql_where
			sql += " order by ac.dept_name "

			cur.execute(sql)
			rows = cur.fetchall()

			deptList = {}
			deptList['struct']	= "name"
			deptList['rows'] 	= [("选择全部", )] + rows

			data['deptList'] 	= deptList

			# 用户组列表
			sql  = " select distinct(ug.id), ug.c_name from system.user_group ug "
			sql += " where ug.name != 'supervisors' and ug.system_user_id = %d"%userInfo['system_user_id']
			sql += " order by ug.c_name "

			# logI("GroupSQL: ", sql)
			cur.execute(sql)
			rows = cur.fetchall()

			groupList = {}
			groupList['struct']	= "id, name"
			groupList['rows'] 	= [(0, "选择全部", )] + rows

			data['groupList'] 	= groupList

			# 用户列表
			sql  = " select distinct(ac.id), ac.name from system.account ac "
			sql += " inner join system.user_group_member ugm on ugm.user_id = ac.id "
			sql += " inner join system.user_group ug on ug.id = ugm.group_id "
			sql += sql_where
			sql += " order by ac.name "

			cur.execute(sql)
			rows = cur.fetchall()

			userList = {}
			userList['struct']	= "id, name"
			userList['rows'] 	= [(0, "选择全部", )] + rows
			data['userList'] 	= userList

			self.response(data)

		# 获取指定用户的地址信息
		elif op == "ua":
			userID = self.getParam(paramData, 'uid', 0) 

			# 获取SA用户的账户信息
			data = getAccountInfo(self, uid = userID )

			key = []
			keyList = data['struct'].split(",")
			for item in keyList:
				key.append(item.strip())

			if len(data['rows']) == 0:
				# 没有找到用户数据
				self.response(0)

			accountInfo = dict(zip(key, data['rows'][0]))

			addrList = []
			addrList.append(accountInfo['address1'])
			addrList.append(accountInfo['address2'])

			# 根据地址信息进行匹配
			data = {}

			Address  = []
			for addr in addrList:
				Address.append(getAreaInfo(self, addr))

			data['Address'] = Address
			self.response(data)




