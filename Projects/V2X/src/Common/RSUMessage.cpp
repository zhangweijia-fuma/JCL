/*------------------------------------------------------------------------------
 * RSU - Component :: RSU Message, Road Side Unit
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    RSUMessage.cpp
 * Purpose: RSU module, service flow realization
 *----------------------------------------------------------------------------*/

#include "RSUMessage.h"

CRSUInitialRequestMsg::CRSUInitialRequestMsg()
{
    m_nMsgType = RSU_MSG_InitialRequest;

    m_nLongtitude = m_nLatitude = 0;
}

bool CRSUInitialRequestMsg::Encode(UINT& nMsgLen)
{
    if (CMsgBase::Encode(nMsgLen) == false)
        return false;

    if (EncodeInt(m_nLongtitude) == false)
        return false;

    if (EncodeInt(m_nLatitude) == false)
        return false;

    if (EncodeUINT(m_nEndFlag) == false)
        return false;

    nMsgLen = m_nMsgLength;
    LogDI("%s: MsgLen: %d, Msg: \n%s", toString(), nMsgLen, ConvertToHex(m_Buffer, nMsgLen));

    return true;
}

bool CRSUInitialRequestMsg::Decode(PUCHAR pBuffer, UINT nMsgLen)
{
    if (CMsgBase::Decode(pBuffer, nMsgLen) == false)
        return false;

    if (DecodeInt(m_nLongtitude) == false)
        return false;

    if (DecodeInt(m_nLatitude) == false)
        return false;

    if (DecodeUINT(m_nEndFlag) == false)
        return false;

    nMsgLen = m_nMsgLength;
    LogDI("%s: MsgLen: %d, Msg: \n%s", toString(), nMsgLen, ConvertToHex(m_Buffer, nMsgLen));

    return true;
}

CRSUUpdateSoftwareMsg::CRSUUpdateSoftwareMsg()
{
    m_nMsgType      = RSU_MSG_UpdateSoftware;

    m_strFileName   = "";
    m_nUpdateMode   = Update_Immediately;
}


bool CRSUUpdateSoftwareMsg::Encode(UINT& nMsgLen)
{
    if (CMsgBase::Encode(nMsgLen) == false)
        return false;

    if (EncodeString(m_strFileName) == false)
        return false;

    if (EncodeBYTE(m_nUpdateMode) == false)
        return false;

    if (EncodeUINT(m_nEndFlag) == false)
        return false;

    nMsgLen = m_nMsgLength;
    LogDI("%s: MsgLen: %d, Msg: \n%s", toString(), nMsgLen, ConvertToHex(m_Buffer, nMsgLen));

    return true;
}

bool CRSUUpdateSoftwareMsg::Decode(PUCHAR pBuffer, UINT nMsgLen)
{
    if (CMsgBase::Decode(pBuffer, nMsgLen) == false)
        return false;

    if (DecodeString(m_strFileName) == false)
        return false;

    if (DecodeBYTE(m_nUpdateMode) == false)
        return false;

    if (DecodeUINT(m_nEndFlag) == false)
        return false;

    nMsgLen = m_nMsgLength;
    LogDI("%s: MsgLen: %d, Msg: \n%s", toString(), nMsgLen, ConvertToHex(m_Buffer, nMsgLen));

    return true;
}


CRSURebootMsg::CRSURebootMsg()
{
    m_nMsgType = RSU_MSG_Reboot;

    m_nDelaySeconds = 0;
    m_nRebootMode   = Reboot_Application;
}


bool CRSURebootMsg::Encode(UINT& nMsgLen)
{
    if (CMsgBase::Encode(nMsgLen) == false)
        return false;

    if (EncodeUINT(m_nDelaySeconds) == false)
        return false;

    if (EncodeBYTE(m_nRebootMode) == false)
        return false;

    if (EncodeUINT(m_nEndFlag) == false)
        return false;

    nMsgLen = m_nMsgLength;
    LogDI("%s: MsgLen: %d, Msg: \n%s", toString(), nMsgLen, ConvertToHex(m_Buffer, nMsgLen));

    return true;
}

