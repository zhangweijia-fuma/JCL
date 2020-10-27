// ------------------------------------------------------------------------------
// RSU Driver - Component :: V2X WiFi Driver definition
// Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    WiFiDrrver.h
// Purpose: RSU module
//-------------------------------------------------------------------------------

#ifndef _WIFI_DRIVER_H_
#define _WIFI_DRIVER_H_

#include "Debug.h"
#include "MsgBase.h"

#define DEFAULT_CAN_INTERFACE           "can0"
#define DEFAULT_SEND_TIMES              100


class CDMA3AWiFiDriver : public CObjBase
{
    public:
        CDMA3AWiFiDriver();
        ~CDMA3AWiFiDriver();

    public:
        bool                InitialBorad(MSG_CALLBACK pCallback, LPVOID pCallbackParam);

        bool                LoadConfig();
        static void*        DriverThread(LPVOID pParam);
        void                StartDriver();

        // void                SendMessage();

    protected:
        MSG_CALLBACK        m_pMsgCallBack;                             // 消息回调函数
        LPVOID              m_pCallbackParam;

        pthread_t           m_ThreadID;

        CString             m_strDeviceName;
};

#endif // _WIFI_DRIVER_H_
