TEMPLATE = subdirs

BUILD_DIR = ../../../../builds/$$TARGET  # или абсолютный путь, напр. /home/user/mybuilds

CONFIG(debug, debug|release) {
    BUILD_SUBDIR = debug
} else {
    BUILD_SUBDIR = release
}

OBJECTS_DIR = $$BUILD_DIR/$$BUILD_SUBDIR/obj
MOC_DIR     = $$BUILD_DIR/$$BUILD_SUBDIR/moc
UI_DIR      = $$BUILD_DIR/$$BUILD_SUBDIR/ui
RCC_DIR     = $$BUILD_DIR/$$BUILD_SUBDIR/rcc
DESTDIR     = $$BUILD_DIR/$$BUILD_SUBDIR/bin




CONFIG += ordered

include(src/gps.pri)

SUBDIRS += examples/gps_device/gps_device_example.pro\

SUBDIRS += tests/tests.pro

