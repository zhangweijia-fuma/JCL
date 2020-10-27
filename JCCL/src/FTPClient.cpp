// ------------------------------------------------------------------------------
// JCCL - Component :: FTP Client
// Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    FTPClient.cpp
// Purpose: Common Module
//-------------------------------------------------------------------------------


#include <sys/timeb.h>
#include <sys/time.h>

#include "FTPClient.h"
#include "MsgSocket.h"
#include "Debug.h"
#include "File.h"


// FTP客户的，支持下载和上传

// 除了错误信息外，FTPClient不输出任何信息，由上层调用者决定如何输出信息



CFTPClient::CFTPClient(void)
{
    m_bLogin = false;
    m_cmdSocket = socket(AF_INET, SOCK_STREAM, 0);

    Reset();
}

CFTPClient::~CFTPClient(void)
{
    if ( m_bLogin == false )
    {
        close(m_cmdSocket);
        return;
    }


    if ( ParseCommand(FTP_COMMAND_QUIT, "") == true )
    {
        Send(m_cmdSocket, m_commandStr);
        close(m_cmdSocket);
    }

    Reset();
}

void CFTPClient::Reset()
{
    m_strServerIP       = "";
    m_nServerPort       = FTP_DEFAULT_PORT;

    m_strUserName       = "";
    m_strPassWord       = "";

    m_nCurrentCommand   = FTP_COMMAND_BASE;
    m_strResponse       = "";
    m_commandStr        = "";

    m_bLogin            = false;
}

// 登录到FTP服务器
bool CFTPClient::LoginToServer(CString serverIP, CString userName, CString password, UINT serverPort)
{
    LogD("Connect to FTP Server: %s:%d", serverIP.GetBuffer(), serverPort);

    if (Connect(m_cmdSocket, serverIP, serverPort) == false )
        return false;

    GetServerResponse(m_cmdSocket);
    int nRetCode = ParseResponse(m_strResponse);
    if ( nRetCode != FTP_RSP_READY_FOR_NEW_USER )
    {
        LogE("Connect to Server failed: %s", m_strResponse.GetBuffer());
        return false;
    }

    if ( ParseCommand(FTP_COMMAND_USERNAME, userName) == false )
        return false;

    if (Send(m_cmdSocket, m_commandStr) < 0)
        return false;

    GetServerResponse(m_cmdSocket);
    nRetCode = ParseResponse(m_strResponse);
    if ( nRetCode != FTP_RSP_REQUIRE_PASSWD )
    {
        LogE("USER cmd failed: %s", m_strResponse.GetBuffer());
        return false;
    }

    if ( ParseCommand(FTP_COMMAND_PASSWORD, password) == false )
        return false;

    if (Send(m_cmdSocket, m_commandStr) < 0)
        return false;

    GetServerResponse(m_cmdSocket);
    nRetCode = ParseResponse(m_strResponse);
    if ( nRetCode != FTP_RSP_LOGIN_SUCCESS )
    {
        LogE("PASS cmd failed: %s", m_strResponse.GetBuffer());
        return false;
    }

    // LogI("Login to FTP server %s success", serverIP.GetBuffer());

    m_bLogin = true;
    m_strServerIP = serverIP;
    m_nServerPort = serverPort;

    m_strUserName = userName;
    m_strPassWord = password;

    return true;
}

// 登出FTP服务器
bool CFTPClient::QuitServer(void)
{
    if ( m_bLogin == false )
    {
        LogD("Not login to FTP server now.");
        return true;
    }

    if ( ParseCommand(FTP_COMMAND_QUIT, "") == false )
        return false;

    if (Send(m_cmdSocket, m_commandStr) < 0)
        return false;

    GetServerResponse(m_cmdSocket);
    int nRetCode = ParseResponse(m_strResponse);
    if ( nRetCode != FTP_RSP_QUIT_OK )
    {
        LogE("QUIT cmd failed: %d %s", nRetCode, m_strResponse.GetBuffer());
        return false;
    }

    LogD("Quit FTP server success.");
    Reset();

    return true;
}

