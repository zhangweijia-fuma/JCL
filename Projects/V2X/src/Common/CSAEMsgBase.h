/*------------------------------------------------------------------------------
 * Common - Component :: T/CSAE 53-2017 Specification
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    CSAEMsgBase.h
 * Purpose: T/CSAE 53-2017 Specification data element definition
 *----------------------------------------------------------------------------*/
#ifndef _CSAE_MSG_BASE_H_
#define _CSAE_MSG_BASE_H_

#include "CString.h"

// 对象的创建和删除
// 谁申请，谁删除。
// 在List或其他使用对象指针的地方，不是由自身创建的对象，都不需要删除该对象。
// 在使用的时候，需要注意该对象是否还存在

#include "ASN1PERCoding.h"
#include "ObjectFactory.h"

#include "CSAEDataElement.h"

// 对数字类型的数据检查其是否符合范围大小
#define CHECK_INT_VALID(type_name, value)  ( value >= type_name##_MIN && value <= type_name##_MAX)

// 对枚举类型的数据检查其是否符合范围大小
#define CHECK_ENUM_VALID(type_name, value)  ( value >= 0 && value <= type_name##_Invalid )

/////////////////////////////////////////////////////////////////////////////////////////////////
// 定义一个对CSAE规范的基类，在此基类中，针对所有的数据元素完成编解码操作

class CCSAEMsgBase : public CASN1PerMsgBase
{
    public:
        CCSAEMsgBase();
        ~CCSAEMsgBase();
        
