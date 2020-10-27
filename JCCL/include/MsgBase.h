/*------------------------------------------------------------------------------
 * JCCL - Component :: Base Message
 * Copyright (c) 2015-2020 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    MsgBase.h
 * Purpose: JCCL module base message definition
 *----------------------------------------------------------------------------*/
#ifndef _MSG_BASE_H_
#define _MSG_BASE_H_

#include "JCCLTypes.h"
#include "CString.h"
#include "Common.h"

#include "Codec.h"

#define MSG_HEAD_FLAG           0xA5A5A5A5                              // 消息头部开始标志
#define MSG_END_FLAG            0xDEDEDEDE                              // 消息结束标志
#define MAX_SOCKET_WAIT_TIME    60 * 60 * 24                            // Socket 接收消息的等待时间
#define MSG_HEARTBEAT_INTERVAL  6                                       // 心跳消息上报时间间隔，单位为秒
#define MSG_EXPIRE_TIMES        3                                       // 消息最大超时次数
#define MSG_RESEND_INTERVAL     5                                       // 消息重发时间间隔，单位：秒
#define MAX_MSG_LEN             4096                                    // 发送和接收的消息最大长度

#define INVALID_MSG             0xFFFF
#define INVALID_MODULE          0xFF

#define MSG_OP_FAILED           0                                       // 消息操作失败
#define MSG_OP_PART             1                                       // 消息操作部分完成
#define MSG_OP_COMPLETE         2                                       // 消息操作完成

#define ACK_RECEIVE             1                                       // 发送ACK消息
#define ACK_SEND                2                                       // 接收ACK消息


// 共有的消息参数类型
enum UPDATE_MODE
{
    Update_NextStart = 0,                                               // 下次重启时更新
    Update_Immediately,                                                 // 立即重启
};


enum REBOOT_MODE
{
    Reboot_Application = 0,                                             // 重启应用
    Reboot_System,                                                      // 重启系统
};


// 消息基类
class CMsgBase : public CCodec
{
    public:
        CMsgBase();
        ~CMsgBase();

    public:
        virtual const char* toString() = 0;
        virtual const char* toParamString() { return ""; }

        virtual bool        Decode(PUCHAR pBuffer, UINT nMsgLen);
        virtual bool        Encode(UINT &nMsgLen);

        // 仅仅获取消息的消息类型
        UINT                DecodeMsgType(PUCHAR pBuffer, UINT nMsgLen);

        void                SetMsgType(UINT nMsgType) { m_nMsgType = nMsgType; };
        UINT                GetMsgType() { return m_nMsgType; };

        const char*         GetSrcAddr()        { return m_SrcAddr.GetBuffer(); };
        UINT                GetSrcPort()        { return m_nSrcPort; };
        const char*         GetDestAddr()       { return m_DestAddr.GetBuffer(); };
        UINT                GetDestPort()       { return m_nDestPort; };
        UINT                GetSerialNo()       { return m_nSerialNo; };
        const char*         GetSrcModuleCode()  { return m_SrcModuleCode.GetBuffer(); };
        UINT                GetSrcModuleType()  { return m_nSrcModuleType; };
        const char*         GetDestModuleCode() { return m_DestModuleCode.GetBuffer(); };
        UINT                GetDestModuleType() { return m_nDestModuleType; };

        void                SetSrcAddr(CString srcAddr)         { m_SrcAddr         = srcAddr; };
        void                SetSrcPort(UINT nSrcPort)           { m_nSrcPort        = nSrcPort; };
        void                SetDestAddr(CString DestAddr)       { m_DestAddr        = DestAddr; };
        void                SetDestPort(UINT nDestPort)         { m_nDestPort       = nDestPort; };
        void                SetSerialNo(UINT nSerialNo)         { m_nSerialNo       = nSerialNo; };
        void                SetSrcModuleCode(CString code)      { m_SrcModuleCode   = code; };
        void                SetSrcModuleType(UINT ModuleType)   { m_nSrcModuleType  = ModuleType; };
        void                SetDestModuleCode(CString code)     { m_DestModuleCode  = code; };
        void                SetDestModuleType(UINT ModuleType)  { m_nDestModuleType = ModuleType; };

