/*------------------------------------------------------------------------------
 * Common - Component :: T/CSAE 53-2017 Specification
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    CSAEDataElement.h
 * Purpose: T/CSAE 53-2017 Specification data element definition
 *----------------------------------------------------------------------------*/
#ifndef _CSAE_DATA_ELEMENT_H_
#define _CSAE_DATA_ELEMENT_H_

#include "CString.h"

// 对象的创建和删除
// 谁申请，谁删除。
// 在List或其他使用对象指针的地方，不是由自身创建的对象，都不需要删除该对象。
// 在使用的时候，需要注意该对象是否还存在

#include "ASN1PERCoding.h"
#include "ObjectFactory.h"

// 基础应用定义
enum RSU_TYPE
{
    // RSU_MSG_TYPE_BASE = 0x0300,

    RSU_SCENE_V2V_FCW,                                                  // 前向碰撞预警，Forward Collision Warning
    RSU_SCENE_V2VI_ICW,                                                 // 交叉路口碰撞预警，Intersection Collision Warning
    RSU_SCENE_V2VI_LTA,                                                 // 左转辅助，Left Turn Assist
    RSU_SCENE_V2V_BSW_LCW,                                              // 盲区预警及变道预警，Bland Spot Warning/Lane Change Warning
    RSU_SCENE_V2V_DNPW,                                                 // 逆向超车预警, Do Not Pass Warning
    RSU_SCENE_V2V_EBW,                                                  // 紧急制动预警, Emergency Brake Warning
    RSU_SCENE_V2V_AVW,                                                  // 异常车辆提醒，Abormal Vehicle Warning
    RSU_SCENE_V2V_CLW,                                                  // 车辆失控预警，Control Loss Warning
    RSU_SCENE_V2I_HLW,                                                  // 道路危险状况提示，Hazardous Location Warning
    RSU_SCENE_V2I_SLW,                                                  // 限速预警， Speed Limit warning
    RSU_SCENE_V2I_RLVW,                                                 // 闯红灯预警，Red Loght Violation Warning
    RSU_SCENE_V2P_VRUCW,                                                // 弱势交通参与者碰撞预警，Vulnerable Road User Collision Warning
    RSU_SCENE_V2I_GLOSA,                                                // 绿波车速引导，Green Light Optimal Speed Advisory
    RSU_SCENE_V2I_IVS,                                                  // 车内标牌，In-Vihicle Signage
    RSU_SCENE_V2I_TJW,                                                  // 前方拥堵提醒，Traffic Jam Warning
    RSU_SCENE_V2V_EVE,                                                  // 紧急车辆提醒，Emergency Vehicle Warning
    RSU_SCENE_V2I_VNFP,                                                 // 汽车近场支付，Vehicle NearField Payment
};


/////////////////////////////////////////////////////////////////////////////////////////////////
// 数据集定义，遵循标准：
// 中国汽车工程学会标准《合作式智能运输系统 车用通信系统应用层及应用数据交互标准 T/CSAE 53-2017》
// 数据集交互的编解码方式遵循非对齐压缩编码规则UPER（Unaligned Packet Encoding Rules）


// 数据取值范围，例如0~127，表示为[0, 127]，包含边界值
// 所有枚举值都不小于0，设定为UINT类型，
// 所有BitIndex及BitLen，都使用UCHAR类型，取值范围0~255

// 一般说来序列编码后都会产生一个前导位图用以指示序列中的可选项或缺省项是否
// 存在。每以可选项或缺省项用 1bit 来指示“1”表示存在“0”表示不存在。若一
// 个序列型中包含 n 个标注为可选或缺省的成员那么前导位图的长度就是 n bit。位
// 图中的比特顺序与序列中各可选或缺省的成员排列一致。然后再对 SEQUENCE 中的各
// 成员进行编码

// 对于Sequence类型的可选项，采用UINT类型作为前导位图标志，每一位来确定可选项是否存在
// 可选项序号从0开始

#define MAX_OPTION_COUNT                16                              // 在规范中，最大可选项数量

#define PLATE_NO_LEN                    16
#define VEHICLE_ID_LEN                  8
#define RSU_ID_LEN                      8


/////////////////////////////////////////////////////////////////////////////////////////////////
// 数据元素定义

// 车辆加速度，分辨率为 0.01m/s2，取值范围-2000~2000，2001为无效值
// 即：值为1的时候，表示加速度为 1*0.01m/s2
#define DE_Acceleration_MAX             2000
#define DE_Acceleration_MIN             -2000
typedef int DE_Acceleration;

// 定义路侧警示信息RSA的类型，数值0表示文本描述信息，大于0的数值表示交通标志标牌信息
// 内容参照GB5768
#define DE_AlertType_MAX               65535
#define DE_AlertType_MIN               0
typedef UINT DE_AlertType;

#define DE_ALERT_TYPE_TEXT_MESSAGE          0
#define DE_ALERT_TYPE_DANGER                37
#define DE_ALERT_TYPE_ROCK_FALL             15
#define DE_ALERT_TYPE_SHARP_TURN            2
#define DE_ALERT_TYPE_UNDER_CONSTRUCTION    38
#define DE_ALERT_TYPE_SLIPPY                17
#define DE_ALERT_TYPE_TUNNEL                21


// 定义一个机动车车道的允许转向行为
// 比特串，按位定义，共12位，定长比特串
// 使用USHORT定义变量
#define DE_AllowedManeuvers_LEN          12
enum DE_AllowedManeuvers
{
    DE_AM_StraightAllowed       = 0b00000000000,
    DE_AM_LeftAllowed           = 0b00000000001,
    DE_AM_RightAllowed          = 0b00000000010,
    DE_AM_UturnAllowed          = 0b00000000100,
    DE_AM_LeftTurnOnRedAllowd   = 0b00000001000,
    DE_AM_RightTurnOnRedAllowd  = 0b00000010000,
    DE_AM_LaneChangedAllowd     = 0b00000100000,
    DE_AM_NoStoppingAllowd      = 0b00001000000,
    DE_AM_YieldAllwaysRequired  = 0b00010000000,
    DE_AM_GoWithHalt            = 0b00100000000,
    DE_AM_Caution               = 0b01000000000,
    DE_AM_Reserved1             = 0b10000000000,

    DE_AllowedManeuvers_Invalid,
};

// 定义刹车防抱死系统（ABS）状态。
// 枚举类型
#define DE_AntiLockBrakeStatus_COUNT    4
enum DE_AntiLockBrakeStatus
{
    DE_ABS_Unavailable          = 0,                                    // 未安装ABS系统，或ABS系统状态不可获取
    DE_ABS_Off                  = 1,                                    // ABS 关闭
    DE_ABS_On                   = 2,                                    // ABS 打开但是没有使用
    DE_ABS_Engaged              = 3,                                    // ABS 已经作用于所有车轮

    DE_AntiLockBrakeStatus_Invalid,
};

