#-------------------------------------------------
#
# Project created by QtCreator 2017-05-22T15:10:20
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ServerTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tcpserver.cpp \
    tcpsocket.cpp \
    threadhandle.cpp

HEADERS  += mainwindow.h \
    tcpserver.h \
    tcpsocket.h \
    threadhandle.h \
    globalvar.h

FORMS    += mainwindow.ui
