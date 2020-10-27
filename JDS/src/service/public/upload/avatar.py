#coding:utf-8
from libs.web.Base  import WebRequestHandler,BaseError,operator_except,public_handler
from system.menu import menuManage
import config
import re
import time
import os
import urllib
import json
import types,random
import PIL.Image

class Restful(WebRequestHandler):
    #文件获取测试
    @operator_except
    def get(self,filename):
        pass
    
    def html5_run(self,PHPconfig,JSconfig):
        if PHPconfig["proportional"] != int(JSconfig["P"]):
            self.erro = "JS设置的比例和python设置不一致！";
            return False;
        file_meta=self.request.files["UpFile"][0]
        fname = file_meta["filename"]
        ftype = file_meta["content_type"].split('/')[-1]
        saveName=PHPconfig["filename"]+'.'+ftype
        filePath = os.path.join(PHPconfig["saveURL"], saveName)
        fileUrl = "/files/avatar/" + saveName
        
        #print("upload_path : " + PHPconfig["saveURL"])
        #print("fileUrl : " + fileUrl)
        #print("filePath : " + filePath)
    
        with open(filePath,"wb") as up:
            up.write(file_meta["body"])
        
        #缩放图片
        img = PIL.Image.open(filePath) 
        width,height = img.size
        targetWidth=PHPconfig["width"][0][0]
        targetImg = img.resize(  
            (int(targetWidth), int(targetWidth)),  
            PIL.Image.ANTIALIAS  
        )
        targetImg.save(filePath,quality=PHPconfig['quality']);
        data={}
        data['ImgHeight']=PHPconfig["width"][0][0]
        data['ImgWidth']=PHPconfig["width"][0][0]
        data['ImgName']=saveName
        data['ImgUrl']=fileUrl
        
        return data

    @operator_except
    def post(self):
        
        user = self.objUserInfo
        uid = "1"
        if user :
            uid = user["id"]            
        else:
            data={'erro':'用户未登录'}
            self.response(data)
            return
        
        #获取文件保存路径
        savePathDir=os.path.join(os.path.dirname(__file__),"../../../app/files/")
        ShearPhoto={}
        ShearPhoto["config"]={
            "proportional":1,
            "quality":100,
            "force_jpg":True,
            "width":[[100,True,"centre"]],
            "water":False,	   
            "water_scope":100,
            "temp":savePathDir+"temp",
            "tempSaveTime":600,
            "saveURL":savePathDir+"avatar",
            "filename":str(time.time())+"_"+str(uid)
        }        
        os.makedirs(ShearPhoto["config"]["saveURL"], exist_ok=True);
        ShearPhotoIW=self.get_argument('ShearPhotoIW')
        ShearPhotoIH=self.get_argument('ShearPhotoIH')
        ShearPhotoFW=self.get_argument('ShearPhotoFW')
        ShearPhotoFH=self.get_argument('ShearPhotoFH')
        ShearPhotoP=self.get_argument('ShearPhotoP')
        shearphoto=self.get_argument('shearphoto',default='0')
        print('*'*60)
        print(shearphoto)
        if ShearPhotoIW and ShearPhotoIH and ShearPhotoFW and ShearPhotoFH and ShearPhotoP:
            JSconfig={}
            JSconfig["P"]=ShearPhotoP
            JSconfig["IW"]=ShearPhotoIW
            JSconfig["IH"]=ShearPhotoIH
            JSconfig["FW"]=ShearPhotoFW
            JSconfig["FH"]=ShearPhotoFH
            
            result =self.html5_run(ShearPhoto["config"],JSconfig)
            if not result:
                data={'erro':'保存失败'}
                self.response(data)   
                return
            else:
                #保存到文件表中
                s = menuManage.Menu(self.db)    
                file_data={
                    "file_name" : result['ImgName'],
                    "create_time"  : self.now_time(),
                    "update_time" : self.now_time(),
                    "path" : result['ImgUrl'],
                    "create_id" : uid,
                    "update_id" : uid            
                }
                fid = s.save(file_data,table='public.file') 
                data={'photo_file':fid}
                if shearphoto==1 or shearphoto=='1':
                    #医生账户新增、修改
                    data['path']=file_data['path']
                    self.response(data)
                    return
                elif shearphoto==2 or shearphoto=='2':
                    #医生账户新增、修改
                    data['path']=file_data['path']
                    self.response(data)
                    return

                if int(user['account_type'])==0:
                    #医生
                    s.save(data,uid,table='public.doctor',key='account_id')
                    
                else:
                    s.save(data,uid,table='patient.patient',key='account_id')
                
                self.response([result]) 
                return                
                
        else :
            data={'erro':'浏览器不支持html5或缺少参数或错误参数'}
            self.response(data)   
            return     
        





