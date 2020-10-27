/*------------------------------------------------------------------------------
 * JCCL - Component :: Base Message
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    MsgBase.cpp
 * Purpose: JCCL module base message definition
 *----------------------------------------------------------------------------*/
#include "stdafx.h"

#include "Common.h"
#include "Debug.h"
#include "MsgBase.h"

// # 消息基类，完成消息头部及基本的编解码功能
// # 
// # 消息定义：
// #   headFlag: 消息开始标志，4字节
// #   msgType ：消息ID，4字节
// #   srcAddr ：消息源IP地址，字符串
// #   srcPort : 消息源端口，4字节，此参数未使用
// #   destAddr: 消息目的地IP地址，字符串
// #   destPort: 消息目的地端口，4字节
// #   serialNo：消息序号，指本消息的重发次数，从0开始，4字节，未使用
// #   macAddr ：发送模块的MAC地址，字符串，通过mac地址或模块编码识别客户端，客户端地址可能不固定，此参数未使用
// #   srcModuleCode: 消息源模块编号，字符串
// #   srcModuleType: 消息源模块类型， 4字节
// #   destModuleCode: 消息目的地模块编号，字符串
// #   destModuleType: 消息目的地模块类型， 4字节
// #   msgLeng：消息体数据长度，4字节
// #   消息体
// #   endFlag：消息结束标志，4字节
// #   
// #   字符串编码方式：首先加入字符串长度，然后再是字符串本身，在解码时首先取得字符串长度，再根据长度取得字符串内容
// #   

////////////////////////////////////////////////////////////////////////
// CMsgBase

CMsgBase::CMsgBase()
{
    m_nMsgType          = INVALID_MSG;

    m_nHeadFlag         = MSG_HEAD_FLAG;
    m_nEndFlag          = MSG_END_FLAG;

    m_LocalAddr         = "";
    m_SrcAddr           = "";
    m_DestAddr          = "";
    m_nSrcPort          = INVALID_SOCKET_PORT;
    m_nDestPort         = INVALID_SOCKET_PORT;

    m_nSerialNo         = 0;
    m_strMacAddr        = "";

    m_SrcModuleCode     = "";
    m_nSrcModuleType    = INVALID_MODULE;
    m_DestModuleCode    = "";
    m_nDestModuleType   = INVALID_MODULE;
}

CMsgBase::~CMsgBase()
{
}

///////////////////////////////////////////////////////////////////////////////
// 消息编码
bool CMsgBase::Encode(UINT& nMsgLen)
{
    m_nMsgLength    = 0;
    m_nPos          = 0;
    m_pBuf          = m_Buffer;

    LogDI("Encode %s", toString() );

    if( EncodeUINT( m_nHeadFlag ) == false)
        return false;

    if( EncodeUINT( m_nMsgType ) == false)
        return false;

    if( EncodeString( m_SrcAddr ) == false)
        return false;

    if( EncodeUINT( m_nSrcPort ) == false)
        return false;

    if( EncodeString( m_SrcModuleCode ) == false)
        return false;
    
    if( EncodeUINT( m_nSrcModuleType ) == false)
        return false;

    if( EncodeString( m_strMacAddr ) == false)
        return false;
        
    if( EncodeString( m_DestAddr ) == false)
        return false;

    if( EncodeUINT( m_nDestPort ) == false)
        return false;

    if( EncodeString( m_DestModuleCode ) == false)
        return false;
    
    if( EncodeUINT( m_nDestModuleType ) == false)
        return false;

    if( EncodeUINT( m_nSerialNo ) == false)
        return false;

    // 消息体及消息尾部标识编码由消息完成

    nMsgLen     = m_nMsgLength;
    return true;
}


/////////////////////////////////////////////////////////////////////////
// 消息解码
bool CMsgBase::Decode(PUCHAR pBuffer, UINT nMsgLen)
{
    m_pBuf          = pBuffer;
    m_nMsgLength    = nMsgLen;
    m_nPos          = 0;

    LogDI("Data: %s", ConvertToHex(pBuffer, nMsgLen));

    if ( DecodeUINT(m_nHeadFlag) == false)
        return false;

    if ( m_nHeadFlag != MSG_HEAD_FLAG )
        return false;

    if ( DecodeUINT(m_nMsgType) == false)
        return false;

    if ( DecodeString(m_SrcAddr) == false)
        return false;

    if ( DecodeUINT(m_nSrcPort) == false)
        return false;

    if (DecodeString( m_SrcModuleCode ) == false)
        return false;

    if ( DecodeUINT( m_nSrcModuleType ) == false)
        return false;

    if( DecodeString( m_strMacAddr ) == false)
        return false;
    
    if ( DecodeString(m_DestAddr) == false)
        return false;

    if ( DecodeUINT(m_nDestPort) == false)
        return false;

    if (DecodeString( m_DestModuleCode ) == false)
        return false;

    if ( DecodeUINT( m_nDestModuleType ) == false)
        return false;

    if ( DecodeUINT( m_nSerialNo ) == false)
        return false;

    // 消息体及尾部标识的解码由消息完成
    LogD("Decoding %s", toString());

    return true;
}


