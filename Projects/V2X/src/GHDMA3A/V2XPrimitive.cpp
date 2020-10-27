// ------------------------------------------------------------------------------
// RSU Driver - Component :: V2X Primitive definition
// Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    V2XPrimitive.cpp
// Purpose: RSU module
//-------------------------------------------------------------------------------

#include "Debug.h"
#include "V2XPrimitive.h"

void PrimitiveTest()
{
    UCHAR data[10];
    for(int i = 0; i < 10; i++)
        data[i] = i;

    CV2XPrimitiveReqAID reqAID;
    reqAID.SetParam(0x11);
    CV2XPrimitiveReqTrafficPeriod reqPeriod;
    reqPeriod.SetParam(0x1);
    CV2XPrimitiveReqNetProtocolType reqProtocolType;
    reqProtocolType.SetParam(0x04);
    CV2XPrimitiveReqPriority reqPriority;
    reqPriority.SetParam(0x00);
    CV2XPrimitiveReqAIDCS reqAIDCS;
    reqAIDCS.SetParam(0x01);
    CV2XPrimitiveReqTrafficID reqTrafficID;
    reqTrafficID.SetParam(0xFFFF);
    CV2XPrimitiveReqSrcAddr reqSrcAddr;
    reqSrcAddr.SetParam("192.168.1.190");
    CV2XPrimitiveReqDestAddr reqDestAddr;
    reqDestAddr.SetParam("192.168.1.190");
    CV2XPrimitiveReqUTC reqUTC;
    reqUTC.SetParam(123ll);
    CV2XPrimitiveReqPayload reqPayload;
    reqPayload.SetParam(data, 10);

    CList list;

    // 在列表尾部加入
    list.AddTail(&reqAID);
    list.Add(&reqPeriod);
    list.Add(&reqPeriod);

    // 在第一个节点加入，为第二个位置
    list.AddAfter(&reqProtocolType, &reqAID);

    // 在第二个节点前加入，成为新的第二个节点
    list.AddBefore(&reqPriority, &reqProtocolType);

    // 在链表头部加入
    list.AddHead(&reqAIDCS);

    list.Add(&reqTrafficID);

    LogI("Find: %d", list.Find(&reqAIDCS));
    LogI("Find: %d", list.Find(&reqPayload));
    LogI("GetHead: %d", list.GetHead());
    LogI("GetTail: %d", list.GetTail());

    LogI("GetNext: %d", list.GetNext(&reqAIDCS));
    LogI("GetPrev: %d", list.GetPrev(&reqAIDCS));
    LogI("GetPrev: %d", list.GetPrev(&reqTrafficID));
    LogI("GetAt: %d", list.GetAt(2));
    LogI("GetAt: %d", list.GetAt(10));

    // 删除列表头部
    list.Remove(&reqAIDCS);

    // 删除尾部
    list.Remove(&reqTrafficID);

    list.Remove(&reqPeriod);

    list.RemoveAll();
    return;


    CPrimitive tlv;
    tlv.InsertPrimitive(&reqAID);
    tlv.InsertPrimitive(&reqPeriod);
    tlv.InsertPrimitive(&reqProtocolType);
    tlv.InsertPrimitive(&reqPriority);
    tlv.InsertPrimitive(&reqAIDCS);
    tlv.InsertPrimitive(&reqTrafficID);
    tlv.InsertPrimitive(&reqSrcAddr);
    tlv.InsertPrimitive(&reqDestAddr);
    tlv.InsertPrimitive(&reqUTC);
    tlv.InsertPrimitive(&reqPayload);

    if(tlv.Encode() == false)
    {
        LogE("Encode TLV failed");
        return;
    }

    PUCHAR pBuffer = tlv.GetMsgBuffer();
    UINT nLen = tlv.GetMsgLen();

    LogI("Encode Data: %s", ConvertToHex(pBuffer, nLen));

    tlv.DeleteAllPrimitive();

    CPrimitive tlv2;
    if ( tlv2.Decode(pBuffer, nLen, true) == false )
    {
        LogE("Decode TLV failed");
        return;
    }

    CV2XPrimitiveIndPayload indPayload;
    CV2XPrimitiveIndSNTraceArray indArray;
    CV2XPrimitiveIndAID indAID;
    indAID.SetParam(0x11);
    CV2XPrimitiveIndPSN indPSN;
    indPSN.SetParam(0x12);
    CV2XPrimitiveIndUTC indUTC;
    indUTC.SetParam(8ll);
    CV2XPrimitiveIndSrcAddr indSrcAddr;
    indSrcAddr.SetParam("192.168.1.190");
    CV2XPrimitiveIndDestAddr indDestAddr;
    indDestAddr.SetParam("192.168.1.190");
    CV2XPrimitiveIndNetProtocolType indProtocolType;
    indProtocolType.SetParam(0x04);
    CV2XPrimitiveIndPriority indPriority;
    indPriority.SetParam(0x00);
    CV2XPrimitiveIndRSRP indRSRP;
    indRSRP.SetParam(0xFFFFFFC9);
    CV2XPrimitiveIndSINR indSINR;
    indSINR.SetParam(0x1400);
    CV2XPrimitiveIndRxTotalPower indRxTotoalPower;
    indRxTotoalPower.SetParam(0x14);
    CV2XPrimitiveIndResourcePool1_CBR indPool1;
    indPool1.SetParam(0x14);
    CV2XPrimitiveIndResourcePool2_CBR indPool2;
    indPool2.SetParam(0x16);
    CV2XPrimitiveIndMaxDataRate indDataRate;
    indDataRate.SetParam(0x666AC0);

    indPayload.SetParam(data, 10);
    indArray.SetParam(data, 10);

    tlv.InsertPrimitive(&indAID);
    tlv.InsertPrimitive(&indPSN);
    tlv.InsertPrimitive(&indUTC);
    tlv.InsertPrimitive(&indSrcAddr);
    tlv.InsertPrimitive(&indDestAddr);
    tlv.InsertPrimitive(&indProtocolType);
    tlv.InsertPrimitive(&indPriority);
    tlv.InsertPrimitive(&indRSRP);
    tlv.InsertPrimitive(&indSINR);
    tlv.InsertPrimitive(&indRxTotoalPower);
    tlv.InsertPrimitive(&indPool1);
    tlv.InsertPrimitive(&indPool2);
    tlv.InsertPrimitive(&indDataRate);
    tlv.InsertPrimitive(&indArray);
    tlv.InsertPrimitive(&indPayload);

    if(tlv.Encode() == false)
    {
        LogE("Encode TLV failed");
        return;
    }

    pBuffer = tlv.GetMsgBuffer();
    nLen = tlv.GetMsgLen();

    LogI("Encode Data: %s", ConvertToHex(pBuffer, nLen));

    CPrimitive tlv3;
    if ( tlv3.Decode(pBuffer, nLen, false) == false )
    {
        LogE("Decode TLV failed");
        return;
    }

    return;
}

