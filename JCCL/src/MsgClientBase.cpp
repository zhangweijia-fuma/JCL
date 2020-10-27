// ------------------------------------------------------------------------------
// YIMS - Component :: Socket Message Client
// Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    MsgClientBase.cpp
// Purpose: based on socket, support TCP/UDP protocol message server
//-------------------------------------------------------------------------------

#include "MsgClientBase.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

CMsgClientBase::CMsgClientBase()
{
    m_pMsgCallback  = NULL;
    m_bRunFlag      = false;
    m_bConnectFlag  = false;
}

// 客户端Socket初始化
// 客户端在初始化时，可能会不指定服务端的IP地址，但是服务端的端口必须指定
// 本地端口也可以不指定，由系统自动分配
bool CMsgClientBase::Initial(UINT nLocalPort, CString ModuleCode, UINT nModuleType, CString strServerAddr, UINT nServerPort, int nSocketType, RECEIVE_DATA_CALLBACK callback, LPVOID pParam)
{
    if ( callback == NULL )
    {
        LogE("Initital Socket Client failed, missing parameters.");
        return false;
    }

    if ( m_Socket.Create(nSocketType) == false )
    {
        LogE("Create Socket failed.");
        return false;
    }

    m_strLocalAddr      = g_strLocalIPAddr;
    m_nLocalPort        = nLocalPort;

    m_strLocalMacAddr   = g_strLocalMACAddr;

    m_strModuleCode     = ModuleCode;
    m_nModuleType       = nModuleType;

    m_strSrvAddr        = strServerAddr;
    m_nSrvPort          = nServerPort;
    m_nSockType         = nSocketType;

    m_pMsgCallback      = callback;
    m_pCallbackParam    = pParam;

    // 在没有指定端口的时候，设置端口为0，由系统自动分配端口
    if ( m_nLocalPort == INVALID_SOCKET_PORT )
        m_nLocalPort = 0;

    if ( m_Socket.Bind(m_nLocalPort, m_strLocalAddr) == false )
    {
        m_Socket.Close();
        return false;
    }

    // 有可能在绑定时没有指定本地端口，所以需要获得本地端口
    CString strAddr;
    if ( m_Socket.GetSockName(strAddr, m_nLocalPort) == false )
    {
        LogE("Get Local Socket Port failed");
        return false;
    }

    if ( nSocketType == SOCK_STREAM )
    {
        // 有可能在初始化时，没有指定服务端的地址
        if ( strServerAddr != "" )
            if ( m_Socket.Connect(strServerAddr, nServerPort) == false )
                return false;
    }

    m_bConnectFlag = true;
    return true;
}

// 每个客户端都运行一个线程来等待接收消息
bool CMsgClientBase::RunClient()
{
    int nRetCode = pthread_create(&m_ThreadID, NULL, CMsgClientBase::ClientThread, (LPVOID)this);
    if ( nRetCode != 0 )
    {
        LogE("Create message server thread error, code: %d", nRetCode);
        return false;
    }

    return true;
}

void* CMsgClientBase::ClientThread(LPVOID pParam)
{
    if ( pParam == NULL )
    {
        LogE("Msg Client thread parameters error.");
        return NULL;
    }

    CMsgClientBase *pMsgClient = (CMsgClientBase*)pParam;
    
    // if ( typeid(*pMsgClient) != typeid(CMsgClientBase))
    // {
    //     LogE("CMsgClientBase::ClientThread parameters Non CMsgClientBase pointer");
    //     return NULL;
    // }

    if ( pMsgClient->GetSockType() == SOCK_STREAM )
        pMsgClient->TCPClient();
    else
        pMsgClient->UDPClient();

    return NULL;
}

// TCP 连接客户端
// TCP连接还没有经过测试-2020-07-02
void CMsgClientBase::TCPClient()
{
    fd_set fdMsgFlag;
    timeval tmWaitTime;

    FD_ZERO( &fdMsgFlag );

    m_bRunFlag = true;
    while( m_bRunFlag )
    {
        if ( m_bConnectFlag == false )
        {
            LogI("Connect to server: %s:%d", m_strSrvAddr.GetBuffer(), m_nSrvPort);
            Initial(m_nLocalPort, m_strModuleCode, m_nModuleType, m_strSrvAddr, m_nSrvPort, m_nSockType, m_pMsgCallback, m_pCallbackParam);
            sleep(1);
            continue;
        }

        FD_SET( m_Socket.GetSocket(), &fdMsgFlag );

        // 检查是否有数据到达的时间间隔，60*60*24
        tmWaitTime.tv_sec  = MAX_SOCKET_WAIT_TIME;
        tmWaitTime.tv_usec = 0;

        int nTemp = select(FD_SETSIZE, &fdMsgFlag, (fd_set*)0, (fd_set*)0, &tmWaitTime );

        if ( nTemp <= 0 )
            continue;
       
        UCHAR msgBuffer[MAX_MSG_LEN];
        int nRecBytes = m_Socket.Receive( (LPVOID)msgBuffer, MAX_MSG_LEN);
        if ( nRecBytes < 0)
        {
            // LogE("Receive data from server error.");
            continue;
        }
        else if ( nRecBytes == 0 )
        {
            // LogI("Receive nothing.");
            continue;
        }
        else
        {
            LogDI("Receive data: Len: %d, Data: %s", nRecBytes, ConvertToHex(msgBuffer, nRecBytes));
            m_pMsgCallback(m_pCallbackParam, msgBuffer, nRecBytes, m_strSrvAddr.GetBuffer(), m_nSrvPort);
        }
    }
}

