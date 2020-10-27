// ------------------------------------------------------------------------------
// JCCL - Component :: Common
// Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    Common.cpp
// Purpose: Common Module
//-------------------------------------------------------------------------------

// 为MFC使用，在非MFC环境时stdafx.h为一个空文件
#include "stdafx.h"

#ifdef QT_CORE_LIB
#include <QDebug>
#include <QProcess>
#include <QTextCodec>
#endif

#include "Common.h"
#include "Debug.h"

CString g_strLocalIPAddr;
CString g_strLocalMACAddr;
CString g_strLocalNetMask;                                              // IP地址对应的子网掩码
CString g_strNetworkName;                                               // IP地址对应的网卡名称


char classNameStr[MAX_STR_LEN];
const char* GetClassName(CObjBase* pObj)
{
    if ( pObj == NULL )
        return "NULL object";
    
    CString className = typeid(*pObj).name();
    CString temp;

    int nPos = className.FindFirstChar();
    if ( nPos < 0 )
        temp = className;
    else
        temp= className.Mid(nPos, className.GetLength() - nPos);

    strcpy(classNameStr, temp.GetBuffer());

    return classNameStr;
}



#ifdef QT_CORE_LIB
const char* GetQString(QString str)
{
    char *ch;
    QByteArray ba = str.toUtf8();
    ch = ba.data();
    return ch;
}
#endif

#ifdef QT_CORE_LIB
#define HEX_STR_LEN                     MAX_MSG_LEN * 2 + MAX_STR_LEN
#else
#define HEX_STR_LEN                     MAX_STR_LEN * 2
#endif

#define BIT_STR_LEN                     MAX_STR_LEN * 8


char HexStr[HEX_STR_LEN];

char* ConvertToHex(unsigned char *pData, unsigned int nLen)
{
    char HexChar[4];
    memset(HexStr, 0, HEX_STR_LEN);

    // 16进制数字显示时，首先换行显示，然后每16个数字换行。每行中前8个数字和后8个数字分隔开

    int nIndex = 0, nLine = 0;
    
    strcpy(HexStr, "\n0000: ");

    for(unsigned int i = 0; i < nLen && i < HEX_STR_LEN; i++)
    {
        sprintf(HexChar, "%02X ", *(pData + i ));
        strcat(HexStr, HexChar);
      
        nIndex ++;
        if ( nIndex == 8 )
            strcat(HexStr, "  ");
        if ( nIndex == 16 )
        {
            nLine ++;
            char t[8];
            sprintf(t, "\n%04d: ", nLine*16);
            strcat(HexStr, t);
            nIndex = 0;
        }
    }

    return HexStr;
}

// 取得某一位的值，返回值只能是1或0
bool GetBit(UCHAR nValue, USHORT nBitIndex)
{
    bool bValue = ( nValue & ( 0x01 << nBitIndex ));
    return bValue;
}

// 取得整数的某一位的值，返回值只能是1或0
bool GetBit(int nValue, USHORT nBitIndex)
{
    nValue = nValue >> (( nBitIndex / 8 ) * 8);
    nBitIndex = nBitIndex - ( nBitIndex / 8 ) * 8;
    bool bValue = ( nValue & ( 0x01 << nBitIndex ));
    return bValue;
}

bool GetBit(long long nValue, USHORT nBitIndex)
{
    nValue = nValue >> (( nBitIndex / 8 ) * 8);
    nBitIndex = nBitIndex - ( nBitIndex / 8 ) * 8;
    bool bValue = ( nValue & ( 0x01 << nBitIndex ));
    return bValue;
}

bool GetBit(UINT nValue, USHORT nBitIndex)
{
    nValue = nValue >> (( nBitIndex / 8 ) * 8);
    nBitIndex = nBitIndex - ( nBitIndex / 8 ) * 8;
    bool bValue = ( nValue & ( 0x01 << nBitIndex ));
    return bValue;
}

