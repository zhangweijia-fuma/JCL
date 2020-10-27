// ------------------------------------------------------------------------------
// RSU - Component :: RSU Message Base
// Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    RSUMsgBase.cpp
// Purpose: Common Module
//-------------------------------------------------------------------------------


#include "RSUMsgBase.h"

CRSUMsgBase::CRSUMsgBase()
{

}

// 无参数请求消息编码
// 有参数请求消息由各自的编码函数进行编码
// 解码方式相同
bool CRSUMsgBase::Encode(UINT &nMsgLen)
{
    if (CMsgBase::Encode(nMsgLen) == false)
        return false;

    // 消息体长度默认为0，未使用
    if (EncodeUINT(0) == false)
        return false;

    if (EncodeUINT(m_nEndFlag) == false)
        return false;

    nMsgLen = m_nMsgLength;
    LogDI("%s: MsgLen: %d, Msg: \n%s", toString(), nMsgLen, ConvertToHex(m_Buffer, nMsgLen));

    return true;
}

bool CRSUMsgBase::Decode(PUCHAR pBuffer, UINT nMsgLen)
{
    LogDI("%s: MsgLen: %d, Msg: \n%s", toString(), nMsgLen, ConvertToHex(pBuffer, nMsgLen));

    if (CMsgBase::Decode(pBuffer, nMsgLen) == false)
        return false;

    // // 消息体长度默认为0，未使用
    // UINT nLen = 0;
    // if (DecodeUINT(nLen) == false)
    //     return false;

    if (DecodeUINT(m_nEndFlag) == false)
        return false;

    if (m_nEndFlag != MSG_END_FLAG )
    {
        LogE("Message end flag %x", m_nEndFlag);
        return false;
    }

    return true;
}



CRSUMsgParam::CRSUMsgParam()
{
    m_nParamTag     = TAG_Invalid;

    m_nIntData      = 0;
    m_nUINTData     = 0;
    m_nByteData     = 0;
    m_nUSHORTData   = 0;
    m_nShortData    = 0;

    m_strData       = "";

    m_RSUParamType  = RPT_Invalid;

}

void CRSUMsgParam::SetParamData(BYTE nTag, int nData)
{
    m_nParamTag = nTag;
    m_nIntData  = nData;

    m_RSUParamType = RPT_INT;
}

void CRSUMsgParam::SetParamData(BYTE nTag, UINT nData)
{
    m_nParamTag  = nTag;
    m_nUINTData  = nData;

    m_RSUParamType = RPT_UINT;
}

void CRSUMsgParam::SetParamData(BYTE nTag, USHORT nData)
{
    m_nParamTag = nTag;
    m_nUSHORTData  = nData;

    m_RSUParamType = RPT_USHORT;
}

void CRSUMsgParam::SetParamData(BYTE nTag, short nData)
{
    m_nParamTag  = nTag;
    m_nShortData  = nData;

    m_RSUParamType = RPT_SHORT;
}

void CRSUMsgParam::SetParamData(BYTE nTag, BYTE nData)
{
    m_nParamTag = nTag;
    m_nByteData  = nData;

    m_RSUParamType = RPT_BYTE;
}

void CRSUMsgParam::SetParamData(BYTE nTag, CString strParam)
{
    m_nParamTag = nTag;
    m_strData = strParam;

    m_RSUParamType = RPT_String;
}
