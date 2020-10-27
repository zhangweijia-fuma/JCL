# coding:utf-8

# IOT 业务配置参数

import sys

from socket import *

# 服务端IP地址
serverAddr = "192.168.0.201"

# 服务端监听端口，接收定位模块上报的位置信息
serverPort = 11615

# 百度地图App Key
appKey = 'sEKdIjahGM3zyXoXx2FSvVbT7NhcSrrT'

# socket连接模式, TCP连接：SOCK_STREAM，UDP连接：SOCK_DGRAM
SOCKET_TYPE = SOCK_DGRAM

IOT_DEBUG = False

# 轨迹显示的数量
IOT_TRACE_COUNT = 100