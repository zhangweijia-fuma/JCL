/*------------------------------------------------------------------------------
 * JCCL - Component :: Base
 * Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    CString.cpp
 * Purpose: Base module
 *----------------------------------------------------------------------------*/

#include "CList.h"
#include "CString.h"
#include "Common.h"

// To avoid stack overflow
// char tempStr[MAX_STR_LEN];

CString::CString()
{
    memset(m_pStrBuf, 0, MAX_STR_LEN);
}

CString::CString(const char* str2)
{
    memset(m_pStrBuf, 0, MAX_STR_LEN);
    Assignment(str2);
}

void CString::Clear()
{
    memset(m_pStrBuf, 0, MAX_STR_LEN);

    return;
}

int CString::Compare(const char* str2)
{    
    if ( GetLength() == 0  && ( str2 == NULL || strlen(str2) == 0 )) 
        return 0;

    if ( GetLength() > 0 && ( str2 == NULL || strlen(str2) == 0 ) )
        return 1;

    if ( GetLength() == 0 && ( str2 != NULL || strlen(str2) != 0 ) )
        return -1;

    return strcmp( m_pStrBuf, str2 );
}


int CString::Compare(CString str2)
{
    return Compare( str2.GetBuffer() );
}

int CString::Compare(PSTR str2)
{
    return Compare( (PCSTR)str2 );
}

int CString::CompareNoCase(PCSTR str2)
{
    if ( GetLength() == 0  && ( str2 == NULL || strlen(str2) == 0 )) 
        return 0;

    if ( GetLength() > 0 && ( str2 == NULL || strlen(str2) == 0 ) )
        return 1;

    if ( GetLength() == 0 && ( str2 != NULL || strlen(str2) != 0 ) )
        return -1;

    int nLen = GetLength(), nLen2 = strlen(str2);
    return strncasecmp(m_pStrBuf, str2, (nLen < nLen2 )? nLen:nLen2 );
}

int CString::CompareNoCase(PSTR str2)
{
    return CompareNoCase( (PCSTR)str2 );
}

int CString::CompareNoCase(CString str2)
{
    return CompareNoCase(str2.GetBuffer());
}

int CString::Find(CString str2, int nStart)
{
    int nLen = str2.GetLength();

    if ( nLen == 0 || nStart < 0 || nStart >= (int)strlen(m_pStrBuf))
        return -1;

    CString temp = Right(nStart);
    PCSTR tempStr = temp.GetBuffer();
    if ( tempStr == NULL )
        return -1;

    PSTR subStr = strstr((PSTR)tempStr, str2.GetBuffer());
    if ( subStr == NULL )
        return -1;

    return (int)(subStr - tempStr ); 
}

int CString::Find(const char* str2, int nStart)
{
    CString temp = str2;
    return Find(temp, nStart);
}

int CString::Find(char* str2, int nStart)
{
    CString temp = str2;
    return Find( temp, nStart);
}

int CString::Find(char cChar, int nStart)
{
    CString temp;
    temp.Format("%c", cChar);
    return Find(temp, nStart);
}

int CString::FindFirstNumber()                                            // 查找第一个字母的位置
{
    for (int i = 0; i < strlen(m_pStrBuf); i++)
    {
        if ( m_pStrBuf[i] >= '0' && m_pStrBuf[i] <= '9')
            return i;
    }

    return -1;
}

int CString::FindFirstChar()                                          // 查找第一个数字的位置
{
    for (int i = 0; i < GetLength(); i++)
    {
        if ( ( m_pStrBuf[i] >= 'a' && m_pStrBuf[i] <= 'z') || ( m_pStrBuf[i] >= 'A' && m_pStrBuf[i] <= 'Z') )
            return i;
    }

    return -1;
}


void CString::Format(const char* pstrFormat, ...)
{
    char temp[MAX_STR_LEN * 2] = {0};

    va_list args;
    va_start(args, pstrFormat);
    vsprintf((char*)temp, pstrFormat, args);
    va_end(args);

    memset(m_pStrBuf, 0, MAX_STR_LEN);
    if ( strlen(temp) < MAX_STR_LEN )
        strcpy(m_pStrBuf, temp);
    else
        strncpy(m_pStrBuf, temp, MAX_STR_LEN - 1);

    m_pStrBuf[strlen(m_pStrBuf)] = '\0';
    return;
}

int CString::GetLength()
{
    return strlen(m_pStrBuf);
}

PCSTR CString::GetBuffer()
{
    return m_pStrBuf;
}

char CString::GetAt(int nIndex)
{
    if ( nIndex >= GetLength() )
        return 0;

    return m_pStrBuf[nIndex];
}

CString CString::Mid(int nFirst, int nCount)
{
    CString strMid;
    int nLen = GetLength();

    if ( nFirst < 0 || nCount < 0 || nLen == 0)
        return strMid;

    if( nFirst > nLen )
        return strMid;

    if( (nFirst + nCount) > nLen )
        nCount = nLen - nFirst;

    if( (nFirst == 0) && ((nFirst + nCount) == nLen ) )
        return( *this );

    char tempStr[MAX_STR_LEN] = {0};
    strncpy( tempStr, (const char*)(m_pStrBuf + nFirst), nCount );
    tempStr[strlen(tempStr)] = '\0';

    strMid = tempStr;

    return strMid;
}

