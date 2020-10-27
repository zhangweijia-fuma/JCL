/*------------------------------------------------------------------------------
 * RSU - Component :: T/CSAE 53-2017 Specification
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    CSAEMessageSet.h
 * Purpose: RSU module, T/CSAE 53-2017 Specification definition
 *----------------------------------------------------------------------------*/
#ifndef _CSAE_MESSAGE_SET_H_
#define _CSAE_MESSAGE_SET_H_

#include "CString.h"


#include "CSAEDataFrame.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

// 车辆基本安全消息
// V2V消息，车辆广播该消息，将自身的实时状态告知周围车辆
class CBasicSafetyMessage : public CCSAEMsgBase
{
    public:
        CBasicSafetyMessage();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(DE_MsgCount, CString, DE_DSecond, CDF_Position3D*, CDF_PositionConfidenceSet*, 
                                DE_TransmissionState transState, DE_Speed, DE_Heading, CDF_AccelerationSet4Way*, 
                                CDF_BrakeSystemStatus*, CDF_VehicleSize*, CDF_VehicleClassification*, CString = "", 
                                DE_SteeringWheelAngle = DE_SteeringWheelAngle_MAX + 1, CDF_MotionConfidenceSet* = NULL, 
                                CDF_VehicleSafetyExtensions* = NULL);
        bool                GetParameter(DE_MsgCount&, CString&, DE_DSecond&, CDF_Position3D*, CDF_PositionConfidenceSet*, 
                                DE_TransmissionState&, DE_Speed&, DE_Heading&, CDF_AccelerationSet4Way*, CDF_BrakeSystemStatus*, 
                                CDF_VehicleSize*, CDF_VehicleClassification*, CString&, DE_SteeringWheelAngle&, 
                                CDF_MotionConfidenceSet*, CDF_VehicleSafetyExtensions*);
        bool                SetParameter(CBasicSafetyMessage*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        DE_MsgCount                     m_MsgCount;
        CString                         m_strVehicleID;                 // 汽车ID
        CString                         m_strPlateNo;                   // 电动汽车ID，可选参数
        DE_DSecond                      m_secMark;
        CDF_Position3D                  m_Position3D;
        CDF_PositionConfidenceSet       m_Accuracy;
        DE_TransmissionState            m_TransissionState;
        DE_Speed                        m_Speed;
        DE_Heading                      m_Heading;
        DE_SteeringWheelAngle           m_SteeringWheelAngle;           // 可选
        CDF_MotionConfidenceSet         m_MotionConfidenceSet;          // 可选
        CDF_AccelerationSet4Way         m_AccelerationSet4Way;
        CDF_BrakeSystemStatus           m_BrakeSystemStatus;
        CDF_VehicleSize                 m_VehicleSize;
        CDF_VehicleClassification       m_VehicleClass;
        CDF_VehicleSafetyExtensions     m_VehicleSafetyExtensions;      // 可选 
};

// 地图消息
// 由RSU广播，向车辆传递局部区域的地图信息，包括路口信息、路段信息、车道信息，道路之间的连接关系等
// 单个地图消息可以包含多个路口或区域的地图数据
// 路口的信号灯信息在SPAT消息中定义
class CMapData : public CCSAEMsgBase
{
    public:
        CMapData();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);
        
