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
        paint/PaintDevice.h \
    EventDispather.h

    unix {
        LIBS += -ldl
    }

<<<<<<< HEAD









=======
OTHER_FILES += \
    ../CMakeLists.txt
>>>>>>> b6535deb629b9d9eed47207a2ad88c73fd47f03c

