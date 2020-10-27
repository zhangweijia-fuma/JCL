/*------------------------------------------------------------------------------
 * JCCL - Component :: Config INI file Access
 * Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    Config.h
 * Purpose: Common module
 *----------------------------------------------------------------------------*/

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "CString.h"
#include "File.h"

#define MC_COMMENT_CHAR                 "#"

class CConfig : public CObjBase
{
    public:
        CConfig(CString = "");
        ~CConfig();

    // Operations
    public:
        bool                OpenConfigFile(CString strConfigFileName);

        int                 GetConfigInt(CString Section, CString Key, int nDefault = -1);
        CString             GetConfigStr(CString Section, CString Key, CString Default);        

    protected:
        CFile               m_ConfigFile;
        CString             GetValidStr(CString line);
        CString             m_strConfigFileName;
};

#endif  // _CONFIG_H_

