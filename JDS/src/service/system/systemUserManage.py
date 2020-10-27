#coding:utf-8

import sys
import time
import datetime

from db import dbMysql
from libs.web.Base  import WebRequestHandler,BaseError,operator_except
from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE 
from libs.utils.debug import *
from libs.utils.utils import *

from system.accountManage import account

class systemUserManage(dbMysql.CURD) :
    def __init__(self,db) :
        if sys.version > '3':
            # python 3.0 +
            super().__init__(db,'system.system_user_list',False) # 定义本实例需要操作的表名
        else :
            # python 2.7
            super(systemUserManage, self).__init__(db,'system.system_user_list',False)

class Restful(WebRequestHandler):

    @operator_except
    def get(self):

        cur  = self.db.getCursor()
        sql = """
            select sul.id, sul.user_name, sul.user_name_en, sul.admin_account_id, sul.admin_group_id, sul.logo_vertical, sul.logo_horizontal, sul.user_desc, sul.user_service,
                sul.abbr_name, sul.abbr_name_en, sul.app_name, sul.app_name_en, ac.user_name
            from system.system_user_list sul 
            inner join system.account ac on ac.id = sul.admin_account_id
            where sul.user_name_en <> 'system' 
            order by id 
            """
        cur.execute(sql)
        rows = cur.fetchall()

        rowdata = {}
        rowdata['struct']  = "id, user_name, user_name_en, admin_account_id, admin_group_id, logo_vertical, logo_horizontal, user_desc, user_service, abbr_name, abbr_name_en, app_name, app_name_en, admin_account_name"
        rowdata['rows'] = rows

        sql  = "select count(*) from system.system_user_list"
        cur.execute(sql)
        row = cur.fetchone() 
        rowdata['count'] = row[0]

        self.response(rowdata)

    @operator_except
    def post(self):

        alldata = self.getRequestData()

        s = systemUserManage(self.db)

        lsData = {
            'user_name'         : 'user_name',
            'user_name_en'      : 'user_name_en',
            'admin_account_id'  : 'admin_account_id',
            'admin_group_id'    : 'admin_group_id',
            'logo_vertical'     : 'logo_vertical', 
            'logo_horizontal'   : 'logo_horizontal',
            'user_desc'         : 'user_desc',
            'user_service'      : 'user_service',
            'abbr_name'         : 'abbr_name',
            'abbr_name_en'      : 'abbr_name_en',
            'app_name'          : 'app_name',
            'app_name_en'       : 'app_name_en',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        line_name = self.getParam( alldata, "user_name", "")
        if line_name == "":
            raise BaseError(801, "参数错误，无系统用户中文名称！")

        line_name_en = self.getParam( alldata, "user_name_en", "")
        if line_name_en == "":
            raise BaseError(801, "参数错误，无系统用户英文名称！")

        db = self.getDB("system.system_user_list")

        # 检查名称是否重复
        result = db.findByCond("user_name", "user_name = '%s' or user_name_en = '%s' "%(data['user_name'], data['user_name_en']))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "参数错误：系统用户名称已经存在！")

        # 创建用户组
        db = self.getDB("system.user_group")
        groupInfo = self.getParam(alldata, 'groupInfo', {})
        groupNameEN = self.getParam(groupInfo, 'name', '')
        groupNameCN = self.getParam(groupInfo, 'c_name', '')
        if groupNameEN == '' or groupNameCN == '':
            raise BaseError(801, '参数错误：无用户组名称！')

        result = db.findByCond("name", "name = '%s'"%groupNameEN)
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "数据错误：用户组名称已经存在！")

        currentTime = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        groupData = {
            'name'              : groupNameEN,
            'c_name'            : groupNameCN,
            'group_type'        : '1', 
            'system_user_id'    : 0,
            'description'       : groupNameCN,
            'create_time'       : currentTime,
            'create_id'         : self.objUserInfo['id'],
        }

        groupID = s.save(groupData, table='system.user_group')

        # 创建用户
        accountInfo = self.getParam(alldata, 'accountInfo', {})
        if not accountInfo:
            raise BaseError(801, '参数错误：无用户信息参数！')

        lsData = {
            'user_name'     : 'user_name',
            'pass'          : 'pass', 
            'name'          : 'name', 
            'country'       : 'country',
            'company'       : 'company',
            'dept_name'     : 'dept_name',
            'description'   : 'description',
            'mobile_phone1' : 'mobile_phone1',
            'mobile_phone2' : 'mobile_phone2', 
            'mailbox'       : 'mailbox',
            'id_no'         : 'id_no',
            'address1'      : 'address1',
            'address2'      : 'address2',
            'photo_path'    : 'photo_path',
            'sex'           : 'sex',
            'default_language'  : 'default_language',
        }

        accountData = {}
        for (k, v) in lsData.items():
            try:
                accountData[k] = accountInfo[v]
            except:
                pass

        accountData['user_type'] = '0'                                  # 用户类型：本单位用户
        accountData['status']    = '2'                                  # 状态：正常

        db = self.getDB("system.account")
        result = db.findByCond("user_name", "user_name = '%s'"%(accountData['user_name']))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "数据错误：用户名已经存在！")

        accountData['create_id']    = self.objUserInfo['id']
        accountData['create_time']  = currentTime

        accountID = s.save(accountData, table = 'system.account')

        # 添加用户组成员
        groupMemberData = {
            'group_id' : groupID,
            'user_id'  : accountID,
        }
        groupMemberID = s.save(groupMemberData, table = 'system.user_group_member')

        # 创建系统用户
        data['create_id']           = self.objUserInfo['id']
        data['create_time']         = currentTime
        data['admin_account_id']    = accountID
        data['admin_group_id']      = groupID

        systemUserID = s.save(data, table = 'system.system_user_list')

        # 更新用户组及用户的system_user_id
        tempData = { 
            'system_user_id' : systemUserID,
        }

        s.save(tempData, groupMemberID, table = 'system.user_group')
        s.save(tempData, groupID,       table = 'system.user_group_member')
        s.save(tempData, accountID,     table = 'system.account')

        # 创建系统用户菜单权限
        menuList = self.getParam(alldata, "menuList", [])
        if len(menuList) == 0:
            raise BaseError(801, "参数错误：没有设置菜单权限！")

        self.SetSystemUserMenu(systemUserID, groupID, menuList)

        # 添加用户组菜单数据

        logInfo = "创建系统用户：%s，%s"%(data['user_name'], data['user_name_en'])
        operation_log(self.db).addLog(self.GetUserInfo(), "systemUserManage", logInfo, systemUserID)

        self.response(systemUserID)

    @operator_except
    def put(self):

        alldata = self.getRequestData()
        if self.getParam( alldata, "id", 0) == 0:
            raise BaseError(801, "参数错误：缺少待更新系统用户的ID！")

        line_name = self.getParam( alldata, "user_name", "")
        if line_name == "":
            raise BaseError(801, "参数错误，无系统用户中文名称！")

        line_name_en = self.getParam( alldata, "user_name_en", "")
        if line_name_en == "":
            raise BaseError(801, "参数错误，无系统用户英文名称！")

        s = systemUserManage(self.db)
        
        lsData = {
            'id'                : 'id',
            'user_name'         : 'user_name',
            'user_name_en'      : 'user_name_en',
            'logo_vertical'     : 'logo_vertical', 
            'logo_horizontal'   : 'logo_horizontal',
            'admin_group_id'    : 'admin_group_id',
            'admin_account_id'  : 'admin_account_id',
            'user_service'      : 'user_service',
            'abbr_name'         : 'abbr_name',
            'abbr_name_en'      : 'abbr_name_en',
            'app_name'          : 'app_name',
            'app_name_en'       : 'app_name_en',
            'user_desc'         : 'user_desc',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        # 检查名称是否重复
        db = self.getDB("system.system_user_list")
        result = db.findByCond("user_name", "( user_name = '%s' or user_name_en = '%s' ) and id <> %s "%(data['user_name'], data['user_name_en'], data['id']) )
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "数据错误：系统用户名称重复！")

        data['update_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        data['update_id']   = self.objUserInfo['id']

        id = s.save(data, data['id'], table = 'system.system_user_list')

        # 检查是否需要更新密码
        accountInfo = self.getParam(alldata, 'accountInfo', {})
        if not accountInfo:
            raise BaseError(801, '参数错误：无用户信息参数！')

        # 只更新管理员密码，其他管理员信息由管理员自己更新
        if 'id' in accountInfo and 'pass' in accountInfo:
            # 更新用户密码
            accountData = {
                'id'    : accountInfo['id'],
                'pass'  : accountInfo['pass'],
            }
            s.save(accountData, accountInfo['id'], table = 'system.account')

        # 更新菜单权限
        menuList = self.getParam(alldata, "menuList", [])
        if len(menuList) == 0:
            raise BaseError(801, "参数错误：没有设置菜单权限！")

        self.SetSystemUserMenu(data['id'], data['admin_group_id'], menuList)

        logInfo = "更新系统用户信息：%s，%s"%(data['user_name'], data['user_name_en'])

        operation_log(self.db).addLog(self.GetUserInfo(), "systemUserManage", logInfo, id)

        self.response(id)

    # 设置系统用户的菜单权限
    def SetSystemUserMenu(self, systemUserID, groupID, menuList):
        
        if systemUserID == 0 or groupID == 0 or len(menuList) == 0:
            raise BaseError(801, "参数错误：设置菜单权限时参数错误！")

        s = systemUserManage(self.db)

        # 删除现有系统用户菜单数据，不管是新建还是更新模式
        db = self.getDB("system.system_user_menu")
        db.deleteByCond("system_user_id = %d"%systemUserID)

        # 删除系统用户的管理员用户组菜单数据
        db = self.getDB("system.user_group_menu")
        db.deleteByCond("group_id = %d"%groupID)

        db = self.getDB("system.menu_item")
        for menuID in menuList:
            # 添加系统用户的菜单权限
            tempData = {
                'create_time'   : datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
                'create_id'     : self.objUserInfo['id'],
                'system_user_id': systemUserID,
                'menu_id'       : menuID,
            }
            s.save( tempData, table = 'system.system_user_menu')

            # 添加系统用户管理员用户组的菜单项权限数据

            # 查找菜单ID对应的菜单项ID，当前模式中，一个菜单只有一个菜单项
            result = db.findByCond('id', "menu_id = %d"%menuID)
            if len( result['rows'] ) == 0 :
                raise BaseError(801, "数据错误：菜单 %d 无对应的菜单项！"%menuID)

            keys = result["struct"].split(",")
            menuItemIDList = []
            for item in result['rows']:
                data = dict(zip(keys, item))
                menuItemIDList.append(data)

            data = {
                'create_time'   : datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
                'create_id'     : self.objUserInfo['id'],
                'group_id'      : groupID,
                'menu_item_id'  : menuItemIDList[0]['id'],
                'system_user_id': systemUserID,
            }

            s.save(data, table = 'system.user_group_menu')

            # 在更新系统用户菜单权限时，需要更新现有系统用户创建的用户组的菜单权限：
            #   原来有的菜单权限，现在没有了，需要删除；
            #   原来没有的，现在有的菜单权限，不需要添加，由系统用户的管理员进行设置；

        return


    @operator_except
    def delete(self):

        alldata = self.getRequestData()
        systemUserID = self.getParam( alldata, "id", 0)
        if systemUserID == 0:
            raise BaseError(801, "参数错误：缺少待删除的系统用户ID！")

        db = self.getDB("system.system_user_list")
        result = db.findByCond('user_name,user_name_en', "id = %d"%systemUserID)
        if len(result['rows']) == 0:
            raise BaseError(801, "参数错误：无法找到系统用户：%d"%systemUserID)

        rows = result['rows']
        logInfo = "删除系统用户：%s, %s"%(rows[0][0], rows[0][1])

        # 删除用户组成员数据
        db = self.getDB("system.user_group_member")
        db.deleteByCond('group_id in (select id from system.user_group where system_user_id = %d)'%systemUserID)
        
        # 删除用户组菜单数据
        db = self.getDB("system.user_group_menu")
        db.deleteByCond('group_id in (select id from system.user_group where system_user_id = %d)'%systemUserID)

        # 删除用户数据
        db = self.getDB("system.account")
        db.deleteByCond('system_user_id = %d'%systemUserID)

        # 删除用户组数据
        db = self.getDB("system.user_group")
        db.deleteByCond('system_user_id = %d'%systemUserID)
        
        # 删除日志记录数据
        db = self.getDB("system.operation_log")
        db.deleteByCond('system_user_id = %d'%systemUserID)

        # 删除系统用户菜单数据
        db = self.getDB("system.system_user_menu")
        db.deleteByCond('system_user_id = %d'%systemUserID)

        # 删除记录
        s = systemUserManage(self.db)
        s.remove( systemUserID, table = "system.system_user_list", key = "id", delete = True)

        # 记录日志并退出
        operation_log(self.db).addLog(self.GetUserInfo(), "systemUserManage", logInfo, systemUserID)
        self.response(systemUserID)

    @operator_except
    def patch(self):

        paramData = self.getRequestData()

        systemUserID = int(self.getParam(paramData, "sid", "0"))

        op  = self.getParam(paramData, "op", "")
        cur = self.db.getCursor()

        if op == 'menu':
            # 获取所有菜单
            sql  = " select sm.id, sm.code, sm.name, sm.sort, mi.id"
            sql += " from system.menu sm "
            sql += " left join system.menu_item mi on mi.menu_id = sm.id"
            sql += " where sm.tier = 0 and sm.code <> 'systemConfig'"
            sql += " order by sort"

            cur.execute(sql)
            rows = cur.fetchall()

            menuRoot = self.tupleToList(rows)

            for menu in menuRoot:
                subMenu = self.getSubMenu(menu, systemUserID)

            # 清洗空的父菜单
            temp = []
            for obj in menuRoot:
                if obj['children']:
                    temp.append(obj)

                    # lic = obj['children']
                    # tempc = []
                    # for objc in lic:
                    #     if objc['isItem'] or objc['children']:
                    #         tempc.append(objc)  
                    
                    # if tempc:
                    #     obj['children'] = tempc
                    #     temp.append(obj)

            menuRoot = temp

            self.response(menuRoot)

        elif op == 'ust':
            # 获取用户业务类型
            sql = """
                select cv.code, cv.name
                    from system.code_value cv
                    where cv.type_code = 'USER_SERVICE_TYPE'
                    order by cv.sort
            """

            cur = self.db.getCursor()
            cur.execute(sql)
            rows = cur.fetchall()

            userServiceList = {}
            userServiceList['struct']   = "code, name"
            userServiceList['rows']     = [('0', "选择全部", )] + rows

            allData = {}
            allData['userServiceList']     = userServiceList
            self.response(allData)
        else:
            self.response(0)

    # 获取指定菜单的下一级菜单及菜单项
    # 参数：
    #   rootMenuID：指定的菜单ID
    #   gid：归属的用户组
    # 返回值：数组，
    def getSubMenu(self, rootMenu, systemUserID):
        
        if rootMenu['id'] == 0:
            return None

        cur  = self.db.getCursor()
        sql  = "select sm.id, sm.code, sm.name, sm.sort, mi.id "
        sql += " from system.menu sm"
        sql += " left join system.menu_item mi on mi.menu_id = sm.id " 
        sql += " where parent_id=%d" % rootMenu['id']
        sql += " order by sort"

        cur.execute(sql)
        rows = cur.fetchall()
        if len(rows) == 0:
            # 该菜单无子菜单
            # rootMenu['chkDisabled'] = False
            return None

        menuList = self.tupleToList(rows)
        rootMenu['children'] = menuList
        for menu in menuList:
            subMenu = self.getSubMenu(menu, systemUserID)
            if subMenu == None:
                # 无子菜单
                # 检查该菜单是否已经在系统用户菜单组里面
                menu['chkDisabled'] = False
                sql = "select sm.id from system.system_user_menu sm where sm.menu_id = %d and sm.system_user_id = %d"%(menu['id'], systemUserID)

                cur.execute(sql)
                rows = cur.fetchall()
                if len(rows) > 0:
                    # 该系统用户组已经具有此菜单项权限
                    menu['checked'] = True
                else:
                    # 该系统用户无此菜单项权限
                    menu['checked'] = False

            else:
                # 该菜单具有子菜单
                pass

        return menuList

    # 元组转换为对象
    def tupleToList(self, rows):
        objList = []
        for row in rows:
            obj = {}
            obj['id']           = row[0]
            obj['menu_id']      = row[0]
            obj['code']         = row[1]
            obj['name']         = row[2]
            obj['sort']         = row[3]
            obj['menu_item_id'] = row[4]
            obj['children']     = []
            obj['chkDisabled']  = True
            obj['open']         = True
            obj['checked']      = False
            objList.append(obj)

        return objList
