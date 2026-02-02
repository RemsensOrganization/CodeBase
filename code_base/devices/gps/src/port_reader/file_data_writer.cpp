#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "gps_data.h"

void saveGpsDataToFile(const GpsData &data, const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << data.toString();
        out.flush();
    } else {
        qDebug() << file.errorString();
    }
}
