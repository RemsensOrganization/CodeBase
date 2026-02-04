#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "gps_data.h"

namespace logger {

const char kGpsFileLogName[] = "/gps.log";

void saveGpsDataToFile(const GpsData &data, const QString &filePath) {
    if (data.valid_gps_flag) {
        QFile file(filePath);
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << data.toCsvString();
            out.flush();
        } else {
            qDebug() << file.errorString();
        }
    }
}

void saveGpsDataToLogFile(const GpsData &data) {
    if (data.valid_gps_flag) {
        saveGpsDataToFile(
            data, QCoreApplication::applicationDirPath() + kGpsFileLogName);
    }
}

}  // namespace logger
