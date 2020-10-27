// ------------------------------------------------------------------------------
// RSU Driver - Component :: V2X CAN Driver definition
// Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    CanDriver.h
// Purpose: RSU module
//-------------------------------------------------------------------------------

#ifndef _CAN_DRIVER_H_
#define _CAN_DRIVER_H_

#include "Debug.h"
#include "MsgBase.h"

#define DEFAULT_CAN_INTERFACE           "can0"
#define DEFAULT_SEND_TIMES              100


class CDMA3ACANDriver : public CObjBase
{
    public:
        CDMA3ACANDriver();
        ~CDMA3ACANDriver();

    public:
        bool                InitialBorad(MSG_CALLBACK pCallback, LPVOID pCallbackParam);

        bool                LoadConfig();
        static void*        DriverThread(LPVOID pParam);
        void                StartDriver();

        int                 GetCANInterface(int fd);

        int                 CreateCANSocket(bool bIsSend);
        void                SendMessage();

    protected:
        MSG_CALLBACK        m_pMsgCallBack;                             // 消息回调函数
        LPVOID              m_pCallbackParam;

        pthread_t           m_ThreadID;

        CString             m_strDeviceName;
        int                 m_nCANID;
        int                 m_nCANSendHandle;
        int                 m_nCANRecvHanlde;
};

#endif // _CAN_DRIVER_H_
