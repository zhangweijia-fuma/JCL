// ------------------------------------------------------------------------------
// RSU Driver - Component :: DT DMA3A Board Driver
// Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    GHDMA3ADriver.cpp
// Purpose: RSU module
//-------------------------------------------------------------------------------

#include "GHDMA3ADriver.h"
#include "Debug.h"
#include "Common.h"
#include "Config.h"

#include "v2x.h"
#include "v2x_ioctl.h"
#include "V2XPrimitive.h"

int period2sleep[13] = {
    5,
    100,200,300,400,500,600,700,800,900,1000,
    20,
    50
};

CGHDMA3ADriver::CGHDMA3ADriver()
{
    m_pMsgCallBack = NULL;                             // 消息回调函数
    m_pCallbackParam    = NULL;
    m_ThreadID          = -1;
    m_strV2XDeviceName  = "";
    m_V2XSocket = -1;
}

bool CGHDMA3ADriver::InitialBorad(MSG_CALLBACK pV2XCallBack, MSG_CALLBACK pGNSSCallBack, MSG_CALLBACK pCANCallBack, LPVOID pCallbackParam)
{
    if ( LoadConfig() == false )
    {
        LogE("CGHDMA3ADriver LoadConfig failed");
        return false;   
    }

    if ( pCallbackParam == NULL || pCallbackParam == NULL )
    {
        LogE("CGHDMA3ADriver::InitialBorad parameters is NULL pointer");
        return false;
    }

    m_pMsgCallBack      = pV2XCallBack;
    m_pCallbackParam    = pCallbackParam;

    int nRetCode = pthread_create(&m_ThreadID, NULL, CGHDMA3ADriver::DriverThread, (LPVOID)this);
    if ( nRetCode != 0 )
    {
        LogE("CGHDMA3ADriver::InitialBorad Create Thread error, code: %d", nRetCode);
        return false;
    }

    if ( m_WiFiDriver.InitialBorad(pGNSSCallBack, pCallbackParam) == false )
    {
        LogE("CGHDMA3ADriver Initial WiFi failed");
        return false;
    }

    if ( m_CanDriver.InitialBorad(pCANCallBack, pCallbackParam) == false )
    {
        LogE("CGHDMA3ADriver Initial CAN failed");
        return false;
    }

    if ( m_GNSSDriver.InitialBorad(pGNSSCallBack, pCallbackParam) == false )
    {
        LogE("CGHDMA3ADriver Initial GNSS failed");
        return false;
    }

    LogI("需要对V2X进行测试...");
    return true;

    // 初始化V2X Socket
    m_V2XSocket = socket(AF_V2X, SOCK_RAW, V2XPROTO_ADAP);
    if (m_V2XSocket < 0) 
    {
        LogE("Create v2x socket failed, errno %d, %s", errno, strerror);
        return false;
    }

    struct sockaddr_v2x addr_v2x;

    memset(&addr_v2x, 0, sizeof(addr_v2x));
    addr_v2x.family = AF_V2X;
    addr_v2x.options[0] = 1;
    addr_v2x.ifindex = GetV2XDeviceInterface();
    socklen_t slen = sizeof(struct sockaddr_v2x);

    struct user_register_aids {
        int cnt;
        int aids[11];
    } user_reg_aids;

    user_reg_aids.cnt = 11;
    user_reg_aids.aids[0] = 7;
    user_reg_aids.aids[1] = 8;
    user_reg_aids.aids[2] = 9;
    user_reg_aids.aids[3] = 10;
    user_reg_aids.aids[4] = 11;
    user_reg_aids.aids[5] = 12;
    user_reg_aids.aids[6] = 13;
    user_reg_aids.aids[7] = 14;
    user_reg_aids.aids[8] = 15;
    user_reg_aids.aids[9] = 16;
    user_reg_aids.aids[10] = 17;

    int ret = bind(m_V2XSocket, (struct sockaddr *)&addr_v2x, slen);
    LogD("Bind lmi40[if=%d] ret %d, %d!",addr_v2x.ifindex, ret,  errno);

    ret = ioctl(m_V2XSocket, REGAIDS, &user_reg_aids);
    LogD("REGAIDS ret %d, %d", ret,  errno);
    user_reg_aids.cnt = 0;
    ret = ioctl(m_V2XSocket, GETREGDAIDSCNT, &user_reg_aids);
    LogD("GETREGDAIDSCNT ret %d, cnt %d, %d", ret, user_reg_aids.cnt, errno);

    int *regd_aids = NULL;

    regd_aids = (int*)malloc((user_reg_aids.cnt + 1)*sizeof(int));
    regd_aids[0] = 0;
    ret = ioctl(m_V2XSocket, GETREGDAIDS, regd_aids);
    LogD("GETREGDAIDS ret %d, cnt %d, last %d, errno: %d", ret, regd_aids[0], regd_aids[regd_aids[0]], errno);
    free(regd_aids);

    return true;
}

