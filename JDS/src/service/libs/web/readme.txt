# Seagulls-Pydo 框架使用说明
#### ── 人生苦短，Python是岸
版本 V 0.2.0.3
作者：薛海 johnxue@163.com
最后一次更新时间：2015年5月21日
最后一次更改人： 薛海

------

**Seagulls-Pydo框架** 
是基于 Tornado Web 服务器为基础的一款简洁、轻量、易用的 Restful Web Services 异步微框架。Seagulls-Pydo微框架封装并简化了对Mysql/Postgresql、Redis等SQL/NOSQL服务器的操作。同时也将WEB开发中常用的模块（如：认证鉴权、用户管理、新闻、广告、圈子、消息、邮件、短信、HTML编辑器、文件上传等）纳入微框架内。

其主要的功能模块如下：

 * Framework - 核心框架
* Base - 基础核心模块
* BaseException - 异常处理模块
* dbMysql - Mysql 基础操作模块
* dbPostgresql - Postgresql 基础操作模块
* dbDBToRedis -  数据库与Redis同步模块
* dbRedis - Redis基础操作模块
 * Service 系统服务模块
* Email - 邮件处理模块
* SMS - 短信处理模块
* uploadfile - 文件上传模块
* RichEditor - 富文本HTML编辑模块
* Random - 随机码生成模块
* VerificationCode - 验证码模块
 * User 用户管理模块
 * easyOAuth 认证鉴权模块
 * News 新闻发布及管理模块
 * AdSense 广告发布及管理模块
 * Group 圈子管理模块
 * Message 消息管理模块
 * Address 多地址管理模块

------

## 1. 环境配置说明

* 操作系统 Ubutun Linux 14.04 + ；
* Python 3.0 + ；
* Tornado 4.0.2 + ;
* Mysql 5.0 以上，建议采用 MariaDB ；

----------

## 2. 项目结构说明
在一个以 Seagulls-PyDo 为基础框架的项目中，必须含有以下三个文件：
main.py - 主文件
route.py - 路由器
config.py - 配置文件

----------

* 2.1 main.py
main.py 是项目框架的主程序，不需要做任何的修改。

----------

* 2.2 route.py
route.py 是项目框架的路由器，它完成项目URI与业务处理类的对接。其主要格式如下：
```
import Framework.Base   # 导入基础框架
...
import Order # 导入订单模块
from Order import attribute,list,detail
...

handlers = [ 
           ...
            #订单管理
            (r"/o2b/v1.0.0/order", Order.list.info),
            (r"/o2b/v1.0.0/order/([0-9]+)", Order.detail.info),
            (r"/o2b/v1.0.0/order/attribute", Order.attribute.info),
            (r"/o2b/v1.0.0/order/returns", Order.Returns.list.info),
            (r"/o2b/v1.0.0/order/returns/upload", Service.uploadfile.Handler),
            ...
        ]
```
其中 `(r"/o2b/v1.0.0/order", Order.list.info)` 中的  `"/o2b/v1.0.0/order"` 是业务相关 URI，`Order.list.info` URI相对应的业务处理类。URI 可用正则的方式表示，如：`r"/o2b/v1.0.0/order/([0-9]+)` 其中的 `([0-9]+)` 代表1位以上的数字，例：/o2b/v1.0.0/order/10010 。

在具体的项目中开发者只需要在handlers中填写相关的URI与业务类的对应关系即可，但需要提醒的是**所添加的业务类必须在import区域中导入**。

----------

* 2.3 config.py
config.py 是项目框架中的配置文件，如数据库的连接、Redis连接、SMTP服务器、短信网关、图片存储位置及格式要求等。开发者也可以在config.py中配置与本项目项关的其它信息。