// UDP客户端
void CMsgClientBase::UDPClient()
{
    fd_set fdMsgFlag;
    timeval tmWaitTime;

    FD_ZERO( &fdMsgFlag );

    m_bRunFlag = true;

    while( m_bRunFlag )
    {
        if ( m_bConnectFlag == false )
        {
            LogI("Connect to server: %s:%d", m_strSrvAddr.GetBuffer(), m_nSrvPort);
            Initial(m_nLocalPort, m_strModuleCode, m_nModuleType, m_strSrvAddr, m_nSrvPort, m_nSockType, m_pMsgCallback, m_pCallbackParam);
            sleep(1);
            continue;
        }

        if ( m_Socket.IsIdle() )
        {
            m_bConnectFlag = false;
            LogE("Socket closed.");
            continue;
        }

        FD_SET( m_Socket.GetSocket(), &fdMsgFlag );

        // 检查是否有数据到达的时间间隔，1秒，在1秒内没有收到消息就继续循环
        tmWaitTime.tv_sec  = MAX_SOCKET_WAIT_TIME;
        tmWaitTime.tv_usec = 0;

        int nTemp = select(FD_SETSIZE, &fdMsgFlag, (fd_set*)0, (fd_set*)0, &tmWaitTime );

        if ( nTemp <= 0 )
            continue;

        UCHAR msgBuffer[MAX_MSG_LEN];
        CString rAddr;
        UINT rPort;

        int nRecBytes = m_Socket.ReceiveFrom( (LPVOID)msgBuffer, MAX_MSG_LEN, rAddr, rPort);
        if ( nRecBytes < 0)
        {
            // LogE("Receive data from server error.");
            continue;
        }
        else if ( nRecBytes == 0 )
        {
            // LogI("Receive nothing.");
            continue;
        }
        else
        {
            LogDI("Receive data from %s:%d", rAddr.GetBuffer(), rPort);
            m_pMsgCallback(m_pCallbackParam, msgBuffer, nRecBytes, rAddr.GetBuffer(), rPort);
        }
    }
}

// 在消息中包含了对方IP地址及端口
void CMsgClientBase::SendMessage(CMsgBase* pMsg, bool bIsBroadcast)
{
    if ( pMsg == NULL )
    {
        LogE("SendMessage parameters Msg is Null");
        return;
    }

    if ( m_bConnectFlag == false )
    {
        LogE("Connect losing.");
        return;
    }

    // 目的地址和本应用地址相同则不发送消息
    if ( m_strLocalAddr == pMsg->GetDestAddr() && m_nLocalPort == pMsg->GetDestPort() )
    {
        LogI("Warning: Send %s to local host(%s, %s:%d).", pMsg->toString(), pMsg->GetDestModuleName(), pMsg->GetDestAddr(), pMsg->GetDestPort() );
        return;
    }

    pMsg->SetSrcAddr(m_strLocalAddr);
    pMsg->SetSrcPort(m_nLocalPort);
    pMsg->SetSrcModuleCode(m_strModuleCode);
    pMsg->SetSrcModuleType(m_nModuleType);

    pMsg->SetSerialNo(0);

    UINT nMsgLen = 0;
    if ( pMsg->Encode(nMsgLen) == false )
    {
        LogE("Encode %s failed.", pMsg->toString());
        return;
    }

    PUCHAR pMsgBuffer = pMsg->GetMsgBuffer();

    if ( g_NoShowMsgList.IsMsgInNoShowMsgList(pMsg->GetMsgType()) == false )
        LogI("Send %s to %s(%s:%d)", pMsg->toString(), pMsg->GetDestModuleName(), pMsg->GetDestAddr(), pMsg->GetDestPort() );
    else
        LogDI("Send %s to %s(%s:%d)", pMsg->toString(), pMsg->GetDestModuleName(), pMsg->GetDestAddr(), pMsg->GetDestPort() );

    LogDI("%s", ConvertToHex(pMsgBuffer, pMsg->GetMsgLen()));

    // 根据连接类型来决定发送方式
    if (GetSockType() == SOCK_STREAM )
    {
        // TCP 连接
        m_Socket.Send((LPVOID)pMsgBuffer, nMsgLen);
    }
    else
    {
        // UDP连接
        if ( bIsBroadcast == true )
        {
            // LogI("Broadcast : %s, %s", g_strLocalIPAddr.GetBuffer(), g_strLocalNetMask.GetBuffer());
            // 广播消息
            m_Socket.Broadcast((LPVOID)pMsgBuffer, nMsgLen, pMsg->GetDestPort() );
        }
        else
        {
            CString strDestAddr = pMsg->GetDestAddr();
            UINT nDestPort      = pMsg->GetDestPort();
            m_Socket.SendTo( (LPVOID)pMsgBuffer, nMsgLen, nDestPort, strDestAddr);
        }
    }
}


// 直接发送数据包，指定目的地址及端口
void CMsgClientBase::SendMessage(PUCHAR pMsgBuffer, UINT nMsgLen, CString strAddr, UINT nPort, bool bIsBroadcast)
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
        if ( bIsBroadcast == true )
        {
            // 广播消息
            m_Socket.Broadcast((LPVOID)pMsgBuffer, nMsgLen, nPort );
        }
        else
        {
            m_Socket.SendTo( (LPVOID)pMsgBuffer, nMsgLen, nPort, strAddr);
        }
    }
}

void CMsgClientBase::Close()
{
    m_Socket.Close();
}