    protected:
        bool                EncodeDE_Acceleration(DE_Acceleration);
        bool                EncodeDE_AlertType(UINT);
        bool                EncodeDE_AllowedManeuvers(USHORT);
        bool                EncodeDE_AntiLockBrakeStatus(DE_AntiLockBrakeStatus);
        bool                EncodeDE_AuxiliaryBrakeStatus(DE_AuxiliaryBrakeStatus);
        bool                EncodeDE_BasicVehcleClass(DE_BasicVehcleClass);
        bool                EncodeDE_BrakeAppliedStatus(UCHAR);
        bool                EncodeDE_BrakeBoostApplied(DE_BrakeBoostApplied);
        bool                EncodeDE_BrakePedalStatus(DE_BrakePedalStatus);
        bool                EncodeDE_CoarseHeading(DE_CoarseHeading);
        bool                EncodeDE_Confidence(DE_Confidence);
        bool                EncodeDE_DDay(DE_DDay);
        bool                EncodeDE_DescriptiveName(DE_DescriptiveName);
        bool                EncodeDE_DHour(DE_DHour);
        bool                EncodeDE_DMinute(DE_DMinute);
        bool                EncodeDE_DMonth(DE_DMonth);
        bool                EncodeDE_DSecond(DE_DSecond);
        bool                EncodeDE_DTimeOffset(DE_DTimeOffset);
        bool                EncodeDE_DYear(DE_DYear);
        bool                EncodeDE_Elevation(DE_Elevation);
        bool                EncodeDE_ElevationConfidence(DE_ElevationConfidence);
        bool                EncodeDE_ExteriorLights(UINT, USHORT nBitLen);
        bool                EncodeDE_GNSSstatus(UCHAR);
        bool                EncodeDE_Heading(DE_Heading);
        bool                EncodeDE_HeadingConfidence(DE_HeadingConfidence);
        bool                EncodeDE_IntersectionStatusObject(USHORT);
        bool                EncodeDE_LaneAttr_Barrier(UINT);
        bool                EncodeDE_LaneAttr_Bike(UINT);
        bool                EncodeDE_LaneAttr_Crosswalk(UINT);
        bool                EncodeDE_LaneAttr_Parking(UINT);
        bool                EncodeDE_LaneAttr_Sidewalk(UINT);
        bool                EncodeDE_LaneAttr_Striping(UINT);
        bool                EncodeDE_LaneAttr_TrackedVehicle(UINT);
        bool                EncodeDE_LaneAttr_Vehicle(UINT, USHORT);
        bool                EncodeDE_LaneID(DE_LaneID);
        bool                EncodeDE_LaneSharing(USHORT);
        bool                EncodeDE_LaneWidth(DE_LaneWidth);
        bool                EncodeDE_Latitude(DE_Latitude);
        bool                EncodeDE_LightState(DE_LightState);
        bool                EncodeDE_Longtitude(DE_Longtitude);
        bool                EncodeDE_MinuteOfTheYear(DE_MinuteOfTheYear);
        bool                EncodeDE_MsgCount(DE_MsgCount);
        bool                EncodeDE_NodeID(DE_NodeID);
        bool                EncodeDE_OffsetLL_B12(DE_OffsetLL_B12);
        bool                EncodeDE_OffsetLL_B14(DE_OffsetLL_B14);
        bool                EncodeDE_OffsetLL_B16(DE_OffsetLL_B16);
        bool                EncodeDE_OffsetLL_B18(DE_OffsetLL_B18);
        bool                EncodeDE_OffsetLL_B22(DE_OffsetLL_B22);
        bool                EncodeDE_OffsetLL_B24(DE_OffsetLL_B24);
        bool                EncodeDE_PartcipantType(DE_PartcipantType);
        bool                EncodeDE_PhaseID(DE_PhaseID);
        bool                EncodeDE_PositionConfidence(DE_PositionConfidence);
        bool                EncodeDE_Priority(DE_Priority);
        bool                EncodeDE_Radius(DE_Radius);
        bool                EncodeDE_RadiusOfCurvation(DE_RadiusOfCurvation);
        bool                EncodeDE_RoadRegulatorID(DE_RoadRegulatorID);
        bool                EncodeDE_SourceType(DE_SourceType);
        bool                EncodeDE_Speed(DE_Speed);
        bool                EncodeDE_SpeedConfidence(DE_SpeedConfidence);
        bool                EncodeDE_SpeedLimitType(DE_SpeedLimitType);
        bool                EncodeDE_StabilityControlStatus(DE_StabilityControlStatus);
        bool                EncodeDE_SteeringWheelAngle(DE_SteeringWheelAngle);
        bool                EncodeDE_SteeringWheelAngleConfidence(DE_SteeringWheelAngleConfidence);
        bool                EncodeDE_TimeConfidence(DE_TimeConfidence);
        bool                EncodeDE_TimeMark(DE_TimeMark);
        bool                EncodeDE_TimeOffset(DE_TimeOffset);
        bool                EncodeDE_TractionControlStatus(DE_TractionControlStatus);
        bool                EncodeDE_TransmissionState(DE_TransmissionState);
        bool                EncodeDE_VehicleEventFlags(UINT, USHORT);
        bool                EncodeDE_VehicleHeight(DE_VehicleHeight);
        bool                EncodeDE_VehicleLength(DE_VehicleLength);
        bool                EncodeDE_VehicleWidth(DE_VehicleWidth);
        bool                EncodeDE_VerticalAcceleration(DE_VerticalAcceleration);
        bool                EncodeDE_VertOffset_B07(DE_VertOffset_B07);
        bool                EncodeDE_VertOffset_B08(DE_VertOffset_B08);
        bool                EncodeDE_VertOffset_B09(DE_VertOffset_B09);
        bool                EncodeDE_VertOffset_B10(DE_VertOffset_B10);
        bool                EncodeDE_VertOffset_B11(DE_VertOffset_B11);
        bool                EncodeDE_VertOffset_B12(DE_VertOffset_B12);
        bool                EncodeDE_YawRate(DE_YawRate);
        

