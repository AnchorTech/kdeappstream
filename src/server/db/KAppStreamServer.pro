#-------------------------------------------------
#
# Project created by QtCreator 2012-01-26T23:16:56
#
#-------------------------------------------------

    QT       += core gui network script

    TARGET = KAppStreamServerDatabase
    TEMPLATE = lib

    SOURCES += DatabaseService.cpp \
        DatabaseObject.cpp \
        Settings.cpp \
        Application.cpp \
        Connection.cpp

    HEADERS  += DatabaseService.h \
        DatabaseObject.h \
        Settings.h \
        Application.h \
        Connection.h

    unix {
        LIBS += -ldl
    }

OTHER_FILES += \
    CMakeLists.txt \
    connections.data

















