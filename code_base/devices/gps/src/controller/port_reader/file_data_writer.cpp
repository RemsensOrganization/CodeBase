#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "data/gps_data.h"

void saveGpsDataToFile(const GpsData &data, const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << data.toString() << "\n";
        out.flush();
    } else {
        qDebug() << "[FileWriter] Ошибка: не удалось "
                    "открыть файл";
    }
}
