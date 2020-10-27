/**************************************************************************
FileName	: FileDataEx.h 
Description	: 文本数据文件的存取定义
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

////////////////////////////////////////////////////////////////////////////
//  文件格式
//  以“|”为字段分隔符
//  “#”为注释行
//  示例： |xxxx}xxxx|xxx|
//

#ifndef _FILE_DATA_EX_H
#define _FILE_DATA_EX_H

#include "CList.h"
#include "CString.h"
#include "Time.h"
#include "ObjectFactory.h"
#include "File.h"

// 数据文件的基本操作，仅仅对数据文件一行数据字段的读取操作
class CFileDataBase : public CObjBase
{
	public:
		CFileDataBase() {};
		virtual ~CFileDataBase(){};

		virtual bool LoadFileData(CFile* pFile) { return false; };
		virtual bool SaveFileData(CFile* pFile) { return false; };
		virtual CFileDataBase* Find(CFileDataBase* pItem ) { return NULL; }

		// 读取一行数据
		bool ReadString( CFile* pFile, CString& line );

		// 读一个字段
		bool ReadField(CString& stLine, CString& stParam);
		bool ReadField(CString& stLine, int& nParam);
		bool ReadField(CString& stLine, bool& nParam);
		bool ReadField(CString& stLine, CTime& date);
		bool ReadField(CString& stLine, DWORD& dwWord);

		// 写一个字段
		void WriteField(CString& stLine, CString stParam);
		void WriteField(CString& stLine, int nParam);
		void WriteField(CString& stLine, bool nParam);
		void WriteField(CString& stLine, CTime date, bool bFull);
		void WriteField(CString& stLine, DWORD dwWord);

		virtual int Compare(CFileDataBase* pData) { return 0; }
};


// CFileDataEx
// 仅仅完成对数据文件的读取操作，数据结构不在此定义，由具体的数据结构定义部分完成；
class CFileDataEx : public CObjBase
{
public:
	CFileDataEx();
	virtual ~CFileDataEx();

	bool					LoadData(CString stFileName, UINT nFieldCount, CString strClassName);
	bool					SaveData();

	void					CreateDataFile(CFile* pFile);
	bool					ReadString( CFile*, CString& line );
	
	CString					GetDataFileName() { return m_strFileName; }
	CList*					GetDataList() { return &m_DataList; }

	bool     				InsertData(CFileDataBase* pData);

	bool 					FindItem(CFileDataBase* pData);
	void					DeleteItem(CFileDataBase* pData);
	void					DeleteAllItems();
	int						GetItemCount();
	CFileDataBase*			GetItem(int nIndex);

protected:
	CString 				m_strFileName;
	CList 					m_DataList;			// 数据存储，使用列表
	void 					ShowList();
};

#endif // _FILE_DATA_EX_H

