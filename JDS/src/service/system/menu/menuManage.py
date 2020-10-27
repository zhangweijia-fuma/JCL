#coding:utf-8

import sys
import time
import datetime
from libs.web.Base  import WebRequestHandler,BaseError,operator_except
from libs.db import dbMysql
from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE
from libs.utils.debug import *


class menuManage(dbMysql.CURD) :
    def __init__(self,db) :
        if sys.version > '3':
            # python 3.0 +
            super().__init__(db,'system.menu', False) # 定义本实例需要操作的表名
        else :
            # python 2.7
            super(menuManage,self).__init__(db,'system.menu', False)

class Restful(WebRequestHandler):
    # 获取菜单树形列表

    @operator_except
    def get(self):
        # 获取所有以及目录

        sql  = "select m.*, a.name create_name,a2.name update_name "
        sql += " from ( select m1.*, m2.name parent_name "
        sql += "    from system.menu m1 "
        sql += "    left join system.menu m2 on m1.parent_id=m2.id) m "
        sql += " left join system.account a on m.create_id=a.id "
        sql += " left join system.account a2 on m.update_id=a2.id "
        sql += " where m.tier=0 "
        sql += " order by m.sort"

        cur=self.db.getCursor()
        cur.execute(sql)
        rows = cur.fetchall()

        menuTree = {}
        menuTree['name']        = '菜单系统'
        menuTree['id']          = 0
        menuTree['chkDisabled'] = False
        menuTree['open']        = True
        menuTree['checked']     = False

        #将所有元组转为对象, 因为前台tree组件无法加载元组类型
        li = self.tupleToList(rows)

        menuTree['children']    = li

        # 分组菜单
        li = self.getChildren(cur,li,1)
        self.response(menuTree)

    def getChildren(self, cur, li, tier):

        if len(li) == 0:
            return li

        sql  = "select m.*,a.name create_name,a2.name update_name "
        sql += " from (select m1.*,m2.name parent_name "
        sql += "        from system.menu m1 "
        sql += "        left join system.menu m2 on m1.parent_id=m2.id) m"
        sql += " left join system.account a on m.create_id=a.id  "
        sql += " left join system.account a2 on m.update_id=a2.id "
        sql += " where m.tier=%s and m.parent_id=%s "
        sql += " order by m.sort"

        for obj in li:
            sqls = sql%(tier, obj['id'])
            cur.execute(sqls);
            rows = cur.fetchall()
            cli = self.tupleToList(rows)
            obj['children'] = self.getChildren(cur, cli, tier + 1)

        return li

    # 创建菜单
    # 参数：
    #   code: 菜单编码
    #   name：菜单名称
    #   sort：排序
    #   tier：层级，顶层为0
    #   parent_id：上级菜单ID
    #   parent_code：上级菜单编码
    @operator_except
    def post(self):

        alldata = self.getRequestData()

        lsData = {
            'code'          : 'code',
            'name'          : 'name',
            'sort'          : 'sort',
            'tier'          : 'tier',
            'parent_code'   : 'parent_code',
            'parent_id'     : 'parent_id',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        if 'code' not in data or 'name' not in data or 'sort' not in data or 'tier' not in data or 'parent_id' not in data or 'parent_code' not in data:
            raise BaseError(801, "参数错误：缺少菜单数据！")

        db = self.getDB("system.menu")

        # 检查菜单编码是否重复
        result = db.findByCond("code", "code = '%s' "%(data['code']))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "数据错误：菜单编号 %s！"%(data['code']))

        data['create_id']   = self.objUserInfo['id']
        data['create_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')

        s = menuManage(self.db)
        id = s.save(data, table = "system.menu")
        if id <= 0:
            raise BaseError(703)  #参数错误

        operation_log(self.db).addLog(self.GetUserInfo(), "menuManage", "创建菜单-%s"%data['name'], id)

        self.response(id)

    @operator_except
    def put(self):

        alldata = self.getRequestData()

        lsData = {
            'id'            : 'id',
            'code'          : 'code',
            'name'          : 'name',
            'sort'          : 'sort',
            'tier'          : 'tier',
            'parent_code'   : 'parent_code',
            'parent_id'     : 'parent_id',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        if 'id' not in data or 'code' not in data or 'name' not in data or 'sort' not in data or 'tier' not in data or 'parent_id' not in data or 'parent_code' not in data:
            raise BaseError(801, "参数错误：缺少菜单数据！")

        db = self.getDB("system.menu")

        # 检查菜单编码是否重复
        result = db.findByCond("code", "code = '%s' and id <> %d"%((data['code'], data['id'])))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "数据错误：菜单编号 %s！"%(data['code']))

        data['update_id']   = self.objUserInfo['id']
        data['update_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')

        s = menuManage(self.db)
        id = s.save(data, data['id'], table = "system.menu", key = 'id')
        operation_log(self.db).addLog(self.GetUserInfo(), "menuManage", "创建菜单-%s"%data['name'], id)

        self.response(id)

    # 删除菜单
    @operator_except
    def delete(self):

        alldata = self.getRequestData()
        if 'id' not in alldata or alldata['id'] < 0:
            raise BaseError(801)

        self.deleteSubMenu(alldata['id'])

        operation_log(self.db).addLog(self.GetUserInfo(), "menuManage", "删除菜单-%s"%alldata['name'], alldata['id'])
        self.response(0)

    # 删除子菜单，含该menuID
    def deleteSubMenu(self, menuID):

        db = self.getDB("system.menu")
        result = db.findByCond("id", "parent_id = %d "%menuID)
        if len( result['rows'] ) == 0 :
            # 此菜单ID无子菜单
            self.deleteMenu(menuID)
            return

        for subMenuID in result['rows']:
            self.deleteSubMenu(subMenuID[0])

        self.deleteMenu(menuID)
        return

    def deleteMenu(self, menuID):
        if menuID is None or menuID < 0:
            return

        logI("删除菜单项及菜单: menuID: %d"%menuID)

        # 删除此菜单的菜单项
        db = self.getDB("system.menu_item")
        db.deleteByCond("menu_id = %d"%menuID)

        # 删除此菜单
        db = self.getDB("system.menu")
        db.deleteByCond("id = %d"%menuID)

    def tupleToList(self, rows):
        li = []
        for row in rows:
            obj={}
            obj['id']           = row[0]
            obj['create_time']  = row[1]
            obj['update_time']  = row[2]
            obj['create_id']    = row[3]
            obj['update_id']    = row[4]
            obj['code']         = row[5]
            obj['name']         = row[6]
            obj['sort']         = row[7]
            obj['tier']         = row[8]
            obj['parent_id']    = row[9]
            obj['parent_code']  = row[10]
            obj['parent_name']  = row[11]
            obj['create_name']  = row[12]
            obj['update_name']  = row[13]

            obj['chkDisabled']  = True
            obj['open']         = True
            obj['checked']      = False

            obj['children']     = []
            li.append(obj)
        return li