// 设置FTP传输模式，二进制模式或文本模式
bool CFTPClient::SetTransferMode(TRANS_MODE mode)
{
    if ( m_bLogin == false )
    {
        LogI("Not login to FTP server now.");
        return false;
    }

    switch(mode)
    {
    case binary:
        ParseCommand(FTP_COMMAND_TYPE_MODE, "I");
        break;

    case ascii:
        ParseCommand(FTP_COMMAND_TYPE_MODE, "A");
        break;

    default:
        break;
    }

    if (Send(m_cmdSocket, m_commandStr) < 0)
        return false;

    GetServerResponse(m_cmdSocket);
    int nRetCode = ParseResponse(m_strResponse);
    if ( nRetCode != FTP_RSP_SUCCESS )
    {
        LogE("Set transfer mode failed, code: %d, desc: %s", nRetCode, m_strResponse.GetBuffer());
        return false;
    }

    LogD("Set transfer mode: %s", m_strResponse.GetBuffer());
    return true;
}

CString CFTPClient::Pasv()
{
    if ( m_bLogin == false )
    {
        LogI("Not login to FTP server now.");
        return "";
    }

    if ( ParseCommand(FTP_COMMAND_PSAV_MODE, "") == false )
        return "";

    if (Send(m_cmdSocket, m_commandStr) < 0)
        return "";

    GetServerResponse(m_cmdSocket);
    return m_strResponse;
}

CString CFTPClient::PWD()
{
    if ( m_bLogin == false )
    {
        LogI("Not login to FTP server now.");
        return "";
    }

    if ( ParseCommand(FTP_COMMAND_CURRENT_PATH, "") == false )
        return "";

    if (Send(m_cmdSocket, m_commandStr) < 0)
        return "";

    GetServerResponse(m_cmdSocket);
    LogDI("PWD Command: %s", m_strResponse.GetBuffer());

    int nRetCode = ParseResponse(m_strResponse);
    if ( nRetCode != FTP_RSP_CREATE_PATH )
    {
        LogE("PWD command failed, code: %d, desc: %s", nRetCode, m_strResponse.GetBuffer() );
    }

    LogD("FTP server current path: %s", m_strResponse.GetBuffer() );

    return m_strResponse;
}

// 列出服务器上指定目录
// 待测试
CString CFTPClient::Dir(CString path)
{
    LogE("FTP Dir function is waiting to test.");
    return "";

    if ( m_bLogin == false )
    {
        LogI("Not login to FTP server now.");
        return "";
    }

    if ( ParseCommand(FTP_COMMAND_DIR, path) == false )
        return "";

    int dataSocket = socket(AF_INET, SOCK_STREAM, 0);
 
    if (CreateDataLink(dataSocket) == false )
        return "";

    // 数据连接成功
    if (Send(m_cmdSocket, m_commandStr) < 0)
    {
        GetServerResponse(m_cmdSocket);
        LogE("Dir command failed: %s", m_strResponse.GetBuffer());
        close(dataSocket);
        return "";
    }
    else
    {
        GetServerResponse(m_cmdSocket);
        LogI("Dir Command Response: %s", m_strResponse.GetBuffer());
        
        GetServerResponse(dataSocket);
        LogI("Dir data Response: %s", m_strResponse.GetBuffer());
        close(dataSocket);

        return m_strResponse;
    }
}

// 更改服务器上的目录到指定目录
// path 为全路径
bool CFTPClient::CD(CString path)
{
    if ( m_bLogin == false )
    {
        LogI("Not login to FTP server now.");
        return false;
    }

    if ( ParseCommand(FTP_COMMAND_CHANGE_FOLDER, path) == false )
        return false;

    if (Send(m_cmdSocket, m_commandStr) < 0)
        return false;

    GetServerResponse(m_cmdSocket);
    LogDI("CD Command: %s", m_strResponse.GetBuffer());

    int nRetCode = ParseResponse(m_strResponse);
    if ( nRetCode != FTP_RSP_FILE_OP_COMPLETE )
    {
        // 有时候是未了测试目录是否存在，所以不输出告警
        LogE("Change FTP directory to %s failed, code: %d, desc: %s", path.GetBuffer(), nRetCode, m_strResponse.GetBuffer() );
        return false;
    }

    LogDI("Change FTP server path to %s.", path.GetBuffer());
    return true;
}

