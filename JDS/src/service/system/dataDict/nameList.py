#coding:utf-8

from libs.web.Base  import WebRequestHandler,BaseError,operator_except

import time
import datetime
from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE
from libs.utils import debug

import sys
from libs.db import dbMysql
from libs.web.baseException import errorDic,BaseError

class DataDictTypeManage(dbMysql.CURD) :
    def __init__(self,db) :
        if sys.version > '3':
            # python 3.0 +
            super().__init__(db,'system.code_type',False) # 定义本实例需要操作的表名
        else :
            # python 2.7
            super(DataDictTypeManage,self).__init__(db,'system.code_type',False)

class Restful(WebRequestHandler):

    # 查询数据字典数据
    # 参数：
    @operator_except
    def get(self):
        s = DataDictTypeManage(self.db)

        type_code   = self.get_argument('code', default = '')
        rowdata     = {}
        cur         = self.db.getCursor()

        sql = """
            select ct.id,ct.code,ct.name,ct.description,ct.status,ct.create_id,ct.create_time,ct.update_id,ct.update_time,
                b.name as status_name, d.name as create_name,e.name as update_name, c.name
            from system.code_type ct
            left join system.code_value b on ct.status = b.code  
            inner join system.code_type c on b.type_id = c.id and c.code = 'CODE_TYPE_STATUS' 
            left join system.account d on ct.create_id = d.id 
            left join system.account e on ct.update_id = e.id 
            """

        if type_code!='':
            sql += " where ct.code='%s'"%(type_code)

        sql += " order by ct.id"

        cur.execute(sql)
        rows = cur.fetchall()
        rowdata['struct']="id, code, name, description, status, create_id, create_time, update_id, update_time, status_name, create_name, update_name"
        rowdata['rows']= rows
        self.response(rowdata)

    # 新建数据字典
    @operator_except
    def post(self):
        # 取POST Form提交的数据
        alldata = self.getRequestData()
       
        lsData = {
            'code'      : 'code',
            'name'      : 'name',
            'status'    : 'status', 
            'description' : 'description',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        s  = DataDictTypeManage(self.db)

        db = self.getDB("system.code_type")

        # 检查名称是否重复
        result = db.findByCond("code", "code = '%s' or name = '%s' "%(data['code'], data['name']))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "数据错误：字典类型编号 %s 或名称 %s 已经存在！"%(data['code'], data['name']))

        id = s.save(data, table = 'system.code_type')

        operation_log(self.db).addLog(self.GetUserInfo(), "dictManage", self.objUserInfo['name'] + " 创建数据字典 " + alldata['name'], id)
        self.response(id)
    
    # 更新数据字典
    @operator_except
    def put(self):
        alldata = self.getRequestData()
        s          = DataDictTypeManage(self.db)

        lsData = {
            'id'        : 'id',
            'code'      : 'code',
            'name'      : 'name',
            'status'    : 'status', 
            'description' : 'description',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        # 检查名称是否重复
        db = self.getDB("system.code_type")
        result = db.findByCond("code", "( code = '%s' or name = '%s' ) and id <> %s "%(data['code'], data['name'], data['id']) )
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "数据错误：字典类型编号 %s 或名称 %s 已经存在！"%(data['code'], data['name']))

        id = s.save(data, data['id'], table='system.code_type', key='id')
        operation_log(self.db).addLog(self.GetUserInfo(), "dictManage", "更新数据字典信-%s"%data['name'], data['id'])
        self.response(data)
            
    # 删除字典
    @operator_except
    def delete(self):
        alldata = self.getRequestData()
        s = DataDictTypeManage(self.db)

        r = s.remove(alldata['id'], delete=True)

        operation_log(self.db).addLog(self.GetUserInfo(), "dictManage", self.objUserInfo['name'] + "删除字典" + alldata['name'], alldata['id'])

        self.response(r) 


