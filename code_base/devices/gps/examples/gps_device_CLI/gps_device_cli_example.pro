TARGET = gps_device_cli_example

include(../root.pri)
TEMPLATE = app

CONFIG += c++11

include(../../src/solutions/gps_device/gps_device.pri)

SOURCES += $$PWD/main.cpp


