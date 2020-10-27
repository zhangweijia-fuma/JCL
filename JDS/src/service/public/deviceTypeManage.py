#coding:utf-8

import sys
import time
import datetime

from db import dbMysql
from libs.web.Base  import WebRequestHandler,BaseError,operator_except
from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE 
from libs.utils.debug import *
from libs.utils.utils import *

from libs.JCL.JCLCommon import *

from config import *

from system.accountManage import account

import xlrd, xlwt
from public.excel import excel

from public.deviceUtils import *

class deviceTypeManage(dbMysql.CURD) :
    def __init__(self,db) :
        if sys.version > '3':
            # python 3.0 +
            super().__init__(db,'public.device_type',False) # 定义本实例需要操作的表名
        else :
            # python 2.7
            super(deviceTypeManage, self).__init__(db,'public.device_type',False)

class Restful(WebRequestHandler):


    @operator_except
    def get(self):

        op              = self.get_argument("op", default='')
        systemUserID    = self.GetSystemUserID()

        tid  = int(self.get_argument('tid',  default = 0))

        offset  = int(self.get_argument('o',  default = '1'))
        rowlimit= int(self.get_argument('r',  default = '20'))

        offset = ( offset - 1 ) * rowlimit
        sql_limit = " limit " + str(rowlimit) + " offset " + str(offset)

        sql = """
            select dt.id, dt.code, dt.name, dt.name_en, dt.abbr_en, dt.description, dt.system_user_id
            from public.device_type dt
            """
        sql_where = " where dt.system_user_id = %d "%systemUserID

        if tid > 0:
            sql_where += " and id = %d "%tid
        sql += sql_where
        sql += " order by dt.code asc" 

        if op == "list":
            sql += sql_limit

        cur  = self.db.getCursor()
        cur.execute(sql)
        rows = cur.fetchall()

        if op == "excel":
            self.exportToExcel( rows )
        else:
            rowdata = {}
            rowdata['struct']  = "id, code, name, name_en, abbr_en, description, system_user_id"
            rowdata['rows'] = rows

            sql  = "select count(*) from public.device_type dt"
            sql += sql_where
            cur.execute(sql)
            row = cur.fetchone() 
            rowdata['count'] = row[0]
            self.response(rowdata)

    # 输出供应商到excel文件
    def exportToExcel(self, rowdata):

        key = ['id', 'code', 'name', 'name_en', 'abbr_en', 'description', 'system_user_id']

        itemList = []
        for i, item in enumerate(rowdata):
            itemInfo = dict(zip(key, item))
            itemList.append(itemInfo)
        
        data = []
        sn = 1
        for item in itemList:

            itemData = []
            itemData.append( sn )
            sn = sn + 1
            itemData.append( item['name'] )
            itemData.append( item['name_en'] )
            itemData.append( item['abbr_en'])
            itemData.append( item['code'] )
            itemData.append( item['description'])
            data.append(itemData) 

        struct = "SN, 中文名称, 英文名称, 英文缩写, 类型编号, 设备说明"

        path = {}
        path = excel.createTempFile("xls")
        
        excel.saveExcel(path['path'], struct, data )

        logInfo = " %s 下载了设备类型清单 "%self.objUserInfo['name']
        operation_log(self.db).addLog(self.GetUserInfo(), "propertyManage", logInfo, 0)

        self.response(path)


    @operator_except
    def post(self):

        alldata = self.getRequestData()
        system_user_id = self.GetSystemUserID()

        s = deviceTypeManage(self.db)

        lsData = {
            'code'          : 'code',
            'name'          : 'name',
            'name_en'       : 'name_en',
            'abbr_en'       : 'abbr_en',
            'description'   : 'description',
            'system_user_id': 'system_user_id',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        db = self.getDB('public.device_type')

        # 检查名称是否重复
        result = db.findByCond("name", " (name = '%s' or name_en = '%s' ) and system_user_id = %d "%(data['name'], data['name_en'], system_user_id))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "参数错误：设备名称 %s 已经存在！"%data['name'])

        userInfo = self.GetUserInfo()

        serialNo = GetSerialNo(userInfo['system_user_id'], "device_type_serial")
        if serialNo == -1:
            raise BaseError(801, "获取设备类型序列号出错！")

        # 设备类型编号规则：DEV_AAAAAA，AAAAAA为序列号，为参数表中的值
        data['code'] = "DEV-TYPE-%06d"%serialNo
        data['system_user_id'] = system_user_id

        data['create_id']   = userInfo['id']
        data['create_time'] = GetSystemTime()

        logI("设备类型：", data)
        device_id = s.save(data, table='public.device_type')

        logInfo = "创建设备类型：%s，%s"%(data['name'], data['name_en'])
        operation_log(self.db).addLog(self.GetUserInfo(), "deviceTypeManage", logInfo, device_id)

        self.response(device_id)

    @operator_except
    def put(self):

        alldata = self.getRequestData()
        if self.getParam( alldata, "id", 0) == 0:
            raise BaseError(801, "参数错误：缺少待更新设备类型的ID！")

        s = deviceTypeManage(self.db)
        system_user_id = self.GetSystemUserID()

        lsData = {
            'id'            : 'id',
            'code'          : 'code',
            'name'          : 'name',
            'name_en'       : 'name_en',
            'abbr_en'       : 'abbr_en',
            'description'   : 'description',
            'system_user_id': 'system_user_id',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        db = self.getDB('public.device_type')

        # 检查名称是否重复
        result = db.findByCond("name", "id != %d and (name = '%s' or name_en = '%s') and system_user_id = %d"%(data['id'], data['name'], data['name_en'], system_user_id))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "参数错误：设备类型名称已经存在！")

        # 在更新时，不再更新编号

        userInfo            = self.GetUserInfo()
        data['update_id']   = userInfo['id']
        data['update_time'] = GetSystemTime()
        
        device_id = s.save(data, data['id'], table='public.device_type')

        logInfo = "更新设备类型：%s，%s"%(data['name'], data['name_en'])
        operation_log(self.db).addLog( userInfo, "propertyManage", logInfo, device_id)

        self.response(device_id)

    @operator_except
    def delete(self):

        alldata = self.getRequestData()
        itemID = self.getParam( alldata, "id", 0)
        if itemID == 0:
            raise BaseError(801, "参数错误：缺少待删除的设备类型ID！")

        logInfo = '设备类型 %s 被删除'%alldata['name']

        # 删除记录
        s = deviceTypeManage(self.db)
        data = { 'id' : itemID}
        device_id = s.save(data, data['id'], table='public.device_type')

        # s.remove( itemID, table = "public.device_type", key = "id", delete = True)

        # 记录日志并退出
        operation_log(self.db).addLog(self.GetUserInfo(), "deviceTypeManage", logInfo, itemID)
        self.response(itemID)

    @operator_except
    def patch(self):

        paramData       = self.getRequestData()
        systemUserID    = self.GetSystemUserID()

        op  = self.getParam(paramData, "op", "")
        cur = self.db.getCursor()

        allData = {}

        if op == 'list':
            # 获取设备类别清单
            sql = """
                select dt.id, dt.name
                from public.device_type dt
                where dt.system_user_id = %d 
            """%self.GetSystemUserID()

            cur = self.db.getCursor()
            cur.execute(sql)
            rows = cur.fetchall()

            typeList = {}
            typeList['struct']   = "id, name"
            typeList['rows']     = [(0, "选择全部", )] + rows

            allData['typeList']  = typeList
        
        self.response(allData)

