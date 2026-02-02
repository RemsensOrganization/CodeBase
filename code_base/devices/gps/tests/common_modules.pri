QT += core testlib serialport

CONFIG += console testcase c++11

TEMPLATE = app

include(../pathes.pri)
include($$SRC_DIR/gps.pri)