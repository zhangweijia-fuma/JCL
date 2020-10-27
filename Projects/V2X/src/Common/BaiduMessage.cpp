/*------------------------------------------------------------------------------
 * RSU - Component :: Baidu Message Protocol Specification
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    BaiduMessage.cpp
 * Purpose: RSU module, Baidu Message Specification definition
 *----------------------------------------------------------------------------*/

#include <math.h>

#include "BaiduMessage.h"

CBaiduMessage::CBaiduMessage()
{
    m_nFrameHead    = BAIDU_FRAME_HEAD;
    m_nFrameEnd     = BAIDU_FRAME_END;

    m_nCurrentVersion   = BAIDU_CURRENT_VERSION;

    m_nFrameType    = 0;
    m_nDataLen      = 0;
    m_nCheckSum     = 0;
}

bool CBaiduMessage::Encode()
{
    m_nMsgLength    = 0;
    m_nPos          = 0;

    LogDI("Encode %s", toString() );

    if( EncodeUSHORT(m_nFrameHead) == false)
        return false;

    if( EncodeUSHORT(m_nCurrentVersion) == false)
        return false;

    if( EncodeUSHORT(m_nFrameType) == false)
        return false;

    USHORT nBitLen = m_MainMsgFrame.Encode();
    if ( nBitLen == CODEC_FAILED )
        return false;

    m_pData = m_MainMsgFrame.GetBuffer( nBitLen );
    m_nDataLen = ceil(nBitLen / 8);

    if( EncodeUSHORT(m_nDataLen) == false)
        return false;

    if( EncodePUCHAR(m_pData, m_nDataLen) == false )
        return false;

    if ( CalculateCheckSum(m_nCheckSum) == false)
        return false;

    if( EncodeUCHAR(m_nCheckSum) == false)
        return false;

    if( EncodeUSHORT(m_nFrameEnd) == false)
        return false;

    return true;
}

bool CBaiduMessage::Decode(PUCHAR pData, USHORT nLen)
{
    m_pBuf          = pData;
    m_nMsgLength    = nLen;
    m_nPos          = 0;

    LogDI("Decode %s", toString() );

    if( DecodeUSHORT(m_nFrameHead) == false)
        return false;

    if( DecodeUSHORT(m_nCurrentVersion) == false)
        return false;

    if( DecodeUSHORT(m_nFrameType) == false)
        return false;

    if( DecodeUSHORT(m_nDataLen) == false)
        return false;

    UCHAR *tempBuffer = new UCHAR[m_nDataLen];

    if( DecodePUCHAR(tempBuffer, m_nDataLen) == false )
        return false;

    if ( m_MainMsgFrame.Decode(tempBuffer, m_nDataLen * 8) == CODEC_FAILED )
        return false;

    if ( CalculateCheckSum( m_nCheckSum ) == false)
        return false;

    UCHAR nTemp;
    if( DecodeUCHAR(nTemp) == false)
        return false;

    if( DecodeUSHORT(m_nFrameEnd) == false)
        return false;

    if ( nTemp != m_nCheckSum )
    {
        LogE("Decode Baidu Message failed: checksum not consistent");
        return false;
    }

    return true;
}

bool CBaiduMessage::CalculateCheckSum(UCHAR &nCheckSum)
{

    return true;
}
