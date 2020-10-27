// ------------------------------------------------------------------------------
// JCCL - Component :: System Base Information
// Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    SystemBaseInfo.cpp
// Purpose: Common Module
//-------------------------------------------------------------------------------

#include "SystemBaseInfo.h"
#include "Debug.h"
#include "Common.h"

#define MAX_INTERFACE       16                                          // 网卡最大数量

// SIOCGIFFLAGS, SIOCSIFFLAGS 读取 或 设置 设备的 活动标志字. ifr_flags 包含 下列值 的 屏蔽位:
// IFF_UP                      接口正在运行.
// IFF_BROADCAST               有效的广播地址集.
// IFF_DEBUG                   内部调试标志.
// IFF_LOOPBACK                这是自环接口.
// IFF_POINTOPOINT             这是点到点的链路接口.
// IFF_RUNNING                 资源已分配.
// IFF_NOARP                   无arp协议, 没有设置第二层目的地址.
// IFF_PROMISC                 接口为杂凑(promiscuous)模式.
// IFF_NOTRAILERS              避免使用trailer .
// IFF_ALLMULTI                接收所有组播(multicast)报文.
// IFF_MASTER                  主负载平衡群(bundle).
// IFF_SLAVE                   从负载平衡群(bundle).
// IFF_MULTICAST               支持组播(multicast).
// IFF_PORTSEL                 可以通过ifmap选择介质(media)类型.
// IFF_AUTOMEDIA               自动选择介质.
// IFF_DYNAMIC                 接口关闭时丢弃地址.


// 定义此变量，仅用于在系统启动时能够自动获取IP地址，而不需要应用单独调用函数GetLocalAddr();
CSystemBaseInfo g_SystemBaseInfo;
CSystemBaseInfo::CSystemBaseInfo()
{
    GetLocalAddr();
}

// 获取本机IP地址、MAC地址、子网掩码
// 只获取最后一个UP的非LoopBack网卡地址信息，
bool CSystemBaseInfo::GetLocalAddr()
{
    int fd;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd <= 0)  
    {
        LogE("Can not open a socket.");
        return false;
    } 

    struct ifreq buf[MAX_INTERFACE];
    struct ifconf ifc;
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = (caddr_t) buf;
    if ( ioctl(fd, SIOCGIFCONF, (char*)&ifc))
    {
        LogE("Get local network config infomation failed.");
        close(fd);
        return false;
    }

    int nInterfaceCount = ifc.ifc_len / sizeof(struct ifreq);
    LogD("Interface number: %d", nInterfaceCount);
    for(int i = 0; i < nInterfaceCount; i++)
    {
        if ( ioctl(fd, SIOCGIFFLAGS, (char*)&buf[i]) )
            continue;

        // Loopback或没有UP/Running的网卡不进行处理
        if ( (buf[i].ifr_flags & IFF_LOOPBACK) || !(buf[i].ifr_flags & IFF_UP) || !(buf[i].ifr_flags & IFF_RUNNING) )
            continue;

        if ( ioctl(fd, SIOCGIFADDR, (char*)&buf[i]) )
            continue;

        g_strLocalIPAddr = inet_ntoa(((struct sockaddr_in *)(&buf[i].ifr_addr))->sin_addr);

        if ( ioctl(fd, SIOCGIFHWADDR, (char*)&buf[i]) )
        {
            LogD("Get Mac address ioctl error");
            g_strLocalMACAddr   = "";
            g_strLocalIPAddr    = "";
            g_strLocalNetMask   = "";
            g_strNetworkName    = "";
            continue;  
        }

        g_strLocalMACAddr.Format("%02x:%02x:%02x:%02x:%02x:%02x",
           (unsigned char)buf[i].ifr_hwaddr.sa_data[0],
           (unsigned char)buf[i].ifr_hwaddr.sa_data[1],
           (unsigned char)buf[i].ifr_hwaddr.sa_data[2],
           (unsigned char)buf[i].ifr_hwaddr.sa_data[3],
           (unsigned char)buf[i].ifr_hwaddr.sa_data[4],
           (unsigned char)buf[i].ifr_hwaddr.sa_data[5]
        );

        g_strNetworkName = buf[i].ifr_name;

        struct ifreq ifr_mask; 
        memset(&ifr_mask, 0, sizeof(ifr_mask));     
        strncpy(ifr_mask.ifr_name, buf[i].ifr_name, sizeof(ifr_mask.ifr_name )-1);

        if( (ioctl( fd, SIOCGIFNETMASK, &ifr_mask ) ) < 0 )   
        {
            LogD("Get NetMask ioctl error");
            g_strLocalMACAddr   = "";
            g_strLocalIPAddr    = "";
            g_strLocalNetMask   = "";
            g_strNetworkName    = "";
            continue;  
        }

        struct sockaddr_in *net_mask = ( struct sockaddr_in * )&( ifr_mask.ifr_netmask );  
        g_strLocalNetMask = inet_ntoa( net_mask -> sin_addr );
    }

    if ( g_strLocalIPAddr == "" || g_strLocalMACAddr == "" || g_strLocalNetMask == "")
    {
        LogE("Can not find a UP network interface.");
        close(fd);
        return false;
    }

    close(fd);

    return true;
}

UINT CSystemBaseInfo::GetMemoryRate()
{
    FILE *fd;          
    char buf[256];
    char name[128];
    UINT nTotal, nFree;
                                                                                                 
    fd = fopen("/proc/meminfo", "r"); 

    fgets(buf, sizeof(buf), fd);
    sscanf(buf, "%s %d", name, &nTotal);
    fgets(buf, sizeof(buf), fd); 
    sscanf(buf, "%s %d", name, &nFree); 
    fclose(fd);

    UINT nRate = ((nTotal - nFree) * 10000 / nTotal);

    return nRate;
}

UINT CSystemBaseInfo::GetCPURate() //对无类型get函数含有一个形参结构体类弄的指针O
{   
    FILE *fd;   
    char buf[256];
    char name[128];
    unsigned int nUser, nNice, nSystem, nIdle, nTotal, nUsed;
                                                                                                               
    fd = fopen("/proc/stat", "r"); 
    fgets(buf, sizeof(buf), fd);
    sscanf(buf, "%s %u %u %u %u", name, &nUser, &nNice, &nSystem, &nIdle);
    fclose(fd);

    nTotal = nUser + nNice + nSystem + nIdle;// 用户+优先级+系统+空闲
    nUsed = nUser + nNice + nSystem;
    UINT nRate = nUsed * 10000 / nTotal;

    return nRate;
}

// df / 命令输出：
// 文件系统          1K-块     已用     可用 已用% 挂载点
// /dev/sda1      80374600 12064580 64204192   16% /
UINT CSystemBaseInfo::GetDiskRate()
{
    FILE * fd;
    char filesystem[128], available[128], use[128], mounted[128], buf[256]; 
    
    float used, blocks;

    fd = popen("df /","r");

    fgets(buf, 256, fd);
    fgets(buf, 256, fd);

    float dev_total = 0,dev_used = 0;

    sscanf(buf, "%s %f %f %s %s %s", filesystem, &blocks, &used, available, use, mounted);

    dev_total += blocks;
    dev_used += used;

    pclose(fd);

    UINT nRate = (dev_used) * 10000 /(dev_total) ;

    return nRate;
}

void CSystemBaseInfo::GetSystemLoad(UINT& nCPURate, UINT& nMemoryRate, UINT& nDiskRate)
{
    nCPURate    = GetCPURate();
    nMemoryRate = GetMemoryRate();
    nDiskRate   = GetDiskRate();

    return;
}