// 指示刹车辅助系统状态（通常为手刹）
// 枚举类型
#define DE_AuxiliaryBrakeStatus_COUNT   3
enum DE_AuxiliaryBrakeStatus
{
    DE_AuxBS_Unavailable        = 0,                                    // 未安装手刹，或手刹状态不可获取
    DE_AuxBS_Off                = 1,                                    // 刹车辅助系统关闭
    DE_AuxBS_On                 = 2,                                    // 刹车辅助系统使用中

    DE_AuxiliaryBrakeStatus_Invalid,
};

// 定义车辆基本类型
// 整数类型，取值范围： 0 ~ 255
#define DE_BasicVehcleClass_MAX         255
#define DE_BasicVehcleClass_MIN         0        
enum DE_BasicVehcleClass
{
    DE_BVC_Unknown                          = 0,                        // 未知类型，或不可获取
    DE_BVC_SpecialVehicleClass              = 1,

    // 基本乘用车类型
    DE_BVC_PassengerVehicleTypeUnknown      = 10,                       // 缺省类型
    DE_BVC_PassengerVehicleTypeOther        = 11,

    // 轻卡，皮卡，箱式货车
    DE_BVC_LightTruckTypeUnknown            = 20,                       // 缺省类型
    DE_BVC_LightTruckTypeOther              = 21,

    // 货车，多轴类型，包括 HPMS
    DE_BVC_TruckTypeUnknown                 = 25,                       // 缺省类型
    DE_BVC_TruckTypeOther                   = 26,
    DE_BVC_TruckAxleCnt2                    = 27,                       // 2轴， 6个轮胎，一个Unit
    DE_BVC_TruckAxleCnt3                    = 28,                       // 3轴，一个Unit
    DE_BVC_TruckAxleCnt4                    = 29,                       // 4轴或更多轴，一个Unit
    DE_BVC_TruckAxleCnt4Trailer             = 30,                       // 4轴或不到4轴，单拖车
    DE_BVC_TruckAxleCnt5Trailer             = 31,                       // 5轴或不到5轴，单拖车
    DE_BVC_TruckAxleCnt6Trailer             = 32,                       // 6轴或更多轴，单拖车
    DE_BVC_TruckAxleCnt5MultiTrailer        = 33,                       // 5轴或不到5轴，多拖车
    DE_BVC_TruckAxleCnt6MultiTrailer        = 34,                       // 6轴，多拖车
    DE_BVC_TruckAxleCnt7MultiTrailer        = 35,                       // 7轴或更多轴，多拖车
    
    // 摩托车
    DE_BVC_MotocycleTypeUnknown             = 40,                       // 缺省类型
    DE_BVC_MotocycleTypeOther               = 41,
    DE_BVC_MotocycleCruiserStandard         = 42,
    DE_BVC_MotocycleSportUnclad             = 43,
    DE_BVC_MotocycleSportTouring            = 44,
    DE_BVC_MotocycleSuperSport              = 45,
    DE_BVC_MotocycleTouring                 = 46,
    DE_BVC_MotocycleTrike                   = 47,
    DE_BVC_MotocyclewPassengers             = 48,

    // 运输类型
    DE_BVC_TransitTypeUnknown               = 50,
    DE_BVC_TransitTypeOther                 = 51,
    DE_BVC_TransitBRT                       = 52,
    DE_BVC_TransitExpressBus                = 53,
    DE_BVC_TransitLocalBus                  = 54,
    DE_BVC_TransitSchoolBus                 = 55,
    DE_BVC_TransitFixedGuideway             = 56,
    DE_BVC_TransitParatransit               = 57,
    DE_BVC_TransitParatransitAmbulance      = 58,

    // 紧急车辆类型
    DE_BVC_EmergenceTypeUnknown             = 60,
    DE_BVC_EmergenceTypeOther               = 61,
    DE_BVC_EmergenceFireLightVehicle        = 62,
    DE_BVC_EmergenceFireHeavyVehicle        = 63,
    DE_BVC_EmergenceFireParamedicVehicle    = 64,
    DE_BVC_EmergenceFireAmbulanceVehicle    = 65,
    DE_BVC_EmergencePoliceLightVehicle      = 66,
    DE_BVC_EmergencePliceHeavyVehicle       = 67,
    DE_BVC_EmergenceOtherResponder          = 68,
    DE_BVC_EmergenceOtherAmbulance          = 69,

    // 其他V2X
    DE_BVC_OtherTravelerUnknown             = 80,
    DE_BVC_OtherTravelerOther               = 81,
    DE_BVC_OtherTravelerPedestrian          = 82,
    DE_BVC_OtherTravelerVisuallyDisabled    = 83,
    DE_BVC_OtherTravelerPhysicallyDisabled  = 84,
    DE_BVC_OtherTravelerBicycle             = 85,
    DE_BVC_OtherTravelerVulnerableRoadworker= 86,

    // 其他V2X 设备类型
    DE_BVC_InfrastructureUnkown             = 90,
    DE_BVC_InfrastructureFixed              = 91,
    DE_BVC_InfrastructureMovable            = 92,
    DE_BVC_EquippedCargoTrailer             = 93,    
};

// 定义四轮分别的刹车状态。这里将车辆的轮胎分为左前、右前、左后、右后四组。当车辆进行刹车时，该数值分别指示了四组轮胎的刹车情况。
// 当车辆为单排轮胎（摩托车等）时，分别以左前和左后表示其前、后轮，后侧轮胎对应数值置为0。当车辆某一组轮胎由多个组成时，其状态将等效为一个数值来表示。
// 位串类型，共5位，该位值为1时，表示刹车状态正常。
// 定长比特串
// 使用UCHAR定义变量
#define DE_BrakeAppliedStatus_LEN       5
enum DE_BrakeAppliedStatus
{
    DE_BAS_Unavailable  = 0b000000, 
    DE_BAS_LeftFront    = 0b000001,
    DE_BAS_LeftRear     = 0b000010,
    DE_BAS_RightFront   = 0b000100,
    DE_BAS_RightRear    = 0b001000,

    DE_BrakeAppliedStatus_Invalid,
};

// 通过刹车辅助系统的状态，指示车辆紧急刹车状态。
// 刹车辅助系统通过对情况的判断，确定是否需要急刹车，进而确定是否需要接管刹车系统，在驾驶员未来得及做出反应时进行刹车。
// 辅助系统可能通过监测油门踏板的突然松开或前置检测器，来判断紧急刹车的需求。
// 枚举类型
#define DE_BrakeBoostApplied_COUNT      3
enum DE_BrakeBoostApplied
{
    DE_BBA_Unavailable  = 0,                                            // 车辆未安装刹车辅助系统或状态不可获取
    DE_BBA_Off          = 1,                                            // 刹车辅助系统关闭
    DE_BBA_On           = 2,                                            // 刹车辅助系统已经启用

    DE_BrakeBoostApplied_Invalid,
};

// 指示刹车踏板状态，即是否处在被踩下状态。
// 枚举类型
#define DE_BrakePedalStatus_COUNT       3 
enum DE_BrakePedalStatus
{
    DE_BPS_Unavailable  = 0,                                            // 车辆踏板状态不可获取
    DE_BPS_Off          = 1,                                            // 踏板未踩下
    DE_BPS_On           = 2,                                            // 踏板已经被踩下

    DE_BrakePedalStatus_Invalid,
};

