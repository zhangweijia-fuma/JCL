/*------------------------------------------------------------------------------
 * Common - Component :: T/CSAE 53-2017 Specification
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    CSAEDataFrame.h
 * Purpose: T/CSAE 53-2017 Specification data frame definition
 *----------------------------------------------------------------------------*/
#ifndef _CSAE_DATA_FRAME_H_
#define _CSAE_DATA_FRAME_H_

#include "CString.h"
#include "Debug.h"
#include "Common.h"

#include "CSAEMsgBase.h"


/////////////////////////////////////////////////////////////////////////////////////////////////
// 数据帧定义 Date Frame
// 共54种类型

extern int DEFAULT_TIME_OFFSET;                                          // 缺省的和UTC时间相差的分钟数，默认是东八区

#define MAX_CONNECTION_LIST_LEN         8
#define MIN_CONNECTION_LIST_LEN         1

#define MAX_INTERSECTION_STATUS_LIST_LEN 32
#define MIN_INTERSECTION_STATUS_LIST_LEN 1

#define MAX_LANE_LEN                    32                              // 车道最大数量
#define MIN_LANE_LEN                    1                               // 车道最小数量

#define MAX_LINK_LEN                    32                              // 路段最大数量
#define MIN_LINK_LEN                    1                               // 路段最小数量

#define MAX_MOVEMENT_LEN                32                              // 最多连接关系数量
#define MIN_MOVEMENT_LEN                1                               // 最少连接关系数量

#define MAX_NODE_LEN                    32                              // 地图节点最大数量
#define MIN_NODE_LEN                    1                               // 地图节点最小数量

#define MAX_PARTICIPANT_LEN             16                              // 最大交通参与者数量
#define MIN_PARTICIPANT_LEN             1                               // 最小交通参与者数量

#define MAX_PATH_HISTORY_POINT_LEN      23                              // 最大历史轨迹点数量
#define MIN_PATH_HISTORY_POINT_LEN      1                               // 最小历史轨迹点数量

#define MAX_PATH_POINT_LEN              32                              // 最大轨迹点数量
#define MIN_PATH_POINT_LEN              2                               // 最小轨迹点数量

#define MAX_PHASE_LEN                   16                              // 最大相位数量
#define MIN_PHASE_LEN                   1                               // 最小相位数量

#define MAX_PHASE_STATE_LIST_LEN        16                              // 最大相位状态数量
#define MIN_PHASE_STATE_LIST_LEN        1                               // 最小相位状态数量

#define MAX_ROAD_POINT_LIST_LEN         31                              // 最大中间位置点数量
#define MIN_ROAD_POINT_LIST_LEN         2                               // 最大中间位置点数量

#define MAX_SPEED_LIMIT_LEN             9                               // 一个路段或车道的最多限速数量
#define MIN_SPEED_LIMIT_LEN             1                               // 一个路段或车道的最小限速数量


// 选择项的索引值
#define VERTICAL_OFFSET_B07             0
#define VERTICAL_OFFSET_B08             1
#define VERTICAL_OFFSET_B09             2
#define VERTICAL_OFFSET_B10             3
#define VERTICAL_OFFSET_B11             4
#define VERTICAL_OFFSET_B12             5
#define VERTICAL_OFFSET_ELEVATION       6

#define MAX_DATA_FRAME_LIST_LEN         48                              // 在规范中，数据帧列表的最大长度

// 定义一个数据帧列表
// 在发送消息中，列表中的对象由业务层创建；
// 在接收消息中，列表中的对象由该类创建并负责删除
class CDF_DataFrameListBase : public CCSAEMsgBase
{
    public:
        CDF_DataFrameListBase();
        ~CDF_DataFrameListBase();

    public:
        int                 AddListItem(CCSAEMsgBase* item);
        bool                RemoveListItem(int nItemIndex);
        CCSAEMsgBase*       GetListItem(int nItemIndex);
        int                 GetListLen() { return m_nListLen; };

        CString             GetListClassName() { return m_strListObjectClassName; }
        int                 GetMinListLen() { return m_nMinListLen; }
        int                 GetMaxListLen() { return m_nMaxListLen; }

        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        void                SetParameter(CDF_DataFrameListBase*);

        virtual void        Test() = 0;                                 // 该类不能直接使用
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        int                 m_nMinListLen, m_nMaxListLen;               // 数据帧列表的最小长度和最大长度
        int                 m_nListLen;                                 // 数据帧列表长度
        CCSAEMsgBase*       m_DataFrameList[MAX_DATA_FRAME_LIST_LEN];   // 数据帧实例列表
        CString             m_strListObjectClassName;                   // 数据帧的类名，由派生类在构造函数中指定
};

// 不需要新增新的成员函数和成员变量
#define Define_List_Class(class_name) \
class class_name : public CDF_DataFrameListBase \
{ \
    public: \
        class_name(); \
        virtual void        Test(); \
};


// 定义参考ID。参考ID 由一个全局唯一的地区ID 和一个地区内部唯一的节点ID 组成。
class CDF_NodeReferenceID : public CCSAEMsgBase
{
    public:
        CDF_NodeReferenceID();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(DE_NodeID, DE_RoadRegulatorID = DE_RoadRegulatorID_MAX + 1 );
        bool                GetParameter(DE_NodeID&, DE_RoadRegulatorID&);

