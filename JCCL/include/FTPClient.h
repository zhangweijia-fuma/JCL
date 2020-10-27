// ------------------------------------------------------------------------------
// JCCL - Component :: FTP Client
// Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    FTPClient.h
// Purpose: Common Module
//-------------------------------------------------------------------------------

#ifndef _FTP_CLIENT_H_
#define _FTP_CLIENT_H_

#include "CString.h"

#define FTP_API                     int
#define MAX_PATH                    260

#define FTP_DEFAULT_PORT            21                              // FTP默认端口号
#define FTP_DEFAULT_BUFFER          1024*4                          // FTP下载缓冲默认大小

#define FTP_COMMAND_BASE            1000
#define FTP_COMMAND_END             FTP_COMMAND_BASE + 30
#define FTP_COMMAND_USERNAME        FTP_COMMAND_BASE + 1            // 用户名
#define FTP_COMMAND_PASSWORD        FTP_COMMAND_BASE + 2            // 密码
#define FTP_COMMAND_QUIT            FTP_COMMAND_BASE + 3            // 退出
#define FTP_COMMAND_CURRENT_PATH    FTP_COMMAND_BASE + 4            // 获取文件路径
#define FTP_COMMAND_TYPE_MODE       FTP_COMMAND_BASE + 5            // 改变传输模式
#define FTP_COMMAND_PSAV_MODE       FTP_COMMAND_BASE + 6            // 被动端口模式
#define FTP_COMMAND_DIR             FTP_COMMAND_BASE + 7            // 获取文件列表
#define FTP_COMMAND_CHANGE_FOLDER   FTP_COMMAND_BASE + 8            // 改变路径
#define FTP_COMMAND_DELETE_FILE     FTP_COMMAND_BASE + 9            // 删除文件
#define FTP_COMMAND_DELETE_FOLDER   FTP_COMMAND_BASE + 10           // 删除目录/文件夹
#define FTP_COMMAND_CREATE_FOLDER   FTP_COMMAND_BASE + 11           // 创建目录/文件夹
#define FTP_COMMAND_RENAME_BEGIN    FTP_COMMAND_BASE  +12           // 开始重命名
#define FTP_COMMAND_RENAME_END      FTP_COMMAND_BASE + 13           // 重命名结束
#define FTP_COMMAND_FILE_SIZE       FTP_COMMAND_BASE + 14           // 获取文件大小
#define FTP_COMMAND_DOWNLOAD_RESET  FTP_COMMAND_BASE + 15           // 下载文件从指定位置开始
#define FTP_COMMAND_DOWNLOAD_FILE   FTP_COMMAND_BASE + 16           // 下载文件
#define FTP_COMMAND_UPLOAD_FILE     FTP_COMMAND_BASE + 17           // 上传文件
#define FTP_COMMAND_APPEND_FILE     FTP_COMMAND_BASE + 18           // 追加上载文件   


#define FTP_RSP_OPEN_DATA_LINK      150                             // 文件状态良好，打开数据连接
#define FTP_RSP_SUCCESS             200                             // 命令成功
#define FTP_RSP_FOLDER_STATUS       212                             // 目录状态
#define FTP_RSP_FILE_STATUS         213                             // 文件状态
#define FTP_RSP_RESTART_TAG         110                             // 重新启动标记应答
#define FTP_RSP_UNKNOWN_COMMAND     500                             // 格式错误，命令不可识别
#define FTP_RSP_INVALID_SYNTAX      501                             // 参数语法错误
#define FTP_RSP_CMD_NOT_IMPLEMENT5  502                             // 命令未实现
#define FTP_RSP_READAY_IN_MINUTES   120                             // 在X分钟内准备好
#define FTP_RSP_READY_TO_TRANS      125                             // 连接打开准备传送
#define FTP_RSP_HELP_INFO           214                             // 帮助信息，信息仅对人类用户有用
#define FTP_RSP_NAME_SYSTEM_TYPE    215                             // 名字系统类型
#define FTP_RSP_READY_FOR_NEW_USER  220                             // 对新用户服务准备好
#define FTP_RSP_QUIT_OK             221                             // 服务关闭控制连接，可以退出登录
#define FTP_RSP_CMD_NOT_IMPLEMENT2  202                             // 命令未实现
#define FTP_RSP_SYSTEM_STATUS       211                             // 系统状态或系统帮助响应
#define FTP_RSP_NO_TRANS            225                             // 数据连接打开，无传输正在进行
#define FTP_RSP_FILE_OP_SUCCESS     226                             // 关闭数据连接，请求的文件操作成功
#define FTP_RSP_ENTER_PASV_MODE     227                             // 进入被动模式
#define FTP_RSP_LOGIN_SUCCESS       230                             // 用户登录
#define FTP_RSP_REQUIRE_PASSWD      331                             // 用户名正确，需要口令
#define FTP_RSP_REQUIRE_USER_NAME   332                             // 登录时需要帐户信息
#define FTP_RSP_FILE_OP_MORE_CMD    350                             // 请求的文件操作需要进一步命令
#define FTP_RSP_EXCEED_CONN_LIMIT   421                             // 连接用户过多
#define FTP_RSP_NOT_OPEN_DATA_LINK  425                             // 不能打开数据连接
#define FTP_RSP_STOP_TRANS          426                             // 关闭连接，中止传输
#define FTP_RSP_FILE_OP_NOT_EXEC    450                             // 请求的文件操作未执行
#define FTP_RSP_LOCAL_ERROR         451                             // 中止请求的操作：有本地错误
#define FTP_RSP_STORAGE_NOT_ENOUGH  452                             // 未执行请求的操作：系统存储空间不足
#define FTP_RSP_FILE_OP_COMPLETE    250                             // 请求的文件操作完成
#define FTP_RSP_CREATE_PATH         257                             // 创建"PATHNAME"
#define FTP_RSP_CMD_SEQUENCE_ERROR  503                             // 命令顺序错误
#define FTP_RSP_FUNC_NOT_IMPLEMENT  504                             // 此参数下的命令功能未实现
#define FTP_RSP_USER_PASS_ERROR     530                             // 账号或密码错误
#define FTP_RSP_NEED_USERNAME       532                             // 存储文件需要帐户信息
#define FTP_RSP_CMD_NOT_EXEC        550                             // 未执行请求的操作
#define FTP_RSP_UNKNOWN_PAGE_TYPE   551                             // 请求操作中止：页类型未知
#define FTP_RSP_STORAGE_OVERFLOW    552                             // 请求的文件操作中止，存储分配溢出
#define FTP_RSP_INVALID_FILENAME    553                             // 未执行请求的操作：文件名不合法

