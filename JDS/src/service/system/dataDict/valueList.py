#coding:utf-8
from libs.web.Base  import WebRequestHandler,BaseError,operator_except
import time
import datetime
from system.operationLog.entity import operation_log,LOG_ADD,LOG_UPDATE,LOG_DELETE

import sys
from libs.db import dbMysql
from libs.web.baseException import errorDic,BaseError

class DataDictValue(dbMysql.CURD) :
    def __init__(self,db) :
        if sys.version > '3':
            # python 3.0 +
            super().__init__(db,'system.code_value',False) # 定义本实例需要操作的表名
        else :
            # python 2.7
            super(DataDictValue,self).__init__(db,'system.code_value',False)

class Restful(WebRequestHandler):
    @operator_except
    def get(self):

        type_code    = self.get_argument('type_code',default='')
        if type_code == '':
            raise BaseError(801, "参数错误：无数据字典类型Code！")

        rowdata = {}

        cur  = self.db.getCursor()
        sql  = """
            select cv.id, cv.type_id, cv.type_code, cv.code, cv.name as valuename, cv.sort, cv.status, 
                cv.create_id, cv.create_time, cv.update_id, cv.update_time, cv.description, b.name, c.name 
            from system.code_value cv 
            left join system.account b on cv.create_id=b.id 
            left join system.account c on  cv.update_id=c.id 
            where cv.type_code='%s' 
            order by cv.sort 
            """%type_code

        cur.execute(sql)
        rows = cur.fetchall()
        rowdata['struct']="id, type_id, type_code, code, name, sort, status, create_id, create_time, update_id, update_time, description, update_name, create_name"
        rowdata['rows']= rows
        self.response(rowdata)

    # 创建数据字典条目
    @operator_except
    def post(self):
        # 取POST Form提交的数据
        alldata = self.getRequestData()
        s       = DataDictValue(self.db)

        lsData = {
        	'type_id' 	: 'type_id',
        	'type_code' : 'type_code',
            'code'      : 'code',
            'name'      : 'name',
            'sort'      : 'sort',
            'status'    : 'status', 
            'description'    : 'description',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        if 'code' not in data.keys():
            raise BaseError(801, "参数错误，无数据字典值code！")

        if 'name' not in data.keys():
            raise BaseError(801, "参数错误，无数据字典值名称！")

        if 'sort' not in data.keys():
            raise BaseError(801, "参数错误，无数据字典值序号！")

        if 'status' not in data.keys():
            data['status'] = 1


        db = self.getDB("system.code_value")

        # 检查名称是否重复
        result = db.findByCond("code", "type_code = '%s' and (code = '%s' or name = '%s') "
            %(data['type_code'], data['code'], data['name']))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "数据错误：字典条目编号 %s 或名称 %s 已经存在！"%(data['code'], data['name']))

        # 检查顺序是否重复
        result = db.findByCond("code", "type_code = '%s' and (sort = '%s')"%(data['type_code'], data['sort']))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "数据错误：字典条目 %s 序号 %s 重复！"%(data['name'], data['sort']))


        # 创建字典条目
        data['create_id']   = self.objUserInfo['id']
        data['create_time'] = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')

        id = s.save(data, table = 'system.code_value')

        operation_log(self.db).addLog(self.GetUserInfo(), "dictManage", self.objUserInfo['name'] + " 新增字典值 " + data['name'], id)
        self.response(id)

    # 更新数据字典条目
    @operator_except
    def put(self):
        alldata = self.getRequestData()
        s       = DataDictValue(self.db)

        # 字典条目归属的字段类型ID及code默认不可能改变
        lsData = {
        	'id' 		: 'id',
            'type_id'   : 'type_id',
            'type_code' : 'type_code',            
            'code'      : 'code',
            'name'      : 'name',
            'sort'      : 'sort',
            'status'    : 'status', 
            'description' : 'description',
        }

        data = {}
        for (k, v) in lsData.items():
            try:
                data[k] = alldata[v]
            except:
                pass

        if 'id' not in data.keys():
            raise BaseError(801, "参数错误，无数据字典值ID！")

        if 'code' not in data.keys():
            raise BaseError(801, "参数错误，无数据字典值code！")

        if 'name' not in data.keys():
            raise BaseError(801, "参数错误，无数据字典值名称！")

        if 'sort' not in data.keys():
            raise BaseError(801, "参数错误，无数据字典值序号！")

        if 'status' not in data.keys():
            data['status'] = 1

        db = self.getDB("system.code_value")
        # 检查名称是否重复
        result = db.findByCond("code", "type_code = '%s' and (code = '%s' or name = '%s') and id <> %d "
            %(data['type_code'], data['code'], data['name'], data['id']))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "数据错误：字典条目编号 %s 或名称 %s 已经存在！"%(data['code'], data['name']))

        # 检查顺序是否重复
        result = db.findByCond("code", "type_code = '%s' and sort = '%s' and id <> %d"
            %(data['type_code'], data['sort'], data['id']))
        if len( result['rows'] ) > 0 :
            raise BaseError(801, "数据错误：字典条目 %s 序号 %s 重复！"%(data['name'], data['sort']))

        id = s.save(data, data['id'], table='system.code_value', key = 'id')
        operation_log(self.db).addLog(self.GetUserInfo(), "dictManage", self.objUserInfo['name'] + " 修改字典值 " + data['name'], data['id'])

        self.response(data)

    @operator_except
    def delete(self):
        alldata = self.getRequestData()

        s = DataDictValue(self.db)
        r = s.remove(alldata['id'], delete = True)

        operation_log(self.db).addLog(self.GetUserInfo(), "dictManage", self.objUserInfo['name'] + " 删除字典值 " + alldata['name'], alldata['id'])
            
        self.response(r) 

    @operator_except
    def patch(self):
        alldata = self.getRequestData()
        cur     =self.db.getCursor()

        sql = "select code,name from system.code_value where type_code='%s' and status='1' order by sort asc"%(alldata['type_code'])
        cur.execute(sql);
        rows = cur.fetchall()
        
        rowdata ={}
        rowdata['struct'] = "code,name"
        rowdata['rows']   = rows
        self.response(rowdata)        
