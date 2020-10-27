/*------------------------------------------------------------------------------
 * V2X - Component :: GB/T 31024-2 Specification
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:   GBT-31024-2_Spec.h
 * Purpose: V2X module, GB∕T 31024.2-2014 合作式智能运输系统 专用短程通信 第2部分：媒体访问控制层和物理层规范
 *----------------------------------------------------------------------------*/
#ifndef _GBT_31024_2_SPEC_H_
#define _GBT_31024_2_SPEC_H_

// 帧类型长度为1bit
#define GBT31024_Frame_ManagementControl 0                              // 管理控制帧
#define GBT31024_Frame_Data             1                               // 数据帧

// 帧子类型长度为5bit
// 管理控制帧子类型
// BCF和CM-REP是有序列号管理控制帧，其他为无序列号管理控制帧
#define GBT31024_Sub_Frame_BCF          0b00000                         // 广播控制帧，有序列号管理帧
#define GBT31024_Sub_Frame_RA_REQ       0b00001                         // 随机接入请求帧
#define GBT31024_Sub_Frame_RA_RSP       0b00010                         // 随机接入响应帧
#define GBT31024_Sub_Frame_OBC_REQ      0b00011                         // 终端基本能力协商请求帧
#define GBT31024_Sub_Frame_OBC_RSP      0b00100                         // 终端基本能力协商响应帧
#define GBT31024_Sub_Frame_DSA_REQ      0b00101                         // 动态业务建立请求帧
#define GBT31024_Sub_Frame_DSA_RSP      0b00110                         // 动态业务建立响应帧
#define GBT31024_Sub_Frame_DSC_REQ      0b00111                         // 动态业务修改请求帧
#define GBT31024_Sub_Frame_DSC_RSP      0b01000                         // 动态业务修改响应帧
#define GBT31024_Sub_Frame_DSD_REQ      0b01001                         // 动态业务删除请求帧
#define GBT31024_Sub_Frame_DSD_RSP      0b01010                         // 动态业务删除响应帧
#define GBT31024_Sub_Frame_RES_REQ      0b01011                         // 独立资源请求帧
// #define GBT31024_Sub_Frame_Reversed1    0b01100                         // 保留
#define GBT31024_Sub_Frame_CQI_FB       0b01101                         // 信道质量反馈帧
#define GBT31024_Sub_Frame_Reversed2    0b01110                         // 保留
#define GBT31024_Sub_Frame_Reversed3    0b01111                         // 保留
#define GBT31024_Sub_Frame_ACK          0b10000                         // 确认帧
#define GBT31024_Sub_Frame_GroupACKReq  0b10001                         // 组确认请求帧
#define GBT31024_Sub_Frame_Group_ACK    0b10010                         // 组确认帧
#define GBT31024_Sub_Frame_Quit         0b10011                         // 退出网络帧
// #define GBT31024_Sub_Frame_Reversed4    0b10100                         // 保留
#define GBT31024_Sub_Frame_SLP_REQ      0b10101                         // 休眠请求帧
#define GBT31024_Sub_Frame_SLP_RSP      0b10110                         // 休眠响应帧
#define GBT31024_Sub_Frame_DTF_IND      0b10111                         // 下行业务指示帧
// #define GBT31024_Sub_Frame_Reversed5    0b11000                         // 保留
#define GBT31024_Sub_Frame_CM_REQ       0b11001                         // 测量请求帧
#define GBT31024_Sub_Frame_CM_RSP       0b11010                         // 测量响应帧
#define GBT31024_Sub_Frame_CM_REP       0b11011                         // 测量报告帧，有序列号管理帧
#define GBT31024_Sub_Frame_HO_REQ       0b11100                         // 切换请求帧
#define GBT31024_Sub_Frame_HO_CMD       0b11101                         // 切换命令帧
// #define GBT31024_Sub_Frame_Reversed6    0b11110                         // 保留
// #define GBT31024_Sub_Frame_Reversed7    0b11111                         // 保留

// 数据帧子类型
#define GBT31024_Sub_Frame_DATA         0b00000                         // 数据帧
#define GBT31024_Sub_Frame_PAD_DATA     0b00001                         // 数据填充帧
// 00010 ~ 11111 全部为保留


// MPDU分为三部分： 通用MAC头，6字节， 帧体，校验（FCS）信息，4字节


typedef struct MPDU_MAC_HEAD
{
    // 帧控制部分，2字节，16bit
    int     nProtocolVer    : 2;                                        // 协议版本，2bit，值为0
    int     nFrameType      : 1;                                        // 帧类型，1bit
    int     nSubFrameType   : 5;                                        // 子类型，5bit

    // 流标识（FID），4bit,0000用于管理控制流，其他用于数据流
    int     nFID            : 4;

    // 重传标识，1bit，若当前帧为前面帧的重传帧则设置为1，否则为0
    int     nResendInd      : 1;
    int     nReversed1      : 3;                                        // 保留，3bit，默认为0


    // 以下参数为有序列号管理控制帧参数

    // 分片号，用于指示MSDU/MMPDU的每个分片的编号，取值范围为0~15。
    // 有多个分片时，第一个分片号为0，同一MSDU/MMPDU的不同分片的分片号以1递增
    int     nSegmentNo      : 4;
    
    // 序号，取值0~4095，用以指示MSDU/MMPDU的序列编号。一个FID流内所有发送的MSDU/MMPDU均被分片一个序号。
    // 第一个为0，同一FID内不同MSDU/MMPDU的序号以1递增
    int     nIndex          : 12;

    int     nReversed2      : 3;                                        // 预留

    // 在所有的数据帧或有序列号管理控制帧中，若后面还有当前MSDU/MMPDU的分片，该字段被设置为1，否则为0
    int     nSegmentInd     : 1;                                        // 分片指示

    // 长度，表示MAC头字段和FCS字段之间的所有字段的总字节长度，取值0~4095
    int     nLength         : 12;
}MPDU_MAC;



