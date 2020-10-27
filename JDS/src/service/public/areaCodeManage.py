#coding:utf-8

import sys
import time
import datetime

from libs.db.dbHelper import *
from libs.web.Base  import WebRequestHandler, BaseError, operator_except
from libs.db import dbMysql
from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE

from libs.utils.debug import *
from libs.utils.utils import *

class areaCodeManage(dbMysql.CURD) :
    def __init__(self,db) :
        if sys.version > '3':
            # python 3.0 +
            super().__init__(db,'system.area_code', False) # 定义本实例需要操作的表名
        else :
            # python 2.7
            super(areaCodeManage,self).__init__(db,'system.area_code', False)

class Restful(WebRequestHandler):
    # 获取区域信息树形列表

    @operator_except
    def get(self):
        # 获取所有区域目录

        sql  = """
            select ac.id, ac.area_type, cv.name, ac.area_code, ac.area_name, ac.area_name_en, ac.parent_area_id, ac.flag
            from system.area_code ac 
            inner join system.code_value cv on cv.type_code = 'AREA_TYPE' and cv.code = ac.area_type
            where ac.area_type = 'S'
            order by ac.area_code
            """

        cur=self.db.getCursor()
        cur.execute(sql)
        rows = cur.fetchall()

        areaTree = {}
        areaTree['name']        = '区域代码信息'
        areaTree['id']          = 0
        areaTree['chkDisabled'] = False
        areaTree['open']        = True
        areaTree['checked']     = False

        # 将所有元组转为对象, 因为前台tree组件无法加载元组类型
        areaItemList = self.tupleToList(rows)

        areaTree['children']    = areaItemList

        # 分组区域
        li = self.getChildren(areaItemList)
        self.response(areaTree)

    def getChildren(self, areaItemList):

        if len(areaItemList) == 0:
            return areaItemList

        sql = """
            select ac.id, ac.area_type, cv.name, ac.area_code, ac.area_name, ac.area_name_en, ac.parent_area_id, ac2.area_name
            from system.area_code ac 
            inner join system.code_value cv on cv.type_code = 'AREA_TYPE' and cv.code = ac.area_type
            inner join system.area_code ac2 on ac2.id = ac.parent_area_id
            where ac.parent_area_id = %d
            order by ac.area_code
            """

        cur = self.db.getCursor()

        for obj in areaItemList:
            sqls = sql%(obj['id'])
            cur.execute(sqls);
            rows = cur.fetchall()
            areaList = self.tupleToList(rows)
            obj['children'] = self.getChildren(areaList)

        return areaItemList


    def tupleToList(self, rows):
        areaList = []
        for row in rows:
            obj={}
            obj['id']               = row[0]
            obj['area_type']        = row[1]
            obj['area_type_name']   = row[2]
            obj['area_code']        = row[3]
            obj['area_name']        = row[4]
            obj['area_name_en']     = row[5]
            obj['parent_area_id']   = row[6]
            if obj['area_type'] != 'S':
                obj['parent_area_name'] = row[7]
                obj['name']         = obj['area_name']
                obj['open']         = False
            else:
                obj['open']         = True
                obj['name']         = obj['area_code'] + ' ' + obj['area_name']
                obj['flag']         = row[7]

            obj['chkDisabled']      = True
            obj['checked']          = False
            obj['children']         = []
            areaList.append(obj)
        return areaList


    # 创建区域
    @operator_except
    def post(self):

        alldata = self.getRequestData()

        lsData = {
            'area_type'     : 'area_type',
            'area_code'     : 'area_code',
            'area_name'     : 'area_name',
            'area_name_en'  : 'area_name_en',
            'tier'          : 'tier',
            'parent_area_id': 'parent_area_id',
            'flag'          : 'flag',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        if 'area_code' not in data or 'area_name' not in data or 'area_name_en' not in data or 'parent_area_id' not in data:
            raise BaseError(801, "参数错误：缺少区域数据！")

        # 区域代码、名称均可以重复，因此不做检查
        # 但是区域代码+名称+parent_area_id不能重复
        db = self.getDB("system.area_code")
        result = db.findByCond("area_code", "area_code = '%s' and area_name = '%s' and parent_area_id = %d"%((data['area_code'], data['area_name'], data['parent_area_id'])))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "数据错误：区域信息 %s, %s 重复 %s！"%(data['area_name'], data['area_code']))

        data['create_id']   = self.objUserInfo['id']
        data['create_time'] = GetSystemTime()

        s = areaCodeManage(self.db)
        id = s.save(data, table = "system.area_code")
        if id <= 0:
            raise BaseError(703)  #参数错误

        operation_log(self.db).addLog(self.GetUserInfo(), "areaCodeManage", "创建区域-%s"%data['area_name'], id)

        self.response(id)

    @operator_except
    def put(self):

        alldata = self.getRequestData()

        lsData = {
            'id'            : 'id',
            'area_type'     : 'area_type',
            'area_code'     : 'area_code',
            'area_name'     : 'area_name',
            'area_name_en'  : 'area_name_en',
            'tier'          : 'tier',
            'parent_area_id': 'parent_area_id',
            'flag'          : 'flag',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        if 'id' not in data or  'area_code' not in data or 'area_name' not in data or 'area_name_en' not in data or 'parent_area_id' not in data:
            raise BaseError(801, "参数错误：缺少区域数据！")

        # 区域代码、名称均可以重复，因此不做检查
        # 但是区域代码+名称+parent_area_id不能重复
        db = self.getDB("system.area_code")
        result = db.findByCond("area_code", "area_code = '%s' and area_name = '%s' and parent_area_id = %d and id <> %d"%((
            data['area_code'], data['area_name'], data['parent_area_id'], data['id'])))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "数据错误：区域信息 %s, %s 重复 %s！"%(data['area_name'], data['area_code']))

        data['update_id']   = self.objUserInfo['id']
        data['update_time'] = GetSystemTime()

        s = areaCodeManage(self.db)
        id = s.save(data, data['id'], table = "system.area_code", key = 'id')
        operation_log(self.db).addLog(self.GetUserInfo(), "areaCodeManage", "更新区域-%s"%data['area_name'], id)

        self.response(id)

    # 删除区域
    @operator_except
    def delete(self):

        alldata = self.getRequestData()
        if 'id' not in alldata or alldata['id'] < 0:
            raise BaseError(801)

        self.deleteSubArea(alldata['id'])

        operation_log(self.db).addLog(self.GetUserInfo(), "areaCodeManage", "删除区域-%s"%alldata['area_name'], alldata['id'])
        self.response(0)

    # 删除子区域，含该areaID
    def deleteSubArea(self, areaID):

        db = self.getDB("system.area_code")
        result = db.findByCond("id", "parent_area_id = %d "%areaID)
        if len( result['rows'] ) == 0 :
            # 此区域ID无子区域
            self.deleteArea(areaID)
            return

        for subAreaID in result['rows']:
            self.deleteSubArea(subAreaID[0])

        self.deleteArea(areaID)
        return

    def deleteArea(self, areaID):
        if areaID is None or areaID < 0:
            return

        logI("删除区域: areaID: %d"%areaID)

        # 删除此区域
        db = self.getDB("system.area_code")
        db.deleteByCond("id = %d"%areaID)

        return


    @operator_except
    def patch(self):
        allParam = self.getRequestData()
        op = self.getParam(allParam, 'op', '')
        if op == '':
            raise BaseError(801, '系统错误：缺少操作类型OP')

        allData = {}

        if op == "area":
            # 根据地址获取相应的区域数据
            addrList = self.getParam(allParam, 'addr', '')
            if len(addrList) == 0:
                raise BaseError(801, '无有效地址！')

            # 根据地址信息进行匹配
            Address  = []
            for addr in addrList:
                Address.append(getAreaInfo(self, addr))

            allData['Address'] = Address

        self.response(allData)

