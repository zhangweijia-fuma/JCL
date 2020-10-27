/*------------------------------------------------------------------------------
 * Common - Component :: T/CSAE 53-2017 Specification
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    CSAEMsgBase.cpp
 * Purpose: Common module, T/CSAE 53-2017 Specification data element definition
 *----------------------------------------------------------------------------*/

#include "Debug.h"
#include "Common.h"

#include "CSAEMsgBase.h"

CCSAEMsgBase::CCSAEMsgBase()
{
    m_nOptionCount    = 0;
    for(int i = 0; i < MAX_OPTION_COUNT; i++)
        m_bOptionFlag[i] = false;

    m_strPeerModuleAddr = "";
    m_nPeerModulePort   = INVALID_SOCKET_PORT;

    m_bIsReceived       = false;
}

CCSAEMsgBase::~CCSAEMsgBase()
{

}

void CCSAEMsgBase::SetPeerAddr(CString strAddr, UINT nPort)
{
    m_strPeerModuleAddr = strAddr;
    m_nPeerModulePort   = nPort;
}

void CCSAEMsgBase::GetPeerAddr(CString &strAddr, UINT &nPort)
{
    strAddr = m_strPeerModuleAddr;
    nPort   = m_nPeerModulePort;
}

bool CCSAEMsgBase::EncodeDE_Acceleration(DE_Acceleration value)
{
    return EncodeInteger(value, DE_Acceleration_MIN, DE_Acceleration_MAX);
}

bool CCSAEMsgBase::EncodeDE_AlertType(UINT value)
{
    return EncodeUINT(value, DE_AlertType_MIN, DE_AlertType_MAX);
}

// 允许的转向行为，12位的比特串，含一种或多种转向行为
// 在作为变量表示时，使用 short类型，2个字节
// 在进行编解码时，进行强制类型转换为UCHAR
bool CCSAEMsgBase::EncodeDE_AllowedManeuvers(USHORT value)
{
    UINTType temp;

    temp.nIntValue = 0;
    temp.nShortValue[0] = value;
    return EncodeBitString(temp.nByteValue, DE_AllowedManeuvers_LEN);
}

bool CCSAEMsgBase::EncodeDE_AntiLockBrakeStatus(DE_AntiLockBrakeStatus value)
{
    return EncodeEnumerated(value, DE_AntiLockBrakeStatus_COUNT);
}

bool CCSAEMsgBase::EncodeDE_AuxiliaryBrakeStatus(DE_AuxiliaryBrakeStatus value)
{
    return EncodeEnumerated(value, DE_AuxiliaryBrakeStatus_COUNT);
}

bool CCSAEMsgBase::EncodeDE_BasicVehcleClass(DE_BasicVehcleClass value)
{
    return EncodeInteger(value, DE_BasicVehcleClass_MIN, DE_BasicVehcleClass_MAX);
}

// 四轮的刹车状态，定长5位比特串，使用UCHAR表示变量
bool CCSAEMsgBase::EncodeDE_BrakeAppliedStatus(UCHAR value)
{
    return EncodeBitString(&value, DE_BrakeAppliedStatus_LEN);
}

bool CCSAEMsgBase::EncodeDE_BrakeBoostApplied(DE_BrakeBoostApplied value)
{
    return EncodeEnumerated(value, DE_BrakeBoostApplied_COUNT);
}

bool CCSAEMsgBase::EncodeDE_BrakePedalStatus(DE_BrakePedalStatus value)
{
    return EncodeEnumerated(value, DE_BrakePedalStatus_COUNT);
}

bool CCSAEMsgBase::EncodeDE_CoarseHeading(DE_CoarseHeading value)
{
    return EncodeUCHAR(value, DE_CoarseHeading_MIN, DE_CoarseHeading_MAX);
}

bool CCSAEMsgBase::EncodeDE_Confidence(DE_Confidence value)
{
    return EncodeUCHAR(value, DE_Confidence_MIN, DE_Confidence_MAX);
}

bool CCSAEMsgBase::EncodeDE_DDay(DE_DDay value)
{
    return EncodeUCHAR(value, DE_DDay_MIN, DE_DDay_MAX);
}

bool CCSAEMsgBase::EncodeDE_DescriptiveName(DE_DescriptiveName value)
{
    return EncodeCharacterString(value, CST_PRINTABLE, DE_DescriptiveName_MIN, DE_DescriptiveName_MAX);
}

bool CCSAEMsgBase::EncodeDE_DHour(DE_DHour value)
{
    return EncodeUCHAR(value, DE_DHour_MIN, DE_DHour_MAX);
}

bool CCSAEMsgBase::EncodeDE_DMinute(DE_DMinute value)
{
    return EncodeUCHAR(value, DE_DMinute_MIN, DE_DMinute_MAX);
}

bool CCSAEMsgBase::EncodeDE_DMonth(DE_DMonth value)
{
    return EncodeUCHAR(value, DE_DMonth_MIN, DE_DMonth_MAX);
}

bool CCSAEMsgBase::EncodeDE_DSecond(DE_DSecond value)
{
    return EncodeUShort(value, DE_DSecond_MIN, DE_DSecond_MAX);
}

bool CCSAEMsgBase::EncodeDE_DTimeOffset(DE_DTimeOffset value)
{
    return EncodeInteger(value, DE_DTimeOffset_MIN, DE_DTimeOffset_MAX);
}

bool CCSAEMsgBase::EncodeDE_DYear(DE_DYear value)
{
    return EncodeUShort(value, DE_DYear_MIN, DE_DYear_MAX);
}

bool CCSAEMsgBase::EncodeDE_Elevation(DE_Elevation value)
{
    return EncodeInteger(value, DE_Elevation_MIN, DE_Elevation_MAX);
}

bool CCSAEMsgBase::EncodeDE_ElevationConfidence(DE_ElevationConfidence value)
{
    return EncodeEnumerated(value, DE_ElevationConfidence_COUNT);
}

