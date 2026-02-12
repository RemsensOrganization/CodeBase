#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "gps_data.h"

namespace logger {

const char kGpsFileLogName[] = "/gps.log";

enum class saveFormat { styled, csv, jsonCompact, jsonIndented };

void saveGpsDataToFile(const GpsData& data, saveFormat format,
                       const QString& filePath) {
    if (data.isGpsDataValid) {
        QFile file(filePath);
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            switch (format) {
                case saveFormat::styled:
                    out << gps::toStyledString(data);
                    break;
                case saveFormat::csv:
                    out << gps::toCsvString(data);
                    break;
                case saveFormat::jsonCompact:
                    out << gps::toCompactJson(data);
                    break;
                case saveFormat::jsonIndented:
                    out << gps::toIndentedJson(data);
                    break;
                default:
                    break;
            }
            out.flush();
        } else {
            qDebug() << file.errorString();
        }
    }
}

void saveGpsDataToFile(const GpsData& data, saveFormat format) {
    if (data.isGpsDataValid) {
        saveGpsDataToFile(
            data, format,
            QCoreApplication::applicationDirPath() + kGpsFileLogName);
    }
}

}  // namespace logger