bool CGHDMA3ADriver::LoadConfig()
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

    strKey = "V2XDevice";
    strValue = configFile.GetConfigStr(strSection, strKey, "");
    if ( strValue == "")
    {
        LogE("Can not load config parameters %s.", strKey.GetBuffer());
        return false;
    }
    m_strV2XDeviceName = strValue;

    return true;
}

// 获取V2X网卡序列号, 目前设备上只有lmi40具备v2xv2x收发能力，序列号通过ioctl命令字获取
int CGHDMA3ADriver::GetV2XDeviceInterface()
{
    struct ifreq ifr;
    
    memset(&ifr, 0, sizeof(ifr));

    strcpy(ifr.ifr_name, m_strV2XDeviceName.GetBuffer());
    ioctl(m_V2XSocket, SIOCGIFINDEX, &ifr);

    return ifr.ifr_ifindex;
}

void* CGHDMA3ADriver::DriverThread(LPVOID pParam)
{
    if ( pParam == NULL )
    {
        LogE("Msg Client thread parameters error.");
        return NULL;
    }

    CGHDMA3ADriver *pGHDMA3ADriver = (CGHDMA3ADriver*)pParam;
    pGHDMA3ADriver->StartDriver();

    return NULL;
}

// 查询是否接收到消息
void CGHDMA3ADriver::StartDriver()
{
    while(1)
    {
        V2X_ReceiveMessage();
    }
}

void CGHDMA3ADriver::V2X_ReceiveMessage()
{
    fd_set fdMsgFlag;
    timeval tmWaitTime;

    FD_ZERO( &fdMsgFlag );

    FD_SET( m_V2XSocket, &fdMsgFlag );

    // 检查是否有数据到达的时间间隔，60*60*24
    tmWaitTime.tv_sec  = 60;
    tmWaitTime.tv_usec = 0;

    int nTemp = select(FD_SETSIZE, &fdMsgFlag, (fd_set*)0, (fd_set*)0, &tmWaitTime );

    if ( nTemp <= 0 )
        return;

    struct v2x_adap_ind_addl *ind_addl = NULL;

    struct sockaddr_v2x addr;
    memset(&addr, 0, sizeof(addr));
    addr.family = AF_V2X;

    memset(addr.aid.bytes, 0xff, AID_BYTES);

    UCHAR buffer[5120];
    memset(buffer, 0, 5120);

    socklen_t slen = sizeof(struct sockaddr_v2x);
    int nLen = recvfrom(m_V2XSocket, buffer, 5120, 0, (struct sockaddr *)&addr, &slen);
    if (nLen > 0)
    {
        ind_addl = (struct v2x_adap_ind_addl *)&addr.options;

        LogD("Receive from V2X side: AID[%d] UTC[%llu] PSN[%u]...", ntohs(addr.aid.be_word), ind_addl->utc, ind_addl->psn);
        LogD("Data: %s", ConvertToHex(buffer, nLen));

        m_pMsgCallBack(m_pCallbackParam, buffer, nLen);

        return;
    }
    else
    {
        LogE("recvfrom return: %d", nLen);
    }

    return;
}

