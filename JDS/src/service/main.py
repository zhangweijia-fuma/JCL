# coding:utf-8
import os.path
import ssl
import sys

import tornado.httpserver
import tornado.ioloop
import tornado.options
import tornado.web
import tornado.gen
from tornado.options import define, options

from sys import path

path.append("./system")                                                 # 系统管理部分
path.append("./public")                                                 # 公共部分
path.append("./libs")                                                   # 库文件

path.append("./pis")                                                    # PIS
path.append("./iot")                                                    # IOT
path.append("./elecMap")                                                # 电子地图部分
path.append("./mro")                                                    # 智能检修部分
path.append("./rsu")                                                    # RSU部分

import logging
import route

import db.dbMysql, db.dbRedis, config

from tornado.log import access_log, app_log, gen_log
from web.Base import BaseError
from libs.utils.debug import *

define("port", default = config.App_Port, help="run port", type=int)
define("portssl", default = config.App_PortSSL, help="run port", type=int)

TEMPLATE_PATH = os.path.join(os.path.dirname(__file__), "templates")
STATIC_PATH   = os.path.join(os.path.dirname(__file__), "static")

class MainHandler(tornado.web.RequestHandler):
    def get(self):
        raise tornado.web.HTTPError(404)

class MyFile(tornado.web.StaticFileHandler):
    def set_extra_headers(self, path):
        self.set_header("Cache-control", "no-cache")

class Application(tornado.web.Application):
    def __init__(self):
        dbConfig       = config.DbConfig                                # 得到 DB Config
        redisConfig    = config.RedisConfig                             # 得到 Redis Config

        settings = dict(                                                # 得到模板及静态地址路径
            debug = config.DEBUG                                        # 开启调试模式
        )
        if config.DEBUG :
            sys.stdout = sys.stderr

        handlers    = route.getRoute()                                  # 注入路由
        tornado.web.Application.__init__(self, handlers, **settings)
        self.db  = db.dbMysql.DB(dbConfig)                              # 注入MySql/Oracle
        #self.rds = db.dbRedis.RedisCache(redisConfig)                  # 注入Redis

    # 为屏蔽tornado的输出，重载了tornado.web的这个函数，
    # 判断如果请求中包含heartbeat则不输出，以避免输出太多的心跳请求
    # 如果要全部屏蔽tornado.web输出，则可以直接返回
    # 其他输出，则调用基类的函数处理
    def log_request(self, handler):
        return
        # if handler._request_summary().find('heartbeat') != -1:
        # 	return

        # super().log_request(handler)

from datetime import datetime

import logging


def main():
    
    tornado.options.parse_command_line()

    app = Application()

    httpServer = tornado.httpserver.HTTPServer(app)
    httpServer.listen(options.port)

    logI("\n\n\n%s %s Starting...\n\n\n"%(VendorNameAbbr, SystemNameAbbr))
    
    # 启动业务模块
    for module in SystemModuleList:
        module['server'].start()


    # 启动服务
    tornado.ioloop.IOLoop.instance().start()

if __name__ == "__main__":
    main()


