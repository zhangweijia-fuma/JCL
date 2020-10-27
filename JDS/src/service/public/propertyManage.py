#coding:utf-8

import sys
import time
import datetime

from db import dbMysql
from libs.web.Base  import WebRequestHandler,BaseError,operator_except
from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE 
from libs.utils.debug import *
from libs.utils.utils import *

from config import *

from system.accountManage import account

import xlrd, xlwt
from public.excel import excel

from public.deviceUtils import *

class propertyManage(dbMysql.CURD) :
    def __init__(self,db) :
        if sys.version > '3':
            # python 3.0 +
            super().__init__(db,'public.property_type',False) # 定义本实例需要操作的表名
        else :
            # python 2.7
            super(propertyManage, self).__init__(db,'public.property_type',False)

class Restful(WebRequestHandler):


    @operator_except
    def get(self):

        op              = self.get_argument("op", default='')

        oid             = int(self.get_argument("oid", default='0'))
        itemType        = self.get_argument("type", default='')
        systemUserID    = self.GetSystemUserID()

        offset          = int(self.get_argument('o',  default = '1'))
        rowlimit        = int(self.get_argument('r',  default = '20'))

        offset          = ( offset - 1 ) * rowlimit
        sql_limit       = " limit " + str(rowlimit) + " offset " + str(offset)

        sql = """
            select pl.id, pl.code, pl.name, pl.name_en, pl.value_type, pl.value_s, pl.value_i, pl.value_f, pl.description,
                cv.name
            from public.property_list pl
            inner join system.code_value cv on cv.code = pl.value_type
            """

        sql_where = ' where pl.system_user_id = %d'%systemUserID
        if itemType != '':
            sql_where += " and pl.value_type = '%s'"%itemType
        if oid > 0:
            sql_where += ' and pl.id in (select property_id from public.component_property_list where component_id = %d)'%oid

        sql += sql_where
        sql += " order by pl.id desc" 

        # logI(sql)
        if op == "list":
            sql += sql_limit

        cur  = self.db.getCursor()
        cur.execute(sql)
        rows = cur.fetchall()
        
        result = []
        for row in rows:
            itemData = []
            for item in row:
                itemData.append(item)

            if row[4] == 'STR':
                itemData.append(row[5])
            elif itemData[4] == 'INT':
                itemData.append(row[6])
            if row[4] == 'FLT':
                itemData.append(row[7])

            result.append(itemData)
        if op == "excel":
            self.exportToExcel( result )
        else:
            rowdata = {}
            rowdata['struct']  = "id, code, name, name_en, value_type, value_s, value_i, value_f, description, value_type_name, value"
            rowdata['rows'] = result

            sql  = "select count(*) from public.property_list pl"
            sql += sql_where
            cur.execute(sql)
            row = cur.fetchone() 
            rowdata['count'] = row[0]
            self.response(rowdata)

    # 输出供应商到excel文件
    def exportToExcel(self, rowdata):

        key = ['id', 'code', 'name', 'name_en', 'value_type', 'value_s', 
            'value_i', 'value_f', 'description', 'value_type_name', 'value']

        itemList = []
        for i, item in enumerate(rowdata):
            itemInfo = dict(zip(key, item))
            itemList.append(itemInfo)

        data = []
        sn = 1
        for item in vendorList:

            itemData = []
            itemData.append( sn )
            sn = sn + 1
            itemData.append( item['code'] )
            itemData.append( item['name'] )
            itemData.append( item['name_en'] )
            itemData.append( item['value_type_name'])
            itemData.append( item['value'])
            itemData.append( item['status_name'])
            itemData.append( item['description'])
            data.append(itemData) 

        struct = "SN, 属性代码, 属性名称, 属性名称（英）, 属性类型, 属性值, 属性描述"

        path = {}
        path = excel.createTempFile("xls")
        
        excel.saveExcel(path['path'], struct, data )

        logInfo = " %s 下载了属性清单 "%self.objUserInfo['name']
        operation_log(self.db).addLog(self.GetUserInfo(), "propertyManage", logInfo, 0)

        self.response(path)


    @operator_except
    def post(self):

        alldata         = self.getRequestData()
        systemUserID    = self.GetSystemUserID()
        
        # 属性归属的componentID，暂时一个属性只能归属一个组件，不能归属多个组件
        oid = int(self.getParam(alldata, "component_id", "0"))
        if oid == 0:
            raise BaseError(801, "缺少属性归属的组件ID！")

        s = propertyManage(self.db)

        lsData = {
            'name'       : 'name',
            'name_en'    : 'name_en',
            'value_type' : 'value_type',
            'value_s'    : 'value_s',
            'value_i'    : 'value_i',
            'value_f'    : 'value_f',
            'description': 'description',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        db = self.getDB('public.property_list')

        # 检查名称是否重复
        result = db.findByCond("name", "(name = '%s' or name_en = '%s') and system_user_id = %d "%(data['name'], data['name_en'], systemUserID))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "参数错误：属性名称已经存在！")

        userInfo = self.GetUserInfo()
        serialNo = GetSerialNo(systemUserID, 'property_serial')
        if serialNo == -1:
            raise BaseError(801, '系统错误：获取序列号失败！')

        # 属性类型编码规则：PRP_AAA_BBBBBBBB。其中，AAA为属性值类型，取值为STR、INT或FLT，BBBBBBBB为属性序列号值。
        data['code'] = 'PRP-' + data['value_type'] + '-%08d'%(serialNo + 1)

        data['system_user_id'] = systemUserID
        data['create_id']   = userInfo['id']
        data['create_time'] = GetSystemTime()

        property_id = s.save(data, table='public.property_list')

        logInfo = "创建属性：%s，%s"%(data['name'], data['name_en'])
        operation_log(self.db).addLog(self.GetUserInfo(), "propertyManage", logInfo, property_id)

        # 记录归属关系
        data = {
            'component_id'  : oid,
            'property_id'   : property_id,
            'system_user_id': systemUserID,
        }

        s.save(data, table='public.component_property_list')

        self.response(property_id)

    @operator_except
    def put(self):

        alldata = self.getRequestData()
        if self.getParam( alldata, "id", 0) == 0:
            raise BaseError(801, "参数错误：缺少待更新属性的ID！")

        s = propertyManage(self.db)
        
        lsData = {
            'id'         : 'id',
            'name'       : 'name',
            'name_en'    : 'name_en',
            'value_type' : 'value_type',
            'value_s'    : 'value_s',
            'value_i'    : 'value_i',
            'value_f'    : 'value_f',
            'description': 'description',
            'system_user_id' : 'system_user_id',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        db = self.getDB('public.property_list')

        # 检查名称是否重复
        result = db.findByCond("name", "id != %d and (name = '%s' or name_en = '%s') "%(data['id'], data['name'], data['name_en']))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "参数错误：属性名称已经存在！")

        # 在更新时，不再更新编号

        userInfo            = self.GetUserInfo()
        data['update_id']   = userInfo['id']
        data['update_time'] = GetSystemTime()
        
        property_id = s.save(data, data['id'], table='public.property_list')

        logInfo = "更新属性：%s，%s"%(data['name'], data['name_en'])
        operation_log(self.db).addLog( userInfo, "propertyManage", logInfo, property_id)

        self.response(property_id)


    @operator_except
    def delete(self):

        alldata = self.getRequestData()
        itemID = self.getParam( alldata, "id", 0)
        if itemID == 0:
            raise BaseError(801, "参数错误：缺少待删除的属性ID！")

        # 属性归属的componentID，暂时一个属性只能归属一个组件，不能归属多个组件
        oid = int(self.getParam(alldata, "component_id", "0"))
        if oid == 0:
            raise BaseError(801, "缺少属性归属的组件ID！")

        logInfo = '属性 %s 被删除'%alldata['name']

        # 删除记录
        s = propertyManage(self.db)
        s.remove( itemID, table = "public.property_list", key = "id", delete = True)

        # 记录日志并退出
        operation_log(self.db).addLog(self.GetUserInfo(), "propertyManage", logInfo, itemID)

        # 删除和组件的关系
        s.remove( itemID, table = "public.component_property_list", key = "property_id", delete = True)

        self.response(itemID)

    @operator_except
    def patch(self):

        paramData       = self.getRequestData()
        systemUserID    = self.GetSystemUserID()

        op  = self.getParam(paramData, "op", "")
        cur = self.db.getCursor()

        allData = {}

        if op == 'list':
            # # 获取所有属性清单
            # sql = """
            #     select pl.id, pl.name from public.property_list pl 
            #     where pl.system_user_id = %d
            #     """%systemUserID

            # cur.execute(sql)
            # rows = cur.fetchall()
            # allItemList = {}
            # allItemList['rows'] = [(0, "选择全部", )] + rows
            # allItemList['struct'] = 'id, name'

            # allData['allItemList'] = allItemList

            # 获取属性类别清单
            sql  = "select cv.code, cv.name "
            sql += " from system.code_value cv"
            sql += " where cv.type_code = 'PROPERTY_VALUE_TYPE' "
            sql += " order by cv.sort"

            cur = self.db.getCursor()
            cur.execute(sql)
            rows = cur.fetchall()

            typeList = {}
            typeList['struct']   = "code, name"
            typeList['rows']     = [('0', "选择全部", )] + rows

            allData['typeList']  = typeList

            # selectList = {}
            # selectList['struct']   = "code, name"
            # selectList['rows']     = rows

            # allData['selectList']  = selectList

        self.response(allData)