// 定义粗粒度的车辆航向角。分辨率为1.5°，取值范围0~240
// 值为240时，表示无法获取
// 值为1时，表示 1.5°
#define DE_CoarseHeading_MAX            240
#define DE_CoarseHeading_MIN            0
typedef UCHAR DE_CoarseHeading;

// 定义置信度。分辨率为0.005。
// 范围： 0 ~ 200
// 值为1时，表示置信度为 5%
#define DE_Confidence_MAX               200
#define DE_Confidence_MIN               0
typedef UCHAR DE_Confidence;

// 定义1 个月中的日期。有效范围是1 ～ 31。0 表示未知日期。
#define DE_DDay_MAX                     31
#define DE_DDay_MIN                     0
typedef UCHAR DE_DDay;

// 定义名称字符串类型。长度为63字节
// DescriptiveName ::= IA5String (SIZE(1..63))
#define DE_DescriptiveName_MAX          63
#define DE_DescriptiveName_MIN          1
typedef CString DE_DescriptiveName;

// 定义1 天中的小时时刻。有效范围是0 ～ 23。24 及以上表示未知或无效
#define DE_DHour_MAX                     23
#define DE_DHour_MIN                     0
typedef UCHAR DE_DHour;

// 定义1 小时中的分钟时刻。有效范围是0 ～ 59。数值60 表示未知分钟时刻。
#define DE_DMinute_MAX                  59
#define DE_DMinute_MIN                  0
typedef UCHAR DE_DMinute;

// 定义1 年中的月份。有效范围是1 ～ 12。数值0 表示未知月份。
#define DE_DMonth_MAX                   12
#define DE_DMonth_MIN                   0
typedef UCHAR DE_DMonth;

// 1分钟内的毫秒级时刻，分辨率为1毫秒，有效范围为0~59999, 超过60000为无效值
#define DE_DSecond_MAX                  59999
#define DE_DSecond_MIN                  0
typedef USHORT DE_DSecond;

// 定义和UTC时间的分钟差，用来表示时区，比UTC快为正，否则为负。取值范围： -840 - 840
#define DE_DTimeOffset_MAX              840
#define DE_DTimeOffset_MIN              -840
typedef int DE_DTimeOffset;

// 定义公历年份。0表示未知年份
#define DE_DYear_MAX                     4095
#define DE_DYear_MIN                     0
typedef USHORT DE_DYear;

// 定义车辆海拔高程。分辨率为0.1米，数值-4096表示无效数值
// 范围在-409.5m ~ 6143.9m
#define DE_Elevation_MAX                61439
#define DE_Elevation_MIN                -4096
typedef int DE_Elevation;

// 车辆高程精度。该精度理论上只考虑了当前高程传感器的误差。但是当系统能够自动检测错误并修正时，相应的精度数值也应该提高
// 枚举类型，编码为4bit长度
#define DE_ElevationConfidence_COUNT    16
enum DE_ElevationConfidence
{
    DE_EC_Unavailable   = 0,
    DE_EC_Elev_500_00   = 1,
    DE_EC_Elev_200_00   = 2,
    DE_EC_Elev_100_00   = 3,
    DE_EC_Elev_050_00   = 4,
    DE_EC_Elev_020_00   = 5,
    DE_EC_Elev_010_00   = 6,
    DE_EC_Elev_005_00   = 7,
    DE_EC_Elev_002_00   = 8,
    DE_EC_Elev_001_00   = 9,
    DE_EC_Elev_000_50   = 10,
    DE_EC_Elev_000_20   = 11,
    DE_EC_Elev_000_10   = 12,
    DE_EC_Elev_000_05   = 13,
    DE_EC_Elev_000_02   = 14,
    DE_EC_Elev_000_01   = 15,

    DE_ElevationConfidence_Invalid,
};

// 定义车身周围的车灯状态
// 比特串，至少9位。值为1表示灯以及打开。最大长度不确定，暂定为32位
// 使用UINT定义变量
#define DE_ExteriorLights_MAX_LEN       32
#define DE_ExteriorLights_MIN_LEN       9
enum DE_ExteriorLights
{
    DE_EL_LowBeamHeadLightsOn       = 0b00000000,
    DE_EL_HighBeamHeadLightsOn      = 0b00000001,
    DE_EL_LeftTurnSignalOn          = 0b00000010,
    DE_EL_RightTurnSignalOn         = 0b00000100,
    DE_EL_HazardSignalOn            = 0b00001000,
    DE_EL_AutomaticLightControlOn   = 0b00010000,
    DE_EL_DaytimeRunningLightsOn    = 0b00100000,
    DE_EL_FogLightOn                = 0b01000000,
    DE_EL_ParkingLightsOn           = 0b10000000,

    DE_ExteriorLights_Invalid,
};

// 定义GNSS系统工作状态，包括设备工作状态、锁星情况和修正情况等。GNSS可以是北斗、GPS等系统；
// 定长比特串，8位，
// 使用UCHAR定义变量
#define DE_GNSSstatus_LEN               8
enum DE_GNSSstatus
{
    DE_GNSS_Unavailable                 = 0b00000000,                   // Not Equipped or unavailable
    DE_GNSS_IsHealthy                   = 0b00000001,
    DE_GNSS_IsMonitored                 = 0b00000010,
    DE_GNSS_BaseStationType             = 0b00000100,                   // Set to zero if a moving base station, or if a rover device (an OBU), set to one if it is a fixed base station
    DE_GNSS_aPDOPofUnder5               = 0b00001000,                   // A dilution of precision greater than 5
    DE_GNSS_inViewOfUnder5              = 0b00010000,                   // Less than 5 satellites in view
    DE_GNSS_LocalCorrectionsPresent     = 0b00100000,                   // DGPS type corrections used
    DE_GNSS_NetworkCorrectionsPresent   = 0b01000000,                   // RTK type corrections used
    
    DE_GNSSstatus_Invalid,
};

// 为车辆航向角，即为车头方向与正北方向的顺时针夹角。分辨率为0.0125°
// 取值范围： 0 ~ 28800，
// 整数型
#define DE_Heading_MAX                  28800
#define DE_Heading_MIN                  0
typedef USHORT DE_Heading;

// 数值描述了95% 置信水平的车辆航向精度。该精度理论上只考虑了当前航向传感器的误差。
// 但当系统能够自动检测错误并修正时，相应的精度数值也应该提高。
// 编码长度：3 bit
#define DE_HeadingConfidence_COUNT      8
enum DE_HeadingConfidence
{
    DE_HC_Unavailable   = 0,                                            // B'000 Not Equipped or unavailable
    DE_HC_Prec10deg     = 1,                                            // B'010 10 degrees
    DE_HC_Prec05deg     = 2,                                            // B'011 5 degrees
    DE_HC_Prec01deg     = 3,                                            // B'100 1 degrees
    DE_HC_Prec0_1deg    = 4,                                            // B'101 0.1 degrees
    DE_HC_Prec0_05deg   = 5,                                            // B'110 0.05 degrees
    DE_HC_Prec0_01deg   = 6,                                            // B'110 0.01 degrees
    DE_HC_Prec0_0125deg = 7,                                            // B'111 0.0125 degrees, aligned with heading LSB

