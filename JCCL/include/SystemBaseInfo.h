// ------------------------------------------------------------------------------
// JCCL - Component :: Common
// Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    Common.h
// Purpose: Common Module
//-------------------------------------------------------------------------------

// 在本文件中定义的各种类型、宏等，为所有业务共同使用


#ifndef _SYSTEM_BASE_INFO_
#define _SYSTEM_BASE_INFO_

#include "CString.h"

extern CString              g_strLocalIPAddr;
extern CString              g_strLocalMACAddr;
extern CString              g_strLocalNetMask;
extern CString              g_strLocalNetMask;                          // IP地址对应的子网掩码
extern CString              g_strNetworkName;                           // IP地址对应的网卡名称

// 自动获取系统的IP地址信息
class CSystemBaseInfo : public CObjBase
{
    public:
        CSystemBaseInfo();

    // Operations
    public:
        void                GetSystemLoad(UINT& nCPURate, UINT& nMemoryRate, UINT& nDiskRate);

    protected:
        bool                GetLocalAddr();                             // 获取本机地址
        UINT                GetMemoryRate();                            // 获取内存占有率
        UINT                GetCPURate();                               // 获取CPU使用率
        UINT                GetDiskRate();                              // 获取硬盘使用率
};
extern CSystemBaseInfo g_SystemBaseInfo;

#endif // _SYSTEM_BASE_INFO_
