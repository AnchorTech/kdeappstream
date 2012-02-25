#-------------------------------------------------
#
# Project created by QtCreator 2012-01-26T23:16:56
#
#-------------------------------------------------

    QT       += core gui network script
    CONFIG   += qtestlib

    TARGET = kappstream
    TEMPLATE = lib

    SOURCES +=\
        paint/PaintEngine.cpp \
        paint/PaintDevice.cpp \
        hooking/AbstractFunctionOverwriter.cpp \
        hooking/UnixFunctionOverwriter.cpp \
        hooking/FunctionOverwriterFactory.cpp \
        hooking/Probe.cpp \
        events/EventDispather.cpp

    HEADERS  += \
        paint/PaintEngine.h \
        paint/PaintDevice.h \
        hooking/AbstractFunctionOverwriter.h \
        hooking/UnixFunctionOverwriter.h \
        hooking/FunctionOverwriterFactory.h \
        events/EventDispather.h

    unix {
        LIBS += -ldl
    }
