// ------------------------------------------------------------------------------
// JCCL - Component :: Socket
// Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    MsgSocket.h
// Purpose: Socket define 
//-------------------------------------------------------------------------------

#include "MsgSocket.h"

CSocket::CSocket()
{
    m_SockType  = SOCK_DGRAM;
    m_hSocket   = INVALID_SOCKET;
}

CSocket::~CSocket()
{
    Close();
    m_SockType   = SOCK_DGRAM;   
}

bool CSocket::Create(int nSocketType)
{
    if (Socket(nSocketType, 0, AF_INET ) == false)
    {
        LogE("Create a socket failed.");
        return false;
    }

    m_SockType   = nSocketType;

    return true;
}

bool CSocket::Socket(int nSocketType, int nProtocolType, int nAddressFormat)
{
    m_hSocket = socket(nAddressFormat, nSocketType, nProtocolType);

    return m_hSocket != INVALID_SOCKET;
}

void CSocket::Close()
{
    if (m_hSocket != INVALID_SOCKET)
        close(m_hSocket);

    m_hSocket = INVALID_SOCKET;
}

bool CSocket::Bind(UINT nSocketPort, CString addr)
{
    SOCKADDR_IN sockAddr;
    memset(&sockAddr, 0, sizeof(SOCKADDR_IN));

    sockAddr.sin_family = AF_INET;
    if ( addr == "" )
        sockAddr.sin_addr.s_addr = INADDR_ANY;
    else
    {
        DWORD lResult;
        lResult = inet_addr(addr.GetBuffer());

        if (lResult == INADDR_NONE)
        {
            LogE("Invalid address: %s", addr.GetBuffer());
            return false;
        }

        sockAddr.sin_addr.s_addr = lResult;
    }

    sockAddr.sin_port = htons((u_short)nSocketPort);

    if ( Bind((SOCKADDR*)&sockAddr, sizeof(sockAddr)) == false )
    {
        LogE("Bind to address %s:%d failed.", addr.GetBuffer(), nSocketPort);
        return false;
    }

    return true;
}

bool CSocket::Bind(const SOCKADDR* lpSockAddr, int nSockAddrLen)
{
    return (SOCKET_ERROR != bind(m_hSocket, lpSockAddr, nSockAddrLen)); 
}

bool CSocket::Listen(int nConnectionBacklog)
{
    return (SOCKET_ERROR != listen(m_hSocket, nConnectionBacklog)); 
}

bool CSocket::Accept(CSocket& rConnectedSocket, SOCKADDR* lpSockAddr, int* lpSockAddrLen)
{
    int hTemp = accept(m_hSocket, lpSockAddr, (socklen_t*)lpSockAddrLen);

    rConnectedSocket.m_hSocket = hTemp;

    return (hTemp != INVALID_SOCKET);
}

bool CSocket::Connect(CString srvAddr, UINT nSrvPort)
{
    SOCKADDR_IN sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(srvAddr.GetBuffer());

    if (sockAddr.sin_addr.s_addr == INADDR_NONE)
    {
        struct hostent *lphost;
        lphost = gethostbyname( srvAddr.GetBuffer() );
        if (lphost == NULL)
        {
            LogE("Connect to server failed, server address %s", srvAddr.GetBuffer());
            Close();
            return false;
        }

        sockAddr.sin_addr.s_addr = ((in_addr*)lphost->h_addr)->s_addr;
    }

    sockAddr.sin_port = htons((u_short)nSrvPort);

    return Connect((SOCKADDR*)&sockAddr, (int)(sizeof(sockAddr)));
}

bool CSocket::Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen)
{
    if ( m_hSocket == INVALID_SOCKET )
    {
        LogE("Socket not initial.");
        return false;
    }

    if (connect(m_hSocket, lpSockAddr, nSockAddrLen) == SOCKET_ERROR)
    {
        LogE("Can not connect to server, code: %d", errno);
        Close();
        return false;
    }

    return true;
}

int CSocket::Receive(void* lpBuf, int nBufLen, int nFlags)
{
    if ( m_hSocket == INVALID_SOCKET )
        return -1;

    return recv(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags);
}

int CSocket::ReceiveFrom(void* lpBuf, int nBufLen, CString &rSrcAddr, UINT& rSocketPort, int nFlags)
{
    if ( m_hSocket == INVALID_SOCKET )
    {
        LogE("Socket not initial");
        return -1;
    }

    SOCKADDR_IN sockAddr;

    memset(&sockAddr, 0, sizeof(sockAddr));

    int nSockAddrLen = (int)(sizeof(sockAddr));

    int nResult = ReceiveFrom(lpBuf, nBufLen, (SOCKADDR*)&sockAddr, &nSockAddrLen, nFlags);
    if(nResult != SOCKET_ERROR)
    {
        rSocketPort = ntohs(sockAddr.sin_port);
        rSrcAddr = inet_ntoa(sockAddr.sin_addr);
    }

    return nResult;
}

int CSocket::ReceiveFrom(void* lpBuf, int nBufLen, SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags)
{
    if ( m_hSocket == INVALID_SOCKET )
    {
        LogE("Socket not initial");
        return -1;
    }
    
    return recvfrom(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags, lpSockAddr, (socklen_t*)lpSockAddrLen);
}

