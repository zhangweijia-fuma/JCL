// ------------------------------------------------------------------------------
// JCCL - Component :: Timer
// Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    Timer.h
// Purpose: Common Module, Only used for Linux system
//-------------------------------------------------------------------------------

#ifndef _TIMER_H_
#define _TIMER_H_

#include "JCCLTypes.h"

#define SYSTEM_TIMER_INTERVAL               100                         // 系统定时器，100毫秒一次
#define MAX_TIMER_COUNT                     10
#define INVALID_TIMER_ID                    0xFF

typedef void (* TIMER_CALLBACK)(TIMER_ID nTimerID, LPVOID pParam);

class CTimerInfo
{
    public:
        CTimerInfo()
        {
            ResetTimer();
        }
        // ~CTimerInfo();

    // Operations
    public:
        bool CreateTimer(UINT nTimes, TIMER_CALLBACK callBack, LPVOID pParam);
        void TimerEvent();
        void ResetTimer();
        TIMER_ID GetTimerID() { return m_nTimerID; };

    // Attributs
    public:

    protected:
        TIMER_ID        m_nTimerID;
        bool            m_bOpen;
        UINT            m_nTimes;
        UINT            m_nCounter;

        TIMER_CALLBACK  m_TimerCallback;
        LPVOID          m_TimerParam;

    friend class CTimer;
};

class CTimer
{
    public:
        CTimer();
        ~CTimer();

    // Operations
    public:
        void StartSystemTimer();
        // void CloseSystemTimer();

        TIMER_ID CreateTimer(UINT nInterval, TIMER_CALLBACK callBack, LPVOID pParam);
        void CloseTimer(TIMER_ID timerID);

        static void OnTimer(int signalParam);
        void TimerEvent();

    // Attributs
    protected:
        CTimerInfo  m_TimerList[MAX_TIMER_COUNT];

};
extern CTimer g_Timer;

#endif // _TIMER_H_

