QT += core testlib serialport
CONFIG += console testcase
TEMPLATE = app
TARGET = gps_parser_tests

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

ROOT_DIR = $$PWD/../..

message("ROOT_DIR = $$ROOT_DIR")
message("src.pri = $$ROOT_DIR/src/src.pri")

include($$ROOT_DIR/src/src.pri)

SOURCES += parser_tests.cpp
HEADERS += parser_tests.h
