/*------------------------------------------------------------------------------
 * JCCL - Component :: List
 * Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
 * Author: Zhang Weijia
 *------------------------------------------------------------------------------
 * Name:    CList.cpp
 * Purpose: List definition
 *----------------------------------------------------------------------------*/

#include "CList.h"
#include "Debug.h"

CList::CList(bool bMode)
{
    m_pListHead = NULL;
    m_bMode     = bMode;
}

CList::~CList()
{
    RemoveAll();
}

int CList::GetCount()
{
    int nCount = 0;
    CListItem* pItem = m_pListHead;
    while(pItem != NULL)
    {
        nCount++;        
        pItem = pItem->m_pNext;
    }

    return nCount;
}

bool CList::IsEmpty()
{
    return ( m_pListHead == NULL );
}

bool CList::Add(CObjBase* pObject)                                      // 默认在列表末尾增加一个节点
{
    return AddTail(pObject);
}

bool CList::AddTail(CObjBase* pObject)                                  // 在列表末尾增加一个节点
{
    if( pObject == NULL )
    {
        LogE("Add a null object to List");
        return false;
    }

    if ( FindObject(pObject) != NULL )
    {
        // 元素已经在列表中
        LogE("Object already in list during add a object to list");
        return false;
    }

    CListItem* pItem = new CListItem();
    if ( pItem == NULL )
    {
        LogE("CList alloc object failed");
        return false;
    }
    pItem->m_pItem = pObject;

    CListItem* pTemp = m_pListHead;
    CListItem* pTail = NULL;
    while(pTemp != NULL)
    {
        pTail = pTemp;
        pTemp = pTemp->m_pNext;
    }

    if ( pTail == NULL )
    {
        // 此时链表为空
        m_pListHead = pItem;
        return true;
    }
    else
    {
        pTail->m_pNext = pItem;
        pItem->m_pPrev = pTail;
    }

    return true;
}

bool CList::AddHead(CObjBase* pObject)                                  // 在列表头部增加一个节点
{
    if( pObject == NULL )
    {
        LogE("Add a null object to List");
        return false;
    }

    if ( FindObject(pObject) != NULL )
    {
        // 元素已经在列表中
        LogE("Object already in list during add a object to list");
        return false;
    }

    CListItem* pItem = new CListItem();
    if ( pItem == NULL )
    {
        LogE("CList alloc object failed");
        return false;
    }

    pItem->m_pItem = pObject;

    if ( m_pListHead == NULL )
    {
        m_pListHead = pItem;
        return true;
    }

    pItem->m_pNext = m_pListHead;
    m_pListHead->m_pPrev = pItem;
    m_pListHead = pItem;

    return true;
}

// 在列表指定节点前增加
bool CList::AddBefore(CObjBase* pObject, CObjBase* pCurObj)
{
    if( pObject == NULL )
    {
        LogE("Add a null object to List");
        return false;
    }

    if ( FindObject(pObject) != NULL )
    {
        // 元素已经在列表中
        LogE("Object already in list during add a object to list");
        return false;
    }

    CListItem* pItem = new CListItem();
    if ( pItem == NULL )
    {
        LogE("CList alloc object failed");
        return false;
    }

    pItem->m_pItem = pObject;

    CListItem* pCurItem = FindObject(pCurObj);

    if ( pCurItem == NULL )
    {
        LogE("Can not find insert object in list");
        return false;
    }

    // 在指定位置前插入
    CListItem* pPrev = pCurItem->m_pPrev;
    if ( pPrev == NULL )
    {
        // 当前节点为列表头
        pItem->m_pNext = m_pListHead;
        m_pListHead->m_pPrev = pItem;
        m_pListHead = pItem;
    }
    else
    {
        pPrev->m_pNext      = pItem;
        pItem->m_pPrev      = pPrev;
        pItem->m_pNext      = pCurItem;
        pCurItem->m_pPrev   = pItem;
    }

    return true;
}

bool CList::AddAfter(CObjBase* pObject, CObjBase* pCurObj)                                 // 在列表指定节点后增加
{
    if( pObject == NULL )
    {
        LogE("Add a null object to List");
        return false;
    }

    if ( FindObject(pObject) != NULL )
    {
        // 元素已经在列表中
        LogE("Object already in list during add a object to list");
        return false;
    }

    CListItem* pItem = new CListItem();
    if ( pItem == NULL )
    {
        LogE("CList alloc object failed");
        return false;
    }

    pItem->m_pItem = pObject;

    CListItem* pCurItem = FindObject(pCurObj);
    if ( pCurItem == NULL )
    {
        LogE("Can not find insert object in list");
        return false;
    }

    // 在指定位置后插入
    CListItem* pNext = pCurItem->m_pNext;
    if ( pNext == NULL )
    {
        // 当前节点为列表尾部
        pCurItem->m_pNext = pItem;
        pItem->m_pPrev = pCurItem;
    }
    else
    {
        pCurItem->m_pNext   = pItem;
        pItem->m_pPrev      = pCurItem;
        pItem->m_pNext      = pNext;
        pNext->m_pPrev      = pItem;
    }

    return true;
}

