// ------------------------------------------------------------------------------
// RSU Driver - Component :: V2X CAN Driver definition
// Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    CanDriver.cpp
// Purpose: RSU module
//-------------------------------------------------------------------------------

#include <linux/can.h>
#include <linux/can/raw.h>
#include <unistd.h>

#include "CanDriver.h"
#include "Config.h"

CDMA3ACANDriver::CDMA3ACANDriver()
{
    m_strDeviceName     = "";
    m_nCANID            = -1;
    m_nCANSendHandle    = -1;
    m_nCANRecvHanlde    = -1;
}

CDMA3ACANDriver::~CDMA3ACANDriver()
{
    if ( m_nCANRecvHanlde > 0 ) close(m_nCANRecvHanlde);
    if ( m_nCANSendHandle > 0 ) close(m_nCANSendHandle);
}

bool CDMA3ACANDriver::InitialBorad(MSG_CALLBACK pCallback, LPVOID pCallbackParam)
{

    if ( LoadConfig() == false )
    {
        LogE("CDMA3ACANDriver LoadConfig failed");
        return false;   
    }

    if ( pCallbackParam == NULL || pCallbackParam == NULL )
    {
        LogE("CDMA3ACANDriver::InitialBorad parameters is NULL pointer");
        return false;
    }

    LogI("需要对CAN进行测试");
    return true;

    m_pMsgCallBack      = pCallback;
    m_pCallbackParam    = pCallbackParam;

    CString cmd;
    cmd.Format("ip link set %s type can bitrate 1000000 1>/dev/null 2>&1", m_strDeviceName.GetBuffer());
    system(cmd.GetBuffer());
    if ( errno > 0 )
    {
        LogI("Link to CAN device failed: %s", strerror(errno));
        return false;
    }

    cmd.Format("ip link set %s up 1>/dev/null 2>&1", m_strDeviceName.GetBuffer());
    system(cmd.GetBuffer());
    if ( errno > 0 )
    {
        LogI("Up CAN device failed, %s", strerror(errno));
        return false;
    }

    m_nCANSendHandle = CreateCANSocket(true);
    m_nCANRecvHanlde = CreateCANSocket(false);

    if ( m_nCANSendHandle == -1 || m_nCANRecvHanlde == -1 )
    {
        LogE("CDMA3ACANDriver::InitialBorad Create CAN socket failed");
        return false;
    }


    int nRetCode = pthread_create(&m_ThreadID, NULL, CDMA3ACANDriver::DriverThread, (LPVOID)this);
    if ( nRetCode != 0)
    {
        LogE("CGNSSDriver::InitialBorad Create Thread error, code: %d", nRetCode);
        return false;
    }

    return true;
}

bool CDMA3ACANDriver::LoadConfig()
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

    strKey = "CANDevice";
    strValue = configFile.GetConfigStr(strSection, strKey, "");
    if ( strValue == "")
    {
        LogE("Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }
    m_strDeviceName = strValue;

    return true;
}

void* CDMA3ACANDriver::DriverThread(LPVOID pParam)
{
    if ( pParam == NULL)
    {
        LogE("Msg Client thread parameters error.");
        return NULL;
    }

    CDMA3ACANDriver *pDMA3ACANDriver = (CDMA3ACANDriver*)pParam;
    pDMA3ACANDriver->StartDriver();

    return NULL;
}

// 查询是否接收到消息
void CDMA3ACANDriver::StartDriver()
{
    struct can_filter rfilter;

    /* <received_can_id> & mask == can_id & mask */
    rfilter.can_id   = 1;
    rfilter.can_mask = 1;

    setsockopt(m_nCANRecvHanlde, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

    while(1) 
    {
        struct sockaddr_can addr;
        struct ifreq ifr;
        socklen_t len = sizeof(addr);
        struct can_frame frame;
        int nbytes;
        int i;
        static unsigned int total_cnt = 0;

        nbytes = recvfrom(m_nCANRecvHanlde, &frame, sizeof(struct can_frame), 0, (struct sockaddr*)&addr, &len);

        /* get interface name of the received CAN frame */
        ifr.ifr_ifindex = addr.can_ifindex;
        ioctl(m_nCANRecvHanlde, SIOCGIFNAME, &ifr);
        LogD("Received a CAN frame from interface %s id %d count %u\n", ifr.ifr_name, frame.can_id, total_cnt++);
        LogD("SIZE %dBytes, DATA ", frame.can_dlc);
        m_pMsgCallBack(m_pCallbackParam, frame.data, frame.can_dlc);
      
    }
}

int CDMA3ACANDriver::GetCANInterface(int fd)
{
    struct ifreq ifr;
    
    memset(&ifr, 0, sizeof(ifr));

    strcpy(ifr.ifr_name, m_strDeviceName.GetBuffer());
    ioctl(fd, SIOCGIFINDEX, &ifr);

    m_nCANID = ifr.ifr_ifindex;

    return m_nCANID;
}

int CDMA3ACANDriver::CreateCANSocket(bool bIsSend)
{
    int can_fd;
    int ret;
    int if_index = 0;
    struct sockaddr_can addr;

    can_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (can_fd < 0) 
    {
        LogE("Try to create can socket failed, %s...", strerror(errno));
        return -1;
    }

    // 在发送消息时，不指定设备名
    if (bIsSend == false)
        if_index = GetCANInterface(can_fd);

    addr.can_family = AF_CAN;
    // index 0 means to bind to all interfaces 
    addr.can_ifindex = if_index;

    bind(can_fd, (struct sockaddr *)&addr, sizeof(addr));

    return can_fd;
}

void CDMA3ACANDriver::SendMessage()
{
    struct can_frame frame;
    memset(&frame, 0, sizeof(frame));

    struct sockaddr_can addr;
    socklen_t len = sizeof(addr);
    int nbytes;

    addr.can_ifindex = m_nCANID;
    addr.can_family  = AF_CAN;

    for (int i = 0; i < DEFAULT_SEND_TIMES; i++) 
    {
        frame.can_id    = 0;
        frame.can_dlc   = 8;
        frame.data[7]   = i;
        // send period 1ms
        usleep(1000);
        nbytes = sendto(m_nCANSendHandle, &frame, sizeof(struct can_frame), 0, (struct sockaddr*)&addr, sizeof(addr));
    }
}
