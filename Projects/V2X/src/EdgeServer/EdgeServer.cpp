/*------------------------------------------------------------------------------
 * RSU - Component :: EdgeServer
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    EdgeServer.cpp
 * Purpose: EdgeServer module, service flow realization
 *----------------------------------------------------------------------------*/

#include "Common.h"
#include "Timer.h"
#include "Config.h"

#include "RSUMessage.h"

#include "EdgeServer.h"

CEdgeServer::CEdgeServer()
{
    m_strModuleName             = "JamesSoft V2X EdgeServer System";

    m_nModuleType               = MODULE_EDGE_SERVER;
    m_strModuleCode             = "EdgeServer001";
 
    m_nEdgeServerPort           = INVALID_SOCKET_PORT;
    m_nEdgeServerProtocol       = SOCK_DGRAM;

    m_strCloudServerAddr        = "";
    m_nCloudPort                = INVALID_SOCKET_PORT;
    m_nCloudProtocol            = SOCK_DGRAM;
}

CEdgeServer::~CEdgeServer()
{

}

// Initial VCCU module
bool CEdgeServer::InitialSystem()
{
    LogI("Initial %s ...", m_strModuleName.GetBuffer());

    CheckUpdate();

    if ( LoadConfig() == false )
    {
        return false;
    }

    // 创建用于和RSU通信的Socket Server
    CModuleMsgServer *pServer = new CModuleMsgServer();
    if ( pServer->Initial(m_strModuleCode, m_nModuleType, m_nEdgeServerPort, m_nEdgeServerProtocol, CEdgeServer::OnMessage, this) == false )
        return false;

    m_ServerList.AddTail(pServer);

    LogI("Create Edge Server Message server successfully.");

    // 创建用于和云端服务器通信的Socket Client
    CModuleMsgClient* pClient = new CModuleMsgClient();
    if ( pClient->Initial( INVALID_SOCKET_PORT, m_strModuleCode, m_nModuleType, m_strCloudServerAddr, m_nCloudPort, m_nCloudProtocol, CEdgeServer::OnMessage, this) == false )
        return false;

    pClient->SetServerModuleType(MODULE_CLOUD_SERVER, m_strCloudServerAddr, m_nCloudPort);
    m_ClientList.AddTail(pClient);

    LogI("Create Cloud client successfully.");

    LogI("Initial %s (ID: %s) successfully...", m_strModuleName.GetBuffer(), m_strModuleCode.GetBuffer());

    return true;
}