        bool                SetParameter(CDF_NodeReferenceID*);

        virtual void        Test();
        virtual void        toParamString();    
        
    protected:
        DE_RoadRegulatorID  m_RegionID;                                 // 0: 用于测试，范围：0 ~ 65535，可选项
        DE_NodeID           m_NodeID;                                   // 0 ~ 255: 预留测试使用，范围： 0 ~ 65535

};

// 三维的坐标位置，包括经纬度和高程
class CDF_Position3D : public CCSAEMsgBase
{
    public:
        CDF_Position3D();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(DE_Latitude, DE_Longtitude, DE_Elevation = DE_Elevation_MAX + 1);
        bool                GetParameter(DE_Latitude&, DE_Longtitude&, DE_Elevation&);

        bool                SetParameter(CDF_Position3D*);

        virtual void        Test();
        virtual void        toParamString();

    protected:
        DE_Latitude         m_Latitude;                 
        DE_Longtitude       m_Longtitude;
        DE_Elevation        m_Elevation;                                // 可选参数，单位：厘米
};

// 定义位置（经纬度和高程）的综合精度
class CDF_PositionConfidenceSet : public CCSAEMsgBase
{
    public:
        CDF_PositionConfidenceSet();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(DE_PositionConfidence, DE_ElevationConfidence = DE_ElevationConfidence_Invalid);
        bool                GetParameter(DE_PositionConfidence&, DE_ElevationConfidence&);

        bool                SetParameter(CDF_PositionConfidenceSet*);

        virtual void        Test();
        virtual void        toParamString();

    protected:
        DE_PositionConfidence   m_PositoinCfd;
        DE_ElevationConfidence  m_ElevationCfd;
};

// 描述车辆运行状态的精度，包括车速精度，航向精度和方向盘转角精度
// 参数均为可选
class CDF_MotionConfidenceSet : public CCSAEMsgBase
{
    public:
        CDF_MotionConfidenceSet();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(DE_SpeedConfidence = DE_SpeedConfidence_Invalid, DE_HeadingConfidence = DE_HeadingConfidence_Invalid,  
                                DE_SteeringWheelAngleConfidence = DE_SteeringWheelAngleConfidence_Invalid);
        bool                GetParameter(DE_SpeedConfidence&, DE_HeadingConfidence&, DE_SteeringWheelAngleConfidence&);

        bool                SetParameter(CDF_MotionConfidenceSet*);

        virtual void        Test();
        virtual void        toParamString();

    protected:
        DE_SpeedConfidence      m_SpeedCfd;
        DE_HeadingConfidence    m_HeadingCfd;
        DE_SteeringWheelAngleConfidence m_SteerCfd;
};

// 描述了信号灯一个相位状态的完整计时状态。其中：
// ● startTime：表示该相位状态下一次开始（距离当前时刻）的时间，或者预测开始的时间。如
// 果当前正值该相位状态，则该数值为0；
// ● minEndTime：如果当前正值该相位状态，则此数值表示当前状态距现在的最短结束时间；
// 如果信号灯相位并未处在该状态，则此数值表示现在距下一个该相位状态的最短时长；
// ● maxEndTime：如果当前正值该相位状态，则此数值表示当前状态距现在的最大结束时间；
// 如果信号灯相位并未处在该状态，则此数值表示现在距下一个该相位状态的最大时长。
// ● likelyEndTime：如果信号灯当前相位是固定时长，则该数值表示当前状态距现在的准确结束
// 时间。如果信号灯当前相位是非固定配时（感应配时、手动控制等），则该数值表示预测
// 的结束时间，且预测时间必须在minEndTime 和maxEndTime 之间，可能由历史数据或一些
// 事件触发等来进行预测。如果信号灯相位并没有处在该状态，则此数值表示下一个该相位
// 状态的（固定或预测）时长；
class CDF_TimeChangeDetails : public CCSAEMsgBase
{
    public:
        CDF_TimeChangeDetails();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(DE_TimeMark, DE_TimeMark, DE_TimeMark = DE_TimeMark_MAX + 1, 
                                DE_TimeMark = DE_TimeMark_MAX + 1,  DE_Confidence = DE_Confidence_MAX + 1, 
                                DE_TimeMark = DE_TimeMark_MAX + 1, DE_TimeMark = DE_TimeMark_MAX + 1);
        bool                GetParameter(DE_TimeMark&, DE_TimeMark&, DE_TimeMark&, DE_TimeMark&, 
                                DE_Confidence&, DE_TimeMark&, DE_TimeMark&);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

        void                SetParameter(CDF_TimeChangeDetails*);

    protected:
        DE_TimeMark         m_StartTime;
        DE_TimeMark         m_MinEndTime;                               // 可选参数，
        DE_TimeMark         m_MaxEndTime;                               // 可选参数，
        DE_TimeMark         m_LikelyEndTime;
        DE_Confidence       m_TimeConfidence;                           // 可选参数，
        DE_TimeMark         m_NextStartTime;                            // 可选参数，
        DE_TimeMark         m_NextDuration;                             // 可选参数，
};

