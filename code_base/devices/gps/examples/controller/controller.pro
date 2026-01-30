QT += core gui widgets serialport

TARGET = GPS_Controller

TEMPLATE = app

CONFIG += c++11

include(../../src/gps.pri)

SOURCES += main.cpp controller.cpp
HEADERS += controller.h

INCLUDEPATH += $$PWD/../../src
