/*------------------------------------------------------------------------------
 * JCCL - Component :: Message Codec
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    Codec.cpp
 * Purpose: JCCL module message codec definition
 *----------------------------------------------------------------------------*/
#include "stdafx.h"

#include "Common.h"
#include "Debug.h"
#include "Codec.h"

CCodec::CCodec()
{
    m_nPos          = 0;
    m_nMsgLength    = 0;
    m_pBuf          = m_Buffer;
}

// 对无符号整数进行编码
bool CCodec::EncodeUINT(UINT unType, bool bSwap )
{
    LogDI("Encode UINT: %X, Pos: %d", unType, m_nMsgLength);

    // 首先判断消息长度是否合法
    if ( (m_nMsgLength + sizeof(UINT) ) > MAX_MSG_LEN )
    {
        LogE("Encode UINT length exceed max limitation");
        return false;
    }

    // 从主机字节顺序转为为网络字节顺序
    unType = (bSwap) ? htonl(unType):unType;

    memcpy( (void*)(m_pBuf + m_nMsgLength ), &unType, sizeof(UINT));
    m_nMsgLength += sizeof(UINT);

    return true;
}

// 对无符号整数解码
bool CCodec::DecodeUINT(UINT &unType)
{
    if ( m_nPos + sizeof(UINT) > m_nMsgLength )
    {
        LogE("Decode UINT error, Pos: %d, MsgLen: %d", m_nPos, m_nMsgLength);
        return false;
    }

    memcpy( (void*)&unType, (void*)(m_pBuf + m_nPos), sizeof(UINT) );
    m_nPos += sizeof(UINT);

    // 将网络字节顺序转换为主机顺序
    unType = ntohl(unType);

    LogDI("Decode UINT: %d, Pos: %d, MsgLen: %d", unType, m_nPos, m_nMsgLength);

    return true;
}

// 对有符号整数进行编码
bool CCodec::EncodeInt(int nType, bool bSwap )
{
    LogDI("Encode int: %X, Pos: %d", nType, m_nMsgLength);

    // 首先判断消息长度是否合法
    if ( (m_nMsgLength + sizeof(int) ) > MAX_MSG_LEN )
    {
        LogE("Encode UINT length exceed max limitation");
        return false;
    }

    // 从主机字节顺序转为为网络字节顺序
    nType = (bSwap) ? htonl(nType):nType;

    memcpy( (void*)(m_pBuf + m_nMsgLength ), &nType, sizeof(int));
    m_nMsgLength += sizeof(int);

    return true;
}

// 对有符号整数解码
bool CCodec::DecodeInt(int &nType)
{
    if ( m_nPos + sizeof(int) > m_nMsgLength )
    {
        LogE("Decode int error, Pos: %d, MsgLen: %d", m_nPos, m_nMsgLength);
        return false;
    }

    memcpy( (void*)&nType, (void*)(m_pBuf + m_nPos), sizeof(int) );
    m_nPos += sizeof(int);

    // 将网络字节顺序转换为主机顺序
    nType = ntohl(nType);

    LogDI("Decode int: %d, Pos: %d, MsgLen: %d", nType, m_nPos, m_nMsgLength);

    return true;
}

// 对无符号长整数进行编码
bool CCodec::EncodeULongLong(ULONGLONG unType)
{
    LogDI("Encode ULONGLONG: %llu, Pos: %d", unType, m_nMsgLength);

    // 首先判断消息长度是否合法
    if ( (m_nMsgLength + sizeof(ULONGLONG) ) > MAX_MSG_LEN )
    {
        LogE("Encode ULONGLONG length exceed max limitation");
        return false;
    }

    // 从主机字节顺序转为为网络字节顺序
    U_ULongLong ullValue1, ullValue2;
    ullValue1.ullValue = unType;

    ullValue2.stullValue.val1 = htonl(ullValue1.stullValue.val1);
    ullValue2.stullValue.val2 = htonl(ullValue1.stullValue.val2);

    memcpy( (void*)(m_pBuf + m_nMsgLength ), &ullValue2.ullValue, sizeof(ULONGLONG));
    m_nMsgLength += sizeof(ULONGLONG);

    return true;
}

// 对无符号长整数解码
bool CCodec::DecodeULongLong(ULONGLONG &unType)
{
    if ( m_nPos + sizeof(ULONGLONG) > m_nMsgLength )
    {
        LogE("Decode ULONGLONG error, Pos: %d, MsgLen: %d", m_nPos, m_nMsgLength);
        return false;
    }

    U_ULongLong ullValue1;
    memcpy( (void*)&ullValue1.ullValue, (void*)(m_pBuf + m_nPos), sizeof(ULONGLONG) );
    m_nPos += sizeof(ULONGLONG);

    // 将网络字节顺序转换为主机顺序
    U_ULongLong ullValue2;
    ullValue2.stullValue.val1 = ntohl(ullValue1.stullValue.val1);
    ullValue2.stullValue.val2 = ntohl(ullValue1.stullValue.val2);
    unType = ullValue2.ullValue;

    LogDI("Decode ULONGLONG: %llu, Pos: %d, MsgLen: %d", unType, m_nPos, m_nMsgLength);

    return true;
}


