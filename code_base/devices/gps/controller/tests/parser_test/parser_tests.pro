QT       += core testlib serialport
CONFIG   += console testcase
TEMPLATE  = app
TARGET    = gps_parser_tests

CONFIG  += c++11
DEFINES += QT_DEPRECATED_WARNINGS

ROOT_DIR = $$PWD/../..

INCLUDEPATH += $$ROOT_DIR
INCLUDEPATH += $$ROOT_DIR/data
INCLUDEPATH += $$ROOT_DIR/data_parser

include($$ROOT_DIR/data/gps_data.pri)
include($$ROOT_DIR/data_parser/gps_parser.pri)

SOURCES += parser_tests.cpp
HEADERS += parser_tests.h
