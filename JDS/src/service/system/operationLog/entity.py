#coding:utf-8
import sys
from db import dbMysql
from web.baseException import errorDic,BaseError
import time

from libs.utils.debug import *

LOG_ADD     = "增添了记录"
LOG_UPDATE  = "修改了记录"
LOG_DELETE  = "删减了记录"

class operation_log(dbMysql.CURD) :
    def __init__(self,db) :
        if sys.version > '3':
            # python 3.0 +
            super().__init__(db,'system.operation_log',False) # 定义本实例需要操作的表名
        else :
            # python 2.7
            super(operation_log,self).__init__(db,'system.operation_log',False)
    
    #添加日志,使用方法参考数据权限list.py
    def addLog(self, userInfo, menu_code, operation_context, operation_id, operation_no = ""):
        operator_id = userInfo['id']
        if not operator_id or not menu_code or not operation_context:
            raise BaseError(801)

        logI("Operation Log: %s"%(operation_context))
        lstData = {
            'operator_id'       : operator_id,
            'menu_code'         : menu_code,
            'operation_context' : operation_context,
            'operation_id'      : operation_id,
            'create_time'       : time.strftime("%Y-%m-%d %H:%M:%S",time.localtime(time.time())),
            'operation_no'      : operation_no,
            'system_user_id'    : userInfo['system_user_id']
        }

        id = self.save(lstData, table = 'system.operation_log')
        if id <= 0:
            raise BaseError(703)

        return True
    
    #添加日志
    def addLog2(self, userInfo, logData):
        if not logData['operator_id'] or not logData['menu_code'] or not logData['operation_context'] or not logData['operation_id']:
            raise BaseError(801)

        logData['system_user_id'] = userInfo['system_user_id']

        logData['create_time']=time.strftime("%Y-%m-%d %H:%M:%S",time.localtime(time.time()))
        id = self.save(logData, table='system.operation_log')
        if id <= 0:
            raise BaseError(703)
        
        return True
        