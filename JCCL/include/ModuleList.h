/*------------------------------------------------------------------------------
 * JCCL - Component :: Module List definition
 * Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    ModuleList.h
 * Purpose: main module
 *----------------------------------------------------------------------------*/

#ifndef _MODULE_MANAGE_H_
#define _MODULE_MANAGE_H_

#include "JCCLTypes.h"
#include "Common.h"
#include "CString.h"
#include "Debug.h"

#include "CList.h"


#define MAX_MODULE_COUNT                128                             // 最大模块连接数


// 业务模块基本信息
class CModuleInfo : public CObjBase
{
    public:
        CModuleInfo();

        CModuleInfo         operator=(CModuleInfo ModuleInfo);

        void                Reset();
        virtual const char* toString();

    // Attributes
    public:
        CString             m_strPeerModuleCode;                        // 远端模块编号
        UINT                m_nPeerModuleType;                          // 远端模块类型
        CString             m_strPeerModuleAddr;                        // 远端模块的地址
        UINT                m_nPeerModulePort;                          // 远端模块的Socket端口，这个端口可以是作为服务端的端口，也也可以是客户端的的端口
        bool                m_bIsOnline;                                // 和远端的连接是否正常
        UINT                m_nHeartbeatExpireTimes;                    // 和远端的心跳超时次数
};


// 模块列表
class CModuleList : public CObjBase
{
    public:
        CModuleList();

    // Operations
    public:
        void                Reset();

        CModuleList         Find(UINT nModuleType);                     // 根据模块类型查找，返回指定类型的模块列表
        CModuleInfo*        Find(CString strCode);                      // 根据模块编号查找
        CModuleInfo*        Find(CString strAddr, UINT nPort);          // 根据模块地址和端口查找
        CModuleInfo*        FindIdle();
        CModuleInfo*        FindOnline(UINT& nIndex, UINT nModuleType = MODULE_UNKNOWN);

        void                Remove(UINT nModuleType);
        void                Remove(CString strAddr, UINT nPort);


        void                toString(UINT nModuleType = MODULE_UNKNOWN);

        UINT                GetOnlineCount(UINT nModuleType = MODULE_UNKNOWN);

        CModuleInfo*        GetModule(UINT nIndex);
        void                SetModule(CModuleInfo* pModuleInfo);
        void                SetModule(CModuleInfo ModuleInfo);

        CList*              GetModuleList() { return &m_ModuleList; };

        CModuleList         operator=(CModuleList ModuleList);

        bool                CheckHeartbeat();

    // Attributes
    protected:
        CList               m_ModuleList;
};

#endif  // _MODULE_MANAGE_H_