核心的配置说明如下：
```
import os

App_Key    = 'fb98ab9159f51fd0'                   # 应用程序 Key 
App_Secret = '09f7c8cba635f7616bc131b0d8e25947s'  # 应用程序安全码

Default_Header='header_boy.jpg'  # 缺省头像

# smtp 服务器配置
smtp_163_server = {
    'smtp.server' : 'smtp.163.com',
    'username'    : 'jct_sender',
    'password'    : 'jct_sender2013'
}

# 开发者可以创建自己的 smtp 服务器配置，如下：
smtp_qq_server ={
    'smtp.server' : '',
    'username'    : '',
    'password'    : ''    
}

# 发送邮件的配置
EmailConfig={
    'server'  : smtp_163_server,                   # 可以更改为用忣自己的 smtp 配置
    'fromAdd' : 'jct_sender@163.com',              # 发送者的账号
    'logo'    : '/image/logo.jpg',                 # 邮件中的logo
    'content_verification' : '',                   # 默认内容
    'subject_verification' : '随e筹注册确认邮件',    #默认邮件主题
}

# 配置SMS网关
# www.sms.com SMS API
www_sms_com_API = {
    'vender'    : 'sms.com',
    'url'       : 'http://api.sms.cn/mtutf8/',
    'method'    : 'GET',
    'arguments_Record' : {   # 已备案账号
        'uid'       : 'jetcloudtech',
        'pwd'       : '2cb1097237d74363f70db9c55cb36fc9',  # pwd=md5('Admin2013jetcloudtech') 即md5(密码+用户名)
        'mobile'    : '',
        'content'   : ''
    },
    'arguments' : {
            'uid'       : 'jctadmin',   #未备案账号
            'pwd'       : 'a4659ff67fd595fbdb59b7e9f16af830',  # pwd=md5('Admin2014jctadmin') 即md5(密码+用户名)
            'mobile'    : '',
            'content'   : ''
    },    
    'status': {
        '100' : '发送成功',
        '101' : '验证失败',
        '102' : '短信不足',
        '103' : '操作失败',
        '104' : '非法字符',
        '105' : '内容过多',
        '106' : '号码过多',
        '107' : '频率过快',
        '108' : '号码内容空',
        '109' : '账号冻结',
        '110' : '禁止频繁单条发送',
        '112' : '号码不正确',
        '120' : '系统升级'
    }
}

# 短信验证码内容配置
SMSConfig={
    'api'          : www_sms_com_API,
    'verification' : '您的验证码是%s。请在页面中提交验证码完成验证。【随e筹】'
}

# 数据库配置
DbConfig = {
  'pool_name'         : 'dbpool',
  'pool_size'         : 16,       # pool_size 最大32
  'user'              : 'root',
  'password'          : '123456',
  'host'              : '192.168.1.210',
  'database'          : 'o2b',
  'raise_on_warnings' : True,
}

# Redis 配置
RedisConfig={
    'host'     : '192.168.1.210',
    'port'     : 6379,
    'db'       : 12,  # 默认为 12 号数据库
    'password' : 'jct2014redis'
}

# 表与Redis的对应关系
TableToRedisNo={
    'tbProductList':'08',
    'tbAdSense' : '09',
    'tbMsg'     : '10',
    'tbUser'    : '12'
}

# 图片文件存放的主路经（绝对路径）
imageRootPath='/var/www/o2b/v1.0.0/app'
imageConfig={
    'product.*' : {                                 # 图片类型 
        'path' : imageRootPath+'/images/products',  # 图片绝对路径
        'url'  : '/images/products',                # 图片URL地址
        'long' : 2560,                              # 图片长度
        'wide' : 900,                               # 图片宽度
        'size' : 4*1024*1024                        # 图片大小Byte,图片最大不充许超过4M
    },
    
   ...  
   
    'userheader'  : {                               # 用户头像
        'path' : imageRootPath+'/images/header',
        'url'  : '/images/header',
        'long' : 80,
        'wide' : 80,
        'size' : 1*512*1024
        },
    'temp' : {                                      # 临时文件路径
        'path':imageRootPath+'/images/tmp',
        'url' :'/images/tmp/{yyyy}{mm}{dd}'
    },
    # 支持的图片类型
    'imageFileType': ['image/gif', 'image/jpeg', 'image/pjpeg', 'image/bmp', 'image/png', 'image/x-png']
}

DEBUG = True    # 生产环境是设为 False
LOG_FILENAME = os.path.join(os.path.abspath('./logs/'), 'o2b.log')       # 日志文件路径
LOG_FORMAT   = '%(filename)s [%(asctime)s] [%(levelname)s] %(message)s'  # 日志文件名
```

 * Key & Secret 说明
* app_key 放置在每一个http请求的header（app_key）中，每一个URI必须要验证app_key的合法性；
*  app_secret 只用于登录验证，放在登录请求的header(app_secret)中。
*  *app_key &  app_secret* 需要开发者手动运行项目框架中的 `makeappkey.py` 产生。
*注：在运行 `makeappkey.py` 程序时，项目数据库中必须有一张名为 tabApps 的表,表结构如下：*
```
CREATE TABLE `tbApps` (
  `appId` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(80) NOT NULL COMMENT '应用名称',
  `description` varchar(240) DEFAULT NULL COMMENT '应用描述',
  `provider` varchar(64) DEFAULT NULL COMMENT 'App提供者',
  `type` varchar(45) DEFAULT NULL COMMENT 'APP产品类型',
  `website` varchar(80) DEFAULT NULL COMMENT '网站',
  `callback_url` varchar(240) DEFAULT NULL COMMENT '回调URL',
  `app_key` char(16) DEFAULT NULL COMMENT 'app key',
  `app_secret` char(64) DEFAULT NULL COMMENT 'app secret',
  `cratetime` datetime DEFAULT NULL,
  `updatetime` datetime DEFAULT NULL,
  `timestamp` varchar(32) DEFAULT NULL COMMENT '时间戳',
  PRIMARY KEY (`appId`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8 COMMENT='应用程序描述库';
```