int CSocket::Send(const void* lpBuf, int nBufLen, int nFlags)
{
    if ( m_hSocket == INVALID_SOCKET )
    {
        LogE("Socket not initial");
        return -1;
    }

    return send(m_hSocket, (LPVOID)lpBuf, nBufLen, nFlags);
}

int CSocket::SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, CString addr, int nFlags)
{
    SOCKADDR_IN sockAddr;

    memset(&sockAddr, 0, sizeof(sockAddr));

    sockAddr.sin_family = AF_INET;

    // if not specific the addrss, then use broadcast address
    if (addr == "")
        sockAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    else
    {
        sockAddr.sin_addr.s_addr = inet_addr( addr.GetBuffer() );

        if (sockAddr.sin_addr.s_addr == INADDR_NONE)
        {
            struct hostent *lphost;
            lphost = gethostbyname( addr.GetBuffer() );
            if (lphost == NULL)
                return SOCKET_ERROR;
            
            sockAddr.sin_addr.s_addr = ((struct in_addr*)lphost->h_addr)->s_addr;
        }
    }

    sockAddr.sin_port = htons((u_short)nHostPort);

    return SendTo(lpBuf, nBufLen, (SOCKADDR*)&sockAddr, (int)(sizeof(sockAddr)), nFlags);
}

// 广播消息，根据子网掩码发送
// nHostport: 接收方端口
int CSocket::Broadcast(const void* lpBuf, int nBufLen, UINT nHostPort)
{
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family   = AF_INET;
    server.sin_port     = htons(nHostPort);
    server.sin_addr.s_addr = inet_addr("255.255.255.255");

    // 设置通讯方式对广播，即本程序发送的一个消息，网络上所有主机均可以收到
    int yes = 1;
    setsockopt(m_hSocket, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &yes, sizeof(yes));
    int nBytes = sendto(m_hSocket, lpBuf, nBufLen, 0, (struct sockaddr *)&server, sizeof(server));

    // 设置为0，结束广播
    yes = 0;
    setsockopt(m_hSocket, SOL_SOCKET, SO_BROADCAST, (void *)&yes, sizeof(yes));

    return nBytes;
}

int CSocket::SendTo(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags)
{
    if ( m_hSocket == INVALID_SOCKET )
        return -1;

    return sendto(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags, lpSockAddr, nSockAddrLen);
}

void CSocket::SetPortMask( fd_set* pReadReady)
{
    FD_SET( m_hSocket, pReadReady);
}

bool CSocket::IsDataArrived( fd_set* pReadReady )
{
    return FD_ISSET( m_hSocket, pReadReady);
}

bool CSocket::GetPeerName(CString& rPeerAddress, UINT& rPeerPort)
{
    struct sockaddr_in sockAddr;

    memset(&sockAddr, 0, sizeof(SOCKADDR_IN));

    int nSockAddrLen = (int)(sizeof(SOCKADDR_IN));

    bool bResult = GetPeerName((SOCKADDR*)&sockAddr, &nSockAddrLen);
    if (bResult)
    {
        rPeerPort       = (sockAddr.sin_port);
        rPeerAddress    = inet_ntoa( sockAddr.sin_addr);
    }
    
    return bResult;
}

bool CSocket::GetPeerName(SOCKADDR* lpSockAddr, int* lpSockAddrLen)
{ 
    return (SOCKET_ERROR != getpeername(m_hSocket, lpSockAddr, (socklen_t*)lpSockAddrLen)); 
}

bool CSocket::GetSockName(CString& rAddr, UINT& rSocketPort)
{
    SOCKADDR_IN sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));

    int nSockAddrLen = (int)(sizeof(sockAddr));

    bool bResult = GetSockName((SOCKADDR*)&sockAddr, &nSockAddrLen);
    if (bResult)
    {
        rSocketPort = ntohs(sockAddr.sin_port);
        rAddr = inet_ntoa( sockAddr.sin_addr);
    }
    
    return bResult;
}

bool CSocket::GetSockName(SOCKADDR* lpSockAddr, int* lpSockAddrLen)
{ 
	return (SOCKET_ERROR != getsockname(m_hSocket, lpSockAddr, (socklen_t*)lpSockAddrLen)); 
}

bool CSocket::SetSockOpt(int nOptionName, const void* lpOptionValue, int nOptionLen, int nLevel)
{
	return (SOCKET_ERROR != setsockopt(m_hSocket, nLevel, nOptionName, (LPCSTR)lpOptionValue, nOptionLen)); 
}

bool CSocket::GetSockOpt(int nOptionName, void* lpOptionValue, int* lpOptionLen, int nLevel)
{ 
	return (SOCKET_ERROR != getsockopt(m_hSocket, nLevel, nOptionName, (LPSTR)lpOptionValue, (socklen_t*)lpOptionLen)); 
}

bool CSocket::IOCtl(long , DWORD* )
{ 
    // lCommand       = 0;
    // *lpArgument    = 0; 
	return true; 
}

// bool CSocket::ShutDown(int nHow)
// {
//     return (SOCKET_ERROR != shutdown(m_hSocket, nHow)); 
// }

