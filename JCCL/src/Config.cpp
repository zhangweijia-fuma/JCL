/*------------------------------------------------------------------------------
 * JCCL - Component :: Config file Access
 * Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    Config.cpp
 * Purpose: Common module
 *----------------------------------------------------------------------------*/

#include "Config.h"
#include "Debug.h"

CConfig::CConfig(CString strConfigFileName)
{
    m_strConfigFileName = strConfigFileName;
}

CConfig::~CConfig()
{
    m_ConfigFile.Close();
}

bool CConfig::OpenConfigFile(CString strConfigFileName)
{
    m_strConfigFileName = strConfigFileName;
    return true;
}

// Get a int variable from ini file
int CConfig::GetConfigInt(CString Section, CString Key, int nDefault)
{
    CString buffer = GetConfigStr(Section, Key, "" );

    // 没有设置参数
    if ( buffer.GetLength() == 0 )
        return nDefault;

    return atoi( buffer.GetBuffer() );
}

// Get a string variable from ini file
CString CConfig::GetConfigStr(CString Section, CString Key, CString Default)
{
    if ( m_ConfigFile.OpenFile(m_strConfigFileName) == false )
    {
        // 配置文件不存在，此时不报错
        // LogE("Can not open config file: %s", m_strConfigFileName.GetBuffer());
        return Default;
    }

    CString buffer = Default;

    if ( Section == "" || Key == "" )
    {
        LogE("Config Parameters error.");
        return buffer;
    }

    CString validStr, ConfigLine;

    // 读配置文件的每一行
    bool bFlag = false;
    while( bFlag == false)
    {
        if ( m_ConfigFile.ReadLine( ConfigLine ) == false )
            break;

        validStr = GetValidStr(ConfigLine);
        if ( validStr.GetLength() == 0 )
            continue;

        validStr.Upper();
        Section.Upper();

        // 是否查找到关键字
        if ( validStr.Find( Section ) != -1 )
            bFlag = true;
    }

    if ( bFlag == false )
    {
        m_ConfigFile.Close();
        LogE("Can not find Config Section: %s in %s", Section.GetBuffer(), m_strConfigFileName.GetBuffer());
        return buffer;
    }

    // 找到Section后，读后续的行，找到Key
    bFlag = false;
    while( bFlag == false )
    {
        if ( m_ConfigFile.ReadLine( ConfigLine ) == false )
            break;

        validStr = GetValidStr( ConfigLine);
        if ( validStr.GetLength() == 0 )
            continue;

        CString temp = validStr;
        temp.Upper();
        Key.Upper();

        // 是否找到Key
        if ( temp.Find( Key ) != -1 )
            bFlag = true;
    }

    m_ConfigFile.Close();

    if ( bFlag == false )
    {
        LogE("Can not find config key: %s", Key.GetBuffer());
        return buffer;
    }

    int nPos = validStr.Find("=");
    if ( nPos == -1 )
    {
        LogE("Can not find config key: %s, config: %s", Key.GetBuffer(), validStr.GetBuffer());
        return buffer;
    }

    buffer = validStr.Right(nPos + 1);
    buffer.Trim();

    return buffer;
}

// 去掉在配置文件中一行的注释
CString CConfig::GetValidStr(CString line)
{
    CString validStr = "";

    int nPos = line.Find(MC_COMMENT_CHAR);

    if ( nPos < 0 || nPos >= line.GetLength() )
    {
        return line;
    }

    validStr = line.Left(nPos);
    if ( validStr.GetLength() == 0 )
        return validStr;

    // 删除2头的空格
    validStr.Trim();
    return validStr;
}