    DE_HeadingConfidence_Invalid,
};

// 路口信号机的工作状态指示。用于SPAT 消息中。
// 定长比特串，共16位。
// 使用UShort定义变量
#define DE_IntersectionStatusObject_LEN 16
enum DE_IntersectionStatusObject
{
    DE_ISO_ManualControlIsEnabled                   = 0b0000000000000,  // Timing reported is per programmed values, etc. but person at cabinet can manually request that certain intervals are terminated early (e.g. green).
    DE_ISO_stopTimeIsActivated                      = 0b0000000000001,  // And all counting/timing has stopped.
    DE_ISO_failureFlash                             = 0b0000000000010,  // Above to be used for any detected hardware failures, e.g. conflict monitor as well as for police flash
    DE_ISO_preemptIsActive                          = 0b0000000000100,
    DE_ISO_signalPriorityIsActive                   = 0b0000000001000,  // Additional states
    DE_ISO_fixedTimeOperation                       = 0b0000000010000,  // Schedule of signals is based on time only(i.e. the state can be calculated)
    DE_ISO_trafficDependentOperation                = 0b0000000100000,  // Operation is based on different levels of traffic parameters(requests, duration of gaps or more complex parameters)
    DE_ISO_standbyOperation                         = 0b0000001000000,  // Controller: partially switched off or partially amber flashing
    DE_ISO_failureMode                              = 0b0000010000000,  // Controller has a problem or failure in operation
    DE_ISO_off                                      = 0b0000100000000,  // Controller is switched off. Related to MAP and SPAT bindings
    DE_ISO_recentMAPmessageUpdate                   = 0b0001000000000,  // Map revision with content changes
    DE_ISO_recentChangeInMAPassignedLanesIDsUsed    = 0b0010000000000,  // Change in MAP's assigned lanes used (lane changes). Changes in the active lane list description
    DE_ISO_noValidMAPisAvailableAtThisTime          = 0b0100000000000,  // MAP (and various lanes indexes) not available
    DE_ISO_noValidSPATisAvailableAtThisTime         = 0b1000000000000,  // SPAT system is not working at this time, Bits 14,15 reserved at this time and shall be zero

    DE_IntersectionStatusObject_Invalid,
};

// 车道隔离的属性定义。主要指示车道隔离的物理形式。
// 定长比特串，共16位，第10 ~ 15 位保留并被设置为0。
// 使用 UINT 定义变量
#define DE_LaneAttr_Barrier_LEN         16
enum DE_LaneAttr_Barrier
{
    DE_LAB_Median_RevocableLane     = 0b0000000000,                     // this lane may be activated or not based on the current SPAT message contents if not asserted, the lane is ALWAYS present
    DE_LAB_Median                   = 0b0000000001,
    DE_LAB_WhiteLineHashing         = 0b0000000010,
    DE_LAB_StripedLines             = 0b0000000100,
    DE_LAB_DoubleStripedLines       = 0b0000001000,
    DE_LAB_TrafficCones             = 0b0000010000,
    DE_LAB_ConstructionBarrier      = 0b0000100000,
    DE_LAB_TrafficChannels          = 0b0001000000,
    DE_LAB_LowCurbs                 = 0b0010000000,
    DE_LAB_HighCurbs                = 0b0100000000,

    DE_LaneAttr_Barrier_Invalid,
};

// 定义自行车道的属性
// 定长比特串，共16位。7~15位保留。
// 使用 UINT 定义变量
#define DE_LaneAttr_Bike_LEN            16
enum DE_LaneAttr_Bike
{
    DE_LA_BIKE_RevocableLane                = 0b0000000000,
    DE_LA_BIKE_PedetrianIesAllowed          = 0b0000000001,
    DE_LA_BIKE_IsnikeFlyOverLane            = 0b0000000010,
    DE_LA_BIKE_FixedCycletime               = 0b0000000100,
    DE_LA_BIKE_BiDirectionalCycleTimes      = 0b0000001000,
    DE_LA_BIKE_IsolatedByBarrier            = 0b0000010000,
    DE_LA_BIKE_UnsignalizedSegmentsPresent  = 0b0000100000,

    DE_LaneAttr_Bike_Invalid,
};

// 人行横道的属性，指示车道的一系列通过属性和人行辅助设施
// 定长比特串，共16位。9~15位保留。
// 使用 UINT 定义变量
#define DE_LaneAttr_Crosswalk_LEN       16
enum DE_LaneAttr_Crosswalk
{
    DE_LA_CW_CrosswalkRevocableLane     = 0b0000000000,
    DE_LA_CW_BiycleuseAllowed           = 0b0000000001,
    DE_LA_CW_isXwalkFlyOverLane         = 0b0000000010,
    DE_LA_CW_FixedcycleTime             = 0b0000000100,
    DE_LA_CW_biDirectionalcycleTimes    = 0b0000001000,
    DE_LA_CW_hasPushToWalkButton        = 0b0000010000,
    DE_LA_CW_audioSupport               = 0b0000100000,
    DE_LA_CW_rfSignalRequestpresent     = 0b0001000000,
    DE_LA_CW_unsignalizedSegmentPresent = 0b0010000000,

    DE_LaneAttr_Crosswalk_Invalid,
};

// 停车车道的属性定义。指示车道允许停车的种类和停靠方式
// 定长比特串，共16位，7~15位保留
// 使用 UINT 定义变量
#define DE_LaneAttr_Parking_LEN         16
enum DE_LaneAttr_Parking
{
    DE_LA_PARK_ParkingRevocableLane         = 0b0000000000,
    DE_LA_PARK_ParallelParkingInUse         = 0b0000000001, 
    DE_LA_PARK_headInParkingInUse           = 0b0000000010,
    DE_LA_PARK_doNotParkZone                = 0b0000000100,             // used to denote fire hydrants as well as short disruptions in a parking zone
    DE_LA_PARK_parkingForBusUse             = 0b0000001000,
    DE_LA_PARK_parkingForTaxiUse            = 0b0000010000,
    DE_LA_PARK_noPublicParkingUse           = 0b0000100000,             // private parking, as in front of private property

    DE_LaneAttr_Parking_Invalid,
};

// 人行道的属性定义
// 定长比特串，共16位，4~15位保留
// 使用 UINT 定义变量
#define DE_LaneAttr_Sidewalk_LEN        16
enum DE_LaneAttr_Sidewalk
{
    DE_LA_SIDEWALK_Sidewalk_RevocableLane   = 0b0000000000,             // this lane may be activated or not based on the current SPAT message contents if not asserted, the lane is ALWAYS present
    DE_LA_SIDEWALK_BicyleUseAllowed         = 0b0000000001,             // The path allows bicycle traffic, if not set, this mode is prohibited
    DE_LA_SIDEWALK_IsSidewalkFlyOverLane    = 0b0000000010,             // path of lane is not at grade
    DE_LA_SIDEWALK_WalkBikes                = 0b0000000100,             // bike traffic must dismount and walk

    DE_LaneAttr_Sidewalk_Invalid,
};

