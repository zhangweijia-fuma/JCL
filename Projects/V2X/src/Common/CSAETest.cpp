
#include "CSAEMessageSet.h"

CDF_NodeReferenceID nodeReferenceID;
CDF_Position3D position3D;
CDF_PositionConfidenceSet posCfdSet, PosAccuracy;
CDF_MotionConfidenceSet motionCfdSet;
CDF_TimeChangeDetails timeChangeDetails;
CDF_PhaseState phaseState;
CDF_PhaseStateList phaseStateList;
CDF_Phase phase;
CDF_PhaseList phaseList;
CDF_RegulatorySpeedLimit speedLimit;
CDF_SpeedLimitList speedLimitList;
CDF_PositionLL24B posLL24; 
CDF_PositionLL28B posLL28;
CDF_PositionLL32B posLL32;
CDF_PositionLL36B posLL36;
CDF_PositionLL44B posLL44;
CDF_PositionLL48B posLL48;
CDF_PositionLLmD_64B posLL64;
CDF_PositionOffsetLL posOffsetLL;
CDF_VerticalOffset verticalOffset;
CDF_PositionOffsetLLV posOffsetLLV;
CDF_RoadPoint roadPoint;
CDF_PointList pointList;
CDF_AccelerationSet4Way accSet4Way;
CDF_BrakeSystemStatus brakeSystemStatus;
CDF_ConnectingLane connectingLane;
CDF_Connection conn;
CDF_ConnectsToList connList;
CDF_DDateTime datetime;
CDF_FullPositionVector fullPositionVector;
CDF_IntersectionState intersectionState;
CDF_IntersectionStateList intersectionStateList;
CDF_LaneTypeAttributes laneTypeAttr;
CDF_LaneAttributes laneAttr;
CDF_Lane lane;
CDF_LaneList laneList;
CDF_Movement movement;
CDF_MovementList movementList;
CDF_Link cdfLink;
CDF_LinkList linkList;
CDF_Node node;
CDF_NodeList nodeList;
CDF_VehicleClassification vehicleClass;
CDF_VehicleSize vehicleSize;
CDF_ParticipantData ptcData;
CDF_ParticipantList ptcList;
CDF_PathHistoryPoint pathHistoryPoint;
CDF_PathHistoryPointList pathHistoryPointList;
CDF_PathHistory pathHistory;
CDF_PathPointList pathPointList;
CDF_PathPrediction pathPrediction;
CDF_SignalState signalState;
CDF_VehicleSafetyExtensions vehicleSafetyExt;

CBasicSafetyMessage bsm;
CMapData mapData;
CRoadSideInformation rsi;
CRoadSideSafetyMessage rsm;
CSPAT spat;
CMainMsgFrame mmf;


