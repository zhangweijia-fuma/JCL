#coding:utf-8
from libs.web.Base  import WebRequestHandler,BaseError,operator_except,public_handler
from system.menu import menuManage
import config
import re
import time, random
import os
import urllib
import json
import shutil
import datetime
import stat

from libs.utils.debug import *

from pis import programFile
from pis import formatFileList
from rsu.RSUManager.RSUConfig import *

# 将文件拷贝到指定目录
# 参数：
#   src：源文件目录，包含路径及文件名
#   destDir：目的目录名称，不包含文件名称
#   filename：目的文件名称，只有名称
def copyFile(src, destDir, filename):

    if not os.path.exists(destDir):
        # 如果不存在则创建目录
        os.makedirs(destDir) 
 
    dest = destDir + filename
    shutil.copy(src, dest)

    # 修改文件的存取属性
    
    # os.chmod(config.PisConfig['PISFileRoot'], 0o777 )
    os.chmod(dest, 0o777 )

    return dest

# 将文件移动到指定目录
# 参数：
#   src：源文件目录，包含路径及文件名
#   destDir：目的目录名称，不包含文件名称
#   filename：目的文件名称，只有名称
def moveFile(src, destDir, filename):

    if not os.path.exists(destDir):
        # 如果不存在则创建目录
        os.makedirs(destDir) 
 
    dest = destDir + filename
    shutil.move(src, dest)

    # 修改文件的存取属性
    os.chmod(destDir, 0o777 )
    os.chmod(dest, 0o777 )

    return dest


class Restful(WebRequestHandler):
    #文件获取测试
    
    def get(self):

        self.finish()#销毁资源          

    # 文件上传测试
    @operator_except
    def post(self):

        fname               = self.get_argument('file_name')
        file_content_type   = self.get_argument('file_content_type')
        file_path           = self.get_argument('file_path')
        file_size           = self.get_argument('file_size')
        
        #获取文件类型
        fnames = fname.split('.')        
        if len(fnames) > 1:
            file_type = fnames[-1]
            #文件重命名
            new_name = file_path + '.' + file_type;
            if os.path.exists(new_name):
                new_name = file_path + str(random.randint(100,999)) + '.' + file_type;
            os.rename(file_path, new_name) 
            #产生url地址
            url = new_name
        else:
            new_name = file_path
            url = new_name
            file_type = file_content_type
        
        url = url.replace(config.FileUploadRootPath,config.FileUrlRoot)

        #保存到文件表中
        s = menuManage.menuManage(self.db)
        user = self.objUserInfo
        uid = "1"
        if user :
            uid = user["id"]        
        file_data={
            "file_name"     : fname,
            "create_time"   : self.now_time(),
            "update_time"   : self.now_time(),
            "path"          : url,
            "create_id"     : uid,
            "update_id"     : uid,
            "file_size"     :file_size,
            "file_type"     :file_type,
            "store_path"    :new_name
        }

        fid = s.save(file_data,table='public.file')

        file_data["id"] = fid
        file_data["file_size"] = file_size
        file_data["file_content_type"] = file_content_type
        file_data["file_path"] = new_name


        # PIS 业务需要将上传的文件复制到指定目录 
        service_type = self.get_argument('service_type', default = "")

        if service_type == "program_list":
            # logI("Upload programFile")
            pass

        elif service_type == "program_file":

            # 节目文件存放目录, root/videofile
            filePath = "%s/videofile/"%(config.PisConfig['PISFileRoot'])
            file_data['file_path'] = moveFile( file_data['store_path'], filePath, os.path.basename(file_data['store_path']))
            file_data['video_type'] = self.get_argument('video_type', default = '1')

            programFile.uploadProgramFile(self, file_data)

            # 移动文件后删除此文件记录
            s.remove( fid, table = "public.file", key = "id", delete = True)

        elif service_type == "format_file":
            # 版式文件存放目录, root/formatfile/filename
            
            filePath = "%s/formatfile/"%(config.PisConfig['PISFileRoot'])
            file_new_path = moveFile( file_data['store_path'], filePath, os.path.basename(file_data['store_path']))
            file_data['file_new_path'] = os.path.basename(file_new_path)
            formatFileList.uploadFormatFile(self, file_data)

            # 移动文件后删除此文件记录
            s.remove( fid, table = "public.file", key = "id", delete = True)

        elif service_type == "update_software":
            # 版本升级文件存在在, root/update
            filePath = "%s/update/"%(config.PisConfig['PISFileRoot'])
            file_data['file_new_path'] = moveFile( file_data['store_path'], filePath, os.path.basename(file_data['store_path']))

            # 移动文件后删除此文件记录
            s.remove( fid, table = "public.file", key = "id", delete = True)

        elif service_type == "logo_file":
            # 业务类型Logo文件
            filePath = os.environ['SRVDIR'] + '/src/app/resource/images/logo/'
            file_data['file_new_path'] = moveFile( file_data['store_path'], filePath, file_data['file_name'])

            # 移动文件后删除此文件记录
            s.remove( fid, table = "public.file", key = "id", delete = True)

        elif service_type == "rsuDevice":
            # 不需要做处理
            # logI("上传了RSU设备数据文件")
            pass

        elif service_type == "rsuUpdateFile":
            # 需要将版本文件移动到RSU FTP目录
            filePath = "%s/update/"%RSU_Root_Path
            file_data['file_new_path'] = moveFile( file_data['store_path'], filePath, file_data['file_name'])

            # 移动文件后删除此文件记录
            s.remove( fid, table = "public.file", key = "id", delete = True)

        else:
            msg = "参数错误：上传文件的业务属性 %s 错误！"%service_type
            logI(msg)
            raise BaseError(801, msg )

        self.response(file_data)

