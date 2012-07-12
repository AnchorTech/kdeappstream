#-------------------------------------------------
#
# Project created by QtCreator 2012-01-26T23:16:56
#
#-------------------------------------------------

    QT       += core gui network script

    TARGET = kappstream-http
    TEMPLATE = app

    SOURCES += main.cpp \
        HttpServer.cpp \
        HttpService.cpp \
    QWsServer.cpp \
    QWsSocket.cpp \
    WebSocketServer.cpp \
    WebSocketService.cpp

    HEADERS  += \
        HttpServer.h \
        HttpService.h \
    QWsServer.h \
    QWsSocket.h \
    WebSocketServer.h \
    WebSocketService.h

    unix {
        LIBS += -ldl
    }

OTHER_FILES += \
    CMakeLists.txt \
    canvas.html \
    index.html

























