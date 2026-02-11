QT += widgets

TEMPLATE = app
TARGET = GpsWidget

include(../../src/gps.pri)

HEADERS += $$PWD/gps_widget.h

SOURCES += $$PWD/gps_widget.cpp\
            main.cpp
