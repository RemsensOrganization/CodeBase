QT += core gui widgets serialport

TARGET = GPS_Controller

TEMPLATE = app

CONFIG += c++11

SRC_DIR = ../../src
include($$SRC_DIR/gps.pri)

SOURCES += main.cpp\
           controller.cpp

HEADERS += controller.h

INCLUDEPATH += $$SRC_DIR
