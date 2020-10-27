// ------------------------------------------------------------------------------
// RSU - Component :: RSU, Road Side Unit
// Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    RSUModule.h
// Purpose: RSU module
//-------------------------------------------------------------------------------

#ifndef _RSU_MODULE_H_
#define _RSU_MODULE_H_

#include "ModuleBase.h"

#include "RSUTypes.h"
#include "RSUMessage.h"

#include "CSAEDataElement.h"

#include "GHDMA3ADriver.h"

#define DEVICE_STATUS_REPORT_TIME       5*60                            // 每5分钟上报一次状态

typedef int TEMPERATURE;
typedef int HUMIDITY;


class CRSUModule : public CModuleBase
{
    public:
        CRSUModule();
        ~CRSUModule();

    // Operations
    public:
        virtual bool        InitialSystem();
        virtual bool        StartSystem();
        virtual bool        LoadConfig();

        virtual void        DecodeMessage(PUCHAR pBuffer, UINT nMsgLen, LPCSTR strSrcAddr, UINT nSrcPort);
        // 接收从边缘服务器来的消息
        void                DecodeV2IMessage(PUCHAR pBuffer, UINT nMsgLen, LPCSTR strSrcAddr, UINT nSrcPort);

        // 接收从RSU Manager来的消息
        void                DecodeManageMessage(PUCHAR pBuffer, UINT nMsgLen, LPCSTR strSrcAddr, UINT nSrcPort);
        
        // 接收从车端来的消息
        static void         OnV2XMessage(LPVOID pParam, PUCHAR pBuffer, int nMsgLen);
        static void         OnGNSSMessage(LPVOID pParam, PUCHAR pBuffer, int nMsgLen);
        static void         OnCANMessage(LPVOID pParam, PUCHAR pBuffer, int nMsgLen);
        
        void                DecodeV2XMessage(PUCHAR pBuffer, UINT nMsgLen);
        void                DecodeGNSSMessage(PUCHAR pBuffer, UINT nMsgLen);
        void                DecodeCANMessage(PUCHAR pBuffer, UINT nMsgLen);

        virtual void        ProcessMessage(CMsgBase* pMsg);

        void                OnTimer(TIMER_ID nTimerID);                 // 定时任务处理
        virtual void        OnDownloadComplete(UINT, CString);          // 下载完成后处理
        void                onReportDeviceStatus();

    protected:
        bool                SelfCheck();                                // 硬件自检

        void                SendInitialRequestMsg();                    // 作为客户端，向服务器发送初始化请求消息

        virtual bool        ProcessInitialRequestAckMsg(CMsgBase* pMsg);
        void                ProcessUpdateSoftwareMsg(CMsgBase* pMsg);
        void                ProcessRebootMsg(CMsgBase* pMsg);
        void                ProcessReportStatusAckMsg(CMsgBase* pMsg);

        void                ProcessMainMsgFrame(CMainMsgFrame* pMsg);

        void                SendMessageToVehicle(CMainMsgFrame* pMsg);
        void                SendMessageToManager(CMsgBase* pMsg);

    // Attributes
    protected:
        CString             m_strRSUID;                                 // RSU ID，全网RSU ID唯一。
        UINT                m_nLocalServerPort;                         // 和边缘服务器通信的服务端端口

        CModuleMsgServer    m_RSUServer;

        CString             m_strEdgeServerAddr;                        // 边缘服务器地址
        UINT                m_nEdgeServerPort;                          // 边缘服务器端口
        CModuleMsgClient    m_RSCUClient;

        // RSU 管理端的地址及端口
        CString             m_strRSUManagerAddr;
        UINT                m_nRSUManagerPort;
        UINT                m_nRSUClientPort;

        CModuleMsgClient    m_RSUManagerClient;

        // 参数设置
        TIMER_ID            m_nDeviceStatusReportTimer;                 // 定时上报设备状态

        // 设备驱动模块
        CGHDMA3ADriver      m_DMA3ADriver;
        
        // ///////////////////////////////////////////////////////////////
        // RSU 硬件参数
        
        DE_Longtitude       m_Longtitude;                               // 经度
        DE_Latitude         m_Latitude;                                 // 纬度

        TEMPERATURE         m_Temperature;                              // 当前空气温度
        HUMIDITY            m_Humidity;                                 // 当前空气湿度

        CString             m_strVersion;                               // 版本号

        CString             m_strSIMNo;                                 // SIM卡移动号码;
        CString             m_strSIMIMSI;                               // SIM卡 IMSI 号
};

#endif // _RSU_MODULE_H_
