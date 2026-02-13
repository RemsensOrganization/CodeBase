#include "parser_tests.h"

#include <QDebug>
#include <QSignalSpy>
#include <QtTest>

#include "gps_parser.h"

namespace mock {

const char GGA_VALID[] =
    "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,,,*47\r\n";
const char RMC_VALID[] =
    "$GPRMC,123519,A,53.23,N,01131.000,E,10.0,90.0,010203,,,A*6C\r\n";

const char GGA_BROKEN[] = "$GPGGA,123519,4807.038,N*47\r\n";
const char RMC_BROKEN[] = "$GPRMC,123519,A,4807.038,N*6C\r\n";

const char RMC_UNREAL[] =
    "$GPRMC,250000.000,A,9u99.999,N,19999.999,E,9999.9,999.9,991299,,,o*7C\r\n";
const char GGA_UNREAL[] =
    "$GPGGA,250000.000,9999.999,N,19999.999,E,8,99,99.9,99999.9,M,9999.9,,*"
    "4A\r\n";

}  // namespace mock

namespace {
void QCOMPARE_FLOATS(double actual, double expected, double epsilon = 0.01) {
    QVERIFY2(qAbs(actual - expected) < epsilon,
             qPrintable(
                 QString("Expected %1 but got %2").arg(expected).arg(actual)));
}

void showParsingErrors(const QStringList &errorList) {
    for (const auto &error : errorList) {
        qWarning() << error;
    }
}

}  // namespace

TestsParser::TestsParser() {}

void TestsParser::test_gga_rmc_pair() {
    qRegisterMetaType<GpsData>("GpsData");
    QByteArray GGA_VALID(mock::GGA_VALID);
    QByteArray RMC_VALID(mock::RMC_VALID);
    GPSParser parser;
    QSignalSpy spy(&parser, &GPSParser::gpsUpdated);
    parser.parseLine(QString::fromLatin1(GGA_VALID));
    parser.parseLine(QString::fromLatin1(RMC_VALID));

    QList<QVariant> args = spy.takeFirst();
    QVERIFY(!args.empty());
    GpsData gpsData = args.at(0).value<GpsData>();
    QVERIFY(gpsData.isGpsDataValid);
    QCOMPARE(gpsData.satellites, 8);
    QCOMPARE(gpsData.altitude, 545.4);
    QCOMPARE_FLOATS(gpsData.latitude, 53.00);
    QCOMPARE_FLOATS(gpsData.longitude, 11.52);
    QCOMPARE_FLOATS(gpsData.speedKmh, 18.52);
    QCOMPARE_FLOATS(gpsData.course, 90.0);
    QCOMPARE(gpsData.timeUtc, QStringLiteral("123519"));
    QCOMPARE(gpsData.date, QStringLiteral("010203"));
    showParsingErrors(gpsData.errors);
}

void TestsParser::test_broken_lines() {
    qRegisterMetaType<GpsData>("GpsData");
    GPSParser parser;
    QSignalSpy spy(&parser, &GPSParser::gpsUpdated);
    QByteArray brokenGga(mock::GGA_BROKEN);
    QByteArray brokenRmc(mock::RMC_BROKEN);
    parser.parseLine(QString::fromLatin1(brokenGga));
    parser.parseLine(QString::fromLatin1(brokenRmc));
    QVERIFY(spy.count() == 0);
    showParsingErrors(parser.data.errors);
}

void TestsParser::test_unreal_values() {
    qRegisterMetaType<GpsData>("GpsData");
    QByteArray GGA_VALID(mock::GGA_UNREAL);
    QByteArray RMC_VALID(mock::RMC_UNREAL);
    GPSParser parser;
    QSignalSpy spy(&parser, &GPSParser::gpsUpdated);
    parser.parseLine(QString::fromLatin1(GGA_VALID));
    parser.parseLine(QString::fromLatin1(RMC_VALID));
    QList<QVariant> args = spy.takeFirst();
    QVERIFY(!args.empty());
    GpsData gpsData = args.at(0).value<GpsData>();
    QVERIFY(gpsData.isGpsDataValid);
    qDebug() << "unreal satellites: " << gpsData.satellites;
    QCOMPARE(gpsData.satellites, 99);
    qDebug() << gpsData.altitude;
    Q_UNUSED(gpsData.altitude;)
    qDebug() << gpsData.latitude;
    Q_UNUSED(gpsData.latitude;)
    qDebug() << gpsData.longitude;
    Q_UNUSED(gpsData.longitude;)
    qDebug() << gpsData.speedKmh;
    Q_UNUSED(gpsData.speedKmh;)
    qDebug() << gpsData.course;
    Q_UNUSED(gpsData.course;)
    qDebug() << gpsData.timeUtc;
    Q_UNUSED(gpsData.timeUtc;)
    qDebug() << gpsData.date;
    Q_UNUSED(gpsData.date;)
    showParsingErrors(parser.data.errors);
}

QTEST_MAIN(TestsParser)
