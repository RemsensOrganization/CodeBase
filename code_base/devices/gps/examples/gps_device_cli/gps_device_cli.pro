TARGET = gps_cli

TEMPLATE = app
CONFIG += gps_console_build
CONFIG += c++11 console
DEFINES += GPS_CONSOLE_BUILD QT_NO_WIDGETS
QT -= all
QT -= gui widgets coregui

include(../../src/solutions/gps_device/gps_device.pri)
include(../../src/gps_cli_parser/gps_cli_parser.pri)

SOURCES += $$PWD/main.cpp