// 删除服务器上的文件
bool CFTPClient::DeleteFile(CString strRemoteFile)
{
    if ( m_bLogin == false )
    {
        LogI("Not login to FTP server now.");
        return false;
    }

    if ( ParseCommand(FTP_COMMAND_DELETE_FILE, strRemoteFile) == false )
        return false;

    if (Send(m_cmdSocket, m_commandStr) < 0)
        return false;

    GetServerResponse(m_cmdSocket);
    LogDI("DeleteFile Command: %s", m_strResponse.GetBuffer());

    int nRetCode = ParseResponse(m_strResponse);
    if ( nRetCode != FTP_RSP_FILE_OP_COMPLETE )
    {
        LogE("Delete file %s failed, code: %d, desc: %s", strRemoteFile.GetBuffer(), nRetCode, m_strResponse.GetBuffer() );
        return false;
    }

    LogD("Delete file %s from FTP server success.", strRemoteFile.GetBuffer());

    return true;
}

// 删除服务器上的目录
// 如果服务器目录中有文件则删除不成功
bool CFTPClient::DeleteDirectory(CString strRemoteDir)
{
    if ( m_bLogin == false )
    {
        LogI("Not login to FTP server now.");
        return false;
    }

    if ( ParseCommand(FTP_COMMAND_DELETE_FOLDER, strRemoteDir) == false )
        return false;

    if (Send(m_cmdSocket, m_commandStr) < 0)
        return false;

    GetServerResponse(m_cmdSocket);
    LogDI("DeleteDirectory Command: %s", m_strResponse.GetBuffer());

    int nRetCode = ParseResponse(m_strResponse);
    if ( nRetCode != FTP_RSP_FILE_OP_COMPLETE )
    {
        LogE("Delete directory %s failed, code: %d, desc: %s", strRemoteDir.GetBuffer(), nRetCode, m_strResponse.GetBuffer() );
        return false;
    }

    LogD("Delete FTP server folder $s success", strRemoteDir.GetBuffer());
    return true;
}

// 在服务器上创建目录
bool CFTPClient::CreateDirectory(CString strRemoteDir)
{
    if ( m_bLogin == false )
    {
        LogI("Not login to FTP server now.");
        return false;
    }

    if ( ParseCommand(FTP_COMMAND_CREATE_FOLDER, strRemoteDir) == false )
        return false;

    if (Send(m_cmdSocket, m_commandStr) < 0)
        return false;

    GetServerResponse(m_cmdSocket);
    LogDI("Create Directory Command Response: %s", m_strResponse.GetBuffer());

    int nRetCode = ParseResponse(m_strResponse);
    switch(nRetCode)
    {
    case FTP_RSP_CREATE_PATH:
    case FTP_RSP_FILE_OP_COMPLETE:
        LogD("FTP Client Create Directory %s success.", strRemoteDir.GetBuffer());
        return true;
    
    // 命令在服务器上未执行，可能是目录已存在
    case FTP_RSP_CMD_NOT_EXEC:
        LogD("FTP Client Create Directory cmd Not Execute in server, Path: %s", strRemoteDir.GetBuffer());
        return true;

    default:        
        LogE("Create Directory %s failed, code: %d, desc: %s", strRemoteDir.GetBuffer(), nRetCode, m_strResponse.GetBuffer() );
        return false;
    }

    return true;
}

// 重命名服务器上的文件
// 文件名不需要带路径
bool CFTPClient::Rename(CString strRemoteFile, CString strNewFile)
{
    if ( m_bLogin == false )
    {
        LogI("Not login to FTP server now.");
        return false;
    }

    if ( ParseCommand(FTP_COMMAND_RENAME_BEGIN, strRemoteFile) == false )
        return false;
    if ( Send(m_cmdSocket, m_commandStr) < 0 )
        return false;

    GetServerResponse(m_cmdSocket);
    LogDI("Rename Begin Command: %s", m_strResponse.GetBuffer());

    if ( ParseCommand(FTP_COMMAND_RENAME_END, strNewFile) == false )
        return false;
    if (Send(m_cmdSocket, m_commandStr) < 0)
        return false;
 
    GetServerResponse(m_cmdSocket);
    LogDI("Rename End Command: %s", m_strResponse.GetBuffer());
    if ( ParseResponse(m_strResponse) != FTP_RSP_FILE_OP_COMPLETE)
    {
        LogE("Rename FTP server file from %s to %s failed.", strRemoteFile.GetBuffer(), strNewFile.GetBuffer());
        return false;
    }

    LogD("Rename FTP server file from %s to %s success.", strRemoteFile.GetBuffer(), strNewFile.GetBuffer());
    return false;
}

