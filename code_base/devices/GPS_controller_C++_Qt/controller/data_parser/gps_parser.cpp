#include <QDebug>

#include "controller/data_parser/gps_parser.h"

GPSParser::GPSParser(QObject *parent) : QObject(parent) {}

void GPSParser::parseLine(const QString &line) {
    if (line.startsWith("$GPGGA")) {
        parseGpgga(line);
    }
    else if (line.startsWith("$GPRMC")) {
        parseGprmc(line);
    }
    if (gotGGA && gotRMC) {
        emit gpsUpdated(latest);
        gotGGA = false;
        gotRMC = false;
    }
}

double GPSParser::convertCoord(const QString &coord, const QString &dir) {
    if (coord.isEmpty() || (dir != "N" && dir != "S" && dir != "E" && dir != "W"))
        return 0.0;

    int degLen = (dir == "N" || dir == "S") ? 2 : 3;
    int deg = coord.left(degLen).toInt();
    double min = coord.mid(degLen).toDouble();
    double decimal = deg + min / 60.0;
    if (dir == "S" || dir == "W")
        decimal *= -1;
    return decimal;
}

void GPSParser::parseGpgga(const QString &line) {
    QStringList parts = line.split(",");
    if (parts.size() < 10)
        return;

    latest.altitude = parts[9].toDouble();
    latest.timeUtc = parts[1];
    latest.valid = (parts[6] != "0");
    latest.satellites = parts[7].toInt();
    gotGGA = true;
}

void GPSParser::parseGprmc(const QString &line) {
    QStringList parts = line.split(",");
    if (parts.size() < 9)
        return;

    if (parts[2] == "A") {
        latest.latitude = convertCoord(parts[3], parts[4]);
        latest.longitude = convertCoord(parts[5], parts[6]);
        latest.speedKmh = parts[7].toDouble() * 1.852;
        latest.course = parts[8].toDouble();
        latest.date = parts[9];
    }
    gotRMC = true;

}

