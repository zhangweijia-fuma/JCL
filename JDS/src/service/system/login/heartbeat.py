#coding:utf-8

import os, sys, time, threading, datetime

from libs.db.dbHelper import *
from libs.web.Base  import WebRequestHandler,BaseError,operator_except
from libs.utils.debug import *
from libs.utils.utils import *

from JCL.JCLCommon import *

# 心跳消息上报时间间隔，单位为秒
HEARTBEAT_INTERVAL      = 10

# 心跳连接最大超时次数
HB_EXPIRE_TIMES         = 3


class CLoginRecord(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)

        self.OnlineUserList = []

    # 启动线程
    def run(self):

        # 获取数据库中有登录时间，无登出时间的用户数据
        dbHelper = DbHelper()
        sql = """
            select lr.id, lr.user_id, lr.user_name, lr.name, lr.ip_address, lr.login_time  
            from system.login_record lr
            where (lr.user_id, lr.id) in (
                select user_id, max(id) id
                from system.login_record
                where login_time is not NULL and logout_time is NULL
                group by user_id
            )
            """

        cur = dbHelper.getCursor()
        cur.execute(sql)
        rows = cur.fetchall()
        dbHelper.closeDB()

        key = ['id', 'user_id', 'user_name', 'name', 'ip_address', 'login_time']
        for row in rows:
            info = dict(zip(key, row))
            info['expire_times'] = 0
            self.OnlineUserList.append(info)
            logI("User %s online, IP: %s, login time: %s, login Session ID: %d"%(info['user_name'], info['ip_address'], info['login_time'], info['id']))

        # 定时更新用户的在线状态
        self.heartbeatTimer = threading.Timer( HEARTBEAT_INTERVAL, self.CheckOnlineStatus)
        self.heartbeatTimer.start()

        # 进入线程主循环，等待发送或接收消息
        while (1):
            time.sleep(1)


    # 检查在线用户是否依然在线
    def CheckOnlineStatus(self):

        for subs in self.OnlineUserList:
            if subs['expire_times'] >= HB_EXPIRE_TIMES:

                # 更新用户状态
                data = {
                    'logout_time'   : GetSystemTime(),
                    'result'        : "心跳超时"
                }

                dbHelper = DbHelper()
                db = dbHelper.getDB("system.login_record")
                db.save(data, subs['id'])
                dbHelper.closeDB()

                # 将该用户从在线列表中移除
                logE("User %s lost connection, login time: %s, IP address: %s, login Session ID: %d"%
                    (subs['user_name'], subs['login_time'], subs['ip_address'], subs['id']))
                self.OnlineUserList.remove(subs)
            else:
                subs['expire_times'] += 1


        # 重新设置定时器
        self.heartbeatTimer = threading.Timer(HEARTBEAT_INTERVAL, self.CheckOnlineStatus)
        self.heartbeatTimer.start()

        return

    # 心跳消息处理
    def UserHeartbeat(self, userInfo):
        for subs in self.OnlineUserList:
            if subs['id'] == userInfo['loginSessionID']:
                # 找到用户，则将超时次数重置为0
                subs['expire_times'] = 0
                return
                
        # 没有找到用户的登录数据，可能是服务器重启
        # 根据记录ID进行查找
        sql = """
            select lr.id, lr.user_id, lr.user_name, lr.name, lr.login_time, lr.ip_address 
            from system.login_record lr 
            where lr.id = %d
            """%userInfo['loginSessionID']

        # logI("SQL: %s"%sql)
        dbHelper = DbHelper()
        db = dbHelper.getDB("system.login_record")

        cur = dbHelper.getCursor()
        cur.execute(sql)
        rows = cur.fetchall()

        dbHelper.closeDB()

        if len(rows) != 1:
            logE("Error：Can not find user %s data"%userInfo['user_name'])
            return
      
        key = ['id', 'user_id', 'user_name', 'name', 'login_time', 'ip_address']
        data = dict(zip(key, rows[0]))
        data['expire_times'] = 0

        logI("User %s re-connection, login Session ID: %d, login time: %s"%(userInfo['user_name'], userInfo['loginSessionID'], data['login_time']))
        loginData = {
            'result' : '重新在线',
        }

        dbHelper = DbHelper()
        db = dbHelper.getDB("system.login_record")
        db.save( loginData, data['id'], table = 'system.login_record', key = 'id')

        dbHelper.closeDB()
        
        self.OnlineUserList.append(data)

        return

    # 用户登录
    def UserLogin(self, accountInfo, IPAddr):

        loginTime = GetSystemTime()

        loginData = {
            'create_time'   : loginTime,
            'create_id'     : accountInfo['id'],
            'user_name'     : accountInfo['user_name'],
            'user_id'       : accountInfo['id'],
            'name'          : accountInfo['name'],
            'login_time'    : loginTime,
            'ip_address'    : IPAddr,
            "result"        : "登陆成功",
        }

        dbHelper = DbHelper()
        db = dbHelper.getDB("system.login_record")
        loginSessionID = db.save( loginData, table = 'system.login_record')
        loginData['id'] = loginSessionID
        dbHelper.closeDB()

        loginData['expire_times'] = 0
        self.OnlineUserList.append(loginData)

        logI("User %s login successful, IP: %s, login Time: %s, login Session ID: %d"%(accountInfo['user_name'], IPAddr, loginTime, loginSessionID))

        return loginSessionID

    # 用户主动登出
    def UserLogout(self, userInfo):

        for subs in self.OnlineUserList:
            if subs['id'] == userInfo['loginSessionID']:
                # 找到用户，更新其登出时间
                self.OnlineUserList.remove(subs)
                
        # 不管是否找到其记录，都更新记录
        logI("User %s logout, login Session ID: %d"%(userInfo['user_name'], userInfo['loginSessionID']))
        loginData = {
            'logout_time'   : GetSystemTime(),
            "result"        : "登出",
        }

        dbHelper = DbHelper()
        db = dbHelper.getDB("system.login_record")
        db.save( loginData, userInfo['loginSessionID'], table = 'system.login_record', key = 'id' )
        dbHelper.closeDB()

        return

LoginRecord = CLoginRecord()
LoginRecord.start()

class Handle(WebRequestHandler):
    @operator_except
    def get(self):

        loginSessionID   = int(self.get_argument('loginSessionID', default = '0'))
        userInfo = self.GetUserInfo()
        userInfo['loginSessionID'] = loginSessionID

        LoginRecord.UserHeartbeat(userInfo)

        self.response(':)')
