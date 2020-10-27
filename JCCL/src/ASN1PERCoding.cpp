/*------------------------------------------------------------------------------
 * RSU - Component :: ASN1 PER Coding
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    ASN1PERCoding.cpp
 * Purpose: ASN.1 PER Encode & Decode base
 *----------------------------------------------------------------------------*/
 
#include "Debug.h"
#include "Common.h"

#include "ASN1PERCoding.h"

bool bIsAlignmentMode = false;                            // 对齐方式或非对齐方式标识

// 默认词语非对齐方式进行编解码
CASN1PerMsgBase::CASN1PerMsgBase()
{
    m_nBitPos   = 0;
    m_nBitLen   = 0;
    m_pBuf      = m_Buffer;

    memset(m_Buffer, 0, MAX_ASN1_MSG_LEN);
}

PUCHAR CASN1PerMsgBase::GetBuffer(USHORT& nBitLen) 
{
    nBitLen = m_nBitPos;
    // nMsgLen = m_nMsgLength;
    return m_Buffer; 
}

// 设置Bit值，从当前POS设置
// nBitValue 只能是1或0，所以设置类型位bool类型
bool CASN1PerMsgBase::SetBufferBit(bool nBitValue)
{
    // 判读编码总长度是否超过了限制
    if ( (m_nBitPos + 1 ) > MAX_ASN1_MSG_LEN * 8 )
    {
        LogE("Encode length exceed max limitation");
        return false;
    }

    // 取得字节序号
    USHORT nByteIndex = m_nBitPos / 8;

    // 取得Bit序号
    USHORT nBitIndex = m_nBitPos - nByteIndex * 8;

    // 设置值
    SetBit( m_Buffer[nByteIndex], nBitIndex, nBitValue);

    m_nBitPos ++;
    m_nBitLen = m_nBitPos;

    return true;
}

// 整型是协议规范中用得最多的一种数据类型。对这种数据类型编码之前首先要确定其
// 取值范围即上下界之间的数的个数。而编码的长度是根据数的范围来确定的。当 2 m 范
// 围≤2 m+1 时,对该整数取值用 m+1 位二进制表示。但对范围内的数的编码不是对其实值而是
// 对其索引值进行编码编码的索引值是从 0 开始的。
// 例如：
//      TimeslotNumber ::= INTEGER (0..14)
// TimeslotNumber 为整型范围是 014 共 15 个数因为 2^3 <15<2^4, 故编码
// 为 4bit若取值为 7则编码为 0111。
bool CASN1PerMsgBase::EncodeLong(long long nValue, long long nMin, long long nMax)
{
    if ( nValue < nMin || nValue > nMax )
    {
        LogE("EncodeLong value %lld is out of range[%lld, %lld]", nValue, nMin, nMax);
        return false;
    }
    
    // 确定编码值的范围大小
    long long nRange = nMax - nMin;

    // 确定编码长度
    USHORT nBitLen = 0;
    while ( nRange > 0 )
    {
        nRange = nRange / 2;
        nBitLen ++;
    }

    // 获取值的索引序号
    long long nIndex = nValue - nMin;

    // 对取值的索引序号编码
    for(USHORT i = 0; i < nBitLen; i++)
    {
        bool bValue = GetBit(nIndex, i);
        SetBufferBit(bValue);
    }

    return true;
}

bool CASN1PerMsgBase::EncodeInteger(int nValue, int nMin, int nMax)
{
    return EncodeLong(nValue, nMin, nMax);
}

bool CASN1PerMsgBase::EncodeUINT(UINT nValue, UINT nMin, UINT nMax)
{
    return EncodeLong(nValue, nMin, nMax);
}

bool CASN1PerMsgBase::EncodeUShort(USHORT nValue, USHORT nMin, USHORT nMax)
{
    return EncodeLong(nValue, nMin, nMax);
}

bool CASN1PerMsgBase::EncodeUCHAR(UCHAR nValue, UCHAR nMin, UCHAR nMax)
{
    return EncodeLong(nValue, nMin, nMax);
}

