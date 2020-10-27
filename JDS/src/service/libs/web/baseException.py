#coding:utf-8
errorDic={
        900 : {
            "code": "900",
            "status": 500,
            "message": "未被定义的错误类型",
            "help_document": "/oauth/v1.0.0/help/900"
        },
        601: {
            "code": "601",
            "status":401,
            "message": "未经过JctOAuth授权的第三方应用",
            "help_document": "/oauth/v1.0.0/help/601"
        },
        602: {
            "code": "602",
            "status":401,
            "message": "未登录授权的应用",
            "help_document": "/oauth/v1.0.0/help/602"
        },
        603: {
            "code": "603",
            "status":404,
            "message": "用户名不存在",
            "help_document": "/oauth/v1.0.0/help/603"
        },
        604: {
            "code": "604",
            "status":404,
            "message": "未授权的访问",
            "help_document": "/oauth/v1.0.0/help/604"
        },
        605: {
            "code": "605",
            "status":404,
            "message": "账号被禁用",
            "help_document": "/oauth/v1.0.0/help/605"
        },
        606: {
            "code": "606",
            "status":404,
            "message": "密码不正确",
            "help_document": "/oauth/v1.0.0/help/606"
        },
        701: {
              "code": "701",
              "status":500,
              "message": "数据库连接失败",
              "help_document": "/oauth/v1.0.0/help/701"
        },
        702: {
              "code": "702",
              "status":500,
              "message": "无法从链接池中获得数据库连接",
              "help_document": "/oauth/v1.0.0/help/702"
        },
        703: {
              "code": "703",
              "status":500,
              "message":"SQL 语句执行失败(I)",
              "help_document":"/oauth/v1.0.0/help/703"
        },
        704: {
              "code": "704",
              "status":500,
              "message":"SQL 语句执行失败(D)",
              "help_document":"/oauth/v1.0.0/help/704"
        },
        705: {
              "code": "705",
              "status":500,
              "message":"SQL 语句执行失败(U)",
              "help_document":"/oauth/v1.0.0/help/705"
        },
        706: {
              "code": "706",
              "status":500,
              "message":"SQL 语句执行失败(S)",
              "help_document":"/oauth/v1.0.0/help/706"
        },
        707: {
              "code": "707",
              "status":500,
              "message":"SQL 语句执行失败(C)",
              "help_document":"/oauth/v1.0.0/help/707"
        },
        801: {
              "code": "801",
              "status":400,
              "message":"参数列表错误",
              "help_document":"/o2b/v1.0.0/help/801"
        },
        802 : {
            "code": "802",
            "status":404,
            "message":"没有找到数据",
            "help_document":"/o2b/v1.0.0/help/802"
        },
        803 : {
            "code": "803",
            "status":404,
            "message":"修改数据失败",
            "help_document":"/o2b/v1.0.0/help/803"
        },
        804 : {
            "code": "804",
            "status":404,
            "message":"数据唯一标示已存在",
            "help_document":"/o2b/v1.0.0/help/803"
        },
        807 : {
            "code": "807",
            "status":404,
            "message":"该菜单编码已存在",
            "help_document":"/o2b/v1.0.0/help/807"
        },
        808 : {
            "code": "808",
            "status":404,
            "message":"该菜单已存在该类菜单项，无法重复添加",
            "help_document":"/o2b/v1.0.0/help/808"
        },
        910: {
            "code": "910",
            "status": 500,
            "message": "用户名重复",
            "help_document": "/oauth/v1.0.0/help/1000"
        },

        911: {
            "code": "911",
            "status": 500,
            "message": "用户组名称重复",
            "help_document": "/oauth/v1.0.0/help/1000"
        }

}


# _version_ = 0.2.0
# 新增strErrorNMessage参数，传递原始的错误信息

class BaseError(Exception) :
    def __init__(self,code,strErrorMessage='') :
        self.code=code
        self.message = strErrorMessage

    def __str__(self) :
        return repl(self.code)


