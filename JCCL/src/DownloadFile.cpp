/*------------------------------------------------------------------------------
 * JCCL - Component :: DownFile
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    DownLoadFile.cpp
 * Purpose: DownLoad File manage realization
 *----------------------------------------------------------------------------*/

#include "Common.h"
#include "DownloadFile.h"

CDownloadTaskInfo::CDownloadTaskInfo()
{
    m_nMsgType          = INVALID_MSG;
    m_strRemoteFile     = "";
    m_strLocalFile      = "";
    m_nCompleteBytes    = 0;
}


CDownloadFile::CDownloadFile() 
{
    m_nDownloadTreadID  = 0;
    m_bRunFlag          = true;

    // 在删除节点时同时删除节点的对象实例
    m_DownloadTaskList.SetMode(true);
}

bool CDownloadFile::Initial(CString strServerAddr, CString strUserName, CString strPassword, DOWNLOAD_CALLBACK pCallback, LPVOID pCallbackParam)
{
    // 创建线程，准备下载文件
    if (pthread_create(&m_nDownloadTreadID, NULL, CDownloadFile::DownloadThread, (void*)this) == -1)
    {
        LogE("Create download thread error!");
        return false;
    }

    m_strServerAddr     = strServerAddr;
    m_strFTPUserName    = strUserName;
    m_strFTPPassword    = strPassword;

    m_pDownloadCallback = pCallback;
    m_pCallbackParam    = pCallbackParam;
    return true;
}

// 需要下载的文件是否已经在下载任务列表中
bool CDownloadFile::IsInDownloadList(CString strLocalFile)
{
    int nTaskCount = m_DownloadTaskList.GetCount();

    for(int i = 0; i < nTaskCount; i++)
    {
        CDownloadTaskInfo* pTaskInfo = (CDownloadTaskInfo*)m_DownloadTaskList.GetAt(i);
        if ( pTaskInfo == NULL )
        {
            LogE("DownloadTaskList item is NULL");
            continue;
        }

        if ( pTaskInfo->m_strLocalFile == strLocalFile )
            return true;
    }

    return false;
}

// strRemoteFile：待下载的文件，全路径文件名
// strLocalFile: 下载的目的文件名，含路径
bool CDownloadFile::InsertDownloadTask(UINT nMsgType, CString strRemoteFile, CString strLocalFile)
{
    if ( IsInDownloadList(strLocalFile) == true )
    {
        return true;
    }

    CDownloadTaskInfo* pTaskInfo = new CDownloadTaskInfo;

    pTaskInfo->m_nMsgType       = nMsgType;
    pTaskInfo->m_strLocalFile   = strLocalFile;
    pTaskInfo->m_strRemoteFile  = strRemoteFile;

    return m_DownloadTaskList.AddTail(pTaskInfo);
}


void* CDownloadFile::DownloadThread(LPVOID pParam)
{
    if ( pParam == NULL )
    {
        LogE("DownloadThread missing parameters.");
        return NULL;
    }

    CDownloadFile* pDownloadFile = (CDownloadFile*)pParam;
    pDownloadFile->StartDownloadTask();

    return NULL;
}

// 文件下载线程主函数
void CDownloadFile::StartDownloadTask()
{
    while(m_bRunFlag)
    {
        if ( m_DownloadTaskList.GetCount() == 0 )
        {
            sleep(1);
            continue;
        }
        
        if ( m_FTPClient.IsLogin() == false )
        {
            if ( m_FTPClient.LoginToServer(m_strServerAddr, m_strFTPUserName, m_strFTPPassword) == false )
            {
                LogE("Login to FTP server %s failed.", m_strServerAddr.GetBuffer());
                sleep(5);
                continue;
            }

            LogI("Login to download FTP server %s success", m_strServerAddr.GetBuffer());

            m_FTPClient.SetTransferMode(binary);
        }

        CDownloadTaskInfo* pTaskInfo = (CDownloadTaskInfo*)m_DownloadTaskList.GetHead();
        while(pTaskInfo != NULL)
        {
            ProcessDownloadTask(pTaskInfo);
            sleep(1);

            pTaskInfo = (CDownloadTaskInfo*)m_DownloadTaskList.GetHead();
        }
    }

    m_FTPClient.QuitServer();
}

// 下载单个文件
void CDownloadFile::ProcessDownloadTask(CDownloadTaskInfo* pTaskInfo)
{
    if ( pTaskInfo == NULL )
    {
        LogE("ProcessDownloadTask parameters missing.");
        return;
    }

    long nReceiveBytes = DownloadFile(pTaskInfo->m_strRemoteFile, pTaskInfo->m_strLocalFile, pTaskInfo->m_nCompleteBytes);
    if ( nReceiveBytes > 0 )
    {
        pTaskInfo->m_nCompleteBytes  += nReceiveBytes;

        // 调用回调函数，通知下载完成
        m_pDownloadCallback(m_pCallbackParam, pTaskInfo->m_nMsgType, pTaskInfo->m_strLocalFile );

        // 下载完成后后删除下载任务
        m_DownloadTaskList.Remove(pTaskInfo);
    }

}

// 从中心服务器下载单个文件
// 文件名含路径
// nPos: 下载偏移量，暂不支持断点续传
// 返回值：已经完成的下载字节数
long CDownloadFile::DownloadFile(CString strRemoteFile, CString strLocalFile, long nCompleteBytes)
{
    LogI("Download %s from FTP server to local %s", strRemoteFile.GetBuffer(), strLocalFile.GetBuffer() );

    CString filePath = strRemoteFile.GetFilePath();
    if ( filePath == "" )
    {
        LogE("Download remote file invalid: %s", strRemoteFile.GetBuffer());
        return -1;
    }

    if ( m_FTPClient.CD(filePath) == false )
    {
        LogE("Change FTP server directory failed. %s", filePath.GetBuffer());
        return -1;
    }

    long nFileLength = m_FTPClient.GetFileLength(strRemoteFile);
    if ( nFileLength <= 0 )
    {
        LogE("Get remote file %s length %d", strRemoteFile.GetBuffer(), nFileLength);
        return -1;
    }

    long nReceiveBytes = m_FTPClient.DownLoad(strRemoteFile, strLocalFile, nCompleteBytes, nFileLength);
    if ( nReceiveBytes <= 0 )
    {
        LogE("Download %s failed.", strRemoteFile.GetBuffer());
        return -1;
    }

    LogI("Download %s success, Receive bytes: %d.", strRemoteFile.GetFileName(), nReceiveBytes);
    return nReceiveBytes;
}