// 取字符串右侧POS开始位置以后的子串，
CString CString::Right(int nPos)
{
    int nLen = strlen(m_pStrBuf);
    if( nPos >= nLen )
    {
        // LogE("Pos %d is exceed string length %d", nPos, GetLength());
        return( *this );
    }

    char tempStr[MAX_STR_LEN] = {0};
    for(int i = nPos; i < GetLength(); i++ )
        tempStr[i - nPos] = m_pStrBuf[i];

    tempStr[strlen(tempStr)] = '\0';
    CString strRight = tempStr;

    return strRight;
}

CString CString::Left(int nCount)
{
    if( nCount >= GetLength() )
        return( *this );

    char tempStr[MAX_STR_LEN] = {0};
  
    strncpy( tempStr, (const char*)m_pStrBuf, nCount );

    tempStr[strlen(tempStr)] = '\0';

    CString strLeft = tempStr;
    return strLeft;
}

void CString::Trim()
{
    TrimLeft();
    TrimRight();
}

void CString::TrimLeft()
{
    int nLen = strlen(m_pStrBuf);
    if ( nLen == 0 )
        return;

    int i = 0;
    while ( i < nLen && m_pStrBuf[i] == ' ' && m_pStrBuf[i] != '\0' )
        i++;

    char tempStr[MAX_STR_LEN] = {0};

    strcpy( tempStr, (m_pStrBuf + i));
    strcpy( m_pStrBuf, tempStr);

    m_pStrBuf[strlen(m_pStrBuf)] = '\0';

    return;
}

void CString::TrimRight()
{
    for( int i = GetLength() - 1; i >= 0; i-- )
    {
        if ( m_pStrBuf[i] != ' ' )
            break;

        m_pStrBuf[i] = '\0';
    }
}

// Description: change string from lower to upper format
void CString::Upper()
{
    int i = 0;

    while( i < GetLength() )
    {
        if ( m_pStrBuf[i] >= 'a' && m_pStrBuf[i] <= 'z' )
            m_pStrBuf[i] -= 32;

        i ++;
    }

    return;
}

bool CString::EndWith(CString str)
{
    return EndWith((const char*)str.GetBuffer());
}

bool CString::EndWith(char* str)
{
    return EndWith((const char*)str);
}

// 字符串是否以str为结尾
bool CString::EndWith(const char* str)
{
    PCSTR pBuf  = GetBuffer();
    int nLen    = GetLength();
    int nLen2   = strlen(str);

    // 待比较的字符串长度大于字符串长度，则返回false
    if ( nLen2 > nLen  )
        return false;

    for( int i = 0; i < nLen2 && i < nLen; i ++)
    {
        if ( *(str + i ) != *(pBuf + nLen - nLen2 + i))
            return false;
    }

    return true;
}

void CString::Splite(char* pSubStr, CList& strList)
{
    CString temp;

    temp = pSubStr;
    Splite(temp, strList);
}

void CString::Splite(const char* pSubStr, CList& strList)
{
    CString temp;

    temp = pSubStr;
    Splite(temp, strList);
}

void CString::Splite(CString SubStr, CList& strList)
{
    int nPos = Find(SubStr);
    if ( nPos == - 1)
    {
        CString *strTemp = new CString;
        *strTemp = m_pStrBuf;
        strTemp->Trim();
        strList.AddTail(strTemp);
        return;
    }

    CString strNext = Right( nPos + 1);

    if ( nPos > 0 )
    {
        CString *strTemp = new CString;
        *strTemp = Left(nPos);
        strTemp->Trim();
        strList.AddTail(strTemp);
    }

    return strNext.Splite(SubStr, strList);
}


PCSTR CString::GetFileName()
{
    if ( m_pStrBuf == NULL )
        return NULL;

    PSTR pFilename = strrchr( m_pStrBuf, '/');
    if ( pFilename == NULL )
        return NULL;

    return (pFilename + 1);
}

PCSTR CString::GetFilePath()
{
    CString strTemp;
    PSTR pFilename = strrchr( m_pStrBuf, '/');
    if ( pFilename == NULL )
        return "";

    char pFilePath[MAX_STR_LEN];
    memset(pFilePath, 0, MAX_STR_LEN);
    memcpy(pFilePath, m_pStrBuf, (pFilename - m_pStrBuf));
    pFilePath[(pFilename - m_pStrBuf)] = '\0';

    strTemp = pFilePath;

    return strTemp.GetBuffer();
}

// 是否全为字母
bool CString::IsAlpha()
{
    for(int i = 0; i < GetLength(); i++ )
    {
        if ( ( m_pStrBuf[i] >= 'a' && m_pStrBuf[i] <= 'Z' ) || ( m_pStrBuf[i] >= 'A' && m_pStrBuf[i] <= 'Z') )
            continue;

        return false;
    }

    return true;
}

