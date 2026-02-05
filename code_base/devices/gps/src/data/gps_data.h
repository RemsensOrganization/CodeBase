#ifndef DEVICES_GPS_SRC_DATA_GPS_DATA_H_
#define DEVICES_GPS_SRC_DATA_GPS_DATA_H_

#include <QMetaType>  // IWYU pragma: keep
#include <QString>
#include <QStringList>

struct GpsData {
    bool isRangeValid = true;
    double latitude = 0.0;
    double longitude = 0.0;
    double altitude = 0.0;
    QString timeUtc;
    QString date;
    bool valid_gps_flag = false;
    int satellites = 0;
    double speedKmh = 0.0;
    double course = 0.0;
};
Q_DECLARE_METATYPE(GpsData)

namespace gps {

QString toStyledString(const GpsData& data);

QString toCsvString(const GpsData& data);

}  // namespace gps

#endif  // DEVICES_GPS_SRC_DATA_GPS_DATA_H_
