TARGET = gps_device_cli_example

TEMPLATE = app
CONFIG += gps_console_build
CONFIG += c++11 console
DEFINES += GPS_CONSOLE_BUILD QT_NO_WIDGETS
QT -= all
QT -= gui widgets coregui

include(../root.pri) # для сборки в отдельную папку в корне репозитория
include(../../src/solutions/gps_device/gps_device.pri)
include(../../src/gps_cli_parser/gps_cli_parser.pri)

SOURCES += $$PWD/main.cpp


