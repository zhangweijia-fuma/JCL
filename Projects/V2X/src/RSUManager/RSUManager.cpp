/*------------------------------------------------------------------------------
 * RSU - Component :: RSUManager
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    RSUManager.cpp
 * Purpose: RSUManager module, service flow realization
 *----------------------------------------------------------------------------*/

#include "Common.h"
#include "Timer.h"
#include "Config.h"

#include "RSUMessage.h"

#include "RSUManager.h"

CRSUManager::CRSUManager()
{
    m_nModuleType   = MODULE_RSU_MANAGER;
    m_strModuleCode = "RSUManager001";
    m_strModuleName = "Fuma AI V2X RSU Manager Server";
    m_strLocalAddr  = g_strLocalIPAddr;
}

// Initial VCCU module
bool CRSUManager::InitialSystem()
{
    LogI("Initial %s...", m_strModuleName.GetBuffer());

    CheckUpdate();

    if ( LoadConfig() == false )
    {
        return false;
    }

    // 创建服务端
    CMsgServerBase *pServer = new CMsgServerBase();

    if ( pServer->Initial(m_strModuleCode, m_nModuleType, m_nCloudPort, SOCK_DGRAM, CModuleBase::OnMessage, this) == false )
        return false;

    m_ServerList.AddTail(pServer);

    LogI("Create Cloud Server Message server successfully.");

    LogI("Initial %s(ID: %s) successfully...", m_strModuleName.GetBuffer(), m_strModuleCode.GetBuffer());

    return true;
}

bool CRSUManager::LoadConfig()
{
    // 加载默认配置参数
    CString strCfgFile = g_strAppRootPath;

    strCfgFile = strCfgFile + "/conf/v2x.conf";

    CConfig config(strCfgFile);

    CString strSection = "COMMON";

    CString strValue, strKey;
    int nValue;

    // RSUManager服务器的信息
    strKey = "RSUManagerAddr";
    strValue = config.GetConfigStr(strSection, strKey, "");
    if ( nValue == 0)
    {
        LogE("Error: Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }
    m_strLocalAddr = strValue;

    strKey = "RSUManagerPort";
    nValue = config.GetConfigInt(strSection, strKey, 0);
    if ( nValue == 0)
    {
        LogE("Error: Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }
    m_nCloudPort = nValue;

    return true;
}

bool CRSUManager::StartSystem()
{
    LogI("%s Starting...", m_strModuleName.GetBuffer());
    
    if ( CModuleBase::StartSystem() == false )
    {
        return false;
    }

    UINT nCount = 0;
    while(1)
    {
        int nLoopTimes = 30 * 60;

        // 250,000,000: 基本上就是1秒左右的时间，这和主频及当时负荷状况有关
        for(int i = 0; i < 250000000; i++)
            for(int j = 0; j < nLoopTimes; j++);

        LogD("%s loop times: %d.",m_strModuleName.GetBuffer(), ++ nCount);
        if ( nCount >= 0x0FFFFFF0)
            nCount = 0;
    }
    
    return true;
}

// 解析消息
void CRSUManager::DecodeMessage(PUCHAR pBuffer, UINT nMsgLen, LPCSTR strSrcAddr, UINT nSrcPort)
{
    CRSUMsgBase msg;
    CMsgBase* pMsg = NULL;

    // 接收到的消息
    CHeartBeatMsg                        HeartbeatMsg;
    CRSUInitialRequestMsg                RSUInitialRequestMsg;

    UINT nMsgType = msg.DecodeMsgType(pBuffer, nMsgLen);
    switch(nMsgType)
    {
    case MSG_HEARTBEAT:
        pMsg = &HeartbeatMsg;
        break;
    case RSU_MSG_InitialRequest:
        pMsg = &RSUInitialRequestMsg;
        break;

    default:
        LogE("Error: Receive unsupport msg %d, %s", nMsgType, ConvertToHex(pBuffer, nMsgLen));
        return;
    }

    if ( pMsg->Decode(pBuffer, nMsgLen) == false )
    {
        LogE("Decode message %s error.", pMsg->toString());
        LogI(ConvertToHex(pBuffer, nMsgLen));
        
        return;
    }

    pMsg->SetSrcAddr(strSrcAddr);
    pMsg->SetSrcPort(nSrcPort);

    if ( pMsg->GetMsgType() != MSG_HEARTBEAT )
        LogI("Receive %s from %s(%s:%d)", pMsg->toString(), pMsg->GetSrcModuleName(), strSrcAddr, nSrcPort);
    else
        LogDI("Receive %s from %s(%s:%d)", pMsg->toString(), pMsg->GetSrcModuleName(), strSrcAddr, nSrcPort);

    ProcessMessage(pMsg);    
}

// Process Message
void CRSUManager::ProcessMessage(CMsgBase* pMsg)
{
    if ( pMsg == NULL )
    {
        LogE("RSU Manager Receive a NULL message");
        return;
    }

    switch ( pMsg->GetMsgType() )
    {
    case MSG_HEARTBEAT:
        ProcessHeartbeatMsg(pMsg);
        break;
    case RSU_MSG_InitialRequest:
        ProcessInitialRequestMsg(pMsg);
        break;
    default:
        LogE("Receive unsupport message %s from %s:%d", pMsg->toString(), pMsg->GetSrcAddr(), pMsg->GetSrcPort());
        return;
    }

    return;
}


// 接收来自RSU或边缘服务器的初始化请求消息
bool CRSUManager::ProcessInitialRequestMsg(CMsgBase* pMsg)
{
    if ( CModuleBase::ProcessInitialRequestMsg(pMsg) == false )
        return false;

    if( pMsg->GetSrcModuleType() == MODULE_RSU)
    {
        // CRSUInitialRequestMsg* pInitialRequestMsg = (CRSUInitialRequestMsg*)pMsg;
        // 处理RSU的初始化请求消息

        // 发送初始化请求响应消息
        CRSUInitialRequestAckMsg msg;

        msg.SetDestAddr(pMsg->GetSrcAddr());
        msg.SetDestPort(pMsg->GetSrcPort());
        msg.SetDestModuleType(pMsg->GetSrcModuleType());
        msg.SetSrcPort(pMsg->GetDestPort());

        SendMessageToClient(&msg);
    }
    else
    {
        LogE("Error: Get Initial Request Message from unkown module %d.", pMsg->GetSrcModuleType());
    }

    return true;
}