// 是否为大写
bool CString::IsUpper()
{
    for(int i = 0; i < GetLength(); i++ )
    {
        if ( m_pStrBuf[i] < 'A' || m_pStrBuf[i] > 'Z')
            return false;
    }

    return true;
}

// 是否为小写
bool CString::IsLower()
{
    for(int i = 0; i < GetLength(); i++ )
    {
        if ( m_pStrBuf[i] < 'a' || m_pStrBuf[i] > 'z')
            return false;
    }

    return true;
}

// 是否全为数字
bool CString::IsNumber()
{
    for(int i = 0; i < GetLength(); i++ )
    {
        if ( m_pStrBuf[i] < '0' || m_pStrBuf[i] > '9')
            return false;
    }

    return true;
}

CString& CString::Assignment(PCSTR str2)
{
    memset(m_pStrBuf, 0, MAX_STR_LEN);

    if ( strlen(str2) >= MAX_STR_LEN)
        return (*this);

    strcpy(m_pStrBuf, str2);
    m_pStrBuf[GetLength()] = '\0';
    return (*this);
}

#ifdef QT_CORE_LIB
CString& CString::operator=(QString str)
{
    QByteArray temp2 = str.toLatin1();
    return Assignment(temp2.data());
}
#endif

CString& CString::operator=(PCSTR str2)
{
    return Assignment(str2);
}

CString& CString::operator=(PSTR str2)
{
    return Assignment((PCSTR)str2);
}

CString& CString::operator=(char cChar)
{
    char temp[1];
    temp[1] = cChar;
    return Assignment(temp);
}

CString& CString::operator=(CString str2)
{
    return Assignment(str2.GetBuffer());
}


CString& CString::Add(const char* str2)
{
    int nLen = GetLength();
    int nLen2 = strlen(str2);

    if ( nLen2 >= MAX_STR_LEN || ( nLen + nLen2) >= MAX_STR_LEN )
        return (*this);

    char temp[MAX_STR_LEN * 2]= {0};

    strcpy(temp, m_pStrBuf);
    strcat(temp, str2);

    if ( strlen(temp) < MAX_STR_LEN)
        strcpy(m_pStrBuf, temp);
    else
        strncpy(m_pStrBuf, temp, MAX_STR_LEN - 1);

    m_pStrBuf[strlen(m_pStrBuf)] = '\0';

    return (*this);    
}

CString& CString::operator+(const char* str2)
{
    return Add(str2);
}

CString& CString::operator+(char* str2)
{
    return Add((const char*)str2);
}

CString& CString::operator+(char cChar)
{
    int nLen = strlen(m_pStrBuf);
    if ( nLen + 1 >= MAX_STR_LEN)
        return (*this);

    m_pStrBuf[nLen] = cChar;
    m_pStrBuf[nLen + 1] = '\0';

    return (*this);
}

CString& CString::operator+(CString str2)
{
    return Add((const char*)str2.GetBuffer());
}

CString& CString::operator+=(const char* str2)
{
    return Add(str2);
}

CString& CString::operator+=(char* str2)
{
    return Add((const char*)str2);
}


CString& CString::operator+=(char cChar)
{
    char temp[1];
    temp[0] = cChar;
    return Add(temp);
}

CString& CString::operator+=(CString str2)
{
    return Add((const char*)str2.GetBuffer());
}


bool CString::operator==(PCSTR str2)
{
    return ( Compare(str2) == 0 );
}

bool CString::operator==(PSTR str2)
{
    return ( Compare(str2) == 0 );
}

bool CString::operator==(CString str2)
{
    return ( Compare(str2 ) == 0 );
}

bool CString::operator!=(PCSTR str2)
{
    return ( Compare(str2) != 0 );
}

bool CString::operator!=(PSTR str2)
{
    return ( Compare(str2) != 0 );
}

bool CString::operator!=(CString str2)
{
    return ( Compare(str2) != 0 );
}

bool CString::operator<(PCSTR str2)
{
    return ( Compare(str2) < 0 );
}

bool CString::operator<(PSTR str2)
{
    return ( Compare(str2) < 0 );
}

bool CString::operator<(CString str2)
{
    return ( Compare(str2) < 0 );
}

bool CString::operator<=(PCSTR str2)
{
    return ( Compare(str2) < 0 );
}

bool CString::operator<=(PSTR str2)
{
    return ( Compare(str2) < 0 );
}

bool CString::operator<=(CString str2)
{
    return ( Compare(str2) <= 0 );
}

bool CString::operator>(PCSTR str2)
{
    return ( Compare(str2) > 0 );
}

bool CString::operator>(PSTR str2)
{
    return ( Compare(str2) > 0 );
}

bool CString::operator>(CString str2)
{
    return ( Compare(str2) > 0 );
}

bool CString::operator>=(PCSTR str2)
{
    return ( Compare(str2) >= 0 );
}

bool CString::operator>=(PSTR str2)
{
    return ( Compare(str2) >= 0 );
}

bool CString::operator>=(CString str2)
{
    return ( Compare(str2) >= 0 );
}
