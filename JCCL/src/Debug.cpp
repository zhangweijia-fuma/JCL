// ------------------------------------------------------------------------------
// JCCL - Component :: Debug interface
// Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    Debug.c
// Purpose: Debug Module, print log info to screen and log file.
//-------------------------------------------------------------------------------

#include "stdafx.h"

#include "Debug.h"
#include "Common.h"
#include "File.h"

#ifdef QT_CORE_LIB
#include <QProcess>
#include <QDebug>
#endif

#define MAX_LOG_LINE_LEN                MAX_STR_LEN * 4

CString g_strAppRootPath = "";
SHOW_LOG_CALLBACK g_pShowLogCallback = NULL;

CDebug cDebug;

const char* dInfo[] = { " ", "Close", "Error:", "Infor:", "Debug:", "Debug Data:"};

// 环境变量设置要求：
//  JDS_App： 为应用名称，用于识别应用类型
//  AppType（应用名称）: 应用的根目录路径 
CDebug::CDebug()
{
    m_nDebugLevel = LOG_NORMAL;

#ifndef _WINDOWS

#ifdef QT_CORE_LIB
    QStringList environment = QProcess::systemEnvironment();

    QString str;
    QString appType;
    foreach(str, environment)
    {
        // qDebug() << str;
        if (str.startsWith("JDS_App"))
        {
            int nIndex = str.indexOf("=");
            appType = str.mid(nIndex + 1);
        }
    }

    if ( appType == "" )
    {
        printf("Not set JDS_App Envirement\r\n");
        exit(0);
    }

    foreach(str, environment)
    {
        if ( str.startsWith(appType) )
        {
            CString temp;
            temp = str;
            int nIndex = temp.Find("=");
            g_strAppRootPath = temp.Right(nIndex + 1);

            m_strLogFileName.Format("%s/JDS.log", g_strAppRootPath.GetBuffer());
            break;
        }
    }

    if ( g_strAppRootPath == "" )
    {
        printf("Not set JDS_App(%s) Envirement\r\n", GetQString(appType));
        exit(0);
    }
#else
    char env[MAX_STR_LEN];

    if ( getenv("JDS_App") == NULL )
    {
        printf("Envirement variable JDS_App not set.\n");
        exit(0);
    }

    strcpy( env,  getenv("JDS_App"));

    if ( getenv(env) == NULL )
    {
        printf("Envirement variable %s not set.\n", env );
        exit(0);
    }

    g_strAppRootPath = getenv(env);  
#endif

    GetDebugLevel();
    
    m_strLogFileName = "";
    m_strLogFileName.Format("%s/JDSlog.txt", g_strAppRootPath.GetBuffer());

#endif // _WINDOWS
}

// 获取调试级别环境变量
// 该环境变量目前是系统启动时获取，也可以在每次输出日志时获取。
// 如果是每次输出日志时获取，可以实时更改调试级别，以便在不中断系统运行的情况下查看更多的日志信息
// 不过这种方式会造成系统的负荷更大
// 测试后发现更改后没有效果
void CDebug::GetDebugLevel()
{
#ifdef QT_CORE_LIB
    QStringList environment = QProcess::systemEnvironment();

    QString str;
    foreach(str, environment)
    {
        if (str.startsWith("DebugLevel"))
        {
            CString temp;
            temp = str;
            int nIndex = temp.Find("=");
            int nDebugLevel = atoi(temp.Right(nIndex + 1).GetBuffer());

            if ( nDebugLevel == 0 )
                m_nDebugLevel = LOG_NORMAL;
            else
                m_nDebugLevel = nDebugLevel;
        }

        if ( m_nDebugLevel >= 1 )
            break;
    }

#else
    char env[MAX_STR_LEN];

    int nDebugLevel = 0;

    if ( getenv("DebugLevel") == NULL )
    {
        printf("Envirement variable DebugLevel not set. \n");
        exit(0);
    }

    strcpy(env, getenv("DebugLevel"));
    if ( strlen(env) > 0 )
        nDebugLevel = atoi(env);

    if ( nDebugLevel == 0 )
        m_nDebugLevel = LOG_NORMAL;
    else
        m_nDebugLevel = nDebugLevel;

#endif
}

void CDebug::SetLogFile(CString filename)
{
    m_strLogFileName = "";
    m_strLogFileName.Format("%s/%s", g_strAppRootPath.GetBuffer(), filename.GetBuffer());
}

void CDebug::Log(unsigned char nDebugLevel, const char* pstrFormat, ...)
{
    // 实时获取环境变量DebugLevel
    // GetDebugLevel();

    if ( nDebugLevel > m_nDebugLevel )
        return;

    char logLine[MAX_LOG_LINE_LEN] = {0};
    char logTemp[MAX_LOG_LINE_LEN*2] = {0};

    va_list args;
    va_start(args, pstrFormat);
    vsprintf( (LPTSTR)logTemp, pstrFormat, args);
    va_end(args);

    if ( strlen(logTemp) < MAX_LOG_LINE_LEN)
    {
        strcpy(logLine, logTemp);
    }
    else
    {
        strncpy(logLine, logTemp, MAX_LOG_LINE_LEN - 1);
        logLine[MAX_LOG_LINE_LEN - 1] = '\0';
    }

    if ( strlen(logLine) > 0 )
        sprintf(logTemp, "%s %s %s", GetSystemTime(), dInfo[nDebugLevel],  logLine);
    else
        strcpy(logTemp, logLine);
    
    if ( strlen(logTemp) < MAX_LOG_LINE_LEN)
    {
        strcpy(logLine, logTemp);
    }
    else
    {
        strncpy(logLine, logTemp, MAX_LOG_LINE_LEN - 1);
        logLine[MAX_LOG_LINE_LEN - 1] = '\0';
    }

#ifdef QT_CORE_LIB
    qDebug() << logLine;
#else
    printf("%s\r\n", logLine);
#endif

#ifndef _WINDOWS
    // 如果设置了显示日志的回调函数，则调用该函数以显示日志到界面
    if ( g_pShowLogCallback != NULL )
    {
        g_pShowLogCallback(logLine);
    }

    // Check log file length, if file length exceed max length 100M,
    // create a new log file with same filename.
    CFile logFile;

    if ( logFile.OpenFile( m_strLogFileName, "a") == false )
    {
        printf("Open log file %s failed.", m_strLogFileName.GetBuffer());
        return;
    }

    if ( logFile.GetFileSize() > MAX_LOG_FILE_LENGTH )
    {
        logFile.Close();
        if ( logFile.OpenFile( m_strLogFileName, "w") == false )
        {
            printf("Open log file %s failed.", m_strLogFileName.GetBuffer());
            return;
        }
    }

    logFile.WriteLine( logLine );
    logFile.Close();
#endif

    return;    
}

void CDebug::operator<<(char* strLogLine)
{
    Log(LOG_NORMAL, strLogLine);
    return;
}

void CDebug::operator<<(const char* strLogLine)
{
    Log(LOG_NORMAL, strLogLine);
    return;
}

void CDebug::operator<<(CString strLogLine)
{
    Log(LOG_NORMAL, strLogLine.GetBuffer());
    return;
}

