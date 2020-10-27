// ------------------------------------------------------------------------------
// JCCL - Component :: Socket
// Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    MsgSocket.h
// Purpose: Socket define 
//-------------------------------------------------------------------------------

#ifndef _MSG_SOCKET_H_
#define _MSG_SOCKET_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include "CString.h"
#include "Debug.h"

typedef struct sockaddr         SOCKADDR;
typedef struct sockaddr_in      SOCKADDR_IN;

/////////////////////////////////////////////////////////////////////////////
// CSocket

class CSocket : public CObjBase
{
    // Construction
    public:
        CSocket();
        ~CSocket();

    // Operations
    public:
        bool                Create(int nSocketType = SOCK_STREAM);

        bool                Bind(UINT nSocketPort, CString addr);
        bool                Bind (const SOCKADDR* lpSockAddr, int nSockAddrLen);

        void                Close();

    	bool                Listen( int nConnectionBacklog = 5 );

        bool                Accept(CSocket& rConnectedSocket, SOCKADDR* lpSockAddr = NULL, int* lpSockAddrLen = NULL);

        bool                Connect(CString srvAddr, UINT nSrvPort);
        bool                Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen);

        bool                IOCtl(long lCommand, DWORD* lpArgument);

        int                 Receive(void* lpBuf, int nBufLen, int nFlags = MSG_DONTWAIT);

        int                 ReceiveFrom(void* lpBuf, int nBufLen, CString &rSrcAddr, UINT& rSocketPort, int nFlags = MSG_DONTWAIT);
        int                 ReceiveFrom(void* lpBuf, int nBufLen, SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags = MSG_DONTWAIT);

        // enum { receives = 0, sends = 1, both = 2 };
        // bool ShutDown(int nHow = sends);

        // MSG_NOSIGNAL, if TCP server closed, client thread will be killed by system SIGPIPE. 
        int                 Send(const void* lpBuf, int nBufLen, int nFlags = MSG_NOSIGNAL);

        int                 SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, CString addr, int nFlags = MSG_DONTWAIT);
        int                 SendTo(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags = MSG_DONTWAIT);

        int                 Broadcast(const void* lpBuf, int nBufLen, UINT nHostPort);

        void                SetPortMask( fd_set* pReadReady);
        bool                IsDataArrived( fd_set* pReadReady );

        bool                GetPeerName(SOCKADDR* lpSockAddr, int* lpSockAddrLen);
        bool                GetPeerName(CString& rPeerAddress, UINT& rPeerPort);

        bool                GetSockName(CString& sockName, UINT& rSocketPort);
        bool                GetSockName(SOCKADDR* lpSockAddr, int* lpSockAddrLen);

        bool                SetSockOpt(int nOptionName, const void* lpOptionValue, int nOptionLen, int nLevel = SOL_SOCKET);
        bool                GetSockOpt(int nOptionName, void* lpOptionValue, int* lpOptionLen, int nLevel = SOL_SOCKET);

        int                 GetSockType() { return m_SockType; };
        
        bool                IsIdle()   { return m_hSocket == INVALID_SOCKET; };
        int                 GetSocket() { return m_hSocket; };

    // Attributes
    protected:
        bool                Socket(int nSocketType = SOCK_STREAM, int nProtocolType = 0, int nAddressFormat = AF_INET);

    	int                 m_hSocket;
        int                 m_SockType;
};

#endif // _MSG_SOCKET_H_