// 车身周围的车灯状态，不定长比特串，最小9位
// 使用UINT定义变量
bool CCSAEMsgBase::EncodeDE_ExteriorLights(UINT value, USHORT nBitLen)
{
    UINTType temp;
    temp.nIntValue = value;
    return EncodeBitString(temp.nByteValue, nBitLen, DE_ExteriorLights_MIN_LEN, DE_ExteriorLights_MAX_LEN);
}

// 8位比特串，使用UCHAR定义变量
bool CCSAEMsgBase::EncodeDE_GNSSstatus(UCHAR value)
{
    return EncodeBitString(&value, DE_GNSSstatus_LEN);
}

bool CCSAEMsgBase::EncodeDE_Heading(DE_Heading value)
{
    return EncodeUShort(value, DE_Heading_MIN, DE_Heading_MAX);
}

bool CCSAEMsgBase::EncodeDE_HeadingConfidence(DE_HeadingConfidence value)
{
    return EncodeEnumerated(value, DE_HeadingConfidence_COUNT);
}

// 路口信号机的工作状态，
// 定长比特串，16位，使用USHORT定义变量
bool CCSAEMsgBase::EncodeDE_IntersectionStatusObject(USHORT value)
{
    UINTType temp;
    temp.nIntValue = 0;
    temp.nShortValue[0] = value;
    return EncodeBitString(temp.nByteValue, DE_IntersectionStatusObject_LEN);
}

// 定长比特串，16位，使用USHORT定义变量
bool CCSAEMsgBase::EncodeDE_LaneAttr_Barrier(UINT value)
{
    UINTType temp;
    temp.nIntValue = value;
    return EncodeBitString(temp.nByteValue, DE_IntersectionStatusObject_LEN);
}

// 定长比特串，16位，使用USHORT定义变量
bool CCSAEMsgBase::EncodeDE_LaneAttr_Bike(UINT value)
{
    UINTType temp;
    temp.nIntValue = value;
    return EncodeBitString(temp.nByteValue, DE_LaneAttr_Bike_LEN);
}

// 定长比特串，16位，使用USHORT定义变量
bool CCSAEMsgBase::EncodeDE_LaneAttr_Crosswalk(UINT value)
{
    UINTType temp;
    temp.nIntValue = value;
    return EncodeBitString(temp.nByteValue, DE_LaneAttr_Crosswalk_LEN);
}

// 定长比特串，16位，使用USHORT定义变量
bool CCSAEMsgBase::EncodeDE_LaneAttr_Parking(UINT value)
{
    UINTType temp;
    temp.nIntValue = value;
    return EncodeBitString(temp.nByteValue, DE_LaneAttr_Parking_LEN);
}

// 定长比特串，16位，使用USHORT定义变量
bool CCSAEMsgBase::EncodeDE_LaneAttr_Sidewalk(UINT value)
{
    UINTType temp;
    temp.nIntValue = value;
    return EncodeBitString(temp.nByteValue, DE_LaneAttr_Sidewalk_LEN);
}

// 定长比特串，16位，使用USHORT定义变量
bool CCSAEMsgBase::EncodeDE_LaneAttr_Striping(UINT value)
{
    UINTType temp;
    temp.nIntValue = value;
    return EncodeBitString(temp.nByteValue, DE_LaneAttr_Striping_LEN);
}

// 定长比特串，16位，使用USHORT定义变量
bool CCSAEMsgBase::EncodeDE_LaneAttr_TrackedVehicle(UINT value)
{
    UINTType temp;
    temp.nIntValue = value;
    return EncodeBitString(temp.nByteValue, DE_LaneAttr_TrackedVehicle_LEN);
}

// 不定长比特串，16位，使用UINT定义变量
bool CCSAEMsgBase::EncodeDE_LaneAttr_Vehicle(UINT value, USHORT nBitLen)
{
    UINTType temp;
    temp.nIntValue = value;
    return EncodeBitString(temp.nByteValue, nBitLen, DE_LaneAttr_Vehicle_MIN, DE_LaneAttr_Vehicle_MAX);
}

bool CCSAEMsgBase::EncodeDE_LaneID(DE_LaneID value)
{
    return EncodeUCHAR(value, DE_LaneID_MIN, DE_LaneID_MAX);
}

// 定长比特串，10位，使用USHORT定义变量
bool CCSAEMsgBase::EncodeDE_LaneSharing(USHORT value)
{
    UINTType temp;
    temp.nIntValue = 0;
    temp.nShortValue[0] = value;
    return EncodeBitString(temp.nByteValue, DE_LaneSharing_LEN);
}

bool CCSAEMsgBase::EncodeDE_LaneWidth(DE_LaneWidth value)
{
    return EncodeUShort(value, DE_LaneWidth_MIN, DE_LaneWidth_MAX);
}

bool CCSAEMsgBase::EncodeDE_Latitude(DE_Latitude value)
{
    return EncodeInteger(value, DE_Latitude_MIN, DE_Latitude_MAX);
}

bool CCSAEMsgBase::EncodeDE_LightState(DE_LightState value)
{
    return EncodeEnumerated(value, DE_LightState_COUNT);
}

bool CCSAEMsgBase::EncodeDE_Longtitude(DE_Longtitude value)
{
    return EncodeLong(value, DE_Longtitude_MIN, DE_Longtitude_MAX);
}

bool CCSAEMsgBase::EncodeDE_MinuteOfTheYear(DE_MinuteOfTheYear value)
{
    return EncodeUINT(value, DE_MinuteOfTheYear_MIN, DE_MinuteOfTheYear_MAX);
}

bool CCSAEMsgBase::EncodeDE_MsgCount(DE_MsgCount value)
{
    return EncodeUCHAR(value, DE_MsgCount_MIN, DE_MsgCount_MAX);
}

bool CCSAEMsgBase::EncodeDE_NodeID(DE_NodeID value)
{
    return EncodeUINT(value, DE_NodeID_MIN, DE_NodeID_MAX);
}

bool CCSAEMsgBase::EncodeDE_OffsetLL_B12(DE_OffsetLL_B12 value)
{
    return EncodeInteger(value, DE_OffsetLL_B12_MIN, DE_OffsetLL_B12_MAX);
}

bool CCSAEMsgBase::EncodeDE_OffsetLL_B14(DE_OffsetLL_B14 value)
{
    return EncodeInteger(value, DE_OffsetLL_B14_MIN, DE_OffsetLL_B14_MAX);
}