----------

* 2.3 项目结构
> -- main.py 主程序
> -- route.py 路由程序             
> -- config.py 配置文件
> -- README 项目说明文件
> -- ChangeLog 项目修改记录
> -+ `Framework` Seagulls-PyDo基础框架包
> -+ `Service` 服务包
> -+ `easyOAuth` 认证鉴权包
> -+ `User` 用户管理包
> -+ `Group` 圈子包（可选）
> -+ `Message` 消息包（可选）
> -+ `AdSense` 广告包（可选）
> -+ `Address` 地址包（可选）
> -+ `Package 1` 项目业务包1（开发得自定义包）
> -+ `Package 2` 项目业务包2（开发得自定义包）
...
> -+ `Package n` 项目业务包N（开发得自定义包）

----------

## 3. 开发说明
### 3.1 框架结构说明
Seagulls-PyDo 微框架共分为三层:
>1. RESTFUL 解析层；
>2. 业务解析层；
>3. 业务逻辑实现层。

 * 3.1.1 RESTFUL 解析层
负责RESTFUL层Http协议的解析；
负责前端输入参数的适配与预处理； 
负责业务解析层和业务实现层的异常处理的捕获及处理;
负责将业务方法执行的结果以json或msgpake的方式返回给前端。
示例代码如下 ：
```
    # Restful Http Post 方法 
    @operator_except                     # 异常捕获及处理
    @tornado.web.asynchronous            # 异步处理
    @tornado.gen.coroutine    
    def post(self):
        user=self.objUserInfo['user']    # 得到用户名           
        objData=self.getRequestData()    # 得到通过 Http 协议提交数据 
        
        # 对从Http 协议提取的参数数据进行匹配
        try :
            data={
                'area'       : objData['a'],
                'empCode'    : objData['e'],
                'titleCode'  : objData['t'],
                'startTime'  : '{{now()}}',
                'createTime' : '{{now()}}'
            }
        except :
            raise BaseError(801) # 参数错误
        
        # 异步处理回调函数
        rowData = yield self.callback_saveTitle(data)
        self.response(rowData)  # 返回执行结果
```

`附：对多参数数据的自动适配`
    对多参数数据的自动适配主要用于针对数据的修改操作，例如：对“产品表”部分字段进行修改。在业务数据处理时前端JavaScript程序只需要提交用户已经修改的字段，后端需要自动适配送过来的更新数据。

程序代码示例如下：
```
            # 要修改的所有产品字段适配
            lstData={
                'code'          : 'c',
                'batchNo'       : 'bn',
                'name'          : 'name',
                'specification' : 'spec',
                'description'   : 'desc',                
                'Image'         : 'img',
                'imagelarge'    : 'imgl',
                'imageBanners'  : 'imgb',
                'imageSmall'    : 'imgs',
                'supplierCode'  : 'sc',
                'categoryCode'  : 'cat',
                'statusCode'    : 'st',
                'startTime'     : 'stm',
                'endTime'       : 'etm',
                'currentPrice'  : 'cp',
                'originalPrice' : 'op',
                'totalAmount'   : 'ta',
                'totalTopic'    : 'tt',
                'totalFollow'   : 'tf',
                'totalSold'     : 'ts',
                'limit'         : 'lmt',
            }
            
            data={}
            
            # 自动适配由前端传过来的数据
            for (k,v) in lstData.items():
                try:
                    data[k]=objData[v]
                except:
                    pass            
            
            if data is None or data=={} :
                raise BaseError(801) # 参数错误
```

 * 3.1.2. 业务解析层
业务解析层是RESTFUL层的异步回调函数；
业务解析层充当RESTFUL层与业务实现层的垫片，即我们需要在业务解析层实例化一个业务对象，并调用该对象与RESTFUL层匹配的方法。业务方法的执行结果以对象的方式传递返回给RESTFUL层。
示例代码如下：
```
    @run_on_executor    
    def callback_saveTitle(self,data) :
        t=entity.title(self.db) # 实例化 title 对象
        id=t.save(data)          # 将数据加入到 title 对象中，即插数据表
        rowData={ 'id' : id }        
        return rowData          # 将执行结果传递给 Restful 层  
```

 * 3.1.3. 业务逻辑实现层
