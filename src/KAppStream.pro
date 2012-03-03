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
    websocket/HttpServer.cpp

    HEADERS  += \
        paint/PaintEngine.h \
        paint/PaintDevice.h \
    EventDispather.h \
    websocket/HttpServer.h

    unix {
        LIBS += -ldl
    }

OTHER_FILES += \
    ../CMakeLists.txt
