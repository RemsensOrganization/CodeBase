QT += serialport core

include(../gps_device_status/gps_device_status.pri)

PORT_READER_DIR = $$PWD

INCLUDEPATH += $$PORT_READER_DIR

SOURCES += \
$$PORT_READER_DIR/gps_receiver.cpp \
$$PWD/gps_logger.cpp

HEADERS += \
$$PORT_READER_DIR/gps_receiver.h\
$$PWD/gps_logger.h
