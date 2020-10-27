################################################################################################
# 2016-12-29, zhangwj, 消息管理接口，消息类型包括站内消息、短信、微信；
# 
#coding:utf-8

import os

# python3.0以后， urllib2 合并到urllib中，成为5个模块：urllib.parse， urllib.error， urllib.request， urllib.response, urllib.robotparser
import urllib
import http.client
import json
import uuid
import requests
import hmac
from hashlib import sha1
import base64

import datetime
import time
import config

from libs.web.Base  import WebRequestHandler,BaseError,operator_except
from libs.db import dbMysql
from libs.utils.utils import *
from libs.utils import debug
from public.pushNotify.baidu import sendPushNotify

# 在不同医院必须更新这个ID值
HospitalID = "yunpian"

class smsManage(dbMysql.CURD) :
    def __init__(self,db) :
        if sys.version > '3':
            # python 3.0 +
            super().__init__(db,'public.save_sms', False) # 定义本实例需要操作的表名
        else :
            # python 2.7
            super(smsManage, self).__init__(db, 'public.save_sms', False)


# 发送消息给指定用户组成员
# 在发送消息时，可以指定消息是发站内消息、短信或微信
# groupName, 指定用户组名称
# hosCode, 用户组归属医院，不同医院可能包含相同名称的用户组
# accountIDList：用户ID列表，每条记录需要包含用户的ID、姓名、电话号码及微信用户ID
# message: 
#   MsgType : 消息类别， 0：站内消息，1：短信，2：微信
#   sendTime: 发送时间，如果为空则立即发送，否则为预约发送，格式为：yyyyMMddhhmmss
#   stationMsg: 站内消息：为一字典列表，包含：
#       sender_id     : 发送者ID,
#       sender_name   : 发送者姓名,
#       target_hospital_code : 接收消息医院编码,
#       receiver_id   : 接收方ID, 
#       receiver_name : 接收方姓名
#       content       : 消息内容,
#       message_type  : 消息类型,
#       status        : 消息状态，默认为0：未读，1：未处理，2：已处理，3：已删除,
#       operation_id  : 操作记录ID,
#       top_status    : 是否置顶标志，1：置顶，0：不置顶,
#   sms： 短信内容分为两个部分，
#           TemplateCode和Param是基于阿里云模式的，短信内容基于阿里云事先审批过的短信模板；
#           content是基于总医院模式，不需要使用参数，短信内容完全自定义。
#       TemplateCode  : 短信模板模板名称，不同医院参数不相同，如总医院就只有content字段，无其他字段, 此时该字段和content字段内容相同
#       Params:       : 参数列表
#           name : 参数名称
#           value: 参数值
#       content  : 字符串，最大700个字符
#   weixin: 微信，包含：
#       待定

def sendMsgByGroup(self, groupName, hosCode, msgContent ):

    # debug.printLog(2, "Send Message by groupName%s, hoscode:%s:  "%(groupName, hosCode), msgContent)

    # 获取用户组成员ID、姓名、电话、微信号
    sql  = " select a.id, a.name, b.phone, e.openid from system.account a "
    sql += " inner join public.doctor b on b.account_id = a.id"
    sql += " inner join system.user_group_member c on c.user_id = a.id"
    sql += " inner join system.user_group d on d.id = c.group_id "
    sql += " left join system.weixin_user e on e.account_id = a.id "
    sql += " where d.name = '%s' and a.hospital_code = '%s' and a.status = '0' "%(groupName, hosCode)
    sql += " order by a.name"

    cur  = self.db.getCursor()
    cur.execute(sql);
    rows = cur.fetchall()

    accountList = []
    for i, row in enumerate(rows):
        key  = ['id', 'name', 'phone', 'weixin']
        item = dict(zip(key, row))
        accountList.append(item)

    sendMsgByAccount(self, accountList, msgContent )

    return

# 根据账户列表发送消息，在IDList中，每个Item包括了目标用户的AccountID，姓名、电话号码及微信号码
def sendMsgByAccount(self, accountList, msgContent ):
    
    # debug.printLog(2, "Send Message by Account: ", accountList, msgContent)

    # 发送站内消息
    if msgContent['msgType'] == "STATION_MESSAGE":
        sendStationMsgByAccountList(self, accountList, msgContent['stationMsg'])
        return

    # 发送短信
    if msgContent['msgType'] == "SMS":
        phoneList = []
        for i, row in enumerate(accountList):
            phone = row['phone']
            if phone == None or phone == "":
                phone = getPhoneByID( self, row['id'] )

            if phone != None and phone != "":
                phoneList.append(phone)

        sendSMSByNumber(self, phoneList, msgContent['sms'], msgContent['sendTime'] )

        return

    # 发送微信
    if msgContent['msgType'] == "WEIXIN":
        sendMicroMsgByAccountList(self, accountList, msgContent['weixin'])
        return

    return

