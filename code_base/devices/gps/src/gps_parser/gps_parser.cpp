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

constexpr int kRmcTimeUtcPartIndex = 1;
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
    "Rmc number of parts is wrong. Must be more than %1 or equal: %2 fact: %3";
constexpr char kGpsMsgWrongGgaNumberOfParts[] =
    "Gga number of parts is wrong. Must be more than %1 or equal: %2 fact: %3";
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
    double decimal = deg + min / 60.0;
    if (dir == "S" || dir == "W") decimal *= -1;
    if (!isOk) {
        errors.append(QString(kGpsMsgConvertToDegreesError));
        decimal = NAN;
    }
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
                          .arg(kGgaMinNumberOfParts)
                          .arg(kGgaMinNumberOfParts)
                          .arg(number));
    }
    return result;
}

bool isRmcNumberValid(const int number, QStringList& errors) {
    bool result = (number >= kRmcMinNumberOfParts);
    if (!result)
        errors.append(QString(kGpsMsgWrongRmcNumberOfParts)
                          .arg(kRmcMinNumberOfParts)
                          .arg(kRmcMinNumberOfParts)
                          .arg(number));
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

bool isDateValid(QString& date, QStringList& errors) {
    if (date.size() != 6 || !date.toUInt()) {
        errors.append(QString(kGpsMsgDateIsInvalid) + QString("(bad format)"));
        date = QString(kNA);
        return false;
    }

    bool okDay, okMonth, okYear;
    int day = date.midRef(0, 2).toInt(&okDay);
    int month = date.midRef(2, 2).toInt(&okMonth);
    int year = date.midRef(4, 2).toInt(&okYear);

    if (!okDay || !okMonth || !okYear) {
        errors.append(QString(kGpsMsgDateIsInvalid) + QString("(parse error)"));
        date = QString(kNA);
        return false;
    }

    year += 2000;

    QDate qdate(year, month, day);
    if (!qdate.isValid()) {
        errors.append(QString(kGpsMsgDateIsInvalid) +
                      QString("(invalid calendar date)"));
        date = QString(kNA);
        return false;
    }

    return true;
}

bool isTimeValid(const QString& timeStr, QStringList& errors) {
    if (timeStr.isEmpty()) {
        errors.append("Time field is empty");
        return false;
    }

    bool ok;
    int hours = timeStr.midRef(0, 2).toInt(&ok);
    if (!ok || hours < 0 || hours > 23) {
        errors.append("Invalid hours in time field");
        return false;
    }

    int minutes = timeStr.midRef(2, 2).toInt(&ok);
    if (!ok || minutes < 0 || minutes > 59) {
        errors.append("Invalid minutes in time field");
        return false;
    }

    double seconds = timeStr.midRef(4).toDouble(&ok);
    if (!ok || seconds < 0.0 || seconds >= 60.0) {
        errors.append("Invalid seconds in time field");
        return false;
    }

    return true;
}

}  // end namespace

GPSParser::GPSParser(QObject* parent, EmitMode mode)
    : QObject(parent), emitMode(mode) {}

void GPSParser::parseLine(const QString line) {
    if (line.isEmpty()) return;
    static QString rmcTime = "";
    static bool isGGA_Ready = false;
    static bool isRMC_Ready = false;

    if (line.startsWith("$GPRMC")) {
        parseRMC(line, rmcTime, isRMC_Ready);
    } else if (line.startsWith("$GPGGA")) {
        parseGGA(line, isGGA_Ready);
    }

    bool shouldEmit = false;
    switch (emitMode) {
        case EmitMode::BothValid:
            shouldEmit = (isGGA_Ready && isRMC_Ready &&
                          isSameMoment(data.timeUtc, data.timeUtc));
            break;
        case EmitMode::AnyValid:
            shouldEmit = (isGGA_Ready || isRMC_Ready);
            break;
    }

    if (shouldEmit) {
        if (isGGA_Ready && isRMC_Ready) {
            emit gpsUpdated(data, QPrivateSignal{});
            if (!data.errors.empty()) {
                qWarning() << data.errors;
            }
            data.clearGpsData();
            isGGA_Ready = false;
            isRMC_Ready = false;
        }
    }
}

void GPSParser::parseGGA(const QString& line, bool& isValid) {
    QStringList parts = line.split(",");
    if (!isGgaNumberValid(parts.size(), data.errors)) {
        isValid = false;
        return;
    }
    bool isOk = false;
    data.altitude = parts[kGgaAltitudeFieldPartIndex].toDouble(&isOk);
    isAltitudeRangeValid(data.altitude, data.errors);
    if (!isOk) {
        data.altitude = NAN;
    }

    data.timeUtc = parts[kGgaTimeUtcPartIndex];
    isOk = isTimeValid(data.timeUtc, data.errors);
    if (!isOk) {
        data.timeUtc = kNAN;
    }

    data.satellites = parts[kGgaSatellitesPartIndex].toInt(&isOk);
    isSatellitesNumberValid(data.satellites, data.errors);
    if (!isOk) {
        data.satellites = NAN;
    }

    data.isGpsDataValid =
        isGpsFlagValid(parts[kGgaValidGpsFlagPartIndex], data.errors);
    isValid = data.isGpsDataValid;
}

void GPSParser::parseRMC(const QString& line, QString& rmcTime, bool& isValid) {
    QStringList parts = line.split(",");
    if (!isRmcNumberValid(parts.size(), data.errors)) {
        isValid = false;
        return;
    }
    rmcTime = parts[kRmcTimeUtcPartIndex];
    if (!isTimeValid(rmcTime, data.errors)) {
        rmcTime = kNAN;
    }

    data.latitude =
        toDegrees(parts[kRmcLatitudeFieldPartIndex],
                  parts[kRmcLatitudeFieldPartDirection], data.errors);
    isLatitudeValid(data.latitude, data.errors);

    data.longitude =
        toDegrees(parts[kRmcLongitudeFieldPartIndex],
                  parts[kRmcLongitudeFieldPartDirection], data.errors);
    isLongitudeValid(data.longitude, data.errors);

    bool isOk = false;
    data.speedKmh =
        parts[kRmcSpeedPartIndex].toDouble(&isOk) * kMileToKmConversion;
    isSpeedValueValid(data.speedKmh, data.errors);
    if (!isOk) {
        data.speedKmh = NAN;
    }

    data.course = parts[kRmcCoursePartIndex].toDouble(&isOk);
    isCourseValid(data.course, data.errors);
    if (!isOk) {
        data.course = NAN;
    }

    data.date = parts[kRmcDatePartIndex];
    isDateValid(data.date, data.errors);

    isValid = isRmcStatusValid(parts[kRmcStatusPartIndex], data.errors);
}

bool GPSParser::isSameMoment(const QString& rmcTime, const QString& ggaTime) {
    if (rmcTime.isEmpty() || ggaTime.isEmpty()) return false;
    return rmcTime.left(6) == ggaTime.left(6);
}
