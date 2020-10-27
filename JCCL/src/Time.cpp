// ------------------------------------------------------------------------------
// JCCL - Component :: Time
// Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    Time.h
// Purpose: Common Module, Only used for Linux system
//-------------------------------------------------------------------------------

#include "Time.h"

CTimeSpan::CTimeSpan()
{
    m_nDays     = 0;
    m_nHours    = 0;
    m_nMinutes  = 0;
    m_nSeconds  = 0;
    m_timeSpan  = 0;
}

// time: 单位：秒
CTimeSpan::CTimeSpan(__time64_t time)
{
    m_timeSpan = time;
    ResetTimeSpan();
}

CTimeSpan::CTimeSpan(LONG lDays, int nHours, int nMins, int nSecs)
{
    m_nDays     = lDays;
    m_nHours    = nHours;
    m_nMinutes  = nMins;
    m_nSeconds  = nSecs;

    m_timeSpan  = m_nDays * 86400l + m_nHours * 3600 + m_nMinutes * 60 + m_nSeconds;
}

void CTimeSpan::ResetTimeSpan()
{
    m_nDays     = int(m_timeSpan / 86400l);

    int temp    = (int)(m_timeSpan % 86400l);
    m_nHours    = temp / 3600;
    m_nMinutes  = temp / 3600 / 60;
    m_nSeconds  = temp - m_nHours * 3600 - m_nMinutes * 60;
}

CTimeSpan CTimeSpan::operator+(CTimeSpan span)
{
    m_timeSpan += span.GetTimeSpan();

    ResetTimeSpan();

    return (*this);
}

CTimeSpan CTimeSpan::operator-(CTimeSpan span)
{
    m_timeSpan -= span.GetTimeSpan();

    ResetTimeSpan();

    return (*this);
}

CTimeSpan& CTimeSpan::operator+=(CTimeSpan span)
{
    m_timeSpan += span.GetTimeSpan();

    ResetTimeSpan();

    return (*this);
}

CTimeSpan& CTimeSpan::operator-=(CTimeSpan span)
{
    m_timeSpan += span.GetTimeSpan();

    ResetTimeSpan();

    return (*this);
}

bool CTimeSpan::operator==(CTimeSpan span)
{
    return (m_timeSpan == span.GetTimeSpan());
}

bool CTimeSpan::operator!=(CTimeSpan span)
{
    return (m_timeSpan != span.GetTimeSpan());
}

bool CTimeSpan::operator<(CTimeSpan span)
{
    return (m_timeSpan < span.GetTimeSpan());
}

bool CTimeSpan::operator>(CTimeSpan span)
{
    return (m_timeSpan > span.GetTimeSpan());
}

bool CTimeSpan::operator<=(CTimeSpan span)
{
    return (m_timeSpan <= span.GetTimeSpan());
}

bool CTimeSpan::operator>=(CTimeSpan span)
{
    return (m_timeSpan >= span.GetTimeSpan());
}

CTime::CTime()
{
    m_nYear     = 0;
    m_nMonth    = 1;
    m_nDay      = 1;
    m_nHour     = 0;
    m_nMinute   = 0;
    m_nSecond   = 0;

    m_timeStamp = 0;
}

CTime::CTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec)
{
    m_nYear     = nYear;
    m_nMonth    = nMonth;
    m_nDay      = nDay;
    m_nHour     = nHour;
    m_nMinute   = nMin;
    m_nSecond   = nSec;

    int nDays = GetDays();
    m_timeStamp = nDays * 86400l + m_nHour * 3600 + m_nMinute * 60 + m_nSecond;
}

