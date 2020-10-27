/*------------------------------------------------------------------------------
 * JCCL - Component :: Module List definition
 * Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    ModuleList.cpp
 * Purpose: main module
 *----------------------------------------------------------------------------*/

#include "ModuleList.h"
#include "MsgBase.h"

CModuleInfo::CModuleInfo()
{
    Reset();
}

CModuleInfo CModuleInfo::operator=(CModuleInfo ModuleInfo)
{
    m_nHeartbeatExpireTimes     = ModuleInfo.m_nHeartbeatExpireTimes;
    m_nPeerModuleType           = ModuleInfo.m_nPeerModuleType;
    m_strPeerModuleCode         = ModuleInfo.m_strPeerModuleCode;
    m_strPeerModuleAddr         = ModuleInfo.m_strPeerModuleAddr;
    m_nPeerModulePort           = ModuleInfo.m_nPeerModulePort;
    m_bIsOnline                 = ModuleInfo.m_bIsOnline;

    return (*this);
}

void CModuleInfo::Reset()
{
    m_nHeartbeatExpireTimes     = 0;
    m_nPeerModuleType           = MODULE_UNKNOWN;
    m_strPeerModuleCode         = "";
    m_strPeerModuleAddr         = "";
    m_nPeerModulePort           = INVALID_SOCKET_PORT;
    m_bIsOnline                 = false;
}

char strModuleInfo[MAX_STR_LEN];
const char* CModuleInfo::toString()
{
    sprintf(strModuleInfo, "Module Info: %s: %s:%d, %s.", m_strPeerModuleCode.GetBuffer(), m_strPeerModuleAddr.GetBuffer(), m_nPeerModulePort, (m_bIsOnline)?"Online":"offline");

    return strModuleInfo;
}


// /////////////////////////////////////////////////////////////////////
CModuleList::CModuleList()
{
    // 在删除节点时同时删除对象
    m_ModuleList.SetMode(true);
    Reset();
};


void CModuleList::Reset()
{
    m_ModuleList.RemoveAll();
}

CModuleList CModuleList::Find(UINT nModuleType)
{
    CModuleList moduleList;
    CModuleInfo* pModuleInfo = (CModuleInfo*)m_ModuleList.GetHead();
    while ( pModuleInfo != NULL )
    {
        if ( pModuleInfo->m_nPeerModuleType == nModuleType && pModuleInfo->m_bIsOnline == true )
            moduleList.SetModule(pModuleInfo);

        pModuleInfo = (CModuleInfo*)m_ModuleList.GetNext(pModuleInfo);
    }

    return moduleList;
}

CModuleInfo* CModuleList::Find(CString strCode)
{
    CModuleInfo* pModuleInfo = (CModuleInfo*)m_ModuleList.GetHead();
    while ( pModuleInfo != NULL )
    {
        if ( pModuleInfo->m_strPeerModuleCode == strCode && pModuleInfo->m_bIsOnline == true )
            return pModuleInfo;

        pModuleInfo = (CModuleInfo*)m_ModuleList.GetNext(pModuleInfo);
    }

    return NULL;
}

// 根据模块地址和端口查找
// 通常情况下，根据地址查找即可，但是在开发测试时，服务端、客户端可能都在一台虚拟机环境中运行，造成多个模块使用同一个地址，因此增加了端口要求
CModuleInfo* CModuleList::Find(CString strAddr, UINT nPort)
{
    CModuleInfo* pModuleInfo = (CModuleInfo*)m_ModuleList.GetHead();
    while ( pModuleInfo != NULL )
    {
        if ( pModuleInfo->m_strPeerModuleAddr == strAddr && pModuleInfo->m_nPeerModulePort == nPort && pModuleInfo->m_bIsOnline == true )
            return pModuleInfo;

        pModuleInfo = (CModuleInfo*)m_ModuleList.GetNext(pModuleInfo);
    }

    return NULL;
}

CModuleInfo* CModuleList::FindIdle()
{
    CModuleInfo* pModuleInfo = NULL;
    pModuleInfo = new CModuleInfo();
    if ( pModuleInfo != NULL )
    {
        m_ModuleList.AddTail(pModuleInfo);
    }

    return pModuleInfo;
}

// 查找指定类型的在线模块
CModuleInfo* CModuleList::FindOnline(UINT& nIndex, UINT nModuleType)
{
    CModuleInfo* pModuleInfo = (CModuleInfo*)m_ModuleList.GetAt(nIndex);
    while ( pModuleInfo != NULL )
    {
        nIndex ++;
        // 指定查找的模块类型没有限制
        if ( pModuleInfo->m_bIsOnline == true && nModuleType == MODULE_UNKNOWN )
            return pModuleInfo;

        else if ( pModuleInfo->m_bIsOnline == true && nModuleType == pModuleInfo->m_nPeerModuleType)
            return pModuleInfo;

        pModuleInfo = (CModuleInfo*)m_ModuleList.GetNext(pModuleInfo);
    }

    return NULL;
}

