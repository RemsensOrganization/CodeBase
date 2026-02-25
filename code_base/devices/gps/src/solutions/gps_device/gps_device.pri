QT += core serialport concurrent

!gps_console_build {
QT += gui widgets
SOURCES += $$PWD/gps_widget.cpp
HEADERS += $$PWD/gps_widget.h
}

include(../../gps.pri)

SOURCES += \
    $$PWD/gps_device.cpp \

HEADERS += \
    $$PWD/gps_device.h \

INCLUDEPATH += $$PWD