bool CRSURebootMsg::Decode(PUCHAR pBuffer, UINT nMsgLen)
{
    if (CMsgBase::Decode(pBuffer, nMsgLen) == false)
        return false;

    if (DecodeUINT(m_nDelaySeconds) == false)
        return false;

    if (DecodeBYTE(m_nRebootMode) == false)
        return false;

    if (DecodeUINT(m_nEndFlag) == false)
        return false;

    nMsgLen = m_nMsgLength;
    LogDI("%s: MsgLen: %d, Msg: \n%s", toString(), nMsgLen, ConvertToHex(m_Buffer, nMsgLen));

    return true;
}


CRSUSetParamMsg::CRSUSetParamMsg()
{
    m_nMsgType = RSU_MSG_SetParam;

    m_nParamCount = 0;
    for (int i = 0; i < MAX_RSU_MSG_PARAM; i++)
        m_MsgParam[i] = NULL;

}


bool CRSUSetParamMsg::Encode(UINT& nMsgLen)
{
    if (CMsgBase::Encode(nMsgLen) == false)
        return false;

    if (EncodeBYTE(m_nParamCount) == false)
        return false;

    for ( int i = 0; i < m_nParamCount; i++ )
    {
        if ( EncodeParam(m_MsgParam[i]) == false )
            return false;
    }

    if (EncodeUINT(m_nEndFlag) == false)
        return false;

    nMsgLen = m_nMsgLength;
    LogDI("%s: MsgLen: %d, Msg: \n%s", toString(), nMsgLen, ConvertToHex(m_Buffer, nMsgLen));

    return true;
}

bool CRSUSetParamMsg::EncodeParam(CRSUMsgParam* pParam)
{
    if ( pParam == NULL )
    {
        LogE("CRSUMsgParam EncodeParam pParam is NULL");
        return false;
    }

    if (EncodeBYTE(pParam->GetParamTag()) == false)
        return false;

    if (EncodeBYTE(pParam->GetDataType()) == false)
        return false;

    switch(pParam->GetDataType())
    {
    case RPT_String:
        if (EncodeString(pParam->GetStringData()) == false)
            return false;
        break;
    case RPT_INT:
        if (EncodeInt(pParam->GetIntData()) == false)
            return false;
        break;
    case RPT_UINT:
        if (EncodeUINT(pParam->GetUINTData()) == false)
            return false;
        break;
    case RPT_SHORT:
        if (EncodeUSHORT(pParam->GetShortData()) == false)
            return false;
        break;
    case RPT_USHORT:
        if (EncodeUSHORT(pParam->GetUshortData()) == false)
            return false;
        break;
    case RPT_BYTE:
        if (EncodeBYTE(pParam->GetByteData()) == false)
            return false;
        break;

    default:
        LogE("RSU Msg Pram Tag %d is out of rang [0, %d)", pParam->GetDataType(), RPT_Invalid);
        return false;
    }

    return true;
}

int CRSUSetParamMsg::SetParamData(CRSUMsgParam* pParamData)
{
    if ( pParamData == NULL )
        return -1;

    if ( m_nParamCount >= MAX_RSU_MSG_PARAM )
        return -1;

    m_MsgParam[m_nParamCount] = pParamData;
    m_nParamCount ++;

    return m_nParamCount - 1;
}

void CRSUSetParamMsg::GetParamData(int nIndex, CRSUMsgParam* pParamData)
{
    if ( nIndex >= m_nParamCount )
    {
        pParamData = NULL;
        return;
    }

    pParamData = m_MsgParam[nIndex];
    return;
}


bool CRSUSetParamMsg::Decode(PUCHAR pBuffer, UINT nMsgLen)
{
    if (CMsgBase::Decode(pBuffer, nMsgLen) == false)
        return false;

    if (DecodeBYTE(m_nParamCount) == false)
        return false;

    for ( int i = 0; i < m_nParamCount; i++ )
    {
        if ( m_MsgParam[i] != NULL )
        {
            delete m_MsgParam[i];
            m_MsgParam[i] = new CRSUMsgParam;
        }

        if ( DecodeParam(m_MsgParam[i]) == false )
            return false;
    }

    if (DecodeUINT(m_nEndFlag) == false)
        return false;

    nMsgLen = m_nMsgLength;
    LogDI("%s: MsgLen: %d, Msg: \n%s", toString(), nMsgLen, ConvertToHex(m_Buffer, nMsgLen));

    return true;
}

