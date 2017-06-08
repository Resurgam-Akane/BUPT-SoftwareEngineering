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
        mainwindow.cpp \
    tcpserver.cpp \
    tcpsocket.cpp \
    threadhandle.cpp \
    statementshow.cpp

HEADERS  += mainwindow.h \
    tcpserver.h \
    tcpsocket.h \
    threadhandle.h \
    globalvar.h \
    connectsql.h \
    statementshow.h

FORMS    += mainwindow.ui \
    statementshow.ui