// FCS 字段，32bit，由MAC帧头和帧体的全部字段计算得到
// 校验方式
// g(x) = x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x+1
// x 值待明确
#define CalcFCS(x) (pow(x,32)+pow(x,26)+pow(x,22)+pow(x,16)+pow(x,12)+pow(x,11)+pow(x,10)+pow(x,8)+pow(x,7)+pow(x,5)+pow(x,4)+pow(x,2)+x+1)

// 数据帧帧格式
// 头部，6字节
struct MPDU_Data_Frame_HEAD
{
    // 帧控制部分，2字节，16bit
    int     nProtocolVer    : 2;                                        // 协议版本，2bit，值为0
    int     nFrameType      : 1;                                        // 帧类型，1bit
    int     nSubFrameType   : 5;                                        // 子类型，5bit

    // 流标识（FID），4bit,0000用于管理控制流，其他用于数据流
    int     nFID            : 4;

    // 重传标识，1bit，若当前帧为前面帧的重传帧则设置为1，否则为0
    int     nResendInd      : 1;
    int     nReversed1      : 3;                                        // 保留，3bit，默认为0

    // 分片号，用于指示MSDU/MMPDU的每个分片的编号，取值范围为0~15。
    // 有多个分片时，第一个分片号为0，同一MSDU/MMPDU的不同分片的分片号以1递增
    int     nSegmentNo      : 4;
    
    // 序号，取值0~4095，用以指示MSDU/MMPDU的序列编号。一个FID流内所有发送的MSDU/MMPDU均被分片一个序号。
    // 第一个为0，同一FID内不同MSDU/MMPDU的序号以1递增
    int     nIndex          : 12;

    int     nReversed2      : 1;                                        // 预留，默认为0

    // 随路请求指示，长度1bit，值为1表示帧体最前面会增加一个随路资源请求字段，0表示无此字段
    int     nFollowRoadReqInd : 1;

    // 立即/延迟确认，长度1bit，值为1表示发送端通知接收端，收到该帧后需要对所有位未确认的数据帧立即确认；
    // 值为0 表示允许延迟确认
    int     nConfirmType    : 1;

    // 在所有的数据帧或有序列号管理控制帧中，若后面还有当前MSDU/MMPDU的分片，该字段被设置为1，否则为0
    int     nSegmentInd     : 1;                                        // 分片指示

    // 长度，表示MAC头字段和FCS字段之间的所有字段的总字节长度，取值0~4095
    int     nLength         : 12;
};

#define Network_Alias_Len               31
#define Time_Stamp_Len                  8

// 管理控制帧定义
// 帧头部分不包含在定义中

// 广播控制帧
typedef struct MPDU_MCF_BCF
{
    UCHAR   RSUMacAddr[6];                                              // RSU MAC地址，48bit，6字节
    int     nChannelNo;
    int     nFrameLen       : 2;
    int     nAntennaCount   : 3;
    int     nReversed1      : 2;
    int     nMCSInd         : 1;                                        // 0：支持MCS0~MCS9，1：支持全部MCS
    int     nAliasLen;                                                  // 网络别名长度，1字节，取值范围：1~31，单位：字节
    int     strAlias[Network_Alias_Len+1];                              // 网络别名，以字母或数字开头的字符串，最大长度31字节，增加1字节是为了包含字符串的结束符
    int     timeStamp[Time_Stamp_Len];                                  // 时间戳，
    int     nBCFInterval    : 16;                                       // BCF间隔，16bit
    int     nRandomAccMinWindow     : 4;                                // 随机接入退避窗口的控制，最小窗口的取值范围是 0 ~ 2^n -1
    int     nScheduleReqMinWIndow   : 4;                                // 用于基于竞争的资源请求的退避窗口的控制
    int     nRandomAccMaxWindow;                                        // 最大窗口，8bit
    int     nScheduleReqMaxWIndow;                                      // 最大窗口，8bit
    int     nRSUTransmitPower       : 8;                                // RSU 发射功率，-128 ~ 127, 负数部分以补码形式表示
    int     nReversed2              : 5;
    int     nDownDetectPilot        : 3;                                // 下行探测导频图样索引
    int     nDownDetectChannelPos   : 8;                                // 下行探测信道在DL-TCH信道中的位置，0~255
    int     nPilotTimeInterval0     : 7;                                // 导频时域间隔
    int     nPilotTimeInterval1     : 7;
    int     nDGI : 2;                                                   // 下行与上行的转换时间，0：保护间隔为2个OFDM符合周期， 1： 4个周期，2~3：保留
    int     nUGI : 2;                                                   // 上行与下行的转换时间
    int     nULRachChannelType      : 2;                                // 随机接入格式1、2、3，11：保留
    int     nRSSIDLDrop             : 8;                                // 表示当前小区信号强度衰落门限值，用来触发测量请求
    int     nReversed3              : 2;

}MCF_BCF;

// 随机接入请求帧

typedef struct MPDU_MCF_RA_REQ
{
    UCHAR   OBUMacAddr[6];
    UCHAR   RSUMacAddr[6];
    int     nOBUPowerAdjustAllowance : 8;                               // OBU 发射功率调整余量，-128~127
    int     nOBUPower : 8;                                              // OBU 发射功率
    int     nReversed : 32;
}MCF_RA_REQ;

#endif // _GBT_31024_2_SPEC_H_
