#coding:utf-8

import os

App_Port                    = 92
App_PortSSL                 = 446
App_Key                     = 'fb98ab9159f51fd2'
App_Secret                  = '09f7c8cba635f7616bc131b0d8e25947s'

UserAuthTimeout             = 3600 * 5
UserAuthOneSession          = False

# 制造商及软件系统名称
# VendorName                  = "JamesSoft Technology Ltd."
# VendorNameAbbr              = "JamesSoft"
# SystemName                  = "DaSheng System"
# SystemNameAbbr              = "JDS"

VendorName                  = "Chengdu Fuma AI Tech. Ltd."
VendorNameAbbr              = "Fuma AI"

SystemName                  = "Fuma RSU Manage System"
SystemNameAbbr              = "Fuma RSU Manage System"

SystemModule                = ['PIS', 'elecMap', 'IOT', 'iMRO', 'oa', 'stock', 'rsu']   # 标记系统业务部件，用于某些业务进行判断
SystemModuleList            = []                                        # 由业务添加自身的实例，如：CPISMsgServer

FileUrlRoot                 = "" 
FileUploadRootPath          = "/usr/local/nginx/html"                   # 文件服务器文件保存根路径
DefaultAvatar               = "/resource/images/default.png"            # 默认头像地址,相对于网页路径

BackupAfterDays             = 7                                         # 设备状态及日志的保留时间，超过该天数则将被删除或转移到历史库

PostgresqlDbConfig = {
    'minconn'   : 1,
    'maxconn'   : 50,
    'host'      : 'localhost',
    'database'  : 'JamesSoft',
    'user'      : 'postgres',
    'password'  : 'postgres',
    'port'      :  5432,
    'datatype'  : 'POSTGRESQL'
}

AppRedisConfig = {
    'host'         : 'localhost',
    'port'         : 6379,
    'db'           : 12,
    'password'     : '123456'
}

PisConfig = {
    "PIS"                       : True,                                 # 是否包含PIS模块
    "formatSwitchTime"          : 20,                                   # 版式文件切换时间，单位：秒
    "snatchScreenTime"          : 20,                                   # 截屏时间间隔，单位：秒

    "packetDebug"               : False,                                # 数据调试开关
    
    "stationServer"             : False,                                # 运行的方式，中心服务器：False，车站服务器：True
    "stationCode"               : "S001",                               # 车站服务器编码
    "PISServerAddr"             : "192.168.0.12",                       # 车站服务器需要配置中心服务器地址

    "PISFileRoot"               : "/home/controller",                   # PIS业务上传下载文件的根目录

    "fileKeepDay"               : 2,                                    # 日志文件、截屏文件保留时长，单位：天

    "ProgramFileKeepDay"        : 30,                                   # 节目文件保留三十天
    "backupAfterDays"           : 7,                                    # 消息列表、消息记录、控制器运行状态及日志的保留时间，超过该天数则将数据转移到历史库

    "videoType"                 : ['mp4', ],                            # 系统支持的视频文件格式
    "failedDaysMsg"             : 7,                                    # 在N天内发送失败的消息可以被重发，可重发的消息类型列表见消息定义
    
    "ModbusPort"                : 502,                                  # Modbus Server端端口
    "TCPServerPort"             : 11615,                                # 和控制器通信的TCP端口，暂未使用
    "UDPServerPort"             : 11616,                                # 和控制器通信的UDP端口
    "StationServerPort"         : 11617,                                # 中心服务器用于和车站服务器通信的UDP端口
    "ControllerTCPPort"         : 6666,                                 # 默认控制器客户端TCP端口
    "ControllerUDPPort"         : 12880,                                # 默认控制器客户端UDP端口

    "StreamServer"              : "/resource/videofile",                # 流媒体服务器地址
    "LiveVideoURL"              : "",                                   # 直播URL
    "StationLogFile"            : "JDSStationLog",                      # 车站服务器日志文件名，路径为 SRV_DIR/src/service目录，扩展名.txt由应用自动添加
    "ServerLogFile"             : "JDSServerLog",                       # 中心服务器日志文件名，路径为 SRV_DIR/src/service目录，扩展名.txt由应用自动添加
}

# IsInitialed = False
DbConfig                    = PostgresqlDbConfig

RedisConfig                 = AppRedisConfig

DefaultCacheExpireTime      = 60 * 60

DcmConfig = {
    'qureyConfig' : 'localhost 11112 -aec DCM4CHEE',
    'revConfig' : '-aem GETDCM --port 12345'
}

TableToRedisNo = {
    'tbProductList' : '08',
    'tbAdSense' : '09',
    'tbMsg'     : '10',
    'tbUser'    : '12'
}

defaultCountryCode          = 'CN'                                      # 默认国家代码，国家代码定义在 system.area_code 表中

DEBUG                       = True                                      # True：输出信息，False：关闭

# 调试级别：0：关闭输出，1：错误信息， 2：普通消息，3：调试信息， 4：调试信息+数据
Debug_Level                 = 2

debugFileSize               = 10 * 1024 * 1024 * 1024                   # 日志文件大小，超过大小后，将旧文件更名后创建新文件

# 服务端、客户端版本号，版本号为字符串
serverVersion               = "1.0"

# 修改客户端版本号后，客户端在心跳响应消息中获得新版本号后就需要到服务器上获取新版本，并重新启动客户端
clientVer                   = "1.0"
clientVersionPath           = "clientVer"                               # 相对路径

