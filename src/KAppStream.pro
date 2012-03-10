#-------------------------------------------------
#
# Project created by QtCreator 2012-01-26T23:16:56
#
#-------------------------------------------------

    QT       += core gui network script

    TARGET = KAppStream
    TEMPLATE = app

    SOURCES += main.cpp\
    websocket/HttpServer.cpp \
    websocket/WebsocketServer.cpp \
    websocket/QWsSocket.cpp \
    websocket/QWsServer.cpp \
    paint/JSONBuilder.cpp

    HEADERS  += \
    EventDispather.h \
    websocket/HttpServer.h \
    websocket/WebsocketServer.h \
    websocket/QWsSocket.h \
    websocket/QWsServer.h \
    paint/JSONBuilder.h

    unix {
        LIBS += -ldl
    }

OTHER_FILES += \
    ../CMakeLists.txt \
    websocket/index.html