// 标线车道的属性定义。指示了车道上网纹或者标志标线所传达的道路信息，如禁行、路线标识
// 等，辅助驾驶员通过一些复杂的路口或路段，提高驾驶安全性。
// 定长比特串，共16位，6~15位保留
// 使用 UINT 定义变量
#define DE_LaneAttr_Striping_LEN        16
enum DE_LaneAttr_Striping
{
    DE_LA_Striping_ToConnectingLanesRevocableLane   = 0b00000,          // this lane may be activated or not activated based on the current SPAT message contents if not asserted, the lane is ALWAYS present
    DE_LA_Striping_DrawOnLeft                       = 0b00001,
    DE_LA_Striping_DrawOnRight                      = 0b00010,          // which side of lane to mark
    DE_LA_Striping_ToConnectingLanesLeft            = 0b00100,
    DE_LA_Striping_ToConnectingLanesRight           = 0b01000,
    DE_LA_Striping_ToConnectingLanesAhead           = 0b10000,          // the stripe type should be presented to the user visually to reflect stripes in the intersection for the type of movement indicated

    DE_LaneAttr_Striping_Invalid,
};

// 轨道车辆车道的属性定义。用来描述一条轨道车辆车道的特殊属性和其允许行驶的车辆种类。
// 定长比特串，共16位，5~15位保留
// 使用 UINT 定义变量
#define DE_LaneAttr_TrackedVehicle_LEN  16
enum DE_LaneAttr_TrackedVehicle
{
    DE_LA_TV_Spec_RevocableLane         = 0b0000000000,                 // this lane may be activated or not based on the current SPAT message contents if not asserted, the lane is ALWAYS present
    DE_LA_TV_Spec_CommuterRailRoadTrack = 0b0000000001,
    DE_LA_TV_Spec_LightRailRoadTrack    = 0b0000000010,
    DE_LA_TV_Spec_HeavyRailRoadTrack    = 0b0000000100,
    DE_LA_TV_Spec_OtherRailType         = 0b0000001000,

    DE_LaneAttr_TrackedVehicle_Invalid,
};

// 车辆行驶车道的属性定义。用来描述一条车用车道的特殊属性和其允许行驶的汽车种类。
// 不定长比特串，最小8位，最大长度不确定，暂定32位
// 使用UINT定义变量
#define DE_LaneAttr_Vehicle_MAX         32
#define DE_LaneAttr_Vehicle_MIN         8
enum DE_LaneAttr_Vehicle
{
    DE_LA_Vehicle_IsVehicleRevocableLane    = 0b0000000000,             // this lane may be activated or not based on the current SPAT message contents if not asserted, the lane is ALWAYS present
    DE_LA_Vehicle_IsVehicleFlyOverLane      = 0b0000000001,             // path of lane is not at grade
    DE_LA_Vehicle_HovLaneUseOnly            = 0b0000000010,
    DE_LA_Vehicle_RestrictedToBusUse        = 0b0000000100,
    DE_LA_Vehicle_RestrictedToTaxiUse       = 0b0000001000,
    DE_LA_Vehicle_RestrictedFromPublicUse   = 0b0000010000,
    DE_LA_Vehicle_hasIRbeaconCoverage       = 0b0000100000,
    DE_LA_Vehicle_PermissionOnRequested     = 0b0001000000,             // e.g. to inform about a lane for e-cars

    DE_LaneAttr_Vehicle_Invalid,
};

// 定义车道ID，每一条有向路段上的每个车道，都有一个单独的ID。数值0表示无效ID。值255保留。
// 整数型
// 使用 UCHAR 定义变量
#define DE_LaneID_MAX                   255
#define DE_LaneID_MIN                   0
typedef UCHAR DE_LaneID;

// 定义车道被恭共享的情况。在已有车道属性定义基础上，该数据表示此车道还会有其他的交通参与值出现，并可能拥有同样的路权。
// 定长比特串，共10位，值为1表示对应的车道共享属性有效
// 如果所有位都是0，表示没有共享和不准重叠
// 使用 UShort 定义变量
#define DE_LaneSharing_LEN              10
enum DE_LaneSharing
{
    DE_LS_OverlappingLaneDescriptionProvided    = 0b0000000000,         // Assert when another lane object is present to describle the part of the overlapping shared lane. this construct is not used for lane objects which simply cross
    DE_LS_MultipleLanesTreadtedAsOneLane        = 0b0000000001,         // Assert if the lane object path and width detailes represents multiple lanes within it that are not further described Various modes and type of traffic that may share this lane
    DE_LS_OtherNonMotorizedTrafficTypes         = 0b0000000010,         // horses drawn etc.
    DE_LS_IndividualMotorizedVehicleTraffic     = 0b0000000100,
    DE_LS_BusVehicleTraffic                     = 0b0000001000,
    DE_LS_TaxiVehicleTraffic                    = 0b0000010000,
    DE_LS_PedestriansTraffic                    = 0b0000100000,
    DE_LS_CyclistVehicleTraffic                 = 0b0001000000,
    DE_LS_TrackedVehicleTraffic                 = 0b0010000000,
    DE_LS_Pedestriantraffic                     = 0b0100000000,

    DE_LaneSharing_Invalid,
};

// 定义车道宽度，分辨率为 1cm
#define DE_LaneWidth_MAX                32767
#define DE_LaneWidth_MIN                0
typedef USHORT DE_LaneWidth;

// 定义纬度数值，北纬为正，南纬为负，分辨率 le-7°。
#define DE_Latitude_MAX                 900000000
#define DE_Latitude_MIN                 -900000000
typedef int DE_Latitude;

// 定义信号灯某一相位的灯色状态
#define DE_LightState_COUNT             9
enum DE_LightState
{
    DE_LightState_Unavaiable                    = 0,                    // 未知状态
    DE_LightState_Dark                          = 1,                    // 信号灯未工作
    DE_LightState_StopTheProceed                = 2,                    // 红闪
    DE_LightState_StopAndRemain                 = 3,                    // 红灯状态
    DE_LightState_PreMovement                   = 4,                    // 绿灯待行状态（红灯闪烁）
    DE_LightState_PermissiveMovementAllowed     = 5,                    // 绿灯状态
    DE_LightState_ProtectedMovementAllowed      = 6,                    // 受保护相位绿灯（箭头灯）
    DE_LightState_IntersectionClearance         = 7,                    // 黄灯状态
    DE_LightState_CautionConflictingTraffic     = 8,                    // 黄灯闪烁

    DE_LightState_Invalid,
};

// 定义经度数值。东经为正，西经为负。分辨率为1e-7°。
#define DE_Longtitude_MAX               1800000001LL
#define DE_Longtitude_MIN               -1799999999LL
typedef long long DE_Longtitude;

// 数值用来表示当前年份，已经过去的总分钟数（UTC 时间）。分辨率为1 分钟。该数值配合
// DSecond 数值，则可表示全年已过去的总毫秒数。范围：0 ~ 527040
// 值527040表示非法数值
#define DE_MinuteOfTheYear_MAX          527040
#define DE_MinuteOfTheYear_MIN          0
typedef UINT DE_MinuteOfTheYear;

