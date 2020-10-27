#coding:utf-8

import re
import time
import datetime
import sys

from db import dbMysql
from web.baseException import errorDic,BaseError
from web.Base  import WebRequestHandler,BaseError,operator_except

from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE
from libs.utils.debug import *

import xlrd, xlwt
import openpyxl
from public.excel import excel
from config import PisConfig

from pis import programInfo

class programUnitManage(dbMysql.CURD) :
    def __init__(self,db) :
        if sys.version > '3':
            # python 3.0 +
            super().__init__(db,'pis.program_unit',False) # 定义本实例需要操作的表名
        else :
            # python 2.7
            super(programUnitManage,self).__init__(db,'pis.program_unit',False)



class Restful(WebRequestHandler):


    # 获取节目栏信息
    # 参数：
    #     无
    # 返回：
    @operator_except
    def get(self):

        raise BaseError(801, "功能建设中。。。")

    # 创建节目栏
    @operator_except
    def post(self):

        alldata     = self.getRequestData()

        s = programUnitManage(self.db)
        
        lsData = {
            'name'         : 'name',
            'start_time' : 'start_time', 
            'end_time'     : 'end_time',
            'list_id'     : 'list_id',
            'sort'          : 'sort',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass
        
        db = self.getDB("pis.program_unit")

        # 检查开始时间和结束时间是否重复
        cond  = " (start_time < '%s' and end_time > '%s' ) "%( data['start_time'], data['end_time'] )
        cond += " or ( start_time > '%s' and end_time < '%s' )"%( data['start_time'], data['end_time'] )
        cond += " and list_id = %d "%data['list_id']

        result = db.findByCond("id", cond )
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "节目栏时间 %s - %s 重复！"%(data['start_time'], data['end_time'] ))


        # 新建节目栏

        data['create_id']     = self.objUserInfo['id']
        data['create_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        
        # 创建节目栏记录
        id = s.save(data, table = 'pis.program_unit')
        data['code'] = id
        s.save(data, id, table = 'pis.program_unit')

        # 创建节目栏成功，记录日志并退出
        operation_log(self.db).addLog(self.GetUserInfo(), "programUnitManage", 
            "创建节目栏信息：开始时间：%s，结束时间：%s"%( data['start_time'], data['end_time']), id)

        data['id'] = id
        self.response(data)

    # 更新节目栏信息
    # 参数：
    #   id: 待更新节目栏的ID，必选
    #   其他参数可选
    @operator_except
    def put(self):

        alldata = self.getRequestData()
        if self.getParam( alldata, "id", 0) == 0:
            raise BaseError(801, "必须指定待更新信息的节目栏ID！")

        s = programUnitManage(self.db)
        
        lsData = {
            'id'         : 'id',
            'name'       : 'name',
            'start_time' : 'start_time', 
            'end_time'   : 'end_time',
            'list_id'    : 'list_id',
            'sort'       : 'sort',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        db = self.getDB("pis.program_unit")

        # # 检查名称是否重复
        # result = db.findByCond("name", "name = '%s' and id <> %s "%(data['name'], data['id']) )
        # if len( result['rows'] ) > 0 :
        #     raise BaseError(801, "节目栏名称 %s 重复！"%data['name'])

        # 检查开始时间和结束时间是否重复
        cond  = " (start_time < '%s' and end_time > '%s' "%( data['start_time'], data['end_time'])
        cond += " or start_time > '%s' and end_time < '%s' ) and ( id <> %d )"%( data['start_time'], data['end_time'], data['id'] )
        cond += " and list_id = %d "%data['list_id']
        
        result = db.findByCond("id", cond )
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "节目栏时间 %s - %s 重复！"%(data['start_time'], data['end_time'] ))

        # # 检查序号是否重复
        # result = db.findByCond("id", "sort = %d and list_id = %d and id <> %d"%(data['sort'], data['list_id'], data['id']) )
        # if len( result['rows'] ) > 0 :
        #     raise BaseError(801, "节目栏序号 %d 在节目单中重复！"%data['sort'])


        data['update_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        data['update_id'] = self.objUserInfo['id']
        id = s.save(data, data['id'], table = 'pis.program_unit')

        logInfo = "更新节目栏信息：开始时间：%s，结束时间：%s"%( data['start_time'], data['end_time'])

        operation_log(self.db).addLog(self.GetUserInfo(), "programUnitManage", logInfo, id)
        self.response(data)


    # 删除指定节目栏
    # 参数：
    #      id: 待删除的节目栏ID，必选 
    #  
    #  返回值：
    @operator_except
    def delete(self):

        alldata = self.getRequestData()
        s = programUnitManage(self.db)

        unit_id = self.getParam( alldata, "id", 0)
        if unit_id == 0:
            raise BaseError(801, "必须指定待更删除的节目栏ID！")

        list_id = self.getParam( alldata, "list_id", 0)
        if list_id == 0:
            raise BaseError(801, "必须指定待更删除车站归属的运营线路ID！")


        # 不删除节目栏归属的节目信息
        # programInfo.deleteProgramFile(self, list_id, unit_id, 0, alldata)

        logInfo = "删除节目栏：开始时间：%s, 结束时间：%s"%(alldata['start_time'], alldata['end_time'])

        # 删除节目栏记录
        s.remove( unit_id, table = "pis.program_unit", key = "id", delete = True)

        # 记录日志并退出
        operation_log(self.db).addLog(self.GetUserInfo(), "programUnitManage", logInfo, unit_id)

        self.response(unit_id)


    @operator_except
    def patch(self):
        paramData = self.getRequestData()
        op = self.getParam( paramData, "op", "")
        if op == "":
            raise BaseError(801, "参数错误：缺少操作模式参数OP！")

        # 同步节目栏
        if op == "sync":
            uid = self.getParam( paramData, "uid", 0 )
            lid = self.getParam( paramData, "lid", 0 )
            if lid == 0:
                raise BaseError(801, "参数错误：缺少节目栏单D！")

            if uid == 0:
                raise BaseError(801, "参数错误：缺少节目栏ID！")

            # 删除其他节目栏的节目文件及记录
            sql = "select pu.id, pu.start_time, pu.end_time, pl.valid_date from pis.program_unit pu "
            sql += " inner join pis.program_list pl on pl.id = %d"%lid
            sql += " where pu.list_id = %d"%lid

            cur  = self.db.getCursor()

            cur.execute(sql)
            rows = cur.fetchall()

            if len(rows) == 0:
                raise BaseError(801, "参数错误：节目单中无节目栏！")

            # 获取节目栏的节目清单
            sql = "select pi.name, pi.program_type, pi.duration, pi.file_name, pi.file_path, pi.program_source, pi.sort, pi.uploader_id, pi.uploader_name, pi.upload_date"
            sql += " from pis.program_info pi"
            sql += " where pi.unit_id = %d"%uid
            sql += " order by pi.sort asc"
            cur.execute(sql)
            programInfoList = cur.fetchall()

            key = ['name', 'program_type', 'duration', 'file_name', 'file_path', 'program_source', 'sort', 'uploader_id', 'uploader_name', 'upload_date']

            valid_date  = ""
            start_time  = ""
            end_time    = ""
            for unitInfo in rows:
                 
                if unitInfo[0] == uid:
                    continue

                start_time     = unitInfo[1]
                end_time     = unitInfo[2]
                valid_date    = unitInfo[3]

                # 删除此节目栏的节目记录，不删除节目文件
                programInfo.deleteProgramFile(self, lid, unitInfo[0], 0, 0 )

                # 复制节目栏节目
                for item in programInfoList:
                    programData = dict(zip(key, item))
                    programData['list_id']         = lid
                    programData['unit_id']         = unitInfo[0]

                    programData['create_time']     = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
                    programData['create_id']     = self.objUserInfo['id']

                    s = programUnitManage(self.db)
                    id = s.save( programData, table = 'pis.program_info')
                    programData['code'] = id
                    s.save(programData, id, table = 'pis.program_info')

            # 记录日志
            logInfo = "%s 同步了节目栏(有效期：%s，开始时间：%s，结束时间：%s)到其他节目栏"%(self.objUserInfo['name'], valid_date, start_time, end_time)
            operation_log(self.db).addLog(self.GetUserInfo(), "programUnit", logInfo, uid)

        elif op == "upload":
            self.uploadProgramList(paramData)


        self.response(0)

    # 上传节目单文件
    def uploadProgramList(self, paramData):

        lid = int(self.getParam( paramData, "lid", "0"))
        if lid == 0:
            raise BaseError(801, "参数错误：缺省节目单ID！")

        uid = int(self.getParam( paramData, "uid", "0"))
        if uid == 0:
            raise BaseError(801, "参数错误：缺省节目栏ID！")

        filename = self.getParam( paramData, "file", "")
        if filename == "":
            raise BaseError(801, "参数错误：缺省上传文件名称！")

        cur = self.db.getCursor()

        # 删除现有节目栏节目
        sql = "delete from pis.program_info where unit_id = %d"%uid
        cur.execute(sql)

        # 判断文件类别是 xls 还是 xlsx
        filetype = self.getParam( paramData, "type", "")
        if filetype == "xls":

            data = xlrd.open_workbook(filename)
            table = data.sheet_by_index(0)
            nRows = table.nrows    #行总数  
            if nRows < 4:
                raise BaseError(801, "文件无内容！")

            nCols = table.ncols    #列总数
            if nCols < 4 :
                raise BaseError(801, "节目栏数据需要包括四列：SN、播出时间、节目名称、时长")  #参数错误

            sort = 1
            rowNum = 3

            while rowNum < nRows:
                logI("行数：%d" % rowNum)
                row = table.row_values(rowNum)                          # 以列表格式输出  
                if row:
                    filename = row[2]
                    duration = row[3]

                    self.InsertProgramInfo(lid, uid, filename, duration, sort)
                    sort += 1
                    rowNum += 1

        elif filetype == "xlsx":
            wb = openpyxl.load_workbook(filename)
            sheet = wb.active
            nRows = sheet.max_row                                       # 行总数
            if nRows < 4:
                raise BaseError(801, "文件无内容！")

            sort = 1
            rowNum = 2
            while rowNum <= nRows:
                filename = sheet["C%d"%rowNum].value
                duration = sheet["D%d"%rowNum].value.strftime('%H:%M:%S')

                self.InsertProgramInfo(lid, uid, filename, duration, sort)
                rowNum += 1
                sort += 1

        else:
            raise BaseError(801, "文件格式错误！")

    # 新增节目栏节目记录
    def InsertProgramInfo(self, lid, uid, file_name, duration, sort):
        cur = self.db.getCursor()

        # 检查节目文件是否已经存在
        sql = "select file_name, file_path from pis.program_file where file_name = '%s'"%file_name
        cur.execute(sql)
        # logI("查询：%s"%sql)
        rows = cur.fetchall()
        if len(rows) == 0:
            raise BaseError(801, "节目单中文件 %s 不存在，请先上传该文件!"%file_name)

        # 文件类型是否支持
        filenameItem = file_name.split(".")
        itemLen = len(filenameItem)
        if itemLen < 2:
            raise BaseError(801, "文件名 %s 无扩展名"%file_name)

        filetype = filenameItem[itemLen - 1]
        if filetype.lower() not in PisConfig['videoType']:
            raise BaseError(801, "节目文件 %s 类型不是mp4文件！"%file_name)

        filePath = rows[0][1]
        fileName = os.path.basename(filePath)
        fileInfo = {}
        fileInfo['name']            = file_name
        fileInfo['list_id']         = lid
        fileInfo['unit_id']         = uid

        fileInfo['program_type']    = filetype.upper()
        fileInfo['file_name']       = fileName
        fileInfo['file_path']       = filePath
        fileInfo['sort']            = sort
        fileInfo['duration']        = duration
        sort = sort + 1
        fileInfo['program_source']  = ""

        fileInfo['uploader_id']     = self.objUserInfo['id']
        fileInfo['uploader_name']   = self.objUserInfo['name']
        fileInfo['upload_date']     = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')

        fileInfo['create_time']     = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        fileInfo['create_id']       = self.objUserInfo['id']

        s = programUnitManage(self.db)
        id = s.save( fileInfo, table = 'pis.program_info')
        fileInfo['code'] = id
        s.save(fileInfo, id, table = 'pis.program_info')

        # 记录日志
        logInfo = "%s 上传了节目文件：名称：%s"%(self.objUserInfo['name'], file_name)
        operation_log(self.db).addLog(self.GetUserInfo(), "programFile", logInfo, id)


# 获取指定节目单的节目栏信息
# 参数：
#     pid： 节目单ID
#     
# 返回值：以节目单名称为根节点的树结构，一级子节点为节目栏，二级子节点为节目
def getProgramUnitList(self, pid):

    cur  = self.db.getCursor()

    sql  = "select pu.id, pu.name, pu.code, pu.start_time, pu.end_time, pu.list_id, pl.name, pu.sort "
    sql += " from pis.program_unit pu"
    sql += " inner join pis.program_list pl on pl.id = %d and pl.id = pu.list_id"%pid
    sql += " order by pu.start_time"

    cur.execute(sql)
    rows = cur.fetchall()

    #将所有元组转为对象,因为前台tree组件无法加载元组类型
    programUnitList = []
    nameSort = 1
    for row in rows:
        obj = {}
        obj['id']                   = row[0]
        obj['name']                 = "%02d  "%nameSort +  row[3] + ' - ' + row[4]
        obj['show_name']        = row[1]
        obj['code']                 = row[2]
        obj['start_time']        = row[3]
        obj['end_time']         = row[4]
        obj['list_id']             = row[5]
        obj['list_name']        = row[6]
        obj['sort']                = row[7]
        
        obj['nodeType']         = 'program_unit'
        obj['children']         = []
        obj['chkDisabled']         = True
        obj['open']                 = True
        obj['checked']          = False
        obj['isItem']           = False

        nameSort += 1
        programUnitList.append(obj)

    return programUnitList
