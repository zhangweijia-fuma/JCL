/*------------------------------------------------------------------------------
 * JCCL - Component :: Module base
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    ModuleBase.cpp
 * Purpose: ModuleBase, 规定了模块的基本功能，可以作为客户端，也可以作为服务端
 *----------------------------------------------------------------------------*/

#include "Common.h"
#include "Timer.h"
#include "Config.h"

#include "ModuleBase.h"

CModuleServerInfo::CModuleServerInfo()
{
    m_strModuleCode         = "";
    m_strSrvAddr            = "";
    m_nSrvPort              = INVALID_SOCKET_PORT;
    m_nServerModuleType     = MODULE_UNKNOWN;

    m_nHeartbeatExpireTimes = 0;
    m_bIsConnectedApp       = false;
}


void CModuleServerInfo::Initial(CString strModuleCode, CString strServerAddr, UINT nSrvPort, UINT nModuleType)
{
    m_strModuleCode         = strModuleCode;
    m_strSrvAddr            = strServerAddr;
    m_nSrvPort              = nSrvPort;
    m_nServerModuleType     = nModuleType;

    m_nHeartbeatExpireTimes = 0;
    m_bIsConnectedApp       = false;
}


// /////////////////////////////////////////////////////////////////////

CModuleMsgClient::CModuleMsgClient()
{
    m_ServerList.SetMode(true);
}

// 初始化消息客户端
bool CModuleMsgClient::Initial(UINT nLocalPort, CString ModuleCode, UINT nModuleType, CString srvAddr, UINT nSrvPort, int nSocketType, RECEIVE_DATA_CALLBACK callback, LPVOID pParam)
{
    if ( CMsgClientBase::Initial(nLocalPort, ModuleCode, nModuleType, srvAddr, nSrvPort, nSocketType, callback, pParam) == false )
    {
        // LogE("Initial CModuleMsgClient failed.");
        return false;
    }

    // 添加服务端信息
    if ( srvAddr != "" )
    {
        AddServer( "", srvAddr, nSrvPort, nModuleType);
    }

    // 如果服务端地址为空，则在得到服务端的初始化请求响应消息后再添加服务端信息
    return true;
}

CModuleServerInfo* CModuleMsgClient::AddServer(CString strModuleCode, CString strServerAddr, UINT nSrvPort, UINT nModuleType)
{
    CModuleServerInfo* pServerInfo = FindServer(strServerAddr, nSrvPort);

    if ( pServerInfo != NULL )
    {
        LogI("Server %s(%s:%d already in server list", GetModuleTypeStr(nModuleType), strServerAddr.GetBuffer(), nSrvPort);
        return pServerInfo;
    }

    try
    {
        // 在内存紧张的使用场合，可能会存在无法申请内存的情况
        pServerInfo = new CModuleServerInfo();
        if ( pServerInfo == NULL )
        {
            return NULL;
        }

        pServerInfo->Initial(strModuleCode, strServerAddr, nSrvPort, nModuleType);
        m_ServerList.AddTail(pServerInfo);
        return pServerInfo;
    }
    catch(...)
    {
        LogE("Can not apply memory in CModuleMsgClient for Server Info");
        return NULL;
    }
}


CModuleServerInfo* CModuleMsgClient::FindServer(CString strServerAddr, UINT nServerPort)
{
    CModuleServerInfo* pServerInfo = (CModuleServerInfo*)m_ServerList.GetHead();;

    // 如果未设置服务端地址，则默认为第一个服务端
    if ( strServerAddr == "" )
        return pServerInfo;

    while ( pServerInfo != NULL )
    {
        if ( pServerInfo->GetServerAddr() == strServerAddr && pServerInfo->GetServerPort() == nServerPort)
            return pServerInfo;

        pServerInfo = (CModuleServerInfo*)m_ServerList.GetNext(pServerInfo);
    }

    return pServerInfo;
}

void CModuleMsgClient::RemoveServer(CString strServerAddr, UINT nServerPort)
{
    CModuleServerInfo* pServerInfo = FindServer(strServerAddr, nServerPort);
    if ( pServerInfo != NULL)
        m_ServerList.Remove(pServerInfo);
}

UINT CModuleMsgClient::GetServerModuleType(CString strServerAddr, UINT nServerPort)
{
    CModuleServerInfo* pServerInfo = NULL;
    pServerInfo = FindServer(strServerAddr, nServerPort);
    if ( pServerInfo != NULL )
        return  pServerInfo->GetServerModuleType();

    return MODULE_UNKNOWN;
}

