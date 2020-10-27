/*------------------------------------------------------------------------------
 * JCCL - Component :: List
 * Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    CList.h
 * Purpose: List definition
 *----------------------------------------------------------------------------*/

#ifndef _CLIST_H_
#define _CLIST_H_

#ifndef _WINDOWS

#include "JCCLTypes.h"

// #define MAX_LIST_LENGTH     4096                                        // 列表最大长度


// /////////////////////////////////////////////////////////////////////
// 
// *********************************************************************
// 注意：由于该列表不创建对象，仅仅保存了对象的地址。
// 因此，传递给队列的对象如果是局部变量，在退出局部变量有效范围后，对象地址已经失效，
// 此时再引用队列中保存的对象地址则会造成不可预知的后果。
// *********************************************************************
// /////////////////////////////////////////////////////////////////////

// 
// 通过数组方式来实现列表，以减少内存频繁申请和释放
// 也可以通过双向链表方式实现列表
class CListItem : public CObjBase
{
    public:
        CListItem() { m_pPrev = NULL; m_pNext = NULL; m_pItem = NULL; }

    public:
        CListItem*          m_pPrev;
        CListItem*          m_pNext;
        CObjBase*           m_pItem;
};

class CList : public CObjBase
{
    public:
        CList(bool bMode = false);
        ~CList();

    // Operations
    public:
        int                 GetCount();                                 // 列表长度
        bool                IsEmpty();                                  // 列表是否为空

        bool                Add(CObjBase* pObject);                     // 默认在列表末尾增加一个节点
        bool                AddTail(CObjBase* pObject);                 // 在列表末尾增加一个节点
        bool                AddHead(CObjBase* pObject);                 // 在列表头部增加一个节点
        bool                AddBefore(CObjBase* pObject, CObjBase* pCurObj);    // 在列表指定节点前增加
        bool                AddAfter(CObjBase* pObject, CObjBase* pCurObj);     // 在列表指定节点后增加

        bool                Remove(CObjBase* pObject);                  // 删除一个节点，若存在的话
        bool                Remove(int nIndex);
        
        bool                RemoveAll();                                // 删除所有节点

        // bool                Merge(CList* pList);                        // 合并列表
        // bool                SubList(CList* pList);                      // 删除两个列表中相同的节点
        // CList               operator=(CList* pLis);

        bool                Find(CObjBase* pObject);                    // 查找一个节点是否在列表中

        CObjBase*           GetHead();                                  // 获取列表的第一个节点
        CObjBase*           GetTail();                                  // 获取列表的最后一个节点
        CObjBase*           GetNext(CObjBase* pObject);                 // 获取下一个节点
        CObjBase*           GetPrev(CObjBase* pObject);                 // 获取上一个节点
        CObjBase*           GetAt(int nIndex);                          // 获取指定位置的节点


        void                SetMode(bool bMode) { m_bMode = bMode; }

    // Attributes
    protected:
        CListItem*          FindObject(CObjBase* pObject);              // 查找节点

        CListItem*          m_pListHead;                                // 列表链表头
        bool                m_bMode;                                    // true: 对象删除时自动删除节点内存
};

#endif  // _WINDOWS

#endif  // _CLIST_H_