bool CCSAEMsgBase::EncodeDE_OffsetLL_B16(DE_OffsetLL_B16 value)
{
    return EncodeInteger(value, DE_OffsetLL_B16_MIN, DE_OffsetLL_B16_MAX);
}

bool CCSAEMsgBase::EncodeDE_OffsetLL_B18(DE_OffsetLL_B18 value)
{
    return EncodeInteger(value, DE_OffsetLL_B18_MIN, DE_OffsetLL_B18_MAX);
}

bool CCSAEMsgBase::EncodeDE_OffsetLL_B22(DE_OffsetLL_B22 value)
{
    return EncodeInteger(value, DE_OffsetLL_B22_MIN, DE_OffsetLL_B22_MAX);
}

bool CCSAEMsgBase::EncodeDE_OffsetLL_B24(DE_OffsetLL_B24 value)
{
    return EncodeInteger(value, DE_OffsetLL_B24_MIN, DE_OffsetLL_B24_MAX);
}

bool CCSAEMsgBase::EncodeDE_PartcipantType(DE_PartcipantType value)
{
    return EncodeEnumerated(value, DE_PartcipantType_COUNT);
}

bool CCSAEMsgBase::EncodeDE_PhaseID(DE_PhaseID value)
{
    return EncodeUShort(value, DE_PhaseID_MIN, DE_PhaseID_MAX);
}

bool CCSAEMsgBase::EncodeDE_PositionConfidence(DE_PositionConfidence value)
{
    return EncodeEnumerated(value, DE_PositionConfidence_COUNT);
}

// 表示消息优先级。数值长度占8 位，其中低五位为0，为无效位，高三位为有效数据位。
// 数值有效范围是B00000000 到B11100000，分别表示8 档由低到高的优先级。
// 8位串，共1个字节，使用UCHAR定义变量
bool CCSAEMsgBase::EncodeDE_Priority(UCHAR value)
{
    return EncodeOctString(&value, DE_Priority_LEN);
}

bool CCSAEMsgBase::EncodeDE_Radius(DE_Radius value)
{
    return EncodeInteger(value, DE_Radius_MIN, DE_Radius_MAX);
}

bool CCSAEMsgBase::EncodeDE_RadiusOfCurvation(DE_RadiusOfCurvation value)
{
    return EncodeInteger(value, DE_RadiusOfCurvation_MIN, DE_RadiusOfCurvation_MAX);
}

bool CCSAEMsgBase::EncodeDE_RoadRegulatorID(DE_RoadRegulatorID value)
{
    return EncodeUINT(value, DE_RoadRegulatorID_MIN, DE_RoadRegulatorID_MAX);
}

bool CCSAEMsgBase::EncodeDE_SourceType(DE_SourceType value)
{
    return EncodeEnumerated(value, DE_SourceType_COUNT);
}

bool CCSAEMsgBase::EncodeDE_Speed(DE_Speed value)
{
    return EncodeUShort(value, DE_Speed_MIN, DE_Speed_MAX);
}

bool CCSAEMsgBase::EncodeDE_SpeedConfidence(DE_SpeedConfidence value)
{
    return EncodeEnumerated(value, DE_SpeedConfidence_COUNT);
}

bool CCSAEMsgBase::EncodeDE_SpeedLimitType(DE_SpeedLimitType value)
{
    return EncodeEnumerated(value, DE_SpeedLimitType_COUNT);
}

bool CCSAEMsgBase::EncodeDE_StabilityControlStatus(DE_StabilityControlStatus value)
{
    return EncodeEnumerated(value, DE_StabilityControlStatus_COUNT);
}

bool CCSAEMsgBase::EncodeDE_SteeringWheelAngle(DE_SteeringWheelAngle value)
{
    return EncodeInteger(value, DE_SteeringWheelAngle_MIN, DE_SteeringWheelAngle_MAX);
}

bool CCSAEMsgBase::EncodeDE_SteeringWheelAngleConfidence(DE_SteeringWheelAngleConfidence value)
{
    return EncodeEnumerated(value, DE_SteeringWheelAngleConfidence_COUNT);
}

bool CCSAEMsgBase::EncodeDE_TimeConfidence(DE_TimeConfidence value)
{
    return EncodeEnumerated(value, DE_TimeConfidence_COUNT);
}

bool CCSAEMsgBase::EncodeDE_TimeMark(DE_TimeMark value)
{
    return EncodeUShort(value, DE_TimeMark_MIN, DE_TimeMark_MAX);
}

bool CCSAEMsgBase::EncodeDE_TimeOffset(DE_TimeOffset value)
{
    return EncodeUShort(value, DE_TimeOffset_MIN, DE_TimeOffset_MAX);
}

bool CCSAEMsgBase::EncodeDE_TractionControlStatus(DE_TractionControlStatus value)
{
    return EncodeEnumerated(value, DE_TractionControlStatus_COUNT);
}

bool CCSAEMsgBase::EncodeDE_TransmissionState(DE_TransmissionState value)
{
    return EncodeEnumerated(value, DE_TransmissionState_COUNT);
}

// 不定长比特串，使用UINT定义变量
bool CCSAEMsgBase::EncodeDE_VehicleEventFlags(UINT value, USHORT nBitLen)
{
    UINTType temp;
    temp.nIntValue = value;
    return EncodeBitString(temp.nByteValue, nBitLen, DE_VehicleEventFlags_MIN_LEN, DE_VehicleEventFlags_MAX_LEN);
}

bool CCSAEMsgBase::EncodeDE_VehicleHeight(DE_VehicleHeight value)
{
    return EncodeUCHAR(value, DE_VehicleHeight_MIN, DE_VehicleHeight_MAX);
}

bool CCSAEMsgBase::EncodeDE_VehicleLength(DE_VehicleLength value)
{
    return EncodeUShort(value, DE_VehicleLength_MIN, DE_VehicleLength_MAX);
}

bool CCSAEMsgBase::EncodeDE_VehicleWidth(DE_VehicleWidth value)
{
    return EncodeUShort(value, DE_VehicleWidth_MIN, DE_VehicleWidth_MAX);
}

