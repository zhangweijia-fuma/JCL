/*------------------------------------------------------------------------------
 * RSU - Component :: ASN1 PER Coding
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    ASN1PERCoding.h
 * Purpose: ASN.1 PER Encode & Decode base
 *----------------------------------------------------------------------------*/
 
#ifndef _ASN1_PER_CODING_H_
#define _ASN1_PER_CODING_H_

#include "CString.h"

#define MAX_ASN1_MSG_LEN                256                             // ASN1 消息长度，单位：字节
#define CODEC_FAILED                    0xFFFF                          // 解码失败标识 

// 定义ASN1 编解码方法
// 基于PER（压缩编码规则），支持对齐方式或非对齐方式

// 所有整数类型的编解码都按照 long long方式进行

enum CHARACTER_STRING_TYPE
{
    CST_NUMBERIC,
    CST_PRINTABLE,
    CST_NAME,
};

extern bool                bIsAlignmentMode;                            // 对齐方式或非对齐方式标识

class CASN1PerMsgBase : public CObjBase
{
    public:
        CASN1PerMsgBase();

        // virtual const char* toString() = 0;                             // 显示消息名称

        // 编解码成功后，都返回当前的编解码Bit位置
        virtual USHORT      Encode() = 0;
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0) = 0;

        PUCHAR              GetBuffer(USHORT& nBitLen);

    protected:
        virtual bool        EncodeInteger(int nValue, int nMin, int nMax);
        virtual bool        EncodeLong(long long nValue, long long nMin, long long nMax);
        virtual bool        EncodeUINT(UINT nValue, UINT nMin, UINT nMax);
        virtual bool        EncodeUShort(USHORT nValue, USHORT nMin, USHORT nMax);
        virtual bool        EncodeUCHAR(UCHAR nValue, UCHAR nMin, UCHAR nMax);
        virtual bool        EncodeBool(bool bValue);

        virtual bool        EncodeBitString(PUCHAR pData, USHORT nBitLen, USHORT nMinBitLen = 0, USHORT nMaxBitLen = 0);
        virtual bool        EncodeOctString(PUCHAR pData, USHORT nByteLen, USHORT nMinByteLen = 0, USHORT nMaxByteLen = 0);

        virtual bool        EncodeCharacterString(CString strValue, CHARACTER_STRING_TYPE nStrType, USHORT nMinStrLen = 1, USHORT nMaxStrLen = MAX_ASN1_MSG_LEN);
        virtual bool        EncodeEnumerated(UINT nValue, UINT nEnumCount);

        virtual bool        DecodeInteger(int &nValue, int nMin, int nMax);
        virtual bool        DecodeLong(long long &nValue, long long nMin, long long nMax);
        virtual bool        DecodeUINT(UINT &nValue, UINT nMin, UINT nMax);
        virtual bool        DecodeUShort(USHORT &nValue, USHORT nMin, USHORT nMax);
        virtual bool        DecodeUCHAR(UCHAR &nValue, UCHAR nMin, UCHAR nMax);
        virtual bool        DecodeBool(bool &bValue);

        virtual bool        DecodeBitString(PUCHAR pData, USHORT nMaxDataByte, USHORT &nBitLen, USHORT nMinBitLen = 0, USHORT nMaxBitLen = 0);
        virtual bool        DecodeOctString(PUCHAR pData, USHORT nMaxDataByte, USHORT &nByteLen, USHORT nMinByteLen = 0, USHORT nMaxByteLen = 0);
        virtual bool        DecodeCharacterString(CString &strValue, CHARACTER_STRING_TYPE nStrType, USHORT nMinStrLen = 1, USHORT nMaxStrLen = MAX_ASN1_MSG_LEN);
        virtual bool        DecodeEnumerated(UINT &nValue, UINT nEnumCount);

        bool                SetBufferBit(bool nBitValue);               // 设置当前位的值
        bool                GetBufferBit(bool &bValue);                 // 获取当前位的值

        USHORT              m_nBitPos;                                  // 编解码起始位置，初始值为0
        USHORT              m_nBitLen;                                  // 编码后消息长度
        UCHAR               m_Buffer[MAX_ASN1_MSG_LEN];                 // 消息数据，为编解码后的数据
        PUCHAR              m_pBuf;                                     // 消息数据，为编解码后的数据
};

#endif // _ASN1_PER_CODING_H_
