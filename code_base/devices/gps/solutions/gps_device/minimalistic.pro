QT += core gui widgets serialport concurrent

TARGET = minimalistic

TEMPLATE = app

CONFIG += c++11

SRC_DIR = ../../src
include($$SRC_DIR/gps.pri)
include(../gui/gui.pri)

SOURCES += main.cpp\
            \
           gps_device.cpp

INCLUDEPATH += $$SRC_DIR

HEADERS += \
    gps_device.h
