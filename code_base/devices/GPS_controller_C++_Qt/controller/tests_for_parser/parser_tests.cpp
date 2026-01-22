#include "parser_tests.h"

#include <QDebug>
#include <QSignalSpy>
#include <QtTest>

#include "data/gps_data.h"
#include "data_parser/gps_parser.h"

namespace {}  // end namespace

TestsParser::TestsParser() {}

void TestsParser::initTestCase() {
    // Инициализация перед запуском всех тестов
    qRegisterMetaType<GpsData>("GpsData");
}

void TestsParser::cleanupTestCase() {
    // Очистка после выполнения всех тестов
}

void TestsParser::init() {
    // Инициализация перед каждым тестом
}

void TestsParser::cleanup() {
    // Очистка после каждого теста
}

static const QByteArray GGA_VALID =
    "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,,,*47\r\n";

static const QByteArray RMC_VALID =
    "$GPRMC,123519,A,53.23,N,01131.000,E,10.0,90.0,010203,,,A*6C\r\n";

void TestsParser::test_gga_rmc_pair() {
    GPSParser parser;
    QSignalSpy spy(&parser, SIGNAL(gpsUpdated(GpsData)));

    parser.parseLine(QString::fromLatin1(GGA_VALID));
    parser.parseLine(QString::fromLatin1(RMC_VALID));

    QVERIFY(parser.data.valid_gps_flag);
    QCOMPARE(parser.data.satellites, 8);
    QCOMPARE(parser.data.altitude, 545.4);
    QVERIFY(parser.data.latitude > 53.0 && parser.data.latitude < 53.1);
    QVERIFY(parser.data.longitude > 11.0 && parser.data.longitude < 11.6);
    QVERIFY(parser.data.speedKmh > 18.0 && parser.data.speedKmh < 19.0);
    QCOMPARE(parser.data.course, 90.0);
    QCOMPARE(parser.data.timeUtc, QStringLiteral("123519"));
    QCOMPARE(parser.data.date, QStringLiteral("010203"));
}

void TestsParser::test_broken_lines() {
    GPSParser parser;

    QByteArray brokenGga = "$GPGGA,123519,4807.038,N*47\r\n";
    QByteArray brokenRmc = "$GPRMC,123519,A,4807.038,N*6C\r\n";

    parser.parseLine(QString::fromLatin1(brokenGga));
    parser.parseLine(QString::fromLatin1(brokenRmc));

    QVERIFY(parser.data.timeUtc.isEmpty());
    QCOMPARE(parser.data.altitude, 0.0);
    QCOMPARE(parser.data.satellites, 0);
    QCOMPARE(parser.data.latitude, 0.0);
    QCOMPARE(parser.data.longitude, 0.0);
    QCOMPARE(parser.data.speedKmh, 0.0);
    QCOMPARE(parser.data.course, 0.0);
    QVERIFY(parser.data.date.isEmpty());
}

QTEST_MAIN(TestsParser)
