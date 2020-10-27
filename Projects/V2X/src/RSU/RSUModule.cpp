/*------------------------------------------------------------------------------
 * RSU - Component :: RSU, Road Side Unit
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    RSUModule.cpp
 * Purpose: RSU module, service flow realization
 *----------------------------------------------------------------------------*/

#include "Common.h"
#include "Timer.h"
#include "Config.h"
#include "File.h"

#include "RSUModule.h"
#include "RSUMessage.h"

// RSU Module
CRSUModule g_RSUModule;

CRSUModule::CRSUModule()
{
    m_strModuleName         = "Fuma AI V2X RSU System";
    m_strRSUID              = "RSU00001";

    m_strModuleCode         = "RSU00001";
    m_nModuleType           = MODULE_RSU;

    m_strLocalAddr          = g_strLocalIPAddr;
    m_nLocalServerPort      = INVALID_SOCKET_PORT;

    m_strEdgeServerAddr     = "";
    m_nEdgeServerPort       = INVALID_SOCKET_PORT;

    m_strRSUManagerAddr     = "";
    m_nRSUManagerPort       = INVALID_SOCKET_PORT;
    m_nRSUClientPort        = INVALID_SOCKET_PORT;

    m_strFTPUserName        = "";
    m_strFTPPassword        = "";

    m_nDeviceStatusReportTimer  = INVALID_TIMER_ID;
}

CRSUModule::~CRSUModule()
{

}

// Initial module
// RSU 作为客户端时，其客户端Socket可以使用同一个Socket，也可以使用不同的Socket。
// 在这里，使用的是不同的Socket和不同的服务端进行通信。
bool CRSUModule::InitialSystem()
{
    if ( LoadConfig() == false )
        return false;

    if ( CModuleBase::InitialSystem() == false )
        return false;

    // 创建用于和边缘服务器通信的Socket Server
    if ( m_RSUServer.Initial(m_strModuleCode, m_nModuleType, m_nLocalServerPort, SOCK_DGRAM, CRSUModule::OnMessage, this) == false )
        return false;

    // 创建用于和边缘服务器通信的Socket Client
    // 作为客户端时，不指定客户端的端口
    if ( m_RSCUClient.Initial( INVALID_SOCKET_PORT, m_strModuleCode, m_nModuleType, m_strEdgeServerAddr, m_nEdgeServerPort, SOCK_DGRAM, CRSUModule::OnMessage, this) == false )
        return false;

    // RSU Manager 客户端
    if ( m_RSUManagerClient.Initial( m_nRSUClientPort, m_strModuleCode, m_nModuleType, m_strRSUManagerAddr, m_nRSUManagerPort, SOCK_DGRAM, CRSUModule::OnMessage, this) == false )
        return false;

    m_RSUManagerClient.SetServerModuleType(MODULE_RSU_MANAGER, m_strRSUManagerAddr, m_nRSUManagerPort);
    m_ClientList.AddTail(&m_RSUManagerClient);

    m_nDeviceStatusReportTimer = g_Timer.CreateTimer( DEVICE_STATUS_REPORT_TIME * 1000, CModuleBase::TimerEvent, this );
    if ( m_nDeviceStatusReportTimer == INVALID_TIMER_ID )
    {
        LogE("Create Device Status Report Timer failed.");
        return false;
    }

    // 还需要对通信模组进行初始化，以接收和发送通信模组的消息
    if ( m_DMA3ADriver.InitialBorad(CRSUModule::OnV2XMessage, CRSUModule::OnGNSSMessage, CRSUModule::OnCANMessage, this) == false )
    {
        LogE("Initial Board Driver failed.");
        return false;
    }

    // 不需要显示在运行日志中的消息
    g_NoShowMsgList.InsertNoShowMsgType(RSU_MSG_ReportStatus);
    g_NoShowMsgList.InsertNoShowMsgType(RSU_MSG_ReportStatusAck);
    
    LogI("Initial %s (ID: %s) successfully...", m_strModuleName.GetBuffer(), m_strRSUID.GetBuffer());

    return true;
}

