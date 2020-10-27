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

class componentManage(dbMysql.CURD) :
    def __init__(self,db) :
        if sys.version > '3':
            # python 3.0 +
            super().__init__(db,'public.device_type',False) # 定义本实例需要操作的表名
        else :
            # python 2.7
            super(componentManage, self).__init__(db,'public.component_list',False)

class Restful(WebRequestHandler):


    @operator_except
    def get(self):

        op              = self.get_argument("op", default='')
        oid             = int(self.get_argument("oid", default='0'))

        systemUserID    = self.GetSystemUserID()

        offset          = int(self.get_argument('o',  default = '1'))
        rowlimit        = int(self.get_argument('r',  default = '20'))

        offset          = ( offset - 1 ) * rowlimit
        sql_limit       = " limit " + str(rowlimit) + " offset " + str(offset)

        sql = """
            select ct.id, ct.code, ct.name, ct.name_en, ct.description, ct.system_user_id
            from public.component_list ct
            """
        sql_where = ' where ct.system_user_id = %d '%systemUserID

        if oid > 0:
            sql_where += " and ct.id = %d"%oid

        sql += sql_where
        sql += " order by ct.name asc" 

        if op == "list":
            sql += sql_limit

        cur  = self.db.getCursor()
        cur.execute(sql)
        rows = cur.fetchall()

        if op == "excel":
            self.exportToExcel( rows )
        else:
            rowdata = {}
            rowdata['struct']  = "id, code, name, name_en, description, system_user_id"
            rowdata['rows'] = rows

            sql  = "select count(*) from public.component_list ct"
            sql += sql_where
            cur.execute(sql)
            row = cur.fetchone() 
            rowdata['count'] = row[0]
            self.response(rowdata)

    # 输出供应商到excel文件
    def exportToExcel(self, rowdata):

        key = ['id', 'code', 'name', 'name_en', 'description', 'system_user_id']

        itemList = []
        for i, item in enumerate(rowdata):
            itemInfo = dict(zip(key, item))
            itemList.append(itemInfo)

        cur = self.db.getCursor()

        data = []
        sn = 1
        for item in vendorList:
            itemData = []
            itemData.append( sn )
            sn = sn + 1
            itemData.append( item['code'] )
            itemData.append( item['name'] )
            itemData.append( item['name_en'] )
            itemData.append( item['description'])
            data.append(itemData) 

        struct = "SN, 组件编号, 名称, 名称（英）, 描述"

        path = {}
        path = excel.createTempFile("xls")
        
        excel.saveExcel(path['path'], struct, data )

        logInfo = " %s 下载了组件清单 "%self.objUserInfo['name']
        operation_log(self.db).addLog(self.GetUserInfo(), "componentManage", logInfo, 0)

        self.response(path)


    @operator_except
    def post(self):

        alldata = self.getRequestData()
        system_user_id = self.GetSystemUserID()

        s = componentManage(self.db)

        lsData = {
            'code'          : 'code',
            'name'          : 'name',
            'name_en'       : 'name_en',
            'description'   : 'description',
            'system_user_id': 'system_user_id',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        db = self.getDB('public.component_list')

        # 检查名称是否重复
        result = db.findByCond("name", " (name = '%s' or name_en = '%s' ) and system_user_id = %d"%(data['name'], data['name_en'], system_user_id))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "参数错误：组件名称 %s 已经存在！"%data['name'])

        userInfo = self.GetUserInfo()

        serialNo = GetSerialNo(userInfo['system_user_id'], "component_serial")
        if serialNo == -1:
            raise BaseError(801, "获取设备类型序列号出错！")

        # 部件编号规则：CMP_AAAAAAAA。其中，AAAAAAAA为部件序列号，从参数表中获取。
        data['code'] = "CMP_%08d"%serialNo
        data['system_user_id'] = system_user_id

        data['create_id']   = userInfo['id']
        data['create_time'] = GetSystemTime()

        component_id = s.save(data, table='public.component_list')

        logInfo = "创建组件：%s，%s"%(data['name'], data['name_en'])
        operation_log(self.db).addLog(self.GetUserInfo(), "componentManage", logInfo, component_id)

        self.response(component_id)

    @operator_except
    def put(self):

        alldata = self.getRequestData()
        if self.getParam( alldata, "id", 0) == 0:
            raise BaseError(801, "参数错误：缺少待更新组件的ID！")

        s = componentManage(self.db)
        system_user_id = self.GetSystemUserID()

        lsData = {
            'id'            : 'id',
            'code'          : 'code',
            'name'          : 'name',
            'name_en'       : 'name_en',
            'description'   : 'description',
            'system_user_id': 'system_user_id',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        db = self.getDB('public.component_list')

        # 检查名称是否重复
        result = db.findByCond("name", "id != %d and (name = '%s' or name_en = '%s') and system_user_id = %d"%(data['id'], data['name'], data['name_en'], system_user_id))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "参数错误：组件名称已经存在！")

        # 在更新时，不再更新编号

        userInfo            = self.GetUserInfo()
        data['update_id']   = userInfo['id']
        data['update_time'] = GetSystemTime()
        
        component_id = s.save(data, data['id'], table='public.component_list')

        logInfo = "更新组件：%s，%s"%(data['name'], data['name_en'])
        operation_log(self.db).addLog( userInfo, "componentManage", logInfo, component_id)

        self.response(component_id)


    @operator_except
    def delete(self):

        alldata = self.getRequestData()
        itemID = self.getParam( alldata, "id", 0)
        if itemID == 0:
            raise BaseError(801, "参数错误：缺少待删除的组件ID！")

        logInfo = '组件 %s 被删除'%alldata['name']

        # 删除记录
        s = componentManage(self.db)
        s.remove( itemID, table = "public.component_list", key = "id", delete = True)

        # 记录日志并退出
        operation_log(self.db).addLog(self.GetUserInfo(), "componentManage", logInfo, itemID)
        self.response(itemID)

    @operator_except
    def patch(self):

        paramData       = self.getRequestData()
        systemUserID    = self.GetSystemUserID()

        op  = self.getParam(paramData, "op", "")
        cur = self.db.getCursor()

        allData = {}

        # if op == 'list':
        #     # 获取设备类别清单
        #     sql  = "select cv.code, cv.name "
        #     sql += " from system.code_value cv"
        #     sql += " where cv.type_code = 'PROPERTY_VALUE_TYPE' "
        #     sql += " order by cv.sort"

        #     cur = self.db.getCursor()
        #     cur.execute(sql)
        #     rows = cur.fetchall()

        #     typeList = {}
        #     typeList['struct']   = "code, name"
        #     typeList['rows']     = [('0', "选择全部", )] + rows

        #     allData['typeList']  = typeList

        #     selectList = {}
        #     selectList['struct']   = "code, name"
        #     selectList['rows']     = rows

        #     allData['selectList']  = selectList
        
        self.response(allData)

