// ------------------------------------------------------------------------------
// JCCL - Component :: Socket Message Server
// Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    MsgServerBase.cpp
// Purpose: based on socket, support TCP/UDP protocol message server
//-------------------------------------------------------------------------------

#include "MsgServerBase.h"

CMsgServerBase::CMsgServerBase()
{
    m_pMsgCallback      = NULL;
    m_bRunFlag          = false;
    m_nModuleType       = MODULE_UNKNOWN;
    m_strModuleCode     = "";

    for (int i = 0; i < MAX_CLIENT_CONNECTION; i++)
    {
        m_ClientList[i].ClientAddr  = "";
        m_ClientList[i].ClientCode  = "";
        m_ClientList[i].nClientPort = INVALID_SOCKET_PORT;
    }
}

bool CMsgServerBase::Initial(CString strModuleCode, UINT nModuleType, UINT nPort, int nSocketType, RECEIVE_DATA_CALLBACK pMsgCallback, LPVOID pMsgCallbackParam)
{
    if ( nPort == INVALID_SOCKET_PORT || pMsgCallback == NULL )
    {
        LogE("Initital Socket Server failed, missing parameters.");
        return false;
    }

    if ( m_Socket.Create(nSocketType) == false )
    {
        LogE("Create socket failed.");
        return false;
    }

    m_strModuleCode     = strModuleCode;
    m_nModuleType       = nModuleType;

    m_strSrvAddr        = g_strLocalIPAddr;
    m_nSrvPort          = nPort;
    m_nSockType         = nSocketType;

    m_pMsgCallback      = pMsgCallback;
    m_pCallbackParam    = pMsgCallbackParam;

    if ( m_Socket.Bind(nPort, "") == false )
    {
        LogE("Bind Server Socket %d failed", nPort);
        m_Socket.Close();
        return false;
    }

    LogDI("Bind local address: %d", nPort);

    if ( nSocketType != SOCK_STREAM )
        return true;

    // TCP Server，准备接收客户端的连接请求
    if ( m_Socket.Listen() == false )
    {
        LogE("Socket listen failed.");
        return false;
    }

    return true;
}

bool CMsgServerBase::RunServer()
{
    if (pthread_create(&m_ThreadID, NULL, CMsgServerBase::ServerThread, (void*)this) == -1)
    {
        LogE("Create message server thread error!");
        return false;
    }

    return true;
}

void* CMsgServerBase::ServerThread(LPVOID pParam)
{
    if ( pParam == NULL )
        return NULL;

    CMsgServerBase *pMsgServer = (CMsgServerBase*)pParam;

    if ( pMsgServer == NULL )
    {
        LogE("CMsgServerBase::ServerThread parameters Non CMsgClient pointer");
        return NULL;
    }

    if ( pMsgServer->GetSockType() == SOCK_STREAM )
        pMsgServer->TCPServer();
    else
        pMsgServer->UDPServer();

    return NULL;
}

void CMsgServerBase::TCPServer()
{
    m_bRunFlag = true;
    while( m_bRunFlag )
    {
        sleep(1);

        SOCKADDR addr;
        int nLen = sizeof(addr);
        
        // 查找空闲连接
        int i = 0;
        for(i = 0; i < MAX_CLIENT_CONNECTION; i++)
        {
            if ( m_ClientList[i].IsIdle() )
                break;
        }

        if (i == MAX_CLIENT_CONNECTION)
        {
            LogE("Client connetion exceed max limit: %d", MAX_CLIENT_CONNECTION);
            continue;
        }

        if ( m_Socket.Accept( m_ClientList[i].clientSock, &addr, &nLen) )
        {
            CONN_PARAM param;
            param.nIndex = i;
            param.pParam = this;

            // 获取客户端的地址
            if( m_ClientList[i].clientSock.GetPeerName( m_ClientList[i].ClientAddr, m_ClientList[i].nClientPort) == false )
            {
                LogE("Get client address failed.");
                return;
            }

            // 查找是否有重新连接
            for(int j = 0; j < MAX_CLIENT_CONNECTION; j++)
            {
                if ( i != j && m_ClientList[i].ClientAddr == m_ClientList[j].ClientAddr && m_ClientList[i].nClientPort == m_ClientList[j].nClientPort )
                {
                    // Find repeat record of connection, close previous one
                    m_ClientList[j].clientSock.Close();
                    m_ClientList[j].ClientAddr  = "";
                    m_ClientList[j].nClientPort = INVALID_SOCKET_PORT;
                    break;
                }
            }

            // LogI("Receive client connect request from %s:%d.", m_ClientList[i].ClientAddr.GetBuffer(), m_ClientList[i].nClientPort);

            // 创建一个线程来处理连接请求
            if ((pthread_create(&m_ClientList[i].ConnThreadID, NULL, CMsgServerBase::ClientConnect, (void*)&param)) == -1)
            {
                LogE("Create client connection thread error!");
                m_ClientList[i].clientSock.Close();
                continue;
            }
        }
    }
}

