QT += serialport

include(devices_ports_explorer/gps_port_autodetector.pri)
include(port_reader/gps_receiver.pri)
include(json_file_creator/json_file_creator.pri)
include(data/gps_data.pri)
include(data_parser/gps_parser.pri)

CONTROLLER_DIR = $$PWD

INCLUDEPATH += $$CONTROLLER_DIR
SOURCES += $$CONTROLLER_DIR/controller.cpp
HEADERS += $$CONTROLLER_DIR/controller.h
