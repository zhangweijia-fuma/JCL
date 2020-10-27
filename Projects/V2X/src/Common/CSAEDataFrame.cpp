/*------------------------------------------------------------------------------
 * RSU - Component :: T/CSAE 53-2017 Specification
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    CSAEDataFrame.cpp
 * Purpose: Common module, T/CSAE 53-2017 Specification data frame definition
 *----------------------------------------------------------------------------*/
 
#include "CSAEDataFrame.h"


int DEFAULT_TIME_OFFSET;                                          // 缺省的和UTC时间相差的分钟数，默认是东八区

CDF_DataFrameListBase::CDF_DataFrameListBase()
{
    m_nListLen      = 0;
    m_nMinListLen   = 0;
    m_nMaxListLen   = MAX_DATA_FRAME_LIST_LEN;

    for ( int i = 0; i < MAX_DATA_FRAME_LIST_LEN; i++)
        m_DataFrameList[i] = NULL;

    m_strListObjectClassName = "";
}

CDF_DataFrameListBase::~CDF_DataFrameListBase()
{
    // 只有在接收消息时，会根据解码时得到的参数来动态生成对象，因此需要删除对此
    // 在发送消息时 不删除对象，由创建者来处理。
    // 因为对象列表中只是指针，因此在消息发送前，需要保证对象不被删除
    if ( m_bIsReceived == false )
        return;

    for(int i = 0; i < m_nListLen; i++)
    {
        if ( m_DataFrameList[i] != NULL )
        {
            delete m_DataFrameList[i];
            m_DataFrameList[i] = NULL;
        }
    }
}

int CDF_DataFrameListBase::AddListItem(CCSAEMsgBase* item)
{
    if ( m_nListLen + 1 >= m_nMaxListLen )
    {
        LogE("CDF_DataFrameListBase AddListItem list length %d is reached max length %d.", m_nListLen, m_nMaxListLen);
        return -1;
    }

    if ( item == NULL )
    {
        LogE("CDF_DataFrameListBase AddListItem Parameter is NULL pointer.");
        return -1;        
    }

    m_DataFrameList[m_nListLen] = item;
    m_nListLen ++;

    return m_nListLen - 1;
}

bool  CDF_DataFrameListBase::RemoveListItem(int nItemIndex)
{
    if ( m_nListLen  >= m_nMaxListLen )
    {
        LogE("CDF_DataFrameListBase RemoveListItem Parameter %d is out of range [0, %d].", nItemIndex, m_nMaxListLen );
        return NULL;
    }

    for ( int i = nItemIndex; i < m_nMaxListLen; i++ )
        m_DataFrameList[i] = m_DataFrameList[i+1];

    m_nListLen --;

    return true;

}

CCSAEMsgBase* CDF_DataFrameListBase::GetListItem(int nItemIndex)
{
    if ( nItemIndex >= m_nListLen || m_nListLen >= m_nMaxListLen )
    {
        LogE("CDF_DataFrameListBase GetListItem Parameter item index  %d is out of range [0, %d].", 
            nItemIndex, m_nListLen );
        return NULL;
    }

    return m_DataFrameList[nItemIndex];
}


// 赋值数据帧列表
// 赋值的只是指针，并没有重新构造对象，因此此时2个列表使用的对象是同一个对象
void CDF_DataFrameListBase::SetParameter(CDF_DataFrameListBase* pList)
{
    if ( pList == NULL )
    {
        LogE("CDF_DataFrameListBase SetParameter CDF_DataFrameListBase point is NULL");
        return;
    }

    CCSAEMsgBase::SetParameter(pList);
    
    m_nMinListLen   = pList->GetMinListLen();
    m_nMaxListLen   = pList->GetMaxListLen();
    m_nListLen      = pList->GetListLen();
    m_strListObjectClassName = pList->GetListClassName();      // 数据帧的类名，由派生类在构造函数中指定

    for(int i = 0; i < m_nListLen; i++)
    {
        // 只是赋值指针，对象本身还是使用的同一个实例
        m_DataFrameList[i] = pList->GetListItem(i);
    }

    return;
}

// 将数据帧列表输出到Log
void CDF_DataFrameListBase::toParamString()
{
    CString className = typeid(*this).name();

    LogI("%s: List Len: %d", GetClassName(this), m_nListLen);
    for(int i = 0; i < m_nListLen; i++)
    {
        if ( m_DataFrameList[i] != NULL )
            m_DataFrameList[i]->toParamString();
    }

    return;
}


