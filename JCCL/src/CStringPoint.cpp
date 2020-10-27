/*------------------------------------------------------------------------------
 * JCCL - Component :: Base
 * Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    CString.cpp
 * Purpose: Base module
 *----------------------------------------------------------------------------*/

#include "CString.h"
#include "Debug.h"


// 使用指针，不是使用数组，可以节省内存，但是存在指针使用错误的风险
// 使用数组，不使用指针，可以避免指针错误，但是会浪费内存

CString::CString()
{
    if ( MC_STR_ARRAY == true )
    {
        memset(m_Buffer, 0, MAX_STR_LEN);
        m_pStrBuf = m_Buffer;
    }
    else
    {
        m_pStrBuf = NULL;
    }
}

CString::CString(PCSTR str2)
{
    if ( MC_STR_ARRAY == true )
    {
        memset(m_Buffer, 0, MAX_STR_LEN);
        m_pStrBuf = m_Buffer;
    }
    else
    {
        m_pStrBuf = NULL;
    }

    Assignment(str2);
}

// 拷贝构造函数，在CString作为参数传递时，会生成一个临时构造函数
// 临时构造函数会copy对象的所有值，也把包括对象中的指针成员变量。
// 因此，在临时对象的析构函数中对指针成员变量进行删除操作时，原对象的指针指向的内存已经不存在，而原对象不知道。
// 在原对象的析构函数在删除指针时，就造成错误
// 因此，通过重定义对象的copy constructor，重新申请内存，使得临时对象和原对象的指针各自指向不同的内存区域
CString::CString(const CString& str2)
{
    if ( MC_STR_ARRAY == true )
    {
        memset(m_Buffer, 0, MAX_STR_LEN);
        m_pStrBuf = m_Buffer;
    }
    else
    {
        m_pStrBuf = NULL;
    }

    Assignment(str2.GetBuffer());
}

CString::~CString()
{
    DeleteBuffer();
}

// 申请新的字符串内存，不一定是对象的字符串
PSTR CString::NewBuffer(int nLen)
{
    if ( MC_STR_ARRAY == true )
        return NULL;

    PSTR pTemp = new char[nLen];
    memset(pTemp, 0, nLen);
    return pTemp;
}

// 删除申请的内存
void CString::DeleteBuffer()
{
    if ( MC_STR_ARRAY == true )
        return;

    try
    {
        if ( m_pStrBuf != NULL )
            delete []m_pStrBuf;

        m_pStrBuf = NULL;
    }
    catch (...)
    {
        LogI("CString::DeleteBuffer failed");
        m_pStrBuf = NULL;
    }

    return;
}

int CString::GetLength()
{
    if ( MC_STR_ARRAY == true )
        return strlen(m_pStrBuf);

    if ( m_pStrBuf == NULL )
        return 0;

    return strlen(m_pStrBuf);
}

// 当对象字符串<str2时，返回为负数；
// 当对象字符串=str2时，返回值= 0；
// 当对象字符串>str2时，返回正数。该正数不确定数值
// 当对象本身为空时，返回-1 or 0
int CString::Compare(PCSTR str2)
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

// 从指定起始位置查找子串
// 找到子串返回子串在字符串的起始位置
// 未找到子串返回-1
int CString::Find(CString str2, int nStart)
{
    if ( m_pStrBuf == NULL )
        return -1;

    int nLen = str2.GetLength();

    if ( nLen == 0 || nStart < 0 || nStart >= GetLength())
    {
        // LogE("CString::Find parameters error");
        return -1;
    }

    CString temp;
    if ( nStart > 0 )
        temp = Right(nStart);
    else
        temp = m_pStrBuf;

    PCSTR tempStr = temp.GetBuffer();
    if ( tempStr == NULL )
        return -1;

    PSTR subStr = strstr((PSTR)tempStr, str2.GetBuffer());
    if ( subStr == NULL )
        return -1;

    return (int)(subStr - tempStr ); 
}

int CString::Find(PCSTR str2, int nStart)
{
    CString temp = str2;
    return Find(temp, nStart);
}

int CString::Find(PSTR str2, int nStart)
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

// 查找字符串中第一个数字的位置
int CString::FindFirstNumber()
{
    for (int i = 0; i < GetLength(); i++)
    {
        if ( m_pStrBuf[i] >= '0' && m_pStrBuf[i] <= '9')
            return i;
    }

    return -1;
}

// 查找字符串中第一个字母的位置，不区分大小写
int CString::FindFirstChar()
{
    for (int i = 0; i < GetLength(); i++)
    {
        if ( ( m_pStrBuf[i] >= 'a' && m_pStrBuf[i] <= 'z') || ( m_pStrBuf[i] >= 'A' && m_pStrBuf[i] <= 'Z') )
            return i;
    }

    return -1;
}

