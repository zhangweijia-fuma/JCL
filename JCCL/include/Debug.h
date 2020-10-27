// ------------------------------------------------------------------------------
// JCCL - Component :: Debug interface
// Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    Debug.h
// Purpose: Debug Module, print log info to screen and log file.
//-------------------------------------------------------------------------------

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "JCCLTypes.h"


#ifdef QT_CORE_LIB
#include <QDebug>
#endif

#include "CString.h"

class CDebug : public CObjBase
{
    public:
        CDebug();

    // Operations
    public:
        // Write String to log file
        void                Log(unsigned char debugLevel, const char* pstrFormat, ...);
        
        void                operator<<(const char* str2);
        void                operator<<(char* str2);
        void                operator<<(CString str2);
        void                SetLogFile(CString filename);               // 日志文件名称，不含路径，只有文件名

    protected:
        void                GetDebugLevel();                            // 实时获取输出级别

    // Atrributes
    protected:
        int                 m_nDebugLevel;
        CString             m_strLogFileName;

};

extern CString g_strAppRootPath;
extern CDebug cDebug;

#define LOG_CLOSE                       1                               // 关闭输出
#define LOG_SYSTEM_ERROR                2                               // 系统错误及告警输出
#define LOG_NORMAL                      3                               // 正常信息
#define LOG_DEBUG                       4                               // 调试信息
#define LOG_DEBUG_DATA                  5                               // 调试信息，带具体数据

#define LogE(fmt, ...)                  cDebug.Log(LOG_SYSTEM_ERROR, fmt, ##__VA_ARGS__)
#define LogI(fmt, ...)                  cDebug.Log(LOG_NORMAL, fmt, ##__VA_ARGS__)
#define LogD(fmt, ...)                  cDebug.Log(LOG_DEBUG, fmt, ##__VA_ARGS__)
#define LogDI(fmt, ...)                 cDebug.Log(LOG_DEBUG_DATA, fmt, ##__VA_ARGS__)

typedef void (*SHOW_LOG_CALLBACK)(LPCSTR strLogLine);                   // 输出日志到回调函数进行处理
extern SHOW_LOG_CALLBACK g_pShowLogCallback;                            // 日志输出的回调函数

#endif //_DEBUG_H_

