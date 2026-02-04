#include "gps_parser.h"

#include <QDebug>

#include "qdatetime.h"

constexpr double kMinLongitudeValue = -180;
constexpr double kMaxLongitudeValue = 180;

constexpr double kMinLatitudeValue = -90;
constexpr double kMaxLatitudeValue = 90;

constexpr int kGgaTimeUtcPartIndex = 1;
constexpr int kGgaValidGpsFlagPartIndex = 6;
constexpr int kGgaSatellitesPartIndex = 7;
constexpr int kGgaAltitudeFieldPartIndex = 9;

constexpr int kRmcStatusPartIndex = 2;
constexpr int kRmcLatitudeFieldPartIndex = 3;
constexpr int kRmcLatitudeFieldPartDirection = 4;
constexpr int kRmcLongitudeFieldPartIndex = 5;
constexpr int kRmcLongitudeFieldPartDirection = 6;
constexpr int kRmcSpeedPartIndex = 7;
constexpr int kRmcCoursePartIndex = 8;
constexpr int kRmcDatePartIndex = 9;

constexpr int kRmcMinNumberOfParts = 13;
constexpr int kGgaMinNumberOfParts = 14;
constexpr int kMaxNumberSatellites = 32;

constexpr double kMileToKmConversion = 1.852;
constexpr double kVelocityMaxLimit = 1850;   // km per hour
constexpr double kAltitudeMaxLimit = 18000;  // meters
constexpr double kAltitudeMinLimit = -500;

constexpr char kGpsMsgGpsFlagIsInvalid[] = "GPS flag is invalid";
constexpr char kGpsMsgRmcFlagIsInvalid[] = "RMC flag is invalid";
constexpr char kGpsMsgLatitudeIsOutOfRange[] = "Latitude is out of range";
constexpr char kGpsMsgLongitudeIsOutOfRange[] = "Longitude is out of range";
constexpr char kGpsMsgAltitudeIsOutOfRange[] = "Altitude is out of range";
constexpr char kGpsMsgVelocityIsOutOfRange[] = "Velocity is out of range";
constexpr char kGpsMsgNumberOfSattelitesIsOutOfRange[] =
    "Number of satellites is out of range";
constexpr char kGpsMsgWrongRmcNumberOfParts[] = "Rmc number of parts is wrong";
constexpr char kGpsMsgWrongGgaNumberOfParts[] = "Gga number of parts is wrong";
constexpr char kGpsMsgNoDirectionSymbolError[] = "Missing direction indicator";
constexpr char kGpsMsgConvertToDegreesError[] = "Failed to convert to degrees";
constexpr char kGpsMsgCourseIsOutOfRange[] = "Course is out of range";
constexpr char kGpsMsgDateIsInvalid[] = "Date is invalid";

