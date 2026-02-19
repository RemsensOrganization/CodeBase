TARGET = gps_device_nogui_example

TEMPLATE = app

CONFIG += c++11

include(../root.pri) # для сборки в отдельную папку в корне репозитория
include(../../src/solutions/gps_device/gps_device.pri)

SOURCES += $$PWD/main.cpp