# 发送站内消息，在IDList中，每个Item包括了目标用户的AccountID，姓名，电话号码及微信号
def sendStationMsgByAccountList(self, accountList, stationMsg):

    dbEntity = smsManage(self.db)

    for i, item in enumerate(accountList):
        stationMsg['receiver_id']   = item['id']
        stationMsg['receiver_name'] = item['name']

        id = dbEntity.save(stationMsg, table='public.station_message')

        sendPushNotify(self, item['id'], "消息", stationMsg["content"])
        
    return

# 发送短信，此时AccountList中仅仅有ID，仅仅提供给RST在总医院使用
def sendMessage(self, accountIDList, msgContent, sendTime ):
    
    if HospitalID != "CDMGH":
        return

    debug.printLog(1, "Send Short Message...")
    debug.printLog(2, "Send Message parameter: ")
    debug.printLog(2, "Account ID List:", accountIDList )
    debug.printLog(2, "message content: ", msgContent)
    debug.printLog(2, "send time:", sendTime)
    numberList = []
    for i, accountID in enumerate(accountIDList):
        if accountID !=None:
            number = getPhoneByID( self, accountID )
            numberList.append(number)

    sendMessageToCDMGH(self, numberList, msgContent, sendTime )

    return

# 获得指定账户ID的电话号码
def getPhoneByID( self, accountID ):
    if not accountID:
        return None
        
    sql = "select a.phone from public.doctor a where a.account_id = %s"%accountID
    
    cur=self.db.getCursor()
    cur.execute(sql)
    row = cur.fetchone()
    
    # debug.printLog(3, "Get phone number by ID:", sql, row )

    return row[0]

# 发送短信
def sendSMSByNumber(self, numberList, msgContent, sendTime ):

    # debug.printLog(2, "HospitalID:", HospitalID )

    if HospitalID == "CDMGH":
        sendMessageToCDMGH(self, numberList, msgContent['content'], sendTime )
    elif HospitalID == "AHSMU":
        sendMessageToAHSMU(self, numberList, msgContent, sendTime )
    elif HospitalID == "AliCloud":
        sendMessageToAliCloud(self, numberList, msgContent )
    elif HospitalID == "yunpian":
        sendMessageToYunPian(self, numberList, msgContent )
     
    return

# 发送给成都军区总医院， msgContent仅仅包括短信文本
def sendMessageToCDMGH(self, numberList, msgContent, sendTime ):
    
    debug.printLog(1, "Send Message to CDMGH...")

    url = "http://192.168.0.249:81/sms/sendsms"
    data = {}
    data['key'] = 'kxkcme'
    data['username'] = 'kxkcme'
    data['pwd'] = 'cme1@kxk'
    data['msgcontent'] = msgContent.encode('GB2312')
    phonenumber = ""
    for i, element in enumerate(numberList):
        phonenumber += element + ","

    data['numberlist'] = phonenumber
    data['appsno'] = ''
    data['smsclass'] = ''
    data['time'] = sendTime
    data['f'] = '1'

    # debug.printLog(2, "POST parameter before encode:", data)
    post_data = urllib.parse.urlencode(data).encode()
    # debug.printLog(2, "POST parameter after encode:", post_data)

    try:
        headers = {}
        request   = urllib.request.Request(url, post_data, headers)
        headers   = urllib.request.urlopen(request, timeout=2)
        rspText   = headers.read().decode('GB2312')
        rspStatus = headers.status
        if rspStatus == 200:
            # debug.printLog(2, "Send SMS to CDMGH success:", rspText, rspStatus, msgContent)
            saveSMS(self, numberList, msgContent, sendTime )

    except:
        debug.printLog(2, "Send POST request error...")
        return

    return

# 发送给西南医科大学
def sendMessageToAHSMU(self, numberList, msgContent, sendTime ):
    debug.printLog(1, "Send Message to AHSMU...")
    
    return