CV2XPrimitiveHead::CV2XPrimitiveHead()
{
    m_nTag = IND_NULL;
    m_nLen = 0;
}

bool CV2XPrimitiveHead::Encode()
{
    m_nMsgLength    = 0;
    m_nPos          = 0;
    m_pBuf          = m_Buffer;

    LogDI("Encode Primitive Head");

    if( EncodeUSHORT( m_nTag ) == false)
        return false;

    if( EncodeUSHORT( m_nLen ) == false)
        return false;

    return true;
}

bool CV2XPrimitiveHead::Decode(PUCHAR pBuffer, UINT nLen)
{
    m_pBuf          = pBuffer;
    m_nMsgLength    = nLen;
    m_nPos          = 0;

    LogDI("Primitive Data: %s", ConvertToHex(pBuffer, nLen));

    if ( DecodeUSHORT(m_nTag) == false)
        return false;

    if ( DecodeUSHORT(m_nLen) == false)
        return false;

    return true;
}

CV2XPrimitivePayload::~CV2XPrimitivePayload()
{
    if ( m_pData != NULL && m_nLen > 0 )
    {
        // LogI("CV2XPrimitivePayload");
        delete m_pData;
        m_pData = NULL;
        m_nLen = 0;
    }
}

void CV2XPrimitivePayload::SetParam(PUCHAR pData, USHORT nLen)
{
    if ( nLen >= MAX_PRIMITIVE_DATA_LEN || pData == NULL )
    {
        LogE("%s SetParam Parameters is out of tange or NULL Pointer", toString());
        return;
    }

    m_nLen = nLen;

    m_pData = new UCHAR(m_nLen);
    if ( m_pData == NULL )
    {
        LogE("%s SetParam alloc memory failed.", toString() );
        return;
    }

    for (int i = 0; i < nLen; i++)
        m_pData[i] = pData[i];

    return;
}