long CFTPClient::GetFileLength(CString strRemoteFile)
{
    if ( m_bLogin == false )
    {
        LogI("Not login to FTP server now.");
        return 0;
    }

    if ( ParseCommand(FTP_COMMAND_FILE_SIZE, strRemoteFile) == false )
        return false;
    if (Send(m_cmdSocket, m_commandStr) < 0)
        return -1;
 
    GetServerResponse(m_cmdSocket);
    int nRetCode = ParseResponse(m_strResponse);
    if (nRetCode != FTP_RSP_FILE_STATUS)
        return 0;

    return atol(m_strResponse.GetBuffer());
}

// 下载文件
bool CFTPClient::Get(CString strRemoteFile, CString strLocalFile)
{
    return DownLoad(strRemoteFile, strLocalFile);
}

// 上传文件
bool CFTPClient::Put(CString strRemoteFile, CString strLocalFile)
{
    if ( m_bLogin == false )
    {
        LogE("Not login to FTP server now.");
        return false;
    }

    const ULONG dataLen = FTP_DEFAULT_BUFFER;
    char strBuf[dataLen] = {0};
    long nSize = GetFileLength(strRemoteFile);
    long nLen = 0;

    FILE *pFile = fopen(strLocalFile.GetBuffer(), "rb");  // 以只读方式打开  且文件必须存在
    if (pFile == NULL)
    {
        LogE("Open local file %s failed.", strLocalFile.GetBuffer());
        return false;
    }

    int data_fd = socket(AF_INET, SOCK_STREAM, 0);
    if ( data_fd == -1 )
    {
        LogE("create data socket failed.");
        return false;
    }

    if (CreateDataLink(data_fd) == false )
        return false;

    UINT nCmd = ( nSize == -1 ) ? FTP_COMMAND_UPLOAD_FILE : FTP_COMMAND_APPEND_FILE;

    if ( ParseCommand( nCmd, strRemoteFile) == false )
        return false;
 
    if (Send(m_cmdSocket, m_commandStr) < 0)
    {
        Close(data_fd);
        return false;
    }

    GetServerResponse(m_cmdSocket);
    int nRetCode = ParseResponse(m_strResponse);
    if ( nRetCode != FTP_RSP_OPEN_DATA_LINK )
    {
        LogE("Send Upload command failed, code: %d, desc: %s", nRetCode, m_strResponse.GetBuffer() );
        return false;
    }

    fseek(pFile, nSize, SEEK_SET);
    while (!feof(pFile))
    {
        nLen = fread(strBuf, 1, dataLen, pFile);
        if (nLen < 0)
            break;

        if (Send(data_fd, strBuf) < 0)
        {
            Close(data_fd);
            return false;
        }
    }

    // GetServerResponse(data_fd);
    // LogI("Upload File Command: %s", m_strResponse.GetBuffer());

    Close(data_fd);

    fclose(pFile);

    GetServerResponse(m_cmdSocket);

    LogDI("Upload File Command: %s", m_strResponse.GetBuffer());
    nRetCode = ParseResponse(m_strResponse);
    if ( nRetCode != FTP_RSP_FILE_OP_SUCCESS )
    {
        LogE("Upload file failed, code: %d, desc: %s", nRetCode, m_strResponse.GetBuffer() );
        return false;
    }

    LogD("Upload file %s success.", strLocalFile.GetBuffer());

    return true;
}

