#ifndef DEVICES_GPS_TESTS_PARSER_TEST_PARSER_TESTS_H_
#define DEVICES_GPS_TESTS_PARSER_TEST_PARSER_TESTS_H_

#include <QObject>
#include <QtTest>

class TestsParser : public QObject {
    Q_OBJECT

public:
    TestsParser();

private slots:
    void
    initTestCase();  // Вызывается перед выполнением первого тестового метода
    void cleanupTestCase();  // Вызывается после выполнения последнего тестового
                             // метода
    void init();  // Вызывается перед каждым тестовым методом
    void cleanup();  // Вызывается после каждого тестового метода

    void test_gga_rmc_pair();
    void test_broken_lines();
};

#endif  // DEVICES_GPS_TESTS_PARSER_TEST_PARSER_TESTS_H_