void CModuleMsgClient::SetServerModuleType(UINT nModuleType, CString strServerAddr, UINT nServerPort)
{
    CModuleServerInfo* pServerInfo = NULL;
    pServerInfo = FindServer(strServerAddr, nServerPort);
    if ( pServerInfo != NULL )
        pServerInfo->SetServerModuleType(nModuleType);

    return;
}

UINT CModuleMsgClient::GetExpireTimes(CString strServerAddr, UINT nServerPort)
{
    CModuleServerInfo* pServerInfo = NULL;
    pServerInfo = FindServer(strServerAddr, nServerPort);
    if ( pServerInfo != NULL )
        return  pServerInfo->GetExpireTimes();

    return 0;
}

void CModuleMsgClient::SetExpireTimes(CString strServerAddr, UINT nServerPort)
{
    CModuleServerInfo* pServerInfo = NULL;
    pServerInfo = FindServer(strServerAddr, nServerPort);
    if ( pServerInfo != NULL )
        pServerInfo->SetExpireTimes();

    return;
}

void CModuleMsgClient::ResetExpireTimres(CString strServerAddr, UINT nServerPort)
{
    CModuleServerInfo* pServerInfo = NULL;
    pServerInfo = FindServer(strServerAddr, nServerPort);
    if ( pServerInfo != NULL )
        pServerInfo->ResetExpireTimres();

    return;
}

void CModuleMsgClient::SetAppConnectStatus(bool bIsConnected, CString strServerAddr, UINT nServerPort)
{
    CModuleServerInfo* pServerInfo = NULL;
    pServerInfo = FindServer(strServerAddr, nServerPort);
    if ( pServerInfo != NULL )
        pServerInfo->SetAppConnectStatus(bIsConnected);

    return;
}

bool CModuleMsgClient::GetAppConnectStatus(CString strServerAddr, UINT nServerPort)
{
    CModuleServerInfo* pServerInfo = NULL;
    pServerInfo = FindServer(strServerAddr, nServerPort);
    if ( pServerInfo != NULL )
        pServerInfo->GetAppConnectStatus();

    return false;
}

// /////////////////////////////////////////////////////////////////////

CModuleBase::CModuleBase()
{
    m_strModuleCode             = "";
    m_nModuleType               = MODULE_UNKNOWN;
    m_strModuleName             = "";

    m_nCheckHeartbeatTimerID    = INVALID_TIMER_ID;

    m_strLocalAddr              = "";

    m_bIsKeepConnection         = true;

    m_nUpdateMode               = Update_Immediately;
    m_nRebootMode               = Reboot_Application;
    
    m_nRebootTimer              = INVALID_TIMER_ID;

    m_strFTPServerAddr          = "";
    m_strFTPUserName            = "";
    m_strFTPPassword            = "";
}

bool CModuleBase::InitialSystem()
{
    // 首先检查是否需要进行升级操作 
    if ( CheckUpdate() == true )
        return false;

    // 创建UDP客户端，用于无连接的UDP通信
    if ( m_MsgClient.Initial( INVALID_SOCKET_PORT, m_strModuleCode, m_nModuleType, "", INVALID_SOCKET_PORT, SOCK_DGRAM, CModuleBase::OnMessage, this) == false )
    {
        LogE("Create Module UDP Message socket failed.");
        return false;
    }

    // LogI("Create Module UDP Message socket successfully.");

    // FTP服务器地址及用户名密码由派生类在初始化前提供
    if ( m_strFTPServerAddr != "" && m_strFTPUserName != "" && m_strFTPPassword != "" )
    {
        if ( m_DownloadFile.Initial(m_strFTPServerAddr, m_strFTPUserName, m_strFTPPassword, CModuleBase::DownloadCallBack, this) == false )
        {
            LogE("Initial download file thread failed.");
            return false;    
        }

        // LogI("Create download file thread successfully.");
    }

    // 设置一个定时器，定时检查系统运行环境
    m_nCheckEnvTimer = g_Timer.CreateTimer( CHECH_SYSTEM_ENVIRONMENT_TIME * 1000, CModuleBase::TimerEvent, this );
    if ( m_nCheckEnvTimer == INVALID_TIMER_ID )
    {
        LogE("Create Check System Environment Timer failed.");
        return false;
    }

    // LogI("Create Check System Environment Timer successfully.");
    return true;
}