bool CCSAEMsgBase::EncodeDE_VerticalAcceleration(DE_VerticalAcceleration value)
{
    return EncodeInteger(value, DE_VerticalAcceleration_MIN, DE_VerticalAcceleration_MAX);
}

bool CCSAEMsgBase::EncodeDE_VertOffset_B07(DE_VertOffset_B07 value)
{
    return EncodeInteger(value, DE_VertOffset_B07_MIN, DE_VertOffset_B07_MAX);
}

bool CCSAEMsgBase::EncodeDE_VertOffset_B08(DE_VertOffset_B08 value)
{
    return EncodeInteger(value, DE_VertOffset_B08_MIN, DE_VertOffset_B08_MAX);
}

bool CCSAEMsgBase::EncodeDE_VertOffset_B09(DE_VertOffset_B09 value)
{
    return EncodeInteger(value, DE_VertOffset_B09_MIN, DE_VertOffset_B09_MAX);
}

bool CCSAEMsgBase::EncodeDE_VertOffset_B10(DE_VertOffset_B10 value)
{
    return EncodeInteger(value, DE_VertOffset_B10_MIN, DE_VertOffset_B10_MAX);
}

bool CCSAEMsgBase::EncodeDE_VertOffset_B11(DE_VertOffset_B11 value)
{
    return EncodeInteger(value, DE_VertOffset_B11_MIN, DE_VertOffset_B11_MAX);
}

bool CCSAEMsgBase::EncodeDE_VertOffset_B12(DE_VertOffset_B12 value)
{
    return EncodeInteger(value, DE_VertOffset_B12_MIN, DE_VertOffset_B12_MAX);
}

bool CCSAEMsgBase::EncodeDE_YawRate(DE_YawRate value)
{
    return EncodeInteger(value, DE_YawRate_MIN, DE_YawRate_MAX);
}

// /////////////////////////////////////////////////////////////////////
// Decode 函数

bool CCSAEMsgBase::DecodeDE_Acceleration(DE_Acceleration& value)
{
    return DecodeInteger(value, DE_Acceleration_MIN, DE_Acceleration_MAX);
}

bool CCSAEMsgBase::DecodeDE_AlertType(UINT& value)
{
    return DecodeUINT(value, DE_AlertType_MIN, DE_AlertType_MAX);
}

bool CCSAEMsgBase::DecodeDE_AllowedManeuvers(USHORT& value)
{
    UINTType temp;
    USHORT nBitLen = DE_AllowedManeuvers_LEN;
    temp.nIntValue = 0;
    if ( DecodeBitString(temp.nByteValue, 8,  nBitLen ) == false )
    {
        LogE("decode DE_AllowedManeuvers failed.");
        return false;
    }

    value = temp.nShortValue[0];
    return true;
}

bool CCSAEMsgBase::DecodeDE_AntiLockBrakeStatus(DE_AntiLockBrakeStatus& value)
{
    UINT nTemp = 0;
    if ( DecodeEnumerated(nTemp, DE_AntiLockBrakeStatus_COUNT) == false)
    {
        LogE("decode DE_AntiLockBrakeStatus failed.");
        return false;
    }

    value = (DE_AntiLockBrakeStatus)nTemp;
    return true;
}

bool CCSAEMsgBase::DecodeDE_AuxiliaryBrakeStatus(DE_AuxiliaryBrakeStatus& value)
{
    UINT nTemp = 0;
    if ( DecodeEnumerated(nTemp, DE_AuxiliaryBrakeStatus_COUNT) == false)
    {
        LogE("decode DE_AuxiliaryBrakeStatus failed.");
        return false;
    }

    value = (DE_AuxiliaryBrakeStatus)nTemp;
    return true;
}

bool CCSAEMsgBase::DecodeDE_BasicVehcleClass(DE_BasicVehcleClass& value)
{
    return DecodeInteger((int&)value, DE_BasicVehcleClass_MIN, DE_BasicVehcleClass_MAX);
}

bool CCSAEMsgBase::DecodeDE_BrakeAppliedStatus(UCHAR& value)
{
    USHORT nBitLen = DE_BrakeAppliedStatus_LEN;
    return DecodeBitString(&value, 1, nBitLen);
}

bool CCSAEMsgBase::DecodeDE_BrakeBoostApplied(DE_BrakeBoostApplied& value)
{
    UINT nTemp = 0;
    if ( DecodeEnumerated(nTemp, DE_BrakeBoostApplied_COUNT) == false)
    {
        LogE("decode DE_BrakeBoostApplied failed.");
        return false;
    }

    value = (DE_BrakeBoostApplied)nTemp;
    return true;
}

bool CCSAEMsgBase::DecodeDE_BrakePedalStatus(DE_BrakePedalStatus& value)
{
    UINT nTemp = 0;
    if ( DecodeEnumerated(nTemp, DE_BrakePedalStatus_COUNT) == false)
    {
        LogE("decode DE_BrakePedalStatus failed.");
        return false;
    }

    value = (DE_BrakePedalStatus)nTemp;
    return true;
}

bool CCSAEMsgBase::DecodeDE_CoarseHeading(DE_CoarseHeading& value)
{
    return DecodeUCHAR(value, DE_CoarseHeading_MIN, DE_CoarseHeading_MAX);
}

bool CCSAEMsgBase::DecodeDE_Confidence(DE_Confidence& value)
{
    return DecodeUCHAR(value, DE_Confidence_MIN, DE_Confidence_MAX);
}

bool CCSAEMsgBase::DecodeDE_DDay(DE_DDay& value)
{
    return DecodeUCHAR(value, DE_DDay_MIN, DE_DDay_MAX);
}

bool CCSAEMsgBase::DecodeDE_DescriptiveName(DE_DescriptiveName& value)
{
    return DecodeCharacterString(value, CST_PRINTABLE, DE_DescriptiveName_MIN, DE_DescriptiveName_MAX);
}

bool CCSAEMsgBase::DecodeDE_DHour(DE_DHour& value)
{
    return DecodeUCHAR(value, DE_DHour_MIN, DE_DHour_MAX);
}

