TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += src/src.pro
SUBDIRS += tests/parser_test/parser_tests.pro
SUBDIRS += examples/example.pro

tests.depends = src
example.depends = src