// 检查是否需要更新
// 具体方式需要再考虑
// 软件升级包的组成为固定模式：
// 1.是一个压缩包，解压后得到一个目录，目录名称和压缩包文件名一致
// 2.在这个目录中，将有一个install.sh的脚本文件
// 3.具体升级过程，由这个脚本文件来处理
// 4.升级完成后应用的重启由watchdog.sh 脚本完成
// 
// 这样不同的软件升级方式由各自的软件包决定
// 
// 返回：
//  True：表示需要升级，应用将退出运行；
//  False：不需要升级，应用继续运行
bool CModuleBase::CheckUpdate()
{
    CString filename = g_strAppRootPath;
    filename += "/conf/Update.conf";

    CConfig configFile(filename);

    CString strFile = configFile.GetConfigStr("Update", "UpdateFile", "");
    if ( strFile == "" )
    {
        // 有配置文件，但配置文件参数错误
        // LogE("Can not find parameter UpdateFile from update config file!");
        return false;
    }

    // 在配置文件的软件包只有文件名，不含路径。路径固定为 update目录
    LogI("Infor: System will update software. software package file: %s", strFile.GetBuffer());

    // 删除此升级配置文件
    if ( remove(filename.GetBuffer()) == -1 )
    {
        LogE("Delete update config file %s failed. Error no: %d, %s", filename.GetBuffer(), errno, strerror);
        return false;
    }

    LogI("Update software ...");

    // 在Shell脚本中会重新启动操作系统或应用软件

    CString strCommand;
    strCommand.Format("%s/bin/systemUpdate.sh JamesSoft %s& 1>/dev/null 2>&1", g_strAppRootPath.GetBuffer(), strFile.GetBuffer());
    LogI("Update Command: %s", strCommand.GetBuffer());

    system(strCommand.GetBuffer());
    return true;
}

// 启动消息Socket线程
bool CModuleBase::StartSystem()
{
    if ( m_bIsKeepConnection )
    {
        // 创建定时器，定时发送心跳消息到服务端，或由服务端定时检查客户端的连接是否正常
        m_nCheckHeartbeatTimerID = g_Timer.CreateTimer( MSG_HEARTBEAT_INTERVAL * 1000, CModuleBase::TimerEvent, this );
        if ( m_nCheckHeartbeatTimerID == INVALID_TIMER_ID )
        {
            LogE("Create CheckHeartbeatTimer timer failed.");
            return false;
        }

        // LogI("Create heartbeat timer successfully.");
    }

    // 启动所有客户端的Socket
    int nCount = 0;
    CModuleMsgClient* pClient = (CModuleMsgClient*)m_ClientList.GetHead();
    while(pClient != NULL )
    {
        if ( pClient->RunClient() == false )
        {
            LogE("Start msg client failed! ");
            return false;
        }

        nCount++;

        pClient = (CModuleMsgClient*)m_ClientList.GetNext(pClient);
    }

    // 启动所有服务端的Socket
    CModuleMsgServer* pServer = (CModuleMsgServer*)m_ServerList.GetHead();
    while(pServer != NULL )
    {
        if ( pServer->RunServer() == false )
        {
            LogE("Start msg server failed! ");
            return false;
        }

        pServer = (CModuleMsgServer*)m_ServerList.GetNext(pServer);
    }

    return true;
}


void CModuleBase::TimerEvent(TIMER_ID nTimerID, LPVOID pParam)
{
    if ( pParam == NULL )
        return;

    CModuleBase *pModuleBase = (CModuleBase*)pParam;
    try 
    {
        // 如果不能获取类名称，则认为此指针错误
        // 此时获取的是派生类的类名
        CString className = typeid(*pModuleBase).name();

        // 如果获取名称成功，则默认此指针为派生类，正常进行
        // 此时不会检查该指针的实例是否为CModuleBase的派生类
        // 此处会调用派生类的函数，如果派生类没有重新实现该函数，则调用基类该函数
        pModuleBase->OnTimer(nTimerID);
    }
    catch(...)
    {
        LogE("CModuleBase::TimerEvent parameters None CModuleBase pointer");
        return;
    }
}

