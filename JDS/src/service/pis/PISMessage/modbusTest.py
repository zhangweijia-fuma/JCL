#!/usr/bin/python3
import socket
import time, datetime
from sys import argv
from array import array
from struct import unpack, pack
from math import ceil
from argparse import ArgumentParser
from random import randint

__all__ = ["client"]


# 通过Modbus向PIS中心/车站服务器发送消息，模拟到站信息、紧急通知消息等
# 参数：
# 
def Help():
    
    print("""
    Usage: modbusTest.py code [stationID] | [msgContent]")
    code:
        d1 X: down time msg, X: Station ID
        d2 X: down next msg, X: Station ID
        u1 X: up time msg, X: Station ID
        u1 X: up next msg, X: Station ID
        eMcCx: M, show mode, 0: scroll, 1: full screen, C: selected Controller, H:Hall, U: UP, D:Down, A:All, x:4:selected, 0:unselected 
        qC: Query Controller Status, C: selected Controller, H:Hall, U: UP, D:Down, A:All
    	stationID: destination Station ID
    msgContent: Emergence message content
    """)

    return

ntime  = datetime.datetime.now()
Hour   = int(ntime.strftime('%H'))
Minute = int(ntime.strftime('%M'))

# 下行目的站点
d1s={
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0xc9,
    "msgData"           : 0x01,
}

# 下行到站时间小时值
d1h={
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0xcd,
    "msgData"           : Hour,
}

# 下行到站时间分钟值
d1m={
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0xcc,
    "msgData"           : Minute,
}

# 下行下次列车目的站点
d2s={
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0xcf,
    "msgData"           : 0x02,
}

# 下行下次列车到站时间小时值
d2h={
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0xd3,
    "msgData"           : Hour,
}

# 下行下次列车到站时间分钟值
d2m={
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0xd2,
    "msgData"           : Minute,
}

# 上行目的站点
u1s={
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0xc8,
    "msgData"           : 0x03,
}

# 上行到站时间小时值
u1h={
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0xcb,
    "msgData"           : 0,
}

# 上行到站时间分钟值
u1m={
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0xca,
    "msgData"           : 17,
}

# 上行下次列车到站目的站点
u2s={
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0xce,
    "msgData"           : 0x04,
}

# 上行下次列车到站时间小时值
u2h={
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0xd1,
    "msgData"           : 1,
}

# 上行下次列车到站时间分钟值
u2m={
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0xd0,
    "msgData"           : 15,
}

# 紧急消息，模式：滚动
em0 = {
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0x16,
    "msgData"           : 0x00,
}

# 紧急消息模式：全屏
em1 = {
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0x16,
    "msgData"           : 0x01,
}

# 紧急消息，取消选择上行控制器
eU0 = {
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0x0c,
    "msgData"           : 0x00,
}

# 紧急消息，选择上行控制器
eU4 = {
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0x0c,
    "msgData"           : 0x04,
}

# 紧急消息，取消选择下行控制器
eD0 = {
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0x0d,
    "msgData"           : 0x00,
}

# 紧急消息，选择下行控制器
eD4 = {
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0x0d,
    "msgData"           : 0x04,
}

# 紧急消息，取消选择站厅控制器
eH0 = {
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0x0b,
    "msgData"           : 0x00,
}

# 紧急消息，选择站厅控制器
eH4 = {
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0x0b,
    "msgData"           : 0x04,
}

# 紧急消息，取消选择所有控制器
eA0 = {
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0x15,
    "msgData"           : 0x00,
}

# 紧急消息，选择所有控制器
eA4 = {
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0x15,
    "msgData"           : 0x04,
}

# 紧急消息，选择所有控制器
ec = {
    "functionCode"      : 0x10,
    "RegisterAddr"      : 0x33,
    "msgData"           : "紧急消息测试",
    # "msgData"         : len("紧急消息测试"),
}