bool CCSAEMsgBase::DecodeDE_DMinute(DE_DMinute& value)
{
    return DecodeUCHAR(value, DE_DMinute_MIN, DE_DMinute_MAX);
}

bool CCSAEMsgBase::DecodeDE_DMonth(DE_DMonth& value)
{
    return DecodeUCHAR(value, DE_DMonth_MIN, DE_DMonth_MAX);
}

bool CCSAEMsgBase::DecodeDE_DSecond(DE_DSecond& value)
{
    return DecodeUShort(value, DE_DSecond_MIN, DE_DSecond_MAX);
}

bool CCSAEMsgBase::DecodeDE_DTimeOffset(DE_DTimeOffset& value)
{
    return DecodeInteger(value, DE_DTimeOffset_MIN, DE_DTimeOffset_MAX);
}

bool CCSAEMsgBase::DecodeDE_DYear(DE_DYear& value)
{
    return DecodeUShort(value, DE_DYear_MIN, DE_DYear_MAX);
}

bool CCSAEMsgBase::DecodeDE_Elevation(DE_Elevation& value)
{
    return DecodeInteger(value, DE_Elevation_MIN, DE_Elevation_MAX);
}

bool CCSAEMsgBase::DecodeDE_ElevationConfidence(DE_ElevationConfidence& value)
{
    UINT nTemp = 0;
    if ( DecodeEnumerated(nTemp, DE_ElevationConfidence_COUNT) == false)
    {
        LogE("decode DE_ElevationConfidence failed.");
        return false;
    }

    value = (DE_ElevationConfidence)nTemp;
    return true;
}

bool CCSAEMsgBase::DecodeDE_ExteriorLights(UINT& value, USHORT &nBitLen)
{
    UINTType temp;
    temp.nIntValue = 0;
    if ( DecodeBitString(temp.nByteValue, 4, nBitLen, DE_ExteriorLights_MIN_LEN, DE_ExteriorLights_MAX_LEN) == false)
    {
        LogE("Decode DE_ExteriorLights failed.");
        return false;
    }

    value = temp.nIntValue;
    return true;
}

bool CCSAEMsgBase::DecodeDE_GNSSstatus(UCHAR& value)
{
    USHORT nBitLen = DE_GNSSstatus_LEN;
    return DecodeBitString(&value, 1, nBitLen);
}

bool CCSAEMsgBase::DecodeDE_Heading(DE_Heading& value)
{
    return DecodeUShort(value, DE_Heading_MIN, DE_Heading_MAX);
}

bool CCSAEMsgBase::DecodeDE_HeadingConfidence(DE_HeadingConfidence& value)
{
    UINT nTemp = 0;
    if ( DecodeEnumerated(nTemp, DE_HeadingConfidence_COUNT) == false)
    {
        LogE("decode DE_HeadingConfidence failed.");
        return false;
    }

    value = (DE_HeadingConfidence)nTemp;
    return true;
}

bool CCSAEMsgBase::DecodeDE_IntersectionStatusObject(USHORT& value)
{
    UINTType temp;
    temp.nIntValue = 0;
    USHORT nBitLen = DE_IntersectionStatusObject_LEN;
    if ( DecodeBitString(temp.nByteValue, 2, nBitLen) == false)
    {
        LogE("Decode DE_ExteriorLights failed.");
        return false;
    }

    value = temp.nShortValue[0];
    return true;
}

bool CCSAEMsgBase::DecodeDE_LaneAttr_Barrier(UINT& value)
{
    UINTType temp;
    temp.nIntValue = 0;
    USHORT nBitLen = DE_LaneAttr_Barrier_LEN;
    if ( DecodeBitString(temp.nByteValue, 2, nBitLen) == false)
    {
        LogE("Decode DE_LaneAttr_Barrier failed.");
        return false;
    }

    value = temp.nIntValue;
    return true;
}

bool CCSAEMsgBase::DecodeDE_LaneAttr_Bike(UINT& value)
{
    UINTType temp;
    temp.nIntValue = 0;
    USHORT nBitLen = DE_LaneAttr_Bike_LEN;
    if ( DecodeBitString(temp.nByteValue, 2, nBitLen) == false)
    {
        LogE("Decode DE_LaneAttr_Bike failed.");
        return false;
    }

    value = temp.nIntValue;
    return true;
}

bool CCSAEMsgBase::DecodeDE_LaneAttr_Crosswalk(UINT& value)
{
    UINTType temp;
    temp.nIntValue = 0;
    USHORT nBitLen = DE_LaneAttr_Crosswalk_LEN;
    if ( DecodeBitString(temp.nByteValue, 2, nBitLen) == false)
    {
        LogE("Decode DE_LaneAttr_Crosswalk failed.");
        return false;
    }

    value = temp.nIntValue;
    return true;
}

bool CCSAEMsgBase::DecodeDE_LaneAttr_Parking(UINT& value)
{
    UINTType temp;
    temp.nIntValue = 0;
    USHORT nBitLen = DE_LaneAttr_Parking_LEN;
    if ( DecodeBitString(temp.nByteValue, 2, nBitLen) == false)
    {
        LogE("Decode DE_LaneAttr_Parking failed.");
        return false;
    }

    value = temp.nIntValue;
    return true;
}

bool CCSAEMsgBase::DecodeDE_LaneAttr_Sidewalk(UINT& value)
{
    UINTType temp;
    temp.nIntValue = 0;
    USHORT nBitLen = DE_LaneAttr_Sidewalk_LEN;
    if ( DecodeBitString(temp.nByteValue, 2, nBitLen) == false)
    {
        LogE("Decode DE_LaneAttr_Sidewalk failed.");
        return false;
    }

    value = temp.nIntValue;
    return true;
}

bool CCSAEMsgBase::DecodeDE_LaneAttr_Striping(UINT& value)
{
    UINTType temp;
    temp.nIntValue = 0;
    USHORT nBitLen = DE_LaneAttr_Striping_LEN;
    if ( DecodeBitString(temp.nByteValue, 2, nBitLen) == false)
    {
        LogE("Decode DE_LaneAttr_Striping failed.");
        return false;
    }

    value = temp.nIntValue;
    return true;
}

