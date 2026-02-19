QT += core testlib serialport

CONFIG += console testcase c++11

TEMPLATE = app


include($$SRC_DIR/gps.pri)

RESOURCES += $$PWD/test_res.qrc\