MB_ADDR_STATUS_HC       = 0x01
MB_ADDR_STATUS_UC       = 0x02
MB_ADDR_STATUS_DC       = 0x03

# 查询站厅控制器状态
qH = {
    "functionCode"      : 0x03,
    "RegisterAddr"      : MB_ADDR_STATUS_HC,
    "msgData"           : 0,
}

qU = {
    "functionCode"      : 0x03,
    "RegisterAddr"      : MB_ADDR_STATUS_UC,
    "msgData"           : 0,
}

qD = {
    "functionCode"      : 0x03,
    "RegisterAddr"      : MB_ADDR_STATUS_DC,
    "msgData"           : 0,
}

CmdLine = {}

# 上下行
CmdLine['d1']       = [d1s, d1h, d1m]
CmdLine['d2']       = [d2s, d2h, d2m]
CmdLine['u1']       = [u1s, u1h, u1m]
CmdLine['u2']       = [u2s, u2h, u2m]

# 紧急消息通知模式
CmdLine['e0ch0']    = [em0, eH0, ec]
CmdLine['e0ch4']    = [em0, eH4, ec]
CmdLine['e0cu0']    = [em0, eU0, ec]
CmdLine['e0cu4']    = [em0, eU4, ec]
CmdLine['e0cd0']    = [em0, eD0, ec]
CmdLine['e0cd4']    = [em0, eD4, ec]
CmdLine['e0ca0']    = [em0, eA0, ec]
CmdLine['e0ca4']    = [em0, eA4, ec]

CmdLine['e1ch0']    = [em1, eH0, ec]
CmdLine['e1ch4']    = [em1, eH4, ec]
CmdLine['e1cu0']    = [em1, eU0, ec]
CmdLine['e1cu4']    = [em1, eU4, ec]
CmdLine['e1cd0']    = [em1, eD0, ec]
CmdLine['e1cd4']    = [em1, eD4, ec]
CmdLine['e1ca0']    = [em1, eA0, ec]
CmdLine['e1ca4']    = [em1, eA4, ec]

# 控制器状态查询
CmdLine['qH']       = [qH, ]
CmdLine['qU']       = [qU, ]
CmdLine['qD']       = [qD, ]
CmdLine['qA']       = [qH, qU, qD ]



class CClient:
    def __init__(self, host='192.168.1.190', unit=1):
        self.host = host
        self.unit = unit
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((host, 502))

    def read(self, FC, ADD, LEN):
        if FC not in [1, 2, 3, 4]: 
            Help()
        else:
            lADD = ADD & 0x00FF
            mADD = ADD >> 8
            lLEN = LEN & 0x00FF
            mLEN = LEN >> 8
            if (FC < 3):
                BYT = ceil(LEN / 8)  # Round off the no. of bytes
            else:
                BYT = LEN * 2
            cmd = array('B', [0, randint(0, 255), 0, 0, 0, 6,
                              self.unit, FC, mADD, lADD, mLEN, lLEN])

            print("TX : " + " ".join(["%02x" % d for d in cmd]))
            self.sock.send(cmd)

            buf = self.sock.recv(1000)
            print("RX : " + " ".join(["%02x" % d for d in buf]))

    def writeData(self, data):
        print("TX : " + " ".join(["%02x" % d for d in data]))
        self.sock.send(data)
        
        buf = self.sock.recv(1000)
        print("RX : " + " ".join(["%02x" % d for d in buf]))

    def write(self, FC, ADD, DAT):
        if FC not in [5, 6, 15, 16]: 
            Help()
        else:
            lADD = ADD & 0x00FF
            mADD = ADD >> 8
            VAL = DAT
            # for i in DAT:
            #     VAL = VAL + pack('>H', int(i))

            if FC == 5 or FC == 6:
                VAL = VAL[0:2]

            # print(DAT)
            # print(VAL)

            if FC == 5 or FC == 15:
                LEN = len(VAL) * 8
            else:
                LEN = int(len(VAL) / 2)

            LEN = 1

            lLEN = LEN & 0x00FF
            mLEN = LEN >> 8
            cmd = array('B', [0, 0, 0, 0, 0, 7 + len(VAL),
                self.unit, FC, mADD, lADD, mLEN, lLEN, len(VAL)])
            cmd.extend(VAL)

            # cmd = cmd + cmd + cmd

            print("TX : " + " ".join(["%02x" % d for d in cmd]))
            self.sock.send(cmd)

            buf = self.sock.recv(1000)
            print("RX : " + " ".join(["%02x" % d for d in buf]))

            # buf = self.sock.recv(1000)
            # print("RX : " + " ".join(["%02x" % d for d in buf]))

            # buf = self.sock.recv(1000)
            # print("RX : " + " ".join(["%02x" % d for d in buf]))


