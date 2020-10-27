# coding:utf-8

# ------------------------------------------------------------------------------
# JCL - Component :: Database Tools
# Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
# Author: Zhang Weijia
# ------------------------------------------------------------------------------
# Name:    JCLDBTools.py
# Purpose: Database Tools
#  *----------------------------------------------------------------------------

import sys
import time
import datetime

from db import dbMysql
from libs.db.dbHelper import *

from libs.utils.debug import *
from libs.utils.utils import *
from libs.JCL.JCLCommon import *

# 将一个表指定时间之前的数据移动到另外一个表
def moveTableData(table, historyTable, beforeTime):
    dbh = DbHelper(auto=False)
    try :
        db = dbh.openDB(table)
        cur = dbh.getCursor()
        fields = db.getFields()
        sql = "insert into %s (%s) select %s from %s where create_time < '%s'" % (historyTable, fields, fields, table, beforeTime)
        # logI(sql)
        cur.execute(sql)
        sql = "delete from %s where create_time < '%s' " % (table, beforeTime)
        cur.execute(sql)
    except Exception as e:
        onException(e)
    dbh.closeDB()

# 删除一个表的数据
def deleteTableData(table, beforeTime, condtion = ""):
    dbh = DbHelper(auto=False)
    try :
        db = dbh.openDB(table)
        cur = dbh.getCursor()
        sql = "delete from %s where create_time < '%s' " % (table, beforeTime)
        cur.execute(sql)
    except Exception as e:
        onException(e)
    dbh.closeDB()


# 获取指定业务类型的系统用户ID
# 暂时规定：一个类型的业务只能有一个系统用户
def GetSystemUserID(service_name):

    if service_name == None or service_name == "":
        logE("Error: getSystemUserID parameter %s is Wrong!"%service_name)
        return 0

    dbHelper = DbHelper()
    cursor = dbHelper.getCursor()
    sql = "select sl.id from system.system_user_list sl where sl.user_service = '%s' "%service_name
    cursor.execute(sql)
    rows = cursor.fetchall()
    dbHelper.closeDB()

    if len(rows) == 0:
        logE("Error: Can not find system user id of RSU")
        return 0

    return rows[0][0]

# 将数据库查询结果转换为元组
# 参数：
#   structs: 以逗号,分割的字段名字符串
#   data： 数据数组，数组的每个元素是一行记录 
def ConvertDataToTuple(structs, data):
    fieldList = structs.split(",")

    keyList = []
    for field in fieldList:
        key = field.strip()
        keyList.append(key)

    dataList = []
    for record in data:
        rec = dict(zip(keyList, record))
        dataList.append(rec)

    return dataList


# 将删除的数据移动到历史数据表中
def MoveDeleteRecord(schema, table, record_id, op_user_id):
    dbHelper = DbHelper(auto=False)
    try :
        orignalTableName    = schema + "." + table
        targetTableName     = "history." + table

        db = dbHelper.openDB(orignalTableName)
        fields = db.getFields()

        # 将字段ID删除
        fieldList = fields.split(",")
        def func(item): return item if item != 'id' else None
        fieldList=filter(func, fieldList)
        fields=','.join(str(i) for i in fieldList)

        # 仅适用于PostgreSQL
        sql = "insert into %s (%s) select %s from %s where id =%d RETURNING id" % (targetTableName, fields, fields, orignalTableName, record_id)
        
        cursor = dbHelper.getCursor()
        cursor.execute(sql)
        id = cursor.fetchone()

        # 在历史数据表中增加记录
        data = {
            'op_user_id' : op_user_id,
            'op_datetime': GetSystemTime(),
            'orginal_id' : record_id,
        }

        id = db.save(data, id, table = targetTableName)

        # 删除原有记录
        # db.remove( record_id, table = orignalTableName, key = "id", delete = True)

    except Exception as e:
        onException(e)

    dbHelper.closeDB()

    return

