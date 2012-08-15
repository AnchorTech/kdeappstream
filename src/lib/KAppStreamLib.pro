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
    events/EventDispather.cpp \
    websocket/WebsocketServer.cpp \
    websocket/QWsSocket.cpp \
    websocket/QWsServer.cpp \
    websocket/HttpServer.cpp \
    events/EventFilter.cpp \
    paint/JSONBuilder.cpp \
    paint/WebRenderer.cpp \
    websocket/WebsocketThread.cpp

HEADERS  += \
    paint/PaintEngine.h \
    paint/PaintDevice.h \
    hooking/AbstractFunctionOverwriter.h \
    hooking/UnixFunctionOverwriter.h \
    hooking/FunctionOverwriterFactory.h \
    websocket/WebsocketServer.h \
    websocket/QWsSocket.h \
    websocket/QWsServer.h \
    websocket/HttpServer.h \
    paint/JSONBuilder.h \
    paint/WebRenderer.h \
    websocket/WebsocketThread.h \
    events/EventFilter.h \
    events/EventDispather.h

unix {
    LIBS += -ldl
}

OTHER_FILES += \
    websocket/index.html \
    ../CMakeLists.txt