// 删除指定地址和端口的业务模块
void CModuleList::Remove(CString strAddr, UINT nPort)
{
    CModuleInfo* pModuleInfo = Find(strAddr, nPort);
    if ( pModuleInfo != NULL )
    {
        m_ModuleList.Remove(pModuleInfo);
    }

    return;
}


// 删除指定类型的业务模块
// 如未指定业务模块类型，则删除所有业务模块信息
void CModuleList::Remove(UINT nModuleType)
{
    if ( nModuleType == MODULE_UNKNOWN )
    {
        m_ModuleList.RemoveAll();
        return;
    }

    CModuleInfo* pModuleInfo = (CModuleInfo*)m_ModuleList.GetHead();
    while ( pModuleInfo != NULL )
    {
        if ( pModuleInfo->m_nPeerModuleType == nModuleType )
        {
            m_ModuleList.Remove(pModuleInfo);
            pModuleInfo = (CModuleInfo*)m_ModuleList.GetHead();
        }
        else
            pModuleInfo = (CModuleInfo*)m_ModuleList.GetNext(pModuleInfo);
    }

    return;
}


void CModuleList::toString(UINT nModuleType)
{
    LogD("Type: %s, online count: %d", GetModuleTypeStr(nModuleType), GetOnlineCount());
    
    CModuleInfo* pModuleInfo = (CModuleInfo*)m_ModuleList.GetHead();
    while ( pModuleInfo != NULL )
    {
        if ( pModuleInfo->m_bIsOnline == false )
            continue;

        if ( nModuleType == MODULE_UNKNOWN || nModuleType == pModuleInfo->m_nPeerModuleType)
            LogD("%s", pModuleInfo->toString());
    }
}

// 查找在线业务模块数量
// 如果未指定模块类型，则返回所有在线模块数量
// 如果指定了模块类型，则返回指定模块类型的在线数量
UINT CModuleList::GetOnlineCount(UINT nModuleType)
{
    UINT nCount = 0;
    CModuleInfo* pModuleInfo = (CModuleInfo*)m_ModuleList.GetHead();
    while ( pModuleInfo != NULL )
    {
        if ( pModuleInfo->m_bIsOnline == true && (pModuleInfo->m_nPeerModuleType == nModuleType || nModuleType == MODULE_UNKNOWN))
            nCount ++;

        pModuleInfo = (CModuleInfo*)m_ModuleList.GetNext(pModuleInfo);
    }

    return nCount;
}

CModuleInfo* CModuleList::GetModule(UINT nIndex)
{
    return (CModuleInfo*)m_ModuleList.GetAt(nIndex);
}

void CModuleList::SetModule(CModuleInfo* pModuleInfo)
{
    m_ModuleList.AddTail(pModuleInfo);
}

void CModuleList::SetModule(CModuleInfo ModuleInfo)
{
    m_ModuleList.AddTail(&ModuleInfo);
}

CModuleList CModuleList::operator=(CModuleList ModuleList)
{
    Reset();

    CModuleInfo* pModuleInfo = (CModuleInfo*)m_ModuleList.GetHead();
    while ( pModuleInfo != NULL )
    {
        SetModule(pModuleInfo);
        pModuleInfo = (CModuleInfo*)m_ModuleList.GetNext(pModuleInfo);
    }

    return *this;
}

// 检查心跳超时次数
// 返回值：bool，为true时表示有模块失去连接，需要更新模块列表
bool CModuleList::CheckHeartbeat()
{
    LogD("Online Module Count: %d", GetOnlineCount());

    bool bIsLost = false;
    // Check all module online status
    
    CModuleInfo* pModuleInfo = (CModuleInfo*)m_ModuleList.GetHead();
    while ( pModuleInfo != NULL )
    {
        if ( pModuleInfo->m_bIsOnline == false )
        {
            pModuleInfo = (CModuleInfo*)m_ModuleList.GetNext(pModuleInfo);
            continue;
        }

        pModuleInfo->m_nHeartbeatExpireTimes ++;
        LogD("Module %s(%s) Heartbeat expire times: %d", GetModuleTypeStr(pModuleInfo->m_nPeerModuleType), 
                pModuleInfo->m_strPeerModuleAddr.GetBuffer(), pModuleInfo->m_nHeartbeatExpireTimes);

        if ( pModuleInfo->m_nHeartbeatExpireTimes <= MSG_EXPIRE_TIMES )
        {
            pModuleInfo = (CModuleInfo*)m_ModuleList.GetNext(pModuleInfo);
            continue;
        }

        // Module lost connection
        LogE("Module %s(%s:%d) lost connection.", GetModuleTypeStr(pModuleInfo->m_nPeerModuleType), 
            pModuleInfo->m_strPeerModuleAddr.GetBuffer(), pModuleInfo->m_nPeerModulePort);

        m_ModuleList.Remove(pModuleInfo);
        pModuleInfo = (CModuleInfo*)m_ModuleList.GetHead();

        bIsLost = true;
    }

    return bIsLost;
}
