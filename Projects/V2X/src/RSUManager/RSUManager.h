/*------------------------------------------------------------------------------
 * RSU - Component :: RSUManager
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    RSUManager.h
 * Purpose: RSUManager module, service flow realization
 *----------------------------------------------------------------------------*/

#ifndef _RSUManager_H_
#define _RSUManager_H_

#include "MsgServerBase.h"
#include "MsgClientBase.h"
#include "ModuleBase.h"

#include "RSUTypes.h"

class CRSUManager : public CModuleBase
{
    public:
        CRSUManager();
        ~CRSUManager(){};

    // Operations
    public:
        virtual bool        InitialSystem();
        virtual bool        StartSystem();

        virtual void        DecodeMessage(PUCHAR pBuffer, UINT nMsgLen, LPCSTR strSrcAddr, UINT nSrcPort);
        virtual bool        LoadConfig();

        virtual void        ProcessMessage(CMsgBase* pMsg);

    protected:
        virtual bool        ProcessInitialRequestMsg(CMsgBase* pMsg);


    // Attributes
    protected:
        UINT                m_nCloudPort;                               // 云端服务器端口
        UINT                m_nCloudProtocol;                           // 云端服务器使用的协议
};

#endif // _RSUManager_H_
