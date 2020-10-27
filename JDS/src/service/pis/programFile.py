#coding:utf-8

import re
import time
import datetime
import sys
import os

import xml.etree.ElementTree as ET

from db import dbMysql
from web.baseException import errorDic,BaseError
from web.Base  import WebRequestHandler,BaseError,operator_except

import xlrd, xlwt
import openpyxl
from public.excel import excel

from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE
from libs.utils import debug, utils
from utils.debug import *

class program_file(dbMysql.CURD) :
    def __init__(self,db) :
        if sys.version > '3':
            # python 3.0 +
            super().__init__(db,'pis.program_file',False) # 定义本实例需要操作的表名
        else :
            # python 2.7
            super(program_file,self).__init__(db,'pis.program_file',False)



# 参数：
#     fileData：节目文件信息
def uploadProgramFile(self, fileData):

    cur  = self.db.getCursor()

    # 检查文件名是否存在，如果已经存在，则不做操作，此时新的节目文件已经覆盖了就的视频文件
    sql  = "select pf.file_name from pis.program_file pf "
    sql += " where pf.file_name = '%s' and pf.video_type = '%s' "%(fileData['file_name'], fileData['video_type'])

    cur.execute(sql)
    rows = cur.fetchall()
    if len(rows) > 0:
        return

    fileInfo = {}
    fileInfo['file_name']       = fileData['file_name']
    fileInfo['file_path']       = fileData['file_path']

    fileInfo['uploader_id']     = self.objUserInfo['id']
    fileInfo['uploader_name']   = self.objUserInfo['name']
    fileInfo['upload_date']     = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    fileInfo['video_type']      = fileData['video_type']

    fileInfo['create_time']     = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    fileInfo['create_id']       = self.objUserInfo['id']

    s = program_file(self.db)
    id = s.save( fileInfo, table = 'pis.program_file')

    # 记录日志
    logInfo = "%s 上传了节目文件：名称：%s"%(self.objUserInfo['name'], fileInfo['file_name'])
    operation_log(self.db).addLog(self.GetUserInfo(), "programFileManage", logInfo, id)



