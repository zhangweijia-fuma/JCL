#coding:utf-8
import time
import datetime
import subprocess, os, re, sys
import psycopg2
import logging

from subprocess import Popen, PIPE

import config
from config import PostgresqlDbConfig

class Scheduler():

    # 获取数据库连接
    def getConn(self):
        try:
            conn=psycopg2.connect(database=PostgresqlDbConfig['database'], user=PostgresqlDbConfig['user'],
                                  password=PostgresqlDbConfig['password'], host=PostgresqlDbConfig['host'],
                                  port=PostgresqlDbConfig['port']);
            return conn
        except Exception as e:
            logging.error('获取数据库连接失败:%s' % e)

    # 日志写入
    def addlog(self,context):
        conn = self.getConn()
        if not conn:
            return

        cur = conn.cursor();
        createTime = time.strftime("%Y-%m-%d %H:%M:%S",time.localtime(time.time()))
        sql  = "insert into system.operation_log (create_time,operation_context) "
        sql += "values('%s','%s') "
        sql  = sql%(createTime, context)
        try:
            cur.execute(sql)
            conn.commit()

        except Exception as e:
            # 如果出错，则事务回滚
            conn.rollback()
            logging.error('数据写入失败: %s' % e)

        finally:
            logging.info(sql)
            cur.close()
            conn.close()

    # #测试数据库连接
    # def test(self):
    #     conn = self.getConn()
    #     if not conn:
    #         return
    #     cur = conn.cursor();
    #     uid=1#用户id
    #     sql="select mis.* from (select m1.id pid,m1.name pname,m1.code pcode,m2.id cid,m2.name cname,m2.code ccode,mi.id,mi.type,mi.path,m1.sort psort,m2.sort csort "
    #     sql+="from (select *from system.menu where tier=0) m1,(select *from system.menu where tier=1) m2,system.menu_item mi "
    #     sql+="where m1.id=m2.parent_id and mi.menu_id=m2.id) mis, "
    #     sql+="((select distinct(marg.menu_item_id) from system.user_group_member magr,system.user_group_menu marg "
    #     sql+="where magr.user_id=%s and marg.menu_group_id=magr.group_id) union "
    #     sql+="(select mar.menu_item_id from system.menu_authority_relation mar where mar.user_id=%s)) mii "
    #     sql+="where mis.id=mii.menu_item_id order by mis.psort desc,mis.csort,mis.cid "
    #     sql=sql%(uid,uid)
    #     try:
    #         cur.execute(sql)
    #         rows = cur.fetchall()
    #         print(rows)
    #         conn.commit()
    #     except Exception as e:
    #         conn.rollback()   #如果出错，则事务回滚
    #         logging.error('数据写入失败:%s' % e)
    #         print(e)
    #     finally:
    #         cur.close();
    #         conn.close();
