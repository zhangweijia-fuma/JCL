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

#define STR_ARM_MODE					1    							// True: 字符串内容为固定最大长度，False: 每次new申请内存方式


#ifdef QT_CORE_LIB
#include <QString>
#endif

class CString : public CObjBase
{
	public:
		CString();
		CString(const char* str2);

		void 				Clear();

		int 				Compare(CString str2);
		int 				Compare(char* str2);
		int 				Compare(const char* str2);

		int 				CompareNoCase(CString str2);
		int 				CompareNoCase(char* str2);
		int 				CompareNoCase(const char* str2);

		int 				Find(CString str2, int nStart = 0);
		int 				Find(char* str2, int nStart = 0);
		int 				Find(const char* str2, int nStart = 0);
		int 				Find(char, int nStart = 0);

		int 				FindFirstChar(); 							// 查找第一个字母的位置
		int 				FindFirstNumber();							// 查找第一个数字的位置

		void 				Format(const char* pstrFormat, ...);

		int 				GetLength();
		bool 				IsEmpty() { return (GetLength() == 0);}
		PCSTR 				GetBuffer();

		char 				GetAt(int nIndex);

		void 				Trim();
		void 				TrimLeft();
		void 				TrimRight();

		CString 			Mid(int nFirst, int nCount);
		CString 			Right(int nCount);
		CString 			Left(int nCount);
		void				Upper();

		bool 				EndWith(CString str);
		bool 				EndWith(char* str);
		bool 				EndWith(const char* str);

		bool 				IsAlpha();
		bool 				IsUpper();
		bool 				IsLower();
		bool 				IsNumber();

		void 				Splite(char* pSubStr, CList& strList);
		void 				Splite(const char* pSubStr, CList& strList);
		void 				Splite(CString SubStr, CList& strList);

		PCSTR				GetFileName();
		PCSTR 				GetFilePath();

#ifdef QT_CORE_LIB
		CString&            operator=(QString str2);
#endif

		CString&            operator=(const char* str2);
		CString&            operator=(char* str2);
		CString&			operator=(char cChar);
		CString&            operator=(unsigned char* str2);
		CString&            operator=(CString str2);

		CString&            operator+(const char* str2);
		CString&            operator+(char* str2);
		CString&            operator+(char str2);
		CString&            operator+(CString str2);

		CString&            operator+=(const char* str2);
		CString&            operator+=(char* str2);
		CString&            operator+=(char str2);
		CString&            operator+=(CString str2);

		bool                operator==(const char* str2);
		bool                operator==(char* str2);
		bool                operator==(CString str2);

		bool                operator!=(const char* str2);
		bool                operator!=(char* str2);
		bool                operator!=(CString str2);

		bool                operator<(const char* str2);
		bool                operator<(char* str2);
		bool                operator<(CString str2);

		bool                operator<=(const char* str2);
		bool                operator<=(char* str2);
		bool                operator<=(CString str2);

		bool                operator>(const char* str2);
		bool                operator>(char* str2);
		bool                operator>(CString str2);

		bool                operator>=(const char* str2);
		bool                operator>=(char* str2);
		bool                operator>=(CString str2);

	protected:
		CString& 			Assignment(PCSTR);		
		CString&			Add(const char*);

	protected:
		char 	            m_pStrBuf[MAX_STR_LEN];
};

#endif  // _WINDOWS
#endif 	// _CSTRING_H_