bool CCSAEMsgBase::DecodeDE_LaneAttr_TrackedVehicle(UINT& value)
{
    UINTType temp;
    temp.nIntValue = 0;
    USHORT nBitLen = DE_LaneAttr_TrackedVehicle_LEN;
    if ( DecodeBitString(temp.nByteValue, 2, nBitLen) == false)
    {
        LogE("Decode DE_LaneAttr_TrackedVehicle failed.");
        return false;
    }

    value = temp.nIntValue;
    return true;
}

bool CCSAEMsgBase::DecodeDE_LaneAttr_Vehicle(UINT& value)
{
    UINTType temp;
    temp.nIntValue = 0;
    USHORT nBitLen = 0;
    if ( DecodeBitString(temp.nByteValue, 8, nBitLen, DE_LaneAttr_Vehicle_MIN, DE_LaneAttr_Vehicle_MAX) == false)
    {
        LogE("Decode DE_LaneAttr_Vehicle failed.");
        return false;
    }

    value = temp.nIntValue;
    return true;
}

bool CCSAEMsgBase::DecodeDE_LaneID(DE_LaneID& value)
{
    return DecodeUCHAR(value, DE_LaneID_MIN, DE_LaneID_MAX);
}

bool CCSAEMsgBase::DecodeDE_LaneSharing(USHORT& value)
{
    UINTType temp;
    temp.nIntValue = 0;
    USHORT nBitLen = DE_LaneSharing_LEN;
    if ( DecodeBitString(temp.nByteValue, 2, nBitLen) == false)
    {
        LogE("Decode DE_LaneSharing failed.");
        return false;
    }

    value = temp.nShortValue[0];
    return true;
}

bool CCSAEMsgBase::DecodeDE_LaneWidth(DE_LaneWidth& value)
{
    return DecodeUShort(value, DE_LaneWidth_MIN, DE_LaneWidth_MAX);
}

bool CCSAEMsgBase::DecodeDE_Latitude(DE_Latitude& value)
{
    return DecodeInteger(value, DE_Latitude_MIN, DE_Latitude_MAX);
}

bool CCSAEMsgBase::DecodeDE_LightState(DE_LightState& value)
{
    UINT nTemp = 0;
    if ( DecodeEnumerated(nTemp, DE_LightState_COUNT) == false)
    {
        LogE("decode DE_HeadingConfidence failed.");
        return false;
    }

    value = (DE_LightState)nTemp;
    return true;
}

bool CCSAEMsgBase::DecodeDE_Longtitude(DE_Longtitude& value)
{
    return DecodeLong(value, DE_Longtitude_MIN, DE_Longtitude_MAX);
}

bool CCSAEMsgBase::DecodeDE_MinuteOfTheYear(DE_MinuteOfTheYear& value)
{
    return DecodeUINT(value, DE_MinuteOfTheYear_MIN, DE_MinuteOfTheYear_MAX);
}

bool CCSAEMsgBase::DecodeDE_MsgCount(DE_MsgCount& value)
{
    return DecodeUCHAR(value, DE_MsgCount_MIN, DE_MsgCount_MAX);
}

bool CCSAEMsgBase::DecodeDE_NodeID(DE_NodeID& value)
{
    return DecodeUINT(value, DE_NodeID_MIN, DE_NodeID_MAX);
}

bool CCSAEMsgBase::DecodeDE_OffsetLL_B12(DE_OffsetLL_B12& value)
{
    return DecodeInteger(value, DE_OffsetLL_B12_MIN, DE_OffsetLL_B12_MAX);
}

bool CCSAEMsgBase::DecodeDE_OffsetLL_B14(DE_OffsetLL_B14& value)
{
    return DecodeInteger(value, DE_OffsetLL_B14_MIN, DE_OffsetLL_B14_MAX);
}

bool CCSAEMsgBase::DecodeDE_OffsetLL_B16(DE_OffsetLL_B16& value)
{
    return DecodeInteger(value, DE_OffsetLL_B16_MIN, DE_OffsetLL_B16_MAX);
}

bool CCSAEMsgBase::DecodeDE_OffsetLL_B18(DE_OffsetLL_B18& value)
{
    return DecodeInteger(value, DE_OffsetLL_B18_MIN, DE_OffsetLL_B18_MAX);
}

bool CCSAEMsgBase::DecodeDE_OffsetLL_B22(DE_OffsetLL_B22& value)
{
    return DecodeInteger(value, DE_OffsetLL_B22_MIN, DE_OffsetLL_B22_MAX);
}

bool CCSAEMsgBase::DecodeDE_OffsetLL_B24(DE_OffsetLL_B24& value)
{
    return DecodeInteger(value, DE_OffsetLL_B24_MIN, DE_OffsetLL_B24_MAX);
}

bool CCSAEMsgBase::DecodeDE_PartcipantType(DE_PartcipantType& value)
{
    UINT nTemp = 0;
    if ( DecodeEnumerated(nTemp, DE_PartcipantType_COUNT) == false)
    {
        LogE("decode DE_PartcipantType failed.");
        return false;
    }

    value = (DE_PartcipantType)nTemp;
    return true;
}

bool CCSAEMsgBase::DecodeDE_PhaseID(DE_PhaseID& value)
{
    return DecodeUShort(value, DE_PhaseID_MIN, DE_PhaseID_MAX);
}

bool CCSAEMsgBase::DecodeDE_PositionConfidence(DE_PositionConfidence& value)
{
    UINT nTemp = 0;
    if ( DecodeEnumerated(nTemp, DE_PositionConfidence_COUNT) == false)
    {
        LogE("decode DE_PositionConfidence failed.");
        return false;
    }

    value = (DE_PositionConfidence)nTemp;
    return true;
}

bool CCSAEMsgBase::DecodeDE_Priority(UCHAR& value)
{
    USHORT nByteLen = 1;
    return DecodeOctString(&value, 1, nByteLen);
}

bool CCSAEMsgBase::DecodeDE_Radius(DE_Radius& value)
{
    return DecodeUShort(value, DE_Radius_MIN, DE_Radius_MAX);
}