bool CEdgeServer::LoadConfig()
{
    // 加载默认配置参数
    CString strCfgFile = g_strAppRootPath;

    strCfgFile = strCfgFile + "/bin/v2x.conf";

    // LogI("Config file: %s", strCfgFile.GetBuffer());

    CString strSection = "COMMON";

    CString strValue, strKey;
    int nValue;

    strKey = "LocalAddr";
    strValue = GetConfigStr(strCfgFile, strSection, strKey, "");
    if ( strValue == "")
    {
        LogE("Error: Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }

    m_strLocalAddr = strValue;

    // 边缘服务器的信息
    strKey = "EdgeServerPort";
    nValue = GetConfigInt(strCfgFile, strSection, strKey, 0);
    if ( nValue == 0)
    {
        LogE("Error: Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }

    m_nEdgeServerPort = nValue;

    strKey = "EdgeServerProtocol";
    nValue = GetConfigInt(strCfgFile, strSection, strKey, 0);
    if ( nValue == 0)
    {
        LogE("Error: Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }
    if ( nValue == 1)
        m_nEdgeServerProtocol = SOCK_STREAM;
    else if ( nValue == 2)
        m_nEdgeServerProtocol = SOCK_DGRAM;
    else
    {
        LogE("Error: Config parameters %s value error: %d.", strKey.GetBuffer(), nValue);
        return false;
    }

    // 云端服务器的信息
    strKey = "CloudServerAddr";
    strValue = GetConfigStr(strCfgFile, strSection, strKey, "");
    if ( strValue == "")
    {
        LogE("Error: Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }

    m_strCloudServerAddr = strValue;

    strKey = "CloudServerPort";
    nValue = GetConfigInt(strCfgFile, strSection, strKey, 0);
    if ( nValue == 0)
    {
        LogE("Error: Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }

    m_nCloudPort = nValue;

    strKey = "CloudProtocol";
    nValue = GetConfigInt(strCfgFile, strSection, strKey, 0);
    if ( nValue == 0)
    {
        LogE("Error: Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }
    if ( nValue == 1)
        m_nCloudProtocol = SOCK_STREAM;
    else if ( nValue == 2)
        m_nCloudProtocol = SOCK_DGRAM;
    else
    {
        LogE("Error: Config parameters %s value error: %d.", strKey.GetBuffer(), nValue);
        return false;
    }

    return true;
}

bool CEdgeServer::StartSystem()
{
    LogI("%s Starting...", m_strModuleName.GetBuffer());

    if ( CModuleBase::StartSystem() == false )
    {
        return false;
    }

    SendInitialRequestMsg();

    UINT nCount = 0;
    while(1)
    {
        int nLoopTimes = 30 * 60;

        // 250,000,000: 基本上就是1秒左右的时间，这和主频及当时负荷状况有关
        for(int i = 0; i < 250000000; i++)
            for(int j = 0; j < nLoopTimes; j++);

        LogD("Edge Server system loop times: %d.", ++ nCount);
        if ( nCount >= 0x0FFFFFF0)
            nCount = 0;
    }    
}

// 发送初始化请求消息到云端服务器
void CEdgeServer::SendInitialRequestMsg()
{
    CModuleMsgClient* pClient = (CModuleMsgClient*)m_ClientList.GetHead();
    while(pClient != NULL)
    {
        // 如果已经连接成功就不需要发生初始化请求消息
        CModuleServerInfo* pServerInfo = (CModuleServerInfo*)pClient->GetServerList()->GetHead();
        while ( pServerInfo != NULL )
        {
            if ( pServerInfo->GetAppConnectStatus() == true )
            {
                pServerInfo = (CModuleServerInfo*)pClient->GetServerList()->GetNext(pServerInfo);
                continue;
            }

            // 根据服务端的类型确定发送什么类型的初始化请求消息
            UINT nServerModuleType = pServerInfo->GetServerModuleType();
            if ( nServerModuleType == MODULE_CLOUD_SERVER )
            {
                CRSUInitialRequestToCloudServerMsg msg;
                msg.SetDestAddr(pClient->GetServerAddr());
                msg.SetDestPort(pClient->GetServerPort());
                msg.SetDestModuleType(nServerModuleType);
                SendMessageToServer(&msg);
            }

            pServerInfo = (CModuleServerInfo*)pClient->GetServerList()->GetNext(pServerInfo);
        }

        pClient = (CModuleMsgClient*)m_ClientList.GetNext(pClient);
    }
}

// 解析消息
void CEdgeServer::DecodeMessage(PUCHAR pBuffer, UINT nMsgLen, LPCSTR strSrcAddr, UINT nSrcPort)
{
    CRSUMsgBase msg;
    CMsgBase* pMsg = NULL;

    // 接收到的消息
    CHeartBeatMsg                           HeartbeatMsg;
    CHeartBeatAckMsg                        HeartbeatAckMsg;
    CRSUInitialRequestToEdgeServerMsg       RSUInitialRequestToEdgeServerMsg;
    CRSUInitialRequestToCloudServerAckMsg   RSUInitialRequestToCloudServerAckMsg;
    CRSUInitialRequestToMasterRSUAckMsg     RSUInitialRequestToMasterRSUAckMsg;

    UINT nMsgType = msg.DecodeMsgType(pBuffer, nMsgLen);
    switch(nMsgType)
    {
    case MSG_HEARTBEAT:
        pMsg = &HeartbeatMsg;
        break;
    case MSG_HEARTBEAT_ACK:
        pMsg = &HeartbeatAckMsg;
        break;
    case RSU_MSG_InitialRequestToEdgeServerMsg:
        pMsg = &RSUInitialRequestToEdgeServerMsg;
        break;
    case RSU_MSG_InitialRequestToCloudServerAckMsg:
        pMsg= &RSUInitialRequestToCloudServerAckMsg;
        break;

    default:
        LogE("Error: Receive unsupport msg %s", ConvertToHex(pBuffer, nMsgLen));
        return;
    }

    if ( pMsg->Decode(pBuffer, nMsgLen) == false )
    {
        LogE("Decode message %s error.", pMsg->toString());
        return;
    }

    pMsg->SetSrcAddr(strSrcAddr);
    pMsg->SetSrcPort(nSrcPort);

    if ( pMsg->GetMsgType() != MSG_HEARTBEAT_ACK && pMsg->GetMsgType() != MSG_HEARTBEAT   )
        LogI("Receive %s from %s(%s:%d)", pMsg->toString(), pMsg->GetSrcModuleName(), strSrcAddr, nSrcPort);
    else
        LogDI("Receive %s from %s(%s:%d)", pMsg->toString(), pMsg->GetSrcModuleName(), strSrcAddr, nSrcPort);

    ProcessMessage(pMsg);    
}

// Process Message
void CEdgeServer::ProcessMessage(CMsgBase* pMsg)
{
    if ( pMsg == NULL )
    {
        return;
    }

    switch ( pMsg->GetMsgType() )
    {
    case MSG_HEARTBEAT:
        ProcessHeartbeatMsg(pMsg);
        break;
    case MSG_HEARTBEAT_ACK:
        ProcessHeartbeatAckMsg(pMsg);
        break;
    case RSU_MSG_InitialRequestToEdgeServerMsg:
        ProcessInitialRequestToEdgeServerMsg(pMsg);
        break;

    case RSU_MSG_InitialRequestToCloudServerAckMsg:
        ProcessInitialRequestToCludServerAckMsg(pMsg);
        break;

    default:
        LogE("Receive unsupport message %s from %s:%d", pMsg->toString(), pMsg->GetSrcAddr(), pMsg->GetSrcPort());
        return;
    }

    return;
}


// 接收来自RSU的初始化请求消息
void CEdgeServer::ProcessInitialRequestToEdgeServerMsg(CMsgBase* pMsg)
{
    if ( CModuleBase::ProcessInitialRequestMsg(pMsg) == false )
        return;

    if( pMsg->GetSrcModuleType() == MODULE_RSU)
    {
        // CRSUInitialRequestToCloudServerMsg* pInitialRequestMsg = (CRSUInitialRequestToCloudServerMsg*)pMsg;
        // 处理RSU的初始化请求消息

        // 发送初始化请求响应消息
        CRSUInitialRequestToEdgeServerAckMsg msg;

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

    return;
}


// 接收来自云端服务器的初始化请求响应消息
void CEdgeServer::ProcessInitialRequestToCludServerAckMsg(CMsgBase* pMsg)
{
    // 和云端服务器已经连接成功
    CModuleBase::ProcessInitialRequestAckMsg(pMsg);
}