CTime CTime::GetCurrentTime()
{
    time_t t;
    struct tm * lt;
    time (&t);
    lt = localtime (&t);

    CTime currTime(lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
    
    return currTime;
}

// 获取自公元元年以来的天数
// 默认闰年从公元元年开始
int CTime::GetDays()
{
    int nDays = 0;

    switch( m_nMonth )
    {
    case 12:
        nDays +=30;
    case 11:
        nDays += 31;
    case 10:
        nDays +=30;
    case 9:
        nDays +=31;
    case 8:
        nDays +=31;
    case 7:
        nDays +=30;
    case 6:
        nDays += 31;
    case 5:
        nDays += 30;
    case 4:
        nDays +=31;
    case 3:
        nDays += 28;
    case 2:
        nDays += 31;
    case 1:
        nDays += m_nDay;
    }

    nDays += m_nYear * 365 + m_nYear / 4;

    return nDays;
}

// 根据时间戳计算年月日时分秒
void CTime::ResetTime()
{
    UINT nDays = (UINT)(m_timeStamp / 86400l);
    UINT temp  = (UINT)(m_timeStamp - nDays * 86400l);

    // 首先计算时分秒
    m_nHour     = temp / 3600;
    m_nMinute   = (temp - m_nHour * 3600 ) / 60;
    m_nSecond   = temp - m_nHour * 3600 - m_nMinute * 60;

    // 计算年月日
    m_nYear = nDays / 365;
    int tmp = nDays % 365;
    int a   = m_nYear / 4;
    if ( a > tmp )
    {
        m_nYear --;
        a = m_nYear / 4;
    }

    tmp = nDays - m_nYear * 365 - a;

    int tmpAdd[] ={ 0,31,59,90,120,151,181,212,243,273,304,334};

    int i;
    if ( m_nYear % 4 == 0 )
    {
        for ( i = 2; i < 12; i++ )
            tmpAdd[i] += 1;
    }

    for (  i = 1; i < 12; i++)
    {
        if ( tmp <= tmpAdd[i] )
            break;
    }

    m_nMonth = i;
    m_nDay   = tmp - tmpAdd[i-1];
}

CTime& CTime::operator=(__time64_t time)
{
    m_timeStamp = time;
    ResetTime();

    return (*this);
}

CTime& CTime::operator=(CTime time)
{
    m_timeStamp = time.GetTime();
    ResetTime();

    return (*this);
}

CTime& CTime::operator+=(CTimeSpan span)
{
    m_timeStamp += span.GetTimeSpan();
    ResetTime();

    return (*this);
}

CTime& CTime::operator-=(CTimeSpan span)
{
    m_timeStamp -= span.GetTimeSpan();
    ResetTime();

    return (*this);    
}

CTimeSpan CTime::operator-(CTime time)
{
    CTimeSpan span(m_timeStamp - time.GetTime());

    return span;
}

CTimeSpan CTime::operator+(CTime time)
{
    CTimeSpan span(m_timeStamp + time.GetTime());

    return span;
}

CTime CTime::operator-(CTimeSpan span)
{
    m_timeStamp -= span.GetTimeSpan();
    ResetTime();

    return (*this);
}

CTime CTime::operator+(CTimeSpan span)
{
    m_timeStamp += span.GetTimeSpan();
    ResetTime();

    return (*this);
}

bool CTime::operator==(CTime time)
{
    return ( m_timeStamp == time.GetTime() );
}

bool CTime::operator!=(CTime time)
{
    return ( m_timeStamp != time.GetTime() );
}

bool CTime::operator<(CTime time)
{
    return ( m_timeStamp < time.GetTime() );
}

bool CTime::operator>(CTime time)
{
    return ( m_timeStamp > time.GetTime() );
}

bool CTime::operator<=(CTime time)
{
    return ( m_timeStamp <= time.GetTime() );
}

bool CTime::operator>=(CTime time)
{
    return ( m_timeStamp >= time.GetTime() );
}

CString CTime::Format(const char* DateConnector, const char* MiddleConnector, const char* timeConnector)
{
    CString strTime;
    
    if ( DateConnector == NULL && MiddleConnector == NULL && timeConnector == NULL )
    {
        // 默认格式显示
        strTime.Format("%04d-%02d-%02d %02d:%02d:%02d", m_nYear, m_nMonth, m_nDay, m_nHour, m_nMinute, m_nSecond);
    }
    else if ( DateConnector != NULL && MiddleConnector != NULL && timeConnector != NULL )
    {
        // 指定格式显示
        strTime.Format("%04d%s%02d%s%02d%s%02d%s%02d%s%02d", 
            m_nYear, DateConnector,   m_nMonth,  DateConnector,   m_nDay, MiddleConnector, 
            m_nHour, timeConnector, m_nMinute, timeConnector, m_nSecond);
    }
    else if ( DateConnector == NULL )
    {
        // 只显示时间, 不显示日期
        if ( timeConnector == NULL )
            strTime.Format("%02d-%02d-%02d", m_nHour, m_nMinute, m_nSecond);
        else
            strTime.Format("%02d%s%02d%s%02d", m_nHour, timeConnector, m_nMinute, timeConnector, m_nSecond);
    }
    else
    {
        // 只显示日期，不显示时间
        if ( DateConnector == NULL )
            strTime.Format("%04d-%02d-%02d", m_nYear, m_nMonth, m_nDay);
        else
            strTime.Format("%04d%s%02d%s%02d", m_nYear, DateConnector, m_nMonth, DateConnector, m_nDay);
    }

    return strTime;
}

// 获取指定日期是一年里的第几周
// 星期天为每周的第一天
int CTime::GetWeek()
{
    CTimeSpan ts = (*this) - CTime(m_nYear, 1, 1, 0, 0, 0);
    int nDays = ts.GetDays();

    int nFirstWeekDay = CTime(m_nYear, 1, 1, 0, 0, 0).GetDayOfWeek();

    return (nDays + nFirstWeekDay) / 7 + 1;
}

// 获取当前日期是星期几
// 星期日为0
// 计算公式：蔡勒公式
// 百度百科：https://baike.baidu.com/item/%E8%94%A1%E5%8B%92%E5%85%AC%E5%BC%8F
int CTime::GetDayOfWeek()
{
    int nYear = m_nYear;
    int nMonth = m_nMonth;

    if( nMonth < 3 )
    {
        nYear -= 1;
        nMonth +=12;
    }

    int c = int(nYear / 100), y = nYear - 100 * c;
    int w = int( c / 4 ) - 2 * c + y + int( y / 4 ) + ( 26 * ( nMonth + 1) / 10 ) + m_nDay - 1;
    w = ( w % 7 + 7 ) % 7;

    return w;
}
