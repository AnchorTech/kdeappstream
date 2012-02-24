#-------------------------------------------------
#
# Project created by QtCreator 2012-01-26T23:16:56
#
#-------------------------------------------------

    QT       += core gui

    TARGET = kappstream_lib
    TEMPLATE = lib

    SOURCES +=\
        paint/PaintEngine.cpp \
        paint/PaintDevice.cpp \
        hooking/AbstractFunctionOverwriter.cpp \
        hooking/UnixFunctionOverwriter.cpp \
        hooking/FunctionOverwriterFactory.cpp \
        hooking/Probe.cpp

    HEADERS  += \
        paint/PaintEngine.h \
        paint/PaintDevice.h \
        hooking/AbstractFunctionOverwriter.h \
        hooking/UnixFunctionOverwriter.h \
        hooking/FunctionOverwriterFactory.h

    unix {
        LIBS += -ldl
    }

