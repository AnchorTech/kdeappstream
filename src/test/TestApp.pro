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
    test/main_qt.cpp \
    paint/WebRenderer.cpp \
    websocket/WebsocketThread.cpp \
    websocket/WebsocketServer.cpp \
    websocket/QWsSocket.cpp \
    websocket/QWsServer.cpp

HEADERS  += \
            test/TestWindow.h \
    paint/PaintEngine.h \
    paint/PaintDevice.h \
    paint/JSONBuilder.h \
    events/EventFilter.h \
    events/EventDispather.h \
    paint/WebRenderer.h \
    websocket/WebsocketThread.h \
    websocket/WebsocketServer.h \
    websocket/QWsSocket.h \
    websocket/QWsServer.h

INCLUDEPATH += paint

FORMS    += \
            test/TestWindow.ui

OTHER_FILES += \
    websocket/index.html













