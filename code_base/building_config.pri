BUILD_DIR = $$ROOT/../builds/$$TARGET

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

CONFIG -= debug_and_release  # отключить dual‑config в Makefile
CONFIG += separate_debug_info # опционально, для debug‑символов
