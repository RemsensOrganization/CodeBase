#ifndef DEVICES_GPS_SRC_GPS_DATA_GPS_DATA_H_
#define DEVICES_GPS_SRC_GPS_DATA_GPS_DATA_H_

#include <QMetaType>
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
            parts << QString("Координаты: %1, %2")
                         .arg(latitude, 0, 'f', 6)
                         .arg(longitude, 0, 'f', 6);

            if (timeUtc.length() >= 6) {
                QString hh = timeUtc.mid(0, 2);
                QString mm = timeUtc.mid(2, 2);
                QString ss = timeUtc.mid(4, 2);
                parts << "Время (UTC): " + hh + ":" + mm + ":" + ss;
            }

            if (date.length() == 6) {
                QString dd = date.mid(0, 2);
                QString mm = date.mid(2, 2);
                QString yy = date.mid(4, 2);
                parts << "Дата: " + dd + "." + mm + ".20" + yy;
            }

            parts << QString("Высота: %1 м").arg(altitude, 0, 'f', 1);
            parts << QString("Спутники: %1").arg(satellites);
            parts << QString("Скорость: %1 км/ч").arg(speedKmh, 0, 'f', 1);
            parts << QString("Курс: %1°").arg(course, 0, 'f', 1);
        } else {
            parts << "Нет валидных данных";
        }

        return parts.join(" | ");
    }
};

#endif  // DEVICES_GPS_SRC_GPS_DATA_GPS_DATA_H_
