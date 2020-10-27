// ------------------------------------------------------------------------------
// JCCL - Component :: Common
// Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    Common.h
// Purpose: Common Module
//-------------------------------------------------------------------------------

// 在本文件中定义的各种类型、宏等，为所有业务共同使用


#ifndef _COMMON_H_
#define _COMMON_H_

#include "JCCLTypes.h"

#include "CString.h"
#include "SystemBaseInfo.h"

// 定义int,short,byte之间的转换
union IntType
{
    long long nLongValue;
    int nIntValue;
    short nShortValue[4];
    UCHAR nByteValue[8];
};

union UINTType
{
    unsigned long long nLongValue;
    UINT nIntValue;
    USHORT nShortValue[4];
    UCHAR nByteValue[8];
};

const char*                 GetClassName(CObjBase*);                    // 获得对象类名称

extern char*                GetSystemTime(const char* DateConnecter="-", const char* MiddleConnector=" ", const char* timeConnector=":");

#ifndef _WINDOWS
// extern bool                 GetLocalAddr();
#endif  // _WINDOWS

char*                       ConvertToHex(unsigned char *pData, UINT nLen);

bool                        GetBit(UCHAR nValue, USHORT nBitIndex);
bool                        GetBit(int nValue, USHORT nBitIndex);
bool                        GetBit(long long nValue, USHORT nBitIndex);
bool                        GetBit(UINT nValue, USHORT nBitIndex);
bool                        GetBit(USHORT nValue, USHORT nBitIndex);

void                        SetBit(UCHAR &nValue, USHORT nBitIndex, bool nBitValue);
void                        SetBit(int &nValue, USHORT nBitIndex, bool nBitValue);
void                        SetBit(long long &nValue, USHORT nBitIndex, bool nBitValue);
void                        SetBit(UINT &nValue, USHORT nBitIndex, bool nBitValue);
void                        SetBit(USHORT &nValue, USHORT nBitIndex, bool nBitValue);

// 在按二进制输出数据时，是按照字节从低到高，还是从高到低
enum BIT_STRING_SEQUENCE
{
    BSS_HIGH_LOW,
    BSS_LOW_HIGH,
};

// 默认字节顺序为从低到高
char*                       ConvertToBit(UCHAR *pData, USHORT nBitLen, USHORT nStartBitPos = 0, BIT_STRING_SEQUENCE bByteSequence = BSS_LOW_HIGH);

const char*                 GetModuleTypeStr(unsigned char nModuleType);
void                        GetSystemLoad(UINT& nCPURate, UINT& nMemoryRate, UINT& nDiskRate);

// 定义所有业务模块的模块类型
enum MODULE_TYPE
{
    MODULE_VCCU                 = 0x01,                                 // PIS VCCU
    MODULE_DVCU,                                                        // PIS DVCU
    MODULE_PECU,                                                        // PIS PECU
    MODULE_SPCU,                                                        // PIS SPCU

    MODULE_SIMULATOR,                                                   // 仿真器
    MODULE_TESTER,

    MODULE_PIS_CONTROLLER,                                              // PIS 播放控制器

    MODULE_CCU,                                                         // TMS CCU
    MODULE_DDU,                                                         // TMS 监控显示模块
    MODULE_DCU,                                                         // TMS 数据采集模块
    MODULE_CCDU,                                                        // TMS 控制显示模块，CCU和DDU合并

    MODULE_RSU,                                                         // RSU
    MODULE_EDGE_SERVER,                                                 // 边缘服务器
    MODULE_CLOUD_SERVER,                                                // 云端服务器
    MODULE_OBU,                                                         // 车载OBU
    MODULE_RSU_MANAGER,                                                 // RSU 管理端
    
    MODULE_UNKNOWN
};

// 定义所有业务使用的消息类型
enum MSG_TYPE 
{
    MSG_HEARTBEAT               = 0,                                    // 心跳消息
    MSG_HEARTBEAT_ACK           = 1,                                    // 心跳响应消息