// 对布尔型进行编码
// 把布尔型作为整数对待进行编码
bool CASN1PerMsgBase::EncodeBool(bool bValue)
{
    return EncodeLong(bValue, 0, 1);
}

// 对比特串进行编码
// 比特串的值，作为一个整数来考虑
// 比特串的编码分成两种情况
// 长度固定直接将比特串与前面的比特流叠加即可
// 长度可变按前述整型编码的规则，以描述长度范围最少比特的原则，
// 在比特串前面加上用于描述长度所需的比特，然后将待编码的比特串加在后边。
// 例1
// RoutingParameter ::= BIT STRING (SIZE (10))
// 上面的 RoutingParameter 是一个定长且长度为 10 的比特串编码时直接给出 10 位二
// 制数例如编码为‘1111100000’。
// 例 2
// SIB-Data-variable ::= BIT STRING (SIZE (1..214))
// SIB-Data-variable 是一个变长的比特串编码之前首先编长度因为长度的范围为 1 ~ 214，共需要编 8 比特。
// 因此若 SIB-Data-variable 是一个 5 位的比特串时则编码为0000010011111。
// 前面 8 位是比特串长度的编码，后面 5 位是该比特串的值。
// 
// pValue: UCHAR类型的数组，每字节存放的是8位比特串，不足8位补0，由BitLen确定比特串总长度
bool CASN1PerMsgBase::EncodeBitString(PUCHAR pData, USHORT nBitLen, USHORT nMinBitLen, USHORT nMaxBitLen)
{
    if ( nMaxBitLen < nMinBitLen || nMinBitLen < 0 || nBitLen < nMinBitLen)
    {
        LogE("EncodeBitString parameter BitLen: %d, MinBitLen: %d, MaxBitLen: %d", nBitLen, nMinBitLen, nMaxBitLen);
        return false;
    }

    if ( nMaxBitLen > 0)
    {
        // 不定长比特串，需要对比特串的长度进行编码
        if ( EncodeUShort(nBitLen, nMinBitLen, nMaxBitLen) == false )
            return false;
    }

    // 定长比特串，或不定长的比特串，对比特串进行编码
    // 从高位向低位进行编码，显示的时候也是高位在前，低位在后
    USHORT nBytes = nBitLen / 8 + 1;
    for ( USHORT i = 0; i < nBytes; i++)
    {
        USHORT ucByte = pData[i];
        for(USHORT j = 0; j < 8 & (j + i * 8) < nBitLen; j++)
        {
            bool bValue = GetBit(ucByte, j);
            SetBufferBit(bValue);
        }
    }

    return true;
}

// 八位组串编码的原则与比特串相同。分成长度可变的八位串和长度不可变的八位串两种。
// 与比特串编码不同的是编码的值是八位串而不是比特串。
// 例
// VALUE = OCTETSTRINGSIZE1..8
// 若 VALUE 值是长度为 3 的八位串则编码为010后面跟着 3 个因为长度区间为八，
// 所以长度编码 L 为 3 位而 3b min =2。因此编码为 010。后面则跟着 3 个八位组，
// 如：00000000 11111111 00000000 。
bool CASN1PerMsgBase::EncodeOctString(PUCHAR pData, USHORT nByteLen, USHORT nMinByteLen, USHORT nMaxByteLen)
{
    if ( nMaxByteLen < nMinByteLen || nMinByteLen < 0 )
    {
        LogE("EncodeOctString parameter nMinByteLen: %d, nMaxByteLen: %d", nMinByteLen, nMaxByteLen);
        return false;
    }

    if ( nMaxByteLen > 0)
    {
        // 不定长比特串，需要对比特串的长度进行编码
        if ( EncodeUShort(nByteLen, nMinByteLen, nMaxByteLen) == false )
            return false;
    }

    // 定长比特串，或不定长的比特串，对比特串进行编码
    // 从高位向低位进行编码，显示的时候也是高位在前，低位在后
    for ( USHORT i = 0; i < nByteLen; i++)
    {
        USHORT ucByte = pData[i];
        for(USHORT j = 0; j < 8; j++)
        {
            bool bValue = GetBit(ucByte, j);
            SetBufferBit(bValue);
        }
    }

    return true;
}