业务逻辑实现层是具体业务逻辑实现方法的集合。
一个业务方法可由多个业务逻辑方法构成。
业务逻辑方法原则上用视图和存储过程的模式简化成单表的CURD操作。
示例代码如下：
```
class title(dbMysql.CURD) :
    def __init__(self,db) :
        super().__init__(db,'EmpTitle') # 定义本实例需要操作的表名
```

----------

### 3.2 开发步聚说明
> 第1步： Clone 一个 Seagulls-PyDo框架；
> 第2步： 用 WingIde 等 Python IDE 集成环境新建一个工程文件，并将 Clone 的Seagulls-PyDo框架导入；
> 第3步： 根据需要配置 config.py 里面的参数；
> 第4步： 在项目中新建一上业务包；
> 第5步： 在新建的业务包中，新建一个 entity 模块，并贴入下面的代码 ：
```
class 类名(dbMysql.CURD) :  # 请将“类名”更换为你定义的类名
    def __init__(self,db) :
        super().__init__(db,'表名')  # 请将“表名”更换为你定义的去把掉前缀"tb"的数据库表名
```
> 注：如果只是简单的增删改查，贴入代码即可。如果开发者需要增加更多的业务逻辑请在这个类中增加相关的业务方法；

> 第6步： 在业务包中，新建一 Restful 层的模块，模块名根据需要自定，例如：list.py , 并贴入下面的代码（以新增为例 ）： 
```
from Framework.Base  import WebRequestHandler,BaseError,operator_except
from mysql.connector import errors,errorcode
from 包名 import entity实体类模块  # 请替换为上一步聚建立的实体类及对象，如 from Title import entity

class Handler(WebRequestHandler):
    # 新增
    @operator_except
    @tornado.web.asynchronous            # 异步处理
    @tornado.gen.coroutine    
    def post(self):
        user=self.objUserInfo['user']             
        objData=self.getRequestData()
        
        # 适配层
        try :
            data={
                # 1. 这城填写需要与表中字段适配的参数,左侧Key为表的字段，右则Value为参数，如 ：
                # 'area'       : objData['a'],
                # 'empCode'    : objData['e'],
                # 'titleCode'  : objData['t']，
                
                # 这城填写需要加入的字段,如 ：
                # 'startTime'  : '{{now()}}',
                # 'createTime' : '{{now()}}'
            }
        except :
            raise BaseError(801) # 参数错误
        
        # 增加异步处理层
        rowData = yield self.callback_saveTitle(data)
        self.response(rowData)
        
   # 异步处理回调函数
   @run_on_executor    
    def callback_addTitle(self,data) :  # data 为要插入的数据
        t=entity.title(self.db)         # 实例话一个对象
        id=t.save(data)                  # 加入数据，并返回 id 
        rowData={ 'id' : id }           # 重新封状数据返回，这城开发者可以根据业务需要重新封装
        return rowData                  # 返回给 Restful 层
```
> 第7步： 定义 URI 并加入到 route.py 中，如上例：
```
import Title           # 导入包
from Title import list # 导入模块
handlers = [
            ...

            # 岗位管理
            (r"/zbdy/v1.0.0/title", Title.list.Handler),  # 指明 URI 与 类的对应关系
            
            ...
        ]
```
> 第8步. 调试运行。


### 3.3 开发实例
3.3.1 学生单表CURD示例
以简化的学生管理为例，做一个订单的增删改查 URI

3.3.1.1 相关表结构说明
学生表：tbStudent(id,classId,name,birthday,gender,createTime,isDelete,status)
班级表：tbClass(id,name,total,createTime,isDelete)
学生视图 : vwStudent(id,classId,className,name,birthday,gender,createTime,status)

3.3.1.2  在项目中创建名为 Student 的包，注意，首字母要大写；