# 发送给云片
def sendMessageToYunPian(self, numberList, msgContent):
    
    debug.printLog(2, "Send Message to Yun Pian...")

    for phone in numberList:
        yunPian = yunPianSMS()
        response_str = yunPian.sendSMSByTemplate(msgContent['TemplateCode'], msgContent['Params'], phone)
        debug.printLog(2, "云片返回值：", response_str)

    # saveSMS(self, numberList, msgContent['content'], "");

    return

class yunPianSMS:
    def __init__(self):

        #服务地址
        self.sms_host = "sms.yunpian.com"
        self.voice_host = "voice.yunpian.com"

        #端口号
        self.port = 443

        #版本号
        self.version = "v2"

        #查账户信息的URI
        self.user_get_uri = "/" + self.version + "/user/get.json"

        #智能匹配模板短信接口的URI
        self.sms_send_uri = "/" + self.version + "/sms/single_send.json"

        #模板短信接口的URI
        self.sms_tpl_send_uri = "/" + self.version + "/sms/tpl_single_send.json"

        #语音短信接口的URI
        self.sms_voice_send_uri = "/" + self.version + "/voice/send.json"

        #语音验证码
        self.voiceCode = 1234

        self.apikey = "d7cac605bca7707f4fe4194692af148c"

    # 获取云片账户信息
    def getUserInfo(self):
        conn = http.client.HTTPSConnection( self.sms_host , port = self.port)
        headers = {"Content-type": "application/x-www-form-urlencoded", "Accept": "text/plain"}
        conn.request('POST', self.user_get_uri, urllib.parse.urlencode( {'apikey' : self.apikey}))
        response = conn.getresponse()
        response_str = response.read().decode('utf-8')
        conn.close()
        return response_str

    # text：短信文本，文本内容需要符合某一个模板的格式，mobile：手机号码，可以以","分隔
    def sendSMS(self, text, mobile):
        params = urllib.parse.urlencode({'apikey': self.apikey, 'text': text, 'mobile':mobile})
        headers = {"Content-type": "application/x-www-form-urlencoded", "Accept": "text/plain"}
        conn = http.client.HTTPSConnection( self.sms_host, port = self.port, timeout=30)
        conn.request("POST", self.sms_send_uri, params, headers)
        response = conn.getresponse()
        response_str = response.read().decode('utf-8')
        conn.close()
        return response_str

    # code: 短信模板编号， params：短信参数，mobile：手机号码，可以以","分隔
    def sendSMSByTemplate(self, code, params, mobile):
        params = urllib.parse.urlencode({'apikey': self.apikey, 'tpl_id':code, 'tpl_value': urllib.parse.urlencode(params), 'mobile':mobile})
        headers = {"Content-type": "application/x-www-form-urlencoded", "Accept": "text/plain"}
        conn = http.client.HTTPSConnection( self.sms_host, port = self.port, timeout=30)
        conn.request("POST", self.sms_tpl_send_uri, params, headers)
        response = conn.getresponse()
        response_str = response.read().decode('utf-8')
        conn.close()
        return response_str

    def sendVoiceSMS(self, code, mobile):
        params = urllib.parse.urlencode({'apikey': self.apikey, 'code': code, 'mobile':mobile})
        headers = {"Content-type": "application/x-www-form-urlencoded", "Accept": "text/plain"}
        conn = http.client.HTTPSConnection( self.voice_host, port = self.port, timeout=30)
        conn.request("POST", self.sms_voice_send_uri, params, headers)
        response = conn.getresponse()
        response_str = response.read().decode('utf-8')
        conn.close()
        return response_str

# # 云片短信测试
# def sendSMSSample(code, mobile):
#     yunpian = yunPianSMS()
#     text = "张医生，您已被邀请参加叙永人民医院远程会诊，会诊时间是2017-04-02，请准时参加。"
#     # 查账户信息
#     # print( yunpian.get_user_info())
#     # 调用智能匹配模板接口发短信
#     # print( yunpian.sendSMS( text, mobile) )
#     # 调用模板接口发短信
#     params = {'#name1#':'张医生','#hos_name#':'叙永人民医院', '#consultation_name#': '远程会诊', '#consultation_time#': '2017-04-02 14:12:00' }
#     print(yunpian.sendSMS( code, params, mobile))

# sendSMSSample(1753940, "13908065070")


# 发送给阿里云
def sendMessageToAliCloud(self, numberList, msgContent):
    
    debug.printLog(1, "Send Message to Ali Cloud...")

    for phone in numberList:
        alicloud = AliCloudSMS()        
        alicloud.sendSingleSMS(phone, msgContent)

    saveSMS(self, numberList, msgContent['content'], "");

    return

