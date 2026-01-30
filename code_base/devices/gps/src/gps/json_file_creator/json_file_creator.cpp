#include "json_file_creator.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

JsonFileCreator::JsonFileCreator(const QString& filePath)
    : jsonFilePath(filePath) {}

void JsonFileCreator::createDefaultDatabase() {
    if (QFile::exists(jsonFilePath)) return;

    QJsonArray deviceArray;

    auto addDevice = [&](const QString& vendor, const QString& vid,
                         const QString& pid, const QStringList& keywords,
                         const QString& comment) {
        QJsonObject obj;
        obj["vendor"] = vendor;
        obj["device_id"] = QJsonObject{{"vid", vid}, {"pid", pid}};
        QJsonArray descArray;
        for (const QString& word : keywords) descArray.append(word.toLower());
        obj["description"] = descArray;
        obj["comment"] = comment;
        deviceArray.append(obj);
    };

    // u-blox series
    addDevice("u-blox", "1546", "01A4", {"gps", "gnss", "ublox", "antaris"},
              "ANTARIS 4 GPS Receiver");
    addDevice("u-blox", "1546", "01A5", {"gps", "gnss", "ublox", "galileo"},
              "u-blox 5 GPS/GALILEO");
    addDevice("u-blox", "1546", "01A6", {"gps", "gnss", "ublox"},
              "u-blox 6 GPS");
    addDevice("u-blox", "1546", "01A7", {"gps", "gnss", "ublox"},
              "u-blox 7 GPS/GNSS");
    addDevice("u-blox", "1546", "01A8", {"gps", "gnss", "ublox"},
              "u-blox 8 GPS/GNSS");
    addDevice("u-blox", "1546", "01A9", {"gps", "gnss", "ublox"},
              "u-blox GNSS Receiver");
    addDevice("u-blox", "1546", "03A7", {"gps", "gnss", "ublox"},
              "u-blox 7 (alternative PID)");
    addDevice("u-blox", "1546", "03A8", {"gps", "gnss", "ublox"},
              "u-blox 8 (alternative PID)");
    for (int i = 0x0502; i <= 0x050D; ++i) {
        addDevice("u-blox", "1546",
                  QString::number(i, 16).toUpper().rightJustified(4, '0'),
                  {"gps", "gnss", "ublox", "evk", "zed", "neo", "odin"},
                  "u-blox EVK board");
    }

    // Garmin
    addDevice("Garmin", "091E", "0003", {"gps", "garmin", "receiver"},
              "Garmin GPS (Generic)");
    addDevice("Garmin", "091E", "0004", {"gps", "garmin", "gpsmap"},
              "Garmin GPSmap series");
    addDevice("Garmin", "091E", "0005", {"gps", "garmin", "etrex"},
              "Garmin eTrex series");

    // GlobalSat
    addDevice("GlobalSat", "067B", "2303", {"gps", "globalsat", "pl2303"},
              "GlobalSat via PL2303");
    addDevice("GlobalSat", "10C4", "EA60", {"gps", "globalsat", "cp210x"},
              "GlobalSat via CP210x");

    // SiRF
    addDevice("SiRF", "10C4", "EA60", {"gps", "sirf", "cp210x"},
              "SiRF Star III via CP210x");

    // Quectel
    addDevice("Quectel", "2C7C", "0001", {"gps", "gnss", "quectel"},
              "Quectel GNSS module");

    // SkyTraq
    addDevice("SkyTraq", "1A86", "7523", {"gps", "gnss", "skytraq", "ch340"},
              "SkyTraq GNSS via CH340");

    // Trimble
    addDevice("Trimble", "0B3E", "0001", {"gps", "trimble"},
              "Trimble GPS Receiver");

    // Holux
    addDevice("Holux", "0E8D", "0003", {"gps", "holux", "gpslim"},
              "Holux GPSlim series");

    // Navilock
    addDevice("Navilock", "0403", "6001", {"gps", "navilock", "ftdi"},
              "Navilock GPS via FTDI");

    // Delorme
    addDevice("Delorme", "067B", "2303", {"gps", "delorme", "earthmate"},
              "Delorme Earthmate GPS LT-20");

    QJsonDocument doc(deviceArray);
    QFile file(jsonFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "Не удалось создать JSON:" << jsonFilePath;
        return;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    qDebug() << "JSON-файл создан:" << jsonFilePath;
}

QList<GpsDeviceEntry> JsonFileCreator::loadDatabase() {
    QList<GpsDeviceEntry> gpsDatabase;

    QFile file(jsonFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть JSON:" << jsonFilePath;
        return gpsDatabase;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isArray()) {
        qWarning() << "Неверный формат JSON";
        return gpsDatabase;
    }

    QJsonArray array = doc.array();
    for (const QJsonValue& val : array) {
        if (!val.isObject()) continue;
        QJsonObject obj = val.toObject();

        GpsDeviceEntry entry;
        entry.vendor = obj["vendor"].toString();
        entry.comment = obj["comment"].toString();

        QJsonObject idObj = obj["device_id"].toObject();
        entry.vid = idObj["vid"].toString().toUpper();
        entry.pid = idObj["pid"].toString().toUpper();

        QJsonArray descArray = obj["description"].toArray();
        for (const QJsonValue& word : descArray)
            entry.keywords.append(word.toString().toLower());

        gpsDatabase.append(entry);
    }

    qDebug() << "Загружено устройств:" << gpsDatabase.size();
    return gpsDatabase;
}
