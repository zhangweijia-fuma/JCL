// ------------------------------------------------------------------------------
// RSU Driver - Component :: V2X Primitive definition
// Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    V2XPrimitive.h
// Purpose: RSU module
//-------------------------------------------------------------------------------

#ifndef _V2X_PRIMITIVE_H_
#define _V2X_PRIMITIVE_H_

#include "Common.h"
#include "Codec.h"

#define MAX_PRIMITIVE_DATA_LEN          1024

enum Primitive_Indicator_Tag
{
    IND_NULL = 0,
    IND_PAYLOAD = 1,
    IND_SN_TRACEARRAY,
    IND_PSN,
    IND_UTC,
    IND_AID,
    IND_SRC_ADDR,
    IND_DST_ADDR,
    IND_NET_PROTOCOL_TYPE,
    IND_PRIORITY,
    IND_RSRP,
    IND_SINR,
    IND_RX_TOTAL_POWER,
    IND_RESOURCE_POOL1_CBR,
    IND_RESOURCE_POOL2_CBR,
    IND_MAX_DATA_RATE,
};

enum Primitive_Request_Tag
{
    REQ_NULL = 0,
    REQ_PAYLOAD = 1,
    REQ_PSN,
    REQ_UTC,
    REQ_AID,
    REQ_TRAFFIC_PERIOD,
    REQ_NET_PROTOCOL_TYPE,
    REQ_PRIORITY,
    REQ_AID_CS,
    REQ_TRAFFIC_ID,
    REQ_SRC_ADDR,
    REQ_DST_ADDR,
};


class CV2XPrimitiveHead: public CCodec
{
    public:
        CV2XPrimitiveHead();

        virtual bool        Encode();
        virtual bool        Decode(PUCHAR pBuffer, UINT nLen);

        USHORT              GetTag() { return m_nTag; }
        USHORT              GetLen() { return m_nLen; }

        virtual PCSTR       toString() { return "CV2XPrimitiveHead"; }

    protected:
        USHORT              m_nTag;
        USHORT              m_nLen;
};

class CV2XPrimitivePayload : public CV2XPrimitiveHead
{
    public:
        CV2XPrimitivePayload(){ m_pData = NULL; m_nLen = 0; };
        ~CV2XPrimitivePayload();

        virtual bool        Encode();
        virtual bool        Decode(PUCHAR pBuffer, UINT nLen);

        void                SetParam(PUCHAR pData, USHORT nLen);
        PUCHAR              GetParam() { return m_pData; }

    protected:
        PUCHAR              m_pData;
};

class CV2XPrimitiveUINT : public CV2XPrimitiveHead
{
    public:
        CV2XPrimitiveUINT() { m_nData = 0; };

        virtual bool        Encode();
        virtual bool        Decode(PUCHAR pBuffer, UINT nLen);

        void                SetParam(UINT nData) { m_nData = nData; }
        UINT                GetParam() { return m_nData; }

    protected:
        UINT                m_nData;
};

class CV2XPrimitiveAddr : public CV2XPrimitiveHead
{
    public:
        CV2XPrimitiveAddr() { m_strAddr = ""; m_nLen = 0x10; };

        virtual bool        Encode();
        virtual bool        Decode(PUCHAR pBuffer, UINT nLen);

        void                SetParam(CString strAddr) { m_strAddr = strAddr; }
        CString             GetParam() { return m_strAddr; }
  
    protected:
        CString             m_strAddr;                                  // 使用字符串表示，编码时为16字节的数字，
};

class CV2XPrimitiveUTC : public CV2XPrimitiveHead
{
    public:
        CV2XPrimitiveUTC() { m_nUTC = 0;};

        virtual bool        Encode();
        virtual bool        Decode(PUCHAR pBuffer, UINT nLen);

        void                SetParam(ULONGLONG nData) { m_nUTC = nData; }
        ULONGLONG           GetParam() { return m_nUTC; }

    protected:
        ULONGLONG           m_nUTC;
};

class CV2XPrimitiveReqPayload : public CV2XPrimitivePayload
{
    public:
        CV2XPrimitiveReqPayload() { m_nTag = REQ_PAYLOAD; m_nLen = 0; }
        virtual PCSTR       toString() { return "CV2XPrimitiveReqPayload"; }
};

class CV2XPrimitiveReqTrafficPeriod : public CV2XPrimitiveUINT
{
    public:
        CV2XPrimitiveReqTrafficPeriod() { m_nTag = REQ_TRAFFIC_PERIOD; m_nLen = 4; }
        virtual PCSTR       toString() { return "CV2XPrimitiveReqTrafficPeriod"; }
};

class CV2XPrimitiveReqTrafficID : public CV2XPrimitiveUINT
{
    public:
        CV2XPrimitiveReqTrafficID() { m_nTag = REQ_TRAFFIC_ID; m_nLen = 4; }
        virtual PCSTR       toString() { return "CV2XPrimitiveReqTrafficID"; }
};

