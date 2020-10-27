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
from public.areaCodeManage import *

import xlrd, xlwt
from public.excel import excel

from public.deviceUtils import *


class vendorManage(dbMysql.CURD) :
    def __init__(self,db) :
        if sys.version > '3':
            # python 3.0 +
            super().__init__(db,'public.vendor_list',False) # 定义本实例需要操作的表名
        else :
            # python 2.7
            super(vendorManage, self).__init__(db,'public.vendor_list',False)

class Restful(WebRequestHandler):

    @operator_except
    def get(self):

        op              = self.get_argument("op", default='')

        vendorID        = int(self.get_argument("vid", default='0'))
        vendorStatus    = self.get_argument("vs", default='')
        vendorType      = self.get_argument("vt", default='')
        systemUserID    = self.GetSystemUserID()

        offset          = int(self.get_argument('o',  default = '1'))
        rowlimit        = int(self.get_argument('r',  default = '20'))

        offset          = ( offset - 1 ) * rowlimit
        sql_limit       = " limit " + str(rowlimit) + " offset " + str(offset)

        sql = """
            select vl.id, vl.vendor_id, vl.vendor_code, po.name, po.name_en, po.abbr, po.abbr_en, po.registe_addr, po.current_addr, 
                po.logo_file, po.contactor_name, po.contactor_mobile, po.business_scope,
                vl.vendor_type, po.description, vl.certifaction_status, vl.certifaction_date, vl.certifaction_desc, vl.start_date, vl.end_date
            from public.vendor_list vl
            inner join public.organization po on po.id = vl.vendor_id
            """

        sql_where = ' where vl.system_user_id = %d'%systemUserID
        if vendorStatus != '':
            sql_where += " and vl.certifaction_status = '%s'"%vendorStatus
        if vendorType != '':
            sql_where += " and vl.vendor_type = '%s'"%vendorType    
        if vendorID > 0:
            sql_where += ' and vl.id = %d'%vendorID

        sql += sql_where
        sql += " order by vl.id desc" 

        if op == "list":
            sql += sql_limit

        cur  = self.db.getCursor()
        cur.execute(sql)
        rows = cur.fetchall()

        if op == "excel":
            self.exportToExcel( rows )
        else:
            rowdata = {}
            rowdata['struct']  = "id, vendor_id, vendor_code, name, name_en, abbr, abbr_en, registe_addr, current_addr, logo_file, contactor_name, contactor_mobile, "
            rowdata['struct'] += "business_scope, vendor_type, description, certifaction_status, certifaction_date, certifaction_desc, start_date, end_date"

            rowdata['rows'] = rows

            sql  = "select count(*) from public.vendor_list vl"
            sql += sql_where
            cur.execute(sql)
            row = cur.fetchone() 
            rowdata['count'] = row[0]
            self.response(rowdata)

    # 输出供应商到excel文件
    def exportToExcel(self, rowdata):

        key = ['id', 'vendor_id', 'vendor_code', 'name', 'name_en', 'abbr', 'abbr_en', 'abbr', 'abbr_en', 'registe_addr', 'current_addr', 'logo_file', 
                'contactor_name', 'contactor_mobile', 'vendor_type', 'description', 'certifaction_status', 'certifaction_date',
                 'certifaction_desc', 'start_date', 'end_date']

        vendorList = []
        for i, item in enumerate(rowdata):
            vendorInfo = dict(zip(key, item))
            vendorList.append(vendorInfo)

        cur = self.db.getCursor()
        getDataDict(cur, vendorList, "certifaction_status", "VENDOR_COOPERATION_STATUS", "status_name")
        getDataDict(cur, vendorList, "vendor_type", "VENDOR_TYPE", "vendor_type_name")
        
        data = []
        sn = 1
        for item in vendorList:

            itemData = []
            itemData.append( sn )
            sn = sn + 1
            itemData.append( item['name'] )
            itemData.append( item['name_en'] )
            itemData.append( item['abbr'])
            itemData.append( item['registe_addr'])
            itemData.append( item['contactor_name'])
            itemData.append( item['contactor_mobile'])
            itemData.append( item['vendor_type_name'])
            itemData.append( item['description'])
            itemData.append( item['status_name'])
            itemData.append( item['certifaction_date'])
            itemData.append( item['start_date'])
            itemData.append( item['end_date'])
            itemData.append( item['certifaction_desc'])

            data.append(itemData) 

        struct = "SN, 供应商名称, 供应商名称（英）, 供应商缩写, 供应商地址, 联系人, 联系电话, 供应商类型, 供应商描述, 供应商状态, 认证日期, 合作开始日期, 合作结束日期"

        path = {}
        path = excel.createTempFile("xls")
        
        excel.saveExcel(path['path'], struct, data )

        logInfo = " %s 下载了供应商清单 "%self.objUserInfo['name']
        operation_log(self.db).addLog(self.GetUserInfo(), "vendorManage", logInfo, 0)

        self.response(path)

    # 传递的参数中，需要包括：
    #   国家代码、省份代码、地市代码、区县及相应的名称
    #   vendor_addr 中包含的地址只是区县以下的地址，后台构建供应商的地址
    #   vendor_code由后台创建
    @operator_except
    def post(self):

        alldata = self.getRequestData()

        s = vendorManage(self.db)

        lsData = {
            'vendor_id'             : 'vendor_id',
            'vendor_type'           : 'vendor_type',
            'certifaction_desc'     : 'certifaction_desc',
            'start_date'            : 'start_date',
            'end_date'              : 'end_date',
            'certifaction_status'   : 'certifaction_status',
            'system_user_id'        : 'system_user_id',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass


        db = self.getDB('public.vendor_list')
        # 检查Vendor是否重复，同一个System_User_ID里面的Vender只能有一个
        result = db.findByCond("vendor_id", "vendor_id = %d and system_user_id = %d "%(data['vendor_id'], data['system_user_id']))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "参数错误：供应商名称已经存在！")

        db = self.getDB('public.organization')
        result = db.findByCond("name, name_en, current_addr", "id = %d and system_user_id = %d "%(data['vendor_id'], data['system_user_id']))
        if len( result['rows'] ) == 0 :
            raise BaseError(801, "参数错误：供应商不存在！")

        name         = result['rows'][0][0]
        name_en      = result['rows'][0][1]
        current_addr = result['rows'][0][2]
        areaInfo = getAreaInfo(self, current_addr)

        if areaInfo['country']['id'] == 0:
            countryCode = 'CN'
        else:
            countryCode = areaInfo['country']['area_code']

        provinceCode    = areaInfo['province']['area_code']
        cityCode        = areaInfo['city']['area_code']
        districtCode    = areaInfo['district']['area_code']
        
        userInfo = self.GetUserInfo()
        serialNo = GetSerialNo(userInfo['system_user_id'], 'vendor_serial', False)
        if serialNo == -1:
            raise BaseError(801, '系统错误：获取供应商序列号失败！')

        data['vendor_code'] = 'VDR-' + countryCode + provinceCode + cityCode + districtCode + '-%08d'%(serialNo + 1)
        logI("Code: %s"%data['vendor_code'])

        data['system_user_id'] = userInfo['system_user_id']
        data['certifaction_status'] = 'CI'
        data['certifaction_date'] = GetSystemTime()

        data['create_id']   = userInfo['id']
        data['create_time'] = GetSystemTime()
        vendor_id = s.save(data, table='public.vendor_list')

        logInfo = "创建供应商：%s，%s"%(name, name_en)

        operation_log(self.db).addLog(self.GetUserInfo(), "vendorManage", logInfo, vendor_id)

        self.response(vendor_id)

    @operator_except
    def put(self):

        alldata = self.getRequestData()
        if self.getParam( alldata, "id", 0) == 0:
            raise BaseError(801, "参数错误：缺少待更新供应商的ID！")

        s = vendorManage(self.db)
        
        lsData = {
            'id'                : 'id',
            'vendor_id'         : 'vendor_id',
            'vendor_type'       : 'vendor_type',
            'certifaction_date' : 'certifaction_date',
            'certifaction_desc' : 'certifaction_desc',
            'start_date'        : 'start_date',
            'end_date'          : 'end_date',
            'certifaction_status' : 'certifaction_status',
            'system_user_id'    : 'system_user_id',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        db = self.getDB('public.vendor_list')
        # 检查名称是否重复
        result = db.findByCond("vendor_id", "id != %d and (vendor_id = '%d') "%(data['id'], data['vendor_id']))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "参数错误：供应商名称已经存在！")

        # 在更新时，不再更新供应商编号
        userInfo            = self.GetUserInfo()
        data['update_id']   = userInfo['id']
        data['update_time'] = GetSystemTime()

        vendor_id = s.save(data, data['id'], table='public.vendor_list')

        logInfo = "更新供应商：%s，%s"%(alldata['name'], alldata['name_en'])
        operation_log(self.db).addLog( userInfo, "vendorManage", logInfo, vendor_id)

        self.response(vendor_id)


    @operator_except
    def delete(self):

        alldata = self.getRequestData()
        vendorID = self.getParam( alldata, "id", 0)
        if vendorID == 0:
            raise BaseError(801, "参数错误：缺少待删除的供应商ID！")

        logInfo = '供应商 %s 被删除'%alldata['vendor_name']

        # 删除记录
        s = vendorManage(self.db)
        s.remove( vendorID, table = "public.vendor_list", key = "id", delete = True)

        # 记录日志并退出
        operation_log(self.db).addLog(self.GetUserInfo(), "vendorManage", logInfo, vendorID)
        self.response(vendorID)

    @operator_except
    def patch(self):

        paramData       = self.getRequestData()
        systemUserID    = self.GetSystemUserID()

        op  = self.getParam(paramData, "op", "")
        cur = self.db.getCursor()

        allData = {}

        if op == 'list':
            # 获取所有组织团体清单
            sql = """
                select po.id, po.name
                from public.organization po
                where po.system_user_id = %d
                """%systemUserID

            cur.execute(sql)
            rows = cur.fetchall()
            organizationList = {}
            organizationList['rows'] = [(0, "选择全部", )] + rows
            organizationList['struct'] = 'id, name'

            allData['organizationList'] = organizationList

            # 所有供应商清单
            sql = """
                select vl.id, po.name
                from public.vendor_list vl
                inner join public.organization po on po.id = vl.vendor_id
                where po.system_user_id = %d
                """%systemUserID

            cur.execute(sql)
            rows = cur.fetchall()
            allVendorList = {}
            allVendorList['rows'] = [(0, "选择全部", )] + rows
            allVendorList['struct'] = 'id, name'

            allData['allVendorList'] = allVendorList

            # 获取供应商状态类别清单
            sql  = "select cv.code, cv.name "
            sql += " from system.code_value cv"
            sql += " where cv.type_code = 'VENDOR_COOPERATION_STATUS' "
            sql += " order by cv.sort"

            cur = self.db.getCursor()
            cur.execute(sql)
            rows = cur.fetchall()

            statusTypeList = {}
            statusTypeList['struct']   = "code, name"
            statusTypeList['rows']     = [('0', "选择全部", )] + rows

            allData['statusTypeList']     = statusTypeList

            # 获取供应商类型清单
            sql  = "select cv.code, cv.name "
            sql += " from system.code_value cv"
            sql += " where cv.type_code = 'VENDOR_TYPE' "
            sql += " order by cv.sort"

            cur = self.db.getCursor()
            cur.execute(sql)
            rows = cur.fetchall()

            vendorTypeList = {}
            vendorTypeList['struct']   = "code, name"
            vendorTypeList['rows']     = [('0', "选择全部", )] + rows

            allData['vendorTypeList']     = vendorTypeList

        elif op == 'orginfo':
            paramData   = self.getRequestData()
            orgid       = int(self.getParam(paramData, "vid", "0"))

            sql = """
            select po.id, po.name, po.name_en, po.abbr, po.abbr_en, po.organization_type, po.registe_addr, po.current_addr, po.business_scope,
                po.contactor_name, po.contactor_mobile, po.description, po.system_user_id
            from public.organization po
            where po.id = %d
            """%orgid

            cur.execute(sql)
            rows = cur.fetchall()
            organizationList = {}
            organizationList['rows'] =  rows
            organizationList['struct'] = 'id, name, name_en, abbr, abbr_en, organization_type, registe_addr, current_addr, business_scope, contactor_name, contactor_mobile, description, system_user_id'

            allData['organizationList'] = organizationList


        elif op == 'area':
            # 根据供应商编码获取相应的区域数据
            code = self.getParam(paramData, 'code', '')
            if code == '':
                raise BaseError(801, '缺少供应商编码！')

            # 根据编码数据获得地址代码

            # 供应商代码规则：VDR-SSPPPCCCDDD-XXXXXX，SS为国家代码，PPP为省份代码，CCC为城市代码，DDD为区县代码
            countryCode     = code[4:6]
            provinceCode    = code[6:9]
            cityCode        = code[9:12]
            districtCode    = code[12:15]

            logI("Code: %s, %s, %s, %s, %s"%(code, countryCode, provinceCode, cityCode, districtCode))
            countryData     = {}
            provinceData    = {}
            cityData        = {}
            districtData = { 'id' : 0, 'area_name': '', 'area_name_en':''}

            # 查找国家代码
            
            db = self.getDB('system.area_code')
            result = db.findByCond("id,area_name,area_name_en", "area_type = 'S' and area_code = '%s' "%(countryCode))
            if len( result['rows'] ) == 0 :
                # 如果没有找到国家代码，则默认为中国
                result = db.findByCond("id,area_name,area_name_en", "area_code = '%s'"%defaultCountryCode)
                if len(result['rows']) == 0:
                    raise BaseError(801, '系统错误：区域代码中无默认国家代码（%s）数据'%defaultCountryCode)

            countryData['id']           = result['rows'][0][0]
            countryData['area_name']    = result['rows'][0][1]
            countryData['area_name_en'] = result['rows'][0][2]

            # 查找省份
            result = db.findByCond("id,area_name,area_name_en", "area_type = 'P' and area_code = '%s' and parent_area_id = %d "%(provinceCode, countryData['id']))
            if len( result['rows'] ) == 0 :
                raise BaseError(801, '系统错误：获取省份代码（%s）失败！'%provinceCode)

            provinceData['id']           = result['rows'][0][0]
            provinceData['area_name']    = result['rows'][0][1]
            provinceData['area_name_en'] = result['rows'][0][2]

            # 查找城市
            result = db.findByCond("id,area_name,area_name_en", "area_type = 'C' and area_code = '%s' and parent_area_id = %d "%(cityCode, provinceData['id']))
            if len( result['rows'] ) == 0 :
                raise BaseError(801, '系统错误：获取城市代码（%s）失败！'%cityCode)

            cityData['id']           = result['rows'][0][0]
            cityData['area_name']    = result['rows'][0][1]
            cityData['area_name_en'] = result['rows'][0][2]

            # 查找区县
            if districtCode != '000':
                # 非直辖市，一定有区县代码
                result = db.findByCond("id,area_name,area_name_en", "area_type = 'D' and area_code = '%s' and parent_area_id = %d "%(districtCode, cityData['id']))
                if len( result['rows'] ) == 0 :
                    raise BaseError(801, '系统错误：获取区县代码（%s）失败！'%districtCode)

                districtData['id']           = result['rows'][0][0]
                districtData['area_name']    = result['rows'][0][1]
                districtData['area_name_en'] = result['rows'][0][2]

            allData['countryData']  = countryData
            allData['provinceData'] = provinceData
            allData['cityData']     = cityData
            allData['districtData'] = districtData

        elif op == 'check':
            # 审核供应商
            
            userInfo = self.GetUserInfo()

            vendorID    = int(self.getParam( paramData, "id", "0"))
            vendorName  = self.getParam( paramData, "vendor_name", "")
            result      = self.getParam( paramData, "result", "")
            opinion     = self.getParam( paramData, "opinion", "")

            currentTime = GetSystemTime()

            data = {
                'vendor_id'     : vendorID,
                'checker_id'    : userInfo['id'],
                'checker_name'  : userInfo['name'],
                'check_date'    : currentTime,
                'check_opinion' : opinion,
                'check_result'  : result,
            }

            s = vendorManage(self.db)
            # 新增审批记录
            s.save(data, table="public.vendor_check_history")
            logInfo = "%s 审批了供应商 %s ，审批结果：%s, 审批意见：%s， 时间：%s"%(data['checker_name'], vendorName, result, opinion, data['check_date'] )
            operation_log(self.db).addLog(self.GetUserInfo(), "vendorManage", logInfo, vendorID)
            
            # 更新供应商状态
            data = {
                'certifaction_status' : 'CO' if result == 'pass' else 'CI',
                'certifaction_date'     : GetSystemTime('-', '', ''),
            }

            s.save(data, vendorID, table = 'public.vendor_list')
            logInfo = "%s 更新了供应商 %s 状态为 %s，时间：%s"%(userInfo['name'], vendorName, data['certifaction_status'], currentTime )
            operation_log(self.db).addLog(self.GetUserInfo(), "vendorManage", logInfo, vendorID)


        self.response(allData)

