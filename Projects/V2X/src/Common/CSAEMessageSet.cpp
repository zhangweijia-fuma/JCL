/*------------------------------------------------------------------------------
 * RSU - Component :: T/CSAE 53-2017 Specification
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    CSAEMessageSet.cpp
 * Purpose: Common module, T/CSAE 53-2017 Specification message set definition
 *----------------------------------------------------------------------------*/

#include "Debug.h"
#include "Common.h"

#include "CSAEMessageSet.h"


REGISTER_CLASS(CBasicSafetyMessage)

CBasicSafetyMessage::CBasicSafetyMessage() 
{
    m_MsgCount              = 0;
    m_strVehicleID          = "";
    m_strPlateNo            = "";
    m_secMark               = 60000;
    m_TransissionState      = DE_TS_Neutral;
    m_Speed                 = 0;
    m_Heading               = 0;
    m_SteeringWheelAngle    = 0;

    m_nOptionCount          = 4;
}

bool CBasicSafetyMessage::SetParameter(DE_MsgCount msgCount, CString strVehicleID, DE_DSecond secMark, 
    CDF_Position3D* pPos, CDF_PositionConfidenceSet* pPosCfdSet, DE_TransmissionState transState, DE_Speed speed, DE_Heading heading, 
    CDF_AccelerationSet4Way* pAccSet4Way, CDF_BrakeSystemStatus* pBrakeStatus, CDF_VehicleSize* pVehicleSize, 
    CDF_VehicleClassification* pVehicleClass, CString strPlateNo, DE_SteeringWheelAngle angle, 
    CDF_MotionConfidenceSet* pMotionCfdSet, CDF_VehicleSafetyExtensions* pVehicleExt)
{
    if ( CHECK_INT_VALID(DE_MsgCount, msgCount) == false )
    {
        LogE("CMapData SetParameter DE_MsgCount %d is out of range [%d, %d]",
            msgCount, DE_MsgCount_MIN, DE_MsgCount_MAX);
        return false;
    }
    m_MsgCount = msgCount;

    if ( strVehicleID.GetLength() != VEHICLE_ID_LEN )
    {
        LogE("CRoadSideInformation Vehicle ID length %d is wrong, Vehicle ID length is: %d", 
            strVehicleID.GetLength(), VEHICLE_ID_LEN);
        return false;
    }
    m_strVehicleID = strVehicleID;

    if ( CHECK_INT_VALID(DE_DSecond, secMark) == false )
    {
        LogE("CMapData SetParameter DE_DSecond %d is out of range [%d, %d]",
            secMark, DE_DSecond_MIN, DE_DSecond_MAX);
        return false;
    }
    m_secMark = secMark;

    if ( pPos == NULL )
    {
        LogE("CBasicSafetyMessage SetParameter CDF_Position3D point is NULL");
        return false;
    }
    m_Position3D.SetParameter(pPos);

    if ( pPosCfdSet == NULL )
    {
        LogE("CBasicSafetyMessage SetParameter CDF_PositionConfidenceSet point is NULL");
        return false;
    }
    m_Accuracy.SetParameter(pPosCfdSet);

    if ( CHECK_ENUM_VALID(DE_TransmissionState, transState) == false )
    {
        LogE("CMapData SetParameter DE_TransmissionState %d is out of range [0, %d]",
            transState, DE_TransmissionState_COUNT);
        return false;
    }
    m_TransissionState = transState;

    if ( CHECK_INT_VALID(DE_Speed, speed) == false )
    {
        LogE("CMapData SetParameter DE_Speed %d is out of range [%d, %d]",
            speed, DE_Speed_MIN, DE_Speed_MAX);
        return false;
    }
    m_Speed = speed;


    if ( CHECK_INT_VALID(DE_Heading, heading) == false )
    {
        LogE("CMapData SetParameter DE_Heading %d is out of range [%d, %d]",
            heading, DE_Heading_MIN, DE_Heading_MAX);
        return false;
    }
    m_Heading = heading;

    if ( pAccSet4Way == NULL )
    {
        LogE("CBasicSafetyMessage SetParameter CDF_AccelerationSet4Way point is NULL");
        return false;
    }
    m_AccelerationSet4Way.SetParameter(pAccSet4Way);

    if ( pBrakeStatus == NULL )
    {
        LogE("CBasicSafetyMessage SetParameter CDF_BrakeSystemStatus point is NULL");
        return false;
    }
    m_BrakeSystemStatus.SetParameter(pBrakeStatus);

    if ( pVehicleSize == NULL )
    {
        LogE("CBasicSafetyMessage SetParameter CDF_VehicleSize point is NULL");
        return false;
    }
    m_VehicleSize.SetParameter(pVehicleSize);

    if ( pVehicleClass == NULL )
    {
        LogE("CBasicSafetyMessage SetParameter CDF_VehicleClassification point is NULL");
        return false;
    }
    m_VehicleClass.SetParameter(pVehicleClass);

    if ( strPlateNo != "" )
    {
        SetOptionFlag(0, true);
        m_strPlateNo = strPlateNo;
    }

    if ( CHECK_INT_VALID(DE_SteeringWheelAngle, angle) == true )
    {
        SetOptionFlag(1, true);
        m_SteeringWheelAngle = angle;
    }

    if ( pMotionCfdSet != NULL )
    {
        SetOptionFlag(2, true);
        m_MotionConfidenceSet.SetParameter(pMotionCfdSet);
    }

    if ( pVehicleExt != NULL )
    {
        SetOptionFlag(3, true);
        m_VehicleSafetyExtensions.SetParameter(pVehicleExt);
    }

    return true;
}