// 登陆步骤
// 1. login2Server
// 2. inputUserName
// 3. inputPassWord
// 4. 具体操作
// 5. quit
enum TRANS_MODE {
    binary = 0x31,
    ascii,
};

class CFTPClient : public CObjBase
{
    public :

        CFTPClient(void);
        ~CFTPClient(void);

        // 登陆服务器
        bool            LoginToServer(CString serverIP, CString userName, CString password, UINT serverPort = FTP_DEFAULT_PORT );
        bool            QuitServer(void);                                       // 退出FTP
        bool            IsLogin() { return m_bLogin; }

        bool            SetTransferMode(TRANS_MODE mode);                       // 设置传输格式 2进制  还是ascii方式传输
        CString         Pasv();                                                 // 设置为被动模式

        CString         PWD();                                                  // 命令： PWD
        CString         Dir(CString path);                                      // 命令： DIR
        bool            CD(CString path);                                       // 命令 ：CD
        bool            DeleteFile(CString strRemoteFile);                      // 删除文件
        bool            DeleteDirectory(CString strRemoteDir);                  // 删除文件夹/目录
        bool            CreateDirectory(CString strRemoteDir);                  // 创建目录/文件夹
        bool            Rename(CString strRemoteFile, CString strNewFile);      // 重命名
        long            GetFileLength(CString strRemoteFile);                   // 获取文件大小

        bool            Get(CString strRemoteFile, CString strLocalFile);       // 下载文件
        bool            Put(CString strRemoteFile, CString strLocalFile);       // 上载文件  支持断点传送方式  

        // 下载文件
        long            DownLoad(CString strRemoteFile, CString strLocalFile, const int pos = 0, long length = 0);

    protected:
        void            Reset();                                                // 重置成员变量

        bool            ParseCommand(const UINT command, CString strParam);     // 合成发送到服务器的命令
        bool            Connect(int socketfd, CString serverIP, UINT nPort);    // 建立连接
        void            GetServerResponse(int sockfd);                          // 返回服务器信息
        FTP_API         GetServerData(int fd, char *strBuf, ULONG length);      // 获取服务器数据
        FTP_API         Send(int fd, CString cmd);                              // 发送命
        FTP_API         Send(int fd, const char *cmd, const size_t len);        // 发送命令
        bool            CreateDataLink(int data_fd);                            // 建立数据连接
        void            Close(int sock);                                        // 关闭连接

        // 解析PASV模式返回的字符串获取FTP端口号和FTP服务器IP
        bool            ParseString(std::list<CString> strArray, UINT & nPort , CString & strServerIp);
        bool            SplitString(CString strSrc, std::list<CString> &strArray , CString strFlag);

        FTP_API         ParseResponse(CString str);                             // 解析返回ftp命令的值

    protected:

        int             m_cmdSocket;                                            // 控制连接套接字
        CString         m_strUserName;                                          // 当前用户名
        CString         m_strPassWord;                                          // 当前用户密码
        CString         m_strServerIP;                                          // 服务器的IP
        UINT            m_nServerPort;                                          // 服务器Port
        CString         m_strResponse;                                          // 服务器回应信息缓存
        CString         m_commandStr;                                           // 保存命令参数
        UINT            m_nCurrentCommand;                                      // 当前使用的命令参数
        bool            m_bLogin;                                               // 是否登陆标志

};

#endif // _FTP_CLIENT_H_