// 发送方为自己发送的同类消息，依次进行编号。编号数值为0 ～ 127。
// 当发送方开始发起某一类数据时，它可以随机选择起始编号，随后依次递增。发送方也可以在
// 连续发送相同的数据帧时，选择使用相同的MsgCount 消息编号。编号到达127 后，则下一个回到0。
#define DE_MsgCount_MAX                 127
#define DE_MsgCount_MIN                 0
typedef UCHAR DE_MsgCount;

// 定义节点ID。路网最基本的构成即节点和连接节点的路段。节点可以是路口，也可以是一条
// 路的端点。一个节点的ID 在同一个区域内是唯一的。数值0 ～ 255 预留为测试使用。
// 范围： 0 ~ 65535
#define DE_NodeID_MAX                   65535
#define DE_NodeID_MIN                   0
typedef UINT DE_NodeID;

// 12 比特数值，表示当前位置点关于参考位置点的经纬度偏差。值得注意的是，根据纬度的不同，
// 相同的经纬度偏差值对应的实际球面距离可能不同。
// 关于参考点，向东或向北偏移，数值为正，反之为负。分辨率为1e-7°。
#define DE_OffsetLL_B12_MAX             2047
#define DE_OffsetLL_B12_MIN             -2047
typedef int DE_OffsetLL_B12;

// 14 比特数值，表示当前位置点关于参考位置点的经纬度偏差。值得注意的是，根据纬度的不同，
// 相同的经纬度偏差值对应的实际球面距离可能不同。
// 关于参考点，向东或向北偏移，数值为正，反之为负。分辨率为1e-7°。
#define DE_OffsetLL_B14_MAX             8191
#define DE_OffsetLL_B14_MIN             -8192
typedef int DE_OffsetLL_B14;

// 16 比特数值，表示当前位置点关于参考位置点的经纬度偏差。值得注意的是，根据纬度的不同，
// 相同的经纬度偏差值对应的实际球面距离可能不同。
// 关于参考点，向东或向北偏移，数值为正，反之为负。分辨率为1e-7°。
#define DE_OffsetLL_B16_MAX             32767
#define DE_OffsetLL_B16_MIN             -32768
typedef int DE_OffsetLL_B16;

// 18 比特数值，表示当前位置点关于参考位置点的经纬度偏差。值得注意的是，根据纬度的不同，
// 相同的经纬度偏差值对应的实际球面距离可能不同。
// 关于参考点，向东或向北偏移，数值为正，反之为负。分辨率为1e-7°。
#define DE_OffsetLL_B18_MAX             131071
#define DE_OffsetLL_B18_MIN             -131072
typedef int DE_OffsetLL_B18;

// 22 比特数值，表示当前位置点关于参考位置点的经纬度偏差。值得注意的是，根据纬度的不同，
// 相同的经纬度偏差值对应的实际球面距离可能不同。
// 关于参考点，向东或向北偏移，数值为正，反之为负。分辨率为1e-7°。
#define DE_OffsetLL_B22_MAX             2097151
#define DE_OffsetLL_B22_MIN             -2097152
typedef int DE_OffsetLL_B22;

// 24 比特数值，表示当前位置点关于参考位置点的经纬度偏差。值得注意的是，根据纬度的不同，
// 相同的经纬度偏差值对应的实际球面距离可能不同。
// 关于参考点，向东或向北偏移，数值为正，反之为负。分辨率为1e-7°。
#define DE_OffsetLL_B24_MAX             8388607
#define DE_OffsetLL_B24_MIN             -8388608
typedef int DE_OffsetLL_B24;

// 路侧单元检测到的交通参与者类型
// 枚举类型，类型值数量未知，暂定32
#define DE_PartcipantType_COUNT         32
enum DE_PartcipantType
{
    DE_PT_Unknown       = 0,    
    DE_PT_Motor         = 1,                                            // 机动车
    DE_PT_NonMotor      = 2,                                            // 非机动车
    DE_PT_Pedestrian    = 3,                                            // 行人
    DE_PT_RSU           = 4,                                            // RSU

    DE_PartcipantType_Invalid,
};

// 定义信号灯相位ID。数值0表示无效ID
#define DE_PhaseID_MAX                  255
#define DE_PhaseID_MIN                  0
typedef USHORT DE_PhaseID;

// 数值描述了95%置信水平的车辆位置精度。该精度理论上只考虑了当前位置传感器的误差。
// 但当系统能够自动检测错误并修正时，相应的精度值也应该提高
// 枚举值，共16个值
// 编码长度：4 bit
#define DE_PositionConfidence_COUNT     16
enum DE_PositionConfidence
{
    DE_PC_Unavailable   = 0,
    DE_PC_A500m         = 1,
    DE_PC_A200m         = 2,
    DE_PC_A100m         = 3,
    DE_PC_A50m          = 4,
    DE_PC_A20m          = 5,
    DE_PC_A10m          = 6,
    DE_PC_A5m           = 7,
    DE_PC_A2m           = 8,
    DE_PC_A1m           = 9,
    DE_PC_A50cm         = 10,
    DE_PC_A20cm         = 11,
    DE_PC_A10cm         = 12,
    DE_PC_A5cm          = 13,
    DE_PC_A2cm          = 14,
    DE_PC_A1cm          = 15,

    DE_PositionConfidence_Invalid,
};

// 表示消息优先级。数值长度占8 位，其中低五位为0，为无效位，高三位为有效数据位。数值
// 有效范围是B00000000 到B11100000，分别表示8 档由低到高的优先级。
// 八位串，长度为1，即1个字节
#define DE_Priority_MAX                 0b11100000
#define DE_Priority_MIN                 0b00100000
#define DE_Priority_LEN                 1
typedef UCHAR DE_Priority;

// 表示一个特定圆形范围的半径大小，如一个交通事件基于圆心参考位置点的作用范围。数值主
// 要用于RSA 消息。分辨率为10 cm。
// 1 = 10 cm
#define DE_Radius_MAX                   1024
#define DE_Radius_MIN                   0
typedef USHORT DE_Radius;

// 表示车辆预测自身前方行驶轨迹的曲率半径。数值可能来源于地图数据、曲率传感器、视觉传感器、定位系统等。
// 分辨率为10 cm。轨迹曲线向右偏转（圆心在车辆行驶方向右侧）数值为正，向左则为负。数值32767 表示直线行驶。
// 单位：厘米/cm
#define DE_RadiusOfCurvation_MAX        32767
#define DE_RadiusOfCurvation_MIN        -32767
typedef int DE_RadiusOfCurvation;

// 定义地图中各个划分区域的ID 号。数值0 仅用于测试。
// 范围： 0 ~ 65535
#define DE_RoadRegulatorID_MAX          65535
#define DE_RoadRegulatorID_MIN          0
typedef UINT DE_RoadRegulatorID;

// 定义路侧交通参与者数据的检测器来源。
// 枚举值，数值数量未知，暂定32个
#define DE_SourceType_COUNT             32
enum DE_SourceType
{
    DE_ST_Unknown           = 0,
    DE_ST_SelfInfo          = 1,
    DE_ST_V2X               = 2,
    DE_ST_Video             = 3,
    DE_ST_MicrowaveRadar    = 4,
    DE_ST_Loop              = 5,

