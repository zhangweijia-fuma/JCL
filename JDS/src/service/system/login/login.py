#coding:utf-8

import config
import base64,time
import datetime
import sys

from libs.db import dbMysql
from libs.web.baseException import errorDic,BaseError
from libs.web.Base  import WebRequestHandler,BaseError,operator_except,public_handler

from JCL.JCLCommon import *

from Crypto.Cipher import AES
from hashlib import md5

from libs.utils.debug import *
from libs.utils.utils import *
from libs.utils import user

from system.login.heartbeat import *

class Login(dbMysql.CURD) :
    def __init__(self,db) :
        if sys.version > '3':
            # python 3.0 +
            super().__init__(db,'system.account',False) # 定义本实例需要操作的表名
        else :
            # python 2.7
            super(Login,self).__init__(db,'system.account',False)
            

class Handle(WebRequestHandler):

	@public_handler
	@operator_except
	def get(self,callback_url,dataPacket):

		s = Login(self.db)

		rowData = {}

		appId	 	= self.getRequestHeader('App-Key')
		appSecret 	= self.getRequestHeader('App-Secret')
		
		#2. 解数据包
		dataPacket 	= base64.standard_b64decode(dataPacket).decode()
		uidLength	= int(dataPacket[17:19])
		iv		   	= dataPacket[:16]
		uid		  	= dataPacket[-uidLength:]
		encryptedUrl= dataPacket[19:-uidLength]

		cur = self.db.getCursor();
		sql = "select at.pass, status, user_type from system.account at where at.user_name = '%s'"%uid
		cur.execute(sql)
		rowData_sysuser = cur.fetchone()

		currentTime = GetSystemTime()

		# if len(rowData_sysuser) == 0 :
		if rowData_sysuser == None:
			# 记录登录日志
			loginData = {
				'create_time' 	: currentTime,
				'create_id' 	: 0,
				'user_name' 	: uid,
				'ip_address' 	: self.request.remote_ip,
				"result"		: "登陆失败，用户名不存在",
			}

			id = s.save( loginData, table = 'system.login_record')
			logI("User %s not exist, IP: %s."%(uid, self.request.remote_ip))
			rowData['code'] = 603
			self.response(rowData)
			# raise BaseError(603) # 用户名错误或密码错误

		elif rowData_sysuser[1]=='1':
			# 记录登录日志
			loginData = {
				'create_time' 	: currentTime,
				'create_id' 	: 0,
				'user_name' 	: uid,
				'ip_address' 	: self.request.remote_ip,
				"result"		: "登陆失败，账号被禁用",
			}
			s.save( loginData, table = 'system.login_record')

			logI("user account %s not allowed login, IP: %s"%(uid, self.request.remote_ip))
			rowData['code'] = 605
			self.response(rowData)	
			# raise BaseError(605)

		else:
			passwd = rowData_sysuser[0]
			# 4. 用passwd对surl进行AES反解码

			# 在新版本的加密库中，需要对passwd和iv进行编码
			iv2 	= iv.encode('utf-8')
			passwd2 =passwd.encode('utf-8')

			generator = AES.new(passwd2, AES.MODE_CBC, iv2)

			cryptedStr = encryptedUrl
			crypt	  = base64.b64decode(cryptedStr)
			try :
				recovery = generator.decrypt(crypt).decode()
			except :
				recovery = ''

			url = recovery.rstrip('\0')

			self.set_header('Access-Control-Allow-Origin','*')
			if callback_url != url :
				logE("Error: User %s login failed because wrong password, IP: %s"%(uid, self.request.remote_ip))

				# 记录登录日志
				loginData = {
					'create_time' 	: currentTime,
					'create_id' 	: 0,
					'user_name' 	: uid,
					'ip_address' 	: self.request.remote_ip,
					"result"		: "登陆失败，密码错误",
				}
				s.save( loginData, table = 'system.login_record')

				self.set_header('Authorization', '')

				rowData['code'] = 606
				self.response(rowData)

			else:
				# 登录成功
				# 
				# 不检查用户是否登录，即允许用户多地同时登录

				rowData['code'] = 0

				#获取用户信息
				cur = self.db.getCursor()
				cur.execute("select a.id,a.user_name,a.name,a.user_type, company, dept_name, a.system_user_id, b.user_service "
							"from system.account a  "
							"inner join system.system_user_list b on b.id = a.system_user_id "
					        "where a.user_name='%s'"%(uid))
				rows = cur.fetchall()
				rowData['struct'] = "id,user_name,name, user_type, company, dept_name, system_user_id, user_service"
				rowData['rows']   = rows

				# save user info to cache
				index 	= 0;
				account = {}
				fields 	= ['id','user_name','name','type',  'company', 'dept_name', 'system_user_id', 'user_service']
				row 	= rows[0]
				for field in fields :
					account[field] = row[index]
					index += 1

				# 获取用户组
				sql  = "select mag.id, mag.name from system.user_group mag, system.user_group_member magr "
				sql += " where magr.user_id=%s and mag.id=magr.group_id"%(account['id'])
				cur.execute(sql)
				userGroup = cur.fetchall()

				rowData['userGroup'] = userGroup
				account["userGroup"] = userGroup

				# 检查用户是否为管理员组成员
				rowData['isAdmin'] = False
				for group in userGroup:
					groupName = group[1].upper()
					if groupName.find('MANAGE') >= 0 :
						rowData['isAdmin'] = True
						break

				authcode = appId + uid + iv + url + str(int(time.time()))
				authcode = md5(authcode.encode()).hexdigest()
				self.set_header('Access-Control-Expose-Headers','Authorization')
				self.set_header('Authorization', authcode)

				u = user.user()
				u.set(authcode, account)

				# 记录登录日志
				loginSessionID = LoginRecord.UserLogin( account, self.request.remote_ip)
				rowData['loginSessionID'] = loginSessionID

				# 获取数据字典
				sql = "select cv.code, cv.name, cv.type_code, cv.status, cv.sort "
				sql += " from system.code_value cv "
				cur.execute(sql)
				rows = cur.fetchall()

				dataDict = {}
				dataDict['rows'] = rows
				dataDict['struct'] = 'code, name, type_code, status, sort'
				rowData['dataDict'] = dataDict

				# 获取系统用户信息
				SystemUserID = account['system_user_id']
				sql  = "select id, user_name, user_name_en, abbr_name, abbr_name_en, app_name, app_name_en, admin_account_id, admin_group_id, user_service,"
				sql += " 	logo_vertical, logo_horizontal"
				sql += " from system.system_user_list"
				sql += " where id = %d"%SystemUserID

				cur.execute(sql)
				rows = cur.fetchall()

				systemUserInfo = {}
				systemUserInfo['rows'] 		= rows
				struct 	= 'id, user_name, user_name_en, abbr_name, abbr_name_en, app_name, app_name_en, admin_account_id, admin_group_id, user_service,'
				struct += ' logo_vertical, logo_horizontal'

				systemUserInfo['struct'] 	= struct
				rowData['systemUserInfo'] 	= systemUserInfo

				# 返回
				self.response(rowData)

