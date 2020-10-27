import sys
import time
import datetime

from db import dbMysql
from libs.db.dbHelper import *

from libs.utils.debug import *
from libs.utils.utils import *

# 获取指定系统用户的序列号参数
# isQuery: True：仅仅是查询，False：自动更新
def GetSerialNo(systemUserID, field, isQuery = False):
    
    dbHelper = DbHelper(auto=True)
    
    db = dbHelper.openDB("public.device_config")
    result = db.findByCond('id,' + field, "system_user_id = %d "%systemUserID)

    id = 0
    serialNo = 0
    if len(result["rows"]) == 0:
        # 仅仅是查询，则对数据不做变动
        if isQuery == True:
            return -1

        # 查询的时候同时更改数据，此时从1开始
        serialNo    = 0
    else:
        serialNo    = result['rows'][0][1]
        id          = result['rows'][0][0]

        # 如果此时没有数据
        if serialNo == None:
            serialNo = 0
            id = 0

    # 更改序号
    serialNo = serialNo + 1
    data = {}
    data[field] = serialNo
    data['system_user_id']  = systemUserID

    # 数据库中保存的序号时候已经使用过的序号
    if id == 0:
        db.save(data, table = "public.device_config")
    else:
        db.save(data, id, table = "public.device_config")

    dbHelper.closeDB()

    return serialNo



# 清理指定目录下的文件
def clearFile(filePath, fileKeepDay):
    now     = datetime.datetime.now()
    delta   = datetime.timedelta(days=fileKeepDay)
    checkDay= now - delta

    checkTimeStamp = checkDay.timestamp()

    for file in os.listdir(filePath):
        file_path = os.path.join(filePath, file)
        if os.path.isdir(file_path):
            clearFile(file_path, fileKeepDay)
        else:
            fileTime = os.path.getmtime(file_path)
            if ( fileTime < checkTimeStamp):
                logD("删除文件：%s"%file)
                os.remove(file_path)