        bool                DecodeDE_Acceleration(DE_Acceleration&);
        bool                DecodeDE_AlertType(UINT&);
        bool                DecodeDE_AllowedManeuvers(USHORT&);
        bool                DecodeDE_AntiLockBrakeStatus(DE_AntiLockBrakeStatus&);
        bool                DecodeDE_AuxiliaryBrakeStatus(DE_AuxiliaryBrakeStatus&);
        bool                DecodeDE_BasicVehcleClass(DE_BasicVehcleClass&);
        bool                DecodeDE_BrakeAppliedStatus(UCHAR&);
        bool                DecodeDE_BrakeBoostApplied(DE_BrakeBoostApplied&);
        bool                DecodeDE_BrakePedalStatus(DE_BrakePedalStatus&);
        bool                DecodeDE_CoarseHeading(DE_CoarseHeading&);
        bool                DecodeDE_Confidence(DE_Confidence&);
        bool                DecodeDE_DDay(DE_DDay&);
        bool                DecodeDE_DescriptiveName(DE_DescriptiveName&);
        bool                DecodeDE_DHour(DE_DHour&);
        bool                DecodeDE_DMinute(DE_DMinute&);
        bool                DecodeDE_DMonth(DE_DMonth&);
        bool                DecodeDE_DSecond(DE_DSecond&);
        bool                DecodeDE_DTimeOffset(DE_DTimeOffset&);
        bool                DecodeDE_DYear(DE_DYear&);
        bool                DecodeDE_Elevation(DE_Elevation&);
        bool                DecodeDE_ElevationConfidence(DE_ElevationConfidence&);
        bool                DecodeDE_ExteriorLights(UINT&, USHORT&);
        bool                DecodeDE_GNSSstatus(UCHAR&);
        bool                DecodeDE_Heading(DE_Heading&);
        bool                DecodeDE_HeadingConfidence(DE_HeadingConfidence&);
        bool                DecodeDE_IntersectionStatusObject(USHORT&);
        bool                DecodeDE_LaneAttr_Barrier(UINT&);
        bool                DecodeDE_LaneAttr_Bike(UINT&);
        bool                DecodeDE_LaneAttr_Crosswalk(UINT&);
        bool                DecodeDE_LaneAttr_Parking(UINT&);
        bool                DecodeDE_LaneAttr_Sidewalk(UINT&);
        bool                DecodeDE_LaneAttr_Striping(UINT&);
        bool                DecodeDE_LaneAttr_TrackedVehicle(UINT&);
        bool                DecodeDE_LaneAttr_Vehicle(UINT&);
        bool                DecodeDE_LaneID(DE_LaneID&);
        bool                DecodeDE_LaneSharing(USHORT&);
        bool                DecodeDE_LaneWidth(DE_LaneWidth&);
        bool                DecodeDE_Latitude(DE_Latitude&);
        bool                DecodeDE_LightState(DE_LightState&);
        bool                DecodeDE_Longtitude(DE_Longtitude&);
        bool                DecodeDE_MinuteOfTheYear(DE_MinuteOfTheYear&);
        bool                DecodeDE_MsgCount(DE_MsgCount&);
        bool                DecodeDE_NodeID(DE_NodeID&);
        bool                DecodeDE_OffsetLL_B12(DE_OffsetLL_B12&);
        bool                DecodeDE_OffsetLL_B14(DE_OffsetLL_B14&);
        bool                DecodeDE_OffsetLL_B16(DE_OffsetLL_B16&);
        bool                DecodeDE_OffsetLL_B18(DE_OffsetLL_B18&);
        bool                DecodeDE_OffsetLL_B22(DE_OffsetLL_B22&);
        bool                DecodeDE_OffsetLL_B24(DE_OffsetLL_B24&);
        bool                DecodeDE_PartcipantType(DE_PartcipantType&);
        bool                DecodeDE_PhaseID(DE_PhaseID&);
        bool                DecodeDE_PositionConfidence(DE_PositionConfidence&);
        bool                DecodeDE_Priority(UCHAR&);
        bool                DecodeDE_Radius(DE_Radius&);
        bool                DecodeDE_RadiusOfCurvation(DE_RadiusOfCurvation&);
        bool                DecodeDE_RoadRegulatorID(DE_RoadRegulatorID&);
        bool                DecodeDE_SourceType(DE_SourceType&);
        bool                DecodeDE_Speed(DE_Speed&);
        bool                DecodeDE_SpeedConfidence(DE_SpeedConfidence&);
        bool                DecodeDE_SpeedLimitType(DE_SpeedLimitType&);
        bool                DecodeDE_StabilityControlStatus(DE_StabilityControlStatus&);
        bool                DecodeDE_SteeringWheelAngle(DE_SteeringWheelAngle&);
        bool                DecodeDE_SteeringWheelAngleConfidence(DE_SteeringWheelAngleConfidence&);
        bool                DecodeDE_TimeConfidence(DE_TimeConfidence&);
        bool                DecodeDE_TimeMark(DE_TimeMark&);
        bool                DecodeDE_TimeOffset(DE_TimeOffset&);
        bool                DecodeDE_TractionControlStatus(DE_TractionControlStatus&);
        bool                DecodeDE_TransmissionState(DE_TransmissionState&);
        bool                DecodeDE_VehicleEventFlags(UINT&, USHORT& nBitLen);
        bool                DecodeDE_VehicleHeight(DE_VehicleHeight&);
        bool                DecodeDE_VehicleLength(DE_VehicleLength&);
        bool                DecodeDE_VehicleWidth(DE_VehicleWidth&);
        bool                DecodeDE_VerticalAcceleration(DE_VerticalAcceleration&);
        bool                DecodeDE_VertOffset_B07(DE_VertOffset_B07&);
        bool                DecodeDE_VertOffset_B08(DE_VertOffset_B08&);
        bool                DecodeDE_VertOffset_B09(DE_VertOffset_B09&);
        bool                DecodeDE_VertOffset_B10(DE_VertOffset_B10&);
        bool                DecodeDE_VertOffset_B11(DE_VertOffset_B11&);
        bool                DecodeDE_VertOffset_B12(DE_VertOffset_B12&);
        bool                DecodeDE_YawRate(DE_YawRate&);