// 对字符串进行编码
bool CCodec::EncodeString(CString strType)
{
    UINT nStrLen = strType.GetLength();

    LogDI("Encode String: %s, Len: %d, Pos: %d", strType.GetBuffer(), nStrLen, m_nMsgLength);

    // 首先判断消息长度是否合法
    if ( (m_nMsgLength + nStrLen + sizeof(int) ) > MAX_MSG_LEN )
    {
        LogE("Encode String length exceed max limitation");
        return false;
    }

    if ( EncodeUINT(nStrLen) == false )
    {
        LogE("Encode String %s length falied.", strType.GetBuffer() );
    }

    memcpy( (void*)(m_pBuf + m_nMsgLength), strType.GetBuffer(), nStrLen );
    m_nMsgLength += nStrLen;

    return true;
}

// 解码字符串
bool CCodec::DecodeString(CString &stType)
{
    UINT unStrLen = 0;
    if (DecodeUINT(unStrLen) == false)
        return false;

    if ( m_nPos + unStrLen > m_nMsgLength )
    {
        LogE("Decode String error, Pos: %d, StrLen: %d, MsgLen: %d", m_nPos, unStrLen, m_nMsgLength);
        return false;
    }

    if ( unStrLen == 0 )
    {
        LogDI("Decode String: , Pos: %d, MsgLen: %d", m_nPos, m_nMsgLength);
        return true;
    }

    if ( unStrLen > MAX_STR_LEN )
    {
        LogE("Decode string len %d exceed max limit(%d)", unStrLen, MAX_STR_LEN);
        return false;
    }

    char pTemp[MAX_STR_LEN];

    memcpy( pTemp, (void*)(m_pBuf + m_nPos), unStrLen );
    pTemp[unStrLen] = '\0';
    stType = pTemp;

    m_nPos += unStrLen;
    LogDI("Decode String: %s, Pos: %d, MsgLen: %d", pTemp, m_nPos, m_nMsgLength);

    return true;
}

#ifdef QT_CORE_LIB
bool CCodec::EncodeString(QString strType)
{  
    char temp[MAX_MSG_LEN];

    QByteArray qStr = strType.toLatin1();
    strcpy(temp, qStr.data());

    LogDI("Encode String: %s, Len: %d, Pos: %d", temp, strlen(temp), m_nMsgLength);

    // 首先判断消息长度是否合法
    int nLen = strlen(temp);
    if ( (m_nMsgLength + nLen + sizeof(int) ) > MAX_MSG_LEN )
    {
        LogDI("String length is too long.");
        return false;
    }

    memcpy( (void*)(m_pBuf + m_nMsgLength ), &nLen, sizeof(int));
    m_nMsgLength += sizeof(int);

    memcpy( (void*)(m_pBuf + m_nMsgLength), temp, nLen );
    m_nMsgLength += nLen;

    return true;
}

bool CCodec::DecodeString(QString& strType)
{
    UINT unStrLen = 0;
    if (DecodeUINT(unStrLen) == false)
        return false;

    if ( m_nPos + unStrLen > m_nMsgLength )
    {
        LogE("Decode String error, Pos: %d, StrLen: %d, MsgLen: %d", m_nPos, unStrLen, m_nMsgLength);
        return false;
    }

    if ( unStrLen == 0 )
    {
        LogDI("Decode String: , Pos: %d, MsgLen: %d", m_nPos, m_nMsgLength);
        return true;
    }

    if ( unStrLen > MAX_MSG_LEN )
    {
        LogE("Decode QString len %d exceed max limit(%d)", unStrLen, MAX_MSG_LEN);
        return false;
    }

    char pTemp[MAX_MSG_LEN];

    memcpy( pTemp, (void*)(m_pBuf + m_nPos), unStrLen );
    pTemp[unStrLen] = '\0';
    strType = pTemp;

    m_nPos += unStrLen;
    LogDI("Decode String: %s, Pos: %d, MsgLen: %d", pTemp, m_nPos, m_nMsgLength);

    return true;
}
#endif


bool CCodec::EncodeUSHORT(USHORT usType)
{
    LogDI("Encode USHORT: %d, Pos: %d", usType, m_nMsgLength);

    // 首先判断消息长度是否合法
    if ( (m_nMsgLength + sizeof(USHORT) ) > MAX_MSG_LEN )
        return false;

    // 主机字节顺序转换为网络字节顺序
    USHORT temp = htons(usType);
    memcpy( (void*)(m_pBuf + m_nMsgLength ), &temp, sizeof(USHORT));
    m_nMsgLength += sizeof(USHORT);

    return true;
}