bool CV2XPrimitivePayload::Encode()
{
    if ( CV2XPrimitiveHead::Encode() == false )
        return false;

    if ( m_pData == NULL )
    {
        LogE("%s Data is NULL Pointer", toString() );
        return false;
    }

    if ( EncodePUCHAR(m_pData, m_nLen) == false )
    {
        LogE("%s Encode Data failed.", toString() );
        return false;
    }

    return true;
}

bool CV2XPrimitivePayload::Decode(PUCHAR pBuffer, UINT nLen)
{
    if ( CV2XPrimitiveHead::Decode(pBuffer, nLen) == false )
        return false;

    if ( m_nLen == 0 )
        return true;

    m_pData = new UCHAR(m_nLen);
    if ( m_pData == NULL )
    {
        LogE("%s SetParam alloc memory failed.", toString() );
        return false;
    }

    if ( DecodePUCHAR(m_pData, m_nLen) == false )
    {
        LogE("%s Encode Data failed.", toString() );
        return false;
    }

    return true;
}

bool CV2XPrimitiveUINT::Encode()
{
    if ( CV2XPrimitiveHead::Encode() == false )
        return false;

    if( EncodeUINT( m_nData ) == false)
        return false;

    return true;
}

bool CV2XPrimitiveUINT::Decode(PUCHAR pBuffer, UINT nLen)
{
    if ( CV2XPrimitiveHead::Decode(pBuffer, nLen) == false )
        return false;

    if( DecodeUINT( m_nData ) == false)
        return false;

    return true;
}

bool CV2XPrimitiveAddr::Encode()
{
    if ( CV2XPrimitiveHead::Encode() == false )
        return false;


    CList list;
    m_strAddr.Splite(".", list);
    if ( list.GetCount() != 4 && list.GetCount() != 6 )
    {
        LogE("%s Addr %s format is %d.", toString(), m_strAddr.GetBuffer(), list.GetCount());
        return false;
    }

    for (int i = 0; i < list.GetCount(); i++)
    {
        CString* pStr = (CString*)list.GetAt(i);
        int temp = atoi(pStr->GetBuffer());
        if( EncodeInt( temp ) == false)
            return false;
    }

    // int nAddr = inet_addr(m_strAddr.GetBuffer());
    // if( EncodeInt( nAddr ) == false)
    //     return false;

    return true;
}

bool CV2XPrimitiveAddr::Decode(PUCHAR pBuffer, UINT nLen)
{
    if ( CV2XPrimitiveHead::Decode(pBuffer, nLen) == false )
        return false;

    int nAddr = -1;
    if( DecodeInt( nAddr ) == false)
        return false;

    struct in_addr addr1;
    memcpy(&addr1, &nAddr, 4);

    m_strAddr = inet_ntoa(addr1);

    return true;
}