// 格式化字符串，此时使用了MAX_STR_LEN宏，限定了字符串的长度
void CString::Format(PCSTR pstrFormat, ...)
{
    char temp[MAX_STR_LEN * 2] = {0};

    va_list args;
    va_start(args, pstrFormat);
    vsprintf((PSTR)temp, pstrFormat, args);
    va_end(args);

    DeleteBuffer();
    if ( MC_STR_ARRAY == false )
    {
        m_pStrBuf = NewBuffer(strlen(temp) + 1);
        if ( m_pStrBuf == NULL )
            return;
    }

    strcpy(m_pStrBuf, temp);
    m_pStrBuf[GetLength()] = '\0';

    return;
}

// 取得字符串中指定位置的字符
char CString::GetAt(int nIndex)
{
    if ( nIndex >= GetLength() )
        return 0;

    return m_pStrBuf[nIndex];
}

// 取得指定开始位置到结束位置的子串
CString CString::Mid(int nFirst, int nCount)
{
    CString strMid;
    int nLen = GetLength();

    if ( nFirst < 0 || nCount < 0 || nFirst >= nLen )
        return strMid;

    if( (nFirst + nCount) > nLen )
        nCount = nLen - nFirst;

    if( (nFirst == 0) && ((nFirst + nCount) == nLen ) )
        return( *this );

    if ( MC_STR_ARRAY == true )
    {
        char tempStr[MAX_STR_LEN];
        memset(tempStr, 0, MAX_STR_LEN);
        strncpy( tempStr, (PCSTR)(m_pStrBuf + nFirst), nCount );
        tempStr[strlen(tempStr)] = '\0';

        strMid = tempStr;
    }
    else
    {
        PSTR tempStr = NewBuffer(nCount + 1);
        if ( tempStr == NULL )
        {
            // 申请内存失败
            return strMid;
        }

        strncpy( tempStr, (PCSTR)(m_pStrBuf + nFirst), nCount );
        tempStr[strlen(tempStr)] = '\0';

        strMid = tempStr;
        delete []tempStr;
    }

    return strMid;
}

// 取字符串右侧POS开始位置以后的子串，
CString CString::Right(int nPos)
{
    CString strRight;

    int nLen = GetLength();
    if( nPos >= nLen )
        return strRight;

    int nStrLen = nLen - nPos;
    if ( MC_STR_ARRAY == true )
    {
        char tempStr[MAX_STR_LEN];
        memset(tempStr, 0, MAX_STR_LEN);
        for(int i = nPos; i < nLen && i < nPos + MAX_STR_LEN; i++ )
            tempStr[i - nPos] = m_pStrBuf[i];

        tempStr[nStrLen] = '\0';

        strRight = tempStr;
    }
    else
    {
        PSTR tempStr = NewBuffer(nStrLen + 1);
        if ( tempStr == NULL )
            return strRight;

        for(int i = nPos; i < nLen; i++ )
            tempStr[i - nPos] = m_pStrBuf[i];

        tempStr[nStrLen] = '\0';

        strRight = tempStr;
        delete []tempStr;
    }

    return strRight;
}

CString CString::Left(int nCount)
{
    CString strLeft;

    if( nCount >= GetLength() )
        return strLeft;

    if ( MC_STR_ARRAY == true )
    {
        char tempStr[MAX_STR_LEN];
        memset(tempStr, 0, MAX_STR_LEN);
        strncpy( tempStr, (PCSTR)m_pStrBuf, nCount );
        tempStr[nCount] = '\0';

        strLeft = tempStr;
    }
    else
    {
        PSTR tempStr = NewBuffer(nCount + 1);
        if ( tempStr == NULL )
            // 申请内存失败
            return strLeft;

        strncpy( tempStr, (PCSTR)m_pStrBuf, nCount );
        tempStr[nCount] = '\0';

        strLeft = tempStr;
        delete []tempStr;
    }

    return strLeft;
}

void CString::Trim()
{
    TrimLeft();
    TrimRight();
}