// 解析FTP命令
bool CFTPClient::ParseCommand(const unsigned int command, CString strParam)
{
    if (command < FTP_COMMAND_BASE || command > FTP_COMMAND_END)
    {
        LogE("FTPClient ParseCommand Parameter cmd ID %d out of range", command);
        return false;
    }

    m_nCurrentCommand = command;

    switch (command)
    {
    case FTP_COMMAND_USERNAME:
        m_commandStr = "USER ";
        break;

    case FTP_COMMAND_PASSWORD:
        m_commandStr = "PASS ";
        break;

    case FTP_COMMAND_QUIT:
        m_commandStr = "QUIT ";
        break;

    case FTP_COMMAND_CURRENT_PATH:
        m_commandStr = "PWD ";
        break;

    case FTP_COMMAND_TYPE_MODE:
        m_commandStr = "TYPE ";
        break;

    case FTP_COMMAND_PSAV_MODE:
        m_commandStr = "PASV ";
        break;

    case FTP_COMMAND_DIR:
        m_commandStr = "LIST ";
        break;

    case FTP_COMMAND_CHANGE_FOLDER:
        m_commandStr = "CWD ";
        break;

    case FTP_COMMAND_DELETE_FILE:
        m_commandStr = "DELE ";
        break;

    case FTP_COMMAND_DELETE_FOLDER:
        m_commandStr = "RMD ";
        break;

    case FTP_COMMAND_CREATE_FOLDER:
        m_commandStr = "MKD ";
        break;

    case FTP_COMMAND_RENAME_BEGIN:
        m_commandStr = "RNFR ";
        break;

    case FTP_COMMAND_RENAME_END:
        m_commandStr = "RNTO ";
        break;

    case FTP_COMMAND_FILE_SIZE:
        m_commandStr = "SIZE ";
        break;

    case FTP_COMMAND_DOWNLOAD_FILE:
        m_commandStr = "RETR ";
        break;

    case FTP_COMMAND_DOWNLOAD_RESET:
        m_commandStr = "REST ";
        break;

    case FTP_COMMAND_UPLOAD_FILE:
        m_commandStr = "STOR ";
        break;

    case FTP_COMMAND_APPEND_FILE:
        m_commandStr = "APPE ";
        break;

    default:
        LogE("FTPClient ParseCommand Parameter cmd ID %d out of range", command);
        return false;
    }

    m_commandStr += strParam;
    m_commandStr += "\n";

    return true;
}

bool CFTPClient::Connect(int socketfd, CString serverIP, unsigned int nPort)
{
    if (socketfd == INVALID_SOCKET)
    {
        LogE("Invalid socket in Connect.");
        return false;
    }

    unsigned int argp = 1;
    int error = -1;
    int len = sizeof(int);
    struct sockaddr_in  addr;
    bool ret = false;
    timeval stime;
    fd_set  set;

    ioctl(socketfd, FIONBIO, &argp);  //设置为非阻塞模式

    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(nPort);
    addr.sin_addr.s_addr = inet_addr(serverIP.GetBuffer());
    bzero(&(addr.sin_zero), 8);

    LogD("Address: %s %d", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

    if (connect(socketfd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) == -1)   //若直接返回 则说明正在进行TCP三次握手
    {
        stime.tv_sec = 20;  //设置为1秒超时
        stime.tv_usec = 0;
        FD_ZERO(&set);
        FD_SET(socketfd, &set);

        if (select(socketfd + 1, NULL, &set, NULL, &stime) > 0)   // 在这边等待 阻塞 返回可以读的描述符 或者超时返回0  或者出错返回-1
        {
            getsockopt(socketfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t*)&len);
            ret = (error == 0) ? true : false;
        }
    }
    else
    {
        LogD("FTP Client Connect Immediately!!!");
        ret = true;
    }

    argp = 0;
    ioctl(socketfd, FIONBIO, &argp);

    if (!ret)
    {
        close(socketfd);
        LogE("Cannot connect server!!");
        return false;
    }

    return true;
}

void CFTPClient::Close(int sock)
{
    shutdown(sock, SHUT_RDWR);
    close(sock);
    sock = INVALID_SOCKET;
}

void CFTPClient::GetServerResponse(int sockfd)
{
    CString temp = "";

    if (sockfd == INVALID_SOCKET)
    {
        LogE("Invalid socket in GetServerResponse.");
        return;
    }

    int nRet = -1;
    char buf[MAX_PATH] = {0};

    while ((nRet = GetServerData(sockfd, buf, MAX_PATH)) > 0)
    {
        buf[MAX_PATH - 1] = '\0';
        temp += buf;
    }

    int nPos = temp.Find("\r\n");
    if ( nPos >= 0 &&  nPos < temp.GetLength() )
        m_strResponse = temp.Left(nPos);

    return;
}

FTP_API CFTPClient::GetServerData(int fd, char *strBuf, unsigned long length)
{
    assert(strBuf != NULL);

    if (fd == INVALID_SOCKET)
    {
        LogE("Invalid socket in GetServerData.");
        return -1;
    }

    memset(strBuf, 0, length);
    timeval stime;
    int nLen;

    stime.tv_sec = 1;
    stime.tv_usec = 0;

    fd_set  readfd;
    FD_ZERO( &readfd );
    FD_SET(fd, &readfd );

    if (select(fd + 1, &readfd, 0, 0, &stime) > 0)
    {
        if ((nLen = recv(fd, strBuf, length, 0)) > 0)
            return nLen;
        else
            return -2;
    }

    return 0;
}

