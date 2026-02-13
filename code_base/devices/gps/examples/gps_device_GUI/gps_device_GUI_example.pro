TARGET = gps_device_example

TEMPLATE = app

CONFIG += c++11

include(../../src/solutions/gps_device/gps_device.pri)

SOURCES += $$PWD/main.cpp \
    mainwindow.cpp

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h


