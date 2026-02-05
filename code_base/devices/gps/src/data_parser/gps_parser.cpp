#include "gps_parser.h"

#include <QDebug>
#include <QStringList>

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
constexpr char kGpsMsgWrongRmcNumberOfParts[] =
    "Rmc number of parts is wrong. Must be more %1 or equal: %2";
constexpr char kGpsMsgWrongGgaNumberOfParts[] =
    "Gga number of parts is wrong. Must be more %1 or equal: %2";
constexpr char kGpsMsgNoDirectionSymbolError[] = "Missing direction indicator";
constexpr char kGpsMsgConvertToDegreesError[] = "Failed to convert to degrees";
constexpr char kGpsMsgCourseIsOutOfRange[] = "Course is out of range";
constexpr char kGpsMsgDateIsInvalid[] = "Date is invalid";

namespace {

double toDegrees(const QString& coord, const QString& dir,
                 QStringList& errors) {
    if (coord.isEmpty() ||
        (dir != "N" && dir != "S" && dir != "E" && dir != "W")) {
        errors.append(QString(kGpsMsgNoDirectionSymbolError));
        return 0.0;
    }

    int degLen = (dir == "N" || dir == "S") ? 2 : 3;
    bool isOk;
    int deg = coord.leftRef(degLen).toInt(&isOk);
    double min = coord.midRef(degLen).toDouble(&isOk);
    if (!isOk) errors.append(QString(kGpsMsgConvertToDegreesError));
    double decimal = deg + min / 60.0;
    if (dir == "S" || dir == "W") decimal *= -1;
    return decimal;
}

bool isLatitudeValid(const double latitude, QStringList& errors) {
    bool result =
        (latitude >= kMinLatitudeValue && latitude <= kMaxLatitudeValue);
    if (!result) errors << QString(kGpsMsgLatitudeIsOutOfRange);
    return result;
}

bool isLongitudeValid(const double longitude, QStringList& errors) {
    bool result =
        (longitude >= kMinLongitudeValue && longitude <= kMaxLongitudeValue);
    if (!result) errors << kGpsMsgLongitudeIsOutOfRange;
    return result;
}

bool isSatellitesNumberValid(const int satellites, QStringList& errors) {
    bool result = (satellites >= 0 && satellites <= kMaxNumberSatellites);
    if (!result) errors << QString(kGpsMsgNumberOfSattelitesIsOutOfRange);
    return result;
};

bool isSpeedValueValid(const double speedKmh, QStringList& errors) {
    bool result = (speedKmh >= 0.0 && speedKmh < kVelocityMaxLimit);
    if (!result) errors << QString(kGpsMsgVelocityIsOutOfRange);
    return result;
}

bool isAltitudeRangeValid(const double altitude, QStringList& errors) {
    bool result =
        (altitude > kAltitudeMinLimit && altitude < kAltitudeMaxLimit);
    if (!result) errors << QString(kGpsMsgAltitudeIsOutOfRange);
    return result;
}

bool isGgaNumberValid(const int number, QStringList& errors) {
    bool result = (number >= kGgaMinNumberOfParts);
    if (!result) {
        errors.append(QString(kGpsMsgWrongGgaNumberOfParts)
                          .arg(number)
                          .arg(kGgaMinNumberOfParts));
    }
    return result;
}

bool isRmcNumberValid(const int number, QStringList& errors) {
    bool result = (number >= kRmcMinNumberOfParts);
    if (!result)
        errors.append(QString(kGpsMsgWrongRmcNumberOfParts)
                          .arg(number)
                          .arg(kRmcMinNumberOfParts));
    ;
    return result;
}

bool isGpsFlagValid(const QString& flag, QStringList& errors) {
    bool result = flag != "0";
    if (!result) errors.append(QString(kGpsMsgGpsFlagIsInvalid));
    return result;
};

bool isRmcStatusValid(const QString& flag, QStringList& errors) {
    bool result = (flag == "A");
    if (!result) errors.append(QString(kGpsMsgRmcFlagIsInvalid));
    return result;
}

bool isCourseValid(const double course, QStringList& errors) {
    bool result = (course >= 0.0 && course <= 360.0);
    if (!result) errors.append(QString(kGpsMsgCourseIsOutOfRange));
    return result;
}

bool isDateValid(const QString& date, QStringList& errors) {
    if (date.size() != 6 || !date.toUInt()) {
        errors.append(QString(kGpsMsgDateIsInvalid) + QString("(bad format)"));
        return false;
    }

    bool okDay, okMonth, okYear;
    int day = date.midRef(0, 2).toInt(&okDay);
    int month = date.midRef(2, 2).toInt(&okMonth);
    int year = date.midRef(4, 2).toInt(&okYear);

    if (!okDay || !okMonth || !okYear) {
        errors.append(QString(kGpsMsgDateIsInvalid) + QString("(parse error)"));
        return false;
    }

    year += 2000;

    QDate qdate(year, month, day);
    if (!qdate.isValid()) {
        errors.append(QString(kGpsMsgDateIsInvalid) +
                      QString("(invalid calendar date)"));
        return false;
    }

    return true;
}

}  // end namespace

void GPSParser::parseLine(const QString& line) {
    if (line.isEmpty()) return;
    static bool isGGA_Ready = false;
    static bool isRMC_Ready = false;

    if (line.startsWith("$GPGGA")) {
        parseGGA(line, isGGA_Ready);
    } else if (line.startsWith("$GPRMC")) {
        parseRMC(line, isRMC_Ready);
    }
    if (isGGA_Ready && isRMC_Ready) {
        if (!data.errors.empty()) {
            qWarning() << data.errors;
            data.clearErrors();
        }
        emit gpsUpdated(data, QPrivateSignal{});
        isGGA_Ready = false;
        isRMC_Ready = false;
    }
}

void GPSParser::parseGGA(const QString& line, bool& isValid) {
    QStringList parts = line.split(",");
    if (!isGgaNumberValid(parts.size(), data.errors)) {
        isValid = false;
        return;
    }

    data.altitude = parts[kGgaAltitudeFieldPartIndex].toDouble();
    isAltitudeRangeValid(data.altitude, data.errors);

    data.timeUtc = parts[kGgaTimeUtcPartIndex];

    data.satellites = parts[kGgaSatellitesPartIndex].toInt();
    isSatellitesNumberValid(data.satellites, data.errors);

    data.isGpsDataValid =
        isGpsFlagValid(parts[kGgaValidGpsFlagPartIndex], data.errors);
    isValid = data.isGpsDataValid;
}

void GPSParser::parseRMC(const QString& line, bool& isValid) {
    QStringList parts = line.split(",");
    if (!isRmcNumberValid(parts.size(), data.errors)) {
        isValid = false;
        return;
    }

    data.latitude =
        toDegrees(parts[kRmcLatitudeFieldPartIndex],
                  parts[kRmcLatitudeFieldPartDirection], data.errors);
    isLatitudeValid(data.latitude, data.errors);

    data.longitude =
        toDegrees(parts[kRmcLongitudeFieldPartIndex],
                  parts[kRmcLongitudeFieldPartDirection], data.errors);
    isLongitudeValid(data.longitude, data.errors);

    data.speedKmh = parts[kRmcSpeedPartIndex].toDouble() * kMileToKmConversion;
    isSpeedValueValid(data.speedKmh, data.errors);

    data.course = parts[kRmcCoursePartIndex].toDouble();
    isCourseValid(data.course, data.errors);

    data.date = parts[kRmcDatePartIndex];
    isDateValid(data.date, data.errors);

    isValid = isRmcStatusValid(parts[kRmcStatusPartIndex], data.errors);
}
