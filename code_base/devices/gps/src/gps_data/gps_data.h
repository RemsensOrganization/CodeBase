#ifndef DEVICES_GPS_SRC_GPS_DATA_GPS_DATA_H_
#define DEVICES_GPS_SRC_GPS_DATA_GPS_DATA_H_

#include <QJsonObject>
#include <QMetaType>  // IWYU pragma: keep
#include <QString>
#include <QStringList>
#include <climits>

extern const char kNA[];
extern const char kNAN[];

struct GpsData {
    double latitude = NAN;
    double longitude = NAN;
    double altitude = NAN;
    double speedKmh = NAN;
    double course = NAN;
    QString date = kNAN;
    QString timeUtc = kNAN;
    int satellites = NAN;
    bool isGpsDataValid = false;
    // parsing errors
    QStringList errors;

    void clearGpsData() {
        latitude = NAN;
        longitude = NAN;
        altitude = NAN;
        speedKmh = NAN;
        course = NAN;
        date = kNAN;
        timeUtc = kNAN;
        satellites = NAN;
        isGpsDataValid = false;
        errors.clear();
    };
};
Q_DECLARE_METATYPE(GpsData)

namespace gps {

QString toStyledString(const GpsData& data);

QString toCsvString(const GpsData& data);

QString toCompactJson(const GpsData& data);

QString toIndentedJson(const GpsData& data);

}  // namespace gps

#endif  // DEVICES_GPS_SRC_GPS_DATA_GPS_DATA_H_