FTP_API CFTPClient::Send(int fd, CString cmd)
{
    return Send(fd, cmd.GetBuffer(), cmd.GetLength());
}
 
FTP_API CFTPClient::Send(int fd, const char *cmd, const size_t len)
{
    if (fd == INVALID_SOCKET)
    {
        LogE("Invalid socket in Send.");
        return -1;
    }

    if((FTP_COMMAND_USERNAME != m_nCurrentCommand) &&(FTP_COMMAND_PASSWORD != m_nCurrentCommand) && (!m_bLogin))
    {
        LogE("Not login to server now.");
        return -1;
    }

    timeval timeout;
    timeout.tv_sec  = 1;
    timeout.tv_usec = 0;

    fd_set  writefd;
    FD_ZERO(&writefd);  
    FD_SET(fd, &writefd);

    if(select(fd + 1, 0, &writefd , 0 , &timeout) > 0)
    {
        size_t nlen  = len; 
        int nSendLen = 0; 
        while (nlen >0) 
        {
            nSendLen = send(fd, cmd , (int)nlen , 0);

            if(nSendLen == -1) 
                return -2; 

            nlen = nlen - nSendLen;
            cmd +=  nSendLen;
        }

        return 0;
    }

    LogE("Send command %s failed.", cmd);
    return -1;
}

bool CFTPClient::CreateDataLink(int data_fd)
{
    if ( data_fd == INVALID_SOCKET)
    {
        LogE("Invalid socket in CreateDataLink.");
        return false;
    }

    CString strData;
    UINT nPort = 0;
    CString strServerIp;
    std::list<CString> strArray;

    CString parseStr = Pasv();

    if (parseStr.GetLength() <= 0)
    {
        LogE("Set PASV failed.");
        return false;
    }

    size_t nBegin = parseStr.Find("(");
    size_t nEnd   = parseStr.Find(")");
    strData       = parseStr.Mid(nBegin + 1, nEnd - nBegin - 1);

    if( SplitString( strData , strArray , "," ) == false )
        return false;

    if( ParseString( strArray , nPort , strServerIp) == false )
        return false;

    if (Connect(data_fd, strServerIp, nPort) == false )
        return false;

    return true;
}

bool CFTPClient::SplitString( CString strSrc, std::list<CString> &strArray , CString strFlag)
{
    int pos = 1; 

    while((pos = (int)strSrc.Find(strFlag)) > 0) 
    {
        strArray.insert(strArray.end(), strSrc.Left(pos));
        strSrc = strSrc.Right(pos + 1); 
    }

    strArray.insert(strArray.end(), strSrc);

    return true; 
}

bool CFTPClient::ParseString(std::list<CString> strArray, UINT & nPort , CString & strServerIp)
{
    if (strArray.size() < 6 )
    {
        LogE("Parameter %d in ParseString", strArray.size());
        return false;
    }

    std::list<CString>::iterator citor;
    citor = strArray.begin();
    strServerIp = *citor;
    strServerIp += ".";
    citor ++;
    strServerIp += *citor;
    strServerIp += ".";
    citor ++ ;
    strServerIp += *citor;
    strServerIp += ".";
    citor ++ ;
    strServerIp += *citor;
    citor = strArray.end();
    citor--;
    nPort = atol( (*citor).GetBuffer());
    citor--;
    nPort += atol( (*(citor)).GetBuffer()) * 256 ;

    return true; 
}

