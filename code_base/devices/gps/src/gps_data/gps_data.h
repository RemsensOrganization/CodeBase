#ifndef DEVICES_GPS_SRC_GPS_DATA_GPS_DATA_H_
#define DEVICES_GPS_SRC_GPS_DATA_GPS_DATA_H_

#include <QJsonObject>
#include <QMetaType>  // IWYU pragma: keep
#include <QString>
#include <QStringList>

struct GpsData {
    double latitude = 0.0;
    double longitude = 0.0;
    double altitude = 0.0;
    double speedKmh = 0.0;
    double course = 0.0;
    QString date;
    QString timeUtc;
    int satellites = 0;
    bool isGpsDataValid = false;
    // parsing errors
    QStringList errors;

    void clearErrors() { errors.clear(); }
};
Q_DECLARE_METATYPE(GpsData)

namespace gps {

QString toStyledString(const GpsData& data);

QString toCsvString(const GpsData& data);

QString toCompactJson(const GpsData& data);

QString toIndentedJson(const GpsData& data);

}  // namespace gps

#endif  // DEVICES_GPS_SRC_GPS_DATA_GPS_DATA_H_