void * CMsgServerBase::ClientConnect(LPVOID pParam)
{
    if ( pParam == NULL )
    {
        LogE("Parameters is NULL.");
        return NULL;
    }

    CONN_PARAM *pConnect = (CONN_PARAM*)pParam;

    CMsgServerBase *pMsgServer = (CMsgServerBase*)pConnect->pParam;
    pMsgServer->ProcessClientConnect(pConnect->nIndex);

    return NULL;
}

void CMsgServerBase::ProcessClientConnect(UINT nConnIndex)
{
    if ( nConnIndex >= MAX_CLIENT_CONNECTION )
    {
        LogE("Parameters Connection Index excedd max limit.");
        return;
    }

    CLIENT_CONNECTION *pConn = &m_ClientList[nConnIndex];

    fd_set fdMsgFlag;
    timeval tmWaitTime;

    FD_ZERO( &fdMsgFlag );

    while(m_bRunFlag)
    {
        if ( pConn->clientSock.IsIdle() )
        {
            LogE("Socket closed.");
            break;
        }

        FD_SET( pConn->clientSock.GetSocket(), &fdMsgFlag );

        // 检查是否有数据到达的时间间隔，60*60*24
        tmWaitTime.tv_sec  = MAX_SOCKET_WAIT_TIME;
        tmWaitTime.tv_usec = 0;

        int nTemp = select(FD_SETSIZE, &fdMsgFlag, (fd_set*)0, (fd_set*)0, &tmWaitTime );

        if ( nTemp <= 0 )
            continue;

        // 需要判断连接是否还存在
        UCHAR msgBuffer[MAX_MSG_LEN];
        
        int nRecBytes = pConn->clientSock.Receive( (LPVOID)msgBuffer, MAX_MSG_LEN);
        if ( nRecBytes < 0)
        {
            // LogE("Receive data from client error.");
            continue;
        }
        else if ( nRecBytes == 0 )
        {
            // LogI("Receive nothing.");
            continue;
        }
        else
        {
            LogDI("Receive data from %s:%d", pConn->ClientAddr.GetBuffer(), pConn->nClientPort);
            m_pMsgCallback(m_pCallbackParam, msgBuffer, nRecBytes, pConn->ClientAddr.GetBuffer(), pConn->nClientPort);
        }
    }
}

