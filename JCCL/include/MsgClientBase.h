// ------------------------------------------------------------------------------
// JCCL - Component :: Socket Message Client Base
// Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    MsgClientBase.h
// Purpose: based on socket, support TCP/UDP protocol message server
//-------------------------------------------------------------------------------

// TCP or UDP服务端其中一种，不支持同时提供TCP及UDP服务端

#ifndef _MSG_CLIENT_BASE_H_
#define _MSG_CLIENT_BASE_H_

#include "MsgBase.h"
#include "MsgSocket.h"

#include <pthread.h>

// 消息客户端基类，完成消息的基本处理
class CMsgClientBase : public CObjBase
{
    public:
        CMsgClientBase();

    // Operations
    public:
        virtual bool        Initial(UINT nLocalPort, CString ModuleCode, UINT nModuleType, CString srvAddr, UINT nSrvPort, int nSocketType = SOCK_DGRAM, RECEIVE_DATA_CALLBACK callback = NULL, LPVOID pParam = NULL);
        bool                RunClient();

        static void*        ClientThread(LPVOID pParam);
        bool                ClientLoop(CString );

        void                SendMessage(CMsgBase* pMsg, bool bIsBroadcast = false);
        void                SendMessage(PUCHAR pMsg, UINT nMsgLen, CString strAddr, UINT nPort, bool bIsBroadcast = false);
        
        int                 GetSockType() { return m_nSockType; };

        CString             GetModuleCode() { return m_strModuleCode; };
        CString             GetLocalAddr() { return m_strLocalAddr; };
        UINT                GetModuleType() { return m_nModuleType; };

        UINT                GetLocalPort() { return m_nLocalPort; };

        CString             GetServerAddr() { return m_strSrvAddr; };
        UINT                GetServerPort() { return m_nSrvPort; };
        
        void                Close();
        CSocket*            GetSocket() { return &m_Socket; };

    protected:
        void                TCPClient();
        void                UDPClient();

    // Attibutes
    protected:
        CString             m_strSrvAddr;                               // 客户端连接的服务端地址
        UINT                m_nSrvPort;                                 // 服务端端口
        int                 m_nSockType;                                // 传输协议是TCP或UDP

        CString             m_strLocalAddr;                             // 客户端使用的IP地址
        CString             m_strLocalMacAddr;                          // MAX 地址
        UINT                m_nLocalPort;                               // 客户端本地端口

        RECEIVE_DATA_CALLBACK m_pMsgCallback;
        LPVOID              m_pCallbackParam;

        CSocket             m_Socket;

        pthread_t           m_ThreadID;

        bool                m_bRunFlag;                                 // 是否运行标志
        bool                m_bConnectFlag;                             // 用于确认TCP连接是否已经建立，不是应用层的连接状态

        CString             m_strModuleCode;                            // 客户端编号
        UINT                m_nModuleType;                              // 客户端的类型
};

#endif  // _MSG_CLIENT_BASE_H_