// 只解码消息头部，获得消息类型
UINT CMsgBase::DecodeMsgType(PUCHAR pBuffer, UINT nMsgLen)
{
    if (CMsgBase::Decode(pBuffer, nMsgLen) == false)
        return INVALID_MSG;

    return m_nMsgType;
}

// 无参数应答消息编码
// 有参数应答消息由各自的编码函数进行编码
// 解码方式相同
bool CMsgAckBase::Encode(UINT &nMsgLen)
{
    if (CMsgBase::Encode(nMsgLen) == false)
        return false;

    if (EncodeUINT(m_nAckType) == false)
        return false;

    if (EncodeUINT(m_nStatus) == false)
        return false;

    if (EncodeUINT(m_nEndFlag) == false)
        return false;

    nMsgLen = m_nMsgLength;
    LogDI("%s: MsgLen: %d, Msg: %s", toString(), nMsgLen, ConvertToHex(m_Buffer, nMsgLen));

    return true;
}

bool CMsgAckBase::Decode(PUCHAR pBuffer, UINT nMsgLen)
{
    if (CMsgBase::Decode(pBuffer, nMsgLen) == false)
        return false;

    if (DecodeUINT(m_nAckType) == false)
        return false;

    if (DecodeUINT(m_nStatus) == false)
        return false;

    if (DecodeUINT(m_nEndFlag) == false)
        return false;

    if (m_nEndFlag != MSG_END_FLAG )
    {
        LogE("Message end flag %x", m_nEndFlag);
        return false;
    }

    return true;
}

bool CHeartBeatMsg::Encode(UINT &nMsgLen)
{
    if (CMsgBase::Encode(nMsgLen) == false)
        return false;

    if (EncodeUINT(m_nEndFlag) == false)
        return false;

    nMsgLen = m_nMsgLength;
    LogDI("%s: MsgLen: %d, Msg: \n%s", toString(), nMsgLen, ConvertToHex(m_Buffer, nMsgLen));

    return true;
}

bool CHeartBeatMsg::Decode(PUCHAR pBuffer, UINT nMsgLen)
{
    LogDI("%s: MsgLen: %d, Msg: \n%s", toString(), nMsgLen, ConvertToHex(pBuffer, nMsgLen));

    if (CMsgBase::Decode(pBuffer, nMsgLen) == false)
    {
        LogE("Decode HeartbeatAckMsg falied.");
        return false;
    }

    if (DecodeUINT(m_nEndFlag) == false)
        return false;

    if (m_nEndFlag != MSG_END_FLAG )
    {
        LogE("Message end flag %x", m_nEndFlag);
        return false;
    }

    return true;
}

bool CHeartBeatAckMsg::Encode(UINT &nMsgLen)
{
    if (CMsgBase::Encode(nMsgLen) == false)
        return false;

    if (EncodeUINT(m_nEndFlag) == false)
        return false;

    nMsgLen = m_nMsgLength;
    LogDI("%s: MsgLen: %d, Msg: \n%s", toString(), nMsgLen, ConvertToHex(m_Buffer, nMsgLen));

    return true;
}

bool CHeartBeatAckMsg::Decode(PUCHAR pBuffer, UINT nMsgLen)
{
    LogDI("%s: MsgLen: %d, Msg: \n%s", toString(), nMsgLen, ConvertToHex(pBuffer, nMsgLen));

    if (CMsgBase::Decode(pBuffer, nMsgLen) == false)
        return false;

    if (DecodeUINT(m_nEndFlag) == false)
        return false;

    if (m_nEndFlag != MSG_END_FLAG )
    {
        LogE("Message end flag %x", m_nEndFlag);
        return false;
    }

    return true;
}


CNoShowMsgList g_NoShowMsgList;

CNoShowMsgList::CNoShowMsgList()
{
    m_nMsgCount = 0;
    for(int i = 0; i < MAX_NO_SHOW_MSG_COUNT; i++)
        m_MsgList[i] = INVALID_MSG;

    InsertNoShowMsgType(MSG_HEARTBEAT);
    InsertNoShowMsgType(MSG_HEARTBEAT_ACK);
}

void CNoShowMsgList::InsertNoShowMsgType(UINT nMsgType)
{
    if ( m_nMsgCount >= MAX_NO_SHOW_MSG_COUNT )
        return;

    if ( IsMsgInNoShowMsgList(nMsgType) == true )
        return;

    m_MsgList[m_nMsgCount] = nMsgType;
    m_nMsgCount ++;
}

bool CNoShowMsgList::IsMsgInNoShowMsgList(UINT nMsgType)
{
    for(int i = 0; i < m_nMsgCount; i++)
    {
        if (m_MsgList[i] == nMsgType )
            return true;
    }

    return false;
}