// 定义信号灯的一个相位状态，包括该状态对应的灯色，以及实时计时信息
class CDF_PhaseState : public CCSAEMsgBase
{
    public:
        CDF_PhaseState();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(DE_LightState LightState, CDF_TimeChangeDetails* pTiming = NULL);
        bool                GetParameter(DE_LightState &LightState, CDF_TimeChangeDetails* pTiming = NULL);
        void                SetParameter(CDF_PhaseState*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        DE_LightState           m_LightState;
        CDF_TimeChangeDetails   m_Timing;                               // 可选参数
};

// 定义一个信号灯的一个相位中的相位状态列表。
// 列表中每一个相位状态物理上对应一种相位灯色，其属性包括了该状态的实时计时信息。
Define_List_Class(CDF_PhaseStateList)

// 定义信号灯相位。一个相位包括一个相位ID以及一个相位状态列表
class CDF_Phase : public CCSAEMsgBase
{
    public:
        CDF_Phase();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(DE_PhaseID phaseID, CDF_PhaseStateList* pList = NULL);
        bool                GetParameter(DE_PhaseID &phaseID, CDF_PhaseStateList* pList = NULL);

        void                SetParameter(CDF_Phase*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        DE_PhaseID          m_PhaseID;
        CDF_PhaseStateList  m_PhaseStatesList;
};

// 定义一组信号灯包含的所以相位的列表
Define_List_Class(CDF_PhaseList)


// 定义限速属性。包括限速类型以及所规定的限速值。
class CDF_RegulatorySpeedLimit : public CCSAEMsgBase
{
    public:
        CDF_RegulatorySpeedLimit();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(DE_SpeedLimitType speedLimitType, DE_Speed speed);
        bool                GetParameter(DE_SpeedLimitType &speedLimitType, DE_Speed &speed);

        void                SetParameter(CDF_RegulatorySpeedLimit*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        DE_SpeedLimitType       m_SpeedLimitType;
        DE_Speed                m_Speed;
};

// 定义路段或车道的限速列表
Define_List_Class(CDF_SpeedLimitList)


class CDF_PositionLLBase : public CCSAEMsgBase
{
    public:
        CDF_PositionLLBase();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(int longtitude, int latitude);
        bool                GetParameter(int &longtitude, int &latitude);

        void                SetParameter(CDF_PositionLLBase*);

        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        void                SetParameterRange();
        int                 m_Longtitude;
        int                 m_Latitude;

        int                 m_nLongtitudeMin, m_nLongtitudeMax;         // 参数的最大最小值，由基类根据派生类的类名自动判断
        int                 m_nLatitudeMin, m_nLatitudeMax;
};

// 不需要新增新的成员函数和成员变量
#define Define_Offset_Class(class_name) \
class class_name : public CDF_PositionLLBase \
{ \
    public: \
        class_name() { SetParameterRange(); }\
};

// 24 比特相对经纬度位置，表示当前位置点关于参考位置点的经纬度偏差。由两个12 比特的经度、纬度偏差值组成。
Define_Offset_Class(CDF_PositionLL24B)

// 28 比特相对经纬度位置，表示当前位置点关于参考位置点的经纬度偏差。由两个14 比特的经度、纬度偏差值组成。
Define_Offset_Class(CDF_PositionLL28B)

// 32 比特相对经纬度位置，表示当前位置点关于参考位置点的经纬度偏差。由两个16 比特的经度、纬度偏差值组成。
Define_Offset_Class(CDF_PositionLL32B)

// 36 比特相对经纬度位置，表示当前位置点关于参考位置点的经纬度偏差。由两个18 比特的经度、纬度偏差值组成。
Define_Offset_Class(CDF_PositionLL36B)


// 44 比特相对经纬度位置，表示当前位置点关于参考位置点的经纬度偏差。由两个22 比特的经度、纬度偏差值组成。
Define_Offset_Class(CDF_PositionLL44B)

// 48 比特相对经纬度位置，表示当前位置点关于参考位置点的经纬度偏差。由两个24 比特的经度、纬度偏差值组成。
Define_Offset_Class(CDF_PositionLL48B)

// 64 比特经纬度位置，该尺度采用了真实经纬度数据进行描述，非相对位置
Define_Offset_Class(CDF_PositionLLmD_64B)

// 经纬度偏差，用来描述一个坐标点的相对位置，提供了7种初度的描述方式
// CDF_PositionLL24B、CDF_PositionLL28B、CDF_PositionLL32B、CDF_PositionLL36B、CDF_PositionLL44B、CDF_PositionLL48B、CDF_PositionLLmD_64B;

class CDF_PositionOffsetLL : public CCSAEMsgBase
{
    public:
        CDF_PositionOffsetLL();
        ~CDF_PositionOffsetLL();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        CDF_PositionLLBase* GetObject();
        void                SetObject(CDF_PositionLLBase* pOffsetObject);

        void                SetParameter(CDF_PositionOffsetLL* pPosOffset);

        virtual void        Test();
        virtual void        toParamString();

    protected:
        int                 m_nChoiceIndex;
        CDF_PositionLLBase* m_pOffsetOject;
};

// 定义垂直方向的位置偏差，提供了7种尺度范围的偏差表示方式
class CDF_VerticalOffset : public CCSAEMsgBase
{
    public:
        CDF_VerticalOffset();
    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(int nChoiceIndex, int nVerticalOffset);
        bool                GetParameter(int &nChoiceIndex, int &nVerticalOffset);

