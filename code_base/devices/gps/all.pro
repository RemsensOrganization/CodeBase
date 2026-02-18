TEMPLATE = subdirs

CONFIG += ordered

include(src/gps.pri)

<<<<<<< simple_use
SUBDIRS += examples/gps_device/gps_device_example.pro\
=======
SUBDIRS += examples/controller/controller.pro\
           examples/minimalistic\
>>>>>>> main

SUBDIRS += tests/tests.pro

