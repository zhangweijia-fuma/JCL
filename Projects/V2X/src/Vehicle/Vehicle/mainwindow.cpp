#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "CSAEMessageSet.h"
#include "Debug.h"

extern CDF_NodeReferenceID nodeReferenceID;
extern CDF_Position3D position3D;
extern CDF_PositionConfidenceSet posCfdSet, PosAccuracy;
extern CDF_MotionConfidenceSet motionCfdSet;
extern CDF_TimeChangeDetails timeChangeDetails;
extern CDF_PhaseState phaseState;
extern CDF_PhaseStateList phaseStateList;
extern CDF_Phase phase;
extern CDF_PhaseList phaseList;
extern CDF_RegulatorySpeedLimit speedLimit;
extern CDF_SpeedLimitList speedLimitList;
extern CDF_PositionLL24B posLL24; 
extern CDF_PositionLL28B posLL28;
extern CDF_PositionLL32B posLL32;
extern CDF_PositionLL36B posLL36;
extern CDF_PositionLL44B posLL44;
extern CDF_PositionLL48B posLL48;
extern CDF_PositionLLmD_64B posLL64;
extern CDF_PositionOffsetLL posOffsetLL;
extern CDF_VerticalOffset verticalOffset;
extern CDF_PositionOffsetLLV posOffsetLLV;
extern CDF_RoadPoint roadPoint;
extern CDF_PointList pointList;
extern CDF_AccelerationSet4Way accSet4Way;
extern CDF_BrakeSystemStatus brakeSystemStatus;
extern CDF_ConnectingLane connectingLane;
extern CDF_Connection conn;
extern CDF_ConnectsToList connList;
extern CDF_DDateTime datetime;
extern CDF_FullPositionVector fullPositionVector;
extern CDF_IntersectionState intersectionState;
extern CDF_IntersectionStateList intersectionStateList;
extern CDF_LaneTypeAttributes laneTypeAttr;
extern CDF_LaneAttributes laneAttr;
extern CDF_Lane lane;
extern CDF_LaneList laneList;
extern CDF_Movement movement;
extern CDF_MovementList movementList;
extern CDF_Link cdfLink;
extern CDF_LinkList linkList;
extern CDF_Node node;
extern CDF_NodeList nodeList;
extern CDF_VehicleClassification vehicleClass;
extern CDF_VehicleSize vehicleSize;
extern CDF_ParticipantData ptcData;
extern CDF_ParticipantList ptcList;
extern CDF_PathHistoryPoint pathHistoryPoint;
extern CDF_PathHistoryPointList pathHistoryPointList;
extern CDF_PathHistory pathHistory;
extern CDF_PathPointList pathPointList;
extern CDF_PathPrediction pathPrediction;
extern CDF_SignalState signalState;
extern CDF_VehicleSafetyExtensions vehicleSafetyExt;

extern CBasicSafetyMessage bsm;
extern CMapData mapData;
extern CRoadSideInformation rsi;
extern CRoadSideSafetyMessage rsm;
extern CSPAT spat;
extern CMainMsgFrame mmf;

extern void SetTestData();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_Socket.Create(SOCK_DGRAM);
    m_nRSUPort = 11630;

    SetTestData();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_BSMBtn_clicked()
{
    SendMessage(0);
}

void MainWindow::on_MapBtn_clicked()
{
    SendMessage(1);
}

void MainWindow::on_RSIBtn_clicked()
{
    SendMessage(2);
}

void MainWindow::on_RSMBtn_clicked()
{
    SendMessage(3);
}

void MainWindow::on_SPATBtn_clicked()
{
    SendMessage(4);
}

void MainWindow::SendMessage(int nChoiceIndex)
{
    switch(nChoiceIndex)
    {
        case 0:
            mmf.SetObject(&bsm);
            break;
        case 1:
            mmf.SetObject(&mapData);
            break;
        case 2:
            mmf.SetObject(&rsi);
            break;
        case 3:
            mmf.SetObject(&rsm);
            break;
        case 4:
            mmf.SetObject(&spat);
            break;
        default:
            return;
    }

    if ( mmf.Encode() == false )
    {
        LogE("Error: CMainMsgFrame encode failed");
        return;
    }

    PUCHAR pBuffer;
    USHORT nBitLen;
    pBuffer = mmf.GetBuffer(nBitLen);

    UINT nMsgLen = nBitLen / 8 + 1;

    mmf.toParamString();

    m_Socket.Broadcast(pBuffer, nMsgLen, m_nRSUPort);
    
}