        void                SetParameter(CDF_VerticalOffset*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        int                 m_nChoiceIndex;                             // 选择项的索引值
        int                 m_nVerticalOffset;                          // 选择项值
};

// 定义三维的相对位置（相对经纬度和相对高程）。
class CDF_PositionOffsetLLV : public CCSAEMsgBase
{
    public:
        CDF_PositionOffsetLLV();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(CDF_PositionOffsetLL*, CDF_VerticalOffset* = NULL );
        bool                GetParameter(CDF_PositionOffsetLL*, CDF_VerticalOffset* = NULL);

        void                SetParameter(CDF_PositionOffsetLLV*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        CDF_PositionOffsetLL    m_OffsetLL;
        CDF_VerticalOffset      m_VerticalOffset;                       // 可选参数
};

// 表示完整道路上标记的一个位置点属性。包括相对位置、位置点处路段截面的特殊属性集合，
// 以及该位置点到下游相邻位置点之间局部路段的特殊属性集合。
// 如果位置点为路段最下游的点，则其包含的下游截取路段属性为无效数据。
class CDF_RoadPoint : public CCSAEMsgBase
{
    public:
        CDF_RoadPoint() {};

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(CDF_PositionOffsetLLV*);
        bool                GetParameter(CDF_PositionOffsetLLV*);

        virtual void        Test();
        virtual void        toParamString();    

    protected:
        CDF_PositionOffsetLLV   m_PositionOffset;
};

// 定义一个有向路段上的中间位置点列表，用来表达路段上截面或片段的特殊属性。列表中所有位置点需按上游至下游顺序排列。
Define_List_Class(CDF_PointList)

// 车辆四轴加速度 
class CDF_AccelerationSet4Way : public CCSAEMsgBase
{
    public:
        CDF_AccelerationSet4Way();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(DE_Acceleration, DE_Acceleration, DE_VerticalAcceleration, DE_YawRate);
        bool                GetParameter(DE_Acceleration&, DE_Acceleration&, DE_VerticalAcceleration&, DE_YawRate&);

        bool                SetParameter(CDF_AccelerationSet4Way*);

        virtual void        Test();
        virtual void        toParamString();    

    protected:
        DE_Acceleration             m_LongAcc;                          // 纵向加速度，向前加速为正，反向为负
        DE_Acceleration             m_LatAcc;                           // 横向加速度，向右加速为正，反向为负
        DE_VerticalAcceleration     m_VertAcc;                          // 垂直加速度，沿重力反向为正，反向为负
        DE_YawRate                  m_YawRate;                          // 横摆角速度，顺时针为正，反向为负
};

// 可选参数的索引值
#define BRAKE_PEDAL_STATUS              0
#define BRAKE_APPLIED_STATUS            1
#define TRACTION_CONTROL_STATUS         2
#define ANTILOCK_BRAKE_STATUS           3
#define STABILITY_CONTROL_STATUS        4
#define BRAKE_BOOST_APPLIED             5
#define AUXILIARY_BRAKE_STATUS          6

// 定义车辆的刹车系统状态，包括7种状态
// 这7种状态都是可选参数
class CDF_BrakeSystemStatus : public CCSAEMsgBase
{
    public:
        CDF_BrakeSystemStatus();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        // 7个可选参数，分别设置其可行性的值
        bool                SetParameter(DE_BrakePedalStatus);
        bool                SetParameter(UCHAR);
        bool                SetParameter(DE_TractionControlStatus);
        bool                SetParameter(DE_AntiLockBrakeStatus);
        bool                SetParameter(DE_StabilityControlStatus);
        bool                SetParameter(DE_BrakeBoostApplied);
        bool                SetParameter(DE_AuxiliaryBrakeStatus);
        
        bool                GetParameter(DE_BrakePedalStatus&);
        bool                GetParameter(UCHAR&);
        bool                GetParameter(DE_TractionControlStatus&);
        bool                GetParameter(DE_AntiLockBrakeStatus&);
        bool                GetParameter(DE_StabilityControlStatus&);
        bool                GetParameter(DE_BrakeBoostApplied&);
        bool                GetParameter(DE_AuxiliaryBrakeStatus&);

        bool                SetParameter(CDF_BrakeSystemStatus*);

        virtual void        Test();
        virtual void        toParamString();    

    protected:
        DE_BrakePedalStatus         m_BrakePadelStatus;
        UCHAR                       m_BrakeAppliedStatus;               // 定长比特串，使用UCHAR定义变量
        DE_TractionControlStatus    m_TractionControlStatus;
        DE_AntiLockBrakeStatus      m_ABS;
        DE_StabilityControlStatus   m_StabilityControlStatus;
        DE_BrakeBoostApplied        m_BrakeBoostApplied;
        DE_AuxiliaryBrakeStatus     m_AuxBrakesStatus;
};

// 用于定位上游车道转向连接的下游车道，包括车道ID以及该转向的允许行驶行为。
// 下游车道ID的作用范围是其所在的路段。
class CDF_ConnectingLane : public CCSAEMsgBase
{
    public:
        CDF_ConnectingLane();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(DE_LaneID, USHORT = DE_AllowedManeuvers_Invalid);
        bool                GetParameter(DE_LaneID&, USHORT&);

        bool                SetParameter(CDF_ConnectingLane*);

        virtual void        Test();
        virtual void        toParamString();    

