#include "parser_tests.h"

#include <QDebug>
#include <QtTest>

#include "gps_parser.h"

namespace mock {

const char GGA_VALID[] =
    "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,,,*47\r\n";
const char RMC_VALID[] =
    "$GPRMC,123519,A,53.23,N,01131.000,E,10.0,90.0,010203,,,A*6C\r\n";

const char GGA_BROKEN[] = "$GPGGA,123519,4807.038,N*47\r\n";
const char RMC_BROKEN[] = "$GPRMC,123519,A,4807.038,N*6C\r\n";

}  // namespace mock

void QCOMPARE_FLOATS(double actual, double expected, double epsilon = 0.01) {
    QVERIFY2(qAbs(actual - expected) < epsilon,
             qPrintable(
                 QString("Expected %1 but got %2").arg(expected).arg(actual)));

}  // namespace

TestsParser::TestsParser() {}

void TestsParser::test_gga_rmc_pair() {
    QByteArray GGA_VALID(mock::GGA_VALID);
    QByteArray RMC_VALID(mock::RMC_VALID);
    GPSParser parser;
    parser.parseLine(QString::fromLatin1(GGA_VALID));
    parser.parseLine(QString::fromLatin1(RMC_VALID));

    QVERIFY(parser.data.valid_gps_flag);
    QCOMPARE(parser.data.satellites, 8);
    QCOMPARE(parser.data.altitude, 545.4);
    qDebug() << parser.data.latitude;
    QCOMPARE_FLOATS(parser.data.latitude, 53.00);
    QCOMPARE_FLOATS(parser.data.longitude, 11.52);
    qDebug() << parser.data.speedKmh;
    QCOMPARE_FLOATS(parser.data.speedKmh, 18.52);
    QCOMPARE_FLOATS(parser.data.course, 90.0);
    QCOMPARE(parser.data.timeUtc, QStringLiteral("123519"));
    QCOMPARE(parser.data.date, QStringLiteral("010203"));
}

void TestsParser::test_broken_lines() {
    GPSParser parser;
    QByteArray brokenGga(mock::GGA_BROKEN);
    QByteArray brokenRmc(mock::RMC_BROKEN);
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
