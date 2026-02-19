TEMPLATE = subdirs

CONFIG += ordered

include(src/gps.pri)
include(../root.pri) # для сборки в отдельную папку в корне репозитория


SUBDIRS += examples/gps_device_gui\
           examples/gps_device_no_gui\
           examples/gps_device_cli\
           tests/tests.pro\