void CString::TrimLeft()
{
    int nLen = GetLength();
    if ( nLen == 0 )
        return;

    int i = 0;
    while ( i < nLen && m_pStrBuf[i] == ' ' && m_pStrBuf[i] != '\0' )
        i++;

    if ( i == 0 )
    {
        // 左边没有空格，直接返回
        return;
    }

    if ( MC_STR_ARRAY == true )
    {
        char tempStr[MAX_STR_LEN];
        memset(tempStr, 0, MAX_STR_LEN);
        strcpy( tempStr, (m_pStrBuf + i));
        tempStr[strlen(tempStr)] = '\0';
        strcpy(m_pStrBuf, tempStr);
    }
    else
    {
        PSTR tempStr = NewBuffer(nLen - i + 1);
        if ( tempStr == NULL )
            // 申请内存失败
            return;

        strcpy( tempStr, (m_pStrBuf + i));
        tempStr[strlen(tempStr)] = '\0';

        DeleteBuffer();

        m_pStrBuf = tempStr;
    }

    return;
}

void CString::TrimRight()
{
    int nLen = GetLength();
    if ( nLen == 0 )
        return;

    int i;
    for( i = GetLength() - 1; i >= 0; i-- )
    {
        if ( m_pStrBuf[i] != ' ' )
            break;
    }

    if ( i == GetLength() - 1 )
        return;

    if ( MC_STR_ARRAY == true )
    {
        char tempStr[MAX_STR_LEN];
        memset(tempStr, 0, MAX_STR_LEN);
        strncpy( tempStr, m_pStrBuf, i + 1 );
        strcpy(m_pStrBuf, tempStr);
        m_pStrBuf[i+1] = '\0';
    }
    else
    {
        PSTR tempStr = NewBuffer(GetLength() - i + 1);
        if ( tempStr == NULL )
            // 申请内存失败
            return;

        strncpy( tempStr, m_pStrBuf, i + 1 );

        DeleteBuffer();

        m_pStrBuf = tempStr;
        m_pStrBuf[i+1] = '\0';
    }

    return;
}

// 将字符串中的字母大写
void CString::Upper()
{
    for( int i = 0; i < GetLength(); i++ )
    {
        if ( m_pStrBuf[i] >= 'a' && m_pStrBuf[i] <= 'z' )
            m_pStrBuf[i] -= 32;
    }

    return;
}

bool CString::EndWith(CString str)
{
    return EndWith((PCSTR)str.GetBuffer());
}

bool CString::EndWith(PSTR str)
{
    return EndWith((PCSTR)str);
}

// 字符串是否以str为结尾
bool CString::EndWith(PCSTR str)
{
    if ( str == NULL )
        return false;

    int nLen    = GetLength();
    int nLen2   = strlen(str);

    // 待比较的字符串长度大于字符串长度，则返回false
    if ( nLen2 > nLen  )
        return false;

    for( int i = 0; i < nLen2 && i < nLen; i++ )
    {
        if ( *(str + i ) != *(m_pStrBuf + nLen - nLen2 + i))
            return false;
    }

    return true;
}

void CString::Splite(PSTR pSubStr, CList& strList)
{
    CString temp;

    temp = pSubStr;
    Splite(temp, strList);
}

void CString::Splite(PCSTR pSubStr, CList& strList)
{
    CString temp = pSubStr;
    Splite(temp, strList);
}