// 定时器处理，派生类需要首先调用基类的OnTimer函数
void CModuleBase::OnTimer(TIMER_ID nTimerID)
{
    if ( nTimerID == m_nCheckEnvTimer )
        OnCheckSystemEnv();
    else if ( nTimerID != m_nCheckHeartbeatTimerID)
        return;

    // 如果客户端和服务端没有心跳连接，则不需要检查
    if ( m_bIsKeepConnection == false )
        return;

    // 检查客户端的连接是否正常
    CheckConnection();

    // 是否有初始化未成功的模块
    bool bHasUnInitialed = false;

    // 检查到服务端的连接是否正常
    CModuleMsgClient* pClient = (CModuleMsgClient*)m_ClientList.GetHead();
    while ( pClient != NULL )
    {
        // 检查客户端连接的每个服务端的连接情况
        CModuleServerInfo* pServerInfo = (CModuleServerInfo*)pClient->m_ServerList.GetHead();
        while( pServerInfo != NULL )
        {
            if ( pServerInfo->GetAppConnectStatus() == true )
            {
                // LogI("Expire %d", pServerInfo->GetExpireTimes());
                pServerInfo->SetExpireTimes();

                if ( pServerInfo->GetExpireTimes() > MSG_EXPIRE_TIMES )
                {
                    // 失去了对服务端的连接，需要重新发送InitialRequestMsg
                    LogE("Lost connection to server %s %s:%d", GetModuleTypeStr(pServerInfo->GetServerModuleType()), 
                        pServerInfo->GetServerAddr().GetBuffer(), pServerInfo->GetServerPort());

                    // 将服务端信息从列表中删除
                    // pClient->GetServerList()->Remove(pServerInfo);
                    pServerInfo->SetAppConnectStatus(false);
                    pServerInfo->ResetExpireTimres();
                    bHasUnInitialed = true;
                }
                else
                {
                    // 连接正常，发送心跳消息
                    SendHeartbeatMsg(pServerInfo->GetServerAddr());
                }
            }
            else
            {
                // 此时还没有建立连接，需要发送初始化请求消息
                bHasUnInitialed = true;
            }

            pServerInfo = (CModuleServerInfo*)pClient->m_ServerList.GetNext(pServerInfo);
        }

        pClient = (CModuleMsgClient*)m_ClientList.GetNext(pClient);
    }

    // 只要有未初始化成功的模块，就发送初始化请求消息
    // 发送初始化请求消息由派生类实现
    if ( bHasUnInitialed == true)
        SendInitialRequestMsg();
    
    return;
}

// 定时检查系统的运行环境
// 定时检查系统的占用空间，以避免因为存储满导致系统崩溃
// 检查内容：
//  检查运行日志文件，当运行日志文件超过最大值后，对运行日志文件压缩并上传到服务端
//  压缩文件命名规则：DeviceCode-YYYYMMDD-HHMMSS.log.tar.gz
//  服务端会将压缩文件根据设备归属的项目移动到 /log/项目名称 目录中
void CModuleBase::OnCheckSystemEnv()
{
    // 在Shell脚本中进行检查和FTP上传
    CString strCommand;
    strCommand.Format("%s/bin/scheduleTask.sh JamesSoft %s %s %s %s log", g_strAppRootPath.GetBuffer(), m_strModuleCode.GetBuffer(),
        m_strFTPServerAddr.GetBuffer(), m_strFTPUserName.GetBuffer(), m_strFTPPassword.GetBuffer());
    
    system(strCommand.GetBuffer());
    return;
}

// 下载任务完成后处理
void CModuleBase::DownloadCallBack(LPVOID pCallbackParam, UINT nMsgType, CString strLocalFile)
{
    if ( pCallbackParam == NULL )
        return;

    CModuleBase *pModuleBase = (CModuleBase*)pCallbackParam;
    try 
    {
        // 如果不能获取类名称，则认为此指针错误
        // 此时获取的是派生类的类名
        CString className = typeid(*pModuleBase).name();

        // 如果获取名称成功，则默认此指针为派生类，正常进行
        // 此时不会检查该指针的实例是否为CModuleBase的派生类
        // 此处会调用派生类的函数，如果派生类没有重新实现该函数，则调用基类该函数
        pModuleBase->OnDownloadComplete(nMsgType, strLocalFile);
    }
    catch(...)
    {
        LogE("CModuleBase::TimerEvent parameters None CModuleBase pointer");
        return;
    }
}