        const char*         GetSrcModuleName() { return GetModuleTypeStr(m_nSrcModuleType); };
        const char*         GetDestModuleName() { return GetModuleTypeStr(m_nDestModuleType); };

    // Attributes
    protected:

        // 消息头部参数
        UINT                m_nHeadFlag;                                // 消息头部标识
        UINT                m_nEndFlag;                                 // 消息尾部标识
        CString             m_LocalAddr;                                // 本机地址
        CString             m_SrcAddr, m_DestAddr;                      // 发送接收方IP地址
        UINT                m_nSrcPort, m_nDestPort;                    // 发送接收方IP端口
        UINT                m_nMsgType;                                 // 消息类型，由业务定义
        UINT                m_nSerialNo;                                // 消息序列号，对发送的每一条消息，代表发送次数，初始值为0
        CString             m_strMacAddr;                               // MAC地址或模块编号表示ID，根据mac地址或模块编码来确定模块
        CString             m_SrcModuleCode;                            // 模块编码由业务模块设置
        UINT                m_nSrcModuleType;                           // 模块类型
        CString             m_DestModuleCode;                           // 模块编码由业务模块设置
        UINT                m_nDestModuleType;                          // 模块类型
};

// 应答消息基类
// 无参数应答消息编码
// 有参数应答消息由各自的编码函数进行编码
class CMsgAckBase : public CMsgBase
{
    public:
        CMsgAckBase()
        {
            m_nAckType  = ACK_RECEIVE;
            m_nStatus   = MSG_OP_COMPLETE;
        };

    // Operations
    public:
        virtual bool        Decode(PUCHAR pBuffer, UINT nMsgLen);
        virtual bool        Encode(UINT &nMsgLen);
        virtual const char* toString() { return "BaseAckMsg";}

    // Attribuytes
    protected:
        UINT                m_nAckType;
        UINT                m_nStatus;
};

// 所有业务模块使用相同的心跳消息及心跳响应消息

// 心跳消息及相应消息为所有业务应用公用的消息，无参数
// 心跳消息，无参数
class CHeartBeatMsg : public CMsgBase 
{
    public:
        CHeartBeatMsg() { m_nMsgType  = MSG_HEARTBEAT; }
        virtual bool        Decode(PUCHAR pBuffer, UINT nMsgLen);
        virtual bool        Encode(UINT &nMsgLen);
        virtual const char* toString() { return "HeartbeatMsg";}
};

// 心跳应答消息
class CHeartBeatAckMsg : public CMsgBase
{
    public:
        CHeartBeatAckMsg() { m_nMsgType  = MSG_HEARTBEAT_ACK; }
        virtual bool        Decode(PUCHAR pBuffer, UINT nMsgLen);
        virtual bool        Encode(UINT &nMsgLen);
        virtual const char* toString() { return "HeartbeatAckMsg";}        
};

// 消息处理回调函数定义
// pCallbackParam：通常为对象实例
typedef void (*MSG_CALLBACK)(LPVOID pCallbackParam, PUCHAR buffer, int nLen); 
typedef void (*RECEIVE_DATA_CALLBACK)(LPVOID pCallbackParam, PUCHAR pBuffer, UINT nMsgLen, LPCSTR strSrcAddr, UINT nSrcPort);

// 不需要在运行日志中显示的消息类型列表
#define MAX_NO_SHOW_MSG_COUNT           10

class CNoShowMsgList : public CObjBase
{
    public:
        CNoShowMsgList();

    public:
        void                InsertNoShowMsgType(UINT nMsgType);
        bool                IsMsgInNoShowMsgList(UINT nMsgType);

    protected:
        UINT                m_MsgList[MAX_NO_SHOW_MSG_COUNT];
        int                 m_nMsgCount;
};

extern CNoShowMsgList g_NoShowMsgList;

#endif // _MSG_BASE_H_