class CV2XPrimitiveReqNetProtocolType : public CV2XPrimitiveUINT
{
    public:
        CV2XPrimitiveReqNetProtocolType() { m_nTag = REQ_NET_PROTOCOL_TYPE; m_nLen = 4; }
        virtual PCSTR       toString() { return "CV2XPrimitiveReqNetProtocolType"; }
};

class CV2XPrimitiveReqPriority : public CV2XPrimitiveUINT
{
    public:
        CV2XPrimitiveReqPriority() { m_nTag = REQ_PRIORITY; m_nLen = 4; }
        virtual PCSTR       toString() { return "CV2XPrimitiveReqPriority"; }
};
class CV2XPrimitiveReqAIDCS : public CV2XPrimitiveUINT
{
    public:
        CV2XPrimitiveReqAIDCS() { m_nTag = REQ_AID_CS; m_nLen = 4; }
        virtual PCSTR       toString() { return "CV2XPrimitiveReqAIDCS"; }
};
class CV2XPrimitiveReqSrcAddr : public CV2XPrimitiveAddr
{
    public:
        CV2XPrimitiveReqSrcAddr() { m_nTag = REQ_SRC_ADDR; m_nLen = 0x10; }
        virtual PCSTR       toString() { return "CV2XPrimitiveReqSrcAddr"; }
};
class CV2XPrimitiveReqDestAddr : public CV2XPrimitiveAddr
{
    public:
        CV2XPrimitiveReqDestAddr() { m_nTag = REQ_DST_ADDR; m_nLen = 0x10; }
        virtual PCSTR       toString() { return "CV2XPrimitiveReqDestAddr"; }
};
class CV2XPrimitiveReqPSN : public CV2XPrimitiveUINT
{
    public:
        CV2XPrimitiveReqPSN() { m_nTag = REQ_PSN; m_nLen = 4; }
        virtual PCSTR       toString() { return "CV2XPrimitiveReqPSN"; }
};
class CV2XPrimitiveReqAID : public CV2XPrimitiveUINT
{
    public:
        CV2XPrimitiveReqAID() { m_nTag = REQ_AID; m_nLen = 4; }
        virtual PCSTR       toString() { return "CV2XPrimitiveReqAID"; }
};
class CV2XPrimitiveReqUTC : public CV2XPrimitiveUTC
{
    public:
        CV2XPrimitiveReqUTC() { m_nTag = REQ_UTC; m_nLen = 8; }
        virtual PCSTR       toString() { return "CV2XPrimitiveReqUTC"; }
};

class CV2XPrimitiveIndPayload : public CV2XPrimitivePayload
{
    public:
        CV2XPrimitiveIndPayload() { m_nTag = IND_PAYLOAD; m_nLen = 0; }
        virtual PCSTR       toString() { return "CV2XPrimitiveIndPayload"; }
};

class CV2XPrimitiveIndSNTraceArray : public CV2XPrimitivePayload
{
    public:
        CV2XPrimitiveIndSNTraceArray() { m_nTag = IND_SN_TRACEARRAY; m_nLen = 0; }
        virtual PCSTR       toString() { return "CV2XPrimitiveIndSNTraceArray"; }
};

class CMAC_SDU_SN : public CV2XPrimitiveUINT
{
    public:
        // CMAC_SDU_SN() { m_nTag = IND_; m_nLen = 4; }
        virtual PCSTR       toString() { return "CMAC_SDU_SN"; }
};
class CMAC_PDU_SN : public CV2XPrimitiveUINT
{
    public:
        // CMAC_PDU_SN() { m_nTag = REQ_AID; m_nLen = 4; }
        virtual PCSTR       toString() { return "CMAC_PDU_SN"; }
};
class CRLC_SDU_SN : public CV2XPrimitiveUINT
{
    public:
        // CRLC_SDU_SN() { m_nTag = REQ_AID; m_nLen = 4; }
        virtual PCSTR       toString() { return "CRLC_SDU_SN"; }
};
class CV2XPrimitiveIndAID : public CV2XPrimitiveUINT
{
    public:
        CV2XPrimitiveIndAID() { m_nTag = IND_AID; m_nLen = 4; }
        virtual PCSTR       toString() { return "CV2XPrimitiveIndAID"; }
};
class CV2XPrimitiveIndPSN : public CV2XPrimitiveUINT
{
    public:
        CV2XPrimitiveIndPSN() { m_nTag = IND_PSN; m_nLen = 4; }
        virtual PCSTR       toString() { return "CV2XPrimitiveIndPSN"; }
};

