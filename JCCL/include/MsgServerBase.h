// ------------------------------------------------------------------------------
// JCCL - Component :: Socket Message Server
// Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    MsgServer.h
// Purpose: based on socket, support TCP/UDP protocol message server
//-------------------------------------------------------------------------------

// TCP or UDP服务端其中一种，不支持同时提供TCP及UDP服务端


#ifndef _MSG_SERVER_BASE_H_
#define _MSG_SERVER_BASE_H_

#include "MsgBase.h"
#include "MsgSocket.h"

#include <pthread.h>

// 最大客户端连接数，受限于操作系统文件句柄数
#define MAX_CLIENT_CONNECTION       512

typedef struct
{
    UINT    nIndex;
    LPVOID  pParam;
}CONN_PARAM;

typedef struct
{
    CSocket     clientSock;
    pthread_t   ConnThreadID;
    CString     ClientAddr;
    UINT        nClientPort;
    CString     ClientCode;

    bool IsIdle() { return clientSock.IsIdle(); };
    bool Find(CString DestAddr, UINT nDestPort )
    {
        return ( ClientAddr == DestAddr && nClientPort == nDestPort );
    }

}CLIENT_CONNECTION;

// 消息服务端
class CMsgServerBase : public CObjBase
{
    public:
        CMsgServerBase();

    // Operations
    public:
        bool                Initial(CString strModuleCode, UINT nModuleType, UINT nPort, int nSocketType = SOCK_DGRAM, 
                                RECEIVE_DATA_CALLBACK callback = NULL, LPVOID pParam = NULL);
        bool                RunServer();

        static void*        ServerThread(LPVOID pParam);
        static void*        ClientConnect(LPVOID pParam);
        void                ProcessClientConnect(UINT nConnIndex);

        int                 GetSockType() { return m_nSockType; };
        UINT                GetServerPort() { return m_nSrvPort; };
        
        void                SendMessage(CMsgBase* pMsg);
        void                SendMessage(PUCHAR pMsg, UINT nMsgLen, CString strAddr, UINT nPort);
        
        void                TCPServer();
        void                UDPServer();

        void                Close();

    // Attibutes
    protected:
        RECEIVE_DATA_CALLBACK m_pMsgCallback;
        LPVOID              m_pCallbackParam;

        CSocket             m_Socket;

        CString             m_strSrvAddr;
        UINT                m_nSrvPort;
        int                 m_nSockType;

        CString             m_strModuleCode;
        UINT                m_nModuleType;

        pthread_t           m_ThreadID;

        bool                m_bRunFlag;

        CLIENT_CONNECTION   m_ClientList[MAX_CLIENT_CONNECTION];
};

#endif  // _MSG_SERVER_BASE_H_