bool CCSAEMsgBase::DecodeDE_RadiusOfCurvation(DE_RadiusOfCurvation& value)
{
    return DecodeInteger(value, DE_RadiusOfCurvation_MIN, DE_RadiusOfCurvation_MAX);
}

bool CCSAEMsgBase::DecodeDE_RoadRegulatorID(DE_RoadRegulatorID& value)
{
    return DecodeUINT(value, DE_RoadRegulatorID_MIN, DE_RoadRegulatorID_MAX);
}

bool CCSAEMsgBase::DecodeDE_SourceType(DE_SourceType& value)
{
    UINT nTemp = 0;
    if ( DecodeEnumerated(nTemp, DE_SourceType_COUNT) == false)
    {
        LogE("decode DE_SourceType failed.");
        return false;
    }

    value = (DE_SourceType)nTemp;
    return true;
}

bool CCSAEMsgBase::DecodeDE_Speed(DE_Speed& value)
{
    return DecodeUShort(value, DE_Speed_MIN, DE_Speed_MAX);
}

bool CCSAEMsgBase::DecodeDE_SpeedConfidence(DE_SpeedConfidence& value)
{
    UINT nTemp = 0;
    if ( DecodeEnumerated(nTemp, DE_SpeedConfidence_COUNT) == false)
    {
        LogE("decode DE_SpeedConfidence failed.");
        return false;
    }

    value = (DE_SpeedConfidence)nTemp;
    return true;
}

bool CCSAEMsgBase::DecodeDE_SpeedLimitType(DE_SpeedLimitType& value)
{
    UINT nTemp = 0;
    if ( DecodeEnumerated(nTemp, DE_SpeedLimitType_COUNT) == false)
    {
        LogE("decode DE_SpeedLimitType failed.");
        return false;
    }

    value = (DE_SpeedLimitType)nTemp;
    return true;
}

bool CCSAEMsgBase::DecodeDE_StabilityControlStatus(DE_StabilityControlStatus& value)
{
    UINT nTemp = 0;
    if ( DecodeEnumerated(nTemp, DE_StabilityControlStatus_COUNT) == false)
    {
        LogE("decode DE_StabilityControlStatus failed.");
        return false;
    }

    value = (DE_StabilityControlStatus)nTemp;
    return true;
}

bool CCSAEMsgBase::DecodeDE_SteeringWheelAngle(DE_SteeringWheelAngle& value)
{
    return DecodeInteger(value, DE_SteeringWheelAngle_MIN, DE_SteeringWheelAngle_MAX);
}

bool CCSAEMsgBase::DecodeDE_SteeringWheelAngleConfidence(DE_SteeringWheelAngleConfidence& value)
{
    UINT nTemp = 0;
    if ( DecodeEnumerated(nTemp, DE_SteeringWheelAngleConfidence_COUNT) == false)
    {
        LogE("decode DE_SpeedLimitType failed.");
        return false;
    }

    value = (DE_SteeringWheelAngleConfidence)nTemp;
    return true;
}

bool CCSAEMsgBase::DecodeDE_TimeConfidence(DE_TimeConfidence& value)
{
    UINT nTemp = 0;
    if ( DecodeEnumerated(nTemp, DE_TimeConfidence_COUNT) == false)
    {
        LogE("decode DE_TimeConfidence failed.");
        return false;
    }

    value = (DE_TimeConfidence)nTemp;
    return true;
}

bool CCSAEMsgBase::DecodeDE_TimeMark(DE_TimeMark& value)
{
    return DecodeUShort(value, DE_TimeMark_MIN, DE_TimeMark_MAX);
}

bool CCSAEMsgBase::DecodeDE_TimeOffset(DE_TimeOffset& value)
{
    return DecodeUShort(value, DE_TimeOffset_MIN, DE_TimeOffset_MAX);
}

bool CCSAEMsgBase::DecodeDE_TractionControlStatus(DE_TractionControlStatus& value)
{
    UINT nTemp = 0;
    if ( DecodeEnumerated(nTemp, DE_TractionControlStatus_COUNT) == false)
    {
        LogE("decode DE_TractionControlStatus failed.");
        return false;
    }

    value = (DE_TractionControlStatus)nTemp;
    return true;
}

bool CCSAEMsgBase::DecodeDE_TransmissionState(DE_TransmissionState& value)
{
    UINT nTemp = 0;
    if ( DecodeEnumerated(nTemp, DE_TransmissionState_COUNT) == false)
    {
        LogE("decode DE_TransmissionState failed.");
        return false;
    }

    value = (DE_TransmissionState)nTemp;
    return true;
}

bool CCSAEMsgBase::DecodeDE_VehicleEventFlags(UINT& value, USHORT& nBitLen)
{
    UINTType temp;
    temp.nIntValue = 0;
    if ( DecodeBitString(temp.nByteValue, 2, nBitLen, DE_VehicleEventFlags_MIN_LEN, DE_VehicleEventFlags_MAX_LEN ) == false)
    {
        LogE("Decode DE_LaneSharing failed.");
        return false;
    }

    value = temp.nIntValue;
    return true;
}

bool CCSAEMsgBase::DecodeDE_VehicleHeight(DE_VehicleHeight& value)
{
    return DecodeUCHAR(value, DE_VehicleHeight_MIN, DE_VehicleHeight_MAX);
}

bool CCSAEMsgBase::DecodeDE_VehicleLength(DE_VehicleLength& value)
{
    return DecodeUShort(value, DE_VehicleLength_MIN, DE_VehicleLength_MAX);
}

bool CCSAEMsgBase::DecodeDE_VehicleWidth(DE_VehicleWidth& value)
{
    return DecodeUShort(value, DE_VehicleWidth_MIN, DE_VehicleWidth_MAX);
}

bool CCSAEMsgBase::DecodeDE_VerticalAcceleration(DE_VerticalAcceleration& value)
{
    return DecodeInteger(value, DE_VerticalAcceleration_MIN, DE_VerticalAcceleration_MAX);
}

