#-------------------------------------------------
#
# Project created by QtCreator 2012-01-26T23:16:56
#
#-------------------------------------------------

    QT       += core gui network script

    TARGET = KAppStream
    TEMPLATE = app

    SOURCES += main.cpp\
        paint/PaintEngine.cpp \
        paint/PaintDevice.cpp \
    websocket/HttpServer.cpp \
    websocket/WebsocketServer.cpp \
    websocket/QWsSocket.cpp \
    websocket/QWsServer.cpp

    HEADERS  += \
        paint/PaintEngine.h \
        paint/PaintDevice.h \
    EventDispather.h \
    websocket/HttpServer.h \
    websocket/WebsocketServer.h \
    websocket/QWsSocket.h \
    websocket/QWsServer.h

    unix {
        LIBS += -ldl
    }

OTHER_FILES += \
    ../CMakeLists.txt \
    websocket/index.html
