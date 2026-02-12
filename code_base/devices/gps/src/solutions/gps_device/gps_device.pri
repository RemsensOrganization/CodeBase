QT += core gui widgets serialport concurrent

include(../../gps.pri)

# Исходники
SOURCES += \
    $$PWD/gps_device.cpp \
    $$PWD/gps_widget.cpp

HEADERS += \
    $$PWD/gps_device.h \
    $$PWD/gps_widget.h

INCLUDEPATH += $$PWD