bool CCSAEMsgBase::DecodeDE_VertOffset_B07(DE_VertOffset_B07& value)
{
    return DecodeInteger(value, DE_VertOffset_B07_MIN, DE_VertOffset_B07_MAX);
}

bool CCSAEMsgBase::DecodeDE_VertOffset_B08(DE_VertOffset_B08& value)
{
    return DecodeInteger(value, DE_VertOffset_B08_MIN, DE_VertOffset_B08_MAX);
}

bool CCSAEMsgBase::DecodeDE_VertOffset_B09(DE_VertOffset_B09& value)
{
    return DecodeInteger(value, DE_VertOffset_B09_MIN, DE_VertOffset_B09_MAX);
}

bool CCSAEMsgBase::DecodeDE_VertOffset_B10(DE_VertOffset_B10& value)
{
    return DecodeInteger(value, DE_VertOffset_B10_MIN, DE_VertOffset_B10_MAX);
}

bool CCSAEMsgBase::DecodeDE_VertOffset_B11(DE_VertOffset_B11& value)
{
    return DecodeInteger(value, DE_VertOffset_B11_MIN, DE_VertOffset_B11_MAX);
}

bool CCSAEMsgBase::DecodeDE_VertOffset_B12(DE_VertOffset_B12& value)
{
    return DecodeInteger(value, DE_VertOffset_B12_MIN, DE_VertOffset_B12_MAX);
}

bool CCSAEMsgBase::DecodeDE_YawRate(DE_YawRate& value)
{
    return DecodeInteger(value, DE_YawRate_MIN, DE_YawRate_MAX);
}


// /////////////////////////////////////////////////////////////////////
// 

// 设置可选项标志
bool CCSAEMsgBase::GetOptionFlag(USHORT nItemIndex)
{
    if ( nItemIndex < 0 || nItemIndex >= MAX_OPTION_COUNT )
    {
        LogE("GetOptionFlag index %d is out of range [0, %d]", nItemIndex, MAX_OPTION_COUNT);
        return false;
    }

    return m_bOptionFlag[nItemIndex];
}

void CCSAEMsgBase::SetOptionFlag(USHORT nItemIndex, bool bFlag)
{
    if ( nItemIndex < 0 || nItemIndex >= MAX_OPTION_COUNT )
    {
        LogE("GetOptionFlag index %d is out of range [0, %d]", nItemIndex, MAX_OPTION_COUNT);
        return;
    }

    m_bOptionFlag[nItemIndex] = bFlag;
}

bool CCSAEMsgBase::EncodeOptionFlag()
{
    if ( m_nOptionCount < 0 || m_nOptionCount >= MAX_OPTION_COUNT )
    {
        LogE("EncodeOptionFlag Optioa count %d is out of range [0, %d]", m_nOptionCount, MAX_OPTION_COUNT);
        return false;
    }

    UINTType temp;
    temp.nIntValue = 0;

    for( USHORT i = 0; i < m_nOptionCount; i++)
        SetBit(temp.nIntValue, i, m_bOptionFlag[i]);

    // 在有多个可选参数的时候，前导位图为定长比特串
    if ( EncodeBitString(temp.nByteValue, m_nOptionCount) == false )
    {
        LogE("CCSAEMsgBase encode option map failed.");
        return false;
    }

    return true;
}

bool CCSAEMsgBase::DecodeOptionFlag()
{
    if ( m_nOptionCount < 0 || m_nOptionCount >= MAX_OPTION_COUNT )
    {
        LogE("DecodeOptionFlag Optioa count %d is out of range [0, %d]", m_nOptionCount, MAX_OPTION_COUNT);
        return false;
    }

    if ( m_nOptionCount == 0 )
        return true;

    // 在有多个可选参数的时候，前导位图为定长比特串
    UINTType temp;
    temp.nIntValue = 0;
    if ( DecodeBitString(temp.nByteValue, sizeof(temp.nByteValue), m_nOptionCount) == false )
    {
        LogE("CCSAEMsgBase decode option map failed.");
        return false;
    }

    for(USHORT i = 0; i < m_nOptionCount; i++)
        m_bOptionFlag[i] = GetBit(temp.nIntValue, i);

    return true;
}

bool CCSAEMsgBase::EncodeDataFrame(CCSAEMsgBase* pFrame)
{
	if ( pFrame == NULL )
	{
		LogE("CCSAEMsgBase EncodeDataFrame parameters NULL pointer");
		return false;
	}

    if ( pFrame->Encode() == CODEC_FAILED)
    {
        LogE("EncodeDataFrame encode failed.");
        return false;
    }

    // 将编码后的数据作为定长比特串附加到数据后
    PUCHAR pData;
    USHORT nBitLen;
    pData = pFrame->GetBuffer(nBitLen);

    if ( EncodeBitString(pData, nBitLen) == false )
    {
        LogE("EncodeDataFrame encode data failed.");
        return false;
    }

    return true;
}

bool CCSAEMsgBase::DecodeDataFrame(CCSAEMsgBase* pFrame)
{
    if ( pFrame == NULL )
    {
        LogE("CCSAEMsgBase DecodeDataFrame parameters NULL pointer");
        return false;
    }

    m_nBitPos = pFrame->Decode( m_pBuf, m_nBitLen, m_nBitPos );
    if ( m_nBitPos == CODEC_FAILED )
    {
        LogE("DecodeDataFrame decode failed.");
        return false;
    }

    return true;
}


CCSAEMsgBase* CCSAEMsgBase::CreateObject(CString strClassName)
{
    return static_cast<CCSAEMsgBase*>(CObjectFactory::createObject(strClassName.GetBuffer()));
}

// 在进行对象赋值时使用
bool CCSAEMsgBase::SetParameter(CCSAEMsgBase* pMsgData)
{
    if ( pMsgData == NULL )
    {
        LogE("CCSAEMsgBase SetParameter CCSAEMsgBase point is NULL");
        return false;
    }

    m_nOptionCount = pMsgData->m_nOptionCount;                          // 可选项个数
    for ( int i = 0; i < m_nOptionCount && i < MAX_OPTION_COUNT; i++)
        m_bOptionFlag[i] = pMsgData->m_bOptionFlag[i];                  // 可选项标志 

    return true;
}
