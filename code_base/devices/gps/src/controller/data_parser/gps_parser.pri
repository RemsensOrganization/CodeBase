QT += serialport

DATA_PARSER_DIR = $$PWD

INCLUDEPATH += $$DATA_PARSER_DIR

SOURCES += \
$$DATA_PARSER_DIR/gps_parser.cpp\

HEADERS += \
$$DATA_PARSER_DIR/gps_parser.h