def swapWord(data):
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

def ModbusClientTest():

    argn = len(argv)
    if argn != 2 and argn != 3:
        Help()
        exit()

    code = argv[1]

    cmdLine = CmdLine.get(code)
    if cmdLine == None:
        print("error: can not find code: %s"%code)
        # Help()
        exit()

    client = CClient()

    for cmd in cmdLine:
        functionCode     = cmd['functionCode']
        RegisterAddr     = cmd['RegisterAddr']
        if (code.find('u') == 0 or code.find('d') == 0) and argn == 3:
            msgData = int(argv[2])
        elif code.find('e') == 0 and argn == 3:
            print("Code: %s" %code)
            msgData = argv[2]
        else:
            msgData     = cmd['msgData']

        if functionCode == 0x10:
            if RegisterAddr == 0x33:
                # print("紧急消息：%s"%msgData)
                data = msgData.encode()
                data = swapWord(data)
                # print("紧急消息：%s"%" ".join(["%02X" % d for d in data]))
            else:
                data = array('B', [(msgData & 0xFF00) >> 8, msgData & 0xFF])    
            client.write(functionCode, RegisterAddr, data)

        elif functionCode == 0x3:
            client.read(functionCode, RegisterAddr, msgData)

        # elif functionCode == 0x00:
        #     data = bytearray(alen - 4)
        #     i = 4
        #     j = 0
        #     while i < alen:
        #         data[j] = parseHex(argv[i])
        #         j += 1
        #         i += 1

        #     client.writeData(data)

if __name__ == "__main__":
    ModbusClientTest()


# python3 modbusTest.py 192.168.1.62 502 10 c9 00
# python3 modbusTest.py 192.168.1.62 502 10 03 01

"""

cd /JamesSoft/JDS/src/service/pis/controller/



#d1 下行 本次列车到站信息
python3 modbusTest.py 172.16.10.136 502 10 c9 01
python3 modbusTest.py 172.16.10.136 502 10 cd 11
python3 modbusTest.py 172.16.10.136 502 10 cc 21

#d2 下行 下次列车到站信息
python3 modbusTest.py 172.16.10.136 502 10 cf 02
python3 modbusTest.py 172.16.10.136 502 10 d3 12
python3 modbusTest.py 172.16.10.136 502 10 d2 22

#u1 上行 本次列车到站信息
python3 modbusTest.py 172.16.10.136 502 10 c8 03
python3 modbusTest.py 172.16.10.136 502 10 cb 13
python3 modbusTest.py 172.16.10.136 502 10 ca 23

#u2 上行 下次列车到站信息
python3 modbusTest.py 172.16.10.136 502 10 ce 04
python3 modbusTest.py 172.16.10.136 502 10 d1 14
python3 modbusTest.py 172.16.10.136 502 10 d0 24

#msg
#python3 modbusTest.py 192.168.1.62 502 00 00 a9 00 00 00 15 ff 10 00 33 00 07 0e f4 bd b1 bc a3 c4 bd ca e2 b2 d4 ca 00 00


"""