3.3.1.3 在 Student 包中创建一个订单实例模块 entity.py 内容如下：
```
class student(dbMysql.CURD) :
    def __init__(self,db) :
        super().__init__(db,'Student') # 定义本实例需要操作的表名
```
3.3.1.4 在 Student 包中创建一个 Restful 处理模块 list.py 内容如 ：
```
from Framework.Base  import WebRequestHandler,BaseError,operator_except
from mysql.connector import errors,errorcode
from Student import entity

class Handler(WebRequestHandler):
    # 创建学生信息
    @operator_except
    def post(self):
        user=self.objUserInfo['user']  # 得到当前的用户             
        objData=self.getRequestData()  # 得到Http Request 数据
        
        # 参数适配层
        try :
            # 匹配数据
            data = {
              'name'       : objData['name'],  
              'classId'    : objData['class'],
              'birthday'   : objData['birthday'],
              'gender'     : 'M' if objData['gender']=='男' else 'F',
              'createTime' : '{{now()}}'
            }
        except :
            raise BaseError(801) # 参数错误
        
        # 创建学生信息
        o=entity.student(self.db)   # 实例话一个对象
        id=o.save(data)           # 保存学生信息，并返回订单 id 
        rowData = {
          'Student Id' : id
        }
        self.response(rowData)
        
    # --------------------------------------------------------------------------
    # 删除学生信息
    @operator_except
    def delete(self,id):
        o=entity.student(self.db)                # 实例话一个对象
        o.remove(id)                           # 从tbStudent表中移除指定id的订单 
        self.response()
        
    # --------------------------------------------------------------------------
    # 更改学生班级编码状态
    @operator_except
    def patch(self):
        objData=self.getRequestData()  # 得到Http Request 数据
        s=entity.student(self.db)                # 实例话一个对象
        try :
            id = objData['id']
            data={
              'classId' : objData['class']
            }
        except :
            raise BaseError(801) # 参数错误
        s.save(data,id)                        # 修改tbStudent表中的status 
        self.response()
        
    # --------------------------------------------------------------------------
    # 查询某个学生的详情
    @operator_except
    def get(self,idOrName):
        s=entity.student(self.db)                # 实例话一个对象
         cond ={
          'select' : 'id,className,name,birthday,gender,createTime,status',
          'where' : "id='%s' or name='%s' " % (idOrName,idOrName)
        }
        studentData=s.find(cond,'vwStudent')           # 查询订单信息
        rowData={
          'struct’ ：studentData['struct'],
          'data'   : studentData['rows']
         }
        self.response(rowData)
    
```
3.3.1.5  在项目中创建名为 Class 的包，注意，首字母要大写；
    
3.3.1.6 在 Class 包中创建一个订单实例模块 entity.py 内容如下：
```
class class(dbMysql.CURD) :
    def __init__(self,db) :
        super().__init__(db,'Student') # 定义本实例需要操作的表名
```
3.3.1.7 在 Class 包中创建一个 Restful 处理模块 list.py 内容如 ：
```
from Framework.Base  import WebRequestHandler,BaseError,operator_except
from mysql.connector import errors,errorcode
from Student import entity

class Handler(WebRequestHandler):
    # 查询某班的所有学生详情
    @operator_except
    def get(self,cid):
        s=entity.student(self.db)                # 实例话一个对象
         cond ={
          'select' : 'id,className,name,birthday,gender,createTime,status',
          'where' : 'classId=%s' % (cid,)
        }
        studentData=s.find(cond,'vwStudent')           # 查询学生信息
        rowData={
          'struct’ ： studentData['struct'],
          'data'   : studentData['rows']
         }
        self.response(rowData)
```
3.3.1.8 在项目的 route.py 模块中添加URI与类的映射 ：
```
...

import Student,Class
from Student import list
from Class import list

handlers = [
            ... 
            # 学生管理
            (r"/zbdy/v1.0.0/student", Student.list.Handler),
            (r"/zbdy/v1.0.0/student/([0-9]+)", Student.list.Handler),
            (r"/zbdy/v1.0.0/class/([0-9]+)", Class.list.Handler),
            ...
        ]
            
```


3.3.2 订单管理示例
以简化的订单管理为例，做一个订单的增删改查 URI

3.3.2.1 相关表结构说明
订单表：tbOrders(id,user,total,amount,createTime,isDelete,status)
订单详单表：tbOrderDetail(id,oid,pid,productName,price,quantity,amount,createTime,isDelete)
用户表：tbUser(id,user,name,email,phone,createTime,isDelete)
地址表: tbAddress(id,user,address,default,createTime,isDelete)
订单视图 : vwOrders(id,total,amount,user,userName,email,phone,address,createTime,status)

3.3.2.2 在项目中创建名为 Order 的包，注意，首字母要大写；

3.3.2.3 在 Order 包中创建一个订单实例模块 entity.py 内容如下：
```
class order(dbMysql.CURD) :
    def __init__(self,db) :
        super().__init__(db,'Order') # 定义本实例需要操作的表名
    
    # 生成订单方法    
    def crateOrder(self,data) :
        lstDetail = data['detail']
        user      = data['user']

        # 计算订单总金额及数量
        numSumTotal = 0
        numSumAmount = 0
        for objDetail in lstDetail :
            numSumTotal  += int(objDetail['quantity'])  # 总数量
            numSumAmount +=float(objDetail['amount'])   # 总金额
            
       # 生成订单数据
       data={
            'user'   : user,
            'total'  : numSumTotal,
            'amount' : numSumAmount,
            'createTime' : '{{now()}}'
       } 
        # 创建订单
        id=self.save(data,table='tbOrders')   # 加入订单数据，并返回 id
        # 创建详单
        for objDetail in lstDetail :
            objDetail['oid']=id
            objDetail['createTime'] = '{{now()}}'
            self.save(objDetail,table='tbOrderDetail')   # 加入订单详单数据，并返回 id
        
        return id
        
    # 查询订单方法 
    def getOrderInfo(self,id):
        condOrder ={
          'select' : 'id,total,amount,user,userName,email,phone,address,status',
          'where' : 'id=%s' % (id,)
        }
        
        condDetail ={
          'select' : 'id,oid,pid,productName,price,quantity,amount',
          'where' : 'oid=%s' % (id,)
        }
        
        orderData=self.find(condOrder,'vwOrders')
        detailData=self.find(condDetail,'tbOrderDetail')
        
        rowData={
          'orderData' : { 'struct' : orderData['struct'],
                          'data'   : orderData['rows']
                        },
          'detailData' : { 'struct' : orderData['struct'],
                           'data'   : detailData['rows']
                         }
        }
        
        return rowData

```
3.3.2.4 在 Order 包中创建一个订单Restful处理模块 list.py 内容如 ：
注：以下示例未采用异步处理方式。

