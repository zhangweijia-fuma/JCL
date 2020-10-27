#coding:utf-8

import sys
import time
import datetime

from libs.db import dbMysql
from libs.web.baseException import errorDic,BaseError
from libs.web.Base  import WebRequestHandler,BaseError,operator_except

from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE
from libs.utils.debug import *


class MenuItem(dbMysql.CURD) :
    def __init__(self,db) :
        if sys.version > '3':
            # python 3.0 +
            super().__init__(db,'system.menu_item',False) # 定义本实例需要操作的表名
        else :
            # python 2.7
            super(MenuItem,self).__init__(db,'system.menu_item',False)

class Restful(WebRequestHandler):

    # 获取菜单项
    # 参数：
    #   mid: 菜单ID(system.menu表)，必选
    @operator_except
    def get(self):

        menuID = self.get_argument("mid", default='')
        if menuID =='':
            raise BaseError(801)

        sql = """
            select mi.*, ac.name, m.name  
            from system.menu_item mi 
            left join system.account ac on mi.create_id = ac.id 
            inner join system.menu m on m.id = mi.menu_id 
            where mi.menu_id = %s
            """%menuID
        
        cur = self.db.getCursor()
        cur.execute(sql)
        rows = cur.fetchall()

        rowdata={}
        rowdata['rows']     = rows;
        rowdata['struct']   = "id, create_time, update_time, create_id, update_id, menu_code, menu_id, path, create_name, menu_name"

        self.response(rowdata)

    # 创建菜单项
    # 参数：
    #   menu_code: 菜单编码
    #   menu_id：菜单ID
    #   path：路径
    #   所有参数必选
    @operator_except
    def post(self):
        alldata = self.getRequestData()

        lsData = {
            'menu_code' : 'menu_code',
            'menu_id'   : 'menu_id',
            'path'      : 'path'
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        if 'menu_code' not in data or 'menu_id' not in data or 'path' not in data:
            raise BaseError(801, "参数错误：缺少菜单项数据！")

        db = self.getDB("system.menu")

        # 检查菜单是否存在
        result = db.findByCond("code", "id = %d"%(data['menu_id']))
        if len( result['rows'] ) == 0 :
            raise BaseError(801, "数据错误：菜单 %s 不存在！"%(data['code']))


        # 检查菜单是否已经有菜单项
        db = self.getDB("system.menu_item")

        result = db.findByCond("id", "menu_code = '%s' "%(data['menu_code']))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "数据错误：菜单编号 %s 已经有菜单项！当前模式下，一个菜单只能有一个菜单项。"%(data['code']))

        data['create_id']   = self.objUserInfo['id']
        data['create_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')

        s = MenuItem(self.db)
        id = s.save(data, table = "system.menu_item")
        operation_log(self.db).addLog(self.GetUserInfo(), 
            "menuItemManage", "创建菜单项: code: %s, path: %s"%(data['menu_code'], data['path']), id)

        self.response(id)


    # 更新菜单项
    @operator_except
    def put(self):
        alldata = self.getRequestData()

        # 更新的时候只能更新路径
        lsData = {
            'id'        : 'id',
            'menu_code' : 'menu_code',
            'path'      : 'path'
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        if 'id' not in data or 'path' not in data or 'menu_code' not in data:
            raise BaseError(801, "参数错误：缺少菜单项数据！")

        data['update_id']   = self.objUserInfo['id']
        data['update_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')

        s = MenuItem(self.db)
        id = s.save(data, data['id'], table = "system.menu_item")

        operation_log(self.db).addLog(self.GetUserInfo(), 
            "menuItemManage", "更新菜单项: code: %s, path: %s"%( data['menu_code'], data['path']), id)

        self.response(id)


    # 删除菜单项
    # 参数：
    #   id : 菜单项ID 
    @operator_except
    def delete(self):
        alldata = self.getRequestData()
        e = MenuItem(self.db)

        if 'id' not in alldata:
            raise BaseError(801, "参数错误：缺少待删除的菜单项ID")

        sql = " select mi.menu_code, mi.path from system.menu_item mi "
        sql += " where mi.id = %s"%alldata['id']

        cur = self.db.getCursor()
        cur.execute(sql)
        rows = cur.fetchall()
        key  = ['menu_code', 'path']
        data = dict(zip(key, rows[0]))

        r = e.remove( alldata['id'], table = 'system.menu_item', delete=True)

        operation_log(self.db).addLog(self.GetUserInfo(), "menuItemManage", "删除菜单项：code: %s, path: %s"%( data['menu_code'], data['path']), r)
        self.response(r)