void SetTestData()
{
    DE_RoadRegulatorID regionID = 32;;
    DE_NodeID nodeID = 21;;

    nodeReferenceID.SetParameter(nodeID, regionID);

    DE_Latitude lat;
    DE_Longtitude lon;
    DE_Elevation elev;

    position3D.SetParameter(333, 4544, 121);

    DE_PositionConfidence posCfd = DE_PC_A500m;
    DE_ElevationConfidence eleCfd = DE_EC_Elev_001_00 ;

    posCfdSet.SetParameter(posCfd, eleCfd);

    DE_SpeedConfidence speedCfd              = DE_SC_Prec1ms;
    DE_HeadingConfidence headingCfd          = DE_HC_Prec01deg;
    DE_SteeringWheelAngleConfidence steerCfd = DE_SWAC_Prec1deg;

    motionCfdSet.SetParameter(speedCfd, headingCfd, steerCfd);

    DE_TimeMark startTime, minEndTime, maxEndTime, likelyEndTime, nextStartTime, nextDuration;
    DE_Confidence timeConfidence;

    startTime       = 1223;
    likelyEndTime   = 32211;
    minEndTime      = 3568;
    maxEndTime      = 9823;
    timeConfidence  = 199;
    nextStartTime   = 7811;
    nextDuration    = 120;

    timeChangeDetails.SetParameter(startTime, likelyEndTime, minEndTime, maxEndTime, timeConfidence, nextStartTime, nextDuration);

    phaseState.SetParameter(DE_LightState_StopTheProceed, &timeChangeDetails);

    phaseStateList.AddListItem(&phaseState);

    DE_PhaseID phaseID = 1;
    phase.SetParameter( phaseID, &phaseStateList );
    phaseList.AddListItem(&phase);

    DE_SpeedLimitType speedLimitType = DE_SLT_TruckMaxSpeed;
    DE_Speed speed = 120;

    speedLimit.SetParameter(speedLimitType, speed);
    speedLimitList.AddListItem(&speedLimit);
    speedLimitList.AddListItem(&speedLimit);

    posLL24.SetParameter(1234, 121);
    posLL28.SetParameter(1234, 4321);
    posLL32.SetParameter(1234, 4321);
    posLL36.SetParameter(1234, 4321);
    posLL44.SetParameter(1234, 4321);
    posLL48.SetParameter(1234, 4321);
    posLL64.SetParameter(12345678, 56784321);

    posOffsetLL.SetObject(&posLL48);

    verticalOffset.SetParameter(VERTICAL_OFFSET_ELEVATION, 12345);
    posOffsetLLV.SetParameter(&posOffsetLL, &verticalOffset);
    roadPoint.SetParameter(&posOffsetLLV);

    pointList.AddListItem(&roadPoint);
    pointList.AddListItem(&roadPoint);

    DE_Acceleration longAcc = 213;
    DE_Acceleration latAcc  = 456;
    DE_VerticalAcceleration vertAcc = 12;
    DE_YawRate yawRate = 12;

    accSet4Way.SetParameter(longAcc, latAcc, vertAcc, yawRate);

    DE_BrakePedalStatus         brakePadelStatus        = DE_BPS_On;
    UCHAR                       brakeAppliedStatus      = DE_BAS_LeftFront;
    DE_TractionControlStatus    tractionControlStatus   = DE_TCS_Engaged ;
    DE_AntiLockBrakeStatus      absStatus               = DE_ABS_Engaged;
    DE_StabilityControlStatus   stabilityControlStatus  = DE_SCS_Engaged;
    DE_BrakeBoostApplied        brakeBoostApplied       = DE_BBA_On;
    DE_AuxiliaryBrakeStatus     auxBrakesStatus         = DE_AuxBS_On;

    if ( brakeSystemStatus.SetParameter(brakePadelStatus) == false )
    {
        LogE("SetParameter failed");
        return;
    }

    if ( brakeSystemStatus.SetParameter(brakeAppliedStatus) == false )
    {
        LogE("SetParameter failed");
        return;
    }

    if ( brakeSystemStatus.SetParameter(tractionControlStatus) == false )
    {
        LogE("SetParameter failed");
        return;
    }

    if ( brakeSystemStatus.SetParameter(absStatus) == false )
    {
        LogE("SetParameter failed");
        return;
    }

    if ( brakeSystemStatus.SetParameter(stabilityControlStatus) == false )
    {
        LogE("SetParameter failed");
        return;
    }

    if ( brakeSystemStatus.SetParameter(brakeBoostApplied) == false )
    {
        LogE("SetParameter failed");
        return;
    }

    if ( brakeSystemStatus.SetParameter(auxBrakesStatus) == false )
    {
        LogE("SetParameter failed");
        return;
    }

    if ( brakeSystemStatus.Encode() == CODEC_FAILED )
    {
        LogI("Encode CDF_BrakeSystemStatus failed.");
        return;
    }

    DE_LaneID laneID = 1;
    USHORT allowedManeuvers = DE_BAS_LeftFront | DE_AM_UturnAllowed | DE_AM_Caution;

    connectingLane.SetParameter(laneID, allowedManeuvers );

    conn.SetParameter(&nodeReferenceID, &connectingLane, phaseID);
    connList.AddListItem(&conn);

    DE_DYear        nYear   = 2020;
    DE_DMonth       nMonth  = 1;
    DE_DDay         nDay    = 1;
    DE_DHour        nHour   = 12;
    DE_DMinute      nMinute = 30;
    DE_DSecond      nSecond = 0;
    DE_DTimeOffset  nTimeOffset = 30;

    datetime.SetParameter(nYear, nMonth, nDay, nHour, nMinute, nSecond, nTimeOffset);


    posCfd = DE_PC_A500m;
    eleCfd = DE_EC_Elev_001_00 ;

    PosAccuracy.SetParameter(posCfd, eleCfd);

    speed = 21;;
    
    DE_Heading              heading     = 12345 ;
    DE_TransmissionState    transState  = DE_TS_ForwardGears;
    DE_TimeConfidence       timeCfd     = DE_TC_Time_050_000;

    fullPositionVector.SetParameter(&datetime, &position3D, heading, transState, speed, &PosAccuracy, timeCfd, &posCfdSet, &motionCfdSet);

    USHORT statusObject = DE_ISO_standbyOperation;
    DE_MinuteOfTheYear nMinuteOfYear = 12;
    DE_DSecond timeStamp = 23;

    if ( intersectionState.SetParameter(&nodeReferenceID, statusObject, &phaseList, nMinuteOfYear, timeStamp) == false )
    {
        LogE("CDF_IntersectionState SetParameter failed");
        return;
    }

    intersectionStateList.AddListItem(&intersectionState);
    UINT nAttr = DE_LA_Vehicle_PermissionOnRequested;
    int nChoiceIndex = LANE_ATTR_VEHICLE;

    if ( laneTypeAttr.SetParameter(nChoiceIndex, nAttr, 12) == false )
    {
        LogE("CDF_LaneTypeAttributes SetParameter failed");
        return;
    }

    USHORT laneSharing = DE_LS_IndividualMotorizedVehicleTraffic|DE_LS_Pedestriantraffic;

    if ( laneAttr.SetParameter(&laneTypeAttr, laneSharing) == false )
    {
        LogE("CDF_LaneAttributes SetParameter failed");
        return;
    }

    laneID = 2;
    DE_AllowedManeuvers nAllowedManeuvers = DE_AM_RightTurnOnRedAllowd;

    if ( lane.SetParameter(laneID, nAllowedManeuvers, &laneAttr, &connList, &speedLimitList, &pointList) == false )
    {
        LogE("CDF_Lane SetParameter failed");
        return;
    }

    laneList.AddListItem(&lane);

    movement.SetParameter(&nodeReferenceID, phaseID);
    movementList.AddListItem(&movement);

    DE_DescriptiveName name = "Chengdu HighTechDisctrict Tianfu 4th st.";
    DE_LaneWidth laneWidth = 100;
    cdfLink.SetParameter(&nodeReferenceID, laneWidth, &laneList, name, &speedLimitList, &pointList, &movementList);

    int nListLen = linkList.AddListItem(&cdfLink);

    node.SetParameter(&nodeReferenceID, &position3D, name, &linkList);
    nodeList.AddListItem(&node);
    vehicleClass.SetParameter( DE_BVC_PassengerVehicleTypeOther );

    vehicleSize.SetParameter(100, 90, 100);

    CString strRSUID = "RSUID123";
    CString strPlateNo = "RSUPlateNo123456";
    DE_DSecond secMark = 0;

    DE_SteeringWheelAngle angle = 20;
    ptcData.SetParameter(DE_PT_Motor, 1, DE_ST_V2X, secMark, &posOffsetLLV, &PosAccuracy, speed, heading, &vehicleSize);
    ptcData.SetParameter(strRSUID, strPlateNo, transState, angle, &motionCfdSet, &accSet4Way, &vehicleClass);
    
    ptcList.AddListItem(&ptcData);

    DE_TimeOffset timeOffset = 1;
    DE_CoarseHeading coarseheading = 200;

    pathHistoryPoint.SetParameter(&posOffsetLLV, timeOffset, speed, &posCfdSet, coarseheading);
    pathHistoryPointList.AddListItem(&pathHistoryPoint);

    pathHistory.SetParameter(&pathHistoryPointList, DE_GNSS_IsHealthy, &fullPositionVector);

    pathPointList.AddListItem(&posOffsetLLV);
    pathPointList.AddListItem(&posOffsetLLV);
    
    pathPrediction.SetParameter(21, 23);

    signalState.SetParameter(DE_ISO_failureFlash, &phaseList, 21, 121);

    DE_VehicleEventFlags event = DE_VEF_TractionControlLoss;
    DE_ExteriorLights    light = DE_EL_DaytimeRunningLightsOn;
    
    vehicleSafetyExt.SetParameter(event, DE_VehicleEventFlags_MIN_LEN, &pathHistory, &pathPrediction, light, DE_ExteriorLights_MIN_LEN);

    DE_MsgCount msgCount = 1;

    CString strVehicleID = "Vehicle0";
    
    bsm.SetParameter(msgCount, strVehicleID, secMark, &position3D, &posCfdSet, transState, speed, heading, &accSet4Way, &brakeSystemStatus,
        &vehicleSize, &vehicleClass, strPlateNo, angle, &motionCfdSet, &vehicleSafetyExt);

    mapData.SetParameter(msgCount, &nodeList, 20);

    UINT alertType = DE_ALERT_TYPE_DANGER;
    DE_Priority priority = 0b01100000;
    DE_Radius alertRadius = 100;
    CString strDesc = "Description";
    
    rsi.SetParameter(msgCount, strRSUID, 2, alertType, &position3D, &pathPointList, alertRadius, timeStamp, strDesc, priority);

    rsm.SetParameter(msgCount, strRSUID, &position3D, &ptcList);

    spat.SetParameter(msgCount, &intersectionStateList, timeStamp, strDesc);
}