// 下载完成后，由派生类进行具体的操作，基类不处理
void CModuleBase::OnDownloadComplete(UINT nMsgType, CString strLocalFile)
{
    LogI("Download Task Complete: MsgType: %04X, LocalFile: %s", nMsgType, strLocalFile.GetFileName());
}


// 检查客户端的连接是否正常
void CModuleBase::CheckConnection()
{
    m_ModuleList.CheckHeartbeat();
}

// Message callback
void CModuleBase::OnMessage(LPVOID pCallbackParam, PUCHAR pBuffer, UINT nMsgLen, LPCSTR strSrcAddr, UINT nSrcPort)
{
    LogD("CModuleBase::OnMessage Receive data from %s:%d", strSrcAddr, nSrcPort);

    if ( pCallbackParam == NULL )
        return;

    CModuleBase *pModuleServer = (CModuleBase*)pCallbackParam;
    try 
    {
        // 如果不能获取类名称，则认为此指针错误
        CString className = typeid(*pModuleServer).name();

        // 如果获取名称成功，则默认此指针为派生类，正常进行
        // 此时不会检查该指针的实例是否为CModuleBase的派生类
        // 此处会调用派生类的函数，如果派生类没有重新实现该函数，则调用基类该函数
        pModuleServer->DecodeMessage(pBuffer, nMsgLen, strSrcAddr, nSrcPort);
    }
    catch(...)
    {
        LogE("CModuleBase::OnMessage parameters None CModuleBase pointer");
        return;
    }
}

// 接收从客户端来的心跳消息
void CModuleBase::ProcessHeartbeatMsg(CMsgBase* pMsg)
{
    CString srcAddr = pMsg->GetSrcAddr();
    UINT nSrcPort   = pMsg->GetSrcPort();

    // 根据源IP地址及端口，查找是否有在线模块，如果找到则设置心跳超时次数为0.
    CModuleInfo * pModuleInfo = m_ModuleList.Find( srcAddr, nSrcPort);
    if ( pModuleInfo != NULL )
    {
        pModuleInfo->m_nHeartbeatExpireTimes = 0;
    }
    else
    {
        // 没有找到源模块
        LogE("Warning: Receive Heartbeat msg from offline module %s: %s:%d", pMsg->GetSrcModuleName(), srcAddr.GetBuffer(), nSrcPort);

        // 添加一个模块信息到队列中
        CModuleInfo* pModuleInfo = NULL;

        pModuleInfo = m_ModuleList.FindIdle();
        if ( pModuleInfo == NULL )
        {
            LogE("Connected module count exceed system limit %d.", MAX_MODULE_COUNT);
            return;
        }

        pModuleInfo->m_bIsOnline              = true;
        pModuleInfo->m_strPeerModuleAddr      = pMsg->GetSrcAddr();
        pModuleInfo->m_nPeerModulePort        = pMsg->GetSrcPort();
        pModuleInfo->m_nPeerModuleType        = pMsg->GetSrcModuleType();
        pModuleInfo->m_nHeartbeatExpireTimes  = 0;

        LogI("Module %s(%s:%d) Online.", pMsg->GetSrcModuleName(), pMsg->GetSrcAddr(), pMsg->GetSrcPort());
    }

    CHeartBeatAckMsg msg;

    msg.SetDestAddr(pMsg->GetSrcAddr());
    msg.SetDestPort(pMsg->GetSrcPort());
    msg.SetDestModuleType(pMsg->GetSrcModuleType());

    msg.SetSrcPort(pMsg->GetDestPort());

    SendMessageToClient(&msg);
}

// 处理从服务端来的心跳响应消息
// 值需要把消息超时次数复位即可
void CModuleBase::ProcessHeartbeatAckMsg(CMsgBase* pMsg)
{
    CString srcAddr = pMsg->GetSrcAddr();
    UINT nSrcPort   = pMsg->GetSrcPort();

    // 根据源IP地址及端口，查找对应的客户端Socket
    CModuleMsgClient* pClient = (CModuleMsgClient*)m_ClientList.GetHead();
    while(pClient != NULL)
    {
        // 匹配客户连接的服务端地址
        CModuleServerInfo* pServerInfo = pClient->FindServer(srcAddr, nSrcPort);
        if( pServerInfo != NULL && pServerInfo->GetServerPort() == pMsg->GetSrcPort() )
        {
            pServerInfo->ResetExpireTimres();
            return;
        }

        pClient = (CModuleMsgClient*)m_ClientList.GetNext(pClient);
    }

    LogE("can not find server %s:%d", srcAddr.GetBuffer(), nSrcPort);
}

