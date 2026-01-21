#ifndef UNITTESTS_H
#define UNITTESTS_H

#include <QObject>
#include <QtTest>

class UnitTests : public QObject {
    Q_OBJECT

public:
    UnitTests();

private slots:
    void
    initTestCase();  // Вызывается перед выполнением первого тестового метода
    void cleanupTestCase();  // Вызывается после выполнения последнего тестового
                             // метода
    void init();  // Вызывается перед каждым тестовым методом
    void cleanup();  // Вызывается после каждого тестового метода

    void test_gga_only();
    void test_rmc_only();
    void test_gga_rmc_pair();
    void test_broken_lines();
};

#endif  // UNITTESTS_H
