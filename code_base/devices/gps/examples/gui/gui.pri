QT += core widgets

include(../../pathes.pri)


HEADERS +=\
           $$PWD/gps_widget.h \
           $$SRC_DIR/data/gps_data.h

SOURCES +=\
    $$PWD/gps_widget.cpp

INCLUDEPATH += $$PWD
