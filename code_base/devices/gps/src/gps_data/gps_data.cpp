#include "gps_data.h"

#include <QDebug>
#include <QJsonObject>

#include "qjsondocument.h"

extern const char kNA[] = "NA";
extern const char kNAN[] = "nan";

namespace {

QJsonDocument GpsDataToJson(const GpsData& data) {
    QJsonObject jo{{"latitude", data.latitude}, {"longitude", data.longitude},
                   {"altitude", data.altitude}, {"speedKmh", data.speedKmh},
                   {"course", data.course},     {"date", data.date},
                   {"timeUtc", data.timeUtc}};
    return QJsonDocument(std::move(jo));
}

}  // namespace

namespace gps {

QString toStyledString(const GpsData& data) {
    if (!data.isGpsDataValid) {
        qWarning() << "GpsData is invalid.";
        return "";
    }
    QStringList parts;
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

    return parts.join(" | ");
}

QString toCsvString(const GpsData& data) {
    if (!data.isGpsDataValid) {
        return "GpsData is invalid.";
    }
    return QString("%1,%2,%3,%4,%5,%6,%7,%8,\n")
        .arg(data.latitude, 0, 'f', 6)
        .arg(data.longitude, 0, 'f', 6)
        .arg(data.altitude, 0, 'f', 1)
        .arg(data.timeUtc)
        .arg(data.date)
        .arg(data.satellites)
        .arg(data.speedKmh, 0, 'f', 1)
        .arg(data.course, 0, 'f', 1);
}

QString toCompactJson(const GpsData& data) {
    QString result = GpsDataToJson(data).toJson(QJsonDocument::Compact);
    return result;
}

QString toIndentedJson(const GpsData& data) {
    QString result = GpsDataToJson(data).toJson(QJsonDocument::Indented);
    return result;
}

}  // end namespace gps