bool CCodec::DecodeUSHORT(USHORT &usType)
{
    if ( m_nPos + sizeof(USHORT) > m_nMsgLength )
    {
        LogE("Decode USHORT exceed message data length: Pos: %d, MsgLen: %d", m_nPos, m_nMsgLength);
        return false;
    }

    USHORT temp;
    memcpy( (void*)&temp, (void*)(m_pBuf + m_nPos), sizeof(USHORT) );
    m_nPos += sizeof(USHORT);

    // 网络字节顺序转换为主机字节顺序
    usType = ntohs(temp);
    
    LogDI("Decode USHORT: %d, Pos: %d, MsgLen: %d", usType, m_nPos, m_nMsgLength);

    return true;
}

bool CCodec::EncodeBYTE(BYTE btType)
{
    LogDI("Encode BYTE: %d, Pos: %d", btType, m_nMsgLength);

    // 首先判断消息长度是否合法
    if ( (m_nMsgLength + sizeof(BYTE) ) > MAX_MSG_LEN )
        return false;

    memcpy( (void*)(m_pBuf + m_nMsgLength ), &btType, sizeof(BYTE));
    m_nMsgLength += sizeof(BYTE);

    return true;
}

bool CCodec::DecodeBYTE(BYTE &btType)
{
    if ( m_nPos + sizeof(BYTE) > m_nMsgLength )
    {
        LogE("Decode BYTE exceed message data length: Pos: %d, MsgLen: %d", m_nPos, m_nMsgLength);
        return false;
    }

    memcpy( (void*)&btType, (void*)(m_pBuf+m_nPos), sizeof(BYTE) );
    m_nPos += sizeof(BYTE);

    LogDI("Decode BYTE: %d, Pos: %d, MsgLen: %d", btType, m_nPos, m_nMsgLength);
    return true;
}

bool CCodec::EncodeBool(bool bType)
{
    LogDI("Encode BOOL: %s, Pos: %d", (bType) ? "True" : "False", m_nMsgLength);

    // 首先判断消息长度是否合法
    if ( (m_nMsgLength + sizeof(bool) ) > MAX_MSG_LEN )
        return false;

    memcpy( (void*)(m_pBuf + m_nMsgLength ), &bType, sizeof(bool));
    m_nMsgLength += sizeof(bool);

    return true;
}

bool CCodec::DecodeBool(bool &bType)
{
    if ( m_nPos + sizeof(bool) > m_nMsgLength )
    {
        LogE("Decode BOOL exceed message data length: Pos: %d, MsgLen: %d", m_nPos, m_nMsgLength);
        return false;
    }

    memcpy( (void*)&bType, (void*)(m_pBuf + m_nPos), sizeof(bool) );
    m_nPos += sizeof(bool);

    LogDI("Decode BOOL: %s, Pos: %d, MsgLen: %d", bType ? "True" : "False", m_nPos, m_nMsgLength);
    return true;
}

bool CCodec::EncodeUCHAR(UCHAR cType)
{
    LogDI("Encode UCHAR: %d, Pos: %d", cType, m_nMsgLength);

    // 首先判断消息长度是否合法
    if ( (m_nMsgLength + sizeof(UCHAR) ) > MAX_MSG_LEN )
        return false;

    memcpy( (void*)(m_pBuf + m_nMsgLength ), &cType, sizeof(UCHAR));
    m_nMsgLength += sizeof(UCHAR);

    return true;
}

bool CCodec::DecodeUCHAR(UCHAR &cType)
{
    if ( m_nPos + sizeof(UCHAR) > m_nMsgLength )
    {
        LogE("Decode UCHAR exceed message data length: Pos: %d, MsgLen: %d", m_nPos, m_nMsgLength);
        return false;
    }

    memcpy( (void*)&cType, (void*)(m_pBuf + m_nPos), sizeof(UCHAR) );
    m_nPos += sizeof(UCHAR);

    LogDI("Decode UCHAR: %d, Pos: %d, MsgLen: %d", cType, m_nPos, m_nMsgLength);
    return true;
}

bool CCodec::EncodePUCHAR(PUCHAR pData, USHORT nLen)
{
    LogDI("Encode PUCHAR: Len: %d, Pos: %d", nLen, m_nMsgLength);

    // 首先判断消息长度是否合法
    if ( (m_nMsgLength + nLen ) > MAX_MSG_LEN )
        return false;

    memcpy( (void*)(m_pBuf + m_nMsgLength ), pData, nLen);
    m_nMsgLength += nLen;

    return true;
}

bool CCodec::DecodePUCHAR(PUCHAR pData, USHORT nLen)
{
    if ( m_nPos + nLen > m_nMsgLength )
    {
        LogE("Decode PUCHAR exceed message data length: Pos: %d, Len: %d, MsgLen: %d", m_nPos, nLen, m_nMsgLength);
        return false;
    }

    memcpy( (void*)pData, (void*)(m_pBuf + m_nPos), nLen );
    m_nPos += nLen;

    LogDI("Decode PUCHAR: Pos: %d, nLen: %d, MsgLen: %d", m_nPos, nLen, m_nMsgLength);
    return true;
}

