QT += core serialport concurrent

include(../../gps.pri)

SOURCES += \
    $$PWD/gps_device.cpp \

HEADERS += \
    $$PWD/gps_device.h \
    $$PWD/gps_view_interface.h

INCLUDEPATH += $$PWD

