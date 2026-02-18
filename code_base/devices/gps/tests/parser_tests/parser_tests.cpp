#include "parser_tests.h"

#include <QDebug>
<<<<<<< simple_use
#include <QSignalSpy>
=======
>>>>>>> main
#include <QtTest>

#include "gps_parser.h"

namespace mock {

const char GGA_VALID[] =
<<<<<<< simple_use
    "$GPGGA,123519,53.23,N,01131.000,E,1,08,0.9,545.4,M,,,*47\r\n";
=======
    "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,,,*47\r\n";
>>>>>>> main
const char RMC_VALID[] =
    "$GPRMC,123519,A,53.23,N,01131.000,E,10.0,90.0,010203,,,A*6C\r\n";

const char GGA_BROKEN[] = "$GPGGA,123519,4807.038,N*47\r\n";
const char RMC_BROKEN[] = "$GPRMC,123519,A,4807.038,N*6C\r\n";

const char RMC_UNREAL[] =
<<<<<<< simple_use
    "$GPRMC,250000.000,A,9u99.999,N,19999.999,E,9999.9,999.9,991299,,,o*7C\r\n";
=======
    "$GPRMC,250000.000,V,9u99.999,N,19999.999,E,9999.9,999.9,991299,,,o*7C\r\n";
>>>>>>> main
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
<<<<<<< simple_use
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
=======
    QByteArray GGA_VALID(mock::GGA_VALID);
    QByteArray RMC_VALID(mock::RMC_VALID);
    GPSParser parser;
    parser.parseLine(QString::fromLatin1(GGA_VALID));
    parser.parseLine(QString::fromLatin1(RMC_VALID));

    QVERIFY(parser.data.isGpsDataValid);
    QCOMPARE(parser.data.satellites, 8);
    QCOMPARE(parser.data.altitude, 545.4);
    QCOMPARE_FLOATS(parser.data.latitude, 53.00);
    QCOMPARE_FLOATS(parser.data.longitude, 11.52);
    QCOMPARE_FLOATS(parser.data.speedKmh, 18.52);
    QCOMPARE_FLOATS(parser.data.course, 90.0);
    QCOMPARE(parser.data.timeUtc, QStringLiteral("123519"));
    QCOMPARE(parser.data.date, QStringLiteral("010203"));
    showParsingErrors(parser.data.errors);
}

void TestsParser::test_broken_lines() {
    GPSParser parser;
>>>>>>> main
    QByteArray brokenGga(mock::GGA_BROKEN);
    QByteArray brokenRmc(mock::RMC_BROKEN);
    parser.parseLine(QString::fromLatin1(brokenGga));
    parser.parseLine(QString::fromLatin1(brokenRmc));
<<<<<<< simple_use
    QVERIFY(spy.count() == 0);
=======

    QVERIFY(parser.data.timeUtc.isEmpty());
    QCOMPARE(parser.data.altitude, 0.0);
    QCOMPARE(parser.data.satellites, 0);
    QCOMPARE(parser.data.latitude, 0.0);
    QCOMPARE(parser.data.longitude, 0.0);
    QCOMPARE(parser.data.speedKmh, 0.0);
    QCOMPARE(parser.data.course, 0.0);
    QVERIFY(parser.data.date.isEmpty());
>>>>>>> main
    showParsingErrors(parser.data.errors);
}

void TestsParser::test_unreal_values() {
<<<<<<< simple_use
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
    qDebug() << gpsData.latitude;
    qDebug() << gpsData.longitude;
    qDebug() << gpsData.speedKmh;
    qDebug() << gpsData.course;
    qDebug() << gpsData.timeUtc;
    qDebug() << gpsData.date;
=======
    QByteArray GGA_VALID(mock::GGA_UNREAL);
    QByteArray RMC_VALID(mock::RMC_UNREAL);
    GPSParser parser;
    parser.parseLine(QString::fromLatin1(GGA_VALID));
    parser.parseLine(QString::fromLatin1(RMC_VALID));
    Q_UNUSED(parser.data.isGpsDataValid;)
    Q_UNUSED(parser.data.satellites;)
    Q_UNUSED(parser.data.altitude;)
    Q_UNUSED(parser.data.latitude;)
    Q_UNUSED(parser.data.longitude;)
    Q_UNUSED(parser.data.speedKmh;)
    Q_UNUSED(parser.data.course;)
    Q_UNUSED(parser.data.timeUtc;)
    Q_UNUSED(parser.data.date;)
>>>>>>> main
    showParsingErrors(parser.data.errors);
}

QTEST_MAIN(TestsParser)