bool CRSUModule::LoadConfig()
{
    // 加载默认配置参数
    CString strCfgFile = g_strAppRootPath;

    strCfgFile = strCfgFile + "/conf/v2x.conf";

    CConfig configFile;
    if( configFile.OpenConfigFile(strCfgFile) == false )
    {
        LogE("Can not open config file: %s", strCfgFile.GetBuffer());
        return false;
    }

    CString strSection = "COMMON";
    CString strValue, strKey;
    UINT nValue;

    strKey = "RSUID";
    strValue = configFile.GetConfigStr(strSection, strKey, "");
    if ( strValue == "")
    {
        LogE("Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }
    m_strRSUID = strValue;

    strKey = "RSUCode";
    strValue = configFile.GetConfigStr(strSection, strKey, "");
    if ( strValue == "")
    {
        LogE("Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }
    m_strModuleCode = strValue;

    strKey = "ServerPort";
    nValue = configFile.GetConfigInt(strSection, strKey, 0);
    if ( nValue == 0)
    {
        LogE("Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }
    m_nLocalServerPort = nValue;

    // 边缘服务器的信息
    strKey = "EdgeServerAddr";
    strValue = configFile.GetConfigStr(strSection, strKey, "");
    if ( strValue == "")
    {
        LogE("Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }

    m_strEdgeServerAddr = strValue;

    strKey = "EdgeServerPort";
    nValue = configFile.GetConfigInt(strSection, strKey, 0);
    if ( nValue == 0)
    {
        LogE("Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }

    m_nEdgeServerPort = nValue;

    // RSU 管理端的信息
    strKey = "RSUManagerAddr";
    strValue = configFile.GetConfigStr(strSection, strKey, "");
    if ( strValue == "")
    {
        LogE("Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }

    m_strRSUManagerAddr = strValue;
    m_strFTPServerAddr  = strValue;

    strKey = "RSUManagerPort";
    nValue = configFile.GetConfigInt(strSection, strKey, 0);
    if ( nValue == 0)
    {
        LogE("Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }

    m_nRSUManagerPort = nValue;

    strKey = "RSUClientPort";
    nValue = configFile.GetConfigInt(strSection, strKey, 0);
    if ( nValue == 0)
    {
        LogE("Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }

    m_nRSUClientPort = nValue;

    strKey = "FTPUserName";
    strValue = configFile.GetConfigStr(strSection, strKey, "");
    if ( strValue == "")
    {
        LogE("Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }
    m_strFTPUserName = strValue;

    strKey = "FTPPassword";
    strValue = configFile.GetConfigStr(strSection, strKey, "");
    if ( strValue == "")
    {
        LogE("Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }
    m_strFTPPassword = strValue;

    return true;
}

bool CRSUModule::StartSystem()
{
    LogI("%s Starting...", m_strModuleName.GetBuffer());

    if ( CModuleBase::StartSystem() == false )
    {
        return false;
    }

    SendInitialRequestMsg();

    UINT nCount = 0;
    while(1)
    {
        int nLoopTimes = 30 * 60;

        // // 250,000,000: 基本上就是1秒左右的时间，这和主频及当时负荷状况有关
        // for(int i = 0; i < 250000000; i++)
        //     for(int j = 0; j < nLoopTimes; j++);

        // LogD("%s loop times: %d.",m_strModuleName.GetBuffer(), ++ nCount);
        // if ( nCount >= 0x0FFFFFF0)
        //     nCount = 0;
        sleep(1);
    }
    
    return true;
}

// 硬件自检
bool CRSUModule::SelfCheck()
{
    return false;
}


void CRSUModule::OnV2XMessage(LPVOID pParam, PUCHAR pBuffer, int nMsgLen)
{
    if ( pBuffer == NULL || nMsgLen == 0 || pParam == NULL)
    {
        LogE("OnV2XMessage parameter error.");
        return;
    }

    CRSUModule* pRSUModule = (CRSUModule*)pParam;
    pRSUModule->DecodeV2XMessage(pBuffer, nMsgLen);
}

void CRSUModule::OnGNSSMessage(LPVOID pParam, PUCHAR pBuffer, int nMsgLen)
{
    if ( pBuffer == NULL || nMsgLen == 0 || pParam == NULL)
    {
        LogE("OnV2XMessage parameter error.");
        return;
    }

    CRSUModule* pRSUModule = (CRSUModule*)pParam;
    pRSUModule->DecodeGNSSMessage(pBuffer, nMsgLen);
}

void CRSUModule::OnCANMessage(LPVOID pParam, PUCHAR pBuffer, int nMsgLen)
{
    if ( pBuffer == NULL || nMsgLen == 0 || pParam == NULL)
    {
        LogE("OnV2XMessage parameter error.");
        return;
    }

    CRSUModule* pRSUModule = (CRSUModule*)pParam;
    pRSUModule->DecodeCANMessage(pBuffer, nMsgLen);
}

// 解析消息
void CRSUModule::DecodeMessage(PUCHAR pBuffer, UINT nMsgLen, LPCSTR strSrcAddr, UINT nSrcPort)
{
    // 根据消息的源地址，确定是边缘服务器还是RSUManager
    if ( m_strEdgeServerAddr == strSrcAddr )
        DecodeV2IMessage(pBuffer, nMsgLen, strSrcAddr, nSrcPort);
    else if ( m_strRSUManagerAddr == strSrcAddr )
        DecodeManageMessage(pBuffer, nMsgLen, strSrcAddr, nSrcPort);
    else
        LogE("Receive message from unknown source: %s:%d", strSrcAddr, nSrcPort);

    return;
}

void CRSUModule::DecodeManageMessage(PUCHAR pBuffer, UINT nMsgLen, LPCSTR strSrcAddr, UINT nSrcPort)
{
    CRSUMsgBase msg;
    CMsgBase* pMsg = NULL;

    // 接收到的消息
    CHeartBeatAckMsg            HeartbeatAckMsg;
    CRSUInitialRequestAckMsg    RSUInitialRequestAckMsg;
    CRSUUpdateSoftwareMsg       RSUUpdateSoftwareMsg;
    CRSURebootMsg               RebootMsg;
    CRSUReportStatusAckMsg      ReportDeviceStatusAckMsg;

    UINT nMsgType = msg.DecodeMsgType(pBuffer, nMsgLen);
    switch(nMsgType)
    {
    case MSG_HEARTBEAT_ACK:
        pMsg = &HeartbeatAckMsg;
        break;
    
    case RSU_MSG_InitialRequestAck:
        pMsg = &RSUInitialRequestAckMsg;
        break;

    case RSU_MSG_UpdateSoftware:
        pMsg = &RSUUpdateSoftwareMsg;
        break;

    case RSU_MSG_Reboot:
        pMsg = &RebootMsg;
        break;

    case RSU_MSG_ReportStatusAck:
        pMsg = &ReportDeviceStatusAckMsg;
        break;

    default:
        LogE("Receive unsupport msg: %04X, %s", nMsgType, ConvertToHex(pBuffer, nMsgLen));
        return;
    }

    if ( pMsg->Decode(pBuffer, nMsgLen) == false )
    {
        LogE("Decode message %s error.", pMsg->toString());
        LogI(ConvertToHex(pBuffer, nMsgLen));
        return;
    }

    pMsg->SetSrcAddr(strSrcAddr);
    pMsg->SetSrcPort(nSrcPort);

    if ( g_NoShowMsgList.IsMsgInNoShowMsgList(pMsg->GetMsgType()) == false )
        LogI("Receive %s from %s(%s:%d)", pMsg->toString(), pMsg->GetSrcModuleName(), strSrcAddr, nSrcPort);
    else
        LogDI("Receive %s from %s(%s:%d)", pMsg->toString(), pMsg->GetSrcModuleName(), strSrcAddr, nSrcPort);

    ProcessMessage(pMsg);
}

// 接收从车端的消息
void CRSUModule::DecodeV2XMessage(PUCHAR pBuffer, UINT nMsgLen)
{
    LogI("DecodeV2XMessage 待建设");
}

// 接收GNSS消息
void CRSUModule::DecodeGNSSMessage(PUCHAR pBuffer, UINT nMsgLen)
{
    LogI("DecodeGNSSMessage 待建设");
}

// 接收CAN消息
void CRSUModule::DecodeCANMessage(PUCHAR pBuffer, UINT nMsgLen)
{
    LogI("DecodeCANMessage 待建设");
}

// 接收从边缘服务器来的消息
void CRSUModule::DecodeV2IMessage(PUCHAR pBuffer, UINT nMsgLen, LPCSTR strSrcAddr, UINT nSrcPort)
{
    CBaiduMessage msg;

    if ( msg.Decode(pBuffer, nMsgLen) == CODEC_FAILED )
    {
        LogE("RSU decode V2X message failed. Message will be discard.");
        LogI("%s", ConvertToBit(pBuffer, nMsgLen * 8));
        return;
    }

    CMainMsgFrame* pMsgFrame = msg.GetMainMsgFrame();
    ProcessMainMsgFrame(pMsgFrame);
    return;
}

// 接收到边缘服务器的消息后，添加网络层头然后转发给车端
// 网络层头定义待确定
void CRSUModule::ProcessMainMsgFrame(CMainMsgFrame* pMsg)
{
    if ( pMsg == NULL )
    {
        LogE("ProcessMainMsgFrame parameter pointer is NULL");
        return;
    }

    CCSAEMsgBase* pMsgFrame = NULL;
    int nChoiceIndex = -1;
    pMsgFrame = pMsg->GetObject(nChoiceIndex);
    if ( pMsgFrame == NULL )
    {
        LogE("CMainMsgFrame Get frame failed.");
        return;
    }

    switch(nChoiceIndex)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        SendMessageToVehicle(pMsg);
        break;

    default:
        LogE("CMainMsgFrame choice index %d is out of range [0, 4]", nChoiceIndex);
        return;
    }

    return;
}

void CRSUModule::SendMessageToVehicle(CMainMsgFrame* pMsg)
{
    CString strAddr;
    UINT nPort;

    PUCHAR pBuffer;
    USHORT nBitLen;

    pBuffer = pMsg->GetBuffer(nBitLen);
    UINT nMsgLen = nBitLen / 8 + 1;

    // 通过通信模组发送消息给车端
    // m_DMA3ADriver.V2X_SendMessage(pBuffer, nMsgLen);
}


// RSU Manager消息处理
void CRSUModule::ProcessMessage(CMsgBase* pMsg)
{
    if ( pMsg == NULL )
    {
        LogE("RSUModule Receive a NULL Message");
        return;
    }

    switch ( pMsg->GetMsgType() )
    {
    case MSG_HEARTBEAT_ACK:
        ProcessHeartbeatAckMsg(pMsg);
        break;
    case RSU_MSG_InitialRequestAck:
        ProcessInitialRequestAckMsg(pMsg);
        break;
    case RSU_MSG_UpdateSoftware:
        ProcessUpdateSoftwareMsg(pMsg);
        break;
    case RSU_MSG_Reboot:
        ProcessRebootMsg(pMsg);
        break;
    case RSU_MSG_ReportStatusAck:
        ProcessReportStatusAckMsg(pMsg);
        break;

    default:
        LogE("Receive unsupport message %s from %s:%d", pMsg->toString(), pMsg->GetSrcAddr(), pMsg->GetSrcPort());
        return;
    }

    return;
}

// 只给RSU管理端发送初始化请求消息
void CRSUModule::SendInitialRequestMsg()
{
    CRSUInitialRequestMsg msg;

    int nLng, nLat;

    m_DMA3ADriver.GetDevicePosition(nLng, nLat, m_strRSUID);

    msg.SetParam(nLng, nLat);

    SendMessageToManager(&msg);
}

// 接收初始化请求响应消息
bool CRSUModule::ProcessInitialRequestAckMsg(CMsgBase* pMsg)
{
    if ( CModuleBase::ProcessInitialRequestAckMsg(pMsg) == false )
        return false;
    
    return true;
}

// 接收更新系统软件消息
void CRSUModule::ProcessUpdateSoftwareMsg(CMsgBase* pMsg)
{
    if(pMsg == NULL)
    {
        LogE("RSUModule Receive a NULL UpdateSoftwareMsg");
        return;
    }

    // 发送响应消息
    CRSUUpdateSoftwareAckMsg msg;
    SendMessageToManager(&msg);

    CRSUUpdateSoftwareMsg* pRequestMsg = (CRSUUpdateSoftwareMsg*)pMsg;

    CString strFileName;
    pRequestMsg->GetParam(strFileName, m_nUpdateMode);

    // 默认为立即升级模式
    LogI("Update Mode: %d", m_nUpdateMode);
    if ( m_nUpdateMode != Update_Immediately && m_nUpdateMode != Update_NextStart)
        m_nUpdateMode = Update_Immediately;

    LogI("Receive %s, Update File: %s, mode: %s", pMsg->toString(), strFileName.GetBuffer(), 
        ( m_nUpdateMode == Update_NextStart ) ? "Update after reboot":"Update immediately");

    // 下载升级文件
    CString strRemoteFile;
    strRemoteFile.Format("%s/%s", SERVER_UPDATE_PATH, strFileName.GetBuffer());

    CString strLocalFile;
    strLocalFile.Format("%s/update/%s", g_strAppRootPath.GetBuffer(), strFileName.GetBuffer());

    if ( m_DownloadFile.InsertDownloadTask(RSU_MSG_UpdateSoftware, strRemoteFile, strLocalFile) == false )
    {
        LogE("Can not create a download task.");
        return;
    }

    // 在升级文件下载完成后进行升级

    return;
}

// 系统重启消息
void CRSUModule::ProcessRebootMsg(CMsgBase* pMsg)
{
    if(pMsg == NULL)
    {
        LogE("ProcessRebootMsg Receive a NULL Msg Pointer");
        return;
    }

    CRSURebootMsg* pRequestMsg = (CRSURebootMsg*)pMsg;
    UINT nDelayTime;
    pRequestMsg->GetParam(nDelayTime, m_nRebootMode);

    LogI("Receive %s, DelayTime: %d, RebootMode: %s", pMsg->toString(), nDelayTime, (m_nRebootMode == Reboot_Application) ? "reload application" : "reboot server");

    // 发送响应消息
    CRSURebootAckMsg msg;
    SendMessageToManager(&msg);

    // 系统重启
    if ( nDelayTime == 0 )
    {
        // 调用基类函数
        OnRebootSystem();
    }
    else
    {
        // 设置一个定时器，定时重启
        m_nRebootTimer = g_Timer.CreateTimer( nDelayTime * 1000, CModuleBase::TimerEvent, this );
        if ( m_nRebootTimer == INVALID_TIMER_ID )
        {
            LogE("Create Reboot Timer failed.");
            return;
        }

        LogI("Create Reboot Timer successfully.");
    }

    return;
}

void CRSUModule::ProcessReportStatusAckMsg(CMsgBase* pMsg)
{
    // 无需操作
}


// 定时任务处理
void CRSUModule::OnTimer(TIMER_ID nTimerID)
{
    // 调用基类的定时任务处理函数
    CModuleBase::OnTimer(nTimerID);

    if ( nTimerID == m_nRebootTimer)
    {
        g_Timer.CloseTimer(m_nRebootTimer);
        m_nRebootTimer = INVALID_TIMER_ID;

        // 调用基类的重启函数
        OnRebootSystem();
    }

    if ( nTimerID == m_nDeviceStatusReportTimer )
    {
        // 定时上报设备状态
        onReportDeviceStatus();
    }
}

// 定时上报设备状态
void CRSUModule::onReportDeviceStatus()
{
    UINT nCPURate, nMemoryRate, nDiskRate;
    g_SystemBaseInfo.GetSystemLoad(nCPURate, nMemoryRate, nDiskRate);

    CRSUReportStatusMsg msg;

    msg.SetParam(nCPURate, nMemoryRate, nDiskRate);

    SendMessageToManager(&msg);
}

// 下载任务完成后处理
void CRSUModule::OnDownloadComplete(UINT nMsgType, CString strLocalFile)
{
    CModuleBase::OnDownloadComplete(nMsgType, strLocalFile);

    if ( nMsgType == RSU_MSG_UpdateSoftware )
    {
        // 升级文件下载完成，生成升级配置文件 update.conf
        CString filename = g_strAppRootPath;
        filename += "/conf/Update.conf";

        CString content = "[Update]\nUpdateFile = ";
        content += strLocalFile.GetFileName();

        CFile file;
        // 创建升级文件，不管以前的升级文件是否存在
        file.OpenFile(filename, "w");

        file.WriteLine(content);
        file.Close();

        // 如果是立即启动模式，则调用基类函数进行升级操作
        if ( m_nUpdateMode == Update_Immediately )
            CheckUpdate();
        else
            // 如果不是，则在下次升级时执行升级操作
            LogI("System will update on next start...");
    }

}

void CRSUModule::SendMessageToManager(CMsgBase* pMsg)
{
    if ( pMsg == NULL )
    {
        LogE("CRSUModule::SendMessageToManager parameter is null");
        return;
    }

    pMsg->SetSrcAddr(m_strLocalAddr);
    pMsg->SetSrcPort(m_nRSUClientPort);
    pMsg->SetSrcModuleType(MODULE_RSU);
    pMsg->SetSrcModuleCode(m_strModuleCode);

    pMsg->SetDestAddr(m_strRSUManagerAddr);
    pMsg->SetDestPort(m_nRSUManagerPort);
    pMsg->SetDestModuleType(MODULE_RSU_MANAGER);
    pMsg->SetDestModuleCode("RSU_MANAGER_001");

    m_RSUManagerClient.SendMessage(pMsg);
}