```
from Framework.Base  import WebRequestHandler,BaseError,operator_except
from mysql.connector import errors,errorcode
from Order import entity

class Handler(WebRequestHandler):
    # 创建订单
    @operator_except
    def post(self):
        user=self.objUserInfo['user']  # 得到当前的用户             
        objData=self.getRequestData()  # 得到Http Request 数据
        
        # 参数适配层
        try :
            # 匹配订单数据
            data = {
              'detail' : objData['detailData'],  # 详单List
              'user'   : user 
        except :
            raise BaseError(801) # 参数错误
        
        # 生成订单
        o=entity.order(self.db)         # 实例话一个对象
        id=o.crateOrder(data)           # 创建订单，并返回订单 id 
        rowData = {
          'Order Id' : id
        }
        self.response(rowData)
        
    # --------------------------------------------------------------------------
    # 删除订单
    @operator_except
    def delete(self,id):
        o=entity.order(self.db)                # 实例话一个对象
        o.remove(id)                           # 从tbOrders表中移除指定id的订单 
        o.remove(id,'tbOrderDetail',key='oid)  # 从tbOrderDetail表中移除oid为id的详单
        self.response()
        
        
    # --------------------------------------------------------------------------
    # 更改订单状态
    @operator_except
    def patch(self,id):
        objData=self.getRequestData()  # 得到Http Request 数据
        o=entity.order(self.db)                # 实例话一个对象
        try :
            id = objData['id']
            data={
              'status' : objData['status']
            }
        except :
            raise BaseError(801) # 参数错误
        o.save(data,id)                        # 修改tbOrders表中的status 
        o.save(data,id,table='tbOrderDetail')  # 修改tbOrderDetail的status
        self.response()
        
    # --------------------------------------------------------------------------
    # 查询订单详情
    @operator_except
    def get(self,id):
        o=entity.order(self.db)                # 实例话一个对象
        orderData=o.getOrderInfo(id)                     # 查询订单信息 
        self.response(orderData)
```
3.3.2.5 在项目的 route.py 模块中添加URI与类的映射 ：
```
...

import Order
from Order import list

handlers = [
            ... 
            # 订单管理
            (r"/zbdy/v1.0.0/order", Order.list.Handler),
            (r"/zbdy/v1.0.0/order/([0-9]+)", Order.list.Handler),
            ...
        ]
            
```

----------

### 3.4 数据库表的命名规范
在 Seagulls-PyDo 微框架中对数据库中的表、视图、存贮过程及函数一律采用驼峰方式命名。

数据库命名无特殊要求；
数据库中的`表`以`"tb"`开头后跟首字母大写的表名，如： `tbStudent`, `tbOrderDetail`; 
数据库中的`视图`以`"vw"`开头后跟首字母大写的表名，如： `vwStudent`, `vwOrderDetail`; 
数据库中的`存贮过程`以`"pr"`开头后跟首字母大写的操作说明，如： `prCreateOrder`；
数据库中的`存贮过程函数`以`"fn"`开头后跟首字母大写的操作说明，如： `fnGetTitle`；
数据库中的`表`必须含用一个`自增量`的`id`字段。

----------

### 3.5 数据库操作的常用方法说明

3.5.1 CURD 类核心方法说明
```
class CURD(object) :
    def __init__(self,db=None,tableName='') :
    def select(self,tableName):
    def find(self,conditions,table=None,isRaise=True):
    def save(self,data,ids='',table=None,_commit=False):
    def remove(self,ids,table=None,key='id',_commit=False) :
```

3.5.1.1 `__init__ 方法`