    DE_SourceType_Invalid   = 32,
};

// 车速大小。分辨率为0.02 m/s。数值8191 表示无效数值。
// 范围： 0 ~ 8191，分辨率：0.02m/s
#define DE_Speed_MAX                    8191
#define DE_Speed_MIN                    0
typedef USHORT DE_Speed;

// 数值描述了95% 置信水平的车速精度。该精度理论上只考虑了当前车速传感器的误差。但当
// 系统能够自动检测错误并修正时，相应的精度数值也应该提高。
// 枚举值，数值为8个
// 编码长度：3 bit
#define DE_SpeedConfidence_COUNT        8
enum DE_SpeedConfidence
{
    DE_SC_Unavailable   = 0,                                            // Not Equipped or unavailable
    DE_SC_Prec100ms     = 1,                                            // 100 meters / sec
    DE_SC_Prec10ms      = 2,                                            // 10 meters / sec
    DE_SC_Prec5ms       = 3,                                            // 5 meters / sec
    DE_SC_Prec1ms       = 4,                                            // 1 meters / sec
    DE_SC_Prec0_1ms     = 5,
    DE_SC_Prec0_05ms    = 6,
    DE_SC_Prec0_01ms    = 7,

    DE_SpeedConfidence_Invalid,
};

// 定义限速类型，指示给出的限速大小对应的参考类型
// 枚举值，数量未知，暂定32
#define DE_SpeedLimitType_COUNT         32
enum DE_SpeedLimitType
{
    DE_SLT_Unknown                                  = 0,
    DE_SLT_MaxSpeedInSchoolZone                     = 1,                // Only sent when limit is active
    DE_SLT_MaxSpeedSchoolZoneWhenChildrenArePresnt  = 2,                // Sent at any time
    DE_SLT_MaxSpeedInConstructionZone               = 3,                // Used for work zones, incident zones, etc
    DE_SLT_VehicleMinspeed                          = 4,
    DE_SLT_VehicleMaxSpeed                          = 5,                // Regualtatory speed limit for general traffic
    DE_SLT_VehicleNightMaxSpeed                     = 6,
    DE_SLT_TruckMinSpeed                            = 7,
    DE_SLT_TruckMaxSpeed                            = 8,
    DE_SLT_TruckNightMaxSpeed                       = 9,
    DE_SLT_VehiclesWithTrailersMinSpeed             = 10,
    DE_SLT_VehiclesWithTrailerMaxSpeed              = 11,
    DE_SLT_VehiclesWithTrailerNightMaxSpeed         = 12,

    DE_SpeedLimitType_Invalid                       = 32,
};

// 定义车辆动态稳定控制系统状态
// 枚举值
#define DE_StabilityControlStatus_COUNT 4
enum DE_StabilityControlStatus
{
    DE_SCS_Unavailable  = 0,
    DE_SCS_Off          = 1,
    DE_SCS_On           = 2,
    DE_SCS_Engaged      = 3,

    DE_StabilityControlStatus_Invalid,
};

// 定义方向盘转角，向右为正，向左为负。分辨率为1.5°。数值127位无效数值
// 范围： -126 ~ 127
#define DE_SteeringWheelAngle_MAX       127
#define DE_SteeringWheelAngle_MIN       -126
typedef int DE_SteeringWheelAngle;

// 数值描述了95% 置信水平的方向盘转角精度。该精度理论上只考虑了当前方向盘传感器的误差。但当
// 系统能够自动检测错误并修正时，相应的精度数值也应该提高。
// 枚举类型，数量为4
// 编码长度：2 bit
#define DE_SteeringWheelAngleConfidence_COUNT   4 
enum DE_SteeringWheelAngleConfidence
{
    DE_SWAC_Unknown     = 0,
    DE_SWAC_Prec2deg    = 1,
    DE_SWAC_Prec1deg    = 2,
    DE_SWAC_Prec0_02deg = 3,

    DE_SteeringWheelAngleConfidence_Invalid,
};

// 数值描述了95% 置信水平的时间精度。该精度理论上只考虑了当前计时系统本身传感器的误差。但当
// 系统能够自动检测错误并修正时，相应的精度数值也应该提高。
#define DE_TimeConfidence_COUNT         39
enum DE_TimeConfidence
{
    DE_TC_Unknown                   = 0,
    DE_TC_Time_100_000              = 1,
    DE_TC_Time_050_000              = 2,
    DE_TC_Time_020_000              = 3,
    DE_TC_Time_010_000              = 4,
    DE_TC_Time_002_000              = 5,
    DE_TC_Time_001_000              = 6,
    DE_TC_Time_000_500              = 7,
    DE_TC_Time_000_200              = 8,
    DE_TC_Time_000_100              = 9,
    DE_TC_Time_000_050              = 10,
    DE_TC_Time_000_020              = 11,
    DE_TC_Time_000_010              = 12,
    DE_TC_Time_000_005              = 13,
    DE_TC_Time_000_002              = 14,
    DE_TC_Time_000_001              = 15,
    DE_TC_Time_000_000_5            = 16,
    DE_TC_Time_000_000_2            = 17,
    DE_TC_Time_000_000_1            = 18,
    DE_TC_Time_000_000_05           = 19,
    DE_TC_Time_000_000_02           = 20,
    DE_TC_Time_000_000_01           = 21,
    DE_TC_Time_000_000_005          = 22,
    DE_TC_Time_000_000_002          = 23,
    DE_TC_Time_000_000_001          = 24,

    DE_TC_Time_000_000_000_5        = 25,
    DE_TC_Time_000_000_000_2        = 26,
    DE_TC_Time_000_000_000_1        = 27,
    DE_TC_Time_000_000_000_05       = 28,
    DE_TC_Time_000_000_000_02       = 29,
    DE_TC_Time_000_000_000_01       = 30,

    DE_TC_Time_000_000_000_005      = 31,
    DE_TC_Time_000_000_000_002      = 32,
    DE_TC_Time_000_000_000_001      = 33,

    DE_TC_Time_000_000_000_000_5    = 34,
    DE_TC_Time_000_000_000_000_2    = 35,
    DE_TC_Time_000_000_000_000_1    = 36,
    DE_TC_Time_000_000_000_000_05   = 37,
    DE_TC_Time_000_000_000_000_02   = 38,
    DE_TC_Time_000_000_000_000_01   = 39,

    DE_TimeConfidence_Invalid,
};

// 以100毫秒为单位，定义一小时中的时间。可以表示当前小时中的时刻，也可以表示长度不超过1 小时的时间段。
// 分辨率为100毫秒。有效范围是0 ～ 35999。数值36000 表示大于1 小时的时间长度。数值36001表示无效数值。
// 单位：毫秒
#define DE_TimeMark_MAX                 36001
#define DE_TimeMark_MIN                 0
typedef USHORT DE_TimeMark;

