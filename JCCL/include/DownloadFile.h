/*------------------------------------------------------------------------------
 * JCCL - Component :: DownFile
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    DownLoadFile.h
 * Purpose: DownLoad File manage realization
 *----------------------------------------------------------------------------*/

#ifndef _DOWNLOADFILE_H
#define _DOWNLOADFILE_H

#include "CList.h"
#include "CString.h"
#include "FTPClient.h"
#include "Debug.h"
#include "File.h"

// 消息处理回调函数定义
// pCallbackParam：通常为对象实例
typedef void (*DOWNLOAD_CALLBACK)(LPVOID pCallbackParam, UINT nMsgType, CString strLocalFile);

// 单个下载任务信息
class CDownloadTaskInfo : public CObjBase
{
    public:
        CDownloadTaskInfo();
        UINT                m_nMsgType;                                 // 启动下载任务的消息类别
        CString             m_strRemoteFile;                            // 下载文件名，含路径
        CString             m_strLocalFile;                             // 下载的本地文件名，含路径
        long                m_nCompleteBytes;                           // 当前已下载字节
};

class CDownloadFile : public CObjBase
{
    public:
        CDownloadFile();

        bool                Initial(CString, CString, CString, DOWNLOAD_CALLBACK pCallback, LPVOID pCallbackParam);

        static void*        DownloadThread(LPVOID pParam);              // 下载线程
        void                StartDownloadTask();
        bool                InsertDownloadTask(UINT nMsgTypr, CString strRemoteFile, CString strLocalFile);
        void                DownloadCompleteSignal(unsigned int, char*, char*);      // 自定义信号

    protected:

        void                ProcessDownloadTask(CDownloadTaskInfo* pTaskInfo);
        long                DownloadFile(CString strFile, CString strLocalFile, long nPos = 0);

        bool                IsInDownloadList(CString strLocalFile);

    // Attributes
    protected:       
        CList               m_DownloadTaskList;                         // 下载任务列表

        pthread_t           m_nDownloadTreadID;                         // 下载线程ID
        CFTPClient          m_FTPClient;                                // FTP 客户端

        bool                m_bRunFlag;                                 // 运行标志
        DOWNLOAD_CALLBACK   m_pDownloadCallback;
        LPVOID              m_pCallbackParam;

        CString             m_strServerAddr;
        CString             m_strFTPUserName;
        CString             m_strFTPPassword;
};

#endif // _DOWNLOADFILE_H