bool CRSUSetParamMsg::DecodeParam(CRSUMsgParam* pParam)
{
    BYTE nTag;
    if (DecodeBYTE(nTag) == false)
        return false;

    BYTE nParamType;
    if (DecodeBYTE(nParamType) == false)
        return false;

    int         nIntData;
    UINT        nUINTData;
    BYTE        nByteData;
    USHORT      nUSHORTData;
    short       nShortData;
    CString     strData;

    switch(nParamType)
    {
    case RPT_String:
        if (DecodeString(strData) == false)
            return false;
        pParam->SetParamData(nTag, strData);
        break;
    case RPT_INT:
        if (DecodeInt(nIntData) == false)
            return false;
        pParam->SetParamData(nTag, nIntData);
        break;
    case RPT_UINT:
        if (DecodeUINT(nUINTData) == false)
            return false;
        pParam->SetParamData(nTag, nUINTData);
        break;
    case RPT_SHORT:
        if (DecodeUSHORT(nUSHORTData) == false)
            return false;
        pParam->SetParamData(nTag, (short)nUSHORTData);
        break;
    case RPT_USHORT:
        if (DecodeUSHORT(nUSHORTData) == false)
            return false;
        pParam->SetParamData(nTag, nUSHORTData);
        break;
    case RPT_BYTE:
        if (DecodeBYTE(nByteData) == false)
            return false;
        pParam->SetParamData(nTag, nByteData);
        break;

    default:
        LogE("RSU Msg Pram Tag %d is out of rang [0, %d)", pParam->GetDataType(), RPT_Invalid);
        return false;
    }

    return true;
}




CRSUGetParamAckMsg::CRSUGetParamAckMsg()
{
    m_nMsgType = RSU_MSG_GetParamAck;

    m_nParamCount = 0;
    for (int i = 0; i < MAX_RSU_MSG_PARAM; i++)
        m_MsgParam[i] = NULL;
}


bool CRSUGetParamAckMsg::Encode(UINT& nMsgLen)
{
    if (CMsgBase::Encode(nMsgLen) == false)
        return false;

    if (EncodeBYTE(m_nParamCount) == false)
        return false;

    for ( int i = 0; i < m_nParamCount; i++ )
    {
        if ( EncodeParam(m_MsgParam[i]) == false )
            return false;
    }

    if (EncodeUINT(m_nEndFlag) == false)
        return false;

    nMsgLen = m_nMsgLength;
    LogDI("%s: MsgLen: %d, Msg: \n%s", toString(), nMsgLen, ConvertToHex(m_Buffer, nMsgLen));

    return true;
}

bool CRSUGetParamAckMsg::EncodeParam(CRSUMsgParam* pParam)
{
    if ( pParam == NULL )
    {
        LogE("CRSUMsgParam EncodeParam pParam is NULL");
        return false;
    }

    if (EncodeBYTE(pParam->GetParamTag()) == false)
        return false;

    switch(pParam->GetDataType())
    {
    case RPT_String:
        if (EncodeString(pParam->GetStringData()) == false)
            return false;
        break;
    case RPT_INT:
        if (EncodeInt(pParam->GetIntData()) == false)
            return false;
        break;
    case RPT_UINT:
        if (EncodeUINT(pParam->GetUINTData()) == false)
            return false;
        break;
    case RPT_SHORT:
        if (EncodeUSHORT(pParam->GetShortData()) == false)
            return false;
        break;
    case RPT_USHORT:
        if (EncodeUSHORT(pParam->GetUshortData()) == false)
            return false;
        break;
    case RPT_BYTE:
        if (EncodeBYTE(pParam->GetByteData()) == false)
            return false;
        break;

    default:
        LogE("RSU Msg Pram Tag %d is out of rang [0, %d)", pParam->GetDataType(), RPT_Invalid);
        return false;
    }

    return true;
}

int CRSUGetParamAckMsg::SetParamData(CRSUMsgParam* pParamData)
{
    if ( pParamData == NULL )
        return -1;

    if ( m_nParamCount >= MAX_RSU_MSG_PARAM )
        return -1;

    m_MsgParam[m_nParamCount] = pParamData;
    m_nParamCount ++;

    return m_nParamCount - 1;
}