// 处理客户端发出的初始化请求消息
bool CModuleBase::ProcessInitialRequestMsg(CMsgBase* pMsg)
{
    if ( pMsg == NULL )
        return false;

    CModuleInfo* pModuleInfo = NULL;

    // 检查具有相同地址和端口的模块是否已经在线
    // 由于客户端的端口可能不固定，因此在同一个客户端，在重新启动以后，
    // 由于端口已经不同，所以是找不到原来的信息的，会作为新的业务模块对待。
    // 原有的连接信息会在超时后从队列中删除
    if ( m_ModuleList.Find( pMsg->GetSrcAddr(), pMsg->GetSrcPort() ) != NULL )
    {
        LogI("Info: Receive InitialRequestMsg from %s(%s:%d) in online status.", 
            GetModuleTypeStr( pMsg->GetSrcModuleType()), pMsg->GetSrcAddr(), pMsg->GetSrcPort());

        // 不回复初始化请求响应消息
        return false;
    }

    pModuleInfo = m_ModuleList.FindIdle();
    if ( pModuleInfo == NULL )
    {
        LogE("Connected module count exceed system limit %d.", MAX_MODULE_COUNT);
        return false;
    }

    pModuleInfo->m_bIsOnline              = true;
    pModuleInfo->m_strPeerModuleAddr      = pMsg->GetSrcAddr();
    pModuleInfo->m_nPeerModulePort        = pMsg->GetSrcPort();
    pModuleInfo->m_nPeerModuleType        = pMsg->GetSrcModuleType();
    pModuleInfo->m_nHeartbeatExpireTimes  = 0;

    LogI("Module %s(%s:%d) Online.", pMsg->GetSrcModuleName(), pMsg->GetSrcAddr(), pMsg->GetSrcPort());

    // 基类处理结束，派生类必须继续对该消息进行处理
    return true;
}

// 处理从服务端来的初始化请求响应消息
// 在基类，只是将和服务端的连接状态更新一下，
// 派生类需要继续进行相应的处理
bool CModuleBase::ProcessInitialRequestAckMsg(CMsgBase* pMsg)
{
    if ( pMsg == NULL )
    {
        LogE("CModuleBase::ProcessInitialRequestAckMsg pMsg is NULL");
        return false;
    }

    CString srcAddr = pMsg->GetSrcAddr();
    UINT nSrcPort   = pMsg->GetSrcPort();

    // 根据消息中的目的端口，查找对应的客户端Socket
    CModuleMsgClient* pClient = (CModuleMsgClient*)m_ClientList.GetHead();
    while(pClient != NULL)
    {
        if ( pClient->GetLocalPort() != pMsg->GetDestPort() )
        {          
            pClient = (CModuleMsgClient*)m_ClientList.GetNext(pClient);
            continue;
        }

        // 如果服务端已经在客户端的服务端列表中，则不需要再添加服务端信息
        CModuleServerInfo* pServerInfo = pClient->FindServer(srcAddr, nSrcPort);
        if ( pServerInfo != NULL )
        {
            // 通常在初始化时，已经将服务端信息加入到列表中
            // 设置已经连接到服务端应用
            pServerInfo->SetAppConnectStatus(true);
            pServerInfo->ResetExpireTimres();
            LogI("Connected to %s %s:%d", GetModuleTypeStr(pMsg->GetSrcModuleType()), srcAddr.GetBuffer(), nSrcPort);

            return true;
        }

        // 新的服务端来的消息，针对服务端数量不确定的应用场景
        // 在通过广播方式寻找服务端的时候，会加入新的服务端信息
        pServerInfo = pClient->AddServer(pMsg->GetSrcModuleCode(), srcAddr, nSrcPort, pMsg->GetSrcModuleType());
        pServerInfo->SetAppConnectStatus(true);

        LogI("Connected to %s %s:%d", GetModuleTypeStr(pMsg->GetSrcModuleType()), srcAddr.GetBuffer(), nSrcPort);
        return true;
    }

    LogE("Can not find Server %s:%d", srcAddr.GetBuffer(), nSrcPort);

    return false;
}

