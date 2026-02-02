#ifndef DEVICES_GPS_SRC_DATA_GPS_DATA_H_
#define DEVICES_GPS_SRC_DATA_GPS_DATA_H_

#include <QDebug>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextStream>

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
        } else {
            parts << "No valid data";
        }

        return parts.join(" | ");
    }

    void saveGpsDataToFile(const GpsData &data, const QString &filePath) {
        QFile file(filePath);
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << data.toString() << "\n";
            out.flush();
        } else {
            qDebug() << "[FileWriter] Error: Failed to open file";
        }
    }
};

#endif  // DEVICES_GPS_SRC_DATA_GPS_DATA_H_