// 将字符串分成几个子串
void CString::Splite(CString SubStr, CList& strList)
{
    if ( m_pStrBuf == NULL )
        return;

    int nPos = Find(SubStr);
    if ( nPos == - 1)
    {
        // 未找到子串
        CString *strTemp = new CString();
        if ( strTemp == NULL )
            return;

        *strTemp = m_pStrBuf;
        strList.AddTail(strTemp);
        return;
    }

    CString strNext = Right( nPos + 1);
    if ( nPos > 0 )
    {
        CString *strTemp = new CString();
        if ( strTemp == NULL )
            return;

        *strTemp = Left(nPos);
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

    if ( MC_STR_ARRAY == true )
    {
        char pFilePath[MAX_STR_LEN];
        memset(pFilePath, 0, MAX_STR_LEN);
        memcpy(pFilePath, m_pStrBuf, (pFilename - m_pStrBuf));
        pFilePath[(pFilename - m_pStrBuf)] = '\0';

        strTemp = pFilePath;
    }
    else
    {
        PSTR pFilePath = NewBuffer(pFilename - m_pStrBuf + 1);
        if ( pFilePath == NULL )
            // 申请内存失败
            return "";

        memcpy(pFilePath, m_pStrBuf, (pFilename - m_pStrBuf));
        pFilePath[(pFilename - m_pStrBuf)] = '\0';

        strTemp = pFilePath;
        delete []pFilePath;
    }

    return strTemp.GetBuffer();
}

// 是否全为字母
bool CString::IsAlpha()
{
    if ( m_pStrBuf == NULL )
        return false;

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
    if ( m_pStrBuf == NULL )
        return false;

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
    if ( m_pStrBuf == NULL )
        return false;

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
    if ( m_pStrBuf == NULL )
        return false;

    for(int i = 0; i < GetLength(); i++ )
    {
        if ( m_pStrBuf[i] < '0' || m_pStrBuf[i] > '9')
            return false;
    }

    return true;
}

CString& CString::Assignment(PCSTR str2)
{
    DeleteBuffer();

    if ( str2 == NULL || strlen(str2) == 0 )
        return (*this);

    if ( MC_STR_ARRAY == true )
    {
        if ( strlen(str2) >= MAX_STR_LEN)
            return (*this);
    }
    else
    {
        m_pStrBuf = NewBuffer(strlen(str2) + 1);
        if ( m_pStrBuf == NULL )
            // 申请内存失败
            return (*this);
    }

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

CString& CString::Add(PCSTR str2)
{
    if ( str2 == NULL || strlen(str2) == 0 )
        return (*this);

    int nLen = GetLength();
    int nLen2 = strlen(str2);

    if (MC_STR_ARRAY == true)
    {
        if ( GetLength() == 0 )
            strcpy(m_pStrBuf, str2);
        else
            sprintf(m_pStrBuf, "%s%s", m_pStrBuf, str2);

        m_pStrBuf[GetLength()] = '\0';
    }
    else
    {
        PSTR pTemp = NewBuffer(nLen + nLen2 + 1);
        if ( pTemp == NULL )
        {
            // 申请内存失败
            return (*this);
        }

        if ( m_pStrBuf == NULL )
            strcpy(pTemp, str2);
        else
        {
            sprintf(pTemp, "%s%s", m_pStrBuf, str2);
            DeleteBuffer();
        }

        m_pStrBuf = pTemp;
        m_pStrBuf[GetLength()] = '\0';
    }

    return (*this);    
}

CString& CString::operator+(PCSTR str2)
{
    return Add(str2);
}

CString& CString::operator+(PSTR str2)
{
    return Add((PCSTR)str2);
}

CString& CString::operator+(char cChar)
{
    char temp[1];
    temp[0] = cChar;
    return Add(temp);
}

CString& CString::operator+(CString str2)
{
    return Add((PCSTR)str2.GetBuffer());
}

CString& CString::operator+=(PCSTR str2)
{
    return Add(str2);
}

CString& CString::operator+=(PSTR str2)
{
    return Add((PCSTR)str2);
}

CString& CString::operator+=(char cChar)
{
    char temp[1];
    temp[0] = cChar;
    return Add(temp);
}

CString& CString::operator+=(CString str2)
{
    return Add(str2.GetBuffer());
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

// 保留此测试代码
int StringTest()
{
    CString a = "Hello,world!";
    printf("%s\n", a.GetBuffer() );

    CString b = "Hello";
    CString c = "world!";
    printf("%s\n", b.GetBuffer());
    printf("Compare: %d\n", a.Compare("A"));
    printf("Compare: %d\n", a.Compare(b));

    printf("Find: %d\n", a.Find("A"));
    printf("Find: %d\n", a.Find(b));
    printf("Find: %d\n", a.Find(c));

    CString d;
    d.Format("%s", "  Hello  ");
    printf("%s\n", d.GetBuffer());
    printf("GetAt: %c\n", a.GetAt(2));

    d.Trim();
    printf("Trim: %s, %d\n", d.GetBuffer(), d.GetLength());

    CString e = a.Mid(6, 7);
    printf("Mid: %s\n", e.GetBuffer() );

    e = a.Left(5);
    printf("Left: %s\n", e.GetBuffer() );

    e = a.Right(6);
    printf("Right: %s\n", e.GetBuffer() );

    printf("EndWith: %d\n", e.EndWith("world!"));
    printf("EndWith: %d\n", e.EndWith("world"));

    printf("IsAlpha: %d\n", e.IsAlpha());
    printf("IsNumber: %d\n", e.IsNumber());
    printf("IsUpper: %d\n", e.IsUpper());
    printf("IsLower: %d\n", e.IsLower());

    CString f = b + ", " + c;
    printf("operator+: %s\n", f.GetBuffer() );

    CString g = "/JamesSoft/Purpose/a.txt";
    CList list;
    g.Splite(",", list);

    for(int i = 0; i < list.GetCount(); i++)
    {
        CString* pStr = (CString*)list.GetAt(i);
        printf("Splite %d: %s\n", i + 1, pStr->GetBuffer());
    }

    CString h = g.GetFileName();
    printf("GetFileName: %s\n", h.GetBuffer() );
    printf("GetFilePath: %s\n", g.GetFilePath() );

    return 0;
}
