/*------------------------------------------------------------------------------
 * JCCL - Component :: Module base
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    ModuleBase.h
 * Purpose: ModuleBase, 规定了业务模块的基本功能
 *----------------------------------------------------------------------------*/

#ifndef _MODULE_BASE_H_
#define _MODULE_BASE_H_

#include "Debug.h"
#include "MsgBase.h"
#include "ModuleList.h"

#include "MsgServerBase.h"
#include "MsgClientBase.h"
#include "DownloadFile.h"

#define CHECH_SYSTEM_ENVIRONMENT_TIME   3600                            // 每小时检查一次系统的运行环境

// 业务模块作为客户端时的Socket客户端
// 根据服务端地址和端口来区分服务端
// 这要求服务端有明确的地址和端口。
// 并且客户端和服务端之间有心跳及初始化请求消息
// 如果两个业务模块之间无持续连接，并且无初始化请求和心跳消息，
// 但是能够通过UDP进行通信，则需要通过UDP客户端进行通信


// 当一个Socket客户端和多个服务端连接时，客户端连接的服务端信息
class CModuleServerInfo : public CObjBase
{
    public:
        CModuleServerInfo();
        void                Initial(CString strModuleCode, CString strServerAddr = "", UINT nSrvPort = INVALID_SOCKET, UINT nModuleType = MODULE_UNKNOWN);

        CString             GetServerAddr() { return m_strSrvAddr; };
        UINT                GetServerPort() { return m_nSrvPort; };

        UINT                GetServerModuleType() { return m_nServerModuleType; };
        void                SetServerModuleType(UINT nModuleType) { m_nServerModuleType = nModuleType; };
        CString             GetServerModuleCode() { return m_strModuleCode; };

        UINT                GetExpireTimes() { return m_nHeartbeatExpireTimes; };
        void                SetExpireTimes() { m_nHeartbeatExpireTimes++; };
        void                ResetExpireTimres() { m_nHeartbeatExpireTimes = 0; };

        void                SetAppConnectStatus(bool bIsConnected) { m_bIsConnectedApp = bIsConnected; };
        bool                GetAppConnectStatus() { return m_bIsConnectedApp; };

        CString             m_strModuleCode;
        CString             m_strSrvAddr;                               // 客户端连接的服务端地址
        UINT                m_nSrvPort;                                 // 服务端端口
        UINT                m_nServerModuleType;                        // 服务端类型

        UINT                m_nHeartbeatExpireTimes;                    // 到服务端应用的连接是否正常，由业务模块设置
        bool                m_bIsConnectedApp;                          // 是否连接到服务端应用
};

// 业务模块作为客户端时的Socket客户端
// 一个Socket客户端，可以连接到多个服务端，
class CModuleMsgClient : public CMsgClientBase
{
    public:
        CModuleMsgClient();
        virtual bool        Initial(UINT nLocalPort, CString ModuleCode, UINT nModuleType, CString srvAddr, UINT nSrvPort, int nSocketType = SOCK_DGRAM, RECEIVE_DATA_CALLBACK callback = NULL, LPVOID pParam = NULL);

        CModuleServerInfo*  AddServer(CString strModuleCode, CString strServerAddr = "", UINT nSrvPort = INVALID_SOCKET_PORT, UINT nModuleType = MODULE_UNKNOWN);
        CModuleServerInfo*  FindServer(CString strServerAddr, UINT nServerPort);
        void                RemoveServer(CString strServerAddr, UINT nServerPort);

        // 如果没有设置服务端地址，则默认为第一个服务端
        UINT                GetServerModuleType(CString strServerAddr, UINT nServerPort);
        void                SetServerModuleType(UINT nModuleType, CString strServerAddr, UINT nServerPort);

        UINT                GetExpireTimes(CString strServerAddr, UINT nServerPort);
        void                SetExpireTimes(CString strServerAddr, UINT nServerPort);
        void                ResetExpireTimres(CString strServerAddr, UINT nServerPort);

        void                SetAppConnectStatus(bool bIsConnected, CString strServerAddr, UINT nServerPort);
        bool                GetAppConnectStatus(CString strServerAddr, UINT nServerPort);

        CList*              GetServerList() { return &m_ServerList; };

    // protected:
        CList               m_ServerList;                               // 服务端信息列表
};

// 业务模块作为服务端时的Socket服务端
class CModuleMsgServer : public CMsgServerBase
{
    public:
        CModuleMsgServer(){};
};