bool CV2XPrimitiveUTC::Encode()
{
    if ( CV2XPrimitiveHead::Encode() == false )
        return false;

    if( EncodeULongLong( m_nUTC ) == false)
        return false;

    return true;
}

bool CV2XPrimitiveUTC::Decode(PUCHAR pBuffer, UINT nLen)
{
    if ( CV2XPrimitiveHead::Decode(pBuffer, nLen) == false )
        return false;

    if( DecodeULongLong( m_nUTC ) == false)
        return false;

    return true;
}

CPrimitive::CPrimitive()
{
    m_pDataBufer    = NULL;
    m_nDataLen      = 0;
    m_bIsEncoding   = false;
}

CPrimitive::~CPrimitive()
{
    if ( m_bIsEncoding == true && m_pDataBufer != NULL )
    {
        delete m_pDataBufer;
        m_pDataBufer = NULL;
    }

    m_TLVItemList.RemoveAll();
}

bool CPrimitive::Encode()
{
    m_nDataLen = 0;
    if ( m_pDataBufer != NULL )
    {
        delete m_pDataBufer;
        m_pDataBufer = NULL;
    }

    if ( m_TLVItemList.GetCount() == 0 )
    {
        LogE("No Primitive in list.");
        return false;
    }

    // 对原语进行编码并获得编码长度
    for(int i = 0; i < m_TLVItemList.GetCount(); i++)
    {
        CV2XPrimitiveHead* pPrimitive = (CV2XPrimitiveHead*)m_TLVItemList.GetAt(i);
        if ( pPrimitive == NULL )
        {
            LogE("Encode TLV Primitive failed for NULL Pointer.");
            return false;
        }

        if ( pPrimitive->Encode() == false )
            return false;

        m_nDataLen += pPrimitive->GetMsgLen();
        LogI("Primitive: %s, Len: %d", pPrimitive->toString(), m_nDataLen);
    }

    if ( m_nDataLen == 0 || m_nDataLen > MAX_PRIMITIVE_DATA_LEN )
    {
        LogE("Encode TLV Primitive failed, length %d is out of range [0, %d].", m_nDataLen, MAX_PRIMITIVE_DATA_LEN);
        return false;
    }

    m_pDataBufer = new UCHAR[m_nDataLen];
    if ( m_pDataBufer == NULL )
    {
        LogE("CPrimitive Encode alloc memory failed.");
        return false;
    }

    m_bIsEncoding = true;
    memset(m_pDataBufer, 0, m_nDataLen);

    int nIndex = 0;
    for(int i = 0; i < m_TLVItemList.GetCount(); i++)
    {
        CV2XPrimitiveHead* pPrimitive = (CV2XPrimitiveHead*)m_TLVItemList.GetAt(i);
        PUCHAR pData = pPrimitive->GetMsgBuffer();
        for(int j = 0; j < pPrimitive->GetMsgLen(); j++)
        {
            m_pDataBufer[nIndex++] = *(pData + j);
        }
    }

    return true;
}

