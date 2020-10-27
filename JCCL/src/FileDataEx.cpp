/**************************************************************************
FileName	: FileDataEx.h 
Description	: Text Data File Access, 文本数据文件的存取定义
Begin		: 2012-04-22
Copyright	:
Email		: zhangwj2005@gmail.com
***************************************************************************/

/***************************************************************************
*                                                                         		
*   This program is free software; you can redistribute it and/or modify        
*   it under the terms of the GNU General Public License as published by 
*   the Free Software Foundation; either version 2 of the License, or 
*   (at your option) any later version.                                  
*
***************************************************************************/

// FileDataEx.cpp : 实现文件
//

#include "stdafx.h"
#include "FileDataEx.h"
#include "Debug.h"

// CFileDataBase

bool CFileDataBase::ReadField(CString& stLine, CString& stParam)
{
	// 去掉第一个“|”
	stLine = stLine.Right(1);

	int nPos = stLine.Find( "|" );
	if ( nPos == -1 )
		return false;

	stParam = stLine.Left(nPos);

	stLine = stLine.Right( nPos );

	return true;
}

bool CFileDataBase::ReadField(CString& stLine, int& nParam)
{
	// 去掉第一个“|”
	stLine = stLine.Right( stLine.GetLength()-1);

	int nPos = stLine.Find("|");
	if ( nPos == -1 )
		return false;

	CString temp = stLine.Left(nPos);
	nParam = atoi(temp.GetBuffer());

	stLine = stLine.Right( stLine.GetLength() - nPos );

	return true;
}

bool CFileDataBase::ReadField(CString& stLine, bool& nParam)
{
	// 去掉第一个“|”
	stLine = stLine.Right( stLine.GetLength()-1);

	int nPos = stLine.Find("|");
	if ( nPos == -1 )
		return false;

	CString temp = stLine.Left(nPos);
	int nTemp = atoi(temp.GetBuffer());
	nParam = (nTemp == 1 )? true:false;

	stLine = stLine.Right( stLine.GetLength() - nPos );

	return true;
}

bool CFileDataBase::ReadField(CString& stLine, CTime& date)
{
	// 去掉第一个“|”
	stLine = stLine.Right( stLine.GetLength()-1);

	int nPos = stLine.Find("|");
	if ( nPos == -1 )
		return false;

	CString lpszTemp = stLine.Left(nPos);

	int nYear, nMonth, nDay, nHour, nMin, nSec;

	// lpszDate YYYY-MM-DD,HH:MM:SS
	nYear 	= atoi( lpszTemp.Mid(0, 4).GetBuffer() );
	nMonth 	= atoi( lpszTemp.Mid(5,2).GetBuffer() );
	nDay 	= atoi( lpszTemp.Mid(8,2).GetBuffer() );
	nHour 	= atoi( lpszTemp.Mid(11,2).GetBuffer() );
	nMin 	= atoi( lpszTemp.Mid(14,2).GetBuffer() );
	nSec 	= atoi( lpszTemp.Mid(17,2).GetBuffer() );

	CTime tm( nYear, nMonth, nDay, nHour, nMin, nSec );
	date = tm;

	stLine = stLine.Right( stLine.GetLength() - nPos );

	return true;
}

bool CFileDataBase::ReadField(CString& stLine, DWORD& dwWord)
{
	// 去掉第一个“|”
	stLine = stLine.Right( stLine.GetLength()-1);

	int nPos = stLine.Find("|");
	if ( nPos == -1 )
		return false;

	CString temp = stLine.Left(nPos);
	dwWord = atoi(temp.GetBuffer());

	stLine = stLine.Right( stLine.GetLength() - nPos );

	return true;
}

void CFileDataBase::WriteField(CString& stLine, DWORD dwWord)
{
	stLine.Format("%s|%d", stLine.GetBuffer(), dwWord);
}

void CFileDataBase::WriteField(CString& stLine, CTime date, bool bFull)
{
	CString tmp;

	tmp = date.Format("-");
	if ( bFull )
		tmp += date.Format(NULL, NULL, "-");

	stLine = stLine + "|" + tmp;
	return;
}

void CFileDataBase::WriteField(CString& stLine, CString stParam)
{
	stLine = stLine + "|" + stParam;
	return;
}