bool CGHDMA3ADriver::V2X_SendMessage(CPrimitive* pPrimitiveList)
{
    if ( pPrimitiveList == NULL )
    {
        LogE("V2X_SendMessage Parameters is Wrong: NULL Pointer");
        return false;
    }

    if ( pPrimitiveList->Encode() == false)
    {
        LogE("Encode Primitive failed");
        return false;
    }

    PUCHAR pBuffer = pPrimitiveList->GetMsgBuffer();
    int nLen = pPrimitiveList->GetMsgLen();

    LogI("V2X_SendMessage 待处理。。。");
    return true;
}

int CGHDMA3ADriver::V2X_IOCtrl(int cnt)
{
    unsigned char ioctl_buf[4096] = {0};

    struct register_aids reg_aids;
    int ret = 0;
    int *regd_aids = NULL;
    unsigned long long utc;
    unsigned int psn;
    unsigned int psn_delay[2] = {0};
    ret = ioctl(m_V2XSocket, GETREGDAIDSCNT, &reg_aids);
    // LogD("ioctl[%d] GETREGDAIDSCNT ret %d, cnt %d\n", cnt, ret, reg_aids.cnt);

    memset(ioctl_buf, 0, sizeof(ioctl_buf));
    regd_aids = (int*)ioctl_buf;

    if (regd_aids)
    {
        ret = ioctl(m_V2XSocket, GETREGDAIDS, regd_aids);
        // LogD("ioctl[%d] GETREGDAIDS ret %d, cnt %d\n", cnt, ret, regd_aids[0]);
    }
    else
    {
        // LogD("ioctl[%d] GETREGDAIDS calloc failed\n", cnt);
    }

    ret = ioctl(m_V2XSocket, GETLASTPSN, &psn);
    // LogD("ioctl[%d] GETLASTPSN ret %d, psn %u\n", cnt, ret, psn);
    ret = ioctl(m_V2XSocket, GETLASTUTC, &utc);
    // LogD("ioctl[%d] GETLASTUTC ret %d, utc %llu\n", cnt, ret, utc);
    ret = ioctl(m_V2XSocket, GETLASTXMITEDPSN, &psn);
    // LogD("ioctl[%d] GETLASTXMITEDPSN ret %d, psn %u\n", cnt, ret, psn);
    ret = ioctl(m_V2XSocket, GETLASTXMITEDUTC, &utc);
    // LogD("ioctl[%d] GETLASTXMITEDUTC ret %d, utc %llu\n", cnt, ret, utc);
    ret = ioctl(m_V2XSocket, GETNEXTPSN, &psn);
    // LogD("ioctl[%d] GETNEXTPSN ret %d, psn %u\n", cnt, ret, psn);
    ret = ioctl(m_V2XSocket, GETLASTPSNDELAY, psn_delay);
    // LogD("ioctl[%d] GETLASTPSNDELAY ret %d, psn %u, delay %uus\n", cnt, ret, psn_delay[0], psn_delay[1]);

    return 0;
}


void CGHDMA3ADriver::GetDevicePosition(int& nLng, int & nLat, CString strID)
{
    LogI("临时设置RSU经纬度");
    if ( strID == "20201001")
    {
        nLng = 103541307;
        nLat = 31030380;
    }
    else if ( strID == "20201002")
    {
        nLng = 103947359;
        nLat = 30753566;
    }
    else if ( strID == "20201003")
    {
        nLng = 104075098;
        nLat = 30571608;
    }
    else
    {
        nLng = 104085447;
        nLat = 30572058;
    }

}

void CGHDMA3ADriver::GetTHData(int& nTemperature, int& nHumidity)
{
    nTemperature = 0;
    nHumidity    = 0;
}