void CRSUGetParamAckMsg::GetParamData(int nIndex, CRSUMsgParam* pParamData)
{
    if ( nIndex >= m_nParamCount )
    {
        pParamData = NULL;
        return;
    }

    pParamData = m_MsgParam[nIndex];
    return;
}

bool CRSUGetParamAckMsg::Decode(PUCHAR pBuffer, UINT nMsgLen)
{
    if (CMsgBase::Decode(pBuffer, nMsgLen) == false)
        return false;

    if (DecodeBYTE(m_nParamCount) == false)
        return false;

    for ( int i = 0; i < m_nParamCount; i++ )
    {
        if ( m_MsgParam[i] != NULL )
        {
            delete m_MsgParam[i];
            m_MsgParam[i] = new CRSUMsgParam;
        }

        if ( DecodeParam(m_MsgParam[i]) == false )
            return false;
    }

    if (DecodeUINT(m_nEndFlag) == false)
        return false;

    nMsgLen = m_nMsgLength;
    LogDI("%s: MsgLen: %d, Msg: \n%s", toString(), nMsgLen, ConvertToHex(m_Buffer, nMsgLen));

    return true;
}

bool CRSUGetParamAckMsg::DecodeParam(CRSUMsgParam* pParam)
{
    BYTE nTag;
    if (DecodeBYTE(nTag) == false)
        return false;

    int         nIntData;
    UINT        nUINTData;
    BYTE        nByteData;
    USHORT      nUSHORTData;
    short       nShortData;
    CString     strData;

    switch(nTag)
    {
    case RPT_String:
        if (DecodeString(strData) == false)
            return false;
        pParam->SetParamData(nTag, strData);
        break;
    case RPT_INT:
        if (DecodeInt(nIntData) == false)
            return false;
        pParam->SetParamData(nTag, nIntData);
        break;
    case RPT_UINT:
        if (DecodeUINT(nUINTData) == false)
            return false;
        pParam->SetParamData(nTag, nUINTData);
        break;
    case RPT_SHORT:
        if (DecodeUSHORT(nUSHORTData) == false)
            return false;
        pParam->SetParamData(nTag, (short)nUSHORTData);
        break;
    case RPT_USHORT:
        if (DecodeUSHORT(nUSHORTData) == false)
            return false;
        pParam->SetParamData(nTag, nUSHORTData);
        break;
    case RPT_BYTE:
        if (DecodeBYTE(nByteData) == false)
            return false;
        pParam->SetParamData(nTag, nByteData);
        break;

    default:
        LogE("RSU Msg Pram Tag %d is out of rang [0, %d)", pParam->GetDataType(), RPT_Invalid);
        return false;
    }

    return true;
}

CRSUReportStatusMsg::CRSUReportStatusMsg()
{
    m_nMsgType      = RSU_MSG_ReportStatus;

    m_nCPURate      = 0;
    m_nMemoryRate   = 0;
    m_nHardDiskRate = 0;
}

void CRSUReportStatusMsg::SetParam(UINT nCPU, UINT nMemory, UINT nHardDisk)
{
    m_nCPURate      = nCPU;
    m_nMemoryRate   = nMemory;
    m_nHardDiskRate = nHardDisk;
}

////////////////////////////////////////////////////////////////////////
// 系统运行状态消息
bool CRSUReportStatusMsg::Encode(UINT &nMsgLen)
{
    if (CMsgBase::Encode(nMsgLen) == false)
        return false;

    if (EncodeUINT(m_nCPURate) == false)
        return false;

    if (EncodeUINT(m_nMemoryRate) == false)
        return false;

    if (EncodeUINT(m_nHardDiskRate) == false)
        return false;

    if (EncodeUINT(m_nEndFlag) == false)
        return false;

    nMsgLen = m_nMsgLength;
    LogDI("%s: MsgLen: %d, Msg: %s", toString(), nMsgLen, ConvertToHex(m_Buffer, nMsgLen));

    return true;
}

bool CRSUReportStatusMsg::Decode(PUCHAR pBuffer, UINT nMsgLen)
{
    if (CMsgBase::Decode(pBuffer, nMsgLen) == false)
        return false;

    if (DecodeUINT(m_nCPURate) == false)
        return false;

    if (DecodeUINT(m_nMemoryRate) == false)
        return false;

    if (DecodeUINT(m_nHardDiskRate) == false)
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