    protected:
        DE_LaneID           m_LaneID;
        USHORT              m_AllowedManeuvers;                         // 可选参数，定长比特串
};

// 定义车道与下游路段车道的连接关系。包括下游路段出口节点ID、连接的下游路段车道基本信息及对应的信号灯相位号。
// 在车道连接中定义的相位还，是对DF_Movement中定义转向的相位号的一个补充。
// 当某一些车道在转向时需要参考一些特殊的信号灯相位（而不是DF_Movement中定义的默认相位），
// 则一个将其定义在本数据帧中。
class CDF_Connection : public CCSAEMsgBase
{
    public:
        CDF_Connection();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(CDF_NodeReferenceID*, CDF_ConnectingLane* = NULL, DE_PhaseID = DE_PhaseID_MAX + 1);
        bool                GetParameter(CDF_NodeReferenceID*, CDF_ConnectingLane* = NULL, DE_PhaseID = DE_PhaseID_MAX + 1);

        bool                SetParameter(CDF_Connection*);

        virtual void        Test();
        virtual void        toParamString();    

    protected:
        CDF_NodeReferenceID m_NodeReferenceID;
        CDF_ConnectingLane  m_ConnectingLane;                           // 可选参数
        DE_PhaseID          m_PhaseID;                                  // 可选参数
};

// 定义路段中每条车道在下游路口处与下游路段车道的转向连接关系列表。
Define_List_Class(CDF_ConnectsToList)

// 可选参数的索引值
#define DATETIME_YEAR                   0
#define DATETIME_MONTH                  1
#define DATETIME_DAY                    2
#define DATETIME_HOUR                   3
#define DATETIME_MINUTE                 4
#define DATETIME_SECOND                 5
#define DATETIME_TIME_OFFSET            6

// 定义完整的日期和时间数据单元
// 所有参数均为可选参数
class CDF_DDateTime : public CCSAEMsgBase
{
    public:
        CDF_DDateTime();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(DE_DYear = 0, DE_DMonth = 0, DE_DDay = 0, DE_DHour = 0, 
                                DE_DMinute = 0, DE_DSecond = 0, DE_DTimeOffset = 0);
        bool                GetParameter(DE_DYear&, DE_DMonth&, DE_DDay&, DE_DHour&, DE_DMinute&, 
                                DE_DSecond&, DE_DTimeOffset&);

        bool                SetParameter(CDF_DDateTime*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        DE_DYear            m_Year;
        DE_DMonth           m_Month;
        DE_DDay             m_Day;
        DE_DHour            m_Hour;
        DE_DMinute          m_Minute;
        DE_DSecond          m_Second;
        DE_DTimeOffset      m_TimeOffset;
};

// 可选参数的索引值
#define FPV_DATETIME                    0
#define FPV_POSTION3D                   1
#define FPV_HEADING                     2
#define FPV_TRANS_STATE                 3
#define FPV_SPEED                       4
#define FPV_POS_ACCURACY                5
#define FPV_TIME_CFD                    6
#define FPV_POS_CFD_SET                 7
#define FPV_MOTION_CFD_SET              8

// 定义完整的参考轨迹点信息。用于车辆历史轨迹数据单元，作为一串轨迹点的参考点数据。
// 所有参数为可选参数
class CDF_FullPositionVector : public CCSAEMsgBase
{
    public:
        CDF_FullPositionVector();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(CDF_DDateTime* = NULL, CDF_Position3D* = NULL, DE_Heading = DE_Heading_MAX + 1, 
                                DE_TransmissionState = DE_TransmissionState_Invalid, DE_Speed = DE_Speed_MAX + 1, 
                                CDF_PositionConfidenceSet* = NULL, DE_TimeConfidence = DE_TimeConfidence_Invalid, 
                                CDF_PositionConfidenceSet* = NULL, CDF_MotionConfidenceSet* = NULL);
        bool                GetParameter(CDF_DDateTime*, CDF_Position3D*, DE_Heading&, DE_TransmissionState&, DE_Speed&, 
                                CDF_PositionConfidenceSet*, DE_TimeConfidence&, CDF_PositionConfidenceSet*, CDF_MotionConfidenceSet*);
        