* CURD 类由 `entity.py` 模块中的类实例继承，初始化时应将系统的self.db及表名引入。如下例代码：
```
class order(dbMysql.CURD) :
    def __init__(self,db) :
        super().__init__(db,'Order') # 定义本实例需要操作的表名
```
此时 order 类中 :
self.table = 'tbOrder'
self.view = 'vwOrder'

3.5.1.2 `select 方法`：
将重置初始化方法中设置的表名与视图名

3.5.1.3  `find 方法`：
将查询满足`conditions`中指定条件的记录，默认查找的表为 self.table ，如果方法中设置table参数将查询table指定的表。如： find(conditions,'vwStudent');

isRaise是指当要多次顺序调用find方法进行查询时，若将isRaise设置为False，find在未能查询到数据时不抛出“802没有找到数据”异常，这样便于程序将多个查询的记录进行汇总比对。
isRaise默认值为True。

 * `find 输入参数`:
conditions 的格式如下：
```
conditions={
    'select' : '此处填写要得到的字段，字段之间用 "," 分隔',
    'where'  : '此处填写查询条件',
    'order'  : '此处填写排序字段',
    'limit'  : '%s,%s' % (offset,rowcount)
}            
```
例如：
```
conditions={
    'select' : "gid,name,cat,membership,totalTopic,status_code,status,isVerifyJoin,isPrivate,{{CONCAT('%s/'}},{{header) as header}}" % (config.imageConfig['groupheader']['url']),
    'where'  : "FIND_IN_SET(UPPER(status_code),'OPEN,PAUSE')>0",
    'limit' : '%s,%s' % (offset,rowcount)
}            
```
在 `select` 或 `where` Key 的 Value 值中可以引用 `{{ }}` 将SQL的关键字括入；
`where` Key 的 Value 值可以是字典，当值为字典时，字典中各Key之间为`与`关系 。如：
```
conditions={
    'select' : "gid,name,cat,membership,totalTopic,status_code,status"),
    'where'  : {
               'cat' : '100000',
               'status' : 'WAIT'
               },
     'limit' : '%s,%s' % (offset,rowcount)
}            
```
即： 条件是 cat='100000' and status='WAIT'

 * `find 返回` ：
{
   'struct' : '与rows list相对应的数据字段名',
   'rows' : list类型的数据列表
}
如果 find 条件 conditions 中设置了分页显示，则返回：
{
   'count' : 满足条件的记录数（如果有分页查询条件时）, 
   'struct' : '与rows list相对应的数据字段名',
   'rows'  : list类型的数据列表
}

3.5.1.4  `save 方法`：
将 data 数据存入默认的表 `self.table` 中 ，如果方法中设置table参数 data 数据将会存入table指定的表。如： 
id = save(data,'tbOrderDetail')
 * `输入参数`:
```
 data = {
   '字段名1' : '值',
   '字段名2' : '值',
   ...
   '字段名n' : '值'
}
```
如果参数中加入 ids , 则表示将数据 data 存入到满足ids的记录中，ids 为id的集合，id 之间用 "," 分隔。 即Update满足id in (ids) 中的数据。如：
id = save(data,'1,3,5,7,9','tbOrderDetail')

 * `返回`: 无 ids 参数返回数据插入后的 id，有ids参数时修改成功后无返回。 

3.5.1.5  `remove 方法`：
将指定的一组 id 从默认的table表中移除。如：  self.remove(ids) 或 self.remove(ids,'tbOrderDetail');

同时也可以通过key指定其它的key值（关键字），如想删除字段为pat_id='A1001,A1002'的数据,
self.remove('A1001,A1002','tbTest',key='pat_id')

 * `输入参数`: ids , ids 为id的集合，id 之间用 "," 分隔。如：
self.remove('1,3,5,7,9')
 * `返回`: 移除成功后无返回。 


----------
3.5.2 DB 类常用方法说明

```
class DB(object):
    def count(table,params={},join='AND'): # 根据条件统计行数
    def getToListByPk(table,criteria={},id=None,pk='id'): # 根据条件查找记录返回List
    def getAllToList(table,criteria={},id=None,pk='id',join='AND'): # 根据条件查找记录返回List
    def getToObjectByPk(table,criteria={},id=None,pk='id'): # 根据条件查找记录返回Object
    def getAllToObject(table,criteria={},id=None,pk='id',join='AND'):# 根据条件查找记录返回Object
    def insert(table,data,commit=True): # 新增一条记录
    def update(table,data,params={},join='AND',commit=True,lock=True): # 更新数据
    def updateByPk(table,data,id,pk='id',commit=True,lock=True): # 根据主键更新，默认是id为主键
```


----------

### 3.6 Http Request 数据的方法 
3.6.1 得到 Http Get 方法传递的参数 `self.get_argument()`
> 用法：
 返回值=self.get_argument(参数,default=缺省值)
 
 示例：
