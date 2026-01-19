QT += serialport

PORT_READER_DIR = $$PWD

INCLUDEPATH += $$PORT_READER_DIR

SOURCES += \
$$PORT_READER_DIR/gps_receiver.cpp\
    $$PWD/file_data_writer.cpp

HEADERS += \
$$PORT_READER_DIR/gps_receiver.h
