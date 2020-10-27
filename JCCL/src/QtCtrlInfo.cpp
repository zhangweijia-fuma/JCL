// ------------------------------------------------------------------------------
// JCCL - Component :: Qt Control Info
// Copyright (c) 2015-2018 JamesSoft Tech. Ltd.. All rights reserved.
// Author: Zhang Weijia
// ------------------------------------------------------------------------------
// Name:    QtCtrlInfo.c
// Purpose: Qt Control Related definition
//-------------------------------------------------------------------------------

#ifdef QT_CORE_LIB
#include <QDebug>
#include <QProcess>
#include <QTextCodec>

#include "QtCtrlInfo.h"

const char* GetQString(QString str)
{
    char *ch;
    QByteArray ba = str.toUtf8();
    ch = ba.data();
    return ch;
}

#endif // QT_CORE_LIB

