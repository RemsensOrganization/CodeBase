QT += core testlib serialport

CONFIG += console testcase c++11

TARGET = parser_tests

TEMPLATE = app

include(../../src/src.pri)

SOURCES += parser_tests.cpp
HEADERS += parser_tests.h
