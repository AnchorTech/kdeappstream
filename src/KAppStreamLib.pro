#-------------------------------------------------
#
# Project created by QtCreator 2012-01-26T23:16:56
#
#-------------------------------------------------

<<<<<<< HEAD
    QT       += core gui network script
=======
    QT       += core gui network
>>>>>>> b6535deb629b9d9eed47207a2ad88c73fd47f03c

    TARGET = kappstream
    TEMPLATE = lib

    SOURCES +=\
        paint/PaintEngine.cpp \
        paint/PaintDevice.cpp \
        hooking/AbstractFunctionOverwriter.cpp \
        hooking/UnixFunctionOverwriter.cpp \
        hooking/FunctionOverwriterFactory.cpp \
        hooking/Probe.cpp \
<<<<<<< HEAD
    events/EventDispather.cpp
=======
        websocket/QWsSocket.cpp \
        websocket/QWsServer.cpp \
        websocket/WebsocketServer.cpp \
        websocket/HttpServer.cpp \
        hooking/Probe.cpp
>>>>>>> b6535deb629b9d9eed47207a2ad88c73fd47f03c

    HEADERS  += \
        paint/PaintEngine.h \
        paint/PaintDevice.h \
        hooking/AbstractFunctionOverwriter.h \
        hooking/UnixFunctionOverwriter.h \
        hooking/FunctionOverwriterFactory.h \
<<<<<<< HEAD
    events/EventDispather.h
=======
        websocket/QWsSocket.h \
        websocket/QWsServer.h \
        websocket/WebsocketServer.h \
        websocket/HttpServer.h \
        hooking/FunctionOverwriterFactory.h
>>>>>>> b6535deb629b9d9eed47207a2ad88c73fd47f03c

    unix {
        LIBS += -ldl
    }

<<<<<<< HEAD


=======
OTHER_FILES += \
    websocket/index.html
>>>>>>> b6535deb629b9d9eed47207a2ad88c73fd47f03c
