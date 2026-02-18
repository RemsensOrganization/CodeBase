TEMPLATE = subdirs

CONFIG += ordered

include(src/gps.pri)


SUBDIRS += examples/gps_device_gui\
           examples/gps_device_no_gui\
           examples/gps_device_cli\
           tests/tests.pro\