// 业务模块基本信息
// 业务模块完成：
// 1. 基本参数初始化
// 2. 更新检查
// 3. 消息的接收、发送
// 4. 心跳、初始化请求消息的处理
// 5、定时任务处理，检查和客户端及服务端的心跳连接
// 
// 能够实现单个业务模块作为多个Socket服务端，连接不同类型的客户端
// 也可以作为多个类型的客户端，和不同的服务端连接
// 同时，单个客户端，也可以和多个服务端连接；
class CModuleBase : public CObjBase
{
    public:
        CModuleBase();
        ~CModuleBase(){};

    // Operations
    public:
        virtual bool        InitialSystem();
        virtual bool        StartSystem();

        // 每个业务模块都需要具有接收消息、消息处理、定时器处理、发送消息等功能
        static void         TimerEvent(TIMER_ID nTimerID, LPVOID pParam);
        virtual void        OnTimer(TIMER_ID nTimerID);

        static void         OnMessage(LPVOID pCallbackParam, PUCHAR pBuffer, UINT nMsgLen, LPCSTR strSrcAddr, UINT nSrcPort);
        virtual void        DecodeMessage(PUCHAR pBuffer, UINT nMsgLen, LPCSTR strSrcAddr, UINT nSrcPort){};

        // 由派生类完成具体操作
        static void         DownloadCallBack(LPVOID pCallbackParam, UINT nMsgType, CString strLocalFile);
        virtual void        OnDownloadComplete(UINT nMsgType, CString strLocalFile);

    protected:
        virtual bool        CheckUpdate();
        virtual bool        LoadConfig() { return false; };
        virtual void        CheckConnection();

        virtual void        ProcessMessage(CMsgBase* pMsg){};

        virtual void        SendHeartbeatMsg(CString strServerAddr = "");   // 作为客户端，向服务端发送心跳消息
        virtual void        SendInitialRequestMsg() {};                     // 作为客户端，向服务器发送初始化请求消息，由派生类进行实现
        virtual void        ProcessHeartbeatMsg(CMsgBase* pMsg);            // 作为服务端，处理客户端的心跳消息
        virtual void        ProcessHeartbeatAckMsg(CMsgBase* pMsg);         // 作为客户端，处理服务端的心跳响应消息
        virtual bool        ProcessInitialRequestMsg(CMsgBase* pMsg);       // 作为服务端，处理客户端的初始化请求消息
        virtual bool        ProcessInitialRequestAckMsg(CMsgBase* pMsg);    // 作为客户端，处理服务端的初始化请求响应消息
        virtual void        SendMessageToClient(CMsgBase* pMsg);            // 发生消息给客户端
        virtual void        SendMessageToServer(CMsgBase* pMsg);            // 发送消息给服务端
        virtual void        SendMessage(CMsgBase* pMsg);                    // 根据消息中的地址发送消息，可支持无连接的消息发送

        void                OnCheckSystemEnv();                         // 检查系统的运行环境
        void                OnRebootSystem();                           // 重启系统
    // Attributes
    protected:
        CString             m_strModuleCode;                            // 模块编号
        UINT                m_nModuleType;                              // 模块类型

        CString             m_strLocalAddr;                             // 本机地址
        CString             m_strModuleName;                            // 业务模块名称

        bool                m_bIsKeepConnection;                        // 服务端和客户端直接是否保持心跳连接，默认为保持
        TIMER_ID            m_nCheckHeartbeatTimerID;                   // 心跳检查定时器ID，检查心跳连接情况或发送心跳消息

        CList               m_ServerList;                               // 业务模块作为服务端时的Socket列表
        CList               m_ClientList;                               // 业务模块作为客户端时的Socket列表 

        CModuleList         m_ModuleList;                               // 连接的客户端模块信息
        
        CMsgClientBase      m_MsgClient;                                // 只支持UDP消息

        BYTE                m_nUpdateMode;                              // 软件更新模式

        BYTE                m_nRebootMode;                              // 重启模式
        TIMER_ID            m_nRebootTimer;                             // 重启定时器

        TIMER_ID            m_nCheckEnvTimer;                           // 检查系统运行环境定时器

        CString             m_strFTPServerAddr;                         // FTP服务器地址及用户名密码，由派生类获取
        CString             m_strFTPUserName;
        CString             m_strFTPPassword;
        CDownloadFile       m_DownloadFile;                             // 文件下载上传

};

#endif // _MODULE_BASE_H_
