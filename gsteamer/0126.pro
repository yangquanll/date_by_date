#-------------------------------------------------
#
# Project created by QtCreator 2018-01-26T17:15:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 0126
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS    +=-lglib-2.0
LIBS    +=-lgobject-2.0
LIBS    +=-lgstreamer-1.0          # <gst/gst.h>
LIBS    +=-lgstvideo-1.0             # <gst/video/videooverlay.h>
LIBS    +=-L/usr/lib/x86_64-linux-gnu/gstreamer-1.0
LIBS    +=-lgstautodetect

INCLUDEPATH += \
            /usr/include/glib-2.0 \
            /usr/lib/x86_64-linux-gnu/glib-2.0/include \
            /usr/include/gstreamer-1.0 \
            /usr/lib/x86_64-linux-gnu/gstreamer-1.0/include/
