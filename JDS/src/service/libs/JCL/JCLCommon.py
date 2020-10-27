# coding:utf-8

# ------------------------------------------------------------------------------
# JCCL - Component :: Common function
# Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
# Author: Zhang Weijia
# ------------------------------------------------------------------------------
# Name:    JCLCommon.py
# Purpose: JCCL module base message definition
#  *----------------------------------------------------------------------------

import uuid
from socket import *

from libs.utils.debug import *

# 获取本机MAC地址
mac = uuid.UUID(int = uuid.getnode()).hex[-12:]
LocalMacAddr = ":".join([mac[e:e+2] for e in range(0,11,2)])

# 将数据流转换为字符串
def ConvertToHex(data):
    str = "\n"
    nIndex = 0
    nLineIndex = 0
    for d in data:
        if nIndex == 0:
            str += "%04d: "%(nLineIndex*16)
        str =  str + "%02X "%d
        nIndex = nIndex + 1
        if nIndex == 8:
            str = str + "  "

        if nIndex == 16:
            str = str + "\n"
            nIndex = 0
            nLineIndex = nLineIndex + 1

    return str


MODULE_VCCU                 = 1                                         # PIS VCCU
MODULE_DVCU                 = 2                                         # PIS DVCU
MODULE_PECU                 = 3                                         # PIS PECU
MODULE_SPCU                 = 4                                         # PIS SPCU

MODULE_SIMULATOR            = 5                                         # 仿真器
MODULE_TESTER               = 6

MODULE_PIS_CONTROLLER       = 7                                         # PIS 播放控制器

MODULE_CCU                  = 8                                         # TMS CCU
MODULE_DDU                  = 9                                         # TMS 监控显示模块
MODULE_DCU                  = 10                                        # TMS 数据采集模块
MODULE_CCDU                 = 11                                        # TMS 控制显示模块，CCU和DDU合并

MODULE_RSU                  = 12                                        # RSU
MODULE_EDGE_SERVER          = 13                                        # 边缘服务器
MODULE_CLOUD_SERVER         = 14                                        # 云端服务器
MODULE_OBU                  = 15                                        # 车载OBU
MODULE_RSU_MANAGER          = 16                                        # RSU 管理端

MODULE_UNKNOWN              = 0xFFFF


# 获取系统IP地址
def getLocalIPAddr():
    # addr = gethostbyname(gethostname())

    addr = "0.0.0.0"
    try:
        s = socket(AF_INET, SOCK_DGRAM)
        s.connect(('8.8.8.8', 80))
        addr = s.getsockname()[0]
        s.close()
    finally:
        pass

    return addr


from xml.etree import ElementTree  
import urllib.request

# 百度地图App Key
baiduAppKey = 'sEKdIjahGM3zyXoXx2FSvVbT7NhcSrrT'

def getAddrByLngLat(lng, lat):

    if lng == 0 or lat == 0 or lng == None or lat == None:
        return None

    logD("经度： %s, 纬度：%s"%(str(lng), str(lat)))

    # logI("根据经纬度获取地址...")
    url = 'http://api.map.baidu.com/geocoder/v2/?callback=renderReverse&location='+ str(lat) + ',' + str(lng) + '&output=xml&pois=1&ak=%s'%baiduAppKey

    # logI("URL: %s"%url)

    # XML格式的返回数据
    req = urllib.request.urlopen(url)

    # 将其他编码的字符串解码成unicode
    res = req.read().decode("utf-8")

    # 解析XML时直接将字符串转换为一个Element，解析树的根节点
    root = ElementTree.fromstring(res)

    # find()用于查找属于某个tag的第一个element，这里查找结构化地址
    formatted_address   = root.find('result/formatted_address')
    sematic_description = root.find('result/sematic_description')
    if formatted_address.text == None:
        return None

    address = formatted_address.text;
    if sematic_description.text != None:
        address += sematic_description.text

    # Province  = root.find('result/addressComponent/province')
    # city      = root.find('result/addressComponent/city')
    # district  = root.find('result/addressComponent/district')
    # name      = root.find('result/pois/poi/name')
    # direction     = root.find('result/pois/poi/direction')
    # distance  = root.find('result/pois/poi/distance')

    # address   = Province.text + city.text + district.text + name.text + direction.text + distance.text
    # logD("获取地址 Lng: %s, Lat: %s, addr: %s"%(str(lng), str(lat), address))

    return address


# 获取当期系统日期及时间
def GetSystemTime(date_connector = '-', middle = ' ', time_connector = ':'):

    if date_connector == '':
        date_connector = None

    if middle == '':
        middle = None

    if time_connector == '':
        time_connector = None

    if date_connector == None and middle == None and time_connector == None:
        # 默认显示格式
        formatStr = '%Y-%m-%d %H:%M:%S'
    elif date_connector != None and middle != None and time_connector != None:
        # 指定格式显示
        formatStr = "%Y" + date_connector + "%m" + date_connector + "%d" + middle
        formatStr += "%H" + time_connector + "%M" + time_connector + "%S"
    elif date_connector == None and time_connector != None:
        # 只显示时间，不显示日期
        formatStr = "%H" + time_connector + "%M" + time_connector + "%S"
    elif time_connector == None and date_connector != None:
        # 只显示日期，不显示时间
        formatStr = "%Y" + date_connector + "%m" + date_connector + "%d"
    else:
        # 默认显示格式
        formatStr = '%Y-%m-%d %H:%M:%S'

    return datetime.datetime.now().strftime(formatStr)
