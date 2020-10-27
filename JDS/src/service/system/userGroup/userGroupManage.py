#coding:utf-8

import sys
import time
import datetime

from db import dbMysql

from libs.web.Base  import WebRequestHandler,BaseError,operator_except
from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE 
from libs.utils import debug

from system.accountManage import account

class userGroupManage(dbMysql.CURD) :
    def __init__(self,db) :
        if sys.version > '3':
            # python 3.0 +
            super().__init__(db,'system.user_group',False) # 定义本实例需要操作的表名
        else :
            # python 2.7
            super(userGroupManage, self).__init__(db,'system.user_group',False)


class Restful(WebRequestHandler):

    # 查询所有或指定用户组的数据
    # id: 操作用户的ID
    # gid: 可选，指定用户组信息，如果没有则查询所有用户组信息
    @operator_except
    def get(self):

        if not self.objUserInfo :
            raise BaseError(604)

        userInfo = self.GetUserInfo()

        opUserID        = int(self.get_argument('id',  default='0'))
        queryGroupID    = int(self.get_argument('gid', default='0'))

        offset          = int(self.get_argument('o',  default='1'))
        rowlimit        = int(self.get_argument('r',  default='20'))
        offset          = ( offset - 1 ) * rowlimit

        rowdata = {}
        cur = self.db.getCursor()

        sql  = "select ug.id, ug.create_time, ug.update_time, ug.create_id, ug.update_id, ug.name, ug.c_name, "
        sql += " ug.description, ug.group_type"
        sql += " from system.user_group ug "
        
        sql_where = " where ug.name != 'supervisors'"

        if userInfo['system_user_id'] != 1:
            sql_where += " and ug.system_user_id = %d "%userInfo['system_user_id']

        if queryGroupID > 0 :
            sql_where += " and ug.id = %d"%queryGroupID

        sql += sql_where
        sql += " order by ug.c_name"
        sql += " limit " + str(rowlimit) + " offset " + str(offset)

        cur.execute(sql)
        rows = cur.fetchall()

        rowdata['struct']  = "id, create_time, update_time, create_id, update_id, name, c_name, "
        rowdata['struct'] += "  description, group_type"
        rowdata['rows']    = rows

        sql  = "select count(*) from system.user_group ug  "
        sql += sql_where

        cur.execute(sql)
        row = cur.fetchone() 
        rowdata['count'] = row[0]

        self.response(rowdata)
        

    # 创建一个用户组
    # 参数：
    #   id: 可选，存在表示是对现有用户组的更新，无则表示为新建用户组
    #   name: 必选，用户组名称，在更新时不能修改
    #   c_name: 必选，用户组中文名称
    #   group_type: 必选，用户组类型，0：系统用户组，不显示，1：不可删除，2：可删除，缺省为2
    #   descritpion: 可选，用户组描述，可为空
    #   member: 必选，用户组成员列表，每个列表元素为一个账户信息
    #   menu : 必选，用户组菜单项列表，每个列表元素为一个菜单项ID
    @operator_except
    def post(self):
        alldata = self.getRequestData()
        s = userGroupManage(self.db)
        
        # 检查参数
        memberList = self.getParam(alldata, 'member', None )
        if memberList == None:
            raise BaseError(801, "无用户组成员列表")

        menuList = self.getParam(alldata, 'menu', None )
        if menuList == None:
            raise BaseError(801, "无用户组菜单项列表")

        lsData = {
            'id'          : 'id', 
            'name'        : 'name',
            'c_name'      : 'c_name', 
            'group_type'  : 'group_type',
            'description' : 'description',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        cur  = self.db.getCursor()
        
        # 是新建用户组操作还是更新用户组操作
        gid = 0
        logStr = ""
        if 'id' in data: 
            # 更新用户组
            gid = int(data['id'])

            id = s.save(data, gid, table = 'system.user_group')

            # 删除现有用户组的成员
            sql  = "delete from system.user_group_member ugm "
            sql += " where ugm.group_id = %d"%gid
            cur.execute(sql)

            # 删除现有用户组的菜单项
            sql  = "delete from system.user_group_menu ugm "
            sql += " where ugm.group_id = %d"%gid
            cur.execute(sql)

            logStr = "更新了用户组"
            if "c_name" in data:
                logStr += data['c_name']
            elif "name" in data:
                logStr += data['name']

        else:
            # 新建用户组
            
            # 检查用户组是否已经存在
            db = self.getDB("system.user_group")
            result = db.findByCond("name", "name = '%s' and system_user_id = %d "%(data['name'], self.GetSystemUserID()))
            if len( result['rows'] ) > 0 :
                raise BaseError(911)

            # 创建的新用户组和创建者属于同一业务类型
            userInfo = self.GetUserInfo()
            data['system_user_id']  = userInfo['system_user_id']
            data['create_id']       = self.objUserInfo['id']
            data['create_time']     = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            
            gid = s.save(data, table = 'system.user_group')
            logStr = "创建了用户组%s"%data['c_name']

        # 添加用户组成员
        ugmData = {
            'create_id'     : self.objUserInfo['id'],
            'create_time'   : datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
            'group_id'      : gid,
            'system_user_id': self.GetSystemUserID(),
        }

        db = self.getDB("system.user_group_member")
        for ugmID in memberList:
            ugmData['user_id'] = ugmID['id']
            id = s.save(ugmData, table = 'system.user_group_member')

        # 添加用户组菜单
        if 'user_id' in ugmData:
            del(ugmData['user_id'])
            
        for ugmID in menuList:
            ugmData['menu_item_id'] = ugmID          
            id = s.save(ugmData, table = 'system.user_group_menu')

        # 操作成功，记录操作日志并返回
        operation_log(self.db).addLog(self.GetUserInfo(), 'groupManage', logStr, gid )
        self.response(gid)

    @operator_except
    def put(self):
        alldata = self.getRequestData()
        s = userGroupManage(self.db)
        
        # 检查参数
        memberList = self.getParam(alldata, 'member', None )
        if memberList == None:
            raise BaseError(801, "无用户组成员列表")

        menuList = self.getParam(alldata, 'menu', None )
        if menuList == None:
            raise BaseError(801, "无用户组菜单项列表")

        lsData = {
            'id'          : 'id', 
            'name'        : 'name',
            'c_name'      : 'c_name', 
            'group_type'  : 'group_type',
            'description' : 'description',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        cur  = self.db.getCursor()
        
        # 是新建用户组操作还是更新用户组操作
        gid = 0
        logStr = ""
        if 'id' in data: 
            # 更新用户组
            gid = int(data['id'])

            id = s.save(data, gid, table = 'system.user_group')

            # 删除现有用户组的成员
            sql  = "delete from system.user_group_member ugm "
            sql += " where ugm.group_id = %d"%gid
            cur.execute(sql)

            # 删除现有用户组的菜单项
            sql  = "delete from system.user_group_menu ugm "
            sql += " where ugm.group_id = %d"%gid
            cur.execute(sql)

            logStr = "更新了用户组"
            if "c_name" in data:
                logStr += data['c_name']
            elif "name" in data:
                logStr += data['name']

        else:
            # 新建用户组
            
            # # 检查用户组是否已经存在
            # db = self.getDB("system.user_group")
            # result = db.findByCond("name", "name = '%s' "%data['name'])
            # if len( result['rows'] ) > 0 :
            #     raise BaseError(911)

            # 检查用户组是否已经存在
            db = self.getDB("system.user_group")
            result = db.findByCond("name", "name = '%s' and system_user_id = %d "%(data['name'], self.GetSystemUserID()))
            if len( result['rows'] ) > 0 :
                raise BaseError(911, "用户组 %d 已经存在"%data['name'])

            # 创建的新用户组和创建者属于同一业务类型
            userInfo = self.getUserInfo()
            data['system_user_id'] = userInfo['system_user_id']

            data['create_id']   = self.objUserInfo['id']
            data['create_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            
            gid = s.save(data, table = 'system.user_group')
            logStr = "创建了用户组%s"%data['c_name']

        # 添加用户组成员
        ugmData = {
            'create_id'     : self.objUserInfo['id'],
            'create_time'   : datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
            'group_id'      : gid,
            'system_user_id': self.GetSystemUserID(),
        }

        db = self.getDB("system.user_group_member")
        for ugmID in memberList:
            ugmData['user_id'] = ugmID['id']
            id = s.save(ugmData, table = 'system.user_group_member')

        # 添加用户组菜单
        if ( "user_id" in ugmData):
            del(ugmData['user_id'])
            
        for ugmID in menuList:
            ugmData['menu_item_id'] = ugmID          
            id = s.save(ugmData, table = 'system.user_group_menu')

        # 操作成功，记录操作日志并返回
        operation_log(self.db).addLog(self.GetUserInfo(), 'groupManage', logStr, gid )
        self.response(gid)

    # 删除用户组
    # 参数：
    #   gid：待删除的用户组ID
    #   gn: 用户组中文名称
    @operator_except
    def delete(self):
        paramData = self.getRequestData()
        groupID   = self.getParam(paramData, 'gid', 0) 
        groupName = self.getParam(paramData, 'gn', "")
        if groupID == 0 or groupName == "":
            raise BaseError(801, "参数错误，没有指定需要删除的用户组")

        s = userGroupManage(self.db)
        r = s.remove( groupID, table = 'system.user_group', delete = True)

        s.remove( groupID, table = "system.user_group_menu",   key = "group_id", delete = True)
        s.remove( groupID, table = "system.user_group_member", key = "group_id", delete = True)
        
        # 操作成功，记录日志并返回
        operation_log(self.db).addLog(self.GetUserInfo(), 'groupManage', "删除了一个用户组：%s"% groupName, groupID )
        self.response(r)


    # 获取用户组相关信息
    # 参数: 
    #   op: 操作类型，必选
    #   gl: 获取用户组清单，
    #   gm: 获取用户组成员
    #   gid: 指定用户组
    #   un：根据用户姓名查找
    @operator_except
    def patch(self):
        paramData = self.getRequestData()
        if 'op' not in paramData:
            raise BaseError(900)

        # 获取用户组清单
        if paramData['op'] == 'gl':
            userInfo = self.GetUserInfo()

            sql  = " select ug.id, ug.c_name from system.user_group ug"
            sql += " where ug.name != 'supervisors' and ug.system_user_id = %d "%userInfo['system_user_id']
            sql += " order by ug.c_name"
            
            rowdata = {}
            cur = self.db.getCursor()

            cur.execute(sql)
            rows = cur.fetchall()

            groupList = {}
            groupList['struct']   = "id, name"
            groupList['rows']     = [(0, "选择全部", )] + rows

            self.response(groupList)

        # 获取用户组成员
        elif paramData['op'] == 'gm':

            groupID = self.getParam(paramData, 'gid', 0) 
            uname   = self.getParam(paramData, 'un', "") 
            offset  = self.getParam(paramData, 'o', 0)
            rowlimit= self.getParam(paramData, 'r', 20)

            accountInfo = account.getAccountInfo(self, 0, uname, groupID, None, None, offset, rowlimit )

            self.response(accountInfo)

