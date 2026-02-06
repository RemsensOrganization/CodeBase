QT += core serialport concurrent

TARGET = gps_linux
TEMPLATE = app

CONFIG += c++11 console
CONFIG -=window

SRC_DIR = ../../src
include($$SRC_DIR/gps.pri)

SOURCES += main.cpp\

INCLUDEPATH += $$SRC_DIR