class Restful(WebRequestHandler):

    # 获取节目信息
    # 参数：
    #     op: list or excel 是下载到文件还是数据清单
    #     uid: 上传人员ID
    #     ut：上传日期

    @operator_except
    def get(self):
        
        op         = self.get_argument('op',  default = "list")
        type       = self.get_argument('type',  default = "1")

        uid        = int(self.get_argument('uid',  default = '0'))
        fid        = int(self.get_argument('fid',  default = '0'))
        ud         = self.get_argument('ud',  default = "")

        offset     = int(self.get_argument('o',  default = '1'))
        rowlimit= int(self.get_argument('r',  default = '20'))

        # logI("Get program File参数：uid:%d, fid:%s, ud:%s"%(uid, fid, ud))

        offset = ( offset - 1 ) * rowlimit
        sql_limit = " limit " + str(rowlimit) + " offset " + str(offset)

        sql  = " select pf.id, pf.create_time, pf.update_time, pf.create_id, pf.update_id, pf.file_name, pf.file_path, "
        sql += " pf.uploader_id, pf.uploader_name, pf.upload_date, pf.video_type"
        sql += " from pis.program_file pf"

        if op == 'list':
            sql_where = " where pf.video_type = '%s' "%type
        else:
            sql_where = "where ( pf.video_type = '1' or pf.video_type = '0' )"

        if uid != 0:
            sql_where = " and pf.uploader_id = %s"%uid

        if fid != 0:
            sql_where += " pf.id = %s"%fid
    
        if ud != "":
            sql_time = "pf.upload_date >= '%s 00:00:00' and pf.upload_date <= '%s 23:59:59'"%(ud, ud)
            sql_where += " and %s"%sql_time

        sql += sql_where
        sql += " order by pf.id DESC"

        if op == "list":
            sql += sql_limit

        cur = self.db.getCursor()
        cur.execute(sql)
        rows = cur.fetchall()

        if op == "excel":
            self.exportToExcel( rows )
        else:

            programFileList = {}
            programFileList['rows'] = rows
            programFileList['struct'] = "id, create_time, update_time, create_id, update_id, file_name, file_path, uploader_id, uploader_name, upload_date, video_type"

            sql  = "select count(pf.id) from pis.program_file pf"
            sql += sql_where

            cur.execute(sql)
            row = cur.fetchone() 
            programFileList['count'] = row[0]

            self.response( programFileList )

    # 输出版式文件列表到excel文件
    def exportToExcel(self, rowdata):

        key = ['id', 'create_time', 'update_time', 'create_id', 'update_id', 'file_name', 'file_path', 'uploader_id', 'uploader_name', 'upload_date', 'video_type']

        fileList = []
        for item in rowdata:
            programInfo = dict(zip(key, item))
            fileList.append(programInfo)

        data = []
        sn = 1
        for item in fileList:
            itemData = []
            itemData.append( sn )
            sn = sn + 1
            itemData.append( item['file_name'] )    
            itemData.append( item['uploader_name'] )        
            itemData.append( item['upload_date'])
            if item['video_type'] == '1':
                itemData.append('节目视频')
            else:
                itemData.append('安全垫片')

            data.append(itemData) 

        struct = "SN, 文件名称, 上传人姓名, 上传日期, 文件类型 "

        path = {}
        path = excel.createTempFile("xls")
                
        excel.saveExcel(path['path'], struct, data )

        logInfo = " %s 下载了节目文件清单 "%self.objUserInfo['name']
        operation_log(self.db).addLog(self.GetUserInfo(), "programFileManage", logInfo, 0)

        self.response(path)

    # 删除指定节目文件
    # 参数：
    #      fileInfo: 待删除的版式文件记录，必选 
    @operator_except
    def delete(self):
        
        fileInfo = self.getRequestData()
        
        s = program_file(self.db)

        id = self.getParam( fileInfo, "id", 0)
        if id == 0:
            raise BaseError(801, "参数错误：缺少待更删除的节目文件ID！")

        filename = self.getParam( fileInfo, "file_name", "")
        if filename == "":
            raise BaseError(801, "参数错误：缺少待更删除的节目文件名称！")

        filePath = self.getParam( fileInfo, "file_path", "")
        if filePath == "":
            raise BaseError(801, "参数错误：缺少待更删除的节目文件路径！")

        logInfo = "%s 删除节目文件：名称：%s"%(self.objUserInfo['name'], filename)

        # 删除节目文件记录
        s.remove( id, table = "pis.program_file", key = "id", delete = True)

        # 删除节目文件
        # logI("删除文件:%s"%filePath)
        if os.path.exists(filePath):
            os.remove(filePath)

        # 记录日志并退出
        operation_log(self.db).addLog(self.GetUserInfo(), "programFileManage", logInfo, id)
        self.response(id)


    # 获取下拉列表信息
    # 参数：
    #     op: list，获取下拉列表
    #     

    @operator_except
    def patch(self):

        paramData = self.getRequestData()
        op = self.getParam( paramData, "op", "")
        if op == "":
            raise BaseError(801, "参数错误：缺少操作模式参数OP")

        cur = self.db.getCursor()

        if op == "list":
            # 返回的数据
            data = {}

            # 版式文件列表
            sql  = "select pf.id, pf.file_name "
            sql += " from pis.program_file pf"
            sql += " order by pf.file_name"
            
            cur.execute(sql)
            rows = cur.fetchall()

            nameList = {}
            nameList['struct']    = "id, name"
            nameList['rows']     = [(0, "选择全部", )] + rows

            data['nameList']     = nameList

            # 上传人员列表
            sql  = "select a.id, a.name "
            sql += " from system.account a"
            sql += " where a.id = (select distinct(uploader_id) from pis.program_file) "
            sql += " order by a.id"

            cur.execute(sql)
            rows = cur.fetchall()

            uploaderList = {}
            uploaderList['struct']   = "id, name"
            uploaderList['rows']     = [('0', "选择全部", )] + rows

            data['uploaderList']     = uploaderList

            self.response(data)

        else:
            raise BaseError(801, "参数错误：操作类型参数OP值错误！")

