// ------------------------------------------------------------------------------
// JCCL - Component :: James Common C Lib, Base types definition
// Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    JCCLTypes.h
// Purpose: define base types
//-------------------------------------------------------------------------------

 #ifndef _JCCL_TYPE_H_
 #define _JCCL_TYPE_H_

using namespace std;
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <typeinfo>

#ifndef _WINDOWS
#include <sys/socket.h>
#include <unistd.h>
#include <net/if.h>                                                     // for ifconf
#include <linux/sockios.h>                                              // for net status mask
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>
#include <errno.h>
#include <string>
#include <list>
#include <assert.h>
#include <time.h>
#include <termios.h>
#include <math.h>
#include <stdint.h>

#define INVALID_SOCKET                  (-1)
#define SOCKET_ERROR                    (-1)

#endif // _WINDOWS

typedef unsigned long 	ULONG;
typedef long 			LONG;
typedef long long       LONGLONG;
typedef unsigned long long ULONGLONG;

typedef ULONG* 			PULONG;
typedef ULONG 			DWORD;
typedef unsigned short 	USHORT;
typedef USHORT* 		PUSHORT;
typedef USHORT 			WORD;
typedef unsigned char 	UCHAR;
typedef UCHAR* 			PUCHAR;
typedef UCHAR 			BYTE;
typedef unsigned int 	UINT;
typedef void* 			LPVOID;

typedef char *LPSTR, *PSTR;
typedef const char *LPCSTR, *PCSTR;

typedef LPSTR   LPTCH, PTCH;
typedef LPSTR   PTSTR, LPTSTR;
typedef LPCSTR  LPCTSTR;

typedef UCHAR   TIMER_ID;
typedef UINT    MSG_ID;

// Max Log file length: 10M Bytes
#define MAX_LOG_FILE_LENGTH             10485760

// Operation Code
#define OP_CODE_SUCCESS                 0
#define OP_CODE_TIMEOUT                 1
#define OP_CODE_DRIVER_BUSY             2
#define OP_CODE_BUS_BUSY                3
#define OP_CODE_OP_ONGOING              4

#define INVALID_SOCKET_PORT             65535

#define MAX_STR_LEN                     512                             // 字符串最大长度

#define MAX_MSG_LEN                     4096                            // 发送和接收的消息最大长度

// 语言类型
enum LANGUAGE_TYPE
{
    LANGUAGE_CHINESE = 0,
    LANGUAGE_ENGLISH,

    LANGUAGE_GUANGDONG,                                                 // 广东话，粤语
    
    LANGUAGE_THAILAND,
    LANGUAGE_JAPANESE,

    LANGUAGE_UNKNOWN
};


// debug switch
#ifndef _DEBUG
#define _DEBUG
#endif

// 所有对象的基类，目前暂时无方法
class CObjBase
{
    public:
        CObjBase(){};

        virtual PCSTR       toString() { return "CObjBase"; }
};

#endif // _JCCL_TYPE_H_
