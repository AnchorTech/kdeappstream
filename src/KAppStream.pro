#-------------------------------------------------
#
# Project created by QtCreator 2012-01-26T23:16:56
#
#-------------------------------------------------

    QT       += core gui

    TARGET = KAppStream
    TEMPLATE = app

    SOURCES += main.cpp\
        paint/PaintEngine.cpp \
        paint/PaintDevice.cpp

    HEADERS  += \
        paint/PaintEngine.h \
        paint/PaintDevice.h

    unix {
        LIBS += -ldl
    }

OTHER_FILES +=
