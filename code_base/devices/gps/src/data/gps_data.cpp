#include "gps_data.h"

namespace gps {

QString toStyledString(const GpsData& data) {
    QStringList parts;
    if (data.isGpsDataValid) {
        parts << QString("Coordinates: %1, %2")
                     .arg(data.latitude, 0, 'f', 6)
                     .arg(data.longitude, 0, 'f', 6);

        if (data.timeUtc.length() >= 6) {
            QString hh = data.timeUtc.mid(0, 2);
            QString mm = data.timeUtc.mid(2, 2);
            QString ss = data.timeUtc.mid(4, 2);
            parts << "Time (UTC): " + hh + ":" + mm + ":" + ss;
        }

        if (data.date.length() == 6) {
            QString dd = data.date.mid(0, 2);
            QString mm = data.date.mid(2, 2);
            QString yy = data.date.mid(4, 2);
            parts << "Date: " + dd + "." + mm + ".20" + yy;
        }

        parts << QString("Altitude: %1 m").arg(data.altitude, 0, 'f', 1);
        parts << QString("Satellites: %1").arg(data.satellites);
        parts << QString("Speed: %1 km/h").arg(data.speedKmh, 0, 'f', 1);
        parts << QString("Course: %1°").arg(data.course, 0, 'f', 1);
        parts << "\n";
    } else {
        parts << "No valid data";
    }

    return parts.join(" | ");
}

QString toCsvString(const GpsData& data) {
    if (!data.isGpsDataValid) {
        return "";
    }
    return QString("%1,%2,%3,%4,%5,%6,%7,%8")
        .arg(data.latitude, 0, 'f', 6)   // широта
        .arg(data.longitude, 0, 'f', 6)  // долгота
        .arg(data.altitude, 0, 'f', 1)   // высота
        .arg(data.timeUtc)               // время UTC
        .arg(data.date)                  // дата
        .arg(data.satellites)  // количество спутников
        .arg(data.speedKmh, 0, 'f', 1)  // скорость
        .arg(data.course, 0, 'f', 1);   // курс
}

}  // end namespace gps