// 以10 毫秒为单位，定义当前描述时刻（较早）相对于参考时间点（较晚）的偏差。用于车辆历史轨迹点的表达。分辨率为10 毫秒。数值65535 表示无效数据。
#define DE_TimeOffset_MAX               65535
#define DE_TimeOffset_MIN               1
typedef USHORT DE_TimeOffset;

// 定义牵引力控制系统实时状态
// 枚举值，数量为4
#define DE_TractionControlStatus_COUNT  4
enum DE_TractionControlStatus
{
    DE_TCS_Unavailable  = 0,                                            // B'00 Not Equipped with traction control or traction control status is unavailable
    DE_TCS_Off          = 1,                                            // B'01 traction control is Off
    DE_TCS_On           = 2,                                            // B'10 traction control is On (but not Engaged)
    DE_TCS_Engaged      = 3,                                            // B'11 traction control is Engaged

    DE_TractionControlStatus_Invalid,
};

// 定义车辆档位状态
// 枚举值，数量为8
#define DE_TransmissionState_COUNT      8
enum DE_TransmissionState
{
    DE_TS_Neutral       = 0,
    DE_TS_Park          = 1,
    DE_TS_ForwardGears  = 2,
    DE_TS_ReverseGears  = 3,
    DE_TS_Reserved1     = 4,
    DE_TS_Reserved2     = 5,
    DE_TS_Reserved3     = 6,
    DE_TS_Unavailable   = 7,

    DE_TransmissionState_Invalid,
};

// 定义了一系列车辆的特殊状态。如果数据某一位被置1，表示车辆处于该位对应的状态。
// 当至少有一种对应状态被激活或从激活状态恢复，该标志数值才应该被设置和交互。
// 当车辆收到一个包含车辆特殊状态的数据后，需要根据数据的内容，来选择特定的操作。
// 比特串，13位或以上。最大长度未知，暂定为32
// 使用UINT定义变量
#define DE_VehicleEventFlags_MAX_LEN    32
#define DE_VehicleEventFlags_MIN_LEN    13
enum DE_VehicleEventFlags
{
    DE_VEF_HazardLights             = 0b000000000000,                   // 车辆警示灯亮起
    DE_VEF_StopLineViolation        = 0b000000000001,                   // 车辆在到达路口前预测可能会来不及刹车而越过停止线
    DE_VEF_ABSactivated             = 0b000000000010,                   // ABS系统被触发并超过100毫秒
    DE_VEF_TractionControlLoss      = 0b000000000100,                   // 牵引力控制系统被触发并超过100毫秒
    DE_VEF_StabilityControlActivated= 0b000000001000,                   // 车身稳定系统被触发并超过100毫秒
    DE_VEF_HazardousMaterials       = 0b000000010000,                   // 危险品运输车
    DE_VEF_Reserved1                = 0b000000100000,                   // 
    DE_VEF_HardBraking              = 0b000001000000,                   // 车辆急刹车，并且减速度大于0.4g，g位重力加速度典型值9.80665m/s2.
    DE_VEF_LightsChanged            = 0b000010000000,                   // 过去2秒内车灯状态改变
    DE_VEF_WipersChanged            = 0b000100000000,                   // 过去2秒内，车辆雨刷9前窗或后窗）状态改变
    DE_VEF_FlatTire                 = 0b001000000000,                   // 车辆发现至少一个轮胎爆胎
    DE_VEF_DisabledVehicle          = 0b010000000000,                   // 车辆故障无法行驶
    DE_VEF_AirBagdeplyment          = 0b100000000000,                   // 至少一个安全气囊弹出

    DE_VehicleEventFlags_Invalid,
};

// 定义车身高度，分辨率为5cm。数值0标书无效数据
#define DE_VehicleHeight_MAX            127
#define DE_VehicleHeight_MIN            0
typedef UCHAR DE_VehicleHeight;

// 定义车辆车身长度，分辨率1cm，数值0表示无效数据
#define DE_VehicleLength_MAX            4095
#define DE_VehicleLength_MIN            0
typedef USHORT DE_VehicleLength;

// 定义车身宽度，分辨率1cm，数据中0表示无效数据
#define DE_VehicleWidth_MAX             1023
#define DE_VehicleWidth_MIN             0
typedef USHORT DE_VehicleWidth;

// 定义Z轴方向的加速度大小，Z轴方向竖直向下，沿着Z轴方向为正，分辨率为0.02g。
// -127表示无效。
// 范围：-127 ~ 127
#define DE_VerticalAcceleration_MAX     127
#define DE_VerticalAcceleration_MIN     -127
typedef int DE_VerticalAcceleration;

// 7比特的数据，定义垂直方向（Z轴）关于参考位置点的偏差。沿着Z轴方向为正。数据分辨率为10cm。-64表示未知大小。
// 范围：-64 ~ 63
// 单位：厘米/cm
#define DE_VertOffset_B07_MAX           63
#define DE_VertOffset_B07_MIN           -64
typedef int DE_VertOffset_B07;

// 8比特的数据，定义垂直方向（Z轴）关于参考位置点的偏差。沿着Z轴方向为正。数据分辨率为10cm。-128表示未知大小。
// 范围：-128 ~ 127
#define DE_VertOffset_B08_MAX           127
#define DE_VertOffset_B08_MIN           -128
typedef int DE_VertOffset_B08;

// 9比特的数据，定义垂直方向（Z轴）关于参考位置点的偏差。沿着Z轴方向为正。数据分辨率为10cm。-256表示未知大小。
// 范围：-256 ~ 255
#define DE_VertOffset_B09_MAX           255
#define DE_VertOffset_B09_MIN           -256
typedef int DE_VertOffset_B09;

// 10比特的数据，定义垂直方向（Z轴）关于参考位置点的偏差。沿着Z轴方向为正。数据分辨率为10cm。-512表示未知大小。
// 范围：-512 ~ 511
#define DE_VertOffset_B10_MAX           511
#define DE_VertOffset_B10_MIN           -512
typedef int DE_VertOffset_B10;

// 11比特的数据，定义垂直方向（Z轴）关于参考位置点的偏差。沿着Z轴方向为正。数据分辨率为10cm。-1024表示未知大小。
// 范围：-1024 ~ 1023
#define DE_VertOffset_B11_MAX           1023
#define DE_VertOffset_B11_MIN           -1024
typedef int DE_VertOffset_B11;

// 12比特的数据，定义垂直方向（Z轴）关于参考位置点的偏差。沿着Z轴方向为正。数据分辨率为10cm。-2048表示未知大小。
// 范围：-2048 ~ 2047
#define DE_VertOffset_B12_MAX           2047
#define DE_VertOffset_B12_MIN           -2048
typedef int DE_VertOffset_B12;

// 车辆横摆角速度。指汽车绕垂直轴的偏移。该偏移的大小代表汽车的稳定程度。
// 如果偏转角速度达到一个阈值，说明汽车处于发送侧滑或甩尾等危险工况。
// 顺时针旋转为正，逆时针为负，数据分辨率为 0.01°/s.
// 范围：-32767 ~ 32767
#define DE_YawRate_MAX                  32767
#define DE_YawRate_MIN                  -32767
typedef int DE_YawRate;


#endif // _CSAE_DATA_ELEMENT_H_
