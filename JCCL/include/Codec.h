/*------------------------------------------------------------------------------
 * JCCL - Component :: Message Codec
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    Codec.h
 * Purpose: JCCL module message codec definition
 *----------------------------------------------------------------------------*/
#ifndef _CODEC_H_
#define _CODEC_H_

#include "JCCLTypes.h"
#include "CString.h"
#include "Common.h"

#ifdef QT_CORE_LIB
#include <QString>
#endif

// 实现Long Long类型的字节顺序转换
typedef struct
{
    UINT val1;
    UINT val2;
}ST_ULongLong;

typedef union 
{
    ULONGLONG ullValue;
    ST_ULongLong stullValue;
}U_ULongLong;

// 消息编解码方法定义
class CCodec : public CObjBase
{
    public:
        CCodec();

    // Operations
    public:
        PUCHAR              GetMsgBuffer() { return m_Buffer; };
        UINT                GetMsgLen() { return m_nMsgLength; }

        bool                EncodeUINT(UINT unType, bool bSwap = true);
        bool                DecodeUINT(UINT &unType);

        bool                EncodeInt(int unType, bool bSwap = true);
        bool                DecodeInt(int &nType);

        bool                EncodeString(CString stType);
        bool                DecodeString(CString &stType);

    #ifdef QT_CORE_LIB
        bool                EncodeString(QString strType);
        bool                DecodeString(QString& strType);
    #endif

        bool                EncodeUSHORT(USHORT usType);
        bool                DecodeUSHORT(USHORT& usType);

        bool                EncodeBYTE(BYTE btType);
        bool                DecodeBYTE(BYTE& btType);

        bool                EncodeUCHAR(UCHAR cType);
        bool                DecodeUCHAR(UCHAR &cType);

        bool                EncodeBool(bool bType);
        bool                DecodeBool(bool& bType);

        bool                EncodePUCHAR(PUCHAR pData, USHORT nLen);
        bool                DecodePUCHAR(PUCHAR pData, USHORT nLen);

        bool                EncodeULongLong(ULONGLONG ullType);
        bool                DecodeULongLong(ULONGLONG &ullType);

    protected:
        // 消息编码
        UINT                m_nPos;                                     // 编解码起始位置，初始值为0
        UINT                m_nMsgLength;                               // 编码后消息长度
        UCHAR               m_Buffer[MAX_MSG_LEN];                      // 消息数据，为编解码后的数据
        PUCHAR              m_pBuf;                                     // 消息数据，为编解码后的数据
};

#endif // _CODEC_H_