// 支持断点续传
// pos: 下载文件的起始偏移量，
// length: 文件长度
// 返回值：下载的字节数
long CFTPClient::DownLoad(CString strRemoteFile, CString strLocalFile, const int pos, long length)
{
    if ( m_bLogin == false )
    {
        LogE("Not login to FTP server now.");
        return -1;
    }

    if ( pos < 0 )
    {
        LogE("Parameter Position: %d", pos);
        return -1;
    }

    if ( length == 0 )
    {
        length = GetFileLength(strRemoteFile);
        if ( length == -1 )
        {
            LogE("Can not get FTP server file %s length: %s", strRemoteFile.GetBuffer());
            return -1;
        }
    }

    long nDataLen = FTP_DEFAULT_BUFFER;
    char strPos[MAX_PATH]  = {0};
    int data_fd = socket(AF_INET, SOCK_STREAM, 0);

    if ((length != 0) && (length < nDataLen))
        nDataLen = length;

    char dataBuf[FTP_DEFAULT_BUFFER];

    sprintf(strPos, "%d", pos);
 
    if (CreateDataLink(data_fd) == false )
    {
        LogE("Create Data Link error");
        return -1;
    }

    if ( ParseCommand(FTP_COMMAND_DOWNLOAD_RESET, strPos) == false )
        return -1;
    if (Send(m_cmdSocket, m_commandStr) < 0)
        return -1;

    GetServerResponse(m_cmdSocket);
    int nRetCode = ParseResponse(m_strResponse);
    if ( nRetCode != FTP_RSP_FILE_OP_MORE_CMD )
    {
        LogE("Get error Response from server FTP_COMMAND_DOWNLOAD_RESET: %d %s", nRetCode, m_strResponse.GetBuffer() );
        return -1;
    }

    if ( ParseCommand(FTP_COMMAND_DOWNLOAD_FILE, strRemoteFile) == false )
        return -1;
    if (Send(m_cmdSocket, m_commandStr) < 0)
        return -1;

    GetServerResponse(m_cmdSocket);
    nRetCode = ParseResponse(m_strResponse);
    if ( nRetCode != FTP_RSP_OPEN_DATA_LINK )
    {
        LogE("Get error Response from server FTP_COMMAND_DOWNLOAD_FILE: %d %s", nRetCode, m_strResponse.GetBuffer() );
        return -1;
    }
 
    CFile localFile;
    if ( localFile.OpenFile(strLocalFile, "w") == false )
    {
        LogE("Can not open local file %s", strLocalFile.GetBuffer());
        return -1;
    }

    int len = 0;
    int nReceiveBytes = 0;
    while ((len = GetServerData(data_fd, dataBuf, nDataLen)) > 0)
    {
        nReceiveBytes += len;

        localFile.WriteData(dataBuf, len);
        memset(dataBuf, 0, sizeof(dataBuf));

        if (nReceiveBytes == length && length != 0)
            break;
 
        if ((nReceiveBytes + nDataLen) > length  && length != 0)
        {
            nDataLen = length - nReceiveBytes;
        }
    }

    Close(data_fd);
    localFile.Close();

    GetServerResponse(m_cmdSocket);
    nRetCode = ParseResponse(m_strResponse);
    if ( nRetCode != FTP_RSP_FILE_OP_SUCCESS && nRetCode != -1 )
    {
        LogE("Get error Response from server End: %d %s", nRetCode, m_strResponse.GetBuffer() );
        // return -1;
    }

    LogD("FTP File %s end, Received bytes: %d", strLocalFile.GetBuffer(), nReceiveBytes);

    return nReceiveBytes;
}

FTP_API CFTPClient::ParseResponse(CString str)
{
    if ( str.IsEmpty() )
    {
        // LogE("ParseString parameter is empty.");
        return -1;
    }

    CString strData = str.Left(3);
    unsigned int val = atoi(strData.GetBuffer());

    m_strResponse = str.Right(4);
    m_strResponse.Trim();

    return val;
}

// // 测试代码
// int main()
// {
//     CFTPClient ftp;
//     ftp.LoginToServer( "192.168.1.190", FTP_USER, FTP_PASSWORD);
//     // ftp.PWD();
//     // ftp.CD("/home/controller");

//     // ftp.CreateDirectory("/home/controller/screen");
//     // ftp.CreateDirectory("/home/controller/screen/test");
//     // ftp.CD("/home/controller/screen/test");

//     // ftp.SetTransferMode(binary);

//     // temp = ftp.Dir(".");
//     // LogI("Dir: %s", temp.GetBuffer());
//     // ftp.Get("m.tar", "m.tar");
//     ftp.CD("/home/controller/screen/test");
//     ftp.Dir("/home/controller/screen/test");

//     // ftp.Put("m1.tar", "m.tar");
//     // ftp.DeleteFile("m.tar");
//     // ftp.CD("/home/controller/screen");
//     ftp.Rename("a.jpg",  "b.jpg");
//     ftp.QuitServer();
// }