USHORT CDF_DataFrameListBase::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeInteger(m_nListLen, m_nMinListLen, m_nMaxListLen) == false )
    {
        LogE("CDF_DataFrameListBase encode data frame list length failed.");
        return CODEC_FAILED;
    }

    for (int i = 0; i < m_nListLen; i++)
    {
        if ( EncodeDataFrame(m_DataFrameList[i]) == false )
        {
            LogE("CDF_DataFrameListBase encode data frame list failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

USHORT CDF_DataFrameListBase::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeInteger(m_nListLen, m_nMinListLen, m_nMaxListLen) == false)
    {
        LogE("CDF_DataFrameListBase decode data frame list length failed.");
        return CODEC_FAILED;
    }

    for (int i = 0; i < m_nListLen; i++)
    {
        m_DataFrameList[i] = CreateObject(m_strListObjectClassName);
        if ( DecodeDataFrame(m_DataFrameList[i]) == false )
        {
            LogE("CDF_DataFrameListBase decode data frame list item failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}



CDF_NodeReferenceID::CDF_NodeReferenceID()
{
    m_RegionID      = 0;                                           
    m_NodeID        = 0;

    m_nOptionCount  = 1;                                        
}

bool CDF_NodeReferenceID::SetParameter(DE_NodeID nodeID, DE_RoadRegulatorID regionID)
{
    m_NodeID = nodeID;

    if ( CHECK_INT_VALID(DE_RoadRegulatorID, regionID) == false )
    {
        // 可选参数超过取值范围，默认为不设置
        return true;
    }

    SetOptionFlag(0, true);

    m_RegionID  = regionID;

    return true;
}

bool CDF_NodeReferenceID::GetParameter(DE_NodeID &nodeID, DE_RoadRegulatorID &regionID)
{
    nodeID      = m_NodeID;
    regionID    = m_RegionID;

    return true;
}

bool CDF_NodeReferenceID::SetParameter(CDF_NodeReferenceID* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_NodeReferenceID SetParameter CDF_NodeReferenceID point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(m_NodeID, m_RegionID);
    return true;
}

void CDF_NodeReferenceID::toParamString()
{
    LogI("DF_NodeReferenceID: Region ID: %d, Node ID: %d", m_RegionID, m_NodeID);

    return;
}

USHORT CDF_NodeReferenceID::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    // 含有一个可选项，前导位图就只有1位，0表示可选项不在，1表示可选项存在
    // 在编码前，由应用完成对可选项的设置
    
    if ( EncodeOptionFlag() == false )
    {
        LogE("DF_NodeReferenceID encode option map faild.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( EncodeDE_RoadRegulatorID(m_RegionID) == false )
        {
            LogE("DF_NodeReferenceID encode DE_RoadRegulatorID failed.");
            return CODEC_FAILED;
        }
    }

    if ( EncodeDE_NodeID(m_NodeID) == false )
    {
        LogE("DF_NodeReferenceID encode NodeID failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

USHORT CDF_NodeReferenceID::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_Position3D decode option map failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( DecodeDE_RoadRegulatorID(m_RegionID) == false )
        {
            LogE("DF_NodeReferenceID decode DE_RoadRegulatorID failed.");
            return CODEC_FAILED;
        }
    }

    if ( DecodeDE_NodeID(m_NodeID) == false )
    {
        LogE("DF_NodeReferenceID decode NodeID failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

CDF_Position3D::CDF_Position3D()
{
    m_Latitude      = 0;
    m_Longtitude    = 0;
    m_Elevation     = 0;

    m_nOptionCount  = 1;
}

bool CDF_Position3D::SetParameter(DE_Latitude lat, DE_Longtitude lon, DE_Elevation elevation)
{
    if ( CHECK_INT_VALID(DE_Latitude, lat) == false )
    {
        LogI("CDF_Position3D SetParameter DE_Latitude %d is out of range [%llu, %llu]", lat, DE_Latitude_MIN, DE_Latitude_MAX);
        return false;
    }

    if ( CHECK_INT_VALID(DE_Longtitude, lon) == false )
    {
        LogI("CDF_Position3D SetParameter DE_Longtitude %d is out of range [%llu, %llu]", lon, DE_Longtitude_MIN, DE_Longtitude_MAX);
        return false;
    }

    m_Latitude      = lat;
    m_Longtitude    = lon;

    if ( CHECK_INT_VALID(DE_Elevation, elevation) == true )
    {
        SetOptionFlag(0, true);
        m_Elevation = elevation;
    }
    else
    {
        // 如果Elevation范围无效，则表示该可选参数值不设置
    }

    return true;
}

bool CDF_Position3D::GetParameter(DE_Latitude &lat, DE_Longtitude &lon, DE_Elevation &elevation)
{
    lat         = m_Latitude;
    lon         = m_Longtitude;

    if ( GetOptionFlag(0) == true )
        elevation   = m_Elevation;

    return true;
}

bool CDF_Position3D::SetParameter(CDF_Position3D* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_Position3D SetParameter CDF_Position3D point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(m_Latitude, m_Longtitude, m_Elevation);

    return true;
}

void CDF_Position3D::toParamString()
{
    LogI("DF_Position3D: Latitude: %d, Longtitude: %d, Elevation: %d", m_Latitude, m_Longtitude, m_Elevation);

    return;
}

USHORT CDF_Position3D::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    // 含有一个可选项，前导位图就只有1位，0表示可选项不在，1表示可选项存在
    // 在编码前，由应用完成对可选项的设置
    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_Position3D encode option map faild.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_Latitude(m_Latitude) == false )
    {
        LogE("CDF_Position3D encode DE_Latitude failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_Longtitude(m_Longtitude) == false )
    {
        LogE("CDF_Position3D encode DE_Longtitude failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( EncodeDE_Elevation(m_Elevation) == false )
        {
            LogE("CDF_Position3D encode DE_RoadRegulatorID failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

USHORT CDF_Position3D::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_Position3D decode option map failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_Latitude(m_Latitude) == false )
    {
        LogE("CDF_Position3D encode DE_Latitude failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_Longtitude(m_Longtitude) == false )
    {
        LogE("CDF_Position3D encode DE_Longtitude failed.");
        return CODEC_FAILED;
    }

    if ( m_bOptionFlag[0] == true )
    {
        if ( DecodeDE_Elevation(m_Elevation) == false )
        {
            LogE("CDF_Position3D decode DE_Elevation failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

CDF_PositionConfidenceSet::CDF_PositionConfidenceSet()
{
    m_PositoinCfd   = DE_PC_Unavailable;
    m_ElevationCfd  = DE_EC_Unavailable;

    m_nOptionCount  = 1;
}

bool CDF_PositionConfidenceSet::SetParameter(DE_PositionConfidence posCfd, DE_ElevationConfidence eleCfd)
{
    if ( CHECK_ENUM_VALID(DE_PositionConfidence, posCfd) == false )
    {
        LogI("CDF_PositionConfidenceSet SetParameter DE_PositionConfidence %d is invalid.", posCfd);
        return false;
    }

    m_PositoinCfd = posCfd;

    if ( CHECK_ENUM_VALID(DE_ElevationConfidence, eleCfd) == true )
    {
        SetOptionFlag(0, true);
        m_ElevationCfd = eleCfd;
    }
    else
    {
        // 超出范围时，默认不设置该值
    }

    return true;
}

bool CDF_PositionConfidenceSet::GetParameter(DE_PositionConfidence& posCfd, DE_ElevationConfidence& eleCfd)
{
    posCfd = m_PositoinCfd;
    eleCfd = m_ElevationCfd;

    return true;
}

bool CDF_PositionConfidenceSet::SetParameter(CDF_PositionConfidenceSet* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_PositionConfidenceSet SetParameter CDF_PositionConfidenceSet point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(m_PositoinCfd, m_ElevationCfd);

    return true;
}

void CDF_PositionConfidenceSet::toParamString()
{
    LogI("posCfd: %d, eleCfd: %d", m_PositoinCfd, m_ElevationCfd);

    return;
}

USHORT CDF_PositionConfidenceSet::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    // 含有一个可选项，前导位图就只有1位，0表示可选项不在，1表示可选项存在
    // 在编码前，由应用完成对可选项的设置
    
    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_PositionConfidenceSet encode option map faild.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_PositionConfidence(m_PositoinCfd) == false )
    {
        LogE("CDF_PositionConfidenceSet encode DE_PositionConfidence failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( EncodeDE_ElevationConfidence(m_ElevationCfd) == false )
        {
            LogE("CDF_PositionConfidenceSet encode DE_ElevationConfidence failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

USHORT CDF_PositionConfidenceSet::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    
    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_PositionConfidenceSet decode option map failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_PositionConfidence(m_PositoinCfd) == false )
    {
        LogE("CDF_PositionConfidenceSet encode DE_PositionConfidence failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( DecodeDE_ElevationConfidence(m_ElevationCfd) == false )
        {
            LogE("CDF_PositionConfidenceSet decode DE_ElevationConfidence failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

CDF_MotionConfidenceSet::CDF_MotionConfidenceSet()
{
    m_SpeedCfd      = DE_SC_Unavailable;
    m_HeadingCfd    = DE_HC_Unavailable;
    m_SteerCfd      = DE_SWAC_Unknown;

    m_nOptionCount  = 3;
}

bool CDF_MotionConfidenceSet::SetParameter(DE_SpeedConfidence speedCfd, DE_HeadingConfidence headingCfd, DE_SteeringWheelAngleConfidence steerCfd)
{
    if ( CHECK_ENUM_VALID(DE_SpeedConfidence, speedCfd) == true )
    {
        m_SpeedCfd = speedCfd;
        SetOptionFlag(0, true);
    }
    else
    {
        // 超出范围时，默认不设置该值
    }

    if ( CHECK_ENUM_VALID(DE_HeadingConfidence, headingCfd) == true )
    {
        m_HeadingCfd = headingCfd;
        SetOptionFlag(1, true);
    }
    else
    {
        // 超出范围时，默认不设置该值
    }

    if ( CHECK_ENUM_VALID(DE_SteeringWheelAngleConfidence, steerCfd) == true )
    {
        m_SteerCfd = steerCfd;
        SetOptionFlag(2, true);
    }
    else
    {
        // 超出范围时，默认不设置该值
    }

    return true;
}

bool CDF_MotionConfidenceSet::GetParameter(DE_SpeedConfidence &speedCfd, DE_HeadingConfidence &headingCfd, DE_SteeringWheelAngleConfidence &steerCfd)
{
    speedCfd    = m_SpeedCfd;
    headingCfd  = m_HeadingCfd;
    steerCfd    = m_SteerCfd;

    return true;
}

bool CDF_MotionConfidenceSet::SetParameter(CDF_MotionConfidenceSet* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_PositionConfidenceSet SetParameter CDF_PositionConfidenceSet point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(m_SpeedCfd, m_HeadingCfd, m_SteerCfd);

    return true;
}

void CDF_MotionConfidenceSet::toParamString()
{
    LogI("SpeedCfd: %d, HeadingCfd: %d, SteerCfd: %d", m_SpeedCfd, m_HeadingCfd, m_SteerCfd);
    return;
}

USHORT CDF_MotionConfidenceSet::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    // 含有3个可选项，前导位图就只有3位，0表示可选项不在，1表示可选项存在
    // 在编码前，由应用完成对可选项的设置
    // 多个可选项时，对每个可选项进行分别编码，在解码时，按照定长BitString进行解码
    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_MotionConfidenceSet encode option map faild.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( EncodeDE_SpeedConfidence(m_SpeedCfd) == false )
        {
            LogE("CDF_MotionConfidenceSet encode DE_SpeedConfidence failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(1) == true )
    {
        if ( EncodeDE_HeadingConfidence(m_HeadingCfd) == false )
        {
            LogE("CDF_MotionConfidenceSet encode DE_HeadingConfidence failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(2) == true )
    {
        if ( EncodeDE_SteeringWheelAngleConfidence(m_SteerCfd) == false )
        {
            LogE("CDF_MotionConfidenceSet encode DE_SteeringWheelAngleConfidence failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

USHORT CDF_MotionConfidenceSet::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;


    // 在有多个可选参数的时候，前导位图为定长比特串
    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_MotionConfidenceSet decode option map failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( DecodeDE_SpeedConfidence(m_SpeedCfd) == false )
        {
            LogE("CDF_MotionConfidenceSet decode DE_SpeedConfidence failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(1) == true )
    {
        if ( DecodeDE_HeadingConfidence(m_HeadingCfd) == false )
        {
            LogE("CDF_MotionConfidenceSet decode DE_HeadingConfidence failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(2) == true )
    {
        if ( DecodeDE_SteeringWheelAngleConfidence(m_SteerCfd) == false )
        {
            LogE("CDF_MotionConfidenceSet decode DE_SteeringWheelAngleConfidence failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

CDF_TimeChangeDetails::CDF_TimeChangeDetails() 
{
    m_StartTime     = 0;
    m_MinEndTime    = 0;
    m_MaxEndTime    = 0;
    m_LikelyEndTime = 0;
    m_TimeConfidence= 0;
    m_NextStartTime = 0;
    m_NextDuration  = 0;

    m_nOptionCount  = 5;
}

bool CDF_TimeChangeDetails::SetParameter(DE_TimeMark startTime, DE_TimeMark likelyEndTime, DE_TimeMark minEndTime, DE_TimeMark maxEndTime, 
    DE_Confidence timeConfidence, DE_TimeMark nextStartTime, DE_TimeMark nextDuration)
{
    if ( CHECK_INT_VALID(DE_TimeMark, startTime) == false )
    {
        LogE("CDF_TimeChangeDetails SetParameter startTime %d is out of range[%d, %d]",
            startTime, DE_TimeMark_MIN, DE_TimeMark_MAX);
        return false;
    }

    m_StartTime = startTime;

    if ( CHECK_INT_VALID(DE_TimeMark, likelyEndTime) == false )
    {
        LogE("CDF_TimeChangeDetails SetParameter likelyEndTime %d is out of range[%d, %d]",
            likelyEndTime, DE_TimeMark_MIN, DE_TimeMark_MAX);
        return false;
    }

    m_LikelyEndTime = likelyEndTime;

    if ( CHECK_INT_VALID(DE_TimeMark, minEndTime) == true )
    {
        m_MinEndTime = minEndTime;
        SetOptionFlag(0, true);
    }
    else
    {
        // 超出范围时，默认不设置该值
    }

    if ( CHECK_INT_VALID(DE_TimeMark, maxEndTime) == true )
    {
        m_MaxEndTime = maxEndTime;
        SetOptionFlag(1, true);
    }
    else
    {
        // 超出范围时，默认不设置该值
    }

    if ( CHECK_INT_VALID(DE_Confidence, timeConfidence) == true )
    {
        m_TimeConfidence = timeConfidence;
        SetOptionFlag(2, true);
    }
    else
    {
        // 超出范围时，默认不设置该值
    }

    if ( CHECK_INT_VALID(DE_TimeMark, nextStartTime) == true )
    {
        m_NextStartTime = nextStartTime;
        SetOptionFlag(3, true);
    }
    else
    {
        // 超出范围时，默认不设置该值
    }

    if ( CHECK_INT_VALID(DE_TimeMark, nextDuration) == true )
    {
        m_NextDuration = nextDuration;
        SetOptionFlag(4, true);
    }
    else
    {
        // 超出范围时，默认不设置该值
    }

    return true;
}

bool CDF_TimeChangeDetails::GetParameter(DE_TimeMark& startTime, DE_TimeMark& likelyEndTime, DE_TimeMark& minEndTime, 
    DE_TimeMark& maxEndTime, DE_Confidence& timeConfidence, DE_TimeMark& nextStartTime, DE_TimeMark& nextDuration)
{
    startTime       = m_StartTime;
    likelyEndTime   = m_LikelyEndTime;
    minEndTime      = m_MinEndTime;
    maxEndTime      = m_MaxEndTime;
    timeConfidence  = m_TimeConfidence;
    nextStartTime   = m_NextStartTime;
    nextDuration    = m_NextDuration;

    return true;
}

USHORT CDF_TimeChangeDetails::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    // 有5个可选参数
    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_TimeChangeDetails encode option map faild.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_TimeMark(m_StartTime) == false )
    {
        LogE("CDF_TimeChangeDetails encode StartTime faild.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( EncodeDE_TimeMark(m_MinEndTime) == false )
        {
            LogE("CDF_TimeChangeDetails encode minEndTime faild.");
            return CODEC_FAILED;
        }        
    }

    if ( GetOptionFlag(1) == true )
    {
        if ( EncodeDE_TimeMark(m_MaxEndTime) == false )
        {
            LogE("CDF_TimeChangeDetails encode maxEndTime faild.");
            return CODEC_FAILED;
        }        
    }

    if ( EncodeDE_TimeMark(m_LikelyEndTime) == false )
    {
        LogE("CDF_TimeChangeDetails encode likelyEndTime faild.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(2) == true )
    {
        if ( EncodeDE_Confidence(m_TimeConfidence) == false )
        {
            LogE("CDF_TimeChangeDetails encode TimeConfidence faild.");
            return CODEC_FAILED;
        }        
    }

    if ( GetOptionFlag(3) == true )
    {
        if ( EncodeDE_TimeMark(m_NextStartTime) == false )
        {
            LogE("CDF_TimeChangeDetails encode nextStartTime faild.");
            return CODEC_FAILED;
        }        
    }

    if ( GetOptionFlag(4) == true )
    {
        if ( EncodeDE_TimeMark(m_NextDuration) == false )
        {
            LogE("CDF_TimeChangeDetails encode nextDuration faild.");
            return CODEC_FAILED;
        }        
    }

    return m_nBitPos;
}

USHORT CDF_TimeChangeDetails::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;


    // 在有多个可选参数的时候，前导位图为定长比特串
    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_MotionConfidenceSet decode option map failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_TimeMark(m_StartTime) == false )
    {
        LogE("CDF_TimeChangeDetails decode StartTime faild.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( DecodeDE_TimeMark(m_MinEndTime) == false )
        {
            LogE("CDF_TimeChangeDetails decode minEndTime faild.");
            return CODEC_FAILED;
        }        
    }

    if ( GetOptionFlag(1) == true )
    {
        if ( DecodeDE_TimeMark(m_MaxEndTime) == false )
        {
            LogE("CDF_TimeChangeDetails decode maxEndTime faild.");
            return CODEC_FAILED;
        }        
    }

    if ( DecodeDE_TimeMark(m_LikelyEndTime) == false )
    {
        LogE("CDF_TimeChangeDetails decode likelyEndTime faild.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(2) == true )
    {
        if ( DecodeDE_Confidence(m_TimeConfidence) == false )
        {
            LogE("CDF_TimeChangeDetails decode TimeConfidence faild.");
            return CODEC_FAILED;
        }        
    }

    if ( GetOptionFlag(3) == true )
    {
        if ( DecodeDE_TimeMark(m_NextStartTime) == false )
        {
            LogE("CDF_TimeChangeDetails decode nextStartTime faild.");
            return CODEC_FAILED;
        }        
    }

    if ( GetOptionFlag(4) == true )
    {
        if ( DecodeDE_TimeMark(m_NextDuration) == false )
        {
            LogE("CDF_TimeChangeDetails decode nextDuration faild.");
            return CODEC_FAILED;
        }        
    }

    return m_nBitPos;
}

void CDF_TimeChangeDetails::SetParameter(CDF_TimeChangeDetails* pTiming)
{
    CCSAEMsgBase::SetParameter(pTiming);

    pTiming->GetParameter(m_StartTime, m_LikelyEndTime, m_MinEndTime, m_MaxEndTime, m_TimeConfidence, m_NextStartTime, m_NextDuration);

    return;
}

void CDF_TimeChangeDetails::toParamString()
{
    LogI("DF_TimeChangeDetails: startTime: %d, likelyEndTime: %d, minEndTime: %d, maxEndTime: %d, timeConfidence: %d, nextStartTime: %d, nextDuration: %d", 
        m_StartTime, m_LikelyEndTime, m_MinEndTime, m_MaxEndTime, m_TimeConfidence, m_NextStartTime, m_NextDuration);

    return;
}


REGISTER_CLASS(CDF_PhaseState)
CDF_PhaseState::CDF_PhaseState()
{
    m_LightState = DE_LightState_Unavaiable;
    m_nOptionCount = 1;
}

bool CDF_PhaseState::SetParameter(DE_LightState LightState, CDF_TimeChangeDetails* pTiming)
{
    if ( CHECK_ENUM_VALID(DE_LightState, LightState) == false )
    {
        LogE("CDF_PhaseState SetParameter DE_LightState %d is out of range[0, %d]",
            LightState, DE_LightState_Invalid);
        return false;
    }

    m_LightState = LightState;

    // 无可选参数
    if ( pTiming == NULL )
        return true;

    m_Timing.SetParameter(pTiming);

    SetOptionFlag(0, true);

    return true;
}

bool CDF_PhaseState::GetParameter(DE_LightState &LightState, CDF_TimeChangeDetails* pTiming)
{
    m_LightState = LightState;

    if ( pTiming == NULL )
        return true;

    pTiming->SetParameter(&m_Timing);
    return true;
}

void CDF_PhaseState::SetParameter(CDF_PhaseState* pPhaseState)
{
    CCSAEMsgBase::SetParameter(pPhaseState);

    pPhaseState->GetParameter(m_LightState, &m_Timing);

    return;
}

USHORT CDF_PhaseState::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    // 含有一个可选项，前导位图就只有1位，0表示可选项不在，1表示可选项存在
    // 在编码前，由应用完成对可选项的设置
    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_PhaseState encode option map faild.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_LightState(m_LightState) == false )
    {
        LogE("CDF_PhaseState encode LightState failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( EncodeDataFrame(&m_Timing) == false )
        {
            LogE("CDF_PhaseState encode CDF_TimeChangeDetails failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

USHORT CDF_PhaseState::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_PhaseState decode option map failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_LightState(m_LightState) == false )
    {
        LogE("CDF_PhaseState encode DE_LightState failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( DecodeDataFrame( &m_Timing ) == false )
        {
            LogE("CDF_Position3D decode CDF_TimeChangeDetails failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

void CDF_PhaseState::toParamString()
{
    LogI("CDF_PhaseState: LightState: %d", m_LightState);
    m_Timing.toParamString();

    return;
}


CDF_PhaseStateList::CDF_PhaseStateList()
{
    m_nMinListLen = MIN_PHASE_STATE_LIST_LEN;
    m_nMaxListLen = MAX_PHASE_STATE_LIST_LEN;
    m_strListObjectClassName = "CDF_PhaseState";
}


REGISTER_CLASS(CDF_Phase)
CDF_Phase::CDF_Phase()
{
    m_PhaseID = 0;
}

bool CDF_Phase::SetParameter(DE_PhaseID phaseID, CDF_PhaseStateList* pList)
{
    m_PhaseID = phaseID;
    m_PhaseStatesList.SetParameter(pList);

    return true;
}

bool CDF_Phase::GetParameter(DE_PhaseID &phaseID, CDF_PhaseStateList* pList)
{
    if ( pList == NULL )
    {
        LogE("CDF_PhaseStateList GetParameter CDF_PhaseStateList point is NULL");
        return false;
    }

    phaseID = m_PhaseID;
    pList->SetParameter(&m_PhaseStatesList);

    return true;
}

void CDF_Phase::SetParameter(CDF_Phase* pPhase)
{
    CCSAEMsgBase::SetParameter(pPhase);

    pPhase->GetParameter(m_PhaseID, &m_PhaseStatesList);

    return;
}

void CDF_Phase::toParamString()                             // 将参数值输出到字符串中
{
    LogI("DF_Phase: PhaseID: %d", m_PhaseID);
    m_PhaseStatesList.toParamString();
    return;
}

USHORT CDF_Phase::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeDE_PhaseID(m_PhaseID) == false )
    {
        LogE("CDF_Phase encode DE_PhaseID failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDataFrame( &m_PhaseStatesList ) == false)
    {
        LogE("CDF_PhaseStateList encode CDF_PhaseState failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

USHORT CDF_Phase::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;


    if ( DecodeDE_PhaseID(m_PhaseID) == false )
    {
        LogE("CDF_Phase decode DE_PhaseID failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDataFrame(&m_PhaseStatesList) == false )
    {
        LogE("CDF_Phase decode CDF_PhaseState failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}


CDF_PhaseList::CDF_PhaseList()
{
    m_nMinListLen = MIN_PHASE_LEN; 
    m_nMaxListLen = MAX_PHASE_LEN;

    m_strListObjectClassName = "CDF_Phase";
}


REGISTER_CLASS(CDF_RegulatorySpeedLimit)
CDF_RegulatorySpeedLimit::CDF_RegulatorySpeedLimit() 
{
    m_SpeedLimitType    = DE_SLT_Unknown;
    m_Speed             = 0;
}

bool CDF_RegulatorySpeedLimit::SetParameter(DE_SpeedLimitType speedLimitType, DE_Speed speed)
{
    m_SpeedLimitType = speedLimitType;
    m_Speed = speed;

    return true;
}

bool CDF_RegulatorySpeedLimit::GetParameter(DE_SpeedLimitType &speedLimitType, DE_Speed &speed)
{
    speedLimitType = m_SpeedLimitType;
    speed = m_Speed;

    return true;
}

void CDF_RegulatorySpeedLimit::SetParameter(CDF_RegulatorySpeedLimit* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_RegulatorySpeedLimit SetParameter CDF_RegulatorySpeedLimit point is NULL");
        return;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(m_SpeedLimitType, m_Speed);

    return;
}

void CDF_RegulatorySpeedLimit::toParamString()                             // 将参数值输出到字符串中
{
    LogI("DF_RegulatorySpeedLimit: speed limit type: %d, speed: %d", m_SpeedLimitType, m_Speed);
    return;
}


USHORT CDF_RegulatorySpeedLimit::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeDE_SpeedLimitType(m_SpeedLimitType) == false )
    {
        LogE("CDF_RegulatorySpeedLimit encode DE_SpeedLimitType failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_Speed(m_Speed) == false )
    {
        LogE("CDF_RegulatorySpeedLimit encode DE_Speed failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

USHORT CDF_RegulatorySpeedLimit::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;


    if ( DecodeDE_SpeedLimitType(m_SpeedLimitType) == false )
    {
        LogE("CDF_RegulatorySpeedLimit decode DE_SpeedLimitType failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_Speed(m_Speed) == false )
    {
        LogE("CDF_RegulatorySpeedLimit decode DE_Speed failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}


CDF_SpeedLimitList::CDF_SpeedLimitList()
{
    m_nMinListLen = MIN_SPEED_LIMIT_LEN; 
    m_nMaxListLen = MAX_SPEED_LIMIT_LEN;

    m_strListObjectClassName = "CDF_RegulatorySpeedLimit";
}


#define POSITION_OFFSET_CLASS_COUNT     7
const char*  PositionOffsetClass[] = {
    "CDF_PositionLL24B", "CDF_PositionLL28B", "CDF_PositionLL32B", "CDF_PositionLL36B",
    "CDF_PositionLL44B", "CDF_PositionLL48B", "CDF_PositionLLmD_64B"
};

REGISTER_CLASS(CDF_PositionLL24B)
REGISTER_CLASS(CDF_PositionLL28B)
REGISTER_CLASS(CDF_PositionLL32B)
REGISTER_CLASS(CDF_PositionLL36B)
REGISTER_CLASS(CDF_PositionLL44B)
REGISTER_CLASS(CDF_PositionLL48B)
REGISTER_CLASS(CDF_PositionLLmD_64B)


CDF_PositionLLBase::CDF_PositionLLBase() 
{
    m_Longtitude        = 0;
    m_Latitude          = 0;

    m_nLongtitudeMin    = 0;
    m_nLongtitudeMax    = 0;
    m_nLatitudeMin      = 0;
    m_nLatitudeMax      = 0; 
}

void CDF_PositionLLBase::SetParameterRange()
{
    CString className = typeid(*this).name();

    if ( className.Find("CDF_PositionLL24B") >= 0 )
    {
        m_nLongtitudeMin = m_nLatitudeMin = DE_OffsetLL_B12_MIN;
        m_nLongtitudeMax = m_nLatitudeMax = DE_OffsetLL_B12_MAX;
    }
    else if ( className.Find("CDF_PositionLL28B") >= 0 )
    {
        m_nLongtitudeMin = m_nLatitudeMin = DE_OffsetLL_B14_MIN;
        m_nLongtitudeMax = m_nLatitudeMax = DE_OffsetLL_B14_MAX;
    }
    else if ( className.Find("CDF_PositionLL32B") >= 0 )
    {
        m_nLongtitudeMin = m_nLatitudeMin = DE_OffsetLL_B16_MIN;
        m_nLongtitudeMax = m_nLatitudeMax = DE_OffsetLL_B16_MAX;
    }
    else if ( className.Find("CDF_PositionLL36B") >= 0 )
    {
        m_nLongtitudeMin = m_nLatitudeMin = DE_OffsetLL_B18_MIN;
        m_nLongtitudeMax = m_nLatitudeMax = DE_OffsetLL_B18_MAX;
    }
    else if ( className.Find("CDF_PositionLL44B") >= 0 )
    {
        m_nLongtitudeMin = m_nLatitudeMin = DE_OffsetLL_B22_MIN;
        m_nLongtitudeMax = m_nLatitudeMax = DE_OffsetLL_B22_MAX;
    }
    else if ( className.Find("CDF_PositionLL48B") >= 0 )
    {
        m_nLongtitudeMin = m_nLatitudeMin = DE_OffsetLL_B24_MIN;
        m_nLongtitudeMax = m_nLatitudeMax = DE_OffsetLL_B24_MAX;
    }
    else if ( className.Find("CDF_PositionLLmD_64B") >= 0 )
    {
        m_nLongtitudeMin    = DE_Longtitude_MIN;
        m_nLongtitudeMax    = DE_Longtitude_MAX;
        m_nLatitudeMin      = DE_Latitude_MIN;
        m_nLatitudeMax      = DE_Latitude_MAX; 
    }
    else 
    {
        LogE("CDF_PositionLLBase Can not find valid class name.");
    }
}

bool CDF_PositionLLBase::SetParameter(int longtitude, int latitude)
{
    if ( longtitude < m_nLongtitudeMin || longtitude > m_nLongtitudeMax )
    {
        LogE("CDF_PositionLLBase SetParameter parameter longtitude %d is out of range [%d, %d]",
            longtitude, m_nLongtitudeMin, m_nLongtitudeMax);

        return false;
    }

    if ( latitude < m_nLatitudeMin || latitude > m_nLatitudeMax)
    {
        LogE("CDF_PositionLLBase SetParameter parameter latitude %d is out of range: [%d, %d]",
            latitude, m_nLatitudeMin, m_nLatitudeMax);

        return false;
    }

    m_Longtitude = longtitude;
    m_Latitude   = latitude;

    return true;
}

bool CDF_PositionLLBase::GetParameter(int &longtitude, int &latitude)
{
    longtitude  = m_Longtitude;
    latitude    = m_Latitude;

    return true;
}

void CDF_PositionLLBase::SetParameter(CDF_PositionLLBase* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_PositionLLBase SetParameter CDF_PositionLLBase point is NULL");
        return;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(m_Longtitude, m_Latitude);
}

void CDF_PositionLLBase::toParamString()                             // 将参数值输出到字符串中
{
    LogI("DF_PositionLLBase: Longtitude: %d, Latitude: %d", m_Longtitude, m_Latitude);

    return;
}

USHORT CDF_PositionLLBase::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( typeid(*this) == typeid(CDF_PositionLL24B))
    {
        if ( EncodeDE_OffsetLL_B12(m_Longtitude) == false )
        {
            LogE("CDF_PositionLLBase encode DE_OffsetLL_B12 Longtitude failed.");
            return CODEC_FAILED;
        }

        if ( EncodeDE_OffsetLL_B12(m_Latitude) == false )
        {
            LogE("CDF_PositionLLBase encode DE_OffsetLL_B12 Latitude failed.");
            return CODEC_FAILED;
        }
    }
    else if ( typeid(*this) == typeid(CDF_PositionLL28B))
    {
        if ( EncodeDE_OffsetLL_B14(m_Longtitude) == false )
        {
            LogE("CDF_PositionLLBase encode DE_OffsetLL_B14 Longtitude failed.");
            return CODEC_FAILED;
        }

        if ( EncodeDE_OffsetLL_B14(m_Latitude) == false )
        {
            LogE("CDF_PositionLLBase encode DE_OffsetLL_B14 Latitude failed.");
            return CODEC_FAILED;
        }
    }
    else if ( typeid(*this) == typeid(CDF_PositionLL32B))
    {
        if ( EncodeDE_OffsetLL_B16(m_Longtitude) == false )
        {
            LogE("CDF_PositionLLBase encode DE_OffsetLL_B16 Longtitude failed.");
            return CODEC_FAILED;
        }

        if ( EncodeDE_OffsetLL_B16(m_Latitude) == false )
        {
            LogE("CDF_PositionLLBase encode DE_OffsetLL_B16 Latitude failed.");
            return CODEC_FAILED;
        }
    }
    else if ( typeid(*this) == typeid(CDF_PositionLL36B))
    {
        if ( EncodeDE_OffsetLL_B18(m_Longtitude) == false )
        {
            LogE("CDF_PositionLLBase encode DE_OffsetLL_B18 Longtitude failed.");
            return CODEC_FAILED;
        }

        if ( EncodeDE_OffsetLL_B18(m_Latitude) == false )
        {
            LogE("CDF_PositionLLBase encode DE_OffsetLL_B18 Latitude failed.");
            return CODEC_FAILED;
        }
    }
    else if ( typeid(*this) == typeid(CDF_PositionLL44B))
    {
        if ( EncodeDE_OffsetLL_B22(m_Longtitude) == false )
        {
            LogE("CDF_PositionLLBase encode DE_OffsetLL_B22 Longtitude failed.");
            return CODEC_FAILED;
        }

        if ( EncodeDE_OffsetLL_B22(m_Latitude) == false )
        {
            LogE("CDF_PositionLLBase encode DE_OffsetLL_B22 Latitude failed.");
            return CODEC_FAILED;
        }
    }
    else if ( typeid(*this) == typeid(CDF_PositionLL48B))
    {
        if ( EncodeDE_OffsetLL_B24(m_Longtitude) == false )
        {
            LogE("CDF_PositionLLBase encode DE_OffsetLL_B24 Longtitude failed.");
            return CODEC_FAILED;
        }

        if ( EncodeDE_OffsetLL_B24(m_Latitude) == false )
        {
            LogE("CDF_PositionLLBase encode DE_OffsetLL_B24 Latitude failed.");
            return CODEC_FAILED;
        }
    }
    else if ( typeid(*this) == typeid(CDF_PositionLLmD_64B))
    {
        long long nTemp = m_Longtitude;
        if ( EncodeDE_Longtitude(nTemp) == false )
        {
            LogE("CDF_PositionLLBase encode  Longtitude failed.");
            return CODEC_FAILED;
        }

        if ( EncodeDE_Latitude(m_Latitude) == false )
        {
            LogE("CDF_PositionLLBase encode  Latitude failed.");
            return CODEC_FAILED;
        }
    }
    else
    {
        LogE("CDF_PositionLLBase encode Parameter error!");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

USHORT CDF_PositionLLBase::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( typeid(*this) == typeid(CDF_PositionLL24B))
    {
        if ( DecodeDE_OffsetLL_B12(m_Longtitude) == false )
        {
            LogE("CDF_PositionLLBase decode DE_OffsetLL_B12 Longtitude failed.");
            return CODEC_FAILED;
        }

        if ( DecodeDE_OffsetLL_B12(m_Latitude) == false )
        {
            LogE("CDF_PositionLLBase decode DE_OffsetLL_B12 Latitude failed.");
            return CODEC_FAILED;
        }
    }
    else if ( typeid(*this) == typeid(CDF_PositionLL28B))
    {
        if ( DecodeDE_OffsetLL_B14(m_Longtitude) == false )
        {
            LogE("CDF_PositionLLBase decode DE_OffsetLL_B14 Longtitude failed.");
            return CODEC_FAILED;
        }

        if ( DecodeDE_OffsetLL_B14(m_Latitude) == false )
        {
            LogE("CDF_PositionLLBase decode DE_OffsetLL_B14 Latitude failed.");
            return CODEC_FAILED;
        }
    }
    else if ( typeid(*this) == typeid(CDF_PositionLL32B))
    {
        if ( DecodeDE_OffsetLL_B16(m_Longtitude) == false )
        {
            LogE("CDF_PositionLLBase decode DE_OffsetLL_B16 Longtitude failed.");
            return CODEC_FAILED;
        }

        if ( DecodeDE_OffsetLL_B16(m_Latitude) == false )
        {
            LogE("CDF_PositionLLBase decode DE_OffsetLL_B16 Latitude failed.");
            return CODEC_FAILED;
        }
    }
    else if ( typeid(*this) == typeid(CDF_PositionLL36B))
    {
        if ( DecodeDE_OffsetLL_B18(m_Longtitude) == false )
        {
            LogE("CDF_PositionLLBase decode DE_OffsetLL_B18 Longtitude failed.");
            return CODEC_FAILED;
        }

        if ( DecodeDE_OffsetLL_B18(m_Latitude) == false )
        {
            LogE("CDF_PositionLLBase decode DE_OffsetLL_B18 Latitude failed.");
            return CODEC_FAILED;
        }
    }
    else if ( typeid(*this) == typeid(CDF_PositionLL44B))
    {
        if ( DecodeDE_OffsetLL_B22(m_Longtitude) == false )
        {
            LogE("CDF_PositionLLBase decode DE_OffsetLL_B22 Longtitude failed.");
            return CODEC_FAILED;
        }

        if ( DecodeDE_OffsetLL_B22(m_Latitude) == false )
        {
            LogE("CDF_PositionLLBase decode DE_OffsetLL_B22 Latitude failed.");
            return CODEC_FAILED;
        }
    }
    else if ( typeid(*this) == typeid(CDF_PositionLL48B))
    {
        if ( DecodeDE_OffsetLL_B24(m_Longtitude) == false )
        {
            LogE("CDF_PositionLLBase decode DE_OffsetLL_B24 Longtitude failed.");
            return CODEC_FAILED;
        }

        if ( DecodeDE_OffsetLL_B24(m_Latitude) == false )
        {
            LogE("CDF_PositionLLBase decode DE_OffsetLL_B24 Latitude failed.");
            return CODEC_FAILED;
        }
    }
    else if ( typeid(*this) == typeid(CDF_PositionLLmD_64B))
    {
        // 因为在编解码时，对整数需要计算整数的取值范围
        // 由于经度的取值范围（最大值 - 最小值）已经超过了整数的最大最小值，
        // 但是经度的取值没有超过整数的最大最小值，
        // 所以在编解码时，使用一个临时变量进行编解码
        long long nTemp = 0;
        if ( DecodeDE_Longtitude(nTemp) == false )
        {
            LogE("CDF_PositionLLBase decode  Longtitude failed.");
            return CODEC_FAILED;
        }
        m_Longtitude = nTemp;

        if ( DecodeDE_Latitude(m_Latitude) == false )
        {
            LogE("CDF_PositionLLBase decode  Latitude failed.");
            return CODEC_FAILED;
        }
    }
    else
    {
        LogE("CDF_PositionLLBase decode Parameter error!");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}


CDF_PositionOffsetLL::CDF_PositionOffsetLL()
{
    m_nChoiceIndex = 0; 
    m_pOffsetOject = NULL; 
}

CDF_PositionOffsetLL::~CDF_PositionOffsetLL()
{
    if ( m_bIsReceived == false )
        return;

    if ( m_pOffsetOject != NULL )
        delete m_pOffsetOject;
}

CDF_PositionLLBase* CDF_PositionOffsetLL::GetObject()
{
    return m_pOffsetOject;
}

void CDF_PositionOffsetLL::SetObject(CDF_PositionLLBase* pOffsetObject)
{
    if ( pOffsetObject == NULL )
        return;

    // 根据对象的派生类类型，确定选择项的序号
    if ( typeid(*pOffsetObject) == typeid(CDF_PositionLL24B))
        m_nChoiceIndex = 0;
    else if ( typeid(*pOffsetObject) == typeid(CDF_PositionLL28B))
        m_nChoiceIndex = 1;
    else if ( typeid(*pOffsetObject) == typeid(CDF_PositionLL32B))
        m_nChoiceIndex = 2;
    else if ( typeid(*pOffsetObject) == typeid(CDF_PositionLL36B))
        m_nChoiceIndex = 3;
    else if ( typeid(*pOffsetObject) == typeid(CDF_PositionLL44B))
        m_nChoiceIndex = 4;
    else if ( typeid(*pOffsetObject) == typeid(CDF_PositionLL48B))
        m_nChoiceIndex = 5;
    else if ( typeid(*pOffsetObject) == typeid(CDF_PositionLLmD_64B))
        m_nChoiceIndex = 6;
    else 
    {
        LogE("CDF_PositionOffsetLL SetObject Parameter error!");
        return;
    }

    m_pOffsetOject = pOffsetObject;

    return;
}

void CDF_PositionOffsetLL::SetParameter(CDF_PositionOffsetLL* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_PositionOffsetLL SetParameter CDF_PositionOffsetLL point is NULL");
        return;
    }

    CCSAEMsgBase::SetParameter(pObj);

    SetObject(pObj->GetObject());
}

void CDF_PositionOffsetLL::toParamString()
{
    LogI("DF_PositionOffsetLL: choice Index: %d", m_nChoiceIndex);
    if ( m_pOffsetOject != NULL )
        m_pOffsetOject->toParamString();

    return;
}

USHORT CDF_PositionOffsetLL::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( m_pOffsetOject == NULL )
    {
        LogE("CDF_PositionOffsetLL encode Parameter error!");
        return CODEC_FAILED;
    }

    if ( EncodeInteger(m_nChoiceIndex, 0, POSITION_OFFSET_CLASS_COUNT - 1) == false )
    {
        LogE("CDF_PositionOffsetLL encode failed!");
        return CODEC_FAILED;
    }

    if ( EncodeDataFrame( m_pOffsetOject ) == false )
    {
        LogE("CDF_PositionOffsetLL encode failed!");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

USHORT CDF_PositionOffsetLL::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    int nByteIndex = m_nBitPos / 8;
    int nBitIndex = m_nBitPos - nByteIndex * 8;

    if ( DecodeInteger(m_nChoiceIndex, 0, POSITION_OFFSET_CLASS_COUNT - 1) == false )
    {
        LogE("CDF_PositionOffsetLL decode Choice failed!");
        return CODEC_FAILED;
    }

    if ( m_nChoiceIndex < 0 || m_nChoiceIndex >= POSITION_OFFSET_CLASS_COUNT )
    {
        LogE("CDF_PositionOffsetLL decode CHOICE %d is out of range [0 ~ 6].", m_nChoiceIndex);
        return CODEC_FAILED;
    }

    m_pOffsetOject = (CDF_PositionLLBase*)CreateObject( PositionOffsetClass[m_nChoiceIndex] );

    if ( DecodeDataFrame(m_pOffsetOject) == false )
    {
        LogE("CDF_PositionOffsetLL decode failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

CDF_VerticalOffset::CDF_VerticalOffset() 
{
    m_nChoiceIndex      = 0;
    m_nVerticalOffset   = 0;
}

bool CDF_VerticalOffset::SetParameter(int nChoiceIndex, int nVerticalOffset)
{
    int nMin, nMax;
    switch(nChoiceIndex)
    {
    case VERTICAL_OFFSET_B07:
        nMin = DE_VertOffset_B07_MIN;
        nMax = DE_VertOffset_B07_MAX;
        break;
    case VERTICAL_OFFSET_B08:
        nMin = DE_VertOffset_B08_MIN;
        nMax = DE_VertOffset_B08_MAX;
        break;
    case VERTICAL_OFFSET_B09:
        nMin = DE_VertOffset_B09_MIN;
        nMax = DE_VertOffset_B09_MAX;
        break;
    case VERTICAL_OFFSET_B10:
        nMin = DE_VertOffset_B10_MIN;
        nMax = DE_VertOffset_B10_MAX;
        break;
    case VERTICAL_OFFSET_B11:
        nMin = DE_VertOffset_B11_MIN;
        nMax = DE_VertOffset_B11_MAX;
        break;
    case VERTICAL_OFFSET_B12:
        nMin = DE_VertOffset_B12_MIN;
        nMax = DE_VertOffset_B12_MAX;
        break;
    case VERTICAL_OFFSET_ELEVATION:
        nMin = DE_Elevation_MIN;
        nMax = DE_Elevation_MAX;
        break;
    default:
        LogE("CDF_VerticalOffset SetOffset Choice Index is out of range. ");
        return false;
    }

    if ( nVerticalOffset < nMin || nVerticalOffset > nMax )
    {
        LogE("CDF_VerticalOffset SetParameter VerticalOffset %d is out of range %d ~ %d. ", nVerticalOffset, nMin, nMax);
        return false;
    }

    m_nChoiceIndex      = nChoiceIndex;
    m_nVerticalOffset   = nVerticalOffset;

    return true;
}

bool CDF_VerticalOffset::GetParameter(int &nChoiceIndex, int &nVerticalOffset)
{
    nChoiceIndex    = m_nChoiceIndex;
    nVerticalOffset = m_nVerticalOffset;

    return true;
}

void CDF_VerticalOffset::SetParameter(CDF_VerticalOffset* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_VerticalOffset SetParameter CDF_VerticalOffset point is NULL");
        return;
    }

    pObj->GetParameter(m_nChoiceIndex, m_nVerticalOffset);
}

void CDF_VerticalOffset::toParamString()                             // 将参数值输出到字符串中
{
    LogI("DF_VerticalOffset: ChoiceIndex: %d, VerticalOffset: %d", m_nChoiceIndex, m_nVerticalOffset);
    return;
}

USHORT CDF_VerticalOffset::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeInteger(m_nChoiceIndex, 0, 6) == false )
    {
        LogE("CDF_VerticalOffset encode Choice index failed!");
        return CODEC_FAILED;
    }

    bool bFlag = false;
    switch(m_nChoiceIndex)
    {
    case VERTICAL_OFFSET_B07:
        bFlag = EncodeDE_VertOffset_B07(m_nVerticalOffset);
        break;
    case VERTICAL_OFFSET_B08:
        bFlag = EncodeDE_VertOffset_B08(m_nVerticalOffset);
        break;
    case VERTICAL_OFFSET_B09:
        bFlag = EncodeDE_VertOffset_B09(m_nVerticalOffset);
        break;
    case VERTICAL_OFFSET_B10:
        bFlag = EncodeDE_VertOffset_B10(m_nVerticalOffset);
        break;
    case VERTICAL_OFFSET_B11:
        bFlag = EncodeDE_VertOffset_B11(m_nVerticalOffset);
        break;
    case VERTICAL_OFFSET_B12:
        bFlag = EncodeDE_VertOffset_B12(m_nVerticalOffset);
        break;
    case VERTICAL_OFFSET_ELEVATION:
        bFlag = EncodeDE_Elevation(m_nVerticalOffset);
        break;
    default:
        LogE("CDF_VerticalOffset Encode choice index %d is out of range [0, 7]", m_nChoiceIndex);
        return CODEC_FAILED;
    }

    if ( bFlag == false )
    {
        LogE("CDF_VerticalOffset encode VerticalOffset failed!");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

USHORT CDF_VerticalOffset::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeInteger(m_nChoiceIndex, 0, 6) == false )
    {
        LogE("CDF_VerticalOffset decode Choice index failed!");
        return CODEC_FAILED;
    }

    bool bFlag = false;
    switch(m_nChoiceIndex)
    {
    case VERTICAL_OFFSET_B07:
        bFlag = DecodeDE_VertOffset_B07(m_nVerticalOffset);
        break;
    case VERTICAL_OFFSET_B08:
        bFlag = DecodeDE_VertOffset_B08(m_nVerticalOffset);
        break;
    case VERTICAL_OFFSET_B09:
        bFlag = DecodeDE_VertOffset_B09(m_nVerticalOffset);
        break;
    case VERTICAL_OFFSET_B10:
        bFlag = DecodeDE_VertOffset_B10(m_nVerticalOffset);
        break;
    case VERTICAL_OFFSET_B11:
        bFlag = DecodeDE_VertOffset_B11(m_nVerticalOffset);
        break;
    case VERTICAL_OFFSET_B12:
        bFlag = DecodeDE_VertOffset_B12(m_nVerticalOffset);
        break;
    case VERTICAL_OFFSET_ELEVATION:
        bFlag = DecodeDE_Elevation(m_nVerticalOffset);
        break;
    default:
        LogE("CDF_VerticalOffset Decode choice index %d is out of range [0, 7]", m_nChoiceIndex);
        return CODEC_FAILED;
    }

    if ( bFlag == false )
    {
        LogE("CDF_VerticalOffset decode VerticalOffset failed!");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}


REGISTER_CLASS(CDF_PositionOffsetLLV)

CDF_PositionOffsetLLV::CDF_PositionOffsetLLV()
{ 
    m_nOptionCount = 1;
}

bool CDF_PositionOffsetLLV::SetParameter(CDF_PositionOffsetLL* pPosOffset, CDF_VerticalOffset* pVerticalOffset)
{
    if ( pPosOffset == NULL )
    {
        LogE("CDF_PositionOffsetLLV SetParameter CDF_PositionOffsetLL poit is NULL");
        return false;
    }

    m_OffsetLL.SetObject( pPosOffset->GetObject() );

    // 如果参数为空，则不设置可选参数
    if ( pVerticalOffset == NULL )
        return true;

    m_VerticalOffset.SetParameter(pVerticalOffset);

    SetOptionFlag(0, true);

    return true;
}

bool CDF_PositionOffsetLLV::GetParameter(CDF_PositionOffsetLL* pPosOffset, CDF_VerticalOffset* pVerticalOffset)
{
    if ( pPosOffset == NULL )
    {
        LogE("CDF_PositionOffsetLLV GetParameter CDF_PositionOffsetLL poit is NULL");
        return false;
    }

    pPosOffset->SetParameter(&m_OffsetLL);

    if ( pVerticalOffset == NULL )
        return true;

    pVerticalOffset->SetParameter(&m_VerticalOffset);

    return true;
}

void CDF_PositionOffsetLLV::SetParameter(CDF_PositionOffsetLLV* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_PositionOffsetLLV GetParameter CDF_PositionOffsetLLV poit is NULL");
        return;
    }

    CCSAEMsgBase::SetParameter(pObj);

    if ( GetOptionFlag(0) == true )
        pObj->GetParameter(&m_OffsetLL, &m_VerticalOffset);
    else
        pObj->GetParameter(&m_OffsetLL);

    return;
}

void CDF_PositionOffsetLLV::toParamString()                             // 将参数值输出到字符串中
{
    LogI("DF_PositionOffsetLLV:");
    m_OffsetLL.toParamString();
    m_VerticalOffset.toParamString();

    return;
}

USHORT CDF_PositionOffsetLLV::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    // 含有一个可选项，前导位图就只有1位，0表示可选项不在，1表示可选项存在
    // 在编码前，由应用完成对可选项的设置
    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_PositionOffsetLLV encode option map faild.");
        return CODEC_FAILED;
    }

    if ( EncodeDataFrame( &m_OffsetLL ) == false )
    {
        LogE("CDF_PositionOffsetLLV encode CDF_PositionOffsetLL failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( EncodeDataFrame( &m_VerticalOffset ) == false )
        {
            LogE("CDF_PositionOffsetLLV encode CDF_VerticalOffset failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

USHORT CDF_PositionOffsetLLV::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf      = pData;
    m_nBitLen   = nBitLen;
    m_nBitPos   = nStartBitPos;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_PositionOffsetLLV decode option map failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDataFrame(&m_OffsetLL) == false )
    {
        LogE("CDF_PositionOffsetLLV decode CDF_PositionOffsetLL failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( DecodeDataFrame( &m_VerticalOffset) == false )
        {
            LogE("CDF_PositionOffsetLLV decode CDF_VerticalOffset failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

REGISTER_CLASS(CDF_RoadPoint)
bool CDF_RoadPoint::SetParameter(CDF_PositionOffsetLLV* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_RoadPoint SetParameter CDF_PositionOffsetLLV point is NULL");
        return false;
    }

    m_PositionOffset.SetParameter(pObj);

    return true;
}

bool CDF_RoadPoint::GetParameter(CDF_PositionOffsetLLV* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_RoadPoint GetParameter CDF_PositionOffsetLLV point is NULL");
        return false;
    }

    pObj->SetParameter(&m_PositionOffset);

    return true;
}

void CDF_RoadPoint::toParamString()
{
    LogI("DF_RoadPoint:");
    m_PositionOffset.toParamString();

    return;
}

USHORT CDF_RoadPoint::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeDataFrame(&m_PositionOffset) == false )
    {
        LogE("CDF_RoadPoint encode CDF_PositionOffsetLLV failed.");
        return false;
    }

    return m_nBitPos;
}

USHORT CDF_RoadPoint::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeDataFrame(&m_PositionOffset) == false )
    {
        LogE("CDF_RoadPoint decode CDF_PositionOffsetLLV failed.");
        return false;
    }

    return m_nBitPos;
}

CDF_PointList::CDF_PointList()
{
    m_nMinListLen = MIN_ROAD_POINT_LIST_LEN;
    m_nMaxListLen = MAX_ROAD_POINT_LIST_LEN;

    m_strListObjectClassName = "CDF_RoadPoint"; 
}


CDF_AccelerationSet4Way::CDF_AccelerationSet4Way() 
{
    m_LongAcc   = 0;
    m_LatAcc    = 0;
    m_VertAcc   = -127;
    m_YawRate   = 0;
}

bool CDF_AccelerationSet4Way::SetParameter(DE_Acceleration longAcc, DE_Acceleration latAcc, DE_VerticalAcceleration vertAcc, DE_YawRate yawRate)
{
    if( CHECK_INT_VALID(DE_Acceleration, longAcc ) == false )
    {
        LogE("CDF_AccelerationSet4Way SetParameter longtitude axis value %d is out of range [%d, %d]",
            longAcc, DE_Acceleration_MIN, DE_Acceleration_MAX);

        return false;
    }

    if( CHECK_INT_VALID(DE_Acceleration, latAcc ) == false )
    {
        LogE("CDF_AccelerationSet4Way SetParameter latitude axis value %d is out of range [%d, %d]",
            latAcc, DE_Acceleration_MIN, DE_Acceleration_MAX);

        return false;
    }

    if( CHECK_INT_VALID(DE_VerticalAcceleration, vertAcc ) == false )
    {
        LogE("CDF_AccelerationSet4Way SetParameter DE_VerticalAcceleration value %d is out of range [%d, %d]",
            vertAcc, DE_VerticalAcceleration_MIN, DE_VerticalAcceleration_MAX);

        return false;
    }

    if( CHECK_INT_VALID(DE_YawRate, yawRate ) == false )
    {
        LogE("CDF_AccelerationSet4Way SetParameter DE_YawRate value %d is out of range [%d, %d]",
            yawRate, DE_YawRate_MIN, DE_YawRate_MAX);

        return false;
    }

    m_LongAcc   = longAcc;
    m_LatAcc    = latAcc;
    m_VertAcc   = vertAcc;
    m_YawRate   = yawRate;

    return true;
}

bool CDF_AccelerationSet4Way::GetParameter(DE_Acceleration& longAcc, DE_Acceleration &latAcc, DE_VerticalAcceleration &vertAcc, DE_YawRate &yawRate)
{
    longAcc = m_LongAcc;
    latAcc  = m_LatAcc;
    vertAcc = m_VertAcc;
    yawRate = m_YawRate;

    return true;
}

bool CDF_AccelerationSet4Way::SetParameter(CDF_AccelerationSet4Way* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_PointList SetParameter CDF_PointList point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(m_LongAcc, m_LatAcc, m_VertAcc, m_YawRate);

    return true;
}

void CDF_AccelerationSet4Way::toParamString()
{
    LogI("Long Acc: %d, Lat Acc: %d, Vert Acc: %d, YawRate: %d", m_LongAcc, m_LatAcc, m_VertAcc, m_YawRate);
    return;
}

USHORT CDF_AccelerationSet4Way::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeDE_Acceleration(m_LongAcc) == false )
    {
        LogE("CDF_AccelerationSet4Way encode failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_Acceleration(m_LatAcc) == false )
    {
        LogE("CDF_AccelerationSet4Way encode failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_Acceleration(m_VertAcc) == false )
    {
        LogE("CDF_AccelerationSet4Way encode failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_YawRate(m_YawRate) == false )
    {
        LogE("CDF_AccelerationSet4Way encode failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

USHORT CDF_AccelerationSet4Way::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeDE_Acceleration(m_LongAcc) == false )
    {
        LogE("CDF_AccelerationSet4Way decode failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_Acceleration(m_LatAcc) == false )
    {
        LogE("CDF_AccelerationSet4Way decode failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_Acceleration(m_VertAcc) == false )
    {
        LogE("CDF_AccelerationSet4Way decode failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_YawRate(m_YawRate) == false )
    {
        LogE("CDF_AccelerationSet4Way decode failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

CDF_BrakeSystemStatus::CDF_BrakeSystemStatus() 
{
    m_BrakePadelStatus      = DE_BPS_Unavailable;
    m_BrakeAppliedStatus    = DE_BAS_Unavailable;
    m_TractionControlStatus = DE_TCS_Unavailable;
    m_ABS                   = DE_ABS_Unavailable;
    m_StabilityControlStatus= DE_SCS_Unavailable;
    m_BrakeBoostApplied     = DE_BBA_Unavailable;
    m_AuxBrakesStatus       = DE_AuxBS_Unavailable;

    m_nOptionCount          = 7;
}

bool CDF_BrakeSystemStatus::SetParameter(DE_BrakePedalStatus brakePedalStatus)
{
    if ( CHECK_ENUM_VALID(DE_BrakePedalStatus, brakePedalStatus) == false )
    {
        LogE("CDF_BrakeSystemStatus SetParameter DE_BrakePedalStatus %d is out of range [0, %d]",
            brakePedalStatus, DE_BrakePedalStatus_Invalid);

        return false;
    }

    SetOptionFlag(BRAKE_PEDAL_STATUS, true);
    m_BrakePadelStatus = brakePedalStatus;
    return true;
}

bool CDF_BrakeSystemStatus::SetParameter(UCHAR brakeAppliedStatus)
{
    // DE_BrakeAppliedStatus temp = (DE_BrakeAppliedStatus)brakeAppliedStatus;
    if ( CHECK_ENUM_VALID(DE_BrakeAppliedStatus, brakeAppliedStatus) == false )
    {
        LogE("CDF_BrakeSystemStatus SetParameter DE_BrakeAppliedStatus %d is out of range [0, %d]",
            brakeAppliedStatus, DE_BrakeAppliedStatus_Invalid);

        return false;
    }

    SetOptionFlag(BRAKE_APPLIED_STATUS, true);
    m_BrakeAppliedStatus = brakeAppliedStatus;
    return true;
}

bool CDF_BrakeSystemStatus::SetParameter(DE_TractionControlStatus tractionControlStatus)
{
    if ( CHECK_ENUM_VALID(DE_TractionControlStatus, tractionControlStatus) == false )
    {
        LogE("CDF_BrakeSystemStatus SetParameter DE_TractionControlStatus %d is out of range [0, %d]",
            tractionControlStatus, DE_TractionControlStatus_Invalid);

        return false;
    }

    SetOptionFlag(TRACTION_CONTROL_STATUS, true);
    m_TractionControlStatus = tractionControlStatus;
    return true;
}

bool CDF_BrakeSystemStatus::SetParameter(DE_AntiLockBrakeStatus absStatus)
{
    if ( CHECK_ENUM_VALID(DE_AntiLockBrakeStatus, absStatus) == false )
    {
        LogE("CDF_BrakeSystemStatus SetParameter DE_AntiLockBrakeStatus %d is out of range [0, %d]",
            absStatus, DE_AntiLockBrakeStatus_Invalid);

        return false;
    }

    SetOptionFlag(ANTILOCK_BRAKE_STATUS, true);
    m_ABS = absStatus;
    return true;
}

bool CDF_BrakeSystemStatus::SetParameter(DE_StabilityControlStatus stabilityControlStatus)
{
    if ( CHECK_ENUM_VALID(DE_StabilityControlStatus, stabilityControlStatus) == false )
    {
        LogE("CDF_BrakeSystemStatus SetParameter DE_StabilityControlStatus %d is out of range [0, %d]",
            stabilityControlStatus, DE_StabilityControlStatus_Invalid);

        return false;
    }

    SetOptionFlag(STABILITY_CONTROL_STATUS, true);
    m_StabilityControlStatus = stabilityControlStatus;
    return true;
}

bool CDF_BrakeSystemStatus::SetParameter(DE_BrakeBoostApplied brakeBoostApplied)
{
    if ( CHECK_ENUM_VALID(DE_BrakeBoostApplied, brakeBoostApplied) == false )
    {
        LogE("CDF_BrakeSystemStatus SetParameter DE_BrakeBoostApplied %d is out of range [0, %d]",
            brakeBoostApplied, DE_BrakeBoostApplied_Invalid);

        return false;
    }

    SetOptionFlag(BRAKE_BOOST_APPLIED, true);
    m_BrakeBoostApplied = brakeBoostApplied;
    return true;
}

bool CDF_BrakeSystemStatus::SetParameter(DE_AuxiliaryBrakeStatus auxiliaryBrakeStatus)
{
    if ( CHECK_ENUM_VALID(DE_AuxiliaryBrakeStatus, auxiliaryBrakeStatus) == false )
    {
        LogE("CDF_BrakeSystemStatus SetParameter DE_AuxiliaryBrakeStatus %d is out of range [0, %d]",
            auxiliaryBrakeStatus, DE_AuxiliaryBrakeStatus_Invalid);

        return false;
    }

    SetOptionFlag(AUXILIARY_BRAKE_STATUS, true);
    m_AuxBrakesStatus = auxiliaryBrakeStatus;
    return true;
}

bool CDF_BrakeSystemStatus::GetParameter(DE_BrakePedalStatus &brakePedalStatus)
{
    brakePedalStatus = m_BrakePadelStatus;
    return true;
}

bool CDF_BrakeSystemStatus::GetParameter(UCHAR &brakeAppliedStatus)
{
    brakeAppliedStatus = m_BrakeAppliedStatus;
    return true;
}

bool CDF_BrakeSystemStatus::GetParameter(DE_TractionControlStatus &tractionControlStatus)
{
    tractionControlStatus = m_TractionControlStatus;
    return true;
}

bool CDF_BrakeSystemStatus::GetParameter(DE_AntiLockBrakeStatus &absStatus)
{
    absStatus = m_ABS;
    return true;
}

bool CDF_BrakeSystemStatus::GetParameter(DE_StabilityControlStatus &stabilityControlStatus)
{
    stabilityControlStatus = m_StabilityControlStatus;
    return true;
}

bool CDF_BrakeSystemStatus::GetParameter(DE_BrakeBoostApplied &brakeBoostApplied)
{
    brakeBoostApplied = m_BrakeBoostApplied;
    return true;
}

bool CDF_BrakeSystemStatus::GetParameter(DE_AuxiliaryBrakeStatus &auxiliaryBrakeStatus)
{
    auxiliaryBrakeStatus = m_AuxBrakesStatus;
    return true;
}

bool CDF_BrakeSystemStatus::SetParameter(CDF_BrakeSystemStatus* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_BrakeSystemStatus SetParameter CDF_BrakeSystemStatus point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(m_BrakePadelStatus);
    pObj->GetParameter(m_BrakeAppliedStatus);
    pObj->GetParameter(m_TractionControlStatus);
    pObj->GetParameter(m_ABS);
    pObj->GetParameter(m_StabilityControlStatus);
    pObj->GetParameter(m_BrakeBoostApplied);
    pObj->GetParameter(m_AuxBrakesStatus);
    
    return true;
}

void CDF_BrakeSystemStatus::toParamString()
{
    LogI("DE_BrakePedalStatus: %d, DE_BrakeAppliedStatus: %d, DE_TractionControlStatus: %d, DE_AntiLockBrakeStatus: %d, DE_StabilityControlStatus: %d, DE_BrakeBoostApplied: %d, DE_AuxiliaryBrakeStatus: %d",
        m_BrakePadelStatus, m_BrakePadelStatus, m_TractionControlStatus, m_ABS, m_StabilityControlStatus, m_BrakeBoostApplied, m_AuxBrakesStatus);
    return;
}

USHORT CDF_BrakeSystemStatus::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_BrakeSystemStatus encode option map faild.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( EncodeDE_BrakePedalStatus(m_BrakePadelStatus) == false )
        {
            LogE("CDF_BrakeSystemStatus encode DE_BrakePedalStatus failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(1) == true )
    {
        if ( EncodeDE_BrakeAppliedStatus(m_BrakeAppliedStatus) == false )
        {
            LogE("CDF_BrakeSystemStatus encode DE_BrakeAppliedStatus failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(2) == true )
    {
        if ( EncodeDE_TractionControlStatus(m_TractionControlStatus) == false )
        {
            LogE("CDF_BrakeSystemStatus encode DE_TractionControlStatus failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(3) == true )
    {
        if ( EncodeDE_AntiLockBrakeStatus(m_ABS) == false )
        {
            LogE("CDF_BrakeSystemStatus encode DE_AntiLockBrakeStatus failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(4) == true )
    {
        if ( EncodeDE_StabilityControlStatus(m_StabilityControlStatus) == false )
        {
            LogE("CDF_BrakeSystemStatus encode DE_StabilityControlStatus failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(5) == true )
    {
        if ( EncodeDE_BrakeBoostApplied(m_BrakeBoostApplied) == false )
        {
            LogE("CDF_BrakeSystemStatus encode DE_BrakeBoostApplied failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(6) == true )
    {
        if ( EncodeDE_AuxiliaryBrakeStatus(m_AuxBrakesStatus) == false )
        {
            LogE("CDF_BrakeSystemStatus encode DE_AuxiliaryBrakeStatus failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

USHORT CDF_BrakeSystemStatus::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;


    // 在有多个可选参数的时候，前导位图为定长比特串
    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_MotionConfidenceSet decode option map failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( DecodeDE_BrakePedalStatus(m_BrakePadelStatus) == false )
        {
            LogE("CDF_BrakeSystemStatus decode DE_BrakePedalStatus failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(1) == true )
    {
        if ( DecodeDE_BrakeAppliedStatus(m_BrakeAppliedStatus) == false )
        {
            LogE("CDF_BrakeSystemStatus decode DE_BrakeAppliedStatus failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(2) == true )
    {
        if ( DecodeDE_TractionControlStatus(m_TractionControlStatus) == false )
        {
            LogE("CDF_BrakeSystemStatus decode DE_TractionControlStatus failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(3) == true )
    {
        if ( DecodeDE_AntiLockBrakeStatus(m_ABS) == false )
        {
            LogE("CDF_BrakeSystemStatus decode DE_AntiLockBrakeStatus failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(4) == true )
    {
        if ( DecodeDE_StabilityControlStatus(m_StabilityControlStatus) == false )
        {
            LogE("CDF_BrakeSystemStatus decode DE_StabilityControlStatus failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(5) == true )
    {
        if ( DecodeDE_BrakeBoostApplied(m_BrakeBoostApplied) == false )
        {
            LogE("CDF_BrakeSystemStatus decode DE_BrakeBoostApplied failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(6) == true )
    {
        if ( DecodeDE_AuxiliaryBrakeStatus(m_AuxBrakesStatus) == false )
        {
            LogE("CDF_BrakeSystemStatus decode DE_AuxiliaryBrakeStatus failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

CDF_ConnectingLane::CDF_ConnectingLane() 
{
    m_LaneID            = 0;
    m_AllowedManeuvers  = DE_AM_StraightAllowed;

    m_nOptionCount      = 1;
}

bool CDF_ConnectingLane::SetParameter(DE_LaneID laneID, USHORT nAllowedManeuvers)
{
    if ( CHECK_INT_VALID(DE_LaneID, laneID) == false )
    {
        LogE("CDF_ConnectingLane SetParameter DE_LaneID %d is out of range [%d, %d]",
            laneID, DE_LaneID_MIN, DE_LaneID_MAX);
        return false;
    }

    m_LaneID = laneID;

    if ( CHECK_ENUM_VALID(DE_AllowedManeuvers, nAllowedManeuvers ) == false )
        return true;

    SetOptionFlag(0, true);
    m_AllowedManeuvers = nAllowedManeuvers;

    return true;
}

bool CDF_ConnectingLane::GetParameter(DE_LaneID &laneID, USHORT &nAllowedManeuvers)
{
    laneID = m_LaneID;

    if ( GetOptionFlag(0) == true )
    {
        nAllowedManeuvers = m_AllowedManeuvers;
    }

    return true;
}

bool CDF_ConnectingLane::SetParameter(CDF_ConnectingLane* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_ConnectingLane SetParameter CDF_ConnectingLane point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(m_LaneID, m_AllowedManeuvers);

    return true;
}

void CDF_ConnectingLane::toParamString()
{
    LogI("DF_ConnectingLane: Lane ID: %d, Allowed Maneuvers: %d", m_LaneID, m_AllowedManeuvers);
    return;
}

USHORT CDF_ConnectingLane::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    // 含有一个可选项，前导位图就只有1位，0表示可选项不在，1表示可选项存在
    // 在编码前，由应用完成对可选项的设置
    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_ConnectingLane encode option map faild.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_LaneID(m_LaneID) == false )
    {
        LogE("CDF_ConnectingLane encode LaneID failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( EncodeDE_AllowedManeuvers(m_AllowedManeuvers) == false )
        {
            LogE("CDF_ConnectingLane encode DE_AllowdManeuvers failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

USHORT CDF_ConnectingLane::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_ConnectingLane decode option map failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_LaneID(m_LaneID) == false )
    {
        LogE("CDF_ConnectingLane decode LaneID failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( DecodeDE_AllowedManeuvers(m_AllowedManeuvers) == false )
        {
            LogE("CDF_ConnectingLane decode DE_AllowdManeuvers failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}


REGISTER_CLASS(CDF_Connection)
CDF_Connection::CDF_Connection()
{ 
    m_PhaseID       = 0; 
    m_nOptionCount  = 2; 
}

bool CDF_Connection::SetParameter(CDF_NodeReferenceID* pNodeID, CDF_ConnectingLane* pLane, DE_PhaseID phaseID)
{
    if ( pNodeID == NULL )
    {
        LogE("CDF_Connection SetParameter CDF_NodeReferenceID point is NULL");
        return false;
    }

    m_NodeReferenceID.SetParameter(pNodeID);

    if ( pLane != NULL )
    {
        SetOptionFlag(0, true);
        m_ConnectingLane.SetParameter(pLane);
    }

    if ( CHECK_INT_VALID(DE_PhaseID, phaseID) == true )
    {
        SetOptionFlag(1, true);
        m_PhaseID = phaseID;
    }

    return true;
}

bool CDF_Connection::GetParameter(CDF_NodeReferenceID* pNodeID, CDF_ConnectingLane* pLane, DE_PhaseID phaseID)
{
    if ( pNodeID == NULL )
    {
        LogE("CDF_Connection GetParameter CDF_NodeReferenceID point is NULL");
        return false;
    }

    pNodeID->SetParameter(&m_NodeReferenceID);

    if ( pLane != NULL && GetOptionFlag(0) == true )
        pLane->SetParameter(&m_ConnectingLane);

    if ( GetOptionFlag(1) == true )
        phaseID = m_PhaseID;

    return true;
}

bool CDF_Connection::SetParameter(CDF_Connection* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_Connection SetParameter CDF_Connection point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(&m_NodeReferenceID, &m_ConnectingLane, m_PhaseID);

    return true;

}

void CDF_Connection::toParamString()
{
    LogI("DF_Connection: DE_PhaseID: %d"); 
    m_NodeReferenceID.toParamString();
    m_ConnectingLane.toParamString();
    return;
}

USHORT CDF_Connection::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_Connection encode option map failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDataFrame(&m_NodeReferenceID) == false )
    {
        LogE("CDF_Connection encode CDF_NodeReferenceID failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;
    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDataFrame(&m_ConnectingLane) == false )
        {
            LogE("CDF_Connection encode CDF_ConnectingLane failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_PhaseID(m_PhaseID) == false )
        {
            LogE("CDF_Connection encode DE_PhaseID failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

USHORT CDF_Connection::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_Connection decode option map failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDataFrame(&m_NodeReferenceID) == false )
    {
        LogE("CDF_Connection decode CDF_NodeReferenceID failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;
    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDataFrame(&m_ConnectingLane) == false )
        {
            LogE("CDF_Connection decode CDF_ConnectingLane failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_PhaseID(m_PhaseID) == false )
        {
            LogE("CDF_Connection decode DE_PhaseID failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}


CDF_ConnectsToList::CDF_ConnectsToList()
{ 
    m_nMinListLen = MIN_CONNECTION_LIST_LEN;
    m_nMaxListLen = MAX_CONNECTION_LIST_LEN;

    m_strListObjectClassName = "CDF_Connection"; 
}


CDF_DDateTime::CDF_DDateTime() 
{
    m_Year          = 0;
    m_Month         = 0;
    m_Day           = 0;
    m_Hour          = 24;
    m_Minute        = 60;
    m_Second        = 60000;
    m_TimeOffset    = DEFAULT_TIME_OFFSET;                          // 默认系统是在东八区

    m_nOptionCount  = 7;
}

bool CDF_DDateTime::SetParameter(DE_DYear nYear, DE_DMonth nMonth, DE_DDay nDay, DE_DHour nHour, DE_DMinute nMinute, DE_DSecond nSecond, DE_DTimeOffset nTimeOffset)
{
    if ( CHECK_INT_VALID(DE_DYear, nYear) == true )
    {
        SetOptionFlag(DATETIME_YEAR, true);
        m_Year = nYear;
    }

    if ( CHECK_INT_VALID(DE_DMonth, nMonth) == true )
    {
        SetOptionFlag(DATETIME_MONTH, true);
        m_Month = nMonth;
    }

    if ( CHECK_INT_VALID(DE_DDay, nDay) == true )
    {
        SetOptionFlag(DATETIME_DAY, true);
        m_Day = nDay;
    }

    if ( CHECK_INT_VALID(DE_DHour, nHour) == true )
    {
        SetOptionFlag(DATETIME_HOUR, true);
        m_Hour = nHour;
    }

    if ( CHECK_INT_VALID(DE_DMinute, nMinute) == true )
    {
        SetOptionFlag(DATETIME_MINUTE, true);
        m_Minute = nMinute;
    }

    if ( CHECK_INT_VALID(DE_DSecond, nSecond) == true )
    {
        SetOptionFlag(DATETIME_SECOND, true);
        m_Second = nSecond;
    }

    if ( CHECK_INT_VALID(DE_DTimeOffset, nTimeOffset) == true )
    {
        SetOptionFlag(DATETIME_TIME_OFFSET, true);
        m_TimeOffset = nTimeOffset;
    }

    return true;
}

bool CDF_DDateTime::GetParameter(DE_DYear &nYear, DE_DMonth &nMonth, DE_DDay &nDay, DE_DHour &nHour, DE_DMinute &nMinute, DE_DSecond &nSecond, DE_DTimeOffset &nTimeOffset)
{
    nYear       = m_Year;
    nMonth      = m_Month;
    nDay        = m_Day;
    nHour       = m_Hour;
    nMinute     = m_Minute;
    nSecond     = m_Second;
    nTimeOffset = m_TimeOffset;

    return true;
}

bool CDF_DDateTime::SetParameter(CDF_DDateTime* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_DDateTime SetParameter CDF_DDateTime point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(m_Year, m_Month, m_Day, m_Hour, m_Minute, m_Second, m_TimeOffset);

    return true;
}

void CDF_DDateTime::toParamString()                             // 将参数值输出到字符串中
{
    // LogI("Year: %d, Month: %d, Day: %d, Hour: %d, Minute: %d, Second: %d, TimeOffset: %d",
    //     m_Year, m_Month, m_Day, m_Hour, m_Minute, m_Second, m_TimeOffset);

    LogI("DF_DateTime: %04d-%02d-%02d %02d:%02d:%02d %d",
        m_Year, m_Month, m_Day, m_Hour, m_Minute, m_Second, m_TimeOffset);

    return;
}

USHORT CDF_DDateTime::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_DDateTime encode option flag failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_DYear(m_Year) == false )
        {
            LogE("CDF_DDateTime encode DE_DYear failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_DMonth(m_Month) == false )
        {
            LogE("CDF_DDateTime encode DE_DMonth failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_DDay(m_Day) == false )
        {
            LogE("CDF_DDateTime encode DE_DDay failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_DHour(m_Hour) == false )
        {
            LogE("CDF_DDateTime encode DE_DHour failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_DMinute(m_Minute) == false )
        {
            LogE("CDF_DDateTime encode DE_DMinute failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_DSecond(m_Second) == false )
        {
            LogE("CDF_DDateTime encode DE_DSecond failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_DTimeOffset(m_TimeOffset) == false )
        {
            LogE("CDF_DDateTime encode DE_DTimeOffset failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

USHORT CDF_DDateTime::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_DDateTime decode option flag failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_DYear(m_Year) == false )
        {
            LogE("CDF_DDateTime decode DE_DYear failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_DMonth(m_Month) == false )
        {
            LogE("CDF_DDateTime decode DE_DMonth failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_DDay(m_Day) == false )
        {
            LogE("CDF_DDateTime decode DE_DDay failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_DHour(m_Hour) == false )
        {
            LogE("CDF_DDateTime decode DE_DHour failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_DMinute(m_Minute) == false )
        {
            LogE("CDF_DDateTime decode DE_DMinute failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_DSecond(m_Second) == false )
        {
            LogE("CDF_DDateTime decode DE_DSecond failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_DTimeOffset(m_TimeOffset) == false )
        {
            LogE("CDF_DDateTime decode DE_DTimeOffset failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

CDF_FullPositionVector::CDF_FullPositionVector() 
{
    m_Heading           = 0;
    m_Speed             = 0;
    m_TransissionState  = DE_TS_Neutral;
    m_TimeConfidence    = DE_TC_Unknown;

    m_nOptionCount      = 8;
}

bool CDF_FullPositionVector::SetParameter(CDF_DDateTime* pDateTime, CDF_Position3D* pPos3D, DE_Heading heading, DE_TransmissionState transState, DE_Speed speed, CDF_PositionConfidenceSet* pPosAccuracy, DE_TimeConfidence timeCfd, CDF_PositionConfidenceSet* pPosCfdSet, CDF_MotionConfidenceSet* pMotionCfdSet)
{
    if ( pDateTime != NULL )
    {
        SetOptionFlag(FPV_DATETIME, true);
        m_DateTime.SetParameter(pDateTime);
    }

    if ( pPos3D != NULL )
    {
        SetOptionFlag(FPV_POSTION3D, true);
        m_Position3D.SetParameter(pPos3D);
    }

    if ( CHECK_INT_VALID(DE_Heading, heading) == true )
    {
        SetOptionFlag(FPV_HEADING, true);
        m_Heading = heading;
    }

    if ( CHECK_ENUM_VALID(DE_TransmissionState, transState) == true )
    {
        SetOptionFlag(FPV_TRANS_STATE, true);
        m_TransissionState = transState;
    }

    if ( CHECK_INT_VALID(DE_Speed, speed) == true )
    {
        SetOptionFlag(FPV_SPEED, true);
        m_Speed = speed;
    }

    if ( pPosAccuracy != NULL )
    {
        SetOptionFlag(FPV_POS_ACCURACY, true);
        m_PositionAccuracy.SetParameter(pPosAccuracy);
    }

    if ( CHECK_ENUM_VALID(DE_TimeConfidence, timeCfd) == true )
    {
        SetOptionFlag(FPV_TIME_CFD, true);
        m_TimeConfidence = timeCfd;
    }

    if ( pPosCfdSet != NULL )
    {
        SetOptionFlag(FPV_POS_CFD_SET, true);
        m_PositionConfidenceSet.SetParameter(pPosCfdSet);
    }

    if ( pMotionCfdSet != NULL )
    {
        SetOptionFlag(FPV_MOTION_CFD_SET, true);
        m_MotionCfdSet.SetParameter(pMotionCfdSet);
    }

    return true;
}

bool CDF_FullPositionVector::GetParameter(CDF_DDateTime* pDateTime, CDF_Position3D* pPos3D, DE_Heading &heading, DE_TransmissionState &transState, DE_Speed &speed, CDF_PositionConfidenceSet* pPosAccuracy, DE_TimeConfidence &timeCfd, CDF_PositionConfidenceSet* pPosCfdSet, CDF_MotionConfidenceSet* pMotionCfdSet)
{
    if ( pDateTime != NULL )
        pDateTime->SetParameter(&m_DateTime);

    if ( pPos3D!= NULL )
        pPos3D->SetParameter(&m_Position3D);

    heading = m_Heading;
    transState = m_TransissionState;
    speed = m_Speed;

    if ( pPosAccuracy != NULL )
        pPosAccuracy->SetParameter(&m_PositionAccuracy);

    timeCfd = m_TimeConfidence;

    if ( pPosCfdSet != NULL )
        pPosCfdSet->SetParameter(&m_PositionConfidenceSet);

    if ( pMotionCfdSet != NULL )
        pMotionCfdSet->SetParameter(&m_MotionCfdSet);

    return true;
}

bool CDF_FullPositionVector::SetParameter(CDF_FullPositionVector* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_FullPositionVector SetParameter CDF_FullPositionVector point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(&m_DateTime, &m_Position3D, m_Heading, m_TransissionState, m_Speed, &m_PositionAccuracy, m_TimeConfidence, &m_PositionConfidenceSet, &m_MotionCfdSet);

    return true;
}

void CDF_FullPositionVector::toParamString()                             // 将参数值输出到字符串中
{
    LogI("DF_FullPositionVector: Heading: %d, transState: %d, Speed: %d, timeCfd: %d", m_Heading, m_TransissionState, m_Speed, m_TimeConfidence);
    m_DateTime.toParamString();
    m_Position3D.toParamString();
    m_PositionAccuracy.toParamString();
    m_PositionConfidenceSet.toParamString();
    m_MotionCfdSet.toParamString();
    return;
}

USHORT CDF_FullPositionVector::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_FullPositionVector encode option flag failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDataFrame(&m_DateTime) == false )
        {
            LogE("CDF_FullPositionVector encode CDF_DDateTime failed.");
            return CODEC_FAILED;
        }
    }

    if ( EncodeDataFrame(&m_Position3D) == false )
    {
        LogE("CDF_FullPositionVector encode CDF_Position3D failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_Heading(m_Heading) == false )
        {
            LogE("CDF_FullPositionVector encode DE_Heading failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_TransmissionState(m_TransissionState) == false )
        {
            LogE("CDF_FullPositionVector encode DE_TransmissionState failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_Speed(m_Speed) == false )
        {
            LogE("CDF_FullPositionVector encode DE_Speed failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDataFrame(&m_PositionAccuracy) == false )
        {
            LogE("CDF_FullPositionVector encode CDF_PositionConfidenceSet failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_TimeConfidence(m_TimeConfidence) == false )
        {
            LogE("CDF_FullPositionVector encode DE_TimeConfidence failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDataFrame(&m_PositionConfidenceSet) == false )
        {
            LogE("CDF_FullPositionVector encode CDF_PositionConfidenceSet failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDataFrame(&m_MotionCfdSet) == false )
        {
            LogE("CDF_FullPositionVector encode CDF_MotionConfidenceSet failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

USHORT CDF_FullPositionVector::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_FullPositionVector decode option flag failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDataFrame(&m_DateTime) == false )
        {
            LogE("CDF_FullPositionVector decode CDF_DDateTime failed.");
            return CODEC_FAILED;
        }
    }

    if ( DecodeDataFrame(&m_Position3D) == false )
    {
        LogE("CDF_FullPositionVector decode CDF_Position3D failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_Heading(m_Heading) == false )
        {
            LogE("CDF_FullPositionVector decode DE_Heading failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_TransmissionState(m_TransissionState) == false )
        {
            LogE("CDF_FullPositionVector decode DE_TransmissionState failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_Speed(m_Speed) == false )
        {
            LogE("CDF_FullPositionVector decode DE_Speed failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDataFrame(&m_PositionAccuracy) == false )
        {
            LogE("CDF_FullPositionVector decode CDF_PositionConfidenceSet failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_TimeConfidence(m_TimeConfidence) == false )
        {
            LogE("CDF_FullPositionVector decode DE_TimeConfidence failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDataFrame(&m_PositionConfidenceSet) == false )
        {
            LogE("CDF_FullPositionVector decode CDF_PositionConfidenceSet failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDataFrame(&m_MotionCfdSet) == false )
        {
            LogE("CDF_FullPositionVector decode CDF_MotionConfidenceSet failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}


REGISTER_CLASS(CDF_IntersectionState)
CDF_IntersectionState::CDF_IntersectionState() 
{ 
    m_TimeStamp                 = 60000; 
    m_IntersectionStatusObject  = DE_ISO_ManualControlIsEnabled;
    m_MinuteOfTheYear           = 0;

    m_nOptionCount              = 2;
}

bool CDF_IntersectionState::SetParameter(CDF_NodeReferenceID* pNodeID, USHORT statusObject, CDF_PhaseList* pPhaseList, DE_MinuteOfTheYear nMinuteOfYear, DE_DSecond timeStamp)
{
    if ( pNodeID == NULL )
    {
        LogE("CDF_IntersectionState SetParameter CDF_NodeReferenceID point is NULL");
        return false;
    }

    if ( CHECK_ENUM_VALID(DE_IntersectionStatusObject, statusObject ) == false )
    {
        LogE("CDF_IntersectionState SetParameter DE_IntersectionStatusObject %d is out of range [0, %d]", statusObject, DE_IntersectionStatusObject_Invalid);
        return false;
    }

    if ( pPhaseList == NULL )
    {
        LogE("CDF_IntersectionState SetParameter CDF_PhaseList is NULL point");
        return false;
    }

    m_InterSectionID.SetParameter(pNodeID);
    m_IntersectionStatusObject = statusObject;
    m_PhaseList.SetParameter(pPhaseList);

    // 检查可选参数情况
    if ( CHECK_INT_VALID(DE_MinuteOfTheYear, nMinuteOfYear) == true )
    {
        SetOptionFlag(0, true);
        m_MinuteOfTheYear = nMinuteOfYear;
    }

    if ( CHECK_INT_VALID(DE_DSecond, timeStamp) == true )
    {
        SetOptionFlag(1, true);
        m_TimeStamp = timeStamp;
    }

    return true;
}

bool CDF_IntersectionState::GetParameter(CDF_NodeReferenceID* pNodeID, USHORT &statusObject, CDF_PhaseList* pPhaseList, DE_MinuteOfTheYear &nMinuteOfYear, DE_DSecond &timeStamp)
{
    if ( pNodeID == NULL )
    {
        LogE("CDF_IntersectionState GetParameter CDF_NodeReferenceID point is NULL");
        return false;
    }

    if ( pPhaseList == NULL )
    {
        LogE("CDF_IntersectionState GetParameter CDF_PhaseList is NULL point");
        return false;
    }

    pNodeID->SetParameter(&m_InterSectionID);
    pPhaseList->SetParameter(&m_PhaseList);
    statusObject = m_IntersectionStatusObject;

    nMinuteOfYear = m_MinuteOfTheYear;
    timeStamp = m_TimeStamp;

    return true;
}

bool CDF_IntersectionState::SetParameter(CDF_IntersectionState* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_IntersectionState SetParameter CDF_IntersectionState point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(&m_InterSectionID, m_IntersectionStatusObject, &m_PhaseList, m_MinuteOfTheYear, m_TimeStamp);

    return true;
}

void CDF_IntersectionState::toParamString()                             // 将参数值输出到字符串中
{
    LogI("DF_IntersectionState: StatusObject: %d, MinuteOfYear: %d, timeStamp: %d", m_IntersectionStatusObject, m_MinuteOfTheYear, m_TimeStamp);
    m_InterSectionID.toParamString();
    m_PhaseList.toParamString();
    return;
}

USHORT CDF_IntersectionState::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_IntersectionState encode option flag failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;

    if ( EncodeDataFrame(&m_InterSectionID) == false )
    {
        LogE("CDF_IntersectionState encode CDF_NodeReferenceID failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_IntersectionStatusObject(m_IntersectionStatusObject) == false )
    {
        LogE("CDF_IntersectionState encode DE_IntersectionStatusObject failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_MinuteOfTheYear(m_MinuteOfTheYear) == false )
        {
            LogE("CDF_IntersectionState encode DE_MinuteOfTheYear failed.");
            return CODEC_FAILED;
        }        
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_DSecond(m_TimeStamp) == false )
        {
            LogE("CDF_IntersectionState encode DE_DSecond failed.");
            return CODEC_FAILED;
        }        
    }

    if ( EncodeDataFrame(&m_PhaseList) == false )
    {
        LogE("CDF_IntersectionState encode CDF_PhaseList failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

USHORT CDF_IntersectionState::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_IntersectionState decode option flag failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;

    if ( DecodeDataFrame(&m_InterSectionID) == false )
    {
        LogE("CDF_IntersectionState decode CDF_NodeReferenceID failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_IntersectionStatusObject(m_IntersectionStatusObject) == false )
    {
        LogE("CDF_IntersectionState decode DE_IntersectionStatusObject failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_MinuteOfTheYear(m_MinuteOfTheYear) == false )
        {
            LogE("CDF_IntersectionState decode DE_MinuteOfTheYear failed.");
            return CODEC_FAILED;
        }        
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_DSecond(m_TimeStamp) == false )
        {
            LogE("CDF_IntersectionState decode DE_DSecond failed.");
            return CODEC_FAILED;
        }        
    }

    if ( DecodeDataFrame(&m_PhaseList) == false )
    {
        LogE("CDF_IntersectionState decode CDF_PhaseList failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}


CDF_IntersectionStateList::CDF_IntersectionStateList()
{
    m_nMinListLen = MIN_INTERSECTION_STATUS_LIST_LEN;
    m_nMaxListLen = MAX_INTERSECTION_STATUS_LIST_LEN;

    m_strListObjectClassName = "CDF_IntersectionState";
}


CDF_LaneTypeAttributes::CDF_LaneTypeAttributes() 
{
    m_nChoiceIndex      = -1;
    m_LaneAttributes    = 0;
    m_nAttrBitLen       = 0;
}

bool CDF_LaneTypeAttributes::GetParameter(int &nChoiceIndex, UINT& attr, USHORT &nAttrBitLen)
{
    nChoiceIndex    = m_nChoiceIndex;
    attr            = m_LaneAttributes;
    nAttrBitLen     = m_nAttrBitLen;
    return true;
}

bool CDF_LaneTypeAttributes::SetParameter(int nChoiceIndex, UINT nLaneAttributes, USHORT nBitLen)
{
    if (nChoiceIndex < 0 || nChoiceIndex > 7 )
    {
        LogE("CDF_LaneTypeAttributes SetLaneAttrbutes Choice Index %d is out of range [0, 7]. ", 
            nChoiceIndex);

        return false;
    }

    m_nChoiceIndex      = nChoiceIndex;

    // 取值为比特串，不验证参数是否合法
    m_LaneAttributes    = nLaneAttributes;

    // Vehicle的属性是变长比特串
    m_nAttrBitLen = (nChoiceIndex == 0) ? nBitLen : 16;

    return true;
}

bool CDF_LaneTypeAttributes::SetParameter(CDF_LaneTypeAttributes* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_LaneTypeAttributes SetParameter CDF_LaneTypeAttributes point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(m_nChoiceIndex, m_LaneAttributes, m_nAttrBitLen);

    return true;
}

void CDF_LaneTypeAttributes::toParamString()
{
    LogI("DF_LaneTypeAttributes: ChoiceIndex: %d, LaneAttr: %d", m_nChoiceIndex, m_LaneAttributes);
    return;
}

USHORT CDF_LaneTypeAttributes::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeInteger(m_nChoiceIndex, 0, 7) == false )
    {
        LogE("CDF_LaneTypeAttributes encode Choice index failed!");
        return CODEC_FAILED;
    }

    bool bFlag = false;
    switch(m_nChoiceIndex)
    {
    case 0:
        bFlag = EncodeDE_LaneAttr_Vehicle(m_LaneAttributes, m_nAttrBitLen);
        break;
    case 1:
        bFlag = EncodeDE_LaneAttr_Crosswalk(m_LaneAttributes);
        break;
    case 2:
        bFlag = EncodeDE_LaneAttr_Bike(m_LaneAttributes);
        break;
    case 3:
        bFlag = EncodeDE_LaneAttr_Sidewalk(m_LaneAttributes);
        break;
    case 4:
        bFlag = EncodeDE_LaneAttr_Barrier(m_LaneAttributes);
        break;
    case 5:
        bFlag = EncodeDE_LaneAttr_Striping(m_LaneAttributes);
        break;
    case 6:
        bFlag = EncodeDE_LaneAttr_TrackedVehicle(m_LaneAttributes);
        break;
    case 7:
        bFlag = EncodeDE_LaneAttr_Parking(m_LaneAttributes);
        break;
    default:
        bFlag = false;
        break;
    }

    if ( bFlag == false )
    {
        LogE("CDF_LaneTypeAttributes encode LaneAttrbutes failed!");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

USHORT CDF_LaneTypeAttributes::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeInteger(m_nChoiceIndex, 0, 7) == false )
    {
        LogE("CDF_LaneTypeAttributes decode choice index failed!");
        return CODEC_FAILED;
    }

    bool bFlag = false;
    switch(m_nChoiceIndex)
    {
    case 0:
        bFlag = DecodeDE_LaneAttr_Vehicle(m_LaneAttributes);
        break;
    case 1:
        bFlag = DecodeDE_LaneAttr_Crosswalk(m_LaneAttributes);
        break;
    case 2:
        bFlag = DecodeDE_LaneAttr_Bike(m_LaneAttributes);
        break;
    case 3:
        bFlag = DecodeDE_LaneAttr_Sidewalk(m_LaneAttributes);
        break;
    case 4:
        bFlag = DecodeDE_LaneAttr_Barrier(m_LaneAttributes);
        break;
    case 5:
        bFlag = DecodeDE_LaneAttr_Striping(m_LaneAttributes);
        break;
    case 6:
        bFlag = DecodeDE_LaneAttr_TrackedVehicle(m_LaneAttributes);
        break;
    case 7:
        bFlag = DecodeDE_LaneAttr_Parking(m_LaneAttributes);
        break;
    default:
        bFlag = false;
        break;
    }

    if ( bFlag == false )
    {
        LogE("CDF_LaneTypeAttributes decode LaneAttrbutes failed!");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

CDF_LaneAttributes::CDF_LaneAttributes()
{ 
    m_ShareWith = DE_LaneSharing_Invalid;

    m_nOptionCount = 1;
}

bool CDF_LaneAttributes::SetParameter(CDF_LaneTypeAttributes* pLaneTypeAttr, USHORT laneSharing )
{
    if ( pLaneTypeAttr == NULL )
    {
        LogE("CDF_LaneAttributes SetParameter CDF_LaneTypeAttributes is NULL point.");
        return false;
    }

    m_LaneTypeAttr.SetParameter(pLaneTypeAttr);

    if ( CHECK_ENUM_VALID(DE_LaneSharing, laneSharing) == true )
    {
        SetOptionFlag(0, true);
        m_ShareWith = laneSharing;
    }

    return true;
}

bool CDF_LaneAttributes::GetParameter(CDF_LaneTypeAttributes* pLaneTypeAttr, USHORT& laneSharing)
{
    if ( pLaneTypeAttr == NULL )
    {
        LogE("CDF_LaneAttributes GetParameter CDF_LaneTypeAttributes is NULL point.");
        return false;
    }

    pLaneTypeAttr->SetParameter(&m_LaneTypeAttr);
    laneSharing = m_ShareWith;

    return true;
}

bool CDF_LaneAttributes::SetParameter(CDF_LaneAttributes* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_LaneTypeAttributes SetParameter CDF_LaneTypeAttributes point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(&m_LaneTypeAttr, m_ShareWith);

    return true;
}

void CDF_LaneAttributes::toParamString()                             // 将参数值输出到字符串中
{
    LogI("DF_LaneAttributes: ShareWith: %d", m_ShareWith);
    m_LaneTypeAttr.toParamString();
    return;
}

USHORT CDF_LaneAttributes::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_IntersectionState encode option flag failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;
    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_LaneSharing(m_ShareWith) == false )
        {
            LogE("CDF_LaneAttributes encode DE_LaneSharing failed.");
            return CODEC_FAILED;
        }
    }

    if ( EncodeDataFrame(&m_LaneTypeAttr) == false )
    {
        LogE("CDF_LaneAttributes encode CDF_LaneTypeAttributes failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

USHORT CDF_LaneAttributes::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_IntersectionState decode option flag failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;
    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_LaneSharing(m_ShareWith) == false )
        {
            LogE("CDF_LaneAttributes decode DE_LaneSharing failed.");
            return CODEC_FAILED;
        }
    }

    if ( DecodeDataFrame(&m_LaneTypeAttr) == false )
    {
        LogE("CDF_LaneAttributes decode CDF_LaneTypeAttributes failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}


REGISTER_CLASS(CDF_Lane)
CDF_Lane::CDF_Lane() 
{
    m_LaneID            = 0;
    m_AllowedManeuvers  = DE_AM_StraightAllowed;

    m_nOptionCount      = 5;
}

bool CDF_Lane::SetParameter(DE_LaneID laneID, USHORT nAllowedManeuvers, CDF_LaneAttributes* pLaneAttr, CDF_ConnectsToList* pConnToList, CDF_SpeedLimitList* pSpeedLimitList, CDF_PointList* pPointList)
{
    if ( CHECK_INT_VALID(DE_LaneID, laneID) == false )
    {
        LogE("CDF_Lane SetParameter laneID %d is out of range [%d, %d]", 
            laneID, DE_LaneID_MIN, DE_LaneID_MAX);
        return false;
    }

    m_LaneID = laneID;

    if ( pLaneAttr != NULL )
    {
        SetOptionFlag(LANE_LANE_ATTR, true);
        m_LaneAttributes.SetParameter(pLaneAttr);
    }

    if ( CHECK_ENUM_VALID(DE_AllowedManeuvers, nAllowedManeuvers ) == true )
    {
        SetOptionFlag(LANE_MANEUVERS, true);
        m_AllowedManeuvers = nAllowedManeuvers;
    }

    if ( pConnToList != NULL )
    {
        SetOptionFlag(LANE_CONNECTS_TOLIST, true);
        m_ConnectsToList.SetParameter(pConnToList);
    }

    if ( pSpeedLimitList != NULL )
    {
        SetOptionFlag(LANE_SPEED_LIMIT_LIST, true);
        m_SpeedLimitList.SetParameter(pSpeedLimitList);
    }

    if ( pPointList != NULL )
    {
        SetOptionFlag(LANE_POINT_LIST, true);
        m_PointList.SetParameter(pPointList);
    }

    return true;
}

bool CDF_Lane::GetParameter(DE_LaneID &laneID, USHORT& nAllowedManeuvers, CDF_LaneAttributes* pLaneAttr, CDF_ConnectsToList* pConnToList, CDF_SpeedLimitList* pSpeedLimitList, CDF_PointList* pPointList)
{
    laneID = m_LaneID;

    if ( pLaneAttr != NULL )
        pLaneAttr->SetParameter(&m_LaneAttributes);

    nAllowedManeuvers = m_AllowedManeuvers;

    if ( pConnToList != NULL )
        pConnToList->SetParameter(&m_ConnectsToList);

    if ( pSpeedLimitList != NULL )
        pSpeedLimitList->SetParameter(&m_SpeedLimitList);

    if ( pPointList != NULL )
        pPointList->SetParameter(&m_PointList);

    return true;
}

bool CDF_Lane::SetParameter(CDF_Lane* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_Lane SetParameter CDF_Lane point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(m_LaneID, m_AllowedManeuvers, &m_LaneAttributes, &m_ConnectsToList, &m_SpeedLimitList, &m_PointList);

    return true;
}

void CDF_Lane::toParamString()                             // 将参数值输出到字符串中
{
    LogI("DF_Lane: LaneID: %d, Maneuvers: %d", m_LaneID, m_AllowedManeuvers); 
    m_LaneAttributes.toParamString();
    m_ConnectsToList.toParamString(),
    m_SpeedLimitList.toParamString();
    m_PointList.toParamString();
    return;
}

USHORT CDF_Lane::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_Lane encode option flag failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;

    if ( EncodeDE_LaneID(m_LaneID) == false )
    {
        LogE("CDF_Lane encode DE_LaneID failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDataFrame(&m_LaneAttributes) == false )
        {
            LogE("CDF_Lane encode CDF_LaneAttributes failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_AllowedManeuvers(m_AllowedManeuvers) == false )
        {
            LogE("CDF_Lane encode DE_AllowdManeuvers failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDataFrame(&m_ConnectsToList) == false )
        {
            LogE("CDF_Lane encode CDF_ConnectsToList failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDataFrame(&m_SpeedLimitList) == false )
        {
            LogE("CDF_Lane encode CDF_SpeedLimitList failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDataFrame(&m_PointList) == false )
        {
            LogE("CDF_Lane encode CDF_PointList failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

USHORT CDF_Lane::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_Lane decode option flag failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;

    if ( DecodeDE_LaneID(m_LaneID) == false )
    {
        LogE("CDF_Lane decode DE_LaneID failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDataFrame(&m_LaneAttributes) == false )
        {
            LogE("CDF_Lane decode CDF_LaneAttributes failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_AllowedManeuvers(m_AllowedManeuvers) == false )
        {
            LogE("CDF_Lane decode DE_AllowdManeuvers failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDataFrame(&m_ConnectsToList) == false )
        {
            LogE("CDF_Lane decode CDF_ConnectsToList failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDataFrame(&m_SpeedLimitList) == false )
        {
            LogE("CDF_Lane decode CDF_SpeedLimitList failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDataFrame(&m_PointList) == false )
        {
            LogE("CDF_Lane decode CDF_PointList failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}


CDF_LaneList::CDF_LaneList()
{
    m_nMinListLen = MIN_LANE_LEN;
    m_nMaxListLen = MAX_LANE_LEN;

    m_strListObjectClassName = "CDF_Lane";
}


REGISTER_CLASS(CDF_Movement)
CDF_Movement::CDF_Movement() 
{
    m_PhaseID       = 0;
    m_nOptionCount  = 1;
}

bool CDF_Movement::SetParameter(CDF_NodeReferenceID * pNodeID, DE_PhaseID phaseID)
{
    if ( pNodeID == NULL )
    {
        LogE("CDF_Movement SetParameter CDF_NodeReferenceID point is NULL");
        return false;
    }

    m_RemoteIntersection.SetParameter(pNodeID);

    if (CHECK_INT_VALID(DE_PhaseID, phaseID) == true )
    {
        SetOptionFlag(0, true);
        m_PhaseID = phaseID;
    }

    return true;
}

bool CDF_Movement::GetParameter(CDF_NodeReferenceID * pNodeID, DE_PhaseID &phaseID)
{
    if ( pNodeID == NULL )
    {
        LogE("CDF_Movement GetParameter CDF_NodeReferenceID point is NULL");
        return false;
    }

    pNodeID->SetParameter(&m_RemoteIntersection);

    phaseID = m_PhaseID;

    return true;
}

bool CDF_Movement::SetParameter(CDF_Movement* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_Movement SetParameter CDF_Movement point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(&m_RemoteIntersection, m_PhaseID);

    return true;
}

void CDF_Movement::toParamString()                             // 将参数值输出到字符串中
{
    LogI("DF_Movement: PhaseID: %d", m_PhaseID);
    m_RemoteIntersection.toParamString();
    return;
}

USHORT CDF_Movement::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_Movement encode option map failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDataFrame(&m_RemoteIntersection) == false)
    {
        LogE("CDF_Movement encode CDF_NodeReferenceID failed.");
        return CODEC_FAILED;   
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( EncodeDE_PhaseID(m_PhaseID) == false )
        {
            LogE("CDF_Movement encode DE_PhaseID failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

USHORT CDF_Movement::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_Movement decode option map failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDataFrame(&m_RemoteIntersection) == false)
    {
        LogE("CDF_Movement decode CDF_NodeReferenceID failed.");
        return CODEC_FAILED;   
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( DecodeDE_PhaseID(m_PhaseID) == false )
        {
            LogE("CDF_Movement decode DE_PhaseID failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}


CDF_MovementList::CDF_MovementList()
{
    m_nMinListLen = MIN_MOVEMENT_LEN;
    m_nMaxListLen = MAX_MOVEMENT_LEN;

    m_strListObjectClassName = "CDF_Movement";
}


REGISTER_CLASS(CDF_Link)
CDF_Link::CDF_Link() 
{
    m_Name          = "";
    m_LaneWidth     = 0;

    m_nOptionCount  = 4;
}

bool CDF_Link::SetParameter(CDF_NodeReferenceID* pNodeID, DE_LaneWidth laneWidth, CDF_LaneList* pLaneList, DE_DescriptiveName name, CDF_SpeedLimitList* pSpeedLimitList, CDF_PointList* pPointList, CDF_MovementList* pMovementList)
{
    if ( pNodeID == NULL )
    {
        LogE("CDF_Link SetParameter CDF_NodeReferenceID point is NULL");
        return false;
    }

    m_UpstreamNodeID.SetParameter(pNodeID);

    if ( CHECK_INT_VALID(DE_LaneWidth, laneWidth) == false )
    {
        LogE("CDF_Link SetParameter DE_LaneWidth %d is out of range [%d, %d]", 
            laneWidth, DE_LaneWidth_MIN, DE_LaneWidth_MAX);

        return false;
    }

    m_LaneWidth = laneWidth;

    if ( pLaneList == NULL )
    {
        LogE("CDF_Link SetParameter CDF_LaneList point is NULL");
        return false;
    }

    m_LaneList.SetParameter(pLaneList);

    int nLen = name.GetLength();
    if ( nLen >= DE_DescriptiveName_MIN || nLen <= DE_DescriptiveName_MAX )
    {
        SetOptionFlag(0, true);
        m_Name = name;
    }

    if ( pSpeedLimitList != NULL )
    {
        SetOptionFlag(1, true);
        m_SpeedLimitList.SetParameter(pSpeedLimitList);
    }

    if ( pPointList != NULL )
    {
        SetOptionFlag(2, true);
        m_PointList.SetParameter(pPointList);
    }

    if ( pMovementList != NULL )
    {
        SetOptionFlag(3, true);
        m_MovementList.SetParameter(pMovementList);
    }

    return true;
}

bool CDF_Link::GetParameter(CDF_NodeReferenceID* pNodeID, DE_LaneWidth& laneWidth, CDF_LaneList* pLaneList, DE_DescriptiveName& name, CDF_SpeedLimitList* pSpeedLimitList, CDF_PointList* pPointList, CDF_MovementList* pMovementList)
{
    if ( pNodeID == NULL )
    {
        LogE("CDF_Link GetParameter CDF_NodeReferenceID point is NULL");
        return false;
    }

    pNodeID->SetParameter(&m_UpstreamNodeID);

    laneWidth = m_LaneWidth;

    if ( pLaneList == NULL )
    {
        LogE("CDF_Link GetParameter CDF_LaneList point is NULL");
        return false;
    }

    pLaneList->SetParameter(&m_LaneList);

    name = m_Name;

    if ( pSpeedLimitList != NULL )    
        pSpeedLimitList->SetParameter(&m_SpeedLimitList);

    if ( pPointList != NULL )
        pPointList->SetParameter(&m_PointList);


    if ( pMovementList != NULL )
        pMovementList->SetParameter(&m_MovementList);

    return true;
}

bool CDF_Link::SetParameter(CDF_Link* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_Link SetParameter CDF_Link point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(&m_UpstreamNodeID, m_LaneWidth, &m_LaneList, m_Name, &m_SpeedLimitList, &m_PointList, &m_MovementList);

    return true;
}

void CDF_Link::toParamString()                             // 将参数值输出到字符串中
{
    LogI("CDF_Link: DescriptiveName: %s, LaneWidth: %d, ", m_Name.GetBuffer(), m_LaneWidth);
    m_UpstreamNodeID.toParamString();
    m_SpeedLimitList.toParamString();
    m_PointList.toParamString();
    m_MovementList.toParamString();
    m_LaneList.toParamString();
    return;
}

USHORT CDF_Link::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_Link encode option map failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_DescriptiveName(m_Name) == false )
        {
            LogE("CDF_Link encode DE_DescriptiveName failed.");
            return CODEC_FAILED;
        }
    }

    if ( EncodeDataFrame(&m_UpstreamNodeID) == false )
    {
        LogE("CDF_Link encode CDF_NodeReferenceID failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDataFrame(&m_SpeedLimitList) == false )
        {
            LogE("CDF_Link encode CDF_SpeedLimitList failed.");
            return CODEC_FAILED;
        }
    }

    if ( EncodeDE_LaneWidth(m_LaneWidth) == false )
    {
        LogE("CDF_Link encode DE_LaneWidth failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDataFrame(&m_PointList) == false )
        {
            LogE("CDF_Link encode CDF_SpeedLimitList failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDataFrame(&m_MovementList) == false )
        {
            LogE("CDF_Link encode CDF_MovementList failed.");
            return CODEC_FAILED;
        }
    }

    if ( EncodeDataFrame(&m_LaneList) == false )
    {
        LogE("CDF_Link encode CDF_LaneList failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

USHORT CDF_Link::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_Link decode option map failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_DescriptiveName(m_Name) == false )
        {
            LogE("CDF_Link decode DE_DescriptiveName failed.");
            return CODEC_FAILED;
        }
    }

    if ( DecodeDataFrame(&m_UpstreamNodeID) == false )
    {
        LogE("CDF_Link decode CDF_NodeReferenceID failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDataFrame(&m_SpeedLimitList) == false )
        {
            LogE("CDF_Link decode CDF_SpeedLimitList failed.");
            return CODEC_FAILED;
        }
    }

    if ( DecodeDE_LaneWidth(m_LaneWidth) == false )
    {
        LogE("CDF_Link decode DE_LaneWidth failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDataFrame(&m_PointList) == false )
        {
            LogE("CDF_Link decode CDF_SpeedLimitList failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDataFrame(&m_MovementList) == false )
        {
            LogE("CDF_Link decode CDF_MovementList failed.");
            return CODEC_FAILED;
        }
    }

    if ( DecodeDataFrame(&m_LaneList) == false )
    {
        LogE("CDF_Link decode CDF_LaneList failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}


CDF_LinkList::CDF_LinkList()
{
    m_nMinListLen = MIN_LINK_LEN;
    m_nMaxListLen = MAX_LINK_LEN;

    m_strListObjectClassName = "CDF_Link";
}


REGISTER_CLASS(CDF_Node)
CDF_Node::CDF_Node()
{
    m_Name          = "";
    m_nOptionCount  = 2;
}

bool CDF_Node::SetParameter(CDF_NodeReferenceID* pNodeID, CDF_Position3D* pPos, DE_DescriptiveName name, CDF_LinkList* pLinkList)
{
    if ( pNodeID == NULL )
    {
        LogE("CDF_Node SetParameter CDF_NodeReferenceID point is NULL");
        return false;
    }

    m_NodeReferenceID.SetParameter(pNodeID);

    if ( pPos == NULL )
    {
        LogE("CDF_Node SetParameter CDF_Position3D point is NULL");
        return false;
    }

    m_RefPosition.SetParameter(pPos);

    int nLen = name.GetLength();
    if ( nLen >= DE_DescriptiveName_MIN || nLen <= DE_DescriptiveName_MAX )
    {
        SetOptionFlag(0, true);
        m_Name = name;
    }

    if ( pLinkList != NULL )
    {
        SetOptionFlag(1, true);
        m_InLink.SetParameter(pLinkList);
    }

    return true;
}

bool CDF_Node::GetParameter(CDF_NodeReferenceID *pNodeID, CDF_Position3D* pPos, DE_DescriptiveName& name, CDF_LinkList* pLinkList)
{
    if ( pNodeID == NULL )
    {
        LogE("CDF_Node GetParameter CDF_NodeReferenceID point is NULL");
        return false;
    }

    pNodeID->SetParameter(&m_NodeReferenceID);

    if ( pPos == NULL )
    {
        LogE("CDF_Node GetParameter CDF_Position3D point is NULL");
        return false;
    }

    pPos->SetParameter(&m_RefPosition);

    m_Name = name;

    if ( pLinkList != NULL )
    {
        pLinkList->SetParameter(&m_InLink);
    }

    return true;
}

bool CDF_Node::SetParameter(CDF_Node* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_Node SetParameter CDF_Node point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(&m_NodeReferenceID, &m_RefPosition, m_Name, &m_InLink);

    return true;
}

void CDF_Node::toParamString()                             // 将参数值输出到字符串中
{
    LogI("DF_Node: ");
    m_NodeReferenceID.toParamString();
    m_RefPosition.toParamString();
    m_Name.GetBuffer();
    m_InLink.toParamString();
    return;
}

USHORT CDF_Node::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_Node encode option map failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;


    if ( GetOptionFlag(0) == true )
    {
        if ( EncodeDE_DescriptiveName(m_Name) == false )
        {
            LogE("CDF_Node encode DE_DescriptiveName failed.");
            return CODEC_FAILED;
        }
    }

    if ( EncodeDataFrame(&m_NodeReferenceID) == false)
    {
        LogE("CDF_Node encode CDF_NodeReferenceID failed.");
        return CODEC_FAILED;   
    }

    if ( EncodeDataFrame(&m_RefPosition) == false)
    {
        LogE("CDF_Node encode CDF_Position3D failed.");
        return CODEC_FAILED;   
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( EncodeDataFrame(&m_InLink) == false )
        {
            LogE("CDF_Node encode CDF_LinkList failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

USHORT CDF_Node::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_Node decode option map failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;


    if ( GetOptionFlag(0) == true )
    {
        if ( DecodeDE_DescriptiveName(m_Name) == false )
        {
            LogE("CDF_Node decode DE_DescriptiveName failed.");
            return CODEC_FAILED;
        }
    }

    if ( DecodeDataFrame(&m_NodeReferenceID) == false)
    {
        LogE("CDF_Node decode CDF_NodeReferenceID failed.");
        return CODEC_FAILED;   
    }

    if ( DecodeDataFrame(&m_RefPosition) == false)
    {
        LogE("CDF_Node decode CDF_Position3D failed.");
        return CODEC_FAILED;   
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( DecodeDataFrame(&m_InLink) == false )
        {
            LogE("CDF_Node decode CDF_LinkList failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}



CDF_NodeList::CDF_NodeList()
{
    m_nMinListLen = MIN_NODE_LEN;
    m_nMaxListLen = MAX_NODE_LEN;

    m_strListObjectClassName = "CDF_Node";
}


CDF_VehicleClassification::CDF_VehicleClassification()
{ 
    m_VehicleClass = DE_BVC_Unknown; 
}


bool CDF_VehicleClassification::SetParameter(DE_BasicVehcleClass vehicleClass)
{
    if ( CHECK_INT_VALID(DE_BasicVehcleClass, vehicleClass) == false )
    {
        LogE("CDF_VehicleClassification SetParameter vehicleClass %d is out of range [%d, %d]",
            vehicleClass, DE_BasicVehcleClass_MIN, DE_BasicVehcleClass_MAX);
        return false;
    }

    m_VehicleClass = vehicleClass;

    return true;
}

bool CDF_VehicleClassification::GetParameter(DE_BasicVehcleClass& vehicleClass)
{
    vehicleClass = m_VehicleClass;

    return true;
}

bool CDF_VehicleClassification::SetParameter(CDF_VehicleClassification* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_VehicleClassification SetParameter CDF_VehicleClassification point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(m_VehicleClass);

    return true;
}

void CDF_VehicleClassification::toParamString()                             // 将参数值输出到字符串中
{
    LogI("DF_VehicleClassification: Vehicle Class: %d", m_VehicleClass);
    return;
}

USHORT CDF_VehicleClassification::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeDE_BasicVehcleClass(m_VehicleClass) == false )
    {
        LogE("CDF_VehicleClassification encode DE_BasicVehcleClass failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

USHORT CDF_VehicleClassification::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeDE_BasicVehcleClass(m_VehicleClass) == false )
    {
        LogE("CDF_VehicleClassification decode DE_BasicVehcleClass failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}


CDF_VehicleSize::CDF_VehicleSize() 
{
    m_Width         = 0;
    m_Length        = 0;
    m_Height        = 0;

    m_nOptionCount  = 1;
}

bool CDF_VehicleSize::SetParameter(DE_VehicleWidth width, DE_VehicleLength length, DE_VehicleHeight heigh)
{
    if ( CHECK_INT_VALID(DE_VehicleWidth, width) == false )
    {
        LogE("CDF_VehicleSize SetParameter DE_VehicleWidth %d is out of range [%d, %d]",
            width, DE_VehicleWidth_MIN, DE_VehicleWidth_MAX);
        return false;
    }
    m_Width = width;

    if ( CHECK_INT_VALID(DE_VehicleLength, length) == false )
    {
        LogE("CDF_VehicleSize SetParameter DE_VehicleLength %d is out of range [%d, %d]",
            length, DE_VehicleLength_MIN, DE_VehicleLength_MAX);
        return false;
    }
    m_Length = length;
    
    if ( CHECK_INT_VALID(DE_VehicleHeight, heigh) == true )
    {
        SetOptionFlag(0, true);
        m_Height = heigh;
    }

    return true;
}

bool CDF_VehicleSize::GetParameter(DE_VehicleWidth& width, DE_VehicleLength& length, DE_VehicleHeight& heigh)
{
    width   = m_Width;
    length  = m_Length;
    heigh   = m_Height;

    return true;
}

bool CDF_VehicleSize::SetParameter(CDF_VehicleSize* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_VehicleSize SetParameter CDF_VehicleSize point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(m_Width, m_Length, m_Height);

    return true;
}

void CDF_VehicleSize::toParamString()                            // 将参数值输出到字符串中
{
    LogI("DF_VehicleSize: width: %d, Length: %d, Height: %d", m_Width, m_Length, m_Height);
    return;    
}

USHORT CDF_VehicleSize::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_VehicleSize encode option map failed.");
        return false;
    }

    if ( EncodeDE_VehicleWidth(m_Width) == false )
    {
        LogE("CDF_VehicleSize encode DE_VehicleWidth failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_VehicleLength(m_Length) == false )
    {
        LogE("CDF_VehicleSize encode DE_VehicleLength failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( EncodeDE_VehicleHeight(m_Height) == false )
        {
            LogE("CDF_VehicleSize encode DE_VehicleHeight failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

USHORT CDF_VehicleSize::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_VehicleSize decode option map failed.");
        return false;
    }

    if ( DecodeDE_VehicleWidth(m_Width) == false )
    {
        LogE("CDF_VehicleSize decode DE_VehicleWidth failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_VehicleLength(m_Length) == false )
    {
        LogE("CDF_VehicleSize decode DE_VehicleLength failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(0) == true )
    {
        if ( DecodeDE_VehicleHeight(m_Height) == false )
        {
            LogE("CDF_VehicleSize decode DE_VehicleHeight failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}


REGISTER_CLASS(CDF_ParticipantData)
CDF_ParticipantData::CDF_ParticipantData() 
{
    m_ParticipantType   = DE_PT_Unknown;
    m_ParticipantID     = 0;
    m_SourceType        = DE_ST_Unknown;
    m_strRSUID          = "";
    m_strPlateNo        = "";
    m_secMark           = 60000;
    m_TransissionState  = DE_TS_Neutral;
    m_Speed             = 0;
    m_Heading           = 0;

    m_nOptionCount      = 7;
}

bool CDF_ParticipantData::SetParameter(DE_PartcipantType ptcType, int ptcID, DE_SourceType sourceType, DE_DSecond secMark, CDF_PositionOffsetLLV* pPos, CDF_PositionConfidenceSet* pPosCfdSet, DE_Speed speed, DE_Heading heading, CDF_VehicleSize* pVehicleSize)
{
    if ( CHECK_ENUM_VALID(DE_PartcipantType, ptcType) == false )
    {
        LogE("CDF_ParticipantData SetParameter DE_PartcipantType %d is out of range [0, %d]",
            ptcType, DE_PartcipantType_Invalid);
        return false;
    }

    m_ParticipantType = ptcType;

    if ( ptcID < 0 || ptcID > 65535 )
    {
        LogE("CDF_ParticipantData SetParameter PartcipantID %d is out of range [0, 65535]",
            ptcID);
        return false;
    }

    m_ParticipantID = ptcID;

    if ( CHECK_ENUM_VALID(DE_SourceType, sourceType) == false )
    {
        LogE("CDF_ParticipantData SetParameter DE_SourceType %d is out of range [0, %d]",
            sourceType, DE_SourceType_Invalid);
        return false;
    }

    m_SourceType = sourceType;

    if ( CHECK_INT_VALID(DE_DSecond, secMark) == false )
    {
        LogE("CDF_ParticipantData SetParameter DE_DSecond %d is out of range [%d, %d]",
            secMark, DE_DSecond_MIN, DE_DSecond_MAX);
        return false;
    }

    m_secMark = secMark;

    if ( pPos == NULL )
    {
        LogE("CDF_ParticipantData SetParameter CDF_PositionOffsetLLV point is NULL");
        return false;
    }

    m_Pos.SetParameter(pPos);

    if ( pPosCfdSet == NULL )
    {
        LogE("CDF_ParticipantData SetParameter CDF_PositionConfidenceSet point is NULL");
        return false;
    }

    m_Accuracy.SetParameter(pPosCfdSet);

    if ( CHECK_INT_VALID(DE_Speed, speed) == false )
    {
        LogE("CDF_ParticipantData SetParameter DE_Speed %d is out of range [%d, %d]",
            speed, DE_Speed_MIN, DE_Speed_MAX);
        return false;
    }

    m_Speed = speed;

    if ( CHECK_INT_VALID(DE_Heading, heading) == false )
    {
        LogE("CDF_ParticipantData SetParameter DE_Heading %d is out of range [%d, %d]",
            heading, DE_Heading_MIN, DE_Heading_MAX);
        return false;
    }

    m_Heading = heading;

    if ( pVehicleSize == NULL )
    {
        LogE("CDF_ParticipantData SetParameter CDF_VehicleSize point is NULL");
        return false;
    }

    m_VehicleSize.SetParameter(pVehicleSize);

    return true;
}

bool CDF_ParticipantData::SetParameter(CString strRSUID, CString strPlateNo, DE_TransmissionState transState, DE_SteeringWheelAngle angle, CDF_MotionConfidenceSet* pMotionCfdSet, CDF_AccelerationSet4Way* pAccSet4Way, CDF_VehicleClassification* pVehicleClass)
{
    if ( strRSUID.GetLength() == RSU_ID_LEN )
    {
        SetOptionFlag(0, true);
        m_strRSUID = strRSUID;
    }

    if ( strPlateNo.GetLength() == PLATE_NO_LEN )
    {
        SetOptionFlag(1, true);
        m_strPlateNo = strPlateNo;
    }

    if ( CHECK_ENUM_VALID(DE_TransmissionState, transState) == true )
    {
        SetOptionFlag(2, true);
        m_TransissionState = transState;
    }

    if ( CHECK_INT_VALID(DE_SteeringWheelAngle, angle) == true )
    {
        SetOptionFlag(3, true);
        m_SteeringWheelAngle = angle;
    }

    if ( pMotionCfdSet != NULL )
    {
        SetOptionFlag(4, true);
        m_MotionCfdSet.SetParameter(pMotionCfdSet);
    }

    if ( pAccSet4Way != NULL )
    {
        SetOptionFlag(5, true);
        m_AccelerationSet4Way.SetParameter(pAccSet4Way);
    }

    if ( pVehicleClass != NULL )
    {
        SetOptionFlag(6, true);
        m_VehicleClass.SetParameter(pVehicleClass);
    }

    return true;
}

bool CDF_ParticipantData::GetParameter(DE_PartcipantType& ptcType, int &ptcID, DE_SourceType& sourceType, DE_DSecond& secMark, CDF_PositionOffsetLLV* pPos, CDF_PositionConfidenceSet* pPosCfdSet, DE_Speed& speed, DE_Heading& heading, CDF_VehicleSize* pVehicleSize)
{
    ptcType     = m_ParticipantType;
    ptcID       = m_ParticipantID;
    sourceType  = m_SourceType;
    secMark     = m_secMark;
    speed       = m_Speed;
    heading     = m_Heading;

    if ( pPos == NULL )
    {
        LogE("CDF_ParticipantData GetParameter CDF_PositionOffsetLLV point is NULL");
        return false;
    }

    pPos->SetParameter(&m_Pos);

    if ( pPosCfdSet == NULL )
    {
        LogE("CDF_ParticipantData GetParameter CDF_PositionConfidenceSet point is NULL");
        return false;
    }

    pPosCfdSet->SetParameter(&m_Accuracy);

    if ( pVehicleSize == NULL )
    {
        LogE("CDF_ParticipantData GetParameter CDF_VehicleSize point is NULL");
        return false;
    }

    pVehicleSize->SetParameter(&m_VehicleSize);

    return true;
}

bool CDF_ParticipantData::GetParameter(CString &strRSUID, CString &strPlateNo, DE_TransmissionState& transState, DE_SteeringWheelAngle& angle, CDF_MotionConfidenceSet* pMotionCfdSet, CDF_AccelerationSet4Way* pAccSet4Way, CDF_VehicleClassification* pVehicleClass)
{
    strRSUID    = m_strRSUID;
    strPlateNo  = m_strPlateNo;
    transState  = m_TransissionState;
    angle       = m_SteeringWheelAngle;

    if ( pMotionCfdSet != NULL )
        pMotionCfdSet->SetParameter(&m_MotionCfdSet);

    if ( pAccSet4Way != NULL )
        pAccSet4Way->SetParameter(&m_AccelerationSet4Way);

    if ( pVehicleClass != NULL )
        pVehicleClass->SetParameter(&m_VehicleClass);
    
    return true;
}

bool CDF_ParticipantData::SetParameter(CDF_ParticipantData* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_ParticipantData SetParameter CDF_ParticipantData point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(m_ParticipantType, m_ParticipantID, m_SourceType, m_secMark, &m_Pos, &m_Accuracy, m_Speed, m_Heading, &m_VehicleSize);
    pObj->GetParameter(m_strRSUID, m_strPlateNo, m_TransissionState, m_SteeringWheelAngle, &m_MotionCfdSet, &m_AccelerationSet4Way, &m_VehicleClass);

    return true;
}

void CDF_ParticipantData::toParamString()                            // 将参数值输出到字符串中
{
    LogI("ptcType: %d, ptcID: %d, SrcType: %d, RSUID: %s, PlateNo: %s, secMark: %d, transState: %d, speed: %d, heading: %d, angle: %d",
        m_ParticipantType, m_ParticipantID, m_SourceType, m_strRSUID.GetBuffer(), m_strPlateNo.GetBuffer(), m_secMark, 
        m_TransissionState, m_Speed, m_Heading, m_SteeringWheelAngle);
    m_Pos.toParamString();
    m_Accuracy.toParamString();
    m_MotionCfdSet.toParamString();
    m_AccelerationSet4Way.toParamString();
    m_VehicleSize.toParamString();
    m_VehicleClass.toParamString();

    return;
}

USHORT CDF_ParticipantData::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_ParticipantData encode option map failed.");
        return false;
    }

    if ( EncodeDE_PartcipantType(m_ParticipantType) == false )
    {
        LogE("CDF_ParticipantData encode DE_PartcipantType failed.");
        return CODEC_FAILED;
    }

    if ( EncodeInteger(m_ParticipantID, 0, 65535) == false )
    {
        LogE("CDF_ParticipantData encode ParticipantID failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_SourceType(m_SourceType) == false )
    {
        LogE("CDF_ParticipantData encode DE_SourceType failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;
    USHORT nLen = 0;

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        nLen = m_strRSUID.GetLength();
        if ( nLen != RSU_ID_LEN )
        {
            LogE("CDF_ParticipantData RSU ID length %d is wrong, RSU_ID_LEN No length is: %d", 
                nLen, RSU_ID_LEN);
            return CODEC_FAILED;
        }

        nLen = RSU_ID_LEN;
        if ( EncodeOctString( (PUCHAR)m_strRSUID.GetBuffer(), nLen) == false )
        {
            LogE("CDF_ParticipantData decode Vehicle ID failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        nLen = m_strPlateNo.GetLength();
        if ( nLen != PLATE_NO_LEN )
        {
            LogE("CDF_ParticipantData Plate No Length %d is wrong, PlateNo length is %d",
                nLen, PLATE_NO_LEN);
            return CODEC_FAILED;
        }

        if ( EncodeOctString( (PUCHAR)m_strPlateNo.GetBuffer(), nLen) == false)
        {
            LogE("CDF_ParticipantData encode Plate No failed.");
            return CODEC_FAILED;
        }
    }

    if ( EncodeDE_DSecond(m_secMark) == false )
    {
        LogE("CDF_ParticipantData encode DE_DSecond failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDataFrame(&m_Pos) == false )
    {
        LogE("CDF_ParticipantData encode CDF_PositionOffsetLLV failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDataFrame(&m_Accuracy) == false )
    {
        LogE("CDF_ParticipantData encode CDF_PositionConfidenceSet failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_TransmissionState(m_TransissionState) == false )
        {
            LogE("CDF_ParticipantData encode DE_TransmissionState failed.");
            return CODEC_FAILED;
        }
    }

    if ( EncodeDE_Speed(m_Speed) == false )
    {
        LogE("CDF_ParticipantData encode DE_Speed failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_Heading(m_Heading) == false )
    {
        LogE("CDF_ParticipantData encode DE_Heading failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDE_SteeringWheelAngle(m_SteeringWheelAngle) == false )
        {
            LogE("CDF_ParticipantData encode DE_TransmissionState failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDataFrame(&m_MotionCfdSet) == false )
        {
            LogE("CDF_ParticipantData encode CDF_MotionConfidenceSet failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDataFrame(&m_AccelerationSet4Way) == false )
        {
            LogE("CDF_ParticipantData encode CDF_AccelerationSet4Way failed.");
            return CODEC_FAILED;
        }
    }

    if ( EncodeDataFrame(&m_VehicleSize) == false )
    {
        LogE("CDF_ParticipantData encode CDF_VehicleSize failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( EncodeDataFrame(&m_VehicleClass) == false )
        {
            LogE("CDF_ParticipantData encode CDF_VehicleClassification failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

USHORT CDF_ParticipantData::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_ParticipantData decode option map failed.");
        return false;
    }

    if ( DecodeDE_PartcipantType(m_ParticipantType) == false )
    {
        LogE("CDF_ParticipantData decode DE_PartcipantType failed.");
        return CODEC_FAILED;
    }

    if ( DecodeInteger(m_ParticipantID, 0, 65535) == false )
    {
        LogE("CDF_ParticipantData decode ParticipantID failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_SourceType(m_SourceType) == false )
    {
        LogE("CDF_ParticipantData decode DE_SourceType failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;

    char temp[MAX_STR_LEN];
    USHORT nLen = RSU_ID_LEN;
    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        nLen = RSU_ID_LEN;
        if ( DecodeOctString( (PUCHAR)temp, MAX_STR_LEN, nLen) == false )
        {
            LogE("CDF_ParticipantData decode Vehicle ID failed.");
            return CODEC_FAILED;
        }

        if ( nLen != RSU_ID_LEN )
        {
            LogE("CBasicSafetyMessage Plate No Length %d is wrong %d",
                nLen, PLATE_NO_LEN);
            return CODEC_FAILED;
        }

        temp[nLen] = '\0';
        m_strRSUID = temp;
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        nLen = PLATE_NO_LEN;
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

    if ( DecodeDE_DSecond(m_secMark) == false )
    {
        LogE("CDF_ParticipantData decode DE_DSecond failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDataFrame(&m_Pos) == false )
    {
        LogE("CDF_ParticipantData decode CDF_PositionOffsetLLV failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDataFrame(&m_Accuracy) == false )
    {
        LogE("CDF_ParticipantData decode CDF_PositionConfidenceSet failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_TransmissionState(m_TransissionState) == false )
        {
            LogE("CDF_ParticipantData decode DE_TransmissionState failed.");
            return CODEC_FAILED;
        }
    }

    if ( DecodeDE_Speed(m_Speed) == false )
    {
        LogE("CDF_ParticipantData decode DE_Speed failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_Heading(m_Heading) == false )
    {
        LogE("CDF_ParticipantData decode DE_Heading failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDE_SteeringWheelAngle(m_SteeringWheelAngle) == false )
        {
            LogE("CDF_ParticipantData decode DE_TransmissionState failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDataFrame(&m_MotionCfdSet) == false )
        {
            LogE("CDF_ParticipantData decode CDF_MotionConfidenceSet failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDataFrame(&m_AccelerationSet4Way) == false )
        {
            LogE("CDF_ParticipantData decode CDF_AccelerationSet4Way failed.");
            return CODEC_FAILED;
        }
    }

    if ( DecodeDataFrame(&m_VehicleSize) == false )
    {
        LogE("CDF_ParticipantData decode CDF_VehicleSize failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex++) == true )
    {
        if ( DecodeDataFrame(&m_VehicleClass) == false )
        {
            LogE("CDF_ParticipantData decode CDF_VehicleClassification failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

CDF_ParticipantList::CDF_ParticipantList()
{
    m_nMinListLen = MIN_PARTICIPANT_LEN;
    m_nMaxListLen = MAX_PARTICIPANT_LEN;

    m_strListObjectClassName = "CDF_ParticipantData";
}


REGISTER_CLASS(CDF_PathHistoryPoint)
CDF_PathHistoryPoint::CDF_PathHistoryPoint() 
{
    m_Speed         = 0;

    m_nOptionCount  = 3;
}

bool CDF_PathHistoryPoint::SetParameter(CDF_PositionOffsetLLV* pPos, DE_TimeOffset timeOffset, DE_Speed speed, CDF_PositionConfidenceSet* pPosCfdSet, DE_CoarseHeading heading)
{
    if ( pPos == NULL )
    {
        LogE("CDF_PathHistoryPoint SetParameter CDF_PositionOffsetLLV point is NULL");
        return false;
    }

    m_PositionOffsetLLV.SetParameter(pPos);

    if ( CHECK_INT_VALID(DE_TimeOffset, timeOffset) == false )
    {
        LogE("CDF_PathHistoryPoint SetParameter DE_TimeOffset %d is out of range [%d, %d]",
            timeOffset, DE_TimeOffset_MIN, DE_TimeOffset_MAX);
        return false;
    }

    m_TimeOffset = timeOffset;

    if ( CHECK_INT_VALID(DE_Speed, speed) == true )
    {
        SetOptionFlag(0, true);
        m_Speed = speed;
    }

    if ( pPosCfdSet != NULL )
    {
        SetOptionFlag(1, true);
        m_PositionConfidenceSet.SetParameter(pPosCfdSet);
    }

    if ( CHECK_INT_VALID(DE_CoarseHeading, heading) == true )
    {
        SetOptionFlag(2, true);
        m_Heading = heading;
    }

    return true;
}

bool CDF_PathHistoryPoint::GetParameter(CDF_PositionOffsetLLV* pPos, DE_TimeOffset& timeOffset, DE_Speed& speed, CDF_PositionConfidenceSet* pPosCfdSet, DE_CoarseHeading& heading)
{
    timeOffset = m_TimeOffset;
    speed = m_Speed;
    heading = m_Heading;

    if ( pPos == NULL )
    {
        LogE("CDF_PathHistoryPoint GetParameter CDF_PositionOffsetLLV point is NULL");
        return false;
    }

    pPos->SetParameter(&m_PositionOffsetLLV);

    if ( pPosCfdSet != NULL )
        pPosCfdSet->SetParameter(&m_PositionConfidenceSet);

    return true;
}

bool CDF_PathHistoryPoint::SetParameter(CDF_PathHistoryPoint* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_PathHistoryPoint SetParameter CDF_PathHistoryPoint point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->SetParameter(&m_PositionOffsetLLV, m_TimeOffset, m_Speed, &m_PositionConfidenceSet, m_Heading);

    return true;
}

void CDF_PathHistoryPoint::toParamString()                             // 将参数值输出到字符串中
{
    LogI("DF_PathHistoryPoint: timeOffset: %d, speed: %d, heading: %d", m_TimeOffset, m_Speed, m_Heading);
    m_PositionOffsetLLV.toParamString();
    m_PositionConfidenceSet.toParamString(); 
    return;
}

USHORT CDF_PathHistoryPoint::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_PathHistoryPoint encode option map failed.");
        return false;
    }

    if ( EncodeDataFrame(&m_PositionOffsetLLV) == false )
    {
        LogE("CDF_PathHistoryPoint encode CDF_PositionOffsetLLV failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_TimeOffset(m_TimeOffset) == false )
    {
        LogE("CDF_PathHistoryPoint encode DE_TimeOffset failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;
    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( EncodeDE_Speed(m_Speed) == false )
        {
            LogE("CDF_PathHistoryPoint encode DE_Speed failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( EncodeDataFrame(&m_PositionConfidenceSet) == false )
        {
            LogE("CDF_PathHistoryPoint encode CDF_PositionConfidenceSet failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( EncodeDE_CoarseHeading(m_Heading) == false )
        {
            LogE("CDF_PathHistoryPoint encode DE_CoarseHeading failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

USHORT CDF_PathHistoryPoint::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_PathHistoryPoint decode option map failed.");
        return false;
    }

    if ( DecodeDataFrame(&m_PositionOffsetLLV) == false )
    {
        LogE("CDF_PathHistoryPoint decode CDF_PositionOffsetLLV failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_TimeOffset(m_TimeOffset) == false )
    {
        LogE("CDF_PathHistoryPoint decode DE_TimeOffset failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;
    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( DecodeDE_Speed(m_Speed) == false )
        {
            LogE("CDF_PathHistoryPoint decode DE_Speed failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( DecodeDataFrame(&m_PositionConfidenceSet) == false )
        {
            LogE("CDF_PathHistoryPoint decode CDF_PositionConfidenceSet failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( DecodeDE_CoarseHeading(m_Heading) == false )
        {
            LogE("CDF_PathHistoryPoint decode DE_CoarseHeading failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}


CDF_PathHistoryPointList::CDF_PathHistoryPointList()
{
    m_nMinListLen = MIN_PATH_HISTORY_POINT_LEN;
    m_nMaxListLen = MAX_PATH_HISTORY_POINT_LEN;

    m_strListObjectClassName = "CDF_PathHistoryPoint";
}


CDF_PathHistory::CDF_PathHistory()
{
    m_currGNSSStatus    = DE_GNSS_Unavailable;
    m_nOptionCount      = 2;
}

bool CDF_PathHistory::SetParameter(CDF_PathHistoryPointList* pList, UCHAR GNSSstatus, CDF_FullPositionVector* pVector)
{
    if ( pList == NULL )
    {
        LogE("CDF_PathHistory SetParameter CDF_FullPositionVector point is NULL");
        return false;
    }

    m_crumbData.SetParameter(pList);

    if ( pVector != NULL )
    {
        SetOptionFlag(0, true);
        m_InitialPosition.SetParameter(pVector);
    }

    if ( CHECK_ENUM_VALID( DE_GNSSstatus, GNSSstatus ) == true )
    {
        SetOptionFlag(1, true);
        m_currGNSSStatus = GNSSstatus;
    }

    return true;
}

bool CDF_PathHistory::GetParameter(CDF_PathHistoryPointList* pList, UCHAR& GNSSstatus, CDF_FullPositionVector* pVector)
{
    if ( pList == NULL )
    {
        LogE("CDF_PathHistory GetParameter CDF_FullPositionVector point is NULL");
        return false;
    }

    pList->SetParameter(&m_crumbData);

    if ( pVector != NULL )
        pVector->SetParameter(&m_InitialPosition);


    GNSSstatus = m_currGNSSStatus;

    return true;
}

bool CDF_PathHistory::SetParameter(CDF_PathHistory* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_PathHistory SetParameter CDF_PathHistory point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter(&m_crumbData, m_currGNSSStatus, &m_InitialPosition);

    return true;
}

void CDF_PathHistory::toParamString()                             // 将参数值输出到字符串中
{
    LogI("DF_PathHistory: GNSSstatus: %d", m_currGNSSStatus);
    m_InitialPosition.toParamString();
    m_crumbData.toParamString();
    return;
}

USHORT CDF_PathHistory::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_PathHistory encode option map failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;

    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( EncodeDataFrame(&m_InitialPosition) == false )
        {
            LogE("CDF_PathHistory encode CDF_FullPositionVector failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( EncodeDE_GNSSstatus(m_currGNSSStatus) == false )
        {
            LogE("CDF_PathHistory encode DE_GNSSstatus failed.");
            return CODEC_FAILED;
        }
    }

    if ( EncodeDataFrame(&m_crumbData) == false )
    {
        LogE("CDF_PathHistory encode CDF_PathHistoryPointList failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

USHORT CDF_PathHistory::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_PathHistory decode option map failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;

    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( DecodeDataFrame(&m_InitialPosition) == false )
        {
            LogE("CDF_PathHistory decode CDF_FullPositionVector failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( DecodeDE_GNSSstatus(m_currGNSSStatus) == false )
        {
            LogE("CDF_PathHistory decode DE_GNSSstatus failed.");
            return CODEC_FAILED;
        }
    }

    if ( DecodeDataFrame(&m_crumbData) == false )
    {
        LogE("CDF_PathHistory decode CDF_PathHistoryPointList failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

CDF_PathPointList::CDF_PathPointList()
{
    m_nMinListLen = MIN_PATH_POINT_LEN;
    m_nMaxListLen = MAX_PATH_POINT_LEN;

    m_strListObjectClassName = "CDF_PositionOffsetLLV";
}


CDF_PathPrediction::CDF_PathPrediction() 
{
    m_RadiusOfCurve     = 0;
    m_Confidence        = 0;
}

bool CDF_PathPrediction::SetParameter(DE_RadiusOfCurvation radius, DE_Confidence cfd)
{
    if ( CHECK_INT_VALID(DE_RadiusOfCurvation, radius) == false )
    {
        LogE("CDF_PathPrediction SetParameter DE_RadiusOfCurvation %d is out of range [%d, %d]",
            radius, DE_RadiusOfCurvation_MIN, DE_RadiusOfCurvation_MAX);
        return false;
    }

    m_RadiusOfCurve = radius;

    if ( CHECK_INT_VALID(DE_Confidence, cfd) == false )
    {
        LogE("CDF_PathPrediction SetParameter DE_Confidence %d is out of range [%d, %d]",
            cfd, DE_Confidence_MIN, DE_Confidence_MAX);
        return false;
    }

    m_Confidence = cfd;

    return true;
}

bool CDF_PathPrediction::GetParameter(DE_RadiusOfCurvation& radius, DE_Confidence& cfd)
{
    radius  = m_RadiusOfCurve;
    cfd     = m_Confidence;

    return true;
}

bool CDF_PathPrediction::SetParameter(CDF_PathPrediction* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_PathPrediction SetParameter CDF_PathPrediction point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter( m_RadiusOfCurve, m_Confidence);

    return true;
}

void CDF_PathPrediction::toParamString()                             // 将参数值输出到字符串中
{
    LogI("DF_PathPrediction: DE_RadiusOfCurvation: %d, DE_Confidence: %d", m_RadiusOfCurve, m_Confidence);
    return;
}

USHORT CDF_PathPrediction::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeDE_RadiusOfCurvation(m_RadiusOfCurve) == false )
    {
        LogE("CDF_PathPrediction encode DE_RadiusOfCurvation failed.");
        return CODEC_FAILED;
    }

    if ( EncodeDE_Confidence(m_Confidence) == false) 
    {
        LogE("CDF_PathPrediction encode DE_Confidence failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

USHORT CDF_PathPrediction::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeDE_RadiusOfCurvation(m_RadiusOfCurve) == false )
    {
        LogE("CDF_PathPrediction decode DE_RadiusOfCurvation failed.");
        return CODEC_FAILED;
    }

    if ( DecodeDE_Confidence(m_Confidence) == false) 
    {
        LogE("CDF_PathPrediction decode DE_Confidence failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

CDF_SignalState::CDF_SignalState() 
{
    m_Status            = DE_ISO_ManualControlIsEnabled;
    m_MinuteOfTheYear   = 0;
    m_TimeStamp         = 60000;

    m_nOptionCount      = 2;
}

bool CDF_SignalState::SetParameter(USHORT status, CDF_PhaseList* pPhaseList, DE_MinuteOfTheYear moy, DE_DSecond timeStamp)
{
    if ( CHECK_ENUM_VALID(DE_IntersectionStatusObject, status ) == false )
    {
        LogE("CDF_SignalState SetParameter DE_IntersectionStatusObject %d is out of range [0, %d)",
            status, DE_IntersectionStatusObject_Invalid);
        return false;
    }

    m_Status = status;

    if ( pPhaseList == NULL )
    {
        LogE("CDF_SignalState SetParameter CDF_PhaseList point is NULL");
        return false;
    }

    m_PhaseList.SetParameter(pPhaseList);

    if ( CHECK_INT_VALID(DE_MinuteOfTheYear, moy) == true )
    {
        SetOptionFlag(0, true);
        m_MinuteOfTheYear = moy;
    }

    if ( CHECK_INT_VALID(DE_DSecond, timeStamp) == true )
    {
        SetOptionFlag(1, true);
        m_TimeStamp = timeStamp;
    }

    return true;
}

bool CDF_SignalState::GetParameter(USHORT& status, CDF_PhaseList* pPhaseList, DE_MinuteOfTheYear &moy, DE_DSecond &timeStamp)
{
    status      = m_Status;
    moy         = m_MinuteOfTheYear;
    timeStamp   = m_TimeStamp;

    if ( pPhaseList == NULL )
    {
        LogE("CDF_SignalState GetParameter CDF_PhaseList point is NULL");
        return false;
    }

    pPhaseList->SetParameter(&m_PhaseList);

    return true;
}

bool CDF_SignalState::SetParameter(CDF_SignalState* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_SignalState SetParameter CDF_SignalState point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter( m_Status, &m_PhaseList, m_MinuteOfTheYear, m_TimeStamp);

    return true;    
}

void CDF_SignalState::toParamString()                             // 将参数值输出到字符串中
{
    LogI("SignalState: status: %d, MinuteOfYear: %d, timeStamp: %d",
        m_Status, m_MinuteOfTheYear, m_TimeStamp);
    m_PhaseList.toParamString();
    return;
}

USHORT CDF_SignalState::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_SignalState encode option map failed.");
        return CODEC_FAILED;
    }
    int nOptionIndex = 0;

    if ( EncodeDE_IntersectionStatusObject(m_Status) == false )
    {
        LogE("CDF_PathPrediction encode DE_IntersectionStatusObject failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( EncodeDE_MinuteOfTheYear(m_MinuteOfTheYear) == false )
        {
            LogE("CDF_PathPrediction encode DE_MinuteOfTheYear failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( EncodeDE_DSecond(m_TimeStamp) == false )
        {
            LogE("CDF_PathPrediction encode DE_DSecond failed.");
            return CODEC_FAILED;
        }
    }

    if ( EncodeDataFrame(&m_PhaseList) == false) 
    {
        LogE("CDF_PathPrediction encode CDF_PhaseList failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}

USHORT CDF_SignalState::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf          = pData;
    m_nBitLen       = nBitLen;
    m_nBitPos       = nStartBitPos;
    m_bIsReceived   = true;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_SignalState decode option map failed.");
        return CODEC_FAILED;
    }
    int nOptionIndex = 0;

    if ( DecodeDE_IntersectionStatusObject(m_Status) == false )
    {
        LogE("CDF_PathPrediction decode DE_IntersectionStatusObject failed.");
        return CODEC_FAILED;
    }

    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( DecodeDE_MinuteOfTheYear(m_MinuteOfTheYear) == false )
        {
            LogE("CDF_PathPrediction decode DE_MinuteOfTheYear failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( DecodeDE_DSecond(m_TimeStamp) == false )
        {
            LogE("CDF_PathPrediction decode DE_DSecond failed.");
            return CODEC_FAILED;
        }
    }

    if ( DecodeDataFrame(&m_PhaseList) == false) 
    {
        LogE("CDF_PathPrediction decode CDF_PhaseList failed.");
        return CODEC_FAILED;
    }

    return m_nBitPos;
}


CDF_VehicleSafetyExtensions::CDF_VehicleSafetyExtensions() 
{ 
    m_Events        = DE_VehicleEventFlags_Invalid;
    m_Lights        = DE_ExteriorLights_Invalid;

    m_nEventBitLen  = DE_VehicleEventFlags_MIN_LEN;
    m_nLightBitLen  = DE_ExteriorLights_MIN_LEN;

    m_nOptionCount  = 4;
}

// 四个参数都为可选参数
bool CDF_VehicleSafetyExtensions::SetParameter(UINT events, USHORT nEventBitLen, CDF_PathHistory* pPathHistory, CDF_PathPrediction* pPathPrediction, UINT lights, USHORT nLightBitLen)
{
    if ( CHECK_ENUM_VALID(DE_VehicleEventFlags, events ) == true )
    {
        SetOptionFlag(0, true);
        m_Events        = events;
        m_nEventBitLen  = nEventBitLen;
    }

    if ( pPathHistory != NULL )
    {
        SetOptionFlag(1, true);
        m_PathHistory.SetParameter(pPathHistory);
    }

    if ( pPathPrediction != NULL )
    {
        SetOptionFlag(2, true);
        m_PathPrediction.SetParameter(pPathPrediction);
    }

    if ( CHECK_ENUM_VALID( DE_ExteriorLights, lights ) == true )
    {
        SetOptionFlag(3, true);
        m_Lights        = lights;
        m_nLightBitLen  = nLightBitLen;
    }

    return true;
}

bool CDF_VehicleSafetyExtensions::GetParameter(UINT& events, USHORT& nEventBitLen, CDF_PathHistory* pPathHistory, CDF_PathPrediction* pPathPrediction, UINT& lights, USHORT &nLightBitLen)
{
    events          = m_Events;
    lights          = m_Lights;
    nEventBitLen    = m_nEventBitLen;
    nLightBitLen    = m_nLightBitLen;

    if ( pPathHistory != NULL )
        pPathHistory->SetParameter(&m_PathHistory);
    
    if ( pPathPrediction != NULL )
        pPathPrediction->SetParameter(&m_PathPrediction);

    return true;
}

bool CDF_VehicleSafetyExtensions::SetParameter(CDF_VehicleSafetyExtensions* pObj)
{
    if ( pObj == NULL )
    {
        LogE("CDF_VehicleSafetyExtensions SetParameter CDF_VehicleSafetyExtensions point is NULL");
        return false;
    }

    CCSAEMsgBase::SetParameter(pObj);

    pObj->GetParameter( m_Events, m_nEventBitLen, &m_PathHistory, &m_PathPrediction, m_Lights, m_nLightBitLen);

    return true;
}

void CDF_VehicleSafetyExtensions::toParamString()                             // 将参数值输出到字符串中
{
    LogI("CDF_VehicleSafetyExtensions event: %d, lights: %d", m_Events, m_Lights);
    m_PathHistory.toParamString();
    m_PathPrediction.toParamString();
    return;
}

USHORT CDF_VehicleSafetyExtensions::Encode()
{
    m_nBitPos = 0;
    m_nBitLen = 0;

    if ( EncodeOptionFlag() == false )
    {
        LogE("CDF_VehicleSafetyExtensions encode option map failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;

    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( EncodeDE_VehicleEventFlags(m_Events, m_nEventBitLen) == false )
        {
            LogE("CDF_VehicleSafetyExtensions encode DE_VehicleEventFlags failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( EncodeDataFrame(&m_PathHistory) == false )
        {
            LogE("CDF_VehicleSafetyExtensions encode CDF_PathHistory failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( EncodeDataFrame(&m_PathPrediction) == false )
        {
            LogE("CDF_VehicleSafetyExtensions encode CDF_PathPrediction failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( EncodeDE_ExteriorLights(m_Lights, m_nLightBitLen) == false )
        {
            LogE("CDF_VehicleSafetyExtensions encode DE_ExteriorLights failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}
    
USHORT CDF_VehicleSafetyExtensions::Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos)
{
    m_pBuf      = pData;
    m_nBitPos   = nStartBitPos;
    m_nBitLen   = nBitLen;

    if ( DecodeOptionFlag() == false )
    {
        LogE("CDF_VehicleSafetyExtensions decode option map failed.");
        return CODEC_FAILED;
    }

    int nOptionIndex = 0;

    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( DecodeDE_VehicleEventFlags(m_Events, m_nEventBitLen) == false )
        {
            LogE("CDF_VehicleSafetyExtensions decode DE_VehicleEventFlags failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( DecodeDataFrame(&m_PathHistory) == false )
        {
            LogE("CDF_VehicleSafetyExtensions decode CDF_PathHistory failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( DecodeDataFrame(&m_PathPrediction) == false )
        {
            LogE("CDF_VehicleSafetyExtensions decode CDF_PathPrediction failed.");
            return CODEC_FAILED;
        }
    }

    if ( GetOptionFlag(nOptionIndex ++) == true )
    {
        if ( DecodeDE_ExteriorLights(m_Lights, m_nLightBitLen) == false )
        {
            LogE("CDF_VehicleSafetyExtensions decode DE_ExteriorLights failed.");
            return CODEC_FAILED;
        }
    }

    return m_nBitPos;
}

