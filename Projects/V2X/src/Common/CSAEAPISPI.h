/*------------------------------------------------------------------------------
 * RSU - Component :: T/CSAE 53-2017 Specification
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    CSAEAPISPI.h
 * Purpose: RSU module, T/CSAE 53-2017 Specification definition
 *----------------------------------------------------------------------------*/
#ifndef _CSAE_API_SPI_H_
#define _CSAE_API_SPI_H_

#include "CString.h"

#include "ASN1PERCoding.h"


////////////////////////////////////////////////////////////////////////
// 应用层数据交互标准及接口规范

// API 定义
enum TCSAE_API_TYPE
{
    // 本机信息
    AppGetHostInfoRequest = 0,                                          // 本机数据请求
    AppGetHostInfoConfirm,                                              // 本机数据请求确认
    
    AppGetHostStatusRequest,                                            // 本机状态请求
    AppGetHostStatusConfirm,                                            // 本机状态请求确认

    AppSetCommCfgRequest,                                               // 本机通信配置请求
    AppSetCommCfgConfirm,                                               // 本机通信配置请求确认
  
    AppGetCommStatusRequest,                                            // 本机通信状态请求
    AppGetCommStatusConfirm,                                            // 本机通信状态请求确认

    // 通讯操作
    AppDSMInitRequest,                                                  // DSM 消息初始化请求
    AppDSMInitConfirm,                                                  // DSM 消息初始化请求确认

    AppDSMTerminateRequest,                                             // DSM 消息终止服务请求
    AppDSMTerminateConfirm,                                             // DSM 消息终止服务请求确认

    AppSendDSMMsgRequest,                                               // DSM 消息发送请求
    AppSendDSMMsgConfirm,                                               // DSM 消息发送请求确认

    // 数据镜像服务
    AppDupBSMRequest,                                                   // 镜像BSM消息请求
    AppDupBSMConfirm,                                                   // 镜像BSM消息请求确认

    AppDupMapRequest,                                                   // 镜像MAP消息请求
    AppDupMapConfirm,                                                   // 镜像Map消息请求应答
    AppDupMapIndication,                                                // 镜像MAP消息通知

    AppDupSPATRequest,                                                  // 镜像SPAT消息请求
    AppDupSPATConfirm,                                                  // 镜像SPAT消息请求应答
    AppDupSPATIndication,                                               // 镜像SPAT消息通知

    AppDupRSMRequest,                                                   // 镜像RSM消息请求
    AppDupRSMConfirm,                                                   // 镜像RSM消息请求应答
    AppDupRSMIndication,                                                // 镜像RSM消息通知

    // 数据应用服务
    AppGetTCRequest,                                                    // TC 车辆信息请求
    AppGetTCConfirm,                                                    // TC 车辆信息请求应答
    AppGetTCIndication,                                                 // TC 车辆信息请求通知

    AppGetRemoteVehiclesRequest,                                        // 远程车辆信息请求
    AppGetRemoteVehiclesConfirm,                                        // 远程车辆信息请求应答
    AppGetRemoteVehiclesIndication,                                     // 远程车辆信息请求通知

    AppGetEventVehiclesRequest,                                         // 事件车辆信息请求
    AppGetEventVehiclesConfirm,                                         // 事件车辆信息请求应答
    AppGetEventVehiclesIndication,                                      // 事件车辆信息请求通知

    AppRoadSideAlertRequest,                                            // 路侧警示信息请求
    AppRoadSideAlertConfirm,                                            // 路侧警示信息请求应答
    AppRoadSideAlertIndication,                                         // 路侧警示信息请求通知

    AppSignalRequest,                                                   // 信号灯信息请求
    AppSignalConfirm,                                                   // 信号灯信息请求应答
    AppSignalIndication,                                                // 信号灯信息请求通知

    AppPedestrianRequest,                                               // 行人信息请求
    AppPedestrianConfirm,                                               // 行人信息请求应答
    AppPedestrianIndication,                                            // 行人信息请求通知

    // 管理服务
    AppGetServicesRequest,                                              // 服务列表获取请求
    AppGetServicesConfirm,                                              // 服务列表请求应答

    AppProviderServicesRequest,                                         // Provider 服务操作请求
    AppProviderServicesConfirm,                                         // Provider 服务操作请求应答

    AppUserServicesRequest,                                             // User 服务操作请求
    AppUserServicesConfirm,                                             // User 服务操作请求应答

    AppPaymentRequest,                                                  // 支付操作请求
    AppPaymentConfirm,                                                  // 支付操作请求应答
};

// 服务提供者接口 SPI
enum TCSAE_SPI_TYPE
{
    CommClientInitRequest = 0,                                          // 发送初始化操作请求
    CommClientInitConfirm,                                              // 发送初始化操作请求确认

    CommDSMSendRequest,                                                 // DSM 消息发送请求
    CommDSMSendConfirm,                                                 // DSM 消息发送请求确认
    CommDSMIndication,                                                  // DSM 消息接收通知

    CommDMESetCfgRequest,                                               // 设置 DME 请求
    CommDMESetCfgConfirm,                                               // 设置 DME 请求曲儿
    
    CommDMEGetCfgRequest,                                               // 获取 DME 属性请求
    CommDMEGetCfgConfirm,                                               // 获取 DME 属性请求确认

    CommDMEProviderServicesRequest,                                     // DME Provider 服务操作请求
    CommDMEProviderServicesConfirm,                                     // DME Provider 服务操作请求应答

    CommDMEUserServicesRequest,                                         // DME User 服务操作请求
    CommDMEUserServicesConfirm,                                         // DME User 服务操作请求应答

    CommDMENotifactionIndication,                                       // DME 接收通知消息
};

#endif // _CSAE_API_SPI_H_