bool CBasicSafetyMessage::GetParameter(DE_MsgCount& msgCount, CString& strVehicleID, DE_DSecond& secMark, CDF_Position3D* pPos,
    CDF_PositionConfidenceSet* pPosCfdSet, DE_TransmissionState& transState, DE_Speed& speed, DE_Heading& heading, CDF_AccelerationSet4Way* pAccSet4Way, 
    CDF_BrakeSystemStatus* pBrakeStatus, CDF_VehicleSize* pVehicleSize, CDF_VehicleClassification* pVehicleClass, 
    CString& strPlateNo, DE_SteeringWheelAngle& angle, CDF_MotionConfidenceSet* pMotionCfdSet, CDF_VehicleSafetyExtensions* pVehicleExt)
{
    msgCount        = m_MsgCount;
    strVehicleID    = m_strVehicleID;
    secMark         = m_secMark;
    transState      = m_TransissionState;
    speed           = m_Speed;
    heading         = m_Heading;
    strPlateNo      = m_strPlateNo;
    angle           = m_SteeringWheelAngle;

    if ( pPos == NULL )
    {
        LogE("CBasicSafetyMessage GetParameter CDF_Position3D point is NULL");
        return false;
    }
    pPos->SetParameter(&m_Position3D);

    if ( pPosCfdSet == NULL )
    {
        LogE("CBasicSafetyMessage GetParameter CDF_PositionConfidenceSet point is NULL");
        return false;
    }
    pPosCfdSet->SetParameter(&m_Accuracy);

    if ( pAccSet4Way == NULL )
    {
        LogE("CBasicSafetyMessage GetParameter CDF_AccelerationSet4Way point is NULL");
        return false;
    }
    pAccSet4Way->SetParameter(&m_AccelerationSet4Way);

    if ( pBrakeStatus == NULL )
    {
        LogE("CBasicSafetyMessage GetParameter CDF_BrakeSystemStatus point is NULL");
        return false;
    }
    pBrakeStatus->SetParameter(&m_BrakeSystemStatus);

    if ( pVehicleSize == NULL )
    {
        LogE("CBasicSafetyMessage GetParameter CDF_VehicleSize point is NULL");
        return false;
    }
    pVehicleSize->SetParameter(&m_VehicleSize);

    if ( pVehicleClass == NULL )
    {
        LogE("CBasicSafetyMessage GetParameter CDF_VehicleClassification point is NULL");
        return false;
    }
    pVehicleClass->SetParameter(&m_VehicleClass);

    if ( pMotionCfdSet != NULL )
        pMotionCfdSet->SetParameter(&m_MotionConfidenceSet);

    if ( pVehicleExt != NULL )
        pVehicleExt->SetParameter(&m_VehicleSafetyExtensions);

    return true;
}

bool CBasicSafetyMessage::SetParameter(CBasicSafetyMessage* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CBasicSafetyMessage SetParameter CBasicSafetyMessage point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(m_MsgCount, m_strVehicleID, m_secMark, &m_Position3D, &m_Accuracy, 
        m_TransissionState, m_Speed, m_Heading, &m_AccelerationSet4Way, &m_BrakeSystemStatus, &m_VehicleSize, 
        &m_VehicleClass, m_strPlateNo, m_SteeringWheelAngle, &m_MotionConfidenceSet, &m_VehicleSafetyExtensions);

    return true;
}

void CBasicSafetyMessage::toParamString()                             // 将参数值输出到字符串中
{
    LogI("CBasicSafetyMessage: msgCount: %d, vehicleID: %s, secMark: %d, speed: %d, heading: %d, plateNo: %s, angle: %d",
        m_MsgCount, m_strVehicleID.GetBuffer(), m_secMark, m_Speed, m_Heading, m_strPlateNo.GetBuffer(), m_SteeringWheelAngle);
    m_Position3D.toParamString();
    m_AccelerationSet4Way.toParamString();
    m_BrakeSystemStatus.toParamString();
    m_VehicleSize.toParamString();
    m_VehicleClass.toParamString();
    m_MotionConfidenceSet.toParamString();
    m_VehicleSafetyExtensions.toParamString();
}

