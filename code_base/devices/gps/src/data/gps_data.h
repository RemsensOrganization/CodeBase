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

    QString toString() const {
        QStringList parts;
        if (valid_gps_flag) {
            parts << QString("Coordinates: %1, %2")
                         .arg(latitude, 0, 'f', 6)
                         .arg(longitude, 0, 'f', 6);

            if (timeUtc.length() >= 6) {
                QString hh = timeUtc.mid(0, 2);
                QString mm = timeUtc.mid(2, 2);
                QString ss = timeUtc.mid(4, 2);
                parts << "Time (UTC): " + hh + ":" + mm + ":" + ss;
            }

            if (date.length() == 6) {
                QString dd = date.mid(0, 2);
                QString mm = date.mid(2, 2);
                QString yy = date.mid(4, 2);
                parts << "Date: " + dd + "." + mm + ".20" + yy;
            }

            parts << QString("Altitude: %1 m").arg(altitude, 0, 'f', 1);
            parts << QString("Satellites: %1").arg(satellites);
            parts << QString("Speed: %1 km/h").arg(speedKmh, 0, 'f', 1);
            parts << QString("Course: %1°").arg(course, 0, 'f', 1);
            parts << "\n";
        } else {
            parts << "No valid data";
        }

        return parts.join(" | ");
    }
};
Q_DECLARE_METATYPE(GpsData)

#endif  // DEVICES_GPS_SRC_DATA_GPS_DATA_H_
