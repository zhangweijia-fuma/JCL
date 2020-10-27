// ------------------------------------------------------------------------------
// JCCL - Component :: Time
// Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    Time.h
// Purpose: Common Module, Only used for Linux system
//-------------------------------------------------------------------------------

#ifndef _TIME_H_
#define _TIME_H_

#include "CString.h"

typedef unsigned long long __time64_t;

class CTimeSpan : public CObjBase
{
    public:
        CTimeSpan();
        CTimeSpan(__time64_t time);
        CTimeSpan(
            LONG lDays,
            int nHours,
            int nMins,
            int nSecs);

        LONGLONG            GetDays() { return m_nDays; }
        LONG                GetHours() { return m_nHours; }
        LONG                GetMinutes() { return m_nMinutes; }
        LONG                GetSeconds() { return m_nSeconds; }

        __time64_t          GetTimeSpan() { return m_timeSpan; }

        CTimeSpan           operator+(CTimeSpan span);
        CTimeSpan           operator-(CTimeSpan span);
        CTimeSpan&          operator+=(CTimeSpan span);
        CTimeSpan&          operator-=(CTimeSpan span);
        bool                operator==(CTimeSpan span);
        bool                operator!=(CTimeSpan span);
        bool                operator<(CTimeSpan span);
        bool                operator>(CTimeSpan span);
        bool                operator<=(CTimeSpan span);
        bool                operator>=(CTimeSpan span);

    protected:
        void                ResetTimeSpan();

        __time64_t          m_timeSpan;
        LONG                m_nDays;
        int                 m_nHours;
        int                 m_nMinutes;
        int                 m_nSeconds;
};


class CTime : public CObjBase
{
    public:
        CTime();
        CTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec);

    // Operations
    public:
        static CTime        GetCurrentTime();

        CTime&              operator=(__time64_t time);
        CTime&              operator=(CTime time);

        CTime&              operator+=(CTimeSpan span);
        CTime&              operator-=(CTimeSpan span);

        CTimeSpan           operator-(CTime time);
        CTimeSpan           operator+(CTime time);

        CTime               operator-(CTimeSpan span);
        CTime               operator+(CTimeSpan span);

        bool                operator==(CTime time);
        bool                operator!=(CTime time);
        bool                operator<(CTime time);
        bool                operator>(CTime time);
        bool                operator<=(CTime time);
        bool                operator>=(CTime time);

        __time64_t          GetTime() { return m_timeStamp; }

        int                 GetYear() { return m_nYear; }
        int                 GetMonth() { return m_nMonth; }
        int                 GetDay() { return m_nDay; }
        int                 GetHour() { return m_nHour; }
        int                 GetMinute() { return m_nMinute; }
        int                 GetSecond() { return m_nSecond; }

        int                 GetWeek();
        int                 GetDayOfWeek();
        int                 GetDays();

        CString             Format(const char* DateConnecter = "-", const char* MiddleConnector = " ", const char* timeConnector = ":");

    protected:
        void                ResetTime();
        
        int                 m_nYear, m_nMonth, m_nDay, m_nHour, m_nMinute, m_nSecond;

        // 自公元元年元旦开始的秒数
        __time64_t          m_timeStamp;
};

#endif // _TIME_H_