bool GetBit(USHORT nValue, USHORT nBitIndex)
{
    nValue = nValue >> (( nBitIndex / 8 ) * 8);
    nBitIndex = nBitIndex - ( nBitIndex / 8 ) * 8;
    bool bValue = ( nValue & ( 0x01 << nBitIndex ));
    return bValue;
}

// nBitValue 只能是1或0，所以设置类型位bool类型
void SetBit(UCHAR &nValue, USHORT nBitIndex, bool nBitValue)
{
    // 设置值
    nValue |= ((UINT)nBitValue << nBitIndex);

    return;
}

// 设置一个整数的某一位的值
void SetBit(int &nValue, USHORT nBitIndex, bool nBitValue)
{
    // 设置值
    nValue |= ((UINT)nBitValue << nBitIndex);

    return;
}

void SetBit(long long &nValue, USHORT nBitIndex, bool nBitValue)
{
    // 设置值
    nValue |= (nBitValue << nBitIndex);

    return;
}

void SetBit(UINT &nValue, USHORT nBitIndex, bool nBitValue)
{
    // 设置值
    nValue |= ((UINT)nBitValue << nBitIndex);

    return;
}

void SetBit(USHORT &nValue, USHORT nBitIndex, bool nBitValue)
{
    // 设置值
    nValue |= ((USHORT)nBitValue << nBitIndex);

    return;
}


#define BYTE_SHOW_IN_LINE               8                               // 每一行显示的字节数

char BitStr[BIT_STR_LEN];

// nStartBitPos: 输出开始的Bit位，默认从0位开始
// nByteSequence: 输出时的字节顺序
// 字节从低到高或从高到低输出
char* ConvertToBit(UCHAR *pData, USHORT nBitLen, USHORT nStartBitPos, BIT_STRING_SEQUENCE nByteSequence)
{
    memset(BitStr, 0, BIT_STR_LEN);

    if ( nBitLen >= BIT_STR_LEN || nStartBitPos >= nBitLen)
    {
        LogE("Bit String length exceed limit.");
        return NULL;
    }

    // 在显示输出时，低字节在前，高字节在后
    // 每个字节的低位在后，高位在前
    // 例如：一个2个字节的数据，0b010000001101, 第0个字节是0b01000000，第1个字节是0b1101，
    // 输出为： 010000001101

    int nOutputBytes    = 0;
    int nLineCount      = 0;
    
    // 一行显示的内容
    CString strLine;

    int nOutputBit = 0;

    int nBytes = nBitLen / 8 + 1;
    for ( int i = 0; i <= nBytes; i++)
    {
        // 每个字节的比特串
        CString byteStr;

        if ( nOutputBytes == 0 )
        {
            byteStr.Format("\n%04d ", nLineCount);
            strcat(BitStr, byteStr.GetBuffer());
            byteStr = "";
        }

        UCHAR ucByte = pData[i];

        // 输出字节的每个bit
        for(int j = 0; j < 8 & (j + i * 8) < nBitLen; j++)
        {
            if ( j + i * 8 < nStartBitPos )
            {
                // 如果空格需要显示在前面，就使用 strLine
                // 如果空格需要显示在后面，就使用 byteStr
                strLine += " ";
                // byteStr += " ";
            }
            else
            {
                // 在显示时，低位在后，高位在前
                CString temp;
                temp.Format("%d%s", GetBit(ucByte, j), byteStr.GetBuffer());
                byteStr = temp;
            }
        }

        // 低字节在后，高字节在前
        if ( nByteSequence == BSS_HIGH_LOW )
        {
            CString temp = byteStr;
            temp += " ";
            temp += strLine;
            strLine = temp;
        }
        else
        {
            strLine += byteStr;
            strLine += " ";
        }

        nOutputBytes ++;

        if ( nOutputBytes == BYTE_SHOW_IN_LINE / 2 )
            strLine += " ";

        // 每一行输出8个字节，字节间有空格
        if ( nOutputBytes >= BYTE_SHOW_IN_LINE )
        {
            nOutputBytes = 0;
            nLineCount ++;

            strcat(BitStr, strLine.GetBuffer());
            strLine = "";
        }
    }

    strcat(BitStr, strLine.GetBuffer());

    return BitStr;
}