        bool                SetParameter(CDF_FullPositionVector*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        CDF_DDateTime               m_DateTime;
        CDF_Position3D              m_Position3D;
        DE_Heading                  m_Heading;
        DE_TransmissionState        m_TransissionState;
        DE_Speed                    m_Speed;
        CDF_PositionConfidenceSet   m_PositionAccuracy;
        DE_TimeConfidence           m_TimeConfidence;
        CDF_PositionConfidenceSet   m_PositionConfidenceSet;
        CDF_MotionConfidenceSet     m_MotionCfdSet;
};

// 定义一个路口信号灯的属性和当前状态。包括路口ID、信号灯工作状态、时间戳以及信号灯的相位列表。
class CDF_IntersectionState : public CCSAEMsgBase
{
    public:
        CDF_IntersectionState();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(CDF_NodeReferenceID*, USHORT, CDF_PhaseList*, 
                                DE_MinuteOfTheYear = DE_MinuteOfTheYear_MAX + 1, DE_DSecond = DE_DSecond_MAX + 1);
        bool                GetParameter(CDF_NodeReferenceID*, USHORT&, CDF_PhaseList*, DE_MinuteOfTheYear&, DE_DSecond& );
        bool                SetParameter(CDF_IntersectionState*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        CDF_NodeReferenceID m_InterSectionID;
        USHORT              m_IntersectionStatusObject;                 // 16位定长比特串
        DE_MinuteOfTheYear  m_MinuteOfTheYear;
        DE_DSecond          m_TimeStamp;
        CDF_PhaseList       m_PhaseList;
};

// 定义一个路口信号灯集合。
Define_List_Class(CDF_IntersectionStateList)

// 选择项的索引值
#define LANE_ATTR_VEHICLE               0
#define LANE_ATTR_CROSSWALK             1
#define LANE_ATTR_BIKE                  2
#define LANE_ATTR_SIDEWALK              3
#define LANE_ATTR_BARRIER               4
#define LANE_ATTR_STRIPPING             5
#define LANE_ATTR_TRACKED_VEHICLE       6
#define LANE_ATTR_PARKING               7

// 定义不同类别车道的属性集合
class CDF_LaneTypeAttributes : public CCSAEMsgBase
{
    public:
        CDF_LaneTypeAttributes();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(int nChoiceIndex, UINT attr, USHORT nBitLen = 16);
        bool                GetParameter(int &nChoiceIndex, UINT &attr, USHORT &nBitLen );
        bool                SetParameter(CDF_LaneTypeAttributes*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        int                 m_nChoiceIndex;
        UINT                m_LaneAttributes;
        USHORT              m_nAttrBitLen;
};

// 定义车道属性，包括车道共享情况及车道本身所属的类别特性
class CDF_LaneAttributes : public CCSAEMsgBase
{
    public:
        CDF_LaneAttributes();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(CDF_LaneTypeAttributes*, USHORT = DE_LaneSharing_Invalid );
        bool                GetParameter(CDF_LaneTypeAttributes*, USHORT&);
        bool                SetParameter(CDF_LaneAttributes*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        USHORT              m_ShareWith;                            // 可选参数
        CDF_LaneTypeAttributes  m_LaneTypeAttr;
};

// 可选参数的索引值
#define LANE_LANE_ATTR                  0
#define LANE_MANEUVERS                  1
#define LANE_CONNECTS_TOLIST            2
#define LANE_SPEED_LIMIT_LIST           3
#define LANE_POINT_LIST                 4

// 定义车道。包含其ID、共享属性、车道出口的允许转向行为、车道与下游路段车道的连接关系列表，以及车道中间点列表。
class CDF_Lane : public CCSAEMsgBase
{
    public:
        CDF_Lane();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(DE_LaneID, USHORT = DE_AllowedManeuvers_Invalid, CDF_LaneAttributes* = NULL, 
                                CDF_ConnectsToList* = NULL, CDF_SpeedLimitList* = NULL, CDF_PointList* = NULL);
        bool                GetParameter(DE_LaneID&, USHORT&, CDF_LaneAttributes*, CDF_ConnectsToList* = NULL, 
                                CDF_SpeedLimitList* = NULL, CDF_PointList* = NULL);
        bool                SetParameter(CDF_Lane*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

        CDF_ConnectsToList* GetConnectsToList() { return &m_ConnectsToList; }
        CDF_SpeedLimitList* GetSpeedLimitList() { return &m_SpeedLimitList; }
        CDF_PointList*      GetPointList() { return &m_PointList; }

    protected:
        DE_LaneID           m_LaneID;
        CDF_LaneAttributes  m_LaneAttributes;
        USHORT              m_AllowedManeuvers;                         // 定长比特串
        CDF_ConnectsToList  m_ConnectsToList;
        CDF_SpeedLimitList  m_SpeedLimitList;
        CDF_PointList       m_PointList;
};

// 定义一个路段中包含的车道列表
Define_List_Class(CDF_LaneList)

// 描述道路与下游路段的连接关系，以及该连接对应的本地路口处信号灯相位ID。
// 此处的相位ID事实上也是MAP消息与SPAT消息的唯一关联。车辆在确定转向行为后，
// 能够通过该相位ID数据，查看到SPAT中对应的相位实时状态，从而获得行车过程中的信号灯数据辅助。
class CDF_Movement : public CCSAEMsgBase
{
    public:
        CDF_Movement();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(CDF_NodeReferenceID *, DE_PhaseID = DE_PhaseID_MAX + 1 );
        bool                GetParameter(CDF_NodeReferenceID *, DE_PhaseID &);
        bool                SetParameter(CDF_Movement*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        CDF_NodeReferenceID     m_RemoteIntersection;
        DE_PhaseID              m_PhaseID;
};

// 描述一条路段与下游路段的连接关系列表
Define_List_Class(CDF_MovementList)


// 定义路段，从一个节点到相邻另一个节点的道路称为一个有向路段。
// 其属性包含名称、上游节点ID、限速集合、车道宽度，以及该路段包含的车道集合与交通标志标牌集合。
class CDF_Link : public CCSAEMsgBase
{
    public:
        CDF_Link();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(CDF_NodeReferenceID *, DE_LaneWidth, CDF_LaneList*, DE_DescriptiveName = "", 
                                CDF_SpeedLimitList* = NULL, CDF_PointList* = NULL, CDF_MovementList* = NULL);
        bool                GetParameter(CDF_NodeReferenceID *, DE_LaneWidth&, CDF_LaneList*, DE_DescriptiveName&, 
                                CDF_SpeedLimitList* = NULL, CDF_PointList* = NULL, CDF_MovementList* = NULL);
        bool                SetParameter(CDF_Link*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        DE_DescriptiveName      m_Name;
        CDF_NodeReferenceID     m_UpstreamNodeID;
        CDF_SpeedLimitList      m_SpeedLimitList;
        DE_LaneWidth            m_LaneWidth;
        CDF_PointList           m_PointList;
        CDF_MovementList        m_MovementList;
        CDF_LaneList            m_LaneList;
};

// 定义路段列表
Define_List_Class(CDF_LinkList)


// 地图节点。节点是地图的最基本组成部分，可以是交叉路口，也可以是一个路段的端点。
// 在地图上，相邻且有序的两个节点确定一条有向路径。
// 节点属性包括名称、ID、位置以及与节点相邻的上游路段集合。
// 节点数据帧中包含的路段列表，均以该节点作为下游端点。而以该节点为上游端点的路段，则归属于该路段下游节点的数据帧
class CDF_Node : public CCSAEMsgBase
{
    public:
        CDF_Node();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(CDF_NodeReferenceID *, CDF_Position3D*, DE_DescriptiveName = "", CDF_LinkList* = NULL);
        bool                GetParameter(CDF_NodeReferenceID *, CDF_Position3D*, DE_DescriptiveName&, CDF_LinkList* = NULL);
        bool                SetParameter(CDF_Node*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        DE_DescriptiveName      m_Name;
        CDF_NodeReferenceID     m_NodeReferenceID;
        CDF_Position3D          m_RefPosition;
        CDF_LinkList            m_InLink;
};

// 定义地图节点列表
Define_List_Class(CDF_NodeList)

// 定义车辆的分类，可以从各个维度对车辆类型进行定义。
// 目前仅有车辆基本类型一项
class CDF_VehicleClassification : public CCSAEMsgBase
{
    public:
        CDF_VehicleClassification();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(DE_BasicVehcleClass);
        bool                GetParameter(DE_BasicVehcleClass&);
        bool                SetParameter(CDF_VehicleClassification*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        DE_BasicVehcleClass m_VehicleClass;
};

// 定义车辆的尺寸大小，由车辆长宽高三个维度就·来定义，其中高度数值为可选项
class CDF_VehicleSize : public CCSAEMsgBase
{
    public:
        CDF_VehicleSize();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(DE_VehicleWidth, DE_VehicleLength, DE_VehicleHeight = DE_VehicleHeight_MAX + 1);
        bool                GetParameter(DE_VehicleWidth&, DE_VehicleLength&, DE_VehicleHeight&);
        bool                SetParameter(CDF_VehicleSize*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        DE_VehicleWidth     m_Width;
        DE_VehicleLength    m_Length;
        DE_VehicleHeight    m_Height;
};

// 定义交通参与者的基本安全信息。对应BSM 消息，该参与者信息由RSU 探测得到。
class CDF_ParticipantData : public CCSAEMsgBase
{
    public:
        CDF_ParticipantData();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        // 因为参数过多，暂时设置2个函数，分别设置必选参数和可选参数
        bool                SetParameter(DE_PartcipantType, int ptcID, DE_SourceType, DE_DSecond, CDF_PositionOffsetLLV*, 
                                CDF_PositionConfidenceSet*, DE_Speed, DE_Heading, CDF_VehicleSize*);
        bool                SetParameter(CString strRSUID = "", CString strPlateNo = "", DE_TransmissionState = DE_TransmissionState_Invalid, 
                                DE_SteeringWheelAngle = DE_SteeringWheelAngle_MAX + 1 , CDF_MotionConfidenceSet* = NULL, 
                                CDF_AccelerationSet4Way* = NULL, CDF_VehicleClassification* = NULL);
        