        bool                EncodeDataFrame(CCSAEMsgBase* pFrame);
        bool                DecodeDataFrame(CCSAEMsgBase* pFrame);

        bool                GetOptionFlag(USHORT nItemIndex);
        void                SetOptionFlag(USHORT nItemIndex, bool bFlag);

        bool                EncodeOptionFlag();
        bool                DecodeOptionFlag();

        // 数据帧列表
        USHORT              EncodeList();
        USHORT              DecodeList(PUCHAR pData, USHORT nMsgLen, USHORT nStartBitPos = 0);

        CCSAEMsgBase*       CreateObject(CString strClassName);         // 根据类名创建类的实例
        bool                SetDataFrameList(CCSAEMsgBase* pList);

    public:
        USHORT              Encode() { return m_nBitPos; }
        USHORT              Decode(PUCHAR pData, USHORT nMsgLen, USHORT nStartBitPos = 0) { return m_nBitPos; }

        bool                SetParameter(CCSAEMsgBase* pMsgData);       // 对基类的参数赋值

        void                SetPeerAddr(CString strAddr, UINT nPort);
        void                GetPeerAddr(CString &strAddr, UINT &nPort);

        virtual void        toParamString() {};                         // 将参数值输出到字符串中

        virtual void        Test() {};                                  // 用于单个类的测试

    protected:

        USHORT              m_nOptionCount;                             // 可选项个数
        bool                m_bOptionFlag[MAX_OPTION_COUNT];            // 可选项标志 

        bool                m_bIsReceived;                              // 是接收的消息还是发送的消息
        CString             m_strPeerModuleAddr;                        // 对端地址及端口
        UINT                m_nPeerModulePort;
};

#endif // _CSAE_MSG_BASE_H_
