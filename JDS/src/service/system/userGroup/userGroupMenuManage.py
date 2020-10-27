#coding:utf-8

import sys
import time
import datetime

from db import dbMysql
from libs.web.Base  import WebRequestHandler,BaseError,operator_except
from libs.utils.debug import *
from system.accountManage import account

class userGroupMenuManage(dbMysql.CURD) :
    def __init__(self,db) :
        if sys.version > '3':
            # python 3.0 +
            super().__init__(db,'system.user_group_menu',False) # 定义本实例需要操作的表名
        else :
            # python 2.7
            super(userGroupManage, self).__init__(db,'system.user_group_menu',False)


class Restful(WebRequestHandler):

    # 查询所有或指定用户组的菜单项数据
    # 参数：
    #   id:  操作用户的ID
    #   gid: 可选，指定用户组信息，如果没有则查询所有菜单项信息
    #   op： 操作类型，必选，gm：获取用户组自己的菜单，am：获取所有菜单
    @operator_except
    def get(self):

        userInfo = self.GetUserInfo()
        systemUserID = userInfo['system_user_id']
        gid = int(self.get_argument('gid', default = '0'))
        op  = self.get_argument('op', default ='')
        
        cur  = self.db.getCursor()
        sql = """
            select id, code, name, sort from system.menu 
            where tier = 0 
            order by sort
            """

        cur.execute(sql)
        rows = cur.fetchall()

        menuRoot = self.tupleToList(rows)
        for menu in menuRoot:
            # logI("检查菜单：", menu)
            subMenu = self.getSubMenu(menu, gid, systemUserID)

        # 清洗空的父菜单
        temp = []
        for obj in menuRoot:
            if obj['children']:
                lic = obj['children']
                tempc = []
                for objc in lic:
                    if objc['isItem'] or objc['children']:
                        tempc.append(objc)  
                
                if tempc:
                    obj['children'] = tempc
                    temp.append(obj)

        menuRoot = temp

        # 只取用户组有的菜单项
        if op == "gm":
            data1 = menuRoot
            data = []
            for l1 in data1:
                if l1['children'] == [] and l1['checked'] == False:
                    pass
                elif l1['children'] == [] and l1['checked'] == True:
                    data.append(l1)
                else: 
                    nl2 = []
                    for l2 in l1['children']:
                        if l2['children'] == [] and l2['checked'] == False:
                            pass
                        elif l2['children'] == [] and l2['checked'] == True:
                            nl2.append(l2)
                        else:
                            nl3 = []
                            for l3 in l2['children']:
                                if l3['children'] == [] and l3['checked'] == False:
                                    pass
                                elif l3['children'] == [] and l3['checked'] == True:
                                    nl3.append(l3)
                            if nl3 == []:
                                pass
                            else:
                                l2['children'] = nl3
                                nl2.append(l2)
                    if nl2 == []:
                        pass
                    else:
                        l1['children'] = nl2
                        data.append(l1)

            menuRoot = data

        self.response(menuRoot)

    # 获取指定菜单的下一级菜单及菜单项
    # 参数：
    #   rootMenuID：指定的菜单ID
    #   gid：归属的用户组
    # 返回值：数组，
    def getSubMenu(self, rootMenu, gid, systemUserID):
        
        if rootMenu['id'] == 0:
            return None

        cur  = self.db.getCursor()
        # sql = "select id, code, name, sort from system.menu where parent_id=%d order by sort"%rootMenu['id']
        sql = """
            select id, code, name, sort from system.menu 
            where parent_id=%d
            and id in (select menu_id from system.system_user_menu sm where sm.system_user_id = %d) 
            order by sort
            """%(rootMenu['id'], systemUserID)

        cur.execute(sql)
        rows = cur.fetchall()
        if len(rows) == 0:
            # 该菜单无子菜单
            rootMenu['chkDisabled'] = False
            return None

        menuList = self.tupleToList(rows)
        rootMenu['children'] = menuList
        for menu in menuList:
            subMenu = self.getSubMenu(menu, gid, systemUserID)
            if subMenu == None:
                # 无子菜单
                # 检查是否有菜单项
                sql = "select mi.id from system.menu_item mi where mi.menu_id = %d"%menu['id']
                cur.execute(sql)
                rows = cur.fetchall()
                if len(rows) == 0:
                    # 无菜单项
                    continue

                menu['id'] = rows[0][0]
                menu['isItem'] = True

                # 检查该用户组是否已经有此菜单项权限
                sql = "select * from system.user_group_menu ugm where ugm.group_id = %d and ugm.menu_item_id = %d"%(gid, rows[0][0])
                cur.execute(sql)
                rows = cur.fetchall()
                if len(rows) > 0:
                    # 该用户组已经具有此菜单项权限
                    menu['checked'] = True
                else:
                    # 该用户组无此菜单项权限
                    menu['checked'] = False

            else:
                # 该菜单具有子菜单
                menu['isItem'] = False

        return menuList

    # 元组转换为对象
    def tupleToList(self, rows):
        objList = []
        for row in rows:
            obj = {}
            obj['id']       = row[0]
            obj['menu_id']  = row[0]
            obj['code']     = row[1]
            obj['name']     = row[2]
            obj['sort']     = row[3]
            obj['children'] = []
            obj['chkDisabled'] = True
            obj['open']     = True
            obj['checked']  = False
            obj['isItem']   = False
            objList.append(obj)

        return objList


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
            sql += " where ug.name != 'supervisors' and ug.system_user_id = %d"%userInfo['system_user_id']
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
            uname   = self.getParam(paramData, 'un', None) 
            offset  = self.getParam(paramData, 'o', 0)
            rowlimit= self.getParam(paramData, 'r', 20)

            accountInfo = account.getAccountInfo(self, 0, uname, groupID, None, None, offset, rowlimit )
            self.response(accountInfo)

