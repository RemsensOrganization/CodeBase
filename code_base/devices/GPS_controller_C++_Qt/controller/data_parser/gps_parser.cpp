#include "data_parser/gps_parser.h"

#include <QDebug>

constexpr int kMaxGgaNumberOfParts = 14;
constexpr int kMaxRmcNumberOfParts = 13;
constexpr int kGgaTimeUtcPartIndex = 1;
constexpr int kRmcStatusPartIndex = 2;
constexpr int kRmcLatitudeFieldPartIndex = 3;
constexpr int kRmcLatitudeFieldPartDirection = 4;
constexpr int kRmcLongitudeFieldPartIndex = 5;
constexpr int kRmcLongitudeFieldPartDirection = 6;
constexpr int kGgaValidGpsFlagPartIndex = 6;
constexpr int kGgaSatellitesPartIndex = 7;
constexpr int kRmcSpeedPartIndex = 7;
constexpr int kRmcCoursePartIndex = 8;
constexpr int kRmcDatePartIndex = 9;
constexpr int kGgaAltitudeFieldPartIndex = 9;
constexpr double kMileToKmConversion = 1.852;

namespace {
bool isLatitudeValid(const double latitude) {
    return (latitude >= -90.0 || latitude <= 90.0);
};
bool isLongitudeValid(const double longitude) {
    return (longitude >= -180.0 || longitude <= 180.0);
};
bool isSatellitesNumberValid(const int satellites) {
    return (satellites > 0 || satellites <= 32);
};
bool isSpeedValueValid(const double speedKmh) { return (speedKmh >= 0.0); }
}  // end namespace

GPSParser::GPSParser(QObject *parent) : QObject(parent) {}

void GPSParser::parseLine(const QString &line) {
    if (line.startsWith("$GPGGA")) {
        parseGpgga(line);
    } else if (line.startsWith("$GPRMC")) {
        parseGprmc(line);
    }
    if (gotGGA && gotRMC) {
        emit gpsUpdated(data);
        gotGGA = false;
        gotRMC = false;
    }
}

double GPSParser::convertCoord(const QString &coord, const QString &dir) {
    if (coord.isEmpty() ||
        (dir != "N" && dir != "S" && dir != "E" && dir != "W"))
        return 0.0;

    int degLen = (dir == "N" || dir == "S") ? 2 : 3;
    int deg = coord.left(degLen).toInt();
    double min = coord.mid(degLen).toDouble();
    double decimal = deg + min / 60.0;
    if (dir == "S" || dir == "W") decimal *= -1;
    return decimal;
}

void GPSParser::parseGpgga(const QString &line) {
    QStringList parts = line.split(",");
    if (parts.size() < kMaxGgaNumberOfParts) return;

    data.altitude = parts[kGgaAltitudeFieldPartIndex].toDouble();
    data.timeUtc = parts[kGgaTimeUtcPartIndex];
    data.valid_gps_flag = (parts[kGgaValidGpsFlagPartIndex] != "0");
    data.satellites = parts[kGgaSatellitesPartIndex].toInt();
    if (!isSatellitesNumberValid(data.satellites)) {
        qWarning() << "Невалидные данные";
        return;
    }

    gotGGA = true;
}

void GPSParser::parseGprmc(const QString &line) {
    QStringList parts = line.split(",");
    if (parts.size() < kMaxRmcNumberOfParts) return;

    if (parts[2] == "A") {
        data.latitude = convertCoord(parts[kRmcLatitudeFieldPartIndex],
                                     parts[kRmcLatitudeFieldPartDirection]);
        if (!isLatitudeValid(data.latitude)) {
            qWarning() << "Невалидные данные";
            return;
        }
        data.longitude = convertCoord(parts[kRmcLongitudeFieldPartIndex],
                                      parts[kRmcLongitudeFieldPartDirection]);
        if (!isLongitudeValid(data.longitude)) {
            qWarning() << "Невалидные данные";
            return;
        }
        data.speedKmh =
            parts[kRmcSpeedPartIndex].toDouble() * kMileToKmConversion;
        if (!isSpeedValueValid(data.speedKmh)) {
            qWarning() << "Невалидные данные";
            return;
        }
        data.course = parts[kRmcCoursePartIndex].toDouble();
        data.date = parts[kRmcDatePartIndex];
    }
    gotRMC = true;
}
