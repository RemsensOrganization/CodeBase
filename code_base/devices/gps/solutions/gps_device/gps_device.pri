QT += core gui widgets serialport concurrent

include(../../pathes.pri)
include($$SRC_DIR/gps.pri)

# Исходники
SOURCES += \
    $$PWD/gps_device.cpp \
    $$PWD/gps_widget.cpp

HEADERS += \
    $$PWD/gps_device.h \
    $$PWD/gps_widget.h

# Зависимости от базовых GPS классов (из ../../src)
INCLUDEPATH += \
$$SRC_DIR \
$$SOLUTIONS_DIR/gps_device
