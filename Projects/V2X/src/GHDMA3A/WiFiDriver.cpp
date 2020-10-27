// ------------------------------------------------------------------------------
// RSU Driver - Component :: V2X WiFi Driver definition
// Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    WiFiDriver.cpp
// Purpose: RSU module
//-------------------------------------------------------------------------------

#include "WiFiDriver.h"
#include "Config.h"

CDMA3AWiFiDriver::CDMA3AWiFiDriver()
{
    m_strDeviceName = "";
}

CDMA3AWiFiDriver::~CDMA3AWiFiDriver()
{
}

bool CDMA3AWiFiDriver::InitialBorad(MSG_CALLBACK pCallback, LPVOID pCallbackParam)
{
    if ( LoadConfig() == false )
    {
        LogE("CDMA3AWiFiDriver LoadConfig failed");
        return false;   
    }

    if ( pCallbackParam == NULL || pCallbackParam == NULL )
    {
        LogE("CDMA3AWiFiDriver::InitialBorad parameters is NULL pointer");
        return false;
    }

    LogI("需要对WiFi进行测试");
    return true;

    m_pMsgCallBack      = pCallback;
    m_pCallbackParam    = pCallbackParam;

    int nRetCode = pthread_create(&m_ThreadID, NULL, CDMA3AWiFiDriver::DriverThread, (LPVOID)this);
    if ( nRetCode != 0)
    {
        LogE("CDMA3AWiFiDriver::InitialBorad Create Thread error, code: %d", nRetCode);
        return false;
    }

    return true;
}

bool CDMA3AWiFiDriver::LoadConfig()
{
    // 加载默认配置参数
    CString strCfgFile = g_strAppRootPath;

    strCfgFile = strCfgFile + "/conf/v2x.conf";

    CConfig configFile(strCfgFile);

    CString strSection = "COMMON";
    CString strValue, strKey;
    UINT nValue;

    strKey = "WiFiDevice";
    strValue = configFile.GetConfigStr(strSection, strKey, "");
    if ( strValue == "")
    {
        LogE("Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }

    m_strDeviceName = strValue;

    return true;
}

void* CDMA3AWiFiDriver::DriverThread(LPVOID pParam)
{
    if ( pParam == NULL)
    {
        LogE("Msg Client thread parameters error.");
        return NULL;
    }

    CDMA3AWiFiDriver *pDMA3ACANDriver = (CDMA3AWiFiDriver*)pParam;
    pDMA3ACANDriver->StartDriver();

    return NULL;
}

// 查询是否接收到消息
void CDMA3AWiFiDriver::StartDriver()
{

}
