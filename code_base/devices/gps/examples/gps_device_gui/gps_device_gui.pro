TARGET = gps_device_gui_example

TEMPLATE = app

CONFIG += c++11
QT += gui widgets

include(../root.pri) # для сборки в отдельную папку в корне репозитория
include(../../src/solutions/gps_device/gps_device.pri)

SOURCES += $$PWD/main.cpp \
    ../../src/solutions/gps_device/gps_simple_widget.cpp \
    mainwindow.cpp

FORMS += \
    mainwindow.ui

HEADERS += \
    ../../src/solutions/gps_device/gps_simple_widget.h \
    mainwindow.h


