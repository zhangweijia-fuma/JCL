// ------------------------------------------------------------------------------
// JCCL - Component :: Timer
// Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    Timer.cpp
// Purpose: Common Module, Only used for Linux system
//-------------------------------------------------------------------------------


#include "Timer.h"

#include <time.h>  
#include <sys/time.h>                                                   // 包含setitimer()函数 
#include <signal.h>                                                     // 包含signal()函数

#include "Debug.h"

void CTimerInfo::TimerEvent()
{
    if ( m_bOpen == false )
        return;

    m_nCounter ++;
    if ( m_nCounter < m_nTimes )
        return;

    m_nCounter = 0;
    m_TimerCallback( m_nTimerID, m_TimerParam );
}

// Interval: Repeat times, unit: millisecond
bool CTimerInfo::CreateTimer(UINT nTimes, TIMER_CALLBACK callback, LPVOID pParam)
{
    if ( m_bOpen == true )
        return false;

    m_nTimes        = nTimes;                                           // 定时器次数
    m_nCounter      = 0;                                                // 计数器，每当计数器到达定时器次数时，定时回调函数被执行一次

    m_bOpen         = true;
    m_TimerCallback = callback;
    m_TimerParam    = pParam;

    return true;
}

void CTimerInfo::ResetTimer()
{
    m_TimerCallback     = NULL;
    m_bOpen             = false;
    m_nTimes            = 0;
    m_nCounter          = 0;
    m_TimerParam        = NULL;
}

CTimer g_Timer;

CTimer::CTimer()
{
    for(int i = 0; i < MAX_TIMER_COUNT; i++ )
    {
        m_TimerList[i].m_nTimerID   = i;
    }

    StartSystemTimer();
}

CTimer::~CTimer()
{
    // CloseSystemTimer();
}

void CTimer::StartSystemTimer()
{
    signal(SIGALRM, CTimer::OnTimer);                                   // SIGALRM处理函数

    struct itimerval itv;
    struct itimerval oldtv;

    itv.it_interval.tv_sec  = 0;                                        // 设置为100毫秒触发一次
    itv.it_interval.tv_usec = 100 * 1000;
    itv.it_value.tv_sec     = 0;
    itv.it_value.tv_usec    = 100 * 1000;

    setitimer(ITIMER_REAL, &itv, &oldtv);                               // 此函数为linux的api,不是c的标准库函数
}

// 系统定时器以100毫秒为基准进行计数。因此应用定时器的时间间隔最小值为100毫秒。
// nInterval: 单位: 毫秒, 最小值: 100
TIMER_ID CTimer::CreateTimer(UINT nInterval, TIMER_CALLBACK callback, LPVOID pParam)
{
    // System timer repeat interval is 100 millisecond, so repeat times should divided 100 
    if ( nInterval < SYSTEM_TIMER_INTERVAL )
    {
        LogE("Timer interval invalid: %d, minimum time: %d millisecond.", nInterval, SYSTEM_TIMER_INTERVAL);
        return INVALID_TIMER_ID;
    }

    for (int i = 0; i < MAX_TIMER_COUNT; i++)
    {
        // Convert from millisecond to times
        if ( m_TimerList[i].CreateTimer(nInterval / SYSTEM_TIMER_INTERVAL, callback, pParam ) == true )
            return m_TimerList[i].GetTimerID();
    }

    LogE("Create timer failed. Desc: No free timer resource!");
    return INVALID_TIMER_ID;
}

void CTimer::CloseTimer(UCHAR nTimerID)
{
    if ( nTimerID >= MAX_TIMER_COUNT )
    {
        // LogE("Timer ID %d invalid.", nTimerID);
        return;
    }

    m_TimerList[nTimerID].ResetTimer();
}

void CTimer::OnTimer(int signalParam)
{
    if ( signalParam != SIGALRM )
        return;
    
    for(int i = 0; i < MAX_TIMER_COUNT; i++)
        g_Timer.m_TimerList[i].TimerEvent();

}