void CCSAETest(CCSAEMsgBase* pSrc, CCSAEMsgBase* pDest)
{
    PUCHAR pBuffer;
    USHORT nBitLen;

    if ( pSrc->Encode() == CODEC_FAILED )
    {
        LogI("Encode %s failed.", typeid(*pSrc).name());
        return;
    }

    LogI("Encode:\n");
    pSrc->toParamString();
    LogI(" \n");

    pBuffer = pSrc->GetBuffer(nBitLen);
    
    // LogI("Buffer Bitlen: %d,  %s", nBitLen, ConvertToBit(pBuffer, nBitLen));

    if ( pDest->Decode(pBuffer, nBitLen) == CODEC_FAILED)
    {
        LogI("Decode %s failed.", typeid(*pDest).name());
        return;
    }

    LogI("Decode:\n");
    pDest->toParamString();
    return; 
}

void CDF_NodeReferenceID::Test()
{
    CDF_NodeReferenceID obj;
    CCSAETest(&nodeReferenceID, &obj);
    return;
}

void CDF_Position3D::Test()
{
    CDF_Position3D obj;
    CCSAETest(&position3D, &obj);
    return;
}

void CDF_PositionConfidenceSet::Test()
{
    CDF_PositionConfidenceSet obj;
    CCSAETest(&posCfdSet, &obj);
    return;
}

