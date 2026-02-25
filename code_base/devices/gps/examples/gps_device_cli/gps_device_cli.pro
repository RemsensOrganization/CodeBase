TARGET = gps_device_cli_example

TEMPLATE = app

CONFIG += c++11 console

include(../root.pri) # для сборки в отдельную папку в корне репозитория
include(../../src/solutions/gps_device/gps_device.pri)
include(../../src/gps_cli_parser/gps_cli_parser.pri)

SOURCES += $$PWD/main.cpp