# 指定查询的类别及地址的开头
def getAreaCode(db, addr, area_type):

    allData = {
        'area_type' : area_type,
        'id'        : 0,
        'area_name' : '',
        'area_code' : '',
        'index'     : 0
    }


    length  = len(addr)
    index   = 0
    while index < length:
        index   = index + 1
        name    = addr[0:index]

        # logI("type: %s, Addr: %s, Name: %s"%(area_type, addr, name))
        result = db.findByCond("id, area_name, area_code", "area_name like '%s' and area_type = '%s'"%(name, area_type) )
        if len( result['rows'] ) == 0 :
            continue

        allData['id']        = result['rows'][0][0]
        allData['area_name'] = result['rows'][0][1]
        allData['area_code'] = result['rows'][0][2]
        allData['index']     = index

        # logI("AreaInfo1:", allData)
        return allData

    allData['index']  = 0
    return allData

# 根据地址获取地址对应的国家码、省市、地市、区县的代码
def getAreaInfo(self, addr):

    if addr == None:
        return None

    result      = {}
    country     = {}
    province    = {}
    city        = {}
    district    = {}
    detailAddr  = ""

    db = self.getDB("system.area_code")

    # 查询国家代码
    country     = getAreaCode(db, addr, 'S')

    index       = country['index'] 

    # 获取地址剩余字符串
    addr        = addr[index:len(addr)]
    province    = getAreaCode(db, addr, 'P')

    index       = province['index'] 

    addr        = addr[index:len(addr)]
    city        = getAreaCode(db, addr, 'C')

    index       = city['index']

    addr        = addr[index:len(addr)]
    district    = getAreaCode(db, addr, 'D')

    index       = district['index'] 

    # 获取街道地址信息
    detailAddr  = addr[index:len(addr)]

    result = {
        'country'   : country,
        'province'  : province,
        'city'      : city,
        'district'  : district,
        'detailAddr': detailAddr,
    }

    return result

