/*------------------------------------------------------------------------------
 * RSU - Component :: Baidu Message Protocol Specification
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    BaiduMessage.h
 * Purpose: RSU module, Baidu Message Specification definition
 *----------------------------------------------------------------------------*/
#ifndef _BAIDU_MESSAGE_H_
#define _BAIDU_MESSAGE_H_

#include "Codec.h"

#include "CSAEMessageSet.h"

// 百度消息定义，用于RSU和边缘服务器之间

#define BAIDU_FRAME_HEAD                0xFAFB
#define BAIDU_FRAME_END                 0xECED

#define BAIDU_CURRENT_VERSION           0x0101
#define BAIDU_MSG_BSM                   0x2201
#define BAIDU_MSG_RSM                   0x2202
#define BAIDU_MSG_MAP                   0x0203
#define BAIDU_MSG_SPAT                  0x0204
#define BAIDU_MSG_RSI                   0x0205

#define BAIDU_MSG_OTHER                 0x0206                          // 百度私有消息

// 百度消息帧结构
// 序号  构成      字段名称    名称代码        字节数 数据类型    说明
// 1     帧开始    帧开始      FrameStart      2       Byte       帧开始为FAFB
// 2     协议版本  当前版本    CurrentVersion  2       Byte       当前协议版本号，组成x.y形式的版本号，当前版本号为1.1，即0x1，0x1
// 3     数据表    帧类型      FrameType       2       Byte       帧类型,
//                                                                 Bsm消息: 0x22, 0x01
//                                                                 RSM消息：
//                                                                 0x22, 0x2
//                                                                 MAP消息:
//                                                                 0x22, 0x3
//                                                                 SPAT消息：
//                                                                 0x22, 0x4
//                                                                 RSI 消息：
//                                                                 0x22, 0x5
//                                                                 私有消息填，0x22, 0x06.
//                                                                 *添加网络层头时，国标消息AID使用17，私有消息AID使用20.
//                 数据长度    Length          2       Byte        表明下个字段Data的长度
//                 数据        Data            变长    Byte        消息内容
// 4   校验码      校验码      CheckSum        1       Byte        帧结束之前，应有校验码，其值为协议版本和数据表部分（第2和第3部分）所有字节按位异或 
// 5   帧结束      帧结束      FrameEnd        2       Byte        帧结束为ECED 

class CBaiduMessage : public CCodec
{
    public:
        CBaiduMessage();

    public:
        bool                Encode();
        bool                Decode(PUCHAR pData, USHORT nLen);

        CMainMsgFrame*      GetMainMsgFrame() { return &m_MainMsgFrame; }

        virtual const char* toString() { return "BaiduMessage"; }

    protected:
        bool                CalculateCheckSum(UCHAR &nCheckSum);

        USHORT              m_nFrameHead;
        USHORT              m_nCurrentVersion;
        USHORT              m_nFrameType;
        USHORT              m_nDataLen;
        PUCHAR              m_pData;                                    // 为编码后的CSAE消息体
        UCHAR               m_nCheckSum;
        USHORT              m_nFrameEnd;

        CMainMsgFrame       m_MainMsgFrame;
};

#endif // _BAIDU_MESSAGE_H_
