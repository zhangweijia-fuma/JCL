/*------------------------------------------------------------------------------
 * JCCL - Component :: Base
 * Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    CString.h
 * Purpose: Base module
 *----------------------------------------------------------------------------*/

#ifndef _CSTRING_H_
#define _CSTRING_H_

#ifndef _WINDOWS

#include "CList.h"
#include "JCCLTypes.h"

// 字符串是否使用数组方式
#define MC_STR_ARRAY 					true

#ifdef QT_CORE_LIB
#include <QString>
#endif

class CString : public CObjBase
{
	public:
		CString();
		CString(PCSTR str2);
		CString(const CString& p);

		~CString();

		void 				DeleteBuffer();
		PCSTR 				GetBuffer() const { return m_pStrBuf; }
		int 				GetLength();
		
		int 				Compare(CString str2);
		int 				Compare(PSTR str2);
		int 				Compare(PCSTR str2);

		int 				CompareNoCase(CString str2);
		int 				CompareNoCase(PSTR str2);
		int 				CompareNoCase(PCSTR str2);

		int 				Find(CString str2, int nStart = 0);
		int 				Find(PSTR str2, int nStart = 0);
		int 				Find(PCSTR str2, int nStart = 0);
		int 				Find(char, int nStart = 0);

		int 				FindFirstChar(); 							// 查找第一个字母的位置
		int 				FindFirstNumber();							// 查找第一个数字的位置

		void 				Format(PCSTR pstrFormat, ...);

		bool 				IsEmpty() { return (GetLength() == 0);}

		char 				GetAt(int nIndex);

		void 				Trim();
		void 				TrimLeft();
		void 				TrimRight();

		CString 			Mid(int nFirst, int nCount);
		CString 			Right(int nCount);
		CString 			Left(int nCount);
		void				Upper();

		bool 				EndWith(CString str);
		bool 				EndWith(PSTR str);
		bool 				EndWith(PCSTR str);

		bool 				IsAlpha();
		bool 				IsUpper();
		bool 				IsLower();
		bool 				IsNumber();

		void 				Splite(PSTR pSubStr, CList& strList);
		void 				Splite(PCSTR pSubStr, CList& strList);
		void 				Splite(CString SubStr, CList& strList);

		PCSTR				GetFileName();
		PCSTR	 			GetFilePath();

#ifdef QT_CORE_LIB
		CString&            operator=(QString str2);
#endif

		CString&            operator=(PCSTR str2);
		CString&            operator=(PSTR str2);
		CString& 			operator=(char cChar);
		CString&            operator=(PUCHAR str2);
		CString&            operator=(CString str2);

		CString&            operator+(PCSTR str2);
		CString&            operator+(PSTR str2);
		CString&            operator+(char str2);
		CString&            operator+(CString str2);

		CString&            operator+=(PCSTR str2);
		CString&            operator+=(PSTR str2);
		CString&            operator+=(char str2);
		CString&            operator+=(CString str2);

		bool                operator==(PCSTR str2);
		bool                operator==(PSTR str2);
		bool                operator==(CString str2);

		bool                operator!=(PCSTR str2);
		bool                operator!=(PSTR str2);
		bool                operator!=(CString str2);

		bool                operator<(PCSTR str2);
		bool                operator<(PSTR str2);
		bool                operator<(CString str2);

		bool                operator<=(PCSTR str2);
		bool                operator<=(PSTR str2);
		bool                operator<=(CString str2);

		bool                operator>(PCSTR str2);
		bool                operator>(PSTR str2);
		bool                operator>(CString str2);

		bool                operator>=(PCSTR str2);
		bool                operator>=(PSTR str2);
		bool                operator>=(CString str2);

	protected:
		CString& 			Add(PCSTR);
		CString& 			Assignment(PCSTR);
		PSTR 				NewBuffer(int nLen);
	protected:
		char 				m_Buffer[MAX_STR_LEN];
		PSTR 				m_pStrBuf;
};


extern int StringTest();

#endif  // _WINDOWS
#endif 	// _CSTRING_H_