# 通过阿里云短信云服务发送短信
class AliCloudSMS:
    def __init__(self):
        self.access_id = 'LTAIEdrCnqbuMqtf'
        self.access_secret = b'TuXpmcVBD2suhJxkYjiCvdfHQZPBsk&'

    def percent_encode(self, encodeStr):
        encodeStr = str(encodeStr)
        res = urllib.parse.quote(encodeStr.encode('utf-8'), '')
        res = res.replace('+', '%20')
        res = res.replace('*', '%2A')
        res = res.replace('%7E', '~')
        return res

    # 发送单条短信
    def sendSingleSMS(self, phone, msgContent ):

        parameters = {
            'Action': 'SingleSendSms', 
            'ParamString': '',
            'RecNum': phone,
            'SignName': '成都极感科技',
            'TemplateCode': msgContent['TemplateCode'],
            'Format': 'JSON',
            'Version': '2016-09-27',
            'AccessKeyId': self.access_id,
            'SignatureVersion': '1.0',
            'SignatureMethod': 'HMAC-SHA1',
            'SignatureNonce': str(uuid.uuid1()),
            'Timestamp': time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime()),
        }

        ParamString = "{"
        for i, param in enumerate(msgContent['Params']):
            ParamString += "\"%s\":\"%s\""%(param['name'], param['value'])
            if i < len(msgContent['Params'])-1:
                ParamString += ", "
        ParamString += "}"

        parameters['ParamString'] = ParamString

        sortedParameters = sorted(parameters.items(), key=lambda parameters: parameters[0])
        canonicalizedQueryString = ''

        for (k, v) in sortedParameters:
            canonicalizedQueryString += '&' + self.percent_encode(k) + '=' + self.percent_encode(v)

        stringToSign = 'GET&%2F&' + self.percent_encode(canonicalizedQueryString[1:])    # 使用get请求方法

        h = hmac.new( self.access_secret, stringToSign.encode(), sha1)
        signature = base64.encodestring(h.digest()).strip()

        parameters['Signature'] = signature

        # debug.printLog(3, "阿里云短信参数", parameters)

        url = "http://sms.aliyuncs.com" + "/?" + urllib.parse.urlencode(parameters)

        session = requests.Session()
        session.headers = {'Content-Type': 'application/x-www-form-urlencoded'}
        session.get('https://sms.aliyuncs.com')
        r = session.get(url)
        
        # debug.printLog(2, "阿里云短信返回：",r.text)

# 阿里云短信测试
# def sendSMStoAliCloud():
#     msgContent = {}
#     msgContent['TemplateCode'] = 'SMS_53635024'
#     msgContent['Params'] = []
#     msgContent['Params'].append({'name': 'name1', 'value': '张医生'})
#     msgContent['Params'].append({'name': 'name2', 'value': '李医生'})
#     msgContent['Params'].append({'name': 'service_name', 'value': '继教学分申请'} )
#     aliyun = AliCloudSMS()
#     aliyun.send_sms('13908065070', msgContent )
# sendSMStoAliCloud()

# 保存短信到数据库
def saveSMS(self, numberList, msgtext, sendTime):

    curTime = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    if sendTime == '':
        sendTime = curTime

    user = self.user
    phonenumber = ""
    for i, phoneNumber in enumerate(numberList):
        data = {}
        data['create_time']     = curTime
        data['create_id']       = user['id']
        data['hospital_code']   = user['hospital_code']
        data['sender_id']       = user['id']
        data['sender_name']     = user['name']
        data['tel']             = phoneNumber
        data['content']         = msgtext
        data['send_time']       = sendTime

        dbEntity = smsManage(self.db)
        id       = dbEntity.save(data, table='public.save_sms')

# 发送微信
def sendMicroMsgByAccountList(self, accountList, microMsg):

    return


# 更新站内消息，根据消息类别及ID值来判断
def UpdateStationMsg(self, updateID, id, msgType, status):
    update_time = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')

    sql  = "update public.station_message a "
    sql += " set update_time = '%s'"%update_time
    sql += ", update_id=%s"%updateID
    sql += ", status=%d "%status            # 0, 未读，1，已读未处理，2为已处理，3已删除
    sql += " where a.message_type = '%s' and a.operation_id= %s"%(msgType, id)
    cur = self.db.getCursor()

    cur.execute(sql)

    return

UpdateMdtStationMsg = UpdateStationMsg