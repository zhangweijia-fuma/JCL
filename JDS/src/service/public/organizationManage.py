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
from public.areaCodeManage import *

import xlrd, xlwt
from public.excel import excel

class organizationManage(dbMysql.CURD) :
    def __init__(self,db) :
        if sys.version > '3':
            # python 3.0 +
            super().__init__(db,'public.organization',False) # 定义本实例需要操作的表名
        else :
            # python 2.7
            super(organizationManage, self).__init__(db,'public.organization',False)

class Restful(WebRequestHandler):

    @operator_except
    def get(self):

        op              = self.get_argument("op", default='')

        organizationID  = int(self.get_argument("oid", default='0'))
        organizationType= self.get_argument("ot", default='')
        systemUserID    = self.GetSystemUserID()

        offset          = int(self.get_argument('o',  default = '1'))
        rowlimit        = int(self.get_argument('r',  default = '20'))

        offset          = ( offset - 1 ) * rowlimit
        sql_limit       = " limit " + str(rowlimit) + " offset " + str(offset)

        sql = """
            select po.id, po.name, po.name_en, po.abbr, po.abbr_en, po.organization_type, po.registe_date, po.registe_addr, po.current_addr, 
                po.business_scope, po.social_credit_code, po.legal_representative, po.legal_id_type, po.legal_id_no,
                po.contactor_name, po.contactor_mobile, po.logo_file, po.description, po.system_user_id
                from public.organization po
            """
        sql_where = ' where po.system_user_id = %d'%systemUserID
        if organizationType != '':
            sql_where += " and po.organization_type = '%s'"%organizationType    
        if organizationID > 0:
            sql_where += ' and po.id = %d'%organizationID

        sql += sql_where
        sql += " order by po.id desc" 

        if op == "list":
            sql += sql_limit

        cur  = self.db.getCursor()
        cur.execute(sql)
        rows = cur.fetchall()

        if op == "excel":
            self.exportToExcel( rows )
        else:
            rowdata = {}
            rowdata['struct']  = "id, name, name_en, abbr, abbr_en, organization_type, registe_date, registe_addr, current_addr, business_scope, social_credit_code"
            rowdata['struct'] += ", legal_representative, legal_id_type, legal_id_no, contactor_name, contactor_mobile, logo_file, description, system_user_id"
            rowdata['rows'] = rows

            sql  = "select count(*) from public.organization po"
            sql += sql_where
            cur.execute(sql)
            row = cur.fetchone() 
            rowdata['count'] = row[0]
            self.response(rowdata)

    # 输出供应商到excel文件
    def exportToExcel(self, rowdata):

        key = ['id', 'name', 'name_en', 'abbr', 'abbr_en', 'organization_type', 'registe_date', 'registe_addr', 'current_addr', 'business_scope',
            'social_credit_code', 'legal_representative', 'legal_id_type', 'legal_id_no', 'contactor_name', 'contactor_mobile',
            'logo_file', 'description', 'system_user_id']

        organizationList = []
        for i, item in enumerate(rowdata):
            organizationInfo = dict(zip(key, item))
            organizationList.append(organizationInfo)

        cur = self.db.getCursor()
        getDataDict(cur, organizationList, "organization_type", "ORGANIZATION_TYPE", "organization_type_name")
        
        data = []
        sn = 1
        for item in organizationList:

            itemData = []
            itemData.append( sn )
            sn = sn + 1
            itemData.append( item['name'] )
            itemData.append( item['name_en'] )
            itemData.append( item['abbr'])
            itemData.append( item['abbr_en'])
            itemData.append( item['organization_type_name'])
            itemData.append( item['registe_date'])
            itemData.append( item['registe_addr'])
            itemData.append( item['current_addr'])
            itemData.append( item['business_scope'])
            itemData.append( item['social_credit_code'])
            itemData.append( item['legal_representative'])
            itemData.append( item['contactor_name'])
            itemData.append( item['contactor_mobile'])
            itemData.append( item['description'])

            data.append(itemData) 

        struct = "SN, 名称, 名称（英）, 中文缩写, 英文缩写, 类型, 注册日期, 注册地址, 当前地址, 经营范围, 社会信用代码, 法人代表, 联系人员, 电话号码, 描述"

        path = {}
        path = excel.createTempFile("xls")
        
        excel.saveExcel(path['path'], struct, data )

        logInfo = " %s 下载了组织团体清单 "%self.objUserInfo['name']
        operation_log(self.db).addLog(self.GetUserInfo(), "organizationManage", logInfo, 0)

        self.response(path)

    # 传递的参数中，需要包括：
    @operator_except
    def post(self):

        alldata = self.getRequestData()

        s = organizationManage(self.db)

        lsData = {
            'name'                  : 'name',
            'name_en'               : 'name_en',
            'abbr'                  : 'abbr',
            'abbr_en'               : 'abbr_en',
            'registe_date'          : 'registe_date',
            'registe_addr'          : 'registe_addr',
            'current_addr'          : 'current_addr',
            'organization_type'     : 'organization_type',
            'business_scope'        : 'business_scope',
            'social_credit_code'    : 'social_credit_code',
            'legal_representative'  :'legal_representative',
            'legal_id_type'         : 'legal_id_type',
            'legal_id_no'           : 'legal_id_no',
            'contactor_name'        : 'contactor_name',
            'contactor_mobile'      : 'contactor_mobile',
            'logo_file'             : 'logo_file',
            'description'           : 'description',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass


        userInfo = self.GetUserInfo()

        data['system_user_id'] = userInfo['system_user_id']

        db = self.getDB('public.organization')

        # 检查是否重复，同一个System_User_ID里面只能有一个
        result = db.findByCond("name", " (name = '%s' or name_en = '%s') and system_user_id = %d "%(data['name'], data['name_en'], data['system_user_id']))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "参数错误: 组织 %s 已经存在！"%data['name'])


        data['create_id']   = userInfo['id']
        data['create_time'] = GetSystemTime()
        id = s.save(data, table='public.organization')

        logInfo = "创建：%s，%s"%(data['name'], data['name_en'])
        operation_log(self.db).addLog(self.GetUserInfo(), "organizationManage", logInfo, id)

        self.response(id)

    @operator_except
    def put(self):

        alldata = self.getRequestData()
        if self.getParam( alldata, "id", 0) == 0:
            raise BaseError(801, "参数错误：缺少待更新供应商的ID！")

        s = organizationManage(self.db)
        
        lsData = {
            'id'                    : 'id',
            'name'                  : 'name',
            'name_en'               : 'name_en',
            'abbr'                  : 'abbr',
            'abbr_en'               : 'abbr_en',
            'registe_date'          : 'registe_date',
            'registe_addr'          : 'registe_addr',
            'organization_type'     : 'organization_type',
            'current_addr'          : 'current_addr',
            'business_scope'        : 'business_scope',
            'social_credit_code'    : 'social_credit_code',
            'legal_representative'  :'legal_representative',
            'legal_id_type'         : 'legal_id_type',
            'legal_id_no'           : 'legal_id_no',
            'contactor_name'        : 'contactor_name',
            'contactor_mobile'      : 'contactor_mobile',
            'logo_file'             : 'logo_file',
            'description'           : 'description',
            'system_user_id'        : 'system_user_id',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        db = self.getDB('public.organization')
        
        # 检查是否重复，同一个System_User_ID里面只能有一个
        result = db.findByCond("name", " (name = '%s' or name_en = '%s') and system_user_id = %d and id <> %d"%(data['name'], data['name_en'], data['system_user_id'], data['id']))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "参数错误: 组织 %s 已经存在！"%data['name'])

        userInfo            = self.GetUserInfo()
        data['update_id']   = userInfo['id']
        data['update_time'] = GetSystemTime()

        id = s.save(data, data['id'], table='public.organization')

        logInfo = "更新组织团体信息：%s，%s"%(data['name'], data['name_en'])
        operation_log(self.db).addLog( userInfo, "organizationManage", logInfo, id)

        self.response(id)


    @operator_except
    def delete(self):

        alldata = self.getRequestData()
        orgID = self.getParam( alldata, "id", 0)
        if orgID == 0:
            raise BaseError(801, "参数错误：缺少待删除的供应商ID！")

        logInfo = '组织团体 %s 被删除'%alldata['name']

        # 删除记录
        s = organizationManage(self.db)
        s.remove( orgID, table = "public.organization", key = "id", delete = True)

        # 记录日志并退出
        operation_log(self.db).addLog(self.GetUserInfo(), "organizationManage", logInfo, orgID)
        self.response(orgID)

    @operator_except
    def patch(self):

        paramData       = self.getRequestData()
        systemUserID    = self.GetSystemUserID()

        op  = self.getParam(paramData, "op", "")
        cur = self.db.getCursor()

        allData = {}

        if op == 'list':
            # 获取所有团体清单
            sql = """
                select po.id, po.name
                from public.organization po 
                where po.system_user_id = %d
                """%systemUserID

            cur.execute(sql)
            rows = cur.fetchall()
            allOrganizationList = {}
            allOrganizationList['rows'] = [(0, "选择全部", )] + rows
            allOrganizationList['struct'] = 'id, name'

            allData['allOrganizationList'] = allOrganizationList

            # 获取组织团体类型清单
            sql  = "select cv.code, cv.name "
            sql += " from system.code_value cv"
            sql += " where cv.type_code = 'ORGANIZATION_TYPE' "
            sql += " order by cv.sort"

            cur = self.db.getCursor()
            cur.execute(sql)
            rows = cur.fetchall()

            organizationTypeList = {}
            organizationTypeList['struct']  = "code, name"
            organizationTypeList['rows']    = [('0', "选择全部", )] + rows

            allData['organizationTypeList'] = organizationTypeList

            # 获取证件类型清单
            sql  = "select cv.code, cv.name "
            sql += " from system.code_value cv"
            sql += " where cv.type_code = 'IDENTITY_TYPE' "
            sql += " order by cv.sort"

            cur = self.db.getCursor()
            cur.execute(sql)
            rows = cur.fetchall()

            identifyTypeList = {}
            identifyTypeList['struct']  = "code, name"
            identifyTypeList['rows']    = [('0', "选择全部", )] + rows

            allData['identifyTypeList'] = identifyTypeList

        self.response(allData)


