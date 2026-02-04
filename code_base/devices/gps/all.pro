TEMPLATE = subdirs

CONFIG += ordered

include(src/gps.pri)

SUBDIRS += examples/controller/controller.pro\
           examples/minimalistic\

SUBDIRS += tests/tests.pro

