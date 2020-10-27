# coding:utf-8
# UDP server 端配置参数，和具体业务无关。业务配置参数由业务定义
# 

MSG_START_FLAG			= 0xA5A5A5A5
MSG_END_FLAG			= 0xDEDEDEDE

MESSAGE_MAX_LEN			= 1024 * 1024

ACK_TIMEOUT				= 4
RESEND_LIMIT			= 3
REQUEST_TIMEOUT			= 20 # 20 seconds

# 消息类型起始值，业务消息在此基础上自行加1
MSG_TYPE_BASE			= 0

# 每个业务模块消息类型值的最大数量
MAX_MSG_COUNT 			= 1000

HEARTBEAT_MSG			= MSG_TYPE_BASE + 1
HEARTBEAT_ACK_MSG		= MSG_TYPE_BASE + 2

#心跳消息上报时间间隔，单位为秒
HEARTBEAT_INTERVAL 	= 60

# 消息最大超时次数
MSG_EXPIRE_TIMES 		= 3

# 消息重发时间间隔
MSG_RESEND_INTERVAL 	= 5