void CDF_MotionConfidenceSet::Test()
{
    CDF_MotionConfidenceSet obj;
    CCSAETest(&motionCfdSet, &obj);
    return;
}

void CDF_TimeChangeDetails::Test()
{
    CDF_TimeChangeDetails obj;
    CCSAETest(&timeChangeDetails, &obj);
    return;
}

void CDF_PhaseState::Test()
{
    CDF_PhaseState obj;
    CCSAETest(&phaseState, &obj);
    return;
}

void CDF_PhaseStateList::Test()
{
    CDF_PhaseStateList obj;
    CCSAETest(&phaseStateList, &obj);
    return;
}

void CDF_Phase::Test()
{
    CDF_Phase obj;
    CCSAETest(&phase, &obj);
    return;
}

void CDF_PhaseList::Test()
{
    CDF_PhaseList obj;
    CCSAETest(&phaseList, &obj);
    return;
}

void CDF_RegulatorySpeedLimit::Test()
{
    CDF_RegulatorySpeedLimit obj;
    CCSAETest(&speedLimit, &obj);
    return;
}

void CDF_SpeedLimitList::Test()
{
    CDF_SpeedLimitList obj;
    CCSAETest(&speedLimitList, &obj);
    return;
}

void CDF_PositionOffsetLL::Test()
{
    posOffsetLL.SetObject(&posLL64);

    CDF_PositionOffsetLL obj;
    CCSAETest(&posOffsetLL, &obj);
    return;
}

void CDF_VerticalOffset::Test()
{
    CDF_VerticalOffset obj;
    CCSAETest(&verticalOffset, &obj);
    return;
}

void CDF_PositionOffsetLLV::Test()
{
    CDF_PositionOffsetLLV obj;
    CCSAETest(&posOffsetLLV, &obj);
    return;
}

void CDF_RoadPoint::Test()
{
    CDF_RoadPoint obj;
    CCSAETest(&roadPoint, &obj);
    return;
}

void CDF_PointList::Test()
{
    CDF_PointList obj;
    CCSAETest(&pointList, &obj);
    return;
}

void CDF_AccelerationSet4Way::Test()
{
    CDF_AccelerationSet4Way obj;
    CCSAETest(&accSet4Way, &obj);
    return;
}

void CDF_BrakeSystemStatus::Test()
{
    CDF_BrakeSystemStatus obj;
    CCSAETest(&brakeSystemStatus, &obj);
    return;
}

void CDF_ConnectingLane::Test()
{
    CDF_ConnectingLane obj;
    CCSAETest(&connectingLane, &obj);
    return;
}

void CDF_Connection::Test()
{
    CDF_Connection obj;
    CCSAETest(&conn, &obj);
    return;
}

void CDF_ConnectsToList::Test()
{
    CDF_ConnectsToList obj;
    CCSAETest(&connList, &obj);
    return;
}

void CDF_DDateTime::Test()
{
    CDF_DDateTime obj;
    CCSAETest(&datetime, &obj);
    return;
}

void CDF_FullPositionVector::Test()
{
    CDF_FullPositionVector obj;
    CCSAETest(&fullPositionVector, &obj);
    return;
}