namespace {

double toDegrees(const QString &coord, const QString &dir) {
    if (coord.isEmpty() ||
        (dir != "N" && dir != "S" && dir != "E" && dir != "W")) {
        qWarning() << QString(kGpsMsgNoDirectionSymbolError);
        return 0.0;
    }

    int degLen = (dir == "N" || dir == "S") ? 2 : 3;
    bool isOk;
    int deg = coord.leftRef(degLen).toInt(&isOk);
    double min = coord.midRef(degLen).toDouble(&isOk);
    if (!isOk) qWarning() << QString(kGpsMsgConvertToDegreesError);
    double decimal = deg + min / 60.0;
    if (dir == "S" || dir == "W") decimal *= -1;
    return decimal;
}

bool isLatitudeValid(const double latitude) {
    bool result =
        (latitude >= kMinLatitudeValue && latitude <= kMaxLatitudeValue);
    if (!result) qWarning() << kGpsMsgLatitudeIsOutOfRange;
    return result;
};

bool isLongitudeValid(const double longitude) {
    bool result =
        (longitude >= kMinLongitudeValue && longitude <= kMaxLongitudeValue);
    if (!result) qWarning() << kGpsMsgLongitudeIsOutOfRange;
    return result;
};

bool isSatellitesNumberValid(const int satellites) {
    bool result = (satellites >= 0 && satellites <= kMaxNumberSatellites);
    if (!result) qWarning() << kGpsMsgNumberOfSattelitesIsOutOfRange;
    return result;
};

bool isSpeedValueValid(const double speedKmh) {
    bool result = (speedKmh >= 0.0 && speedKmh < kVelocityMaxLimit);
    if (!result) qWarning() << kGpsMsgVelocityIsOutOfRange;
    return result;
}

bool isAltitudeRangeValid(const double altitude) {
    bool result =
        (altitude > kAltitudeMinLimit && altitude < kAltitudeMaxLimit);
    if (!result) qWarning() << kGpsMsgAltitudeIsOutOfRange;
    return result;
}

bool isGgaNumberValid(const int number) {
    bool result = (number >= kGgaMinNumberOfParts);
    if (!result)
        qWarning() << kGpsMsgWrongGgaNumberOfParts << number
                   << "must be more or equal: " << kGgaMinNumberOfParts;
    return result;
}

bool isRmcNumberValid(const int number) {
    bool result = (number >= kRmcMinNumberOfParts);
    if (!result)
        qWarning() << kGpsMsgWrongRmcNumberOfParts << number
                   << "must be more or equal: " << kRmcMinNumberOfParts;
    ;
    return result;
}

bool isGpsFlagValid(const QString &flag) {
    bool result = flag != "0";
    if (!result) qWarning() << kGpsMsgGpsFlagIsInvalid;
    return result;
};

bool isRmcStatusValid(const QString &flag) {
    bool result = (flag == "A");
    if (!result) qWarning() << kGpsMsgRmcFlagIsInvalid;
    return result;
}

bool isCourseValid(const double course) {
    bool result = (course >= 0.0 && course <= 360.0);
    if (!result) qWarning() << kGpsMsgCourseIsOutOfRange;
    return result;
}

bool isDateValid(const QString &date) {
    if (date.size() != 6 || !date.toUInt()) {
        qWarning() << kGpsMsgDateIsInvalid << "(bad format)";
        return false;
    }

    bool okDay, okMonth, okYear;
    int day = date.midRef(0, 2).toInt(&okDay);
    int month = date.midRef(2, 2).toInt(&okMonth);
    int year = date.midRef(4, 2).toInt(&okYear);

    if (!okDay || !okMonth || !okYear) {
        qWarning() << kGpsMsgDateIsInvalid << "(parse error)";
        return false;
    }

    year += 2000;

    QDate qdate(year, month, day);
    if (!qdate.isValid()) {
        qWarning() << kGpsMsgDateIsInvalid << "(invalid calendar date)";
        return false;
    }

    return true;
}

}  // end namespace

void GPSParser::parseLine(const QString &line) {
    if (line.isEmpty()) return;
    static bool isGGA_Ready = false;
    static bool isRMC_Ready = false;

    if (line.startsWith("$GPGGA")) {
        parseGGA(line, isGGA_Ready);
    } else if (line.startsWith("$GPRMC")) {
        parseRMC(line, isRMC_Ready);
    }
    if (isGGA_Ready && isRMC_Ready) {
        emit gpsUpdated(data);
        isGGA_Ready = false;
        isRMC_Ready = false;
    }
}

void GPSParser::parseGGA(const QString &line, bool &isValid) {
    QStringList parts = line.split(",");
    if (!isGgaNumberValid(parts.size())) {
        isValid = false;
        return;
    }

    data.altitude = parts[kGgaAltitudeFieldPartIndex].toDouble();
    isAltitudeRangeValid(data.altitude);

    data.timeUtc = parts[kGgaTimeUtcPartIndex];

    data.satellites = parts[kGgaSatellitesPartIndex].toInt();
    isSatellitesNumberValid(data.satellites);

    data.valid_gps_flag = isGpsFlagValid(parts[kGgaValidGpsFlagPartIndex]);
    isValid = data.valid_gps_flag;
}

void GPSParser::parseRMC(const QString &line, bool &isValid) {
    QStringList parts = line.split(",");
    if (!isRmcNumberValid(parts.size())) {
        isValid = false;
        return;
    }

    data.latitude = toDegrees(parts[kRmcLatitudeFieldPartIndex],
                              parts[kRmcLatitudeFieldPartDirection]);
    isLatitudeValid(data.latitude);

    data.longitude = toDegrees(parts[kRmcLongitudeFieldPartIndex],
                               parts[kRmcLongitudeFieldPartDirection]);
    isLongitudeValid(data.longitude);

    data.speedKmh = parts[kRmcSpeedPartIndex].toDouble() * kMileToKmConversion;
    isSpeedValueValid(data.speedKmh);

    data.course = parts[kRmcCoursePartIndex].toDouble();
    isCourseValid(data.course);

    data.date = parts[kRmcDatePartIndex];
    isDateValid(data.date);

    isValid = isRmcStatusValid(parts[kRmcStatusPartIndex]);
}