USHORT CBasicSafetyMessage::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeOptionFlag() == false )
    {
        LogE("Error:: CBasicSafetyMessage encode Optioanl MAP failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_MsgCount(m_MsgCount) == CODEC_FAILED )
    {
        LogE("CBasicSafetyMessage encode DE_MsgCount failed.");
        return CODEC_FAILED;
    }

    if ( m_strVehicleID.GetLength() != VEHICLE_ID_LEN )
    {
        LogE("CBasicSafetyMessage Vehicle No length %d is wrong! Vehicle No length is: %d", 
            m_strVehicleID.GetLength(), VEHICLE_ID_LEN);
        return CODEC_FAILED;
    }

    USHORT nLen = VEHICLE_ID_LEN;
    if ( EncodeOctString( (PUCHAR)m_strVehicleID.GetBuffer(), nLen) == false )
    {
        LogE("CBasicSafetyMessage encode Vehicle ID failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;
    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        USHORT nLen = m_strPlateNo.GetLength();
        if ( nLen != PLATE_NO_LEN )
        {
            LogE("CBasicSafetyMessage Plate No Length %d is wrong [%d, %d]",
                nLen, PLATE_NO_LEN);
            return CODEC_FAILED;
        }

        if ( EncodeOctString( (PUCHAR)m_strPlateNo.GetBuffer(), nLen) == false)
        {
            LogE("CBasicSafetyMessage encode Plate No failed.");
            return CODEC_FAILED;
        }
    }

    if ( EncodeDE_DSecond(m_secMark) == false)
    {
        LogE("CBasicSafetyMessage encode DE_DSecond failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDataFrame(&m_Position3D) == false )
    {
        LogE("CBasicSafetyMessage encode CDF_Position3D failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDataFrame(&m_Accuracy) == false )
    {
        LogE("CBasicSafetyMessage encode CDF_PositionConfidenceSet failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_TransmissionState(m_TransissionState) == false)
    {
        LogE("CBasicSafetyMessage encode DE_TransmissionState failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_Speed(m_Speed) == false)
    {
        LogE("CBasicSafetyMessage encode DE_Speed failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_Heading(m_Heading) == false)
    {
        LogE("CBasicSafetyMessage encode DE_Heading failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_SteeringWheelAngle(m_SteeringWheelAngle) == false)
        {
            LogE("CBasicSafetyMessage encode DE_SteeringWheelAngle failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDataFrame(&m_MotionConfidenceSet) == false )
        {
            LogE("CBasicSafetyMessage encode CDF_MotionConfidenceSet failed.");
            return CODEC_FAILED;
        }
    }

    if ( EncodeDataFrame(&m_AccelerationSet4Way) == false )
    {
        LogE("CBasicSafetyMessage encode CDF_AccelerationSet4Way failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDataFrame(&m_BrakeSystemStatus) == false )
    {
        LogE("CBasicSafetyMessage encode CDF_BrakeSystemStatus failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDataFrame(&m_VehicleSize) == false )
    {
        LogE("CBasicSafetyMessage encode CDF_VehicleSize failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDataFrame(&m_VehicleClass) == false )
    {
        LogE("CBasicSafetyMessage encode CDF_VehicleClassification failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex++) == true)
    {
        if ( EncodeDataFrame(&m_VehicleSafetyExtensions) == false )
        {
            LogE("CBasicSafetyMessage encode CDF_VehicleSafetyExtensions failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

USHORT CBasicSafetyMessage::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CBasicSafetyMessage decode OptionFlag failed.");
        return CODEC_FAILED; 
    }

    if ( DecodeDE_MsgCount(m_MsgCount) == CODEC_FAILED )
    {
        LogE("CBasicSafetyMessage decode DE_MsgCount failed.");
        return CODEC_FAILED;
    }

    char temp[MAX_STR_LEN];
    memset(temp, 0, MAX_STR_LEN);

    USHORT nLen = VEHICLE_ID_LEN;
    if ( DecodeOctString( (PUCHAR)temp, MAX_STR_LEN, nLen) == false )
    {
        LogE("CBasicSafetyMessage decode Vehicle ID failed.");
        return CODEC_FAILED;
    }

    temp[VEHICLE_ID_LEN] = '\0';
    m_strVehicleID = temp;

    int nOptionIndex = 0;
    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        nLen = PLATE_NO_LEN;
        memset(temp, 0, MAX_STR_LEN);
        if ( DecodeOctString( (PUCHAR)temp, MAX_STR_LEN, nLen) == false)
        {
            LogE("CBasicSafetyMessage decode Plate No failed.");
            return CODEC_FAILED;
        }

        if ( nLen != PLATE_NO_LEN )
        {
            LogE("CBasicSafetyMessage Plate No Length %d is wrong %d",
                nLen, PLATE_NO_LEN);
            return CODEC_FAILED;
        }

        temp[nLen] = '\0';
        m_strPlateNo = temp;
    }

    if ( DecodeDE_DSecond(m_secMark) == false)
    {
        LogE("CBasicSafetyMessage decode DE_DSecond failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDataFrame(&m_Position3D) == false )
    {
        LogE("CBasicSafetyMessage decode CDF_Position3D failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDataFrame(&m_Accuracy) == false )
    {
        LogE("CBasicSafetyMessage decode CDF_PositionConfidenceSet failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_TransmissionState(m_TransissionState) == false)
    {
        LogE("CBasicSafetyMessage decode DE_TransmissionState failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_Speed(m_Speed) == false)
    {
        LogE("CBasicSafetyMessage decode DE_Speed failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_Heading(m_Heading) == false)
    {
        LogE("CBasicSafetyMessage decode DE_Heading failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_SteeringWheelAngle(m_SteeringWheelAngle) == false)
        {
            LogE("CBasicSafetyMessage decode DE_SteeringWheelAngle failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDataFrame(&m_MotionConfidenceSet) == false )
        {
            LogE("CBasicSafetyMessage decode CDF_MotionConfidenceSet failed.");
            return CODEC_FAILED;
        }
    }

    if ( DecodeDataFrame(&m_AccelerationSet4Way) == false )
    {
        LogE("CBasicSafetyMessage decode CDF_AccelerationSet4Way failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDataFrame(&m_BrakeSystemStatus) == false )
    {
        LogE("CBasicSafetyMessage decode CDF_BrakeSystemStatus failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDataFrame(&m_VehicleSize) == false )
    {
        LogE("CBasicSafetyMessage decode CDF_VehicleSize failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDataFrame(&m_VehicleClass) == false )
    {
        LogE("CBasicSafetyMessage decode CDF_VehicleClassification failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex++) == true)
    {
        if ( DecodeDataFrame(&m_VehicleSafetyExtensions) == false )
        {
            LogE("CBasicSafetyMessage decode CDF_VehicleSafetyExtensions failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}


REGISTER_CLASS(CMapData)
CMapData::CMapData() 
{
    m_MsgCount      = 0;
    m_TimeStamp     = 0;

    m_nOptionCount  = 1;
}

bool CMapData::SetParameter(DE_MsgCount msgCount, CDF_NodeList* pNodeList, DE_MinuteOfTheYear moy)
{
    if ( CHECK_INT_VALID(DE_MsgCount, msgCount) == false )
    {
        LogE("CMapData SetParameter DE_MsgCount %d is out of range [%d, %d]",
            msgCount, DE_MsgCount_MIN, DE_MsgCount_MAX);
        return false;
    }

    m_MsgCount = msgCount;

    if ( pNodeList == NULL )
    {
        LogE("CMapData SetParameter CDF_NodeList point is NULL");
        return false;
    }

    m_NodesList.SetParameter(pNodeList);

    if ( CHECK_INT_VALID(DE_MinuteOfTheYear, moy) == true )
    {
        SetOptionFlag(0, true);
        m_TimeStamp = moy;
    }

    return true;
}

bool CMapData::GetParameter(DE_MsgCount& msgCount, CDF_NodeList* pNodeList, DE_MinuteOfTheYear moy )
{
    msgCount = m_MsgCount;
    moy = m_TimeStamp;

    if ( pNodeList == NULL )
    {
        LogE("CMapData GetParameter CDF_NodeList point is NULL");
        return false;
    }

    pNodeList->SetParameter(&m_NodesList);

    return true;
}

bool CMapData::SetParameter(CMapData* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CMapData SetParameter CMapData point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(m_MsgCount, &m_NodesList, m_TimeStamp);

    return true;
}

void CMapData::toParamString()                             // 将参数值输出到字符串中
{
    LogI("CMapData: msgCount: %d, MinuteOfYear: %d", m_MsgCount, m_TimeStamp);
    m_NodesList.toParamString();
}

USHORT CMapData::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeOptionFlag() == false )
    {
        LogE("Error:: CMapData encode Optioanl MAP failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_MsgCount(m_MsgCount) == CODEC_FAILED )
    {
        LogE("CMapData encode DE_MsgCount failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( EncodeDE_MinuteOfTheYear(m_TimeStamp) == false )
        {
            LogE("CMapData encode DE_MinuteOfTheYear failed");
            return CODEC_FAILED;
        }
    }

    if ( EncodeDataFrame( &m_NodesList ) == false )
    {
        LogE("CMapData encode CDF_NodeList failed");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

USHORT CMapData::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CMapData decode OptionFlag failed.");
        return CODEC_FAILED; 
    }

    if ( DecodeDE_MsgCount(m_MsgCount) == CODEC_FAILED )
    {
        LogE("CMapData decode DE_MsgCount failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( DecodeDE_MinuteOfTheYear(m_TimeStamp) == false )
        {
            LogE("CMapData decode DE_MinuteOfTheYear failed");
            return CODEC_FAILED;
        }
    }

    if ( DecodeDataFrame( &m_NodesList ) == false )
    {
        LogE("CMapData decode CDF_NodeList failed");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

REGISTER_CLASS(CRoadSideInformation)
CRoadSideInformation::CRoadSideInformation()
{
    m_MsgCount      = 0;
    m_TimeStamp     = 0;
    m_strRSUID      = "";
    m_nRSIID        = 0;
    m_AlertType     = DE_ALERT_TYPE_TEXT_MESSAGE;
    m_IA5StringDesc = "";
    m_Priority      = 0B00000000;
    m_AlertRadius   = 0;

    m_nOptionCount  = 3;
}

bool CRoadSideInformation::SetParameter(DE_MsgCount msgCount, CString strRSUID, int nRSIID, UINT alertType, CDF_Position3D* pRefPos, CDF_PathPointList* pPathPointList, DE_Radius alertRadius, DE_MinuteOfTheYear timeStamp, CString strDesc, DE_Priority priority)
{
    if ( CHECK_INT_VALID(DE_MsgCount, msgCount) == false )
    {
        LogE("CMapData SetParameter DE_MsgCount %d is out of range [%d, %d]",
            msgCount, DE_MsgCount_MIN, DE_MsgCount_MAX);
        return false;
    }

    m_MsgCount = msgCount;

    if ( strRSUID.GetLength() != RSU_ID_LEN )
    {
        LogE("CRoadSideInformation RSU ID length %d is wrong, RSU_ID_LEN No length is: %d", 
            strRSUID.GetLength(), RSU_ID_LEN);
        return false;
    }

    m_strRSUID = strRSUID;

    if ( nRSIID > 255 || nRSIID < 0 )
    {
        LogE("CRoadSideInformation RSIID  %d is out of range [0, 255]", nRSIID);
        return false;
    }

    m_nRSIID = nRSIID;

    if (CHECK_INT_VALID(DE_AlertType, alertType) == false )
    {
        LogE("CMapData SetParameter DE_AlertType %d is out of range [%d, %d]",
            alertType, DE_AlertType_MIN, DE_AlertType_MAX);
        return false;
    }

    m_AlertType = alertType;

    if ( pRefPos == NULL )
    {
        LogE("CMapData SetParameter CDF_Position3D point is NULL");
        return false;
    }
    m_Position3D.SetParameter(pRefPos);

    if ( pPathPointList == NULL )
    {
        LogE("CMapData SetParameter CDF_PathPointList point is NULL");
        return false;
    }
    m_PathPointList.SetParameter(pPathPointList);
    
    if ( CHECK_INT_VALID(DE_Radius, alertRadius) == false )
    {
        LogE("CMapData SetParameter DE_Radius %d is out of range [%d, %d]",
            alertRadius, DE_Radius_MIN, DE_Radius_MAX);
        return false;
    }
    m_AlertRadius = alertRadius;

    if ( CHECK_INT_VALID(DE_MinuteOfTheYear, timeStamp) == true )
    {
        SetOptionFlag(0, true);
        m_TimeStamp = timeStamp;
    }

    if ( strDesc != "")
    {
        SetOptionFlag(1, true);
        m_IA5StringDesc = strDesc;
    }

    if ( CHECK_INT_VALID(DE_Priority, priority) == true )
    {
        SetOptionFlag(2, true);
        m_Priority = priority;
    }

    return true;
}

bool CRoadSideInformation::GetParameter(DE_MsgCount &msgCount, CString &strRSUID, int &nRSIID, UINT &alertType, CDF_Position3D* pRefPos, CDF_PathPointList* pPathPointList, DE_Radius &alertRadius, DE_MinuteOfTheYear &timeStamp, CString &strDesc, DE_Priority &priority)
{
    msgCount    = m_MsgCount;
    strRSUID    = m_strRSUID;
    nRSIID      = m_nRSIID;;
    alertType   = m_AlertType;
    alertRadius = m_AlertRadius;
    timeStamp   = m_TimeStamp;
    strDesc     = m_IA5StringDesc;
    priority    = m_Priority;

    if ( pRefPos == NULL )
    {
        LogE("CMapData GetParameter CDF_Position3D point is NULL");
        return false;
    }
    pRefPos->SetParameter(&m_Position3D);

    if ( pPathPointList == NULL )
    {
        LogE("CMapData SetParameter CDF_PathPointList point is NULL");
        return false;
    }
    pPathPointList->SetParameter(&m_PathPointList);
    
    return true;
}

bool CRoadSideInformation::SetParameter(CRoadSideInformation* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CRoadSideInformation SetParameter CRoadSideInformation point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(m_MsgCount, m_strRSUID, m_nRSIID, m_AlertType, &m_Position3D, &m_PathPointList, m_AlertRadius, m_TimeStamp, m_IA5StringDesc, m_Priority);

    return true;
}

void CRoadSideInformation::toParamString()                            // 将参数值输出到字符串中
{
    LogI("CRoadSideInformation: msgCnt: %d, RSUID: %s, RSIID: %d, alertType: %d, alertRadius: %d, timeStamp: %d, desc: %s, priority: %d",
        m_MsgCount, m_strRSUID.GetBuffer(), m_nRSIID, m_AlertType, m_AlertRadius, m_TimeStamp, m_IA5StringDesc.GetBuffer(), m_Priority);
    m_Position3D.toParamString();
    m_PathPointList.toParamString();
}

USHORT CRoadSideInformation::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeOptionFlag() == false )
    {
        LogE("Error:: CRoadSideInformation encode Optioanl MAP failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_MsgCount(m_MsgCount) == CODEC_FAILED )
    {
        LogE("CRoadSideInformation encode DE_MsgCount failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_MinuteOfTheYear(m_TimeStamp) == false )
        {
            LogE("CRoadSideInformation encode DE_MinuteOfTheYear failed.");
            return CODEC_FAILED;
        }
    }

    if ( m_strRSUID.GetLength() != RSU_ID_LEN )
    {
        LogE("CRoadSideInformation RSU ID length %d is wrong! Vehicle No length is: %d", 
            m_strRSUID.GetLength(), RSU_ID_LEN);
        return CODEC_FAILED;
    }

    if ( EncodeOctString( (PUCHAR)m_strRSUID.GetBuffer(), RSU_ID_LEN) == false )
    {
        LogE("CRoadSideInformation encode Vehicle ID failed.");
        return CODEC_FAILED;
    }

    if ( EncodeUINT(m_nRSIID, 0, 255) == false )
    {
        LogE("CRoadSideInformation encode RSI ID failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_AlertType(m_AlertType) == false )
    {
        LogE("CRoadSideInformation encode DE_AlertType failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeCharacterString(m_IA5StringDesc, CST_PRINTABLE, DE_DescriptiveName_MIN, DE_DescriptiveName_MAX) == false )
        {
            LogE("CRoadSideInformation encode DE_AlertType failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_Priority(m_Priority) == false )
        {
            LogE("CRoadSideInformation encode DE_Priority failed.");
            return CODEC_FAILED;
        }
    }

    if ( EncodeDataFrame(&m_Position3D) == false )
    {
        LogE("CRoadSideInformation encode CDF_Position3D failed.");
        return CODEC_FAILED;
    }


    if ( EncodeDataFrame(&m_PathPointList) == false )
    {
        LogE("CRoadSideInformation encode CDF_PathPointList failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_Radius(m_AlertRadius) == false )
    {
        LogE("CRoadSideInformation encode DE_Radius failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

USHORT CRoadSideInformation::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("Error:: CRoadSideInformation decode Optioanl MAP failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_MsgCount(m_MsgCount) == CODEC_FAILED )
    {
        LogE("CRoadSideInformation decode DE_MsgCount failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_MinuteOfTheYear(m_TimeStamp) == false )
        {
            LogE("CRoadSideInformation decode DE_MinuteOfTheYear failed.");
            return CODEC_FAILED;
        }
    }

    char temp[MAX_STR_LEN];
    memset(temp, 0, MAX_STR_LEN);

    USHORT nLen = RSU_ID_LEN;
    if ( DecodeOctString( (PUCHAR)temp, MAX_STR_LEN, nLen) == false )
    {
        LogE("CRoadSideInformation decode RSU ID failed.");
        return CODEC_FAILED;
    }

    temp[nLen] = '\0';
    m_strRSUID = temp;

    if ( m_strRSUID.GetLength() != RSU_ID_LEN )
    {
        LogE("CRoadSideInformation RSU ID(%s) length %d is wrong! RSU No length is: %d", 
            m_strRSUID.GetBuffer(), m_strRSUID.GetLength(), RSU_ID_LEN);
        return CODEC_FAILED;
    }

    if ( DecodeInteger(m_nRSIID, 0, 255) == false )
    {
        LogE("CRoadSideInformation decode RSI ID failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_AlertType(m_AlertType) == false )
    {
        LogE("CRoadSideInformation decode DE_AlertType failed.");
        return CODEC_FAILED;
    }
    
    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeCharacterString( m_IA5StringDesc, CST_PRINTABLE, DE_DescriptiveName_MIN, DE_DescriptiveName_MAX) == false )
        {
            LogE("CRoadSideInformation decode DE_AlertType failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_Priority(m_Priority) == false )
        {
            LogE("CRoadSideInformation decode DE_Priority failed.");
            return CODEC_FAILED;
        }
    }

    if ( DecodeDataFrame(&m_Position3D) == false )
    {
        LogE("CRoadSideInformation decode CDF_Position3D failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDataFrame(&m_PathPointList) == false )
    {
        LogE("CRoadSideInformation decode CDF_PathPointList failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_Radius(m_AlertRadius) == false )
    {
        LogE("CRoadSideInformation decode DE_Radius failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

REGISTER_CLASS(CRoadSideSafetyMessage)

CRoadSideSafetyMessage::CRoadSideSafetyMessage() 
{
    m_MsgCount      = 0;
    m_strRSUID      = "";
}

bool CRoadSideSafetyMessage::SetRSUID(CString strID)
{
    if ( strID.GetLength() != RSU_ID_LEN )
    {
        LogE("CRoadSideSafetyMessage SetRSUID  RSU ID length %d is wrong! RSU ID length is: %d", 
            strID.GetLength(), RSU_ID_LEN);
        return false;
    }

    m_strRSUID = strID;
    return true;
}

bool CRoadSideSafetyMessage::SetParameter(DE_MsgCount msgCount, CString strRSUID, CDF_Position3D* pRefPos, CDF_ParticipantList* pPtcList)
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( CHECK_INT_VALID(DE_MsgCount, msgCount) == false )
    {
        LogE("CRoadSideSafetyMessage SetParameter DE_MsgCount %d is out of range [%d, %d]",
            msgCount, DE_MsgCount_MIN, DE_MsgCount_MAX);
        return false;
    }

    m_MsgCount = msgCount;

    if ( strRSUID.GetLength() != RSU_ID_LEN )
    {
        LogE("CRoadSideSafetyMessage RSU ID length %d is wrong, RSU_ID_LEN No length is: %d", 
            strRSUID.GetLength(), RSU_ID_LEN);
        return false;
    }

    m_strRSUID = strRSUID;

    if ( pRefPos == NULL )
    {
        LogE("CRoadSideSafetyMessage SetParameter CDF_Position3D point is NULL");
        return false;
    }
    m_Position3D.SetParameter(pRefPos);

    if ( pPtcList == NULL )
    {
        LogE("CRoadSideSafetyMessage SetParameter CDF_ParticipantList point is NULL");
        return false;
    }
    m_ParticipantList.SetParameter(pPtcList);

    return true;
}

bool CRoadSideSafetyMessage::GetParameter(DE_MsgCount& msgCount, CString &strRSUID, CDF_Position3D* pRefPos, CDF_ParticipantList* pPtcList)
{
    msgCount = m_MsgCount;
    strRSUID = m_strRSUID;

    if ( pRefPos == NULL )
    {
        LogE("CRoadSideSafetyMessage GetParameter CDF_Position3D point is NULL");
        return false;
    }
    pRefPos->SetParameter(&m_Position3D);

    if ( pPtcList == NULL )
    {
        LogE("CRoadSideSafetyMessage GetParameter CDF_ParticipantList point is NULL");
        return false;
    }
    pPtcList->SetParameter(&m_ParticipantList);

    return true;
}

bool CRoadSideSafetyMessage::SetParameter(CRoadSideSafetyMessage* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CRoadSideSafetyMessage SetParameter CRoadSideSafetyMessage point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(m_MsgCount, m_strRSUID, &m_Position3D, &m_ParticipantList);

    return true;
}

void CRoadSideSafetyMessage::toParamString()                             // 将参数值输出到字符串中
{
    LogI("CRoadSideSafetyMessage: msgCnt: %d, RSUID: %s", m_MsgCount, m_strRSUID.GetBuffer());
    m_Position3D.toParamString();
    m_ParticipantList.toParamString();
}

USHORT CRoadSideSafetyMessage::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeDE_MsgCount(m_MsgCount) == CODEC_FAILED )
    {
        LogE("CRoadSideSafetyMessage encode DE_MsgCount failed.");
        return CODEC_FAILED;
    }

    if ( m_strRSUID.GetLength() != RSU_ID_LEN )
    {
        LogE("CRoadSideSafetyMessage RSU ID length %d is wrong! RSU ID length is: %d", 
            m_strRSUID.GetLength(), RSU_ID_LEN);
        return CODEC_FAILED;
    }

    if ( EncodeOctString( (PUCHAR)m_strRSUID.GetBuffer(), RSU_ID_LEN) == false )
    {
        LogE("CRoadSideSafetyMessage encode RSU ID failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDataFrame(&m_Position3D) == false )
    {
        LogE("CRoadSideSafetyMessage encode CDF_Position3D failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDataFrame(&m_ParticipantList) == false )
    {
        LogE("CRoadSideSafetyMessage encode CDF_ParticipantList failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

USHORT CRoadSideSafetyMessage::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeDE_MsgCount(m_MsgCount) == CODEC_FAILED )
    {
        LogE("CRoadSideSafetyMessage decode DE_MsgCount failed.");
        return CODEC_FAILED;
    }

    char temp[MAX_STR_LEN];
    memset(temp, 0, MAX_STR_LEN);

    USHORT nLen = RSU_ID_LEN;
    if ( DecodeOctString( (PUCHAR)temp, MAX_STR_LEN, nLen) == false )
    {
        LogE("CRoadSideSafetyMessage decode Vehicle ID failed.");
        return CODEC_FAILED;
    }

    temp[nLen] = '\0';
    m_strRSUID = temp;

    if ( m_strRSUID.GetLength() != RSU_ID_LEN )
    {
        LogE("CRoadSideSafetyMessage RSU ID length %d is wrong! RSU ID length is: %d", 
            m_strRSUID.GetLength(), RSU_ID_LEN);
        return CODEC_FAILED;
    }

    if ( DecodeDataFrame(&m_Position3D) == false )
    {
        LogE("CRoadSideSafetyMessage decode CDF_Position3D failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDataFrame(&m_ParticipantList) == false )
    {
        LogE("CRoadSideSafetyMessage decode CDF_ParticipantList failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

REGISTER_CLASS(CSPAT)
CSPAT::CSPAT() 
{
    m_MsgCount          = 0;
    m_TimeStamp         = 0;
    m_DescriptiveName   = "";

    m_nOptionCount      = 2;
}

bool CSPAT::SetParameter(DE_MsgCount msgCount, CDF_IntersectionStateList* pStateList, DE_MinuteOfTheYear timeStamp, DE_DescriptiveName strDesc)
{
    if ( CHECK_INT_VALID(DE_MsgCount, msgCount) == false )
    {
        LogE("CSPAT SetParameter DE_MsgCount %d is out of range [%d, %d]",
            msgCount, DE_MsgCount_MIN, DE_MsgCount_MAX);
        return false;
    }
    m_MsgCount = msgCount;

    if ( pStateList == NULL )
    {
        LogE("CSPAT SetParameter CDF_IntersectionStateList point is NULL");
        return false;
    }
    m_InterSectionStateList.SetParameter(pStateList);

    if ( CHECK_INT_VALID(DE_MinuteOfTheYear, timeStamp) == true )
    {
        SetOptionFlag(0, true);
        m_TimeStamp = timeStamp;
    }

    if ( strDesc != "" )
    {
        SetOptionFlag(1, true);
        m_DescriptiveName = strDesc;
    }

    return true;
}

bool CSPAT::GetParameter(DE_MsgCount& msgCount, CDF_IntersectionStateList* pStateList, DE_MinuteOfTheYear& timeStamp, DE_DescriptiveName &strDesc)
{
    msgCount    = m_MsgCount;
    strDesc     = m_DescriptiveName;
    timeStamp   = m_TimeStamp;

    if ( pStateList == NULL )
    {
        LogE("CSPAT GetParameter CDF_IntersectionStateList point is NULL");
        return false;
    }
    pStateList->SetParameter(&m_InterSectionStateList);

    return true;
}

bool CSPAT::SetParameter(CSPAT* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CSPAT SetParameter CSPAT point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(m_MsgCount, &m_InterSectionStateList, m_TimeStamp, m_DescriptiveName);

    return true;
}

void CSPAT::toParamString()                             // 将参数值输出到字符串中
{
    LogI("CSPAT: msgCount: %d, timeStamp: %d, descName: %s", m_MsgCount, m_TimeStamp, m_DescriptiveName.GetBuffer());
    m_InterSectionStateList.toParamString();
}

USHORT CSPAT::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeOptionFlag() == false )
    {
        LogE("Error:: CSPAT encode Optioanl MAP failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_MsgCount(m_MsgCount) == CODEC_FAILED )
    {
        LogE("CSPAT encode DE_MsgCount failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;
    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_MinuteOfTheYear(m_TimeStamp) == false )
        {
            LogE("CSPAT encode DE_MinuteOfTheYear failed");
            return CODEC_FAILED;
        } 
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_DescriptiveName(m_DescriptiveName) == false )
        {
            LogE("CSPAT encode DE_DescriptiveName failed");
            return CODEC_FAILED;
        } 
    }

    if ( EncodeDataFrame(&m_InterSectionStateList) == false )
    {
        LogE("CSPAT encode CDF_IntersectionStateList failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}


USHORT CSPAT::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("Error:: CSPAT decode Optioanl MAP failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_MsgCount(m_MsgCount) == CODEC_FAILED )
    {
        LogE("CSPAT decode DE_MsgCount failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;
    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_MinuteOfTheYear(m_TimeStamp) == false )
        {
            LogE("CSPAT decode DE_MinuteOfTheYear failed");
            return CODEC_FAILED;
        } 
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_DescriptiveName(m_DescriptiveName) == false )
        {
            LogE("CSPAT decode DE_DescriptiveName failed");
            return CODEC_FAILED;
        } 
    }

    if ( DecodeDataFrame(&m_InterSectionStateList) == false )
    {
        LogE("CSPAT decode CDF_IntersectionStateList failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}


#define MSG_SET_COUNT                   5
const char* MsgSetClassName[] = { "CBasicSafetyMessage", "CMapData", "CRoadSideSafetyMessage", 
    "CSPAT", "CRoadSideInformation" };

REGISTER_CLASS(CMainMsgFrame)
CMainMsgFrame::CMainMsgFrame()
{
    m_nChoiceIndex  = -1;
    m_pMsg          = NULL; 
}

CMainMsgFrame::~CMainMsgFrame()
{
    if ( m_bIsReceived == false )
        return;

    if ( m_pMsg != NULL )
    {
        delete m_pMsg;
        m_pMsg = NULL;
    }
}

bool CMainMsgFrame::SetObject(CCSAEMsgBase* pMsg)
{
    if ( typeid(*pMsg) == typeid(CBasicSafetyMessage))
        m_nChoiceIndex = 0;
    else if ( typeid(*pMsg) == typeid(CMapData))
        m_nChoiceIndex = 1;
    else if ( typeid(*pMsg) == typeid(CRoadSideSafetyMessage))
        m_nChoiceIndex = 2;
    else if ( typeid(*pMsg) == typeid(CSPAT))
        m_nChoiceIndex = 3;
    else if ( typeid(*pMsg) == typeid(CRoadSideInformation))
        m_nChoiceIndex = 4;
    else
    {
        LogE("CMainMsgFrame SetObject parameter is wrong!");
        return false;
    }

    // LogI("CMainMsgFrame: Choice index : %d", m_nChoiceIndex);

    m_pMsg = pMsg;

    return true;
}

CCSAEMsgBase* CMainMsgFrame::GetObject(int &nChoiceIndex)
{
    nChoiceIndex = m_nChoiceIndex;
    return m_pMsg;
}

void CMainMsgFrame::toParamString()                             // 将参数值输出到字符串中
{
    LogI("CMainMsgFrame: MsgBody: %s", MsgSetClassName[m_nChoiceIndex]);
    if ( m_pMsg != NULL )
        m_pMsg->toParamString();
}

USHORT CMainMsgFrame::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    memset(m_Buffer, 0, MAX_ASN1_MSG_LEN);

    if ( m_pMsg == NULL )
    {
        LogE("CMainMsgFrame Msg is NULL!");
        return CODEC_FAILED;
    }

    if ( EncodeInteger(m_nChoiceIndex, 0, MSG_SET_COUNT) == false )
    {
        LogE("CMainMsgFrame encode choice index failed!");
        return CODEC_FAILED;
    }

    if ( EncodeDataFrame( m_pMsg ) == false )
    {
        LogE("CMainMsgFrame encode failed!");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

USHORT CMainMsgFrame::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeInteger(m_nChoiceIndex, 0, MSG_SET_COUNT) == false )
    {
        LogE("CMainMsgFrame decode choice index failed!");
        return CODEC_FAILED;
    }

    if ( m_nChoiceIndex < 0 || m_nChoiceIndex >= MSG_SET_COUNT )
    {
        LogE("CMainMsgFrame Decoed choice index %d is out of range [0, 4].", m_nChoiceIndex);
        return CODEC_FAILED;        
    }

    m_pMsg = CreateObject(MsgSetClassName[m_nChoiceIndex]);

    if ( DecodeDataFrame( m_pMsg ) == false )
    {
        LogE("CMainMsgFrame decode Msg body failed!");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}
