#-------------------------------------------------
#
# Project created by QtCreator 2020-07-21T15:47:39
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Vehicle
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        /JamesSoft/JCCL/src/Debug.cpp \
        /JamesSoft/JCCL/src/CString.cpp \
        /JamesSoft/JCCL/src/Config.cpp \
        /JamesSoft/JCCL/src/Common.cpp \
        /JamesSoft/JCCL/src/CList.cpp \
        /JamesSoft/JCCL/src/File.cpp \
        /JamesSoft/JCCL/src/MsgBase.cpp \
        /JamesSoft/JCCL/src/MsgSocket.cpp \
        /JamesSoft/JCCL/src/MsgServerBase.cpp \
        /JamesSoft/JCCL/src/MsgClientBase.cpp \
        /JamesSoft/JCCL/src/ModuleList.cpp \
        /JamesSoft/JCCL/src/ModuleBase.cpp \
        /JamesSoft/JCCL/src/Timer.cpp \
        /JamesSoft/JCCL/src/ASN1PERCoding.cpp \
        /JamesSoft/Projects/V2X/src/Common/CSAEMsgBase.cpp \
        /JamesSoft/Projects/V2X/src/Common/CSAEDataFrame.cpp \
        /JamesSoft/Projects/V2X/src/Common/CSAEMessageSet.cpp \
        /JamesSoft/Projects/V2X/src/Common/CSAETest.cpp \

HEADERS += \
        mainwindow.h \
        /JamesSoft/JCCL/include/JCCLTypes.h \
        /JamesSoft/JCCL/include/Debug.h \
        /JamesSoft/JCCL/include/CString.h \
        /JamesSoft/JCCL/include/Config.h \
        /JamesSoft/JCCL/include/Common.h \
        /JamesSoft/JCCL/include/CList.h \
        /JamesSoft/JCCL/include/File.h \
        /JamesSoft/JCCL/include/MsgBase.h \
        /JamesSoft/JCCL/include/MsgSocket.h \
        /JamesSoft/JCCL/include/MsgServerBase.h \
        /JamesSoft/JCCL/include/MsgclientBase.h \
        /JamesSoft/JCCL/include/ModuleBase.h \
        /JamesSoft/JCCL/include/Timer.h \
        /JamesSoft/JCCL/include/stdafx.h \
        /JamesSoft/JCCL/include/ASN1PERCoding.h \
        /JamesSoft/Projects/V2X/src/Common/CSAEMsgBase.h \
        /JamesSoft/Projects/V2X/src/Common/CSAEDataFrame.h \
        /JamesSoft/Projects/V2X/src/Common/CSAEMessageSet.h \
        /JamesSoft/Projects/V2X/src/Common/CSAEAPISPI.h \
        /JamesSoft/Projects/V2X/src/Common/RSUTypes.h \

FORMS += \
        mainwindow.ui


INCLUDEPATH += /JamesSoft/JCCL/include/
INCLUDEPATH += /JamesSoft/Projects/V2X/src/Common

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