bool CList::Remove(CObjBase* pObject)                                   // 删除一个节点，若存在的话
{
    CListItem* pItem = FindObject(pObject);
    if ( pItem == NULL )
    {
        LogE("Can not find object from list during Remove");
        return false;
    }

    CListItem* pPrev, *pNext;

    pPrev = pItem->m_pPrev;
    pNext = pItem->m_pNext;
    if ( pPrev == NULL && pNext != NULL )
    {
        // 被删除节点为列表头
        m_pListHead = pNext;
    }
    else if ( pPrev != NULL && pNext == NULL )
    {
        // 被删除节点是列表未
        pPrev->m_pNext = NULL;
    }
    else if ( pPrev == NULL && pNext == NULL )
    {
        // 此时列表就只有一个节点
        if ( m_bMode == true && pItem->m_pItem != NULL )
            delete pItem->m_pItem;

        delete pItem;
        m_pListHead = NULL;
    }
    else
    {
        // 删除节点为中间节点，且列表长度至少为3
        pPrev->m_pNext = pNext;
        pNext->m_pPrev = pPrev;

        if ( m_bMode == true && pItem->m_pItem != NULL )
            delete pItem->m_pItem;

        delete pItem;
    }

    return true;
}

bool CList::Remove(int nIndex)
{
    int nPos = 0;
    CListItem* pItem = m_pListHead;
    while( pItem != NULL )
    {
        if ( nPos == nIndex )
            return Remove(pItem->m_pItem);

        nPos ++;
        pItem = pItem->m_pNext;
    }

    return false;
}

bool CList::RemoveAll()                                                 // 删除所有节点
{
    CListItem* pItem = m_pListHead;
    while( pItem != NULL )
    {
        if ( m_bMode == true && pItem->m_pItem != NULL )
            delete pItem->m_pItem;
 
        CListItem* pTemp = pItem->m_pNext;
        delete pItem;
        pItem = pTemp;
    }

    m_pListHead = NULL;
    return true;
}

// bool CList::Merge(CList* pList)                                         // 合并两个列表
// {
//     return false;
// }

// bool CList::SubList(CList* pList)                                       // 删除相同节点
// {
//     return false;
// }

// CList CList::operator=(CList* pList)
// {
//     return (*this);
// }

bool CList::Find(CObjBase* pObject)                                     // 查找一个节点是否在列表中
{
    CListItem* pItem = m_pListHead;
    while(pItem != NULL)
    {
        if ( pItem->m_pItem == pObject)
            return true;

        pItem = pItem->m_pNext;
    }

    return false;
}

CObjBase* CList::GetHead()                                              // 获取列表的第一个节点
{
    if ( m_pListHead != NULL )
        return m_pListHead->m_pItem;

    return NULL;
}

CObjBase* CList::GetTail()                                              // 获取列表的最后一个节点
{
    CListItem* pItem = m_pListHead;
    CListItem* pTail = NULL;
    while(pItem != NULL)
    {
        pTail = pItem;
        pItem = pItem->m_pNext;
    }

    if ( pTail != NULL )
        return pTail->m_pItem;

    return NULL;
}

CObjBase* CList::GetNext(CObjBase* pObject)                             // 获取下一个节点
{
    CListItem* pItem = m_pListHead;
    while(pItem != NULL)
    {
        if ( pItem->m_pItem == pObject)
        {
            CListItem* pNext = pItem->m_pNext;
            if ( pNext != NULL )
                return pNext->m_pItem;
            else
                return NULL;
        }
        
        pItem = pItem->m_pNext;
    }

    return NULL;
}

CObjBase* CList::GetPrev(CObjBase* pObject)                             // 获取上一个节点
{
    CListItem* pItem = m_pListHead;
    CListItem* pPrev = NULL;
    while(pItem != NULL)
    {
        if ( pItem->m_pItem == pObject)
        {
            if ( pPrev != NULL )
                return pPrev->m_pItem;
            else
                return NULL;
        }
        
        pPrev = pItem;
        pItem = pItem->m_pNext;
    }

    return NULL;
}

CObjBase* CList::GetAt(int nIndex)
{
    int nPos = 0;
    CListItem* pItem = m_pListHead;
    while(pItem != NULL)
    {
        if ( nPos == nIndex)
            return pItem->m_pItem;
        
        nPos ++;
        pItem = pItem->m_pNext;
    }

    return NULL;
}

CListItem* CList::FindObject(CObjBase* pObject)
{
    CListItem* pTemp = m_pListHead;
    while(pTemp != NULL)
    {
        if ( pTemp->m_pItem == pObject )
            return pTemp;

        pTemp = pTemp->m_pNext;
    }

    return NULL;
}