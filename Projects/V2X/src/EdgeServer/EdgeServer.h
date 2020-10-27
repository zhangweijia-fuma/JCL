/*------------------------------------------------------------------------------
 * RSU - Component :: EdgeServer
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    EdgeServer.h
 * Purpose: EdgeServer module, service flow realization
 *----------------------------------------------------------------------------*/

#ifndef _EDGE_SERVER_H_
#define _EDGE_SERVER_H_

#include "ModuleBase.h"

#include "RSUTypes.h"

class CEdgeServer : public CModuleBase
{
    public:
        CEdgeServer();
        ~CEdgeServer();

    // Operations
    public:
        virtual bool        InitialSystem();
        virtual bool        StartSystem();
        virtual bool        LoadConfig();

        virtual void        DecodeMessage(PUCHAR pBuffer, UINT nMsgLen, LPCSTR strSrcAddr, UINT nSrcPort);
        void                ProcessMessage(CMsgBase* pMsg);


        void                ProcessInitialRequestToEdgeServerMsg(CMsgBase* pMsg);
        void                ProcessInitialRequestToCludServerAckMsg(CMsgBase* pMsg);

    protected:

        void                SendInitialRequestMsg();                    // 作为客户端，向服务器发送初始化请求消息

    // Attributes
    protected:
        UINT                m_nEdgeServerPort;                          // 边缘服务器端口
        UINT                m_nEdgeServerProtocol;                      // 边缘服务器使用的协议

        CMsgClientBase      m_CloudClient;                              // 和云端通信的客户端
        CString             m_strCloudServerAddr;                       // 云端服务器地址
        UINT                m_nCloudPort;                               // 云端服务器端口
        UINT                m_nCloudProtocol;                           // 云端服务器使用的协议

};

#endif // _EDGE_SERVER_H_
