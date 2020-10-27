// ------------------------------------------------------------------------------
// RSU - Component :: RSU Message Base
// Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    RSUMsgBase.h
// Purpose: RSU-Common Module
//-------------------------------------------------------------------------------

#ifndef _RSU_MSGBASE_H_
#define _RSU_MSGBASE_H_

#include "Debug.h"
#include "MsgBase.h"
#include "RSUTypes.h"

#define SERVER_UPDATE_PATH              "/Fuma/V2X/update"              // 系统更新文件路径
#define SERVER_LOG_PATH                 "/Fuma/V2X/log"                 // 日志文件在服务器上的路径

// RSU Message 消息类型，用于和RSU Manager之间的消息类型
enum RSU_MSG
{
    RSU_MSG_TYPE_BASE       = 0x0300,

    RSU_MSG_InitialRequest,
    RSU_MSG_InitialRequestAck,
    RSU_MSG_UpdateSoftware,
    RSU_MSG_UpdateSoftwareAck,
    RSU_MSG_Reboot,
    RSU_MSG_RebootAck,
    RSU_MSG_SetParam,
    RSU_MSG_SetParamAck,
    RSU_MSG_GetParam,
    RSU_MSG_GetParamAck,

    RSU_MSG_ReportStatus,
    RSU_MSG_ReportStatusAck,

};


#define MAX_RSU_MSG_PARAM               120                             // RSU可设置的最多参数数量

// 参数Tag列表
enum RSU_PARAM_TAG
{
    Tag_RSUID = 0,
    Tag_longtitude,
    Tag_Latitude,
    Tag_Temperature,
    Tag_Humidity,

    TAG_Invalid = MAX_RSU_MSG_PARAM,
};


// RSU消息基类，所有RSU消息从这里进行继承
class CRSUMsgBase : public CMsgBase
{
    public:
        CRSUMsgBase();

    // Operations
    public:
        // USHORT              DecodeMsgType(PUCHAR pBuffer, UINT nMsgLen);
        virtual bool        Decode(PUCHAR pBuffer, UINT nMsgLen);
        virtual bool        Encode(UINT &nMsgLen);
        virtual const char* toString() { return "RSUMsgBase";}
};


enum RSU_PARAM_TYPE
{
    RPT_String = 0,                                                     // 字符串类型参数
    RPT_UINT,
    RPT_INT,
    RPT_SHORT,
    RPT_USHORT,
    RPT_BYTE,

    RPT_Invalid,
};

class CRSUMsgParam : public CObjBase
{
    public:
        CRSUMsgParam();

    public:
        BYTE                GetParamTag() { return m_nParamTag; }
        RSU_PARAM_TYPE      GetDataType() { return m_RSUParamType; }

        int                 GetIntData() { return m_nIntData; }
        UINT                GetUINTData() { return m_nUINTData; }
        BYTE                GetByteData() { return m_nByteData; }
        USHORT              GetUshortData() { return m_nUSHORTData; }
        short               GetShortData() { return m_nShortData; }
        CString             GetStringData() { return m_strData; }

        void                SetParamData(BYTE nTag, int nParam);
        void                SetParamData(BYTE nTag, UINT nParam);
        void                SetParamData(BYTE nTag, BYTE nParam);
        void                SetParamData(BYTE nTag, USHORT nParam);
        void                SetParamData(BYTE nTag, short nParam);
        void                SetParamData(BYTE nTag, CString strParam);

    protected:
        BYTE                m_nParamTag;                                // 参数的Tag

        int                 m_nIntData;
        UINT                m_nUINTData;
        BYTE                m_nByteData;
        USHORT              m_nUSHORTData;
        short               m_nShortData;
        CString             m_strData;

        RSU_PARAM_TYPE      m_RSUParamType;
};


#endif // _RSU_MSGBASE_H_