        bool                GetParameter(DE_PartcipantType&, int &ptcID, DE_SourceType&, DE_DSecond&, CDF_PositionOffsetLLV*, 
                                CDF_PositionConfidenceSet*, DE_Speed&, DE_Heading&, CDF_VehicleSize*);
        bool                GetParameter(CString &strRSUID, CString &strPlateNo, DE_TransmissionState&, DE_SteeringWheelAngle&, 
                                CDF_MotionConfidenceSet* = NULL, CDF_AccelerationSet4Way* = NULL, CDF_VehicleClassification* = NULL);
        
        bool                SetParameter(CDF_ParticipantData*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        DE_PartcipantType           m_ParticipantType;
        int                         m_ParticipantID;                    // 由RSU分配的临时ID，0为RSU
        DE_SourceType               m_SourceType;

        CString                     m_strRSUID;                         // id OCTET STRING (SIZE(8)) OPTIONAL,
        CString                     m_strPlateNo;                       // plateNo OCTET STRING (SIZE(16)) OPTIONAL,

        DE_DSecond                  m_secMark;
        CDF_PositionOffsetLLV       m_Pos;
        CDF_PositionConfidenceSet   m_Accuracy;
        DE_TransmissionState        m_TransissionState;
        DE_Speed                    m_Speed;
        DE_Heading                  m_Heading;

        DE_SteeringWheelAngle       m_SteeringWheelAngle;
        CDF_MotionConfidenceSet     m_MotionCfdSet;
        CDF_AccelerationSet4Way     m_AccelerationSet4Way;
        CDF_VehicleSize             m_VehicleSize;
        CDF_VehicleClassification   m_VehicleClass;
};

// 定义交通参与者列表。应用于RSM消息中，表示所有或部分RSU当前探测到的交通参与者信息。
Define_List_Class(CDF_ParticipantList)


// 定义车辆的历史轨迹点，包括位置、时间戳，以及轨迹点处的车速、位置精度和航向等
class CDF_PathHistoryPoint : public CCSAEMsgBase
{
    public:
        CDF_PathHistoryPoint();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(CDF_PositionOffsetLLV* pPos, DE_TimeOffset, DE_Speed = DE_Speed_MAX + 1, 
                                CDF_PositionConfidenceSet* = NULL, DE_CoarseHeading = DE_CoarseHeading_MAX + 1);
        bool                GetParameter(CDF_PositionOffsetLLV* pPos, DE_TimeOffset&, DE_Speed&, CDF_PositionConfidenceSet*, DE_CoarseHeading&);
        bool                SetParameter(CDF_PathHistoryPoint*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        CDF_PositionOffsetLLV       m_PositionOffsetLLV;
        DE_TimeOffset               m_TimeOffset;
        DE_Speed                    m_Speed;
        CDF_PositionConfidenceSet   m_PositionConfidenceSet;
        DE_CoarseHeading            m_Heading;
};

// 定义车辆的历史轨迹点列表
Define_List_Class(CDF_PathHistoryPointList)


// 定义车辆历史轨迹
// 利用一个参考轨迹点信息，以及一系列基于该信息的历史轨迹点，给出车辆一段完整的历史轨迹。
// 车辆历史轨迹能够真实地反映其在当时的行驶状态，且从侧面反映当时其所在的道路和交通环境。
// 数据单元中的参考轨道点信息在时间上比所有轨迹点都要靠后（时间较晚）。
class CDF_PathHistory : public CCSAEMsgBase
{
    public: 
        CDF_PathHistory();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(CDF_PathHistoryPointList* pList, UCHAR = DE_GNSSstatus_Invalid, CDF_FullPositionVector* = NULL);
        bool                GetParameter(CDF_PathHistoryPointList* pList, UCHAR&, CDF_FullPositionVector* = NULL);
        bool                SetParameter(CDF_PathHistory*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        CDF_FullPositionVector      m_InitialPosition;
        UCHAR                       m_currGNSSStatus;
        CDF_PathHistoryPointList    m_crumbData;
};

// 定义RSA消息中，消息作用范围的车辆行进轨迹路段。
// 该区段由有序位置点列组成，排列顺序与车辆行进方向一致。
Define_List_Class(CDF_PathPointList)


// 定义车辆的预测线路，主要是预测线路的曲率半径
class CDF_PathPrediction : public CCSAEMsgBase
{
    public:
        CDF_PathPrediction();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(DE_RadiusOfCurvation, DE_Confidence);
        bool                GetParameter(DE_RadiusOfCurvation&, DE_Confidence&);
        bool                SetParameter(CDF_PathPrediction*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        DE_RadiusOfCurvation        m_RadiusOfCurve;        // 精度：10cm
        DE_Confidence               m_Confidence;
};

// 定义一个路口信号灯的属性和当前状态。包括其当前工作模式、当前时间戳，以及实时的相位信息列表。
class CDF_SignalState : public CCSAEMsgBase
{
    public:
        CDF_SignalState();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(USHORT, CDF_PhaseList*, DE_MinuteOfTheYear moy = DE_MinuteOfTheYear_MAX + 1, 
                                DE_DSecond timeStamp = DE_DSecond_MAX + 1);
        bool                GetParameter(USHORT&, CDF_PhaseList*, DE_MinuteOfTheYear &moy, DE_DSecond &timeStamp);
        bool                SetParameter(CDF_SignalState*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        USHORT              m_Status;
        DE_MinuteOfTheYear  m_MinuteOfTheYear;
        DE_DSecond          m_TimeStamp;
        CDF_PhaseList       m_PhaseList;
};

// 定义车辆安全辅助信息集合，用于BSM消息中，作为基础安全数据的补充。
// 包括车辆特殊事件状态，车辆历史轨迹、线路预测、车身灯光状态等。
// 各辅助信息均为可选项
class CDF_VehicleSafetyExtensions : public CCSAEMsgBase
{
    public:
        CDF_VehicleSafetyExtensions();

    public:
        virtual USHORT      Encode();
        virtual USHORT      Decode(PUCHAR pData, USHORT nBitLen, USHORT nStartBitPos = 0);

        bool                SetParameter(UINT = DE_VehicleEventFlags_Invalid, USHORT = DE_VehicleEventFlags_MIN_LEN, 
                                CDF_PathHistory* = NULL, CDF_PathPrediction* = NULL, UINT = DE_ExteriorLights_Invalid, 
                                USHORT = DE_ExteriorLights_MIN_LEN);
        bool                GetParameter(UINT&, USHORT&, CDF_PathHistory*, CDF_PathPrediction*, UINT&, USHORT&);
        bool                SetParameter(CDF_VehicleSafetyExtensions*);

        virtual void        Test();
        virtual void        toParamString();                            // 将参数值输出到字符串中

    protected:
        UINT                m_Events;                                   // 不定长比特串
        CDF_PathHistory     m_PathHistory;
        CDF_PathPrediction  m_PathPrediction;
        UINT                m_Lights;                                   // 不定长比特串

        USHORT              m_nEventBitLen;
        USHORT              m_nLightBitLen;
};

#endif // _CSAE_DATA_FRAME_H_
