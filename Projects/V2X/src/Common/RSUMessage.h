/*------------------------------------------------------------------------------
 * RSU - Component :: RSU Message, Road Side Unit
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    RSUMessage.h
 * Purpose: RSU module, service flow realization
 *----------------------------------------------------------------------------*/

#ifndef _RSU_MESSAGE_H_
#define _RSU_MESSAGE_H_

#include "RSUTypes.h"
#include "RSUMsgBase.h"


#include "CSAEMsgBase.h"
#include "CSAEDataFrame.h"
#include "CSAEMessageSet.h"

#include "BaiduMessage.h"


// 初始化请求消息
class CRSUInitialRequestMsg : public CRSUMsgBase
{
    public:
        CRSUInitialRequestMsg();

    public:
        virtual bool        Decode(PUCHAR pBuffer, UINT nMsgLen);
        virtual bool        Encode(UINT &nMsgLen);

        void                SetParam(int nLng, int nLat) { m_nLongtitude = nLng; m_nLatitude = nLat; }
        void                GetParam(int& nLng, int& nLat) { nLng = m_nLongtitude; nLat = m_nLatitude; }
        
    protected:
        // 设备所在的经纬度，在经纬度值的基础上*1000000。如，值为123.3451231，则值为 123345123
        int                 m_nLongtitude, m_nLatitude;

        virtual const char* toString() { return "RSU InitialRequestMsg";}
};

// 初始化应答消息
class CRSUInitialRequestAckMsg : public CMsgAckBase
{
    public:
        CRSUInitialRequestAckMsg() { m_nMsgType = RSU_MSG_InitialRequestAck; }
    
    public:
        virtual const char* toString() { return "RSU InitialRequestAckMsg";}
};  

// RSU软件更新
class CRSUUpdateSoftwareMsg : public CRSUMsgBase
{
    public:
        CRSUUpdateSoftwareMsg();

    public:
        virtual bool        Decode(PUCHAR pBuffer, UINT nMsgLen);
        virtual bool        Encode(UINT &nMsgLen);

        virtual const char* toString() { return "RSU UpdateSoftwareMsg";}

        void                GetParam(CString& strFileName, BYTE& nUpdateMode) { strFileName = m_strFileName; nUpdateMode = m_nUpdateMode; }

    protected:
        CString             m_strFileName;
        BYTE                m_nUpdateMode;
};

class CRSUUpdateSoftwareAckMsg : public CMsgAckBase
{
    public:
        CRSUUpdateSoftwareAckMsg() { m_nMsgType = RSU_MSG_UpdateSoftwareAck; }
    
    public:
        virtual const char* toString() { return "RSU UpdateSoftwareAckMsg";}
};

class CRSURebootMsg : public CRSUMsgBase
{
    public:
        CRSURebootMsg();

    public:
        virtual bool        Decode(PUCHAR pBuffer, UINT nMsgLen);
        virtual bool        Encode(UINT &nMsgLen);

        virtual const char* toString() { return "RSU RebootMsg";}
        void                GetParam(UINT& nDelayTime, BYTE& nRebootMode) { nDelayTime = m_nDelaySeconds; nRebootMode = m_nRebootMode; }

    protected:
        UINT                m_nDelaySeconds;                            // 延时时间，单位：秒
        BYTE                m_nRebootMode;                              // 重启模式：0，重启应用，1：重启RSU
};

class CRSURebootAckMsg : public CMsgAckBase
{
    public:
        CRSURebootAckMsg() { m_nMsgType = RSU_MSG_RebootAck; }
    
    public:
        virtual const char* toString() { return "RSU RebootAckMsg";}
};


class CRSUSetParamMsg : public CRSUMsgBase
{
    public:
        CRSUSetParamMsg();

    public:
        virtual bool        Decode(PUCHAR pBuffer, UINT nMsgLen);
        virtual bool        Encode(UINT &nMsgLen);

        virtual const char* toString() { return "RSU SetParamMsg";}


        bool                EncodeParam(CRSUMsgParam* pParam);
        bool                DecodeParam(CRSUMsgParam* pParam);

        BYTE                GetParamCount() { return m_nParamCount; }
        void                GetParamData(int nIndex, CRSUMsgParam* pParamData);

        int                 SetParamData(CRSUMsgParam* pParamData);

    protected:
        BYTE                m_nParamCount;                              // 需要设置的参数个数
        CRSUMsgParam*       m_MsgParam[MAX_RSU_MSG_PARAM];
};

class CRSUSetParamAckMsg : public CMsgAckBase
{
    public:
        CRSUSetParamAckMsg() { m_nMsgType = RSU_MSG_SetParamAck; }
    
    public:
        virtual const char* toString() { return "RSU SetParamAckMsg";}
};

class CRSUGetParamMsg : public CRSUMsgBase
{
    public:
        CRSUGetParamMsg() { m_nMsgType = RSU_MSG_GetParam; }

    public:
        virtual const char* toString() { return "RSU GetParamMsg"; }
};

class CRSUGetParamAckMsg : public CMsgAckBase
{
    public:
        CRSUGetParamAckMsg();
    
    public:
        virtual bool        Decode(PUCHAR pBuffer, UINT nMsgLen);
        virtual bool        Encode(UINT &nMsgLen);

        virtual const char* toString() { return "RSU GetParamAckMsg";}

        bool                EncodeParam(CRSUMsgParam* pParam);
        bool                DecodeParam(CRSUMsgParam* pParam);

        BYTE                GetParamCount() { return m_nParamCount; }
        void                GetParamData(int nIndex, CRSUMsgParam* pParamData);

        int                 SetParamData(CRSUMsgParam* pParamData);

    protected:
        BYTE                m_nParamCount;                              // 需要设置的参数个数
        CRSUMsgParam*       m_MsgParam[MAX_RSU_MSG_PARAM];
};


class CRSUReportStatusMsg : public CMsgBase
{
    public:
        CRSUReportStatusMsg();

    public:
        virtual bool        Decode(PUCHAR pBuffer, UINT nMsgLen);
        virtual bool        Encode(UINT &nMsgLen);

        virtual const char* toString() { return "RSU ReportStatusMsg";}

        void                SetParam(UINT nCPU, UINT nMemory, UINT nHardDisk);

    // Attributes
    protected:
        UINT                m_nCPURate;                                 // CPU 占有率，为0-9999之间的数字，百分比值*1000
        UINT                m_nMemoryRate;
        UINT                m_nHardDiskRate;
};

class CRSUReportStatusAckMsg : public CMsgAckBase
{
    public:
        CRSUReportStatusAckMsg() { m_nMsgType = RSU_MSG_ReportStatusAck; }
    
    public:
        virtual const char* toString() { return "RSU ReportStatusAckMsg";}
};


#endif // _RSU_MESSAGE_H_
