#coding:utf-8

import re
import time
import datetime
import sys

from db import dbMysql
from web.baseException import errorDic,BaseError
from web.Base  import WebRequestHandler,BaseError,operator_except

import xlrd, xlwt
from public.excel import excel

from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE
from libs.utils import utils
from utils.debug import *

from libs.JCL.JCLDBTools import *

class oaBulletinBoardManager(dbMysql.CURD) :
    def __init__(self,db) :
        if sys.version > '3':
            # python 3.0 +
            super().__init__(db,'oa.bulletinboard', False) # 定义本实例需要操作的表名
        else :
            # python 2.7
            super(oaBulletinBoardManager,self).__init__(db,'oa.bulletinboard', False)

class Restful(WebRequestHandler):

    # 获取公告牌列表
    # 参数：

    @operator_except
    def get(self):
        
        op              = self.get_argument('op',  default = "data")
        startDate       = self.get_argument('sd',  default = '')
        endDate         = self.get_argument('ed', default = '')
        bulletinContent = self.get_argument('bc', default = '')
        bulletinStatus  = self.get_argument('status', default = 'checked')

        offset  = int(self.get_argument('o',  default = '1'))
        rowlimit= int(self.get_argument('r',  default = '20'))

        offset = ( offset - 1 ) * rowlimit
        sql_limit = " limit " + str(rowlimit) + " offset " + str(offset)

        sql  = """
            select bb.id, bb.code, bb.title, bb.publisher_id, ac.name, bb.publisher_dept, bb.publish_date, 
                bb.content, bb.file_path, bb.status, bb.apply_date, bb.checker_id, ac2.name, 
                bb.check_date, bb.check_opinion 
                from oa.bulletinboard bb
                inner join  system.account ac on ac.id = bb.publisher_id
                left join  system.account ac2 on ac2.id = bb.checker_id
            """

        sql_where = ""
        if startDate != "":
            sql_where = " where bb.publish_date >= '%s'"%startDate

        if endDate != "":
            if sql_where == "":
                sql_where = " where bb.publish_date <= '%s'"%endDate
            else:
                sql_where += " and bb.publish_date <= '%s'"%endDate

        if bulletinContent != "":
            if sql_where == "":
                sql_where = " where bb.content = '%s'"%bulletinContent
            else:
                sql_where += " and bb.content = '%s'"%bulletinContent
    
        if bulletinStatus == "checked":
            # 只需要已经审核通过对公告
            if sql_where == "":
                sql_where = " where bb.status = '5'"
            else:
                sql_where += " and bb.status = '5'"
        else:
            # 只需要未经过审核的公告
            if sql_where == "":
                sql_where = " where bb.status <> '5'"
            else:
                sql_where += " and bb.status <> '5'"

        sql += sql_where
        sql += " order by bb.publish_date"

        # logI(sql)
        
        if op == "data":
            sql += sql_limit

        cur = self.db.getCursor()
        cur.execute(sql)
        rows = cur.fetchall()

        structs = """
            id, code, title, publisher_id, publisher_name, publusher_dept, publish_date, content, 
            file_path, status, apply_date, checker_id, checker_name, check_date, check_opinion"""

        if op == "excel":
            path = self.exportToExcel(structs, rows )
            self.response(path)
        else:

            bulletinList = {}
            bulletinList['rows'] = rows
            bulletinList['struct']  = structs

            sql  = "select count(bb.id) from oa.bulletinboard bb"
            sql += sql_where

            cur.execute(sql)
            row = cur.fetchone() 
            bulletinList['count'] = row[0]

            self.response( bulletinList )


    # 输出日志到excel文件
    def exportToExcel(self, structs, rowdata):
      
        bulletinList = []
        bulletinList = ConvertDataToTuple(structs, rows)

        cur = self.db.getCursor()
        utils.getDataDict(cur, bulletinList, "device_type", "GPS_MODULE_DEVICE_TYPE", "device_type_name")
        utils.getDataDict(cur, bulletinList, "install_type", "GPS_MODULE_INSTALL_TYPE", "install_type_name")
        utils.getDataDict(cur, bulletinList, "status", "GPS_MODULE_STATUS", "status_name")

        data = []
        sn = 1
        for module in bulletinList:

            itemData = []
            itemData.append( sn )
            sn = sn + 1
            itemData.append( module['name'] )
            itemData.append( module['code'] )
            itemData.append( module['serial_no'] )
            itemData.append( module['device_type_name'] )
            itemData.append( module['vendor'] )
            itemData.append( module['mac_addr'] )
            itemData.append( module['install_date'])
            itemData.append( module['install_batch'] )
            itemData.append( module['install_type_name'] )
            itemData.append( module['status_name'] )
            itemData.append( module['addr'] )

            data.append(itemData) 

        struct = "SN, 名称, 编号, 序列号, 设备类型, 厂家, MAC地址, 安装日期, 安装批次, 安装类型, 状态, 最新位置"

        path = {}
        path = excel.createTempFile("xls")

        excel.saveExcel(path['path'], struct, data )
        
        logInfo = " %s 下载了定位模块清单 "%self.objUserInfo['name']
        operation_log(self.db).addLog(self.GetUserInfo(), "oaBulletinBoardManager", logInfo, 0)

        return path

    # 保存公告
    @operator_except
    def post(self):

        alldata = self.getRequestData()

        SystemUserID = self.GetSystemUserID()

        s = oaBulletinBoardManager(self.db)
        
        lsData = {
            'code'              : 'code',
            'title'             : 'title', 
            'publisher_id'      : 'publisher_id',
            'publisher_dept'    : 'publisher_dept',
            'content'           : 'content', 
            'publish_date'      : 'publish_date',
            'apply_date'        : 'apply_date',
            'checker_id'        : 'checker_id',
            'check_date'        : 'check_date',
            'check_opinion'     : 'check_opinion',
            'status'            : 'status',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        db = self.getDB("oa.bulletinboard")

        # 检查公告编号模块名称是否重复
        result = db.findByCond("code", "code = '%s'"%(data['code']))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "公告编号 %s 已经存在！"%data['code'])

        data['create_id']   = self.objUserInfo['id']
        data['create_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        
        id = s.save(data, table = 'oa.bulletinboard')

        # 更新参数表中的公告编号值
        year = datetime.datetime.now().year
        sql = """
            select id, int_value from oa.oa_system_parameter where item='bulletinCode' and year = %d
            """%year
        cur = self.db.getCursor()
        cur.execute(sql)
        rows = cur.fetchall()

        code    = 1
        rec_id  = 0
        if len(rows) > 0:
            code = rows[0][1] + 1
            rec_id = rows[0][0]

        code_data = { 
            'system_user_id' : SystemUserID,
            'item'      : 'bulletinCode',
            'int_value' : code,
            'year'      : year,
        }

        if rec_id == 0:
            s.save( code_data, table="oa.oa_system_parameter" )
        else:
            s.save( code_data, rec_id, table="oa.oa_system_parameter" )

        # 创建公告成功，记录日志并退出
        operation_log(self.db).addLog(self.GetUserInfo(), "oaBulletinBoardManage", 
            "%s 创建公告成功：名称：%s，编号：%s"%(self.objUserInfo['name'], data['title'], data['code']), id)

        self.response(id)

    # 更新公告
    @operator_except
    def put(self):
        
        alldata = self.getRequestData()
        if self.getParam( alldata, "id", 0) == 0:
            raise BaseError(801, "必须指定待更新信息的公告ID！")

        s = oaBulletinBoardManager(self.db)

        # 公告牌编号不能更新，因此也无需检查是否重复
        lsData = {
            'id'                : 'id',
            'title'             : 'title', 
            'publisher_id'      : 'publisher_id',
            'publisher_dept'    : 'publisher_dept',
            'content'           : 'content', 
            'publish_date'      : 'publish_date',
            'apply_date'        : 'apply_date',
            'checker_id'        : 'checker_id',
            'check_date'        : 'check_date',
            'check_opinion'     : 'check_opinion',
            'status'            : 'status',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        db = self.getDB("oa.bulletinboard")

        # 保存数据
        data['update_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        data['update_id'] = self.objUserInfo['id']

        id = s.save(data, data['id'], table = 'oa.bulletinboard')

        operation_log(self.db).addLog(self.GetUserInfo(), "oaBulletinBoardManage", 
            "%s 更新公告成功：名称：%s，编号：%s"%(self.objUserInfo['name'], data['title'], alldata['code']), id)

        self.response(id)

    # 删除公告
    @operator_except
    def delete(self):
        
        alldata = self.getRequestData()
        if self.getParam( alldata, "id", 0) == 0:
            raise BaseError(801, "必须指定待删除的公告ID！")

        bid = self.getParam( alldata, "id", 0)

        s = oaBulletinBoardManager(self.db)

        sql  = " select bb.title, bb.code from oa.bulletinboard bb "
        sql += " where bb.id = %s"%bid

        cur = self.db.getCursor()
        cur.execute(sql)
        rows = cur.fetchall()
        logInfo = "删除公告：标题：%s, 编号：%s"%(rows[0][0], rows[0][1])

        # 删除记录，将数据移动到历史表中
        MoveDeleteRecord("oa", "bulletinboard", bid, self.GetUserID())

        # 记录日志并退出
        operation_log(self.db).addLog(self.GetUserInfo(), "BulletinBoardManage", logInfo, bid)

        self.response(bid)


    @operator_except
    def patch(self):

        paramData = self.getRequestData()
        SystemUserID = self.GetSystemUserID()

        op = self.getParam(paramData, "op", "")
        if op == "":
            raise BaseError(801, "参数错误：缺少操作类型！")

        # 获取公告编号
        if op == "code":
            s = oaBulletinBoardManager(self.db)

            year = datetime.datetime.now().year
            sql = """
                select id, int_value from oa.oa_system_parameter 
                where item='bulletinCode' and year = %d and system_user_id = %d
                """%(year, SystemUserID)
            cur = self.db.getCursor()
            cur.execute(sql)
            rows = cur.fetchall()

            code    = 1
            rec_id  = 0
            if len(rows) > 0:
                code    = rows[0][1] + 1
                rec_id  = rows[0][0]

            # logI("SQL: %s, Code = %d"%(sql, code))

            code_data = { 
                'system_user_id': SystemUserID,
                'item'          : 'bulletinCode',
                'int_value'     : code,
                'year'          : year,
            }
            if rec_id == 0:
                s.save( code_data, table="oa.oa_system_parameter" )
            else:
                s.save( code_data, rec_id, table="oa.oa_system_parameter" )

            self.response(code)
        
        elif op == "upload":
            # 批量上传定位模块
            fid = int(self.getParam(paramData, "fid", "0"))
            if fid == 0:
                raise BaseError(801, "参数错误：缺少上传文件ID！")


            # 获取文件
            sql = "select a.store_path from public.file a where a.id = %s "%fid

            cur.execute(sql)
            rows = cur.fetchall()
            if len(rows) == 0:
                raise BaseError(801, "参数错误：文件ID错误")

            filename = rows[0][0]

            excel = xlrd.open_workbook( filename )
            table = excel.sheet_by_index(0)
            nrows = table.nrows #行总数  
            ncols = table.ncols #列总数
            if ncols != 2 :
                msg = "数据中只需要包含列参数，第一行为标题。"
                logE("数据中只需要包含卡号、打卡时间两列参数：" )
                raise BaseError(801, msg)

            # 文件格式待定
            
            self.response(0)
            
