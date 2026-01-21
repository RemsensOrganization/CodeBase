#include "parser_tests.h"

#include <QDebug>
#include <QSignalSpy>
#include <QtTest>

#include "data/gps_data.h"
#include "data_parser/gps_parser.h"

namespace {}  // end namespace

UnitTests::UnitTests() {}

void UnitTests::initTestCase() {
    // Инициализация перед запуском всех тестов
    qRegisterMetaType<GpsData>("GpsData");
}

void UnitTests::cleanupTestCase() {
    // Очистка после выполнения всех тестов
}

void UnitTests::init() {
    // Инициализация перед каждым тестом
}

void UnitTests::cleanup() {
    // Очистка после каждого теста
}

static const QByteArray GGA_VALID =
    "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,,,*47\r\n";

static const QByteArray RMC_VALID =
    "$GPRMC,123519,A,4807.038,N,01131.000,E,10.0,90.0,010203,,,A*6C\r\n";

void UnitTests::test_gga_only() {
    GPSParser parser;
    QSignalSpy spy(&parser, SIGNAL(gpsUpdated(GpsData)));

    parser.parseLine(QString::fromLatin1(GGA_VALID));

    QCOMPARE(spy.count(), 0);
}

void UnitTests::test_rmc_only() {
    GPSParser parser;
    QSignalSpy spy(&parser, SIGNAL(gpsUpdated(GpsData)));

    parser.parseLine(QString::fromLatin1(RMC_VALID));

    QCOMPARE(spy.count(), 0);
}

void UnitTests::test_gga_rmc_pair() {
    GPSParser parser;
    QSignalSpy spy(&parser, SIGNAL(gpsUpdated(GpsData)));

    parser.parseLine(QString::fromLatin1(GGA_VALID));
    parser.parseLine(QString::fromLatin1(RMC_VALID));

    QCOMPARE(spy.count(), 1);

    const QList<QVariant> args = spy.takeFirst();
    QVERIFY(args.size() == 1);
    GpsData data = qvariant_cast<GpsData>(args.at(0));

    QVERIFY(data.valid);
    QCOMPARE(data.satellites, 8);
    QCOMPARE(data.altitude, 545.4);
    QVERIFY(data.latitude > 48.0 && data.latitude < 48.2);
    QVERIFY(data.longitude > 11.0 && data.longitude < 11.6);
    QVERIFY(data.speedKmh > 18.0 && data.speedKmh < 19.0);
    QCOMPARE(data.course, 90.0);
    QCOMPARE(data.timeUtc, QStringLiteral("123519"));
    QCOMPARE(data.date, QStringLiteral("010203"));
}

void UnitTests::test_broken_lines() {
    GPSParser parser;
    QSignalSpy spy(&parser, SIGNAL(gpsUpdated(GpsData)));

    QByteArray brokenGga = "$GPGGA,123519,4807.038,N*47\r\n";

    QByteArray brokenRmc = "$GPRMC,123519,A,4807.038,N*6C\r\n";

    parser.parseLine(QString::fromLatin1(brokenGga));
    parser.parseLine(QString::fromLatin1(brokenRmc));

    QCOMPARE(spy.count(), 0);
}

QTEST_MAIN(UnitTests)
