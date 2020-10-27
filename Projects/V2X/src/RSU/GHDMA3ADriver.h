// ------------------------------------------------------------------------------
// RSU Driver - Component :: DT DMA3A Board Driver
// Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    GHDMA3ADriver.h
// Purpose: RSU module
//-------------------------------------------------------------------------------

#ifndef _GH_DMA3A_DRIVER_H_
#define _GH_DMA3A_DRIVER_H_

#include "MsgBase.h"

#include "GNSS.h"
#include "V2XPrimitive.h"
#include "CanDriver.h"
#include "WiFiDriver.h"

// 基于大唐高鸿模组的设备驱动
class CGHDMA3ADriver : public CObjBase
{
    public:
        CGHDMA3ADriver();

    public:
        bool                InitialBorad(MSG_CALLBACK pV2XCallback, MSG_CALLBACK pGNSSCallBack, MSG_CALLBACK pCANCallBack, LPVOID pCallbackParam);
        bool                LoadConfig();

        void                DecodeV2XMessage(PUCHAR pBuffer, UINT nMsgLen);

        bool                V2X_SendMessage(CPrimitive* pPrimitiveList);
        void                V2X_ReceiveMessage();

        void                GetDevicePosition(int& nLng, int & nLat, CString strID = "");   // 获取GNSS数据
        void                GetTHData(int& nTemperature, int& nHumidity);                   // 获取温湿度传感器数据

    protected:
        static void*        DriverThread(LPVOID pParam);
        void                StartDriver();

        int                 GetV2XDeviceInterface();
        int                 V2X_IOCtrl(int cnt);

        MSG_CALLBACK        m_pMsgCallBack;                             // 消息回调函数
        LPVOID              m_pCallbackParam;

        pthread_t           m_ThreadID;


        CString             m_strV2XDeviceName;
        int                 m_V2XSocket;

        CPrimitive          m_Primitive;

        CGNSSDriver         m_GNSSDriver;
        CDMA3ACANDriver     m_CanDriver;
        CDMA3AWiFiDriver    m_WiFiDriver;
};

#endif // _GH_DMA3A_DRIVER_H_
