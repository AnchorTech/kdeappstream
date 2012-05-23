#-------------------------------------------------
#
# Project created by QtCreator 2012-01-26T23:16:56
#
#-------------------------------------------------

    QT       += core gui network script

    TARGET = KAppStreamServer
    TEMPLATE = app

    SOURCES += main.cpp \
        HttpServer.cpp \
    HttpService.cpp

    HEADERS  += \
        HttpServer.h \
    HttpService.h

    unix {
        LIBS += -ldl
    }

OTHER_FILES += \
    CMakeLists.txt \
    index.html

