// 作为客户端，向所有连接的服务端发送心跳消息
void CModuleBase::SendHeartbeatMsg(CString strServerAddr)
{
    CModuleMsgClient* pClient = (CModuleMsgClient*)m_ClientList.GetHead();
    while( pClient != NULL )
    {
        CModuleServerInfo* pServerInfo = (CModuleServerInfo*)pClient->m_ServerList.GetHead();
        while( pServerInfo != NULL )
        {
            // 如果指定了服务端地址，并且地址不匹配，则跳过
            if ( strServerAddr != "" && pServerInfo->GetServerAddr() != strServerAddr)
            {
                pServerInfo = (CModuleServerInfo*)pClient->m_ServerList.GetNext(pServerInfo); 
                continue;
            }

            // 服务端地址确定，但是和服务端的应用层连接此时还没有初始化成功，跳过
            if ( pServerInfo->GetAppConnectStatus() == false)
            {
                pServerInfo = (CModuleServerInfo*)pClient->m_ServerList.GetNext(pServerInfo); 
                continue;
            }

            CHeartBeatMsg msg;

            msg.SetDestAddr(pServerInfo->GetServerAddr());
            msg.SetDestPort(pServerInfo->GetServerPort());
            msg.SetDestModuleType(pServerInfo->GetServerModuleType());
            msg.SetDestModuleCode(pServerInfo->GetServerModuleCode());

            SendMessageToServer(&msg);
            pServerInfo = (CModuleServerInfo*)pClient->m_ServerList.GetNext(pServerInfo); 
        }

        pClient = (CModuleMsgClient*)m_ClientList.GetNext(pClient);
    }
}

// 发送消息给服务端
void CModuleBase::SendMessageToServer(CMsgBase* pMsg)
{
    if ( pMsg == NULL )
    {
        LogE("CModuleBase::SendMessage pMsg parameters is null!");
        return;
    }

    CString strDestAddr = pMsg->GetDestAddr();
    UINT nDestPort = pMsg->GetDestPort();

    // 根据接收地址从列表中查找对应的客户端Socket
    CModuleMsgClient* pMsgClient = (CModuleMsgClient*)m_ClientList.GetHead();
    while( pMsgClient != NULL )
    {
        CModuleServerInfo* pServerInfo = pMsgClient->FindServer(strDestAddr, nDestPort);
        if( pServerInfo == NULL )
        {
            pMsgClient = (CModuleMsgClient*)m_ClientList.GetNext(pMsgClient);
            continue;
        }

        pMsgClient->SendMessage(pMsg);
        return;
    }

    LogE("CModuleBase::SendMessageToServer Send %s failed! Can not find server address: %s", pMsg->toString(), strDestAddr.GetBuffer());
    return;
}

// 发送消息给客户端，根据服务端的端口号来区分不同的Socket
void CModuleBase::SendMessageToClient(CMsgBase* pMsg)
{
    if ( pMsg == NULL )
    {
        LogE("CModuleBase::SendMessage pMsg parameters is null!");
        return;
    }

    UINT nSrcPort = pMsg->GetSrcPort();

    CModuleMsgServer* pMsgServer = (CModuleMsgServer*)m_ServerList.GetHead();
    while( pMsgServer != NULL )
    {
        // 查找对应的服务端Socket
        if ( pMsgServer->GetServerPort() != nSrcPort )
        {
            pMsgServer = (CModuleMsgServer*)m_ServerList.GetNext(pMsgServer);
            continue;
        }

        pMsgServer->SendMessage(pMsg);
        return;
    }

    LogE("Send %s failed! Can not find server port: %d", pMsg->toString(), nSrcPort);
    return;
}

// 支持UDP消息
void CModuleBase::SendMessage(CMsgBase* pMsg)
{
    m_MsgClient.SendMessage(pMsg);
}


void CModuleBase::OnRebootSystem()
{
    // 系统立刻重启
    if ( m_nRebootMode == Reboot_System )
    {
        LogI("System will reboot immediately...");
    }
    else
    {
        LogI("Application will reload immediately...");
    }

    // 在Shell脚本中会重新启动操作系统或应用软件
    CString strCommand;
    strCommand.Format("%s/bin/systemReboot.sh %d", g_strAppRootPath.GetBuffer(), m_nRebootMode);
    
    system(strCommand.GetBuffer());
    return;
}