void CMsgServerBase::UDPServer()
{
    fd_set fdMsgFlag;
    timeval tmWaitTime;

    FD_ZERO( &fdMsgFlag );

    m_bRunFlag = true;
    while( m_bRunFlag )
    {
        FD_SET( m_Socket.GetSocket(), &fdMsgFlag );

        // 检查是否有数据到达的时间间隔，60*60*24
        tmWaitTime.tv_sec  = MAX_SOCKET_WAIT_TIME * 5;
        tmWaitTime.tv_usec = 0;

        int nTemp = select(FD_SETSIZE, &fdMsgFlag, (fd_set*)0, (fd_set*)0, &tmWaitTime );

        if ( nTemp < 0 )
        {
            LogI("Select failed, %d, Error No: %d", nTemp, errno);
            continue;
        }

        if ( nTemp == 0 )
        {
            LogI("No data arrive");
            continue;
        }

        UCHAR msgBuffer[MAX_MSG_LEN];
        CString srcAddr;
        UINT nSrcPort;

        int nRecBytes = m_Socket.ReceiveFrom( (LPVOID)msgBuffer, MAX_MSG_LEN, srcAddr, nSrcPort);
        if ( nRecBytes < 0)
        {
            LogE("Receive data from client %d.", nRecBytes);
            continue;
        }
        else if ( nRecBytes == 0 )
        {
            // 无数据
            LogI("Not receive any data");
            continue;
        }

        LogD("Receive data from %s:%d", srcAddr.GetBuffer(), nSrcPort);
        m_pMsgCallback(m_pCallbackParam, msgBuffer, nRecBytes, srcAddr.GetBuffer(), nSrcPort);
    }
}

// 在消息中包含了对方IP地址及端口
void CMsgServerBase::SendMessage(CMsgBase* pMsg)
{
    if ( pMsg == NULL )
    {
        LogE("SendMessage parameters Msg is Null");
        return;
    }

    pMsg->SetSrcAddr(m_strSrvAddr);
    pMsg->SetSrcPort(m_nSrvPort);
    pMsg->SetSrcModuleType(m_nModuleType);
    pMsg->SetSrcModuleCode(m_strModuleCode);

    UINT nMsgLen = 0;
    if ( pMsg->Encode(nMsgLen) == false )
    {
        LogE("Encode %s failed.", pMsg->toString());
    }

    PUCHAR pMsgBuffer = pMsg->GetMsgBuffer();

    CString strDestAddr = pMsg->GetDestAddr();
    UINT nDestPort      = pMsg->GetDestPort();


    if ( g_NoShowMsgList.IsMsgInNoShowMsgList(pMsg->GetMsgType()) == false )
    // if( pMsg->GetMsgType() != MSG_HEARTBEAT_ACK && pMsg->GetMsgType() != MSG_HEARTBEAT)
        LogI("Send %s to %s(%s:%d)", pMsg->toString(), pMsg->GetDestModuleName(), pMsg->GetDestAddr(), pMsg->GetDestPort() );
    else
        LogDI("Send %s to %s(%s:%d)", pMsg->toString(), pMsg->GetDestModuleName(), pMsg->GetDestAddr(), pMsg->GetDestPort() );
    
    // 根据连接类型来决定发送方式
    if (GetSockType() == SOCK_STREAM )
    {
        // TCP 连接

        int i = 0;
        for(i = 0; i < MAX_CLIENT_CONNECTION; i++)
        {
            if ( m_ClientList[i].Find(strDestAddr, nDestPort) )
                break;
        }

        if ( i == MAX_CLIENT_CONNECTION )
        {
            LogE("Can not find client connection: %s:%d", pMsg->GetDestAddr(), pMsg->GetDestPort());
            return;
        }
        
        m_ClientList[i].clientSock.Send((LPVOID)pMsgBuffer, nMsgLen);
    }
    else
    {
        // UDP连接
        m_Socket.SendTo( (LPVOID)pMsgBuffer, nMsgLen, pMsg->GetDestPort(), pMsg->GetDestAddr() );
    }
}

// 直接发送数据包，指定目的地址及端口
void CMsgServerBase::SendMessage(PUCHAR pMsgBuffer, UINT nMsgLen, CString strAddr, UINT nPort)
{
    // 根据连接类型来决定发送方式
    if (GetSockType() == SOCK_STREAM )
    {
        // TCP 连接
        m_Socket.Send((LPVOID)pMsgBuffer, nMsgLen);
    }
    else
    {
        // UDP连接
        m_Socket.SendTo( (LPVOID)pMsgBuffer, nMsgLen, nPort, strAddr);
    }
}

void CMsgServerBase::Close()
{
    m_Socket.Close();
}
