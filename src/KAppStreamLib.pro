#-------------------------------------------------
#
# Project created by QtCreator 2012-01-26T23:16:56
#
#-------------------------------------------------

    QT       += core gui network

    TARGET = kappstream_lib
    TEMPLATE = lib

    SOURCES +=\
        paint/PaintEngine.cpp \
        paint/PaintDevice.cpp \
        hooking/AbstractFunctionOverwriter.cpp \
        hooking/UnixFunctionOverwriter.cpp \
        hooking/FunctionOverwriterFactory.cpp \
        hooking/Probe.cpp \
    websocket/QWsSocket.cpp \
    websocket/QWsServer.cpp \
    websocket/WebsocketServer.cpp \
    websocket/HttpServer.cpp

    HEADERS  += \
        paint/PaintEngine.h \
        paint/PaintDevice.h \
        hooking/AbstractFunctionOverwriter.h \
        hooking/UnixFunctionOverwriter.h \
        hooking/FunctionOverwriterFactory.h \
    websocket/QWsSocket.h \
    websocket/QWsServer.h \
    websocket/WebsocketServer.h \
    websocket/HttpServer.h

    unix {
        LIBS += -ldl
    }

OTHER_FILES += \
    websocket/index.html
