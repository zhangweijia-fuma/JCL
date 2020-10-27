// ------------------------------------------------------------------------------
// JCCL - Component :: File
// Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    File.cpp
// Purpose: Common Module
//-------------------------------------------------------------------------------

#include "File.h"
#include "Debug.h"

#ifdef QT_CORE_LIB
#include <QFileInfo>
#endif

// File Object, used by Application.
CFile::CFile()
{
    m_hFile = NULL;
    m_FileName = "";
}

CFile::~CFile()
{
    if ( m_hFile != NULL )
        Close();
}

// strFullFileName: 全路径文件名
bool CFile::IsFileExist(CString strFullFileName)
{
#ifdef QT_CORE_LIB
    QFileInfo fileInfo(strFullFileName.GetBuffer());
    return (fileInfo.isFile());
#else
    // Test File is exist
    CFile file;
    if ( file.OpenFile(strFullFileName, "r") == false )
        return false;

    file.Close();
    return true;
#endif
}


// Open a file
bool CFile::OpenFile(PCSTR filename, const char* mode)
{
    if (m_hFile != NULL )
    {
        LogE("Already open file %s", m_FileName.GetBuffer());
        return false;
    }

    m_hFile = fopen(filename, mode);
    if ( m_hFile == NULL )
    {
        // LogE("Open File Failed: %s.", filename);    
        return false;
    }

    m_FileName = filename;
    return true;
}

bool CFile::OpenFile(CString filename, const char* mode)
{  
    return OpenFile( filename.GetBuffer(), mode);
}

void CFile::Close()
{
    if ( m_hFile == NULL )
        return;

    fclose(m_hFile);
    m_hFile = NULL;
    return;
}

// read a line from file
bool CFile::ReadLine(CString& line)
{
    if ( m_hFile == NULL )
        return false;

    char fileLine[MAX_STR_LEN];

    if ( fgets(fileLine, MAX_STR_LEN, m_hFile) == NULL )
    {
        // LogE("Get one line from %s error.", m_FileName.GetBuffer() );
        return false;
    }
    
    for( UINT i = 0; i < strlen(fileLine); i++)
    {
        if ( fileLine[i] != '\r' && fileLine[i] != '\n')
            continue;

        memset( &fileLine[i], 0, MAX_STR_LEN - i);
        break;
    }

    line = fileLine;
    return true;
}

// read data from file
// return: bytes readed.
int CFile::ReadData(void* buffer, int nLength)
{
    if ( nLength <= 0 || m_hFile == NULL )
        return 0;

    return fread( buffer, 1, nLength, m_hFile);
}

// write a string to file
// return: bytes writed.
int CFile::WriteLine(char* line)
{
    if ( m_hFile == NULL || line == NULL || strlen(line) == 0)
        return -1;

    int nBytes = fputs(line, m_hFile);
    nBytes += fputs("\n", m_hFile);
    return nBytes;
}

int CFile::WriteLine(CString strLine)
{
    return WriteLine(strLine.GetBuffer());
}

// write data to file
// return: bytes writed.  
int CFile::WriteData(void* pData, int nLength)
{
    if ( m_hFile == NULL )
        return 0;

    return fwrite(pData, 1, nLength, m_hFile);
}

void CFile::Flush()
{
    if ( m_hFile == NULL )
        return;

    fflush( m_hFile);
}

void CFile::Seek(unsigned long offset, int nOrigin)
{
    if ( m_hFile == NULL )
        return;

    fseek( m_hFile, offset, nOrigin );
}

void CFile::SeekToEnd()
{
    if ( m_hFile == NULL )
        return;

    fseek( m_hFile, 0L, SEEK_END );
}

long CFile::GetFileSize()
{
    if ( m_hFile == NULL )
        return -1;

    fseek( m_hFile, 0L, SEEK_END);  
    long size = ftell( m_hFile ); 

    fseek( m_hFile, 0L, SEEK_SET);
    
    return size;  
}