void CDF_IntersectionState::Test()
{
    CDF_IntersectionState obj;
    CCSAETest(&intersectionState, &obj);
    return;
}

void CDF_IntersectionStateList::Test()
{
    CDF_IntersectionStateList obj;
    CCSAETest(&intersectionStateList, &obj);
    return;
}

void CDF_LaneTypeAttributes::Test()
{
    CDF_LaneTypeAttributes obj;
    CCSAETest(&laneTypeAttr, &obj);
    return;
}

void CDF_LaneAttributes::Test()
{
    CDF_LaneAttributes obj;
    CCSAETest(&laneAttr, &obj);
    return;
}

void CDF_Lane::Test()
{
    CDF_Lane obj;
    CCSAETest(&lane, &obj);
    return;
}

void CDF_LaneList::Test()
{
    CDF_LaneList obj;
    CCSAETest(&laneList, &obj);
    return;
}

void CDF_Movement::Test()
{
    CDF_Movement obj;
    CCSAETest(&movement, &obj);
    return;
}

void CDF_MovementList::Test()
{
    CDF_MovementList obj;
    CCSAETest(&movementList, &obj);
    return;
}

void CDF_Link::Test()
{
    CDF_Link obj;
    CCSAETest(&cdfLink, &obj);
    return;
}

void CDF_LinkList::Test()
{
    CDF_LinkList obj;
    CCSAETest(&linkList, &obj);
    return;
}

void CDF_Node::Test()
{
    CDF_Node obj;
    CCSAETest(&node, &obj);
    return;
}

void CDF_NodeList::Test()
{
    CDF_NodeList obj;
    CCSAETest(&nodeList, &obj);
    return;
}

void CDF_VehicleClassification::Test()
{
    CDF_VehicleClassification obj;
    CCSAETest(&vehicleClass, &obj);
    return;
}

void CDF_VehicleSize::Test()
{
    CDF_VehicleSize obj;
    CCSAETest(&vehicleSize, &obj);
    return;
}

void CDF_ParticipantData::Test()
{
    CDF_ParticipantData obj;
    CCSAETest(&ptcData, &obj);
    return;
}

void CDF_ParticipantList::Test()
{
    CDF_ParticipantList obj;
    CCSAETest(&ptcList, &obj);
    return;
}


void CDF_PathHistoryPoint::Test()
{
    CDF_PathHistoryPoint obj;
    CCSAETest(&pathHistoryPoint, &obj);
    return;
}

void CDF_PathHistoryPointList::Test()
{
    CDF_PathHistoryPointList obj;
    CCSAETest(&pathHistoryPointList, &obj);
    return;
}

void CDF_PathHistory::Test()
{
    CDF_PathHistory obj;
    CCSAETest(&pathHistory, &obj);
    return;
}


void CDF_PathPointList::Test()
{
    CDF_PathPointList obj;
    CCSAETest(&pathPointList, &obj);
    return;
}

void CDF_PathPrediction::Test()
{
    CDF_PathPrediction obj;
    CCSAETest(&pathPrediction, &obj);
    return;
}

void CDF_SignalState::Test()
{
    CDF_SignalState obj;
    CCSAETest(&signalState, &obj);
    return;
}

void CDF_VehicleSafetyExtensions::Test()
{
    CDF_VehicleSafetyExtensions obj;
    CCSAETest(&vehicleSafetyExt, &obj);
    return; 
}


void CBasicSafetyMessage::Test()
{
    CBasicSafetyMessage obj;
    CCSAETest(&bsm, &obj);
    return;
}


void CMapData::Test()
{
    CMapData obj;
    CCSAETest(&mapData, &obj);
    return;
}

void CRoadSideInformation::Test()
{
    CRoadSideInformation obj;
    CCSAETest(&rsi, &obj);
    return;
}

void CRoadSideSafetyMessage::Test()
{
    CRoadSideSafetyMessage obj;
    CCSAETest(&rsm, &obj);
    return;
}

void CSPAT::Test()
{
    CSPAT obj;
    CCSAETest(&spat, &obj);
    return;
}

void CMainMsgFrame::Test()
{
    CCSAEMsgBase* pMsg[5];
    pMsg[0] = &bsm;
    pMsg[1] = &mapData;
    pMsg[2] = &rsi;
    pMsg[3] = &rsm;
    pMsg[4] = &spat;

    for(int i = 0; i < 5; i++)
    {
        LogI("\n\nCCSAE Message Test: %s\n", GetClassName(pMsg[i]));
        mmf.SetObject(pMsg[i]);
        CMainMsgFrame obj;
        CCSAETest(&mmf, &obj);
    }

    return;
}
