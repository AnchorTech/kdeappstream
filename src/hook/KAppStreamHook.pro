CONFIG   += qtestlib

TARGET = kappstreamhook
TEMPLATE = lib

SOURCES +=\
    kernel/qwidget.cpp

unix {
    LIBS += -ldl
}

OTHER_FILES += \
    ../CMakeLists.txt