```
# 取得 http://localhost:8080/zbdy/stduent?o=0&r=10 链接中的 o 与 r 值
offset=int(self.get_argument("o",default=0))
rowcount=int(self.get_argument("r",default=1000))
```

3.6.2 得到 Http Post 方法传递的数据 `self.getRequestData()`
> 用法：
 返回值=self.getRequestData()

 示例：
```
# 取得 http://localhost:8080/zbdy Post 方法传送的数据 name 与 id
objData=self.getRequestData()
id   = objData['id']
name = objData['name']
```

----------

### 3.7 得到已登录用户的用信息  `self.getUserInfo()`
> 用法：
 用户数据对象=self.getUserInfo()

 示例：
```
# 取得当前用户的用户名/头像/email/phone
objUser=self.getUserInfo()
user=objUser['user']
img_header=objUser['header']
email=objUser['email']
phone=objUser['mobile']
```

----------
### 3.8 不需要用户登录直接访问  `super().get(self,userAuth=False)`
> 用法：
 1) 方法前不设置修饰符： @operator_except ;  
 2) 在get/post/put/patch方法中继承上一级的调和方法，并将userAuth=False ;
 3) 如果要操作数据库需要，手功设置开启( self.openDB() )和关闭( self.closeDB() )数据库链接 。
 
 示例：
```

 def get(self):
        try :
            super().get(self,userAuth=False)
            self.db=self.openDB()
            hu=entity.user()
            uid=hu.add(data,self.db)
            self.closeDB()
            ...

```

----------

### 3.9 错误处理
异常捕获错误处理的代码及信息定义 `Framework` 包的 `baseException.py` 模块中。
错误码用于URI的错误信息返回，其中：
>* code - 是业务层级的定义；
其中：
   code = 600-699 -->  鉴权类错误
   code = 700-799 -->  数据库类错误
   code = 800-899 -->  业务类错误
   code = 900     -->  未被定义的错误类型
>* status - 为http对应的status；
>* message - 是错误文字说明；
>* original message - 是原始错误信息，比如针对数据操作时的错误信息。该信息只是在需要时显示；
>* help_document - 是错误说明指引URI。


   常见错误代码定义如下: 
```
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
            "message": "无法识别的用户名或密码",
            "help_document": "/oauth/v1.0.0/help/603"
        },
        604: {
            "code": "604",
            "status":404,
            "message": "未授权的访问",
            "help_document": "/oauth/v1.0.0/help/604"
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
        811 : {
            "code"          : "811",
            "status"        : 400,
            "message"       : "未知的图片格式（从文件后缀上判断）",
            "help_document" : "/o2b/v1.0.0/help/811"
        },
        812 : {
                    "code"          : "812",
                    "status"        : 400,
                    "message"       : "未知的图片格式",
                    "help_document" : "/o2b/v1.0.0/help/811"
        },        
        813 : {
            "code"          : "813",
            "status"        : 400,
            "message"       : "值得怀疑的图片(非法图片格式)",
            "help_document" : "/o2b/v1.0.0/help/812"
        },
        814 : {
            "code"          : "814",
            "status"        : 400,
            "message"       : "图片长宽超界",
            "help_document" : "/o2b/v1.0.0/help/814"
        },
        815 : {
            "code"          : "815",
            "status"        : 404,
            "message"       : "没有找到图片",
            "help_document" : "/o2b/v1.0.0/help/815"
        },
        816 : {
            "code"          : "816",
            "status"        : 400,
            "message"       : "文件存贮失败！",
            "help_document" : "/o2b/v1.0.0/help/816"
        },
        817 : {
            "code"          : "817",
            "status"        : 400,
            "message"       : "文件移动失败！",
            "help_document" : "/o2b/v1.0.0/help/817"
        },  
        818 : {
            "code"          : "818",
            "status"        : 400,
            "message"       : "发送消息失败！",
            "help_document" : "/o2b/v1.0.0/help/818"
        },
        823 : {
            "code"          : "823",
            "status"        : 400,
            "message"       : "插入数据失败!(redis)",
            "help_document" : "/o2b/v1.0.0/help/823"
        }, 
        831 : {
            "code"          : "831",
            "status"        : 400,
            "message"       : "短信网关通信失败！",
            "help_document" : "/o2b/v1.0.0/help/831"
        },
        832 : {
            "code"          : "832",
            "status"        : 400,
            "message"       : "短信网关API回传错误！",
            "help_document" : "/o2b/v1.0.0/help/832"
        },        
        890 : {
            "code"          : "890",
            "status"        : 400,
            "message"       : "读 Config.json 配置文件失败！",
            "help_document" : "/o2b/v1.0.0/help/890"            
        }
}
```