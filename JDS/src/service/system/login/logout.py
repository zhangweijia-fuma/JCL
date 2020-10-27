#coding:utf-8
from libs.web.Base  import WebRequestHandler,BaseError,operator_except
from libs.utils import user

import config

import base64,time
from Crypto.Cipher import AES
from hashlib import md5

from system.login.heartbeat import *

class Handle(WebRequestHandler):
    @operator_except
    def get(self):
        
        loginSessionID   = int(self.get_argument('loginSessionID', default = '0'))
        
        userInfo = self.GetUserInfo()
        userInfo['loginSessionID'] = loginSessionID
        LoginRecord.UserLogout(userInfo)

        token = self.request.headers.get('Authorization')
        u = user.user();
        u.delete(token);

        res = ':)'

        self.response(res)