        bool                SetParameter(DE_MsgCount, CDF_NodeList*, DE_MinuteOfTheYear = DE_MinuteOfTheYear_MAX + 1);
        bool                GetParameter(DE_MsgCount&, CDF_NodeList*, DE_MinuteOfTheYear = DE_MinuteOfTheYear_MAX + 1);
        bool                SetParameter(CMapData*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        DE_MsgCount             m_MsgCount;
        DE_MinuteOfTheYear      m_TimeStamp;                            // 可选参数
        CDF_NodeList            m_NodesList;       
};

// 由路侧单元向周围车载单元发布的交通事件消息及交通标志牌信息
class CRoadSideInformation : public CCSAEMsgBase
{
    public:
        CRoadSideInformation();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(DE_MsgCount msgCnt, CString strRSUID, int nRSIID, UINT alertType, 
                                CDF_Position3D* pRefPos, CDF_PathPointList* pPathPointList, DE_Radius alertRadius, 
                                DE_MinuteOfTheYear timeStamp = 0, CString strDesc = "", DE_Priority priority = DE_Priority_MIN);
        bool                GetParameter(DE_MsgCount&, CString &strRSUID, int &nRSIID, UINT &alertType, CDF_Position3D* pRefPos, 
                                CDF_PathPointList* pPathPointList, DE_Radius &alertRadius, DE_MinuteOfTheYear &timeStamp, 
                                CString &strDesc, DE_Priority &priority);
        bool                SetParameter(CRoadSideInformation*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:

        DE_MsgCount             m_MsgCount;
        DE_MinuteOfTheYear      m_TimeStamp;                            // 可选
        CString                 m_strRSUID;                             // 8位字符串，未定义格式
        int                     m_nRSIID;                               // 0-255, RSU 设置的这RSI信息的ID
        UINT                    m_AlertType;                            // 文本信息，根据国标GB5768定义的交通标志警告 
        CString                 m_IA5StringDesc;                        // AlertType =0, 文本信息； > 0，附加的交通标志描述
        DE_Priority             m_Priority;                             // 可选
        CDF_Position3D          m_Position3D;                           // 交通标志的位置
        CDF_PathPointList       m_PathPointList;
        DE_Radius               m_AlertRadius;
};

// 路侧安全消息，由路侧单元广播给周边车辆
class CRoadSideSafetyMessage : public CCSAEMsgBase
{
    public:
        CRoadSideSafetyMessage();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetRSUID(CString strID);
        CDF_ParticipantList* GetParticipantList() { return &m_ParticipantList; };

        bool                SetParameter(DE_MsgCount msgCnt, CString strRSUID, CDF_Position3D* pRefPos, CDF_ParticipantList* pPtcList);
        bool                GetParameter(DE_MsgCount&, CString &strRSUID, CDF_Position3D* pRefPos, CDF_ParticipantList* pPtcList);
        bool                SetParameter(CRoadSideSafetyMessage*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        DE_MsgCount         m_MsgCount;
        CString             m_strRSUID;
        CDF_Position3D      m_Position3D;
        CDF_ParticipantList m_ParticipantList;
};

// 信号灯消息。包含一个或多个路口信号灯的当前状态信息
class CSPAT : public CCSAEMsgBase
{
    public:
        CSPAT();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(DE_MsgCount msgCnt, CDF_IntersectionStateList* pStateList, 
                                DE_MinuteOfTheYear = DE_MinuteOfTheYear_MAX + 1, DE_DescriptiveName strName = "");
        bool                GetParameter(DE_MsgCount&, CDF_IntersectionStateList* pStateList, 
                                DE_MinuteOfTheYear&, DE_DescriptiveName &strDesc);
        bool                SetParameter(CSPAT*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        DE_MsgCount                 m_MsgCount;
        DE_MinuteOfTheYear          m_TimeStamp;
        DE_DescriptiveName          m_DescriptiveName; // 可选
        CDF_IntersectionStateList   m_InterSectionStateList;
};

// 消息帧是应用层数据包收发的基本单位。由不同类别的消息体组成。
class CMainMsgFrame : public CCSAEMsgBase
{
    public:
        CMainMsgFrame();
        ~CMainMsgFrame();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        CCSAEMsgBase*       GetObject(int &nChoiceIndex);
        bool                SetObject(CCSAEMsgBase* pMsg);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        int                     m_nChoiceIndex;
        CCSAEMsgBase*           m_pMsg;
};


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
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

#endif // _CSAE_MESSAGE_SET_H_
