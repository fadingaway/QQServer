#-------------------------------------------------
#
# Project created by QtCreator 2018-11-05T09:48:39
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QQServer
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    filesavethread.cpp \
    tcpserver.cpp

HEADERS  += widget.h \
    filesavethread.h \
    tcpserver.h

FORMS    +=