CString NumbericString  = "0123456789";
CString PrintableString = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ,()+`-/.=?:";
CString NameString      = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

// 字符串的编码方式有多种情况的编码方式与 BITSTRING 和 OCTETSTRING 的编码方式类似，
// 只是对不同类型的字符串有不同的编码方法。
// 下面将着重说明 CHARACTER STRINGS 中常用的 NumbericString、PrintableString、NameString 的编码方法
// 1. NumbericString 类型中成员为 0~9，共十个数。因此在编码时长度可定为4bit(2 3 <10<2 4 ).
// 因此此种类型的串编码是每个数字编为 4 比特。
// 同理，PrintableString 中包含的字符为“a..z”、“A..Z”、 “0..9”、“space”、“”、“”、
// “”“”、“、”、“-”、“/”、“.”、“”、“”、“”共 74 个字符每个字母应编码为 7 比特。
// NameString 共 52 个大小写字母每个字母编码则为 6bit。
// 2. NumbericString 123 的编码为：0001 0010 0011。
// 3. NameString “cat”编码为：011100 011010 100010，上面每个字母编 6 比特，每个字母的值是其在 NameString 中的索引值。
bool CASN1PerMsgBase::EncodeCharacterString(CString strValue, CHARACTER_STRING_TYPE nStrType, USHORT nMinStrLen, USHORT nMaxStrLen)
{
    PCSTR pString = strValue.GetBuffer();

    // 首先编码长度
    if ( EncodeInteger(strValue.GetLength(), nMinStrLen, nMaxStrLen) == false )
    {
        LogE("EncodeCharacterString: Encode string length failed.");
        return false;
    }

    for(int i = 0; i < strValue.GetLength(); i++)
    {
        char cValue = *(pString + i );
        int nIndex = -1;
        CString *pString = NULL;

        if(nStrType == CST_NUMBERIC)
        {
            pString = &NumbericString;
        }
        else if ( nStrType == CST_PRINTABLE)
        {
            pString = &PrintableString;
        }
        else if ( nStrType == CST_NAME)
        {
            pString = &NameString;
        }
        else
        {
            LogE("EncodeCharacterString: String type %d", nStrType);
            return false;
        }

        nIndex = pString->Find(cValue);
        if ( nIndex == -1 )
        {
            LogE("EncodeCharacterString: Can not find charactor %c in %s", cValue, pString->GetBuffer());
            return false;
        }

        if (EncodeInteger(nIndex, 0, pString->GetLength())== false)
        {
            LogE("EncodeCharacterString: encode integer failed.");
            return false;
        }
    }

    return true;
}

// 枚举型的编码类似于整型也是以描述枚举内全体成员所需比特数最少的原则进行编码。
// 其值的范围从 0 开始。枚举类型编码时，只对其项的索引值编码。
// 例
//     Modulation = ENUMERATED
//     {
//         mod-QPSK ,
//         mod-8PSK
//     }
// 上面是一个 Modulation 是枚举类型，编码时，首先看枚举项的个数为 2，2^1=2.
// 因此编码为 1bit。当选 mod-QPSK 调制模式时编码为 0，选 mod-8PSK 调制模式时编码为 1。
bool CASN1PerMsgBase::EncodeEnumerated(UINT nValue, UINT nEnumCount)
{
    return EncodeLong(nValue, 0, nEnumCount);
}

// 选择型的编码类似于整型，以最少的比特来描述选择体内各成员的索引值。值的范围从0开始。
// 与枚举不同的是，编完 CHOICE 的索引值，紧接着就应编码索引值对应的单元。
// 而枚举型将索引值编完后就结束。下面将以一个例子对这种类型的编码进行说明
// 例
//     NumberOfTransportBlocks::= CHOICE
//     {
//         zero NULL,
//         one NULL,
//         small INTEGER (2..17),
//         large INTEGER (18..512)
//     }
// NumberOfTransportBlocks 是一个选择类型,若信息传输块的个数为 16, 则应该选择CHOICE 的 small 项，
// 然后在对该项值进行编码。
// 首先 CHOICE 里有 4 项，编码为 2bit。small为第 3 项编码值为 2，选择项编码为10。
// 然后再对 small 项编码。因为取值为 16，索引值为 14。整数范围是 2~17，共 16 个数，要编 4 比特。
// 因此要对索引值14编4比特，即编码为：1110。

// 因为Choice类型涉及到不同类型的值，基类无法确定类型，因此其编码由消息自身完成
// 因为Sequence类型涉及到不同类型的值，基类无法确定类型，因此其编码由消息自身完成


// 获得当前位的值
bool CASN1PerMsgBase::GetBufferBit(bool &bValue)
{
    if ( m_nBitPos > m_nBitLen )
    {
        LogE("CASN1PerMsgBase GetBufferBit BitPos %d is out of max data buffer range [0, %d]!",
            m_nBitPos, m_nBitLen);
        return false;
    }

    USHORT nByteIndex   = m_nBitPos / 8;
    USHORT nBitIndex    = m_nBitPos - nByteIndex * 8;

    UCHAR ucByte = *(m_pBuf + nByteIndex);
    bValue = GetBit(ucByte, nBitIndex);
    m_nBitPos ++;

    return true;
}

// 对整数进行解码
// 解码过程和编码相反
bool CASN1PerMsgBase::DecodeLong(long long &nValue, long long nMin, long long nMax)
{
    nValue = 0;

    // 确定编码值的范围大小
    long long nRange = nMax - nMin;

    // 确定编码长度
    USHORT nBitLen = 0;
    while ( nRange > 0 )
    {
        nRange = nRange / 2;
        nBitLen ++;
    }

    for(USHORT i = 0; i < nBitLen; i++)
    {
        bool bValue;
        if ( GetBufferBit(bValue) == false )
        {
            LogE("DecodeLong Get Buffer Bit failed.");
            return false;
        }

        SetBit(nValue, i, bValue);
    }

    nValue += nMin;

    if ( nValue > nMax || nValue < nMin )
    {
        LogE("DecodeLong result %lld is out of range [%lld, %lld]", nValue, nMin, nMax);
        return false;
    }

    return true;
}

bool CASN1PerMsgBase::DecodeInteger(int &nValue, int nMin, int nMax)
{
    long long temp = 0;
    bool bFlag = DecodeLong(temp, nMin, nMax);
    nValue = temp;
    return bFlag;
}

bool CASN1PerMsgBase::DecodeUINT(UINT &nValue, UINT nMin, UINT nMax)
{
    long long temp = 0;
    bool bFlag = DecodeLong(temp, nMin, nMax);
    nValue = temp;
    return bFlag;
}


bool CASN1PerMsgBase::DecodeUCHAR(UCHAR &nValue, UCHAR nMin, UCHAR nMax)
{
    long long temp = 0;
    bool bFlag = DecodeLong(temp, nMin, nMax);
    nValue = temp;
    return bFlag;
}

bool CASN1PerMsgBase::DecodeUShort(USHORT &nValue, USHORT nMin, USHORT nMax)
{
    long long temp = 0;
    bool bFlag = DecodeLong(temp, nMin, nMax);
    nValue = temp;
    return bFlag;
}

// 对Bool值进行解码
bool CASN1PerMsgBase::DecodeBool(bool &bValue)
{
    long long nValue = 0;
    if ( DecodeLong(nValue, 0, 1) == false)
    {
        LogE("Decode bool failed.");
        return false;
    }

    if ( nValue == 0 )
        bValue = false;
    else if ( nValue == 1)
        bValue = true;
    else
    {
        LogE("Decode bool failed.");
        return false;        
    }

    return true;
}

// 解码比特串
bool CASN1PerMsgBase::DecodeBitString(PUCHAR pData, USHORT nMaxDataByte, USHORT &nBitLen, USHORT nMinBitLen, USHORT nMaxBitLen)
{
    if ( nMaxBitLen < nMinBitLen || nMinBitLen < 0 )
    {
        LogE("DecodeBitString parameter nMinBitLen: %d, nMaxBitLen: %d", nMinBitLen, nMaxBitLen);
        return false;
    }

    if ( nMaxBitLen > 0)
    {
        // 不定长比特串，需要对比特串的长度进行解码
        if ( DecodeUShort(nBitLen, nMinBitLen, nMaxBitLen) == false )
        {
            LogE("DecodeBitString: Decode BitString length failed.");
            return false;
        }
    }
    else
    {
        // 定长比特串，nBitLen为输入参数
    }

    USHORT nBytes = nBitLen / 8;

    if ( nBytes > nMaxDataByte )
    {
        LogE("DecodeBitString: BitString length %d exceed buffer length %d.", nBytes, nMaxDataByte);
        return false;
    }

    // 定长比特串，或不定长的比特串，对比特串进行编码
    for(USHORT i = 0; i < nBitLen; i++)
    {
        bool bValue;
        if ( GetBufferBit(bValue) == false )
        {
            LogE("DecodeBitString GetBufferBit failed.");
            return false;
        }

        USHORT nByteIndex = i / 8;

        USHORT nBitIndex  = i - nByteIndex * 8;
        SetBit( *(pData + nByteIndex), nBitIndex, bValue);
    }

    return true;
}

// 解码八位串
bool CASN1PerMsgBase::DecodeOctString(PUCHAR pData, USHORT nMaxDataByte, USHORT &nByteLen, USHORT nMinByteLen, USHORT nMaxByteLen)
{
    if ( nMaxByteLen < nMinByteLen || nMinByteLen < 0 )
    {
        LogE("DecodeOctString parameter nMinByteLen: %d, nMaxByteLen: %d", nMinByteLen, nMaxByteLen);
        return false;
    }

    if ( nMaxByteLen > 0)
    {
        // 不定长8位串，需要对8位串的长度进行解码
        USHORT nIndex = 0;
        if ( DecodeUShort(nIndex, nMinByteLen, nMaxByteLen) == false )
        {
            LogE("DecodeOctString: Decode OctString length failed: %d.", nIndex);
            return false;
        }

        nByteLen = nIndex + nMinByteLen;
    }

    if ( nByteLen > nMaxDataByte )
    {
        LogE("DecodeOctString: OctString length %d exceed buffer length %d.", nByteLen, nMaxDataByte);
        return false;
    }

    // 定长比特串，或不定长的比特串，对比特串进行解码
    for ( USHORT i = 0; i < nByteLen; i++)
    {
        for(USHORT j = 0; j < 8; j++)
        {
            bool bValue;
            if ( GetBufferBit(bValue) == false )
            {
                LogE("DecodeOctString GetBufferBit failed.");
                return false;
            }
            SetBit( *(pData + i), j, bValue);
        }
    }

    return true;
}

// 解码字符串
bool CASN1PerMsgBase::DecodeCharacterString(CString &strValue, CHARACTER_STRING_TYPE nStrType, USHORT nMinStrLen, USHORT nMaxStrLen)
{
    CString *pString = NULL;

    if(nStrType == CST_NUMBERIC)
    {
        pString = &NumbericString;
    }
    else if ( nStrType == CST_PRINTABLE)
    {
        pString = &PrintableString;
    }
    else if ( nStrType == CST_NAME)
    {
        pString = &NameString;
    }
    else
    {
        LogE("DecodeCharacterString: String type %d", nStrType);
        return false;
    }

    // 首先解码字符串长度
    int nStrLen = 0;
    if (DecodeInteger(nStrLen, nMinStrLen, nMaxStrLen) == false)
    {
        LogE("DecodeCharacterString: Decode string length failed.");
        return false;
    }

    for(int i = 0; i < nStrLen; i++)
    {
        long long nIndex = 0;
        if (DecodeLong(nIndex, 0, pString->GetLength())== false)
        {
            LogE("DecodeCharacterString: Decode failed.");
            return false;
        }

        char cValue = 0;
        cValue = pString->GetAt(nIndex);
        if ( cValue == 0 )
        {
            LogE("Can not find decoded character  %c from %s. ", cValue, pString->GetBuffer());
            return false;
        }

        strValue += cValue;
    }

    return true;
}


bool CASN1PerMsgBase::DecodeEnumerated(UINT &nValue, UINT nEnumCount)
{
    return DecodeUINT(nValue, 0, nEnumCount);
}

