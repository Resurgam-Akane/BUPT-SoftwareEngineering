#-------------------------------------------------
#
# Project created by QtCreator 2017-05-22T15:10:20
#
#-------------------------------------------------

QT       += core gui network
QT += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ServerTest
TEMPLATE = app


SOURCES += main.cpp\
    tcpserver.cpp \
    tcpsocket.cpp \
    threadhandle.cpp \
    statementshow.cpp \
    adminstrator.cpp \
    controller.cpp \
    workmode.cpp \
    coldorwarm.cpp \
    fxxkyou.cpp

HEADERS  += \
    tcpserver.h \
    tcpsocket.h \
    threadhandle.h \
    globalvar.h \
    connectsql.h \
    statementshow.h \
    adminstrator.h \
    controller.h \
    workmode.h \
    coldorwarm.h \
    fxxkyou.h

FORMS    += \
    statementshow.ui \
    controller.ui \
    workmode.ui \
    fxxkyou.ui