class CV2XPrimitiveIndUTC : public CV2XPrimitiveUTC
{
    public:
        CV2XPrimitiveIndUTC() { m_nTag = IND_UTC; m_nLen = 8;}
        virtual PCSTR       toString() { return "CV2XPrimitiveIndUTC"; }
};
class CV2XPrimitiveIndSrcAddr : public CV2XPrimitiveAddr
{
    public:
        CV2XPrimitiveIndSrcAddr() { m_nTag = IND_SRC_ADDR; m_nLen = 0x10; }
        virtual PCSTR       toString() { return "CV2XPrimitiveIndSrcAddr"; }
};
class CV2XPrimitiveIndDestAddr : public CV2XPrimitiveAddr
{
    public:
        CV2XPrimitiveIndDestAddr() { m_nTag = IND_DST_ADDR; m_nLen = 0x10; }
        virtual PCSTR       toString() { return "CV2XPrimitiveIndDestAddr"; }
};
class CV2XPrimitiveIndNetProtocolType : public CV2XPrimitiveUINT
{
    public:
        CV2XPrimitiveIndNetProtocolType() { m_nTag = IND_NET_PROTOCOL_TYPE; m_nLen = 4; }
        virtual PCSTR       toString() { return "CV2XPrimitiveIndNetProtocolType"; }
};
class CV2XPrimitiveIndPriority : public CV2XPrimitiveUINT
{
    public:
        CV2XPrimitiveIndPriority() { m_nTag = IND_PRIORITY; m_nLen = 4; }
        virtual PCSTR       toString() { return "CV2XPrimitiveIndPriority"; }
};
class CV2XPrimitiveIndRSRP : public CV2XPrimitiveUINT
{
    public:
        CV2XPrimitiveIndRSRP() { m_nTag = IND_RSRP; m_nLen = 4; }
        virtual PCSTR       toString() { return "CV2XPrimitiveIndRSRP"; }
};
class CV2XPrimitiveIndSINR : public CV2XPrimitiveUINT
{
    public:
        CV2XPrimitiveIndSINR() { m_nTag = IND_SINR; m_nLen = 4; }
        virtual PCSTR       toString() { return "CV2XPrimitiveIndSINR"; }
};
class CV2XPrimitiveIndRxTotalPower : public CV2XPrimitiveUINT
{
    public:
        CV2XPrimitiveIndRxTotalPower() { m_nTag = IND_RX_TOTAL_POWER; m_nLen = 4; }
        virtual PCSTR       toString() { return "CV2XPrimitiveIndRxTotalPower"; }
};
class CV2XPrimitiveIndResourcePool1_CBR : public CV2XPrimitiveUINT
{
    public:
        CV2XPrimitiveIndResourcePool1_CBR() { m_nTag = IND_RESOURCE_POOL1_CBR; m_nLen = 4; }
        virtual PCSTR       toString() { return "CV2XPrimitiveIndResourcePool1_CBR"; }
};
class CV2XPrimitiveIndResourcePool2_CBR : public CV2XPrimitiveUINT
{
    public:
        CV2XPrimitiveIndResourcePool2_CBR() { m_nTag = IND_RESOURCE_POOL2_CBR; m_nLen = 4; }
        virtual PCSTR       toString() { return "CV2XPrimitiveIndResourcePool2_CBR"; }
};
class CV2XPrimitiveIndMaxDataRate : public CV2XPrimitiveUINT
{
    public:
        CV2XPrimitiveIndMaxDataRate() { m_nTag = IND_MAX_DATA_RATE; m_nLen = 4; }
        virtual PCSTR       toString() { return "CV2XPrimitiveIndMaxDataRate"; }
};


class CPrimitive : public CObjBase
{
    public:
        CPrimitive();
        ~CPrimitive();

    public:

        bool                Encode();
        bool                Decode(PUCHAR pBuffer, UINT nLen, bool bIsReqPrimitive);

        PUCHAR              GetMsgBuffer() { return m_pDataBufer; };
        int                 GetMsgLen() { return m_nDataLen; };

        bool                InsertPrimitive(CV2XPrimitiveHead* pItem) { return m_TLVItemList.AddTail(pItem); };
        bool                DeletePrimitive(CV2XPrimitiveHead* pItem) { return m_TLVItemList.Remove(pItem); };
        bool                DeletePrimitive(int nIndex) { return m_TLVItemList.Remove(nIndex); };
        bool                DeleteAllPrimitive() { return m_TLVItemList.RemoveAll(); };
        CV2XPrimitiveHead*  GetPrimitive(int nIndex) { return (CV2XPrimitiveHead*)m_TLVItemList.GetAt(nIndex); };
        int                 GetPrimitiveCount() { return m_TLVItemList.GetCount(); };

    protected:
        PUCHAR              m_pDataBufer;
        int                 m_nDataLen;
        bool                m_bIsEncoding;

        CList               m_TLVItemList;                              // 原语列表
};

void PrimitiveTest();

#endif  // _V2X_PRIMITIVE_H_

