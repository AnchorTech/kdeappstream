#-------------------------------------------------
#
# Project created by QtCreator 2012-01-26T23:16:56
#
#-------------------------------------------------

QT       += core gui network script

TARGET = test
TEMPLATE = app

SOURCES += \
           test/TestWindow.cpp \
    paint/PaintEngine.cpp \
    paint/PaintDevice.cpp \
    paint/JSONBuilder.cpp \
    events/EventFilter.cpp \
    events/EventDispather.cpp \
    test/main_qt.cpp

HEADERS  += \
            test/TestWindow.h \
    paint/PaintEngine.h \
    paint/PaintDevice.h \
    paint/JSONBuilder.h \
    events/EventFilter.h \
    events/EventDispather.h

FORMS    += \
            test/TestWindow.ui






