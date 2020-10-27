// ------------------------------------------------------------------------------
// JCCL - Component :: Qt Control Info
// Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    QtCtrlInfo.c
// Purpose: Qt Control Related definition
//-------------------------------------------------------------------------------

#ifndef _QT_CTRL_INFO_H_
#define _QT_CTRL_INFO_H_

#ifdef QT_CORE_LIB

#include <QString>

extern const char*          GetQString(QString str);

// Qt 标签的区域、字体颜色等信息
struct LABEL_INFO
{
    LABEL_INFO()
    {
        x = y = width = height = fontSizecn = fontSizeen = 0;
        color = "";
        backgroundColor = "";
    }

    int GetFontSize(LANGUAGE_TYPE nLanguage = LANGUAGE_CHINESE)
    {
        if ( nLanguage == LANGUAGE_CHINESE )
            return fontSizecn;
        else
            return fontSizeen;
    }

    int                     x;                                          // x 坐标
    int                     y;                                          // y 坐标
    int                     width;                                      // 宽度
    int                     height;                                     // 高度
    int                     fontSizecn;                                 // 中文字体大小
    int                     fontSizeen;                                 // 英文字体大小
    CString                 color;                                      // 字体颜色
    CString                 backgroundColor;                            // 背景颜色
};

// QT 标签的滚动模式
enum QT_LABEL_SCROLL_SHOW_MODE
{
    SCROLL_SHOW_SCROLL = 0,                                               // 滚动显示
    SCROLL_SHOW_FULL_SCREEN,                                              // 全屏显示
    SCROLL_SHOW_NONE                                                      // 不滚动
};


#endif // QT_CORE_LIB

#endif // _QT_CTRL_INFO_H_