bool CPrimitive::Decode(PUCHAR pBuffer, UINT nLen, bool bIsReqPrimitive)
{
    CV2XPrimitiveHead* pTemp = new CV2XPrimitiveReqPayload();
    delete pTemp;

    if ( pBuffer == NULL || nLen < 0 || nLen >= MAX_PRIMITIVE_DATA_LEN )
    {
        LogE("Decode TLV Primitive failed, parameters is wrong");
        return false;
    }

    int nIndex = 0;
    while( nIndex < nLen )
    {
        CV2XPrimitiveHead head;
        if (head.Decode( (pBuffer + nIndex), nLen) == false )
        {
            LogE("Decode Primitive head failed.");
            return false;
        }

        CV2XPrimitiveHead* pPrimitive = NULL;
        if ( bIsReqPrimitive == true )
        {
            switch(head.GetTag())
            {
            case REQ_PAYLOAD:
                pPrimitive = new CV2XPrimitiveReqPayload();
                break;
            case REQ_PSN:
                pPrimitive = new CV2XPrimitiveReqPSN();
                break;
            case REQ_AID:
                pPrimitive = new CV2XPrimitiveReqAID();
                break;
            case REQ_TRAFFIC_PERIOD:
                pPrimitive = new CV2XPrimitiveReqTrafficPeriod();
                break;
            case REQ_NET_PROTOCOL_TYPE:
                pPrimitive = new CV2XPrimitiveReqNetProtocolType();
                break;
            case REQ_PRIORITY:
                pPrimitive = new CV2XPrimitiveReqPriority();
                break;
            case REQ_AID_CS:
                pPrimitive = new CV2XPrimitiveReqAIDCS();
                break;
            case REQ_TRAFFIC_ID:
                pPrimitive = new CV2XPrimitiveReqTrafficID();
                break;
            case REQ_UTC:
                pPrimitive = new CV2XPrimitiveReqUTC();
                break;           
            case REQ_SRC_ADDR:
                pPrimitive = new CV2XPrimitiveReqSrcAddr();
                break;
            case REQ_DST_ADDR:
                pPrimitive = new CV2XPrimitiveReqDestAddr();
                break;

            default:
                LogE("Decode Primitive failed: tag %d is out of range.", head.GetTag());
                return false;
            }
        }
        else
        {
            switch(head.GetTag())
            {
            case IND_PAYLOAD:
                pPrimitive = new CV2XPrimitiveIndPayload();
                break;
            case IND_SN_TRACEARRAY:
                pPrimitive = new CV2XPrimitiveIndSNTraceArray();
                break;
            case IND_PSN:
                pPrimitive = new CV2XPrimitiveIndPSN();
                break;
            case IND_AID:
                pPrimitive = new CV2XPrimitiveIndAID();
                break;
            case IND_NET_PROTOCOL_TYPE:
                pPrimitive = new CV2XPrimitiveIndNetProtocolType();
                break;
            case IND_PRIORITY:
                pPrimitive = new CV2XPrimitiveIndPriority();
                break;
            case IND_RSRP:
                pPrimitive = new CV2XPrimitiveIndRSRP();
                break;
            case IND_SINR:
                pPrimitive = new CV2XPrimitiveIndSINR();
                break;
            case IND_RX_TOTAL_POWER:
                pPrimitive = new CV2XPrimitiveIndRxTotalPower();
                break;
            case IND_RESOURCE_POOL1_CBR:
                pPrimitive = new CV2XPrimitiveIndResourcePool1_CBR();
                break;
            case IND_RESOURCE_POOL2_CBR:
                pPrimitive = new CV2XPrimitiveIndResourcePool2_CBR();
                break;
            case IND_MAX_DATA_RATE:
                pPrimitive = new CV2XPrimitiveIndMaxDataRate();
                break;
            case IND_UTC:
                pPrimitive = new CV2XPrimitiveIndUTC();
                break;
            case IND_SRC_ADDR:
                pPrimitive = new CV2XPrimitiveIndSrcAddr();
                break;
            case IND_DST_ADDR:
                pPrimitive = new CV2XPrimitiveIndDestAddr();
                break;

            default:
                LogE("Decode Primitive failed: tag %d is out of range.", head.GetTag());
                return false;
            }
        }

        if ( pPrimitive->Decode( (pBuffer + nIndex), nLen ) == false )
        {
            LogE("Decode Primitive failed.");
            return false;
        }

        LogI("Decode Primitive: %s", pPrimitive->toString());

        m_TLVItemList.AddTail(pPrimitive);
        nIndex += pPrimitive->GetLen() + 4;
    }

    // 在对象销毁时自动释放申请的内存
    m_TLVItemList.SetMode(true);

    return true;
}

