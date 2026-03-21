#include "test_template_code_text.h"

// R"testMakerDlmtr()testMakerDlmtr"

const char kProTestText[] = R"testMakerDlmtr(
QT += testlib core

TARGET = %1

HEADERS += %2\

SOURCES += %3\

)testMakerDlmtr";

const char kHeaderTestText[] = R"testMakerDlmtr(
#ifndef %1
#define %1

#include <QObject>
#include <QtTest>

class %2 : public QObject {
    Q_OBJECT

public:
    %2();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
};

#endif  // %1
)testMakerDlmtr";

const char kCppTestText[] = R"testMakerDlmtr(
#include "%1"

#include <QDebug>

namespace {}  // end namespace

%2::%2() {}

void %2::initTestCase() {
    // Инициализация перед запуском всех тестов
}

void %2::cleanupTestCase() {
    // Очистка после выполнения всех тестов
}

void %2::init() {
    // Инициализация перед каждым тестом
}

void %2::cleanup() {
    // Очистка после каждого теста
}

QTEST_MAIN(%2)
)testMakerDlmtr";
