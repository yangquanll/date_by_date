#-------------------------------------------------
#
# Project created by QtCreator 2016-01-04T21:39:12
#
#-------------------------------------------------

QT       += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = heartbeatMonitor
TEMPLATE = app

SOURCES += heartbeatMonitor.cpp\
    heartbeat.cpp

HEADERS  += \
    heartbeat.h 

INCLUDEPATH += \
            /usr/include/glib-2.0 \
            /usr/lib/x86_64-linux-gnu/glib-2.0/include

LIBS    +=-lglib-2.0
LIBS    +=-lgobject-2.0

RESOURCES +=

QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder  #remove warning like 'will be initialized after'
QMAKE_CXXFLAGS_WARN_ON += -Wno-format-security #remove warning: format not a string literal and no format arguments [-Wformat-security]

CONFIG += console