void CFileDataBase::WriteField(CString& stLine, int nParam)
{
	CString temp;

	temp.Format("%s|%d", stLine.GetBuffer(), nParam);

	stLine = temp;
}

void CFileDataBase::WriteField(CString& stLine, bool nParam)
{
	int temp = nParam?1:0;

	stLine.Format("%s|%d", stLine.GetBuffer(), temp);
}

bool CFileDataBase::ReadString(CFile* pFile, CString& line )
{
	bool bFlag = pFile->ReadLine(line);

	while( bFlag )
	{
		// 处理空白行，及前后空白
		line.Trim();
		if ( line.IsEmpty() )
		{
			bFlag = pFile->ReadLine( line );
		}
		else
			return true;
	}

	return false;
}

// CFileDataEx
CFileDataEx::CFileDataEx()
{
	m_strFileName = "";
}

CFileDataEx::~CFileDataEx()
{
	DeleteAllItems();
}


// 打开数据文件

// nFieldCount: 每行字段数
// pDataList 是输出， 所有数据保存在此对象里面
// stFileName: 绝对路径，含文件名
bool CFileDataEx::LoadData(CString stFileName, UINT nFieldCount, CString strClassName)
{
	// 至少需要一个字段；
	if ( nFieldCount < 1 )
		return false;

	m_strFileName = stFileName;

    // Test File is exist
	if ( CFile::IsFileExist(m_strFileName) == false )
	{
		// 文件不存在，创建文件
        CFile temp;
        temp.OpenFile( m_strFileName, "a+");
        CreateDataFile( &temp );
        temp.Close();       

		return true;
    }

    CFile dataFile;
    dataFile.OpenFile( m_strFileName, "r" );

	bool bFlag = true;
	while ( bFlag == true )
	{
		// 处理单行记录，具体处理由对象的LoadFileData函数实现
		CFileDataBase* pData = (CFileDataBase*)CreateClassObject(strClassName);
		if ( pData == NULL )
		{
			LogE("Create Object %s failed.", strClassName.GetBuffer());
			return false;
		}

		bFlag = pData->LoadFileData(&dataFile);
		if ( bFlag == true )
			InsertData( pData );
	}

    dataFile.Close();
	return false;
}


bool CFileDataEx::SaveData()
{
	CFile dataFile;
	dataFile.OpenFile( m_strFileName, "w+" );

	CFileDataBase* pData = (CFileDataBase*)m_DataList.GetHead();
	while ( pData != NULL )
	{
		pData->SaveFileData(&dataFile);

		// 处理单行记录，具体处理由对象的SaveFileData函数实现
		CFileDataBase* pData = (CFileDataBase*)m_DataList.GetNext(pData);
	}

    dataFile.Close();
	return true;
}


// 创建数据文件的结构
void CFileDataEx::CreateDataFile(CFile* pFile)
{
	CString stTemp = "";

	stTemp = "////////////////////////////////////////////////////////////////////////////\r\n";
	pFile->WriteLine(stTemp);
	stTemp = "# 文件格式: “|”为字段分隔符;“#”、“//”为注释行\r\n";
	pFile->WriteLine(stTemp);;
	stTemp = "# 示例： |xxxx|xxx|xxxx|\r\n";
	pFile->WriteLine(stTemp);

	return;
}


bool CFileDataEx::InsertData(CFileDataBase* pData)
{
	if ( pData == NULL )
		return false;

	if ( FindItem( pData ) == true )	
		return false;

	return m_DataList.AddTail( pData );
}

bool CFileDataEx::FindItem(CFileDataBase* pData)
{
	return m_DataList.Find(pData);
}

void CFileDataEx::DeleteItem(CFileDataBase* pData)
{
	if ( pData == NULL )
		return;

	m_DataList.Remove(pData);
	return;
}

void CFileDataEx::DeleteAllItems()
{
	m_DataList.RemoveAll();
}

int CFileDataEx::GetItemCount()
{
	return m_DataList.GetCount();
}

CFileDataBase* CFileDataEx::GetItem(int nIndex)
{
	return (CFileDataBase*)m_DataList.GetAt(nIndex);
}