const char* GetModuleTypeStr(unsigned char nModuleType)
{
    switch(nModuleType)
    {
    case MODULE_VCCU:
        return "VCCU";
    case MODULE_DVCU:
        return "DVCU";
    case MODULE_PECU:
        return "PECU";
    case MODULE_SPCU:
        return "SPCU";
    case MODULE_SIMULATOR:
        return "Simulator";
    case MODULE_TESTER:
        return "Tester";
    case MODULE_PIS_CONTROLLER:
        return "PIS Controller";
    case MODULE_CCU:
        return "TMS CCU";
    case MODULE_DDU:
        return "TMS DDU";
    case MODULE_DCU:
        return "TMS DCU";
    case MODULE_CCDU:
        return "TMS CCDU";
    case MODULE_RSU:
        return "RSU";
    case MODULE_EDGE_SERVER:
        return "Edge Server";
    case MODULE_CLOUD_SERVER:
        return "Cloud Server";
    case MODULE_OBU:
        return "OBU";
    case MODULE_RSU_MANAGER:
        return "RSU Manager";
        
    default:
        return "Unkown Object";
    }
}

// Linux操作

#ifdef _WINDOWS
#include "atltime.h"
char* GetSystemTime(const char* DateConnector, const char* MiddleConnector, const char* timeConnector)
{
    CTime time = CTime::GetCurrentTime();
    CString strTime;
    // 默认格式显示
    strTime = time.Format("%Y-%m-%d %H:%M:%S");

    return strTime.GetBuffer();

}
#endif

#ifndef _WINDOWS

char timeStr[MAX_STR_LEN];
char* GetSystemTime(const char* DateConnector, const char* MiddleConnector, const char* timeConnector)
{
    memset( timeStr, 0, MAX_STR_LEN );
    
    time_t t;
    struct tm * lt;
    time (&t);
    lt = localtime (&t);

    if ( DateConnector == NULL && MiddleConnector == NULL && timeConnector == NULL )
    {
        // 默认格式显示
        sprintf(timeStr, "%04d-%02d-%02d %02d:%02d:%02d", lt->tm_year + 1900, 
            lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
    }
    else if ( DateConnector != NULL && MiddleConnector != NULL && timeConnector != NULL )
    {
        // 指定格式显示
        sprintf(timeStr, "%04d%s%02d%s%02d%s%02d%s%02d%s%02d", lt->tm_year + 1900, 
            DateConnector, lt->tm_mon + 1, DateConnector, lt->tm_mday, MiddleConnector, 
            lt->tm_hour, timeConnector, lt->tm_min, timeConnector, lt->tm_sec);
    }
    else if ( DateConnector == NULL )
    {
        // 只显示时间, 不显示日期
        if ( timeConnector == NULL )
            sprintf(timeStr, "%02d-%02d-%02d", lt->tm_hour, lt->tm_min, lt->tm_sec);
        else
            sprintf(timeStr, "%02d%s%02d%s%02d", lt->tm_hour, timeConnector, lt->tm_min, timeConnector, lt->tm_sec);
    }
    else
    {
        // 只显示日期，不显示时间
        if ( DateConnector == NULL )
            sprintf(timeStr, "%04d-%02d-%02d", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday);
        else
            sprintf(timeStr, "%04d%s%02d%s%02d", lt->tm_year + 1900, DateConnector, lt->tm_mon + 1, DateConnector, lt->tm_mday);
    }

    return timeStr;
}

#endif  // _WINDOWS
