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
    ApplicationWrapperThread.cpp \
    ACLProvider.cpp

    HEADERS  += \
        HttpServer.h \
        HttpService.h \
    ApplicationWrapperThread.h \
    ACLProvider.h

    unix {
        LIBS += -ldl
    }

OTHER_FILES += \
    CMakeLists.txt \
    canvas.html \
    index.html





































