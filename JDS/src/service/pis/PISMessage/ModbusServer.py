
# coding:utf-8

# 车站服务器 Modbus 消息处理

import sys, time, threading, datetime

import urllib.request
from socket import *

from libs.utils.debug import *
from libs.utils.utils import *
from libs.db.dbHelper import *

from config import PisConfig

from pis.PISMessage.PISMessageBase import *
from pis.PISMessage.PISMessage import *
from pis.PISMessage.PISSocket import *
from pis.PISMessage.PISMsgServer import *


# Modbus 服务器，处理综合监控Modbus消息
# 中心服务器只处理综合监控的紧急消息，如果综合监控发紧急消息给中心服务器的话
# 车站服务器处理紧急消息和到站消息
# 
class CModbusServer(threading.Thread):
    def __init__(self, msgServer):
        threading.Thread.__init__(self)

        # # 站厅、上下行控制器状态
        self.ctrlHState     = 0
        self.ctrlUState     = 0
        self.ctrlDState     = 0

        self.ctrlHError     = 0
        self.ctrlUError     = 0
        self.ctrlDError     = 0

        self.TCPServer      = None                                          # TCP Server Socket，Modbus通信
        self.msgServer      = msgServer
        self.LocalAddr      = msgServer.LocalAddr

        # 从综合监控要接收5类消息
        self.EmergenceMsg   = CModbusEmergenceMsg()                         # 待接收的紧急消息
        self.DownTimeMsg    = CModbusStationTimeMsg(ModbusDownTimeMsg)      # 下行到站消息
        self.UpTimeMsg      = CModbusStationTimeMsg(ModbusUpTimeMsg)        # 上行到站消息

        self.stationInfo    = None
        self.hallCtrl       = None
        self.upCtrl         = None
        self.downCtrl       = None

        self.ctrlStatus     = {}                                            # 控制器状态数据

        self.isStation      = msgServer.isStation

        return

    def __del__(self):
        self.Close()

        return

    # 启动
    def run(self):

        # 是否配置了PIS业务
        if "PIS" not in config.SystemModule:
            logE("Error: System not config PIS module")
            return

        self.GetStationInfo()

        # 启动TCP服务端，开始准备接收Modbus消息
        self.TCPServer = CSocketServer( SOCK_STREAM, PisConfig["ModbusPort"], self.ModbusMessageProcess )
        self.TCPServer.start()

        # 进入线程主循环，等待发送或接收消息
        while (1):
            time.sleep(1)

    def Close(self):
        if self.TCPServer: 
            self.TCPServer.close()

        return

    # 获取车站及车站控制器信息
    def GetStationInfo(self):
        
        if not self.isStation or self.isStation == False:
            # 配置为中心服务器
            self.stationInfo = {
                "id"            : 0,
                "install_type"  : '8',
                "name"          : "中心服务器",
                "code"          : "CCCCCC",
                "ip_address"    : self.LocalAddr,
            }

            self.hallCtrl   = None
            self.upCtrl     = None
            self.downCtrl   = None
            return

        # 获取该车站信息及站厅、上下行控制器
        dbHelper = DbHelper()
        try:
            db      = dbHelper.getDB("pis.station")
            stationCode    = PisConfig["stationCode"]
            result  = db.findByCond(db.getFields(), "code='%s'" % stationCode)
            if not len(result["rows"]):
                logE("Error: Can not find station(%s) infomation"%stationCode)
                return

            keys = result["struct"].split(",")
            station = dict(zip(keys, result["rows"][0]))

            stationId   = station["id"]
            stationName = station["name"]

            hallCtrl    = None
            upCtrl      = None
            downCtrl    = None

            db      = dbHelper.getDB("pis.controller")
            fields  = db.getFields()
            keys    = fields.split(",")

            result = db.findByCond(fields, "install_id=%d and install_type='%s'" % (stationId, INSTALL_TYPE_HALL))
            if len(result["rows"]):
                hallCtrl = dict(zip(keys, result["rows"][0]))
                hallCtrl['name'] = self.msgServer.getControllerName(hallCtrl)

            result = db.findByCond(fields, "install_id=%d and install_type='%s'" % (stationId, INSTALL_TYPE_UP))
            if len(result["rows"]):
                upCtrl = dict(zip(keys, result["rows"][0]))
                upCtrl['name'] = self.msgServer.getControllerName(upCtrl)

            result = db.findByCond(fields, "install_id=%d and install_type='%s'" % (stationId, INSTALL_TYPE_DOWN))
            if len(result["rows"]):
                downCtrl = dict(zip(keys, result["rows"][0]))
                downCtrl['name'] = self.msgServer.getControllerName(downCtrl)

            station['install_type'] = '10'
            station['name']         = stationName + "车站服务器"

            self.stationInfo        = station
            self.hallCtrl           = hallCtrl
            self.upCtrl             = upCtrl
            self.downCtrl           = downCtrl

        except Exception as e:
            onException(e)

        dbHelper.closeDB()

    # Modbus消息处理
    # 功能码：
    #   0x03: 读取控制器状态
    #   0x10：向控制器发送消息
    def ModbusMessageProcess(self, dataBuf, clientSocket, clientAddr):

        # logI("Modbus Data: %s:%d %s"%(clientAddr[0], clientAddr[1], ConvertToHex(dataBuf)))

        msg             = CModbusMsg()
        msg.srcAddr     = clientAddr[0]
        msg.isSendMsg   = False
        msg.isAckMsg    = False

        msg.setMsgBuffer(dataBuf)
        # 在接收到的数据中，可能会有连续多条消息的数据
        while True:
            if msg.decode(clientAddr) == False:
                break

            if msg.FunctionCode == 0x03:
                # self.OnModbusReadData(msg)
                retMsg = self.GetModbusResponseMsg(msg)

            elif msg.FunctionCode == 0x10:
                try:
                    self.OnModbusWriteData(msg)
                except Exception as e:
                    onException(e)

                retMsg = self.GetModbusResponseMsg(msg)

            else:
                retMsg = sekf.GetModbusResponseMsg(msg, error = True)

            if retMsg:
                # 记录消息日志
                self.LogMsg(msg)
                self.SendModbusMessage(clientSocket, clientAddr, retMsg)

    # 设置响应消息的消息数据，控制器状态
    def GetControllerStatus(self, RegisterAddr):
        
        if RegisterAddr == MB_ADDR_STATUS_HC:
            controller = self.hallCtrl
        elif RegisterAddr == MB_ADDR_STATUS_UC:
            controller = self.upCtrl
        elif RegisterAddr == MB_ADDR_STATUS_DC:
            controller = self.downCtrl
        else:
            logE("Error: ModbusServer read, not supported RegisterAddr : %04x" % RegisterAddr)
            return

        logI("Modbus Query %s Status"%controller['name'])

        # 查询控制器连接状态
        if self.isStation == False:
            # 中心服务器，直接从中心服务器的在线控制器列表中查询
            onlineStatus = (self.msgServer.FindOnlineController(controller) == None)
        else:
            # 车站服务器，从数据库中查询
            dbHelper    = DbHelper()
            db          = dbHelper.getDB("pis.controller")
            result      = db.findByCond('status', "id=%d"%controller['id'])
            dbHelper.closeDB()

            if len(result["rows"]):
                onlineStatus = ( result['rows'][0][0] == '0')           # 0: 在线，5：离线
            else:
                onlineStatus = False
     
        logI("Query %s status: %s"%(controller['name'], "Online" if onlineStatus else "Offline"))

        # 查询控制器当前连接状态
        data = 1 if onlineStatus else 0
        # self.ctrlStatus[RegisterAddr] = 1 if onlineStatus else 0 
        return data

    # 接收读控制器状态消息
    def OnModbusReadData(self, msg):
        pass
        # RegisterAddr = msg.RegisterAddr
        # logD("ModbusServer onReadData RegisterAddr : %04x" % RegisterAddr)

        # if RegisterAddr == MB_ADDR_STATUS_HC:
        #     self.SetModbusData([self.ctrlHState | self.ctrlHError])
        # elif RegisterAddr == MB_ADDR_STATUS_UC:
        #     self.SetModbusData([self.ctrlUState | self.ctrlUError])
        # elif RegisterAddr == MB_ADDR_STATUS_DC:
        #     self.SetModbusData([self.ctrlDState | self.ctrlDError])
        # else:
        #     self.SetModbusData([0])
        #     logE("Error: ModbusServer read, not supported RegisterAddr : %04x" % RegisterAddr)

    # 接收综合监控发出的消息
    def OnModbusWriteData(self, requestMsg):

        RegisterAddr    = requestMsg.RegisterAddr
        ModbusMsgData   = requestMsg.msgData

        if RegisterAddr == MB_ADDR_CTRL_HC or RegisterAddr == MB_ADDR_CTRL_UC or RegisterAddr == MB_ADDR_CTRL_DC or RegisterAddr == MB_ADDR_MSG_SEL_ALL:
            val = self.decodeWord(ModbusMsgData)

            msg = self.EmergenceMsg
            msg.srcAddr = requestMsg.srcAddr

            msg.selectedCtrl = RegisterAddr
            msg.selectMode = (val & 0x04) != 0
            logI("Receive %s, select: %s"%(ModbusMsgType[RegisterAddr], "True" if msg.selectMode else "False"))
            self.SendEmergenceMsg()

        elif RegisterAddr == MB_ADDR_MSG_MODE:
            val = self.decodeWord(ModbusMsgData)

            msg = self.EmergenceMsg
            msg.srcAddr = requestMsg.srcAddr
            msg.emergenceMode = (val & 0x01)
            logI("Receive %s, Mode: %d"%(ModbusMsgType[RegisterAddr], msg.emergenceMode))
            self.SendEmergenceMsg()

        elif RegisterAddr == MB_ADDR_MSG_CONTENT:
            contentData = self.swapWord(ModbusMsgData)
            
            msg = self.EmergenceMsg
            msg.srcAddr = requestMsg.srcAddr

            msg.msgContent = self.decodeString(contentData)
            logI("Receive %s, content: %s"%(ModbusMsgType[RegisterAddr], msg.msgContent))
            self.SendEmergenceMsg()

        elif RegisterAddr == MB_ADDR_D1_DEST:
            msg = self.DownTimeMsg
            msg.srcAddr = requestMsg.srcAddr

            msg.stationID = self.decodeWord(ModbusMsgData)
            logI("Receive %s, Arriving Station ID: %d"%(ModbusMsgType[RegisterAddr], msg.stationID))
            self.SendDownArriveMsg()

        elif RegisterAddr == MB_ADDR_D1_TIME_HH:
            msg = self.DownTimeMsg
            msg.srcAddr = requestMsg.srcAddr

            msg.arriveHour = self.decodeWord(ModbusMsgData)
            logI("Receive %s, Hour: %d"%(ModbusMsgType[RegisterAddr], msg.arriveHour))
            self.SendDownArriveMsg()

        elif RegisterAddr == MB_ADDR_D1_TIME_MM:
            msg = self.DownTimeMsg
            msg.srcAddr = requestMsg.srcAddr

            msg.arriveMinute = self.decodeWord(ModbusMsgData)
            logI("Receive %s, Minute: %d"%(ModbusMsgType[RegisterAddr], msg.arriveMinute))
            self.SendDownArriveMsg()

        elif RegisterAddr == MB_ADDR_D2_DEST:
            msg = self.DownTimeMsg
            msg.srcAddr = requestMsg.srcAddr
            msg.nextStationID = self.decodeWord(ModbusMsgData)
            logI("Receive %s, Arriving Station ID: %d"%(ModbusMsgType[RegisterAddr], msg.stationID))
            self.SendDownArriveMsg()

        elif RegisterAddr == MB_ADDR_D2_TIME_HH:
            msg = self.DownTimeMsg
            msg.srcAddr = requestMsg.srcAddr

            msg.nextArriveHour = self.decodeWord(ModbusMsgData)
            logI("Receive %s, Hour: %d"%(ModbusMsgType[RegisterAddr], msg.arriveHour))
            self.SendDownArriveMsg()

        elif RegisterAddr == MB_ADDR_D2_TIME_MM:
            msg = self.DownTimeMsg
            msg.srcAddr = requestMsg.srcAddr

            msg.nextArriveMinute = self.decodeWord(ModbusMsgData)
            logI("Receive %s, Minute: %d"%(ModbusMsgType[RegisterAddr], msg.arriveMinute))
            self.SendDownArriveMsg()

        elif RegisterAddr == MB_ADDR_U1_DEST:
            msg = self.UpTimeMsg
            msg.srcAddr = requestMsg.srcAddr

            msg.stationID = self.decodeWord(ModbusMsgData)
            logI("Receive %s, Arriving Station ID: %d"%(ModbusMsgType[RegisterAddr], msg.stationID))
            self.SendUpArriveMsg()

        elif RegisterAddr == MB_ADDR_U1_TIME_HH:
            msg = self.UpTimeMsg
            msg.srcAddr = requestMsg.srcAddr

            msg.arriveHour = self.decodeWord(ModbusMsgData)
            logI("Receive %s, Hour: %d"%(ModbusMsgType[RegisterAddr], msg.arriveHour))
            self.SendUpArriveMsg()

        elif RegisterAddr == MB_ADDR_U1_TIME_MM:
            msg = self.UpTimeMsg
            msg.srcAddr = requestMsg.srcAddr

            msg.arriveMinute = self.decodeWord(ModbusMsgData)
            logI("Receive %s, Minute: %d"%(ModbusMsgType[RegisterAddr], msg.arriveMinute))
            self.SendUpArriveMsg()

        elif RegisterAddr == MB_ADDR_U2_DEST:
            msg = self.UpTimeMsg
            msg.srcAddr = requestMsg.srcAddr

            msg.nextStationID = self.decodeWord(ModbusMsgData)
            logI("Receive %s, Arriving Station ID: %d"%(ModbusMsgType[RegisterAddr], msg.stationID))
            self.SendUpArriveMsg()

        elif RegisterAddr == MB_ADDR_U2_TIME_HH:
            msg = self.UpTimeMsg
            msg.srcAddr = requestMsg.srcAddr

            msg.nextArriveHour = self.decodeWord(ModbusMsgData)
            logI("Receive %s, Hour: %d"%(ModbusMsgType[RegisterAddr], msg.arriveHour))
            self.SendUpArriveMsg()

        elif RegisterAddr == MB_ADDR_U2_TIME_MM:
            msg = self.UpTimeMsg
            msg.srcAddr = requestMsg.srcAddr

            msg.nextArriveMinute = self.decodeWord(ModbusMsgData)
            logI("Receive %s, Minute: %d"%(ModbusMsgType[RegisterAddr], msg.arriveMinute))
            self.SendUpArriveMsg()

        else:
            logW("ModbusServer write, not supported RegisterAddr : %04x" % RegisterAddr)

    # 设置响应消息
    def GetModbusResponseMsg(self, msg, error = False):
        if error:
            FunctionCode = msg.FunctionCode | 0x80
            length = 3
        else:
            FunctionCode = msg.FunctionCode
            length = 0

        retMsg = CModbusMsg()
        if error or msg.FunctionCode == 0x03:
            if not error:
                length = msg.msgDataLen + 3

            # self.setBuf(bytearray(length + 6))
            retMsg.encodeWord(msg.TransactionID)
            retMsg.encodeWord(msg.ProtocolID)
            retMsg.encodeWord(length)
            retMsg.encodeByte(msg.UnitID)
            retMsg.encodeByte(FunctionCode)
            if not error:
                retMsg.encodeByte(1)
                retMsg.encodeWord(self.GetControllerStatus(msg.RegisterAddr))
                # retMsg.encodeByte(self.modbusMsgDataLen)
                # retMsg.writeData(self.modbusMsgData)
            else:
                retMsg.encodeByte(0x01)

        elif msg.FunctionCode == 0x10:
            if not error:
                length = 6

            # retMsg.setBuf(bytearray(length + 6))
            retMsg.encodeWord(msg.TransactionID)
            retMsg.encodeWord(msg.ProtocolID)
            retMsg.encodeWord(length)
            retMsg.encodeByte(msg.UnitID)
            retMsg.encodeByte(FunctionCode)
            if not error:
                retMsg.encodeWord(msg.RegisterAddr)
                retMsg.encodeWord(0)
                # retMsg.encodeWord(self.modbusMsgDataLen)
            else:
                retMsg.encodeByte(0x01)
        else:
            return None

        # retMsg.ack = True
        # retMsg._length = length
        # retMsg.FunctionCode = FunctionCode

        # self.packet = retMsg.getMsgBuffer()
        retMsg.MsgListID    = msg.MsgListID
        retMsg.MsgLogID     = msg.MsgLogID

        return retMsg

    # 发送下行到达消息
    def SendDownArriveMsg(self):
        downCtrl = self.downCtrl
        if not downCtrl:
            logE("Error: Send Down Arriving msg but not get controller")
            return

        msg = self.DownTimeMsg
        # 消息还没有收全
        if msg.isMsgComplete() == False:
            return

        stationTime = {
            "EndStationID"      : msg.stationID,
            "ArriveTimeH"       : msg.arriveHour,
            "ArriveTimeM"       : msg.arriveMinute, 
            "NextEndStationID"  : msg.nextStationID,
            "NextArriveTimeH"   : msg.nextArriveHour,
            "NextArriveTimeM"   : msg.nextArriveMinute,
        }

        logI(msg.toString())

        msg.encode()
        self.LogMsg(msg)

        msgInfo = {
            "stationTime" : stationTime,
            "id" : msg.MsgListID,
        }

        self.msgServer.sendStationTime([downCtrl], msgInfo)

        self.DownTimeMsg.InitMsg()

        return


    # 发送上行到达消息
    def SendUpArriveMsg(self):
        upCtrl = self.upCtrl
        if not upCtrl:
            logE("Error: No UP Controller")
            return

        msg = self.UpTimeMsg

        # 消息还没有收全
        if msg.isMsgComplete() == False:
            logI("UP Msg not complete")
            return

        stationTime = {
            "EndStationID"      : msg.stationID,
            "ArriveTimeH"       : msg.arriveHour,
            "ArriveTimeM"       : msg.arriveMinute, 
            "NextEndStationID"  : msg.nextStationID,
            "NextArriveTimeH"   : msg.nextArriveHour,
            "NextArriveTimeM"   : msg.nextArriveMinute,
        }

        logI(msg.toString())

        msg.encode()
        self.LogMsg(msg)
        msgInfo = {
            "stationTime" : stationTime,
            "id" : msg.MsgListID,
        }

        self.msgServer.sendStationTime([upCtrl], msgInfo)

        self.UpTimeMsg.InitMsg()

        return


    # 向控制器发送紧急消息
    # 中心服务器也可以接收综合监控的紧急消息
    def SendEmergenceMsg(self, clean = False):

        msg = self.EmergenceMsg 
        if msg.isMsgComplete() == False:
            return

        if not clean:
            msgInfo = {
                "content"       : msg.msgContent,
                "lifeTime"      : 0,
                "id"            : None,
                "operation_id1" : msg.emergenceMode,
                "data"          : [msg.emergenceMode,],
            }
        else:
            msgInfo = {
                "content"       : "",
                "id"            : None,
                "operation_id1" : msg.emergenceMode,
                "data"          : [msg.emergenceMode,],
            }

        # logI("Send %s to %s"%(msg.toString())
        msg.encode()
        self.LogMsg(msg)
        msgInfo['id'] = msg.MsgListID

        if self.isStation == False:
            # 中心服务器时，控制器列表为空，表示向所有控制器发送该消息
            # 获取当前运营线路的所有控制器
            self.msgServer.sendEmergence([], msgInfo)

        else:
            # 车站服务器
            if self.hallCtrl and ( msg.selectedCtrl == MB_ADDR_CTRL_HC or msg.selectedCtrl == MB_ADDR_MSG_SEL_ALL):
                self.msgServer.sendEmergence([self.hallCtrl], msgInfo)

            if self.upCtrl and ( msg.selectedCtrl == MB_ADDR_CTRL_UC or msg.selectedCtrl == MB_ADDR_MSG_SEL_ALL):
                self.msgServer.sendEmergence([self.upCtrl], msgInfo)

            if self.downCtrl and ( msg.selectedCtrl == MB_ADDR_CTRL_DC or msg.selectedCtrl == MB_ADDR_MSG_SEL_ALL):
                self.msgServer.sendEmergence([self.downCtrl], msgInfo)

        self.EmergenceMsg = CModbusEmergenceMsg()

    # 向综合监控发送消息
    def SendModbusMessage(self, clientSocket, clientAddr, retMsg):
        msgData = retMsg.getMsgBuffer()
        clientSocket.sendMessage( msgData, clientAddr )

        retMsg.isSendMsg    = True
        retMsg.isAckMsg     = True

        self.LogMsg(retMsg)

    # 将接收或发送的Modbus消息记录消息日志
    # def LogMsg(self, msg):
    def LogMsg(self, msg):

        dbHelper = DbHelper()

        ntime = datetime.datetime.now()
        cdate = ntime.strftime('%Y-%m-%d')
        ctime = ntime.strftime('%H:%M:%S')

        # 因为消息内容不同，所以接收请求消息或发送响应消息都需要单独建立一条记录

        ATSInfo = {
            "id"            : 0,
            "install_type"  : '8',
            "name"          : "综合监控",
            "code"          : "AAAAAA",
            "ip_address"    : msg.srcAddr,
        }

        if msg.isAckMsg == False:
            if msg.msgType == ModbusMsg:
                msg.msgType = msg.RegisterAddr | 0x100

            senderInfo  = ATSInfo
            receiveInfo = self.stationInfo
        else:
            senderInfo  = self.stationInfo
            receiveInfo = ATSInfo

        # 先在message_list表中新增一条记录
        data = {
            'create_time'   : datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
            'create_id'     : 0,
            'message_type'  : msg.msgType,
            'operation_id1' : None,
            'operation_id2' : None,
            'content'       : msg.toString(),
            'sender_id'     : senderInfo['id'],
            'sender_name'   : senderInfo['name'],
            'send_date'     : cdate,
            'send_time'     : ctime,
            'send_status'   : 3,                                # 下发成功，表示服务器已经接收该消息
        }

        db = dbHelper.getDB('pis.message_list')
        # logI("Modbus 消息：", data)
        msg.MsgListID = db.add(data)

        # 在message_log表中新增一条记录

        data = {
            "sender_id"     : senderInfo["id"],
            "sender_type"   : senderInfo["install_type"],
            "sender_name"   : senderInfo["name"],
            "sender_code"   : senderInfo["code"],
            "sender_addr"   : senderInfo["ip_address"],

            "receiver_id"   : receiveInfo["id"],
            "receiver_type" : receiveInfo["install_type"],
            "receiver_name" : receiveInfo["name"],
            "receiver_code" : receiveInfo["code"],
            "receiver_addr" : receiveInfo["ip_address"],

            "message_type"  : msg.msgType,
            "msg_list_id"   : msg.MsgListID,
            "message_sn"    : 1,                                # 默认为1
            "operation_id1" : 0,
            "operation_id2" : 0,
            "send_date"     : cdate,
            "send_time"     : ctime,
            "receive_date"  : None,
            "receive_time"  : None,
            "content_type"  : "0",
            "bin_content"   : ConvertToHex(msg.getMsgBuffer()),
            "text_content"  : msg.toString(),
            "create_time"   : cdate + " " + ctime,
            "send_status"   : 1,                                # 1: 发送成功，表示服务器已经接收该消息                  
        }

        db = dbHelper.getDB("pis.message_log")
        msg.MsgLogID = db.add(data)
        dbHelper.closeDB()


    def decodeByte(self, data):
        if len(data) < 1:
            raise Exception("decodeByte too short")
        return int(data[0])

    def decodeWord(self, data):
        if len(data) < 2:
            raise Exception("decodeWord too short")

        return int((data[0] << 8) + data[1])

    def swapWord(self, data):
        dlen = len(data)
        rdata = bytearray(len(data))
        i = 0
        while i + 2 <= dlen:
            rdata[i] = data[i + 1]
            rdata[i + 1] = data[i]
            i += 2
        if i < dlen:
            rdata[i] = data[i]
        return rdata

    def decodeString(self, data):
        try:
            # logI("紧急消息：%s"%ConvertToHex(data))
            return data.decode("utf-8")
            # return s
        except Exception as e:
            onException(e)

        try:
            return  data.decode("gbk")
            # return s
        except Exception as e:
            onException(e)

        return "Invalid emergency message"

