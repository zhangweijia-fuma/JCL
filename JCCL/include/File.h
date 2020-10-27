// ------------------------------------------------------------------------------
// JCCL - Component :: File
// Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    File.h
// Purpose: Common Module
//-------------------------------------------------------------------------------

#ifndef _FILE_H_
#define _FILE_H_

#ifndef _WINDOWS

#include "CString.h"

// File Operation, used by Application
class CFile
{
    public:
        CFile();
        ~CFile();

    // Operations
    public:
        static bool         IsFileExist(CString strFullFileName);
        bool                OpenFile(PCSTR filename, const char* mode);
        bool                OpenFile(CString filename, const char* mode = "r");
        void                Close();

        long                GetFileSize();

        bool                ReadLine(CString& line);
        int                 ReadData(void* buffer, int nLength);

        int                 WriteLine(char* line);
        int                 WriteLine(CString strLine);
        int                 WriteData(void* pData, int nLength);

        // int                 GetConfigNumber(CString label);
        // CString             GetConfigString(CString label);

        void                Flush();
        void                Seek(unsigned long offset, int origin);
        void                SeekToEnd();

    // Attributes
    protected:
        FILE*               m_hFile;
        CString             m_FileName;
};

#endif // _WINDOWS
#endif // _FILE_H_