    // PIS 消息
    MSG_SEND_PROGRAM_LIST       = 2,                                    // 发送节目单
    MSG_SEND_PROGRAM_LIST_ACK   = 3,                                    // 发送节目单响应
    MSG_SEND_NEWS               = 4,                                    // 发送新闻
    MSG_SEND_NEWS_ACK           = 5,                                    // 发送新闻响应
    // MSG_CLEAR_NEWS              = 6,                                    // 清除新闻
    // MSG_CLEAR_NEWS_ACK          = 7,                                    // 清除新闻响应
    MSG_SEND_EMERGENCE          = 8,                                    // 发送紧急通知
    MSG_SEND_EMERGENCE_ACK      = 9,                                    // 发送紧急通知响应
    // MSG_CLEAR_EMERGENCE         = 10,                                   // 清除紧急通知
    // MSG_CLEAR_EMERGENCE_ACK     = 11,                                   // 清除紧急通知响应
    MSG_SEND_LINE_INFO          = 12,                                   // 发送运营线路基本信息
    MSG_SEND_LINE_INFO_ACK      = 13,                                   // 发送运营线路基本信息响应
    MSG_SEND_STATION_TIME       = 14,                                   // 发送到站信息
    MSG_SEND_STATION_TIME_ACK   = 15,                                   // 发送到站信息响应
    MSG_SEND_FORMAT_FILE        = 16,                                   // 发送版式文件
    MSG_SEND_FORMAT_FILE_ACK    = 17,                                   // 发送版式文件响应
    MSG_SYSTEM_REBOOT           = 18,                                   // 系统重启
    MSG_SYSTEM_REBOOT_ACK       = 19,                                   // 系统重启响应
    MSG_RUN_STATUS              = 20,                                   // 控制器运行状态报告
    MSG_RUN_STATUS_ACK          = 21,                                   // 运行报告响应
    MSG_RUN_LOG                 = 22,                                   // 控制器日志报告
    MSG_RUN_LOG_ACK             = 23,                                   // 控制器日志报告响应
    MSG_SEND_UPDATE             = 24,                                   // 控制器软件更新
    MSG_SEND_UPDATE_ACK         = 25,                                   // 控制器软件更新响应
    MSG_INITIAL_REQUEST         = 26,                                   // 控制器初始化请求
    MSG_INITIAL_REQUEST_ACK     = 27,                                   // 控制器初始化请求响应
    MSG_UPDATE_WORK_TIME        = 28,                                   // 更新控制器工作时间
    MSG_UPDATE_WORK_TIME_ACK    = 29,                                   // 更新控制器工作时间响应
    MSG_VIEW_VIDEO              = 30,                                   // 查看控制器视频
    MSG_VIEW_VIDEO_ACK          = 31,                                   // 查看控制器视频响应
    MSG_SEND_SAFE_VIDEO         = 32,                                   // 发送安全垫片
    MSG_SEND_SAFE_VIDEO_ACK     = 33,                                   // 发送安全垫片响应
    MSG_DOWNLOAD_COMPLETE       = 34,                                   // 控制器在下载视频文件完成后，需要向服务器报告下载已经完成
    MSG_DOWNLOAD_COMPLETE_ACK   = 35,                                   // 下载完成响应


    EVENT_EMGC_BTN,
    MSG_EMGC_REQUEST,
    MSG_EMGC_ANSWER,
    MSG_EMGC_END,

    EVENT_BROADCAST_BTN,
    MSG_BROADCAST_REQUEST,
    MSG_BROADCAST_END,

    EVENT_SOS_BTN,
    EVENT_IC_BTN,
    MSG_DRIVER_COMM_REQUEST,
    MSG_DRIVER_COMM_ANSWER,
    MSG_DRIVER_COMM_END,

    EVENT_PA_ARRIVING,
    EVENT_PA_ARRIVED,
    EVENT_PA_LEAVING,
    EVENT_PA_PREV_STATION,
    EVENT_PA_NEXT_STATION,
    EVENT_PA_TEST1,
    EVENT_PA_TEST2,

    MSG_PA_ARRIVING,
    MSG_PA_ARRIVED,
    MSG_PA_LEAVING,
    MSG_PA_PREV_STATION,
    MSG_PA_NEXT_STATION,
    MSG_PA_TEST1,
    MSG_PA_TEST2,

    MSG_TCMS_ARRIVING,
    MSG_TCMS_LEAVING,
    MSG_TCMS_ARRIVED,
    MSG_TCMS_INFOMATION,

    MSG_TRAIN_STATION,

    EVENT_VOL_UP,
    EVENT_VOL_DOWN,

    MSG_CONNECT_SPEAKER,
    MSG_RELEASE_SPEAKER,

    MSG_SOFTWARE_UPDATE,

    MSG_MODULE_LIST,

    // 模块内部消息 
    EVENT_UPLOAD_FILE_END,                                              // 录音结束
    EVENT_PLAY_VOICE_END,                                               // 放音结束
    EVENT_PLAY_VIDEO_END,                                               // 视频播放结束

    // 仿真测试消息
    MSG_STATUS_REPORT,                                                  // 状态报告消息

    INVALID_MSG                 = 0xFFFF
};

// 错误代码定义
enum ERROR_CODE
{
    ERROR_NONE                  = 0,

    ERROR_NOT_LOGIN_FTP_SERVER,
    ERROR_FTP_FILE_FAILED,

    ERR_CAN_ADDR_ERROR,
    ERR_DEVICE_BUSY,

    ERR_END
};

#ifdef QT_CORE_LIB
#include <QString>
extern const char*          GetQString(QString str);
#endif

#endif // _COMMON_H_
