#ifndef DEVICES_GPS_SRC_JSON_FILE_CREATOR_JSON_FILE_CREATOR_H_
#define DEVICES_GPS_SRC_JSON_FILE_CREATOR_JSON_FILE_CREATOR_H_

#include <QList>
#include <QString>
#include <QStringList>

struct GpsDeviceEntry {
    QString vendor;
    QString vid;
    QString pid;
    QString comment;
    QStringList keywords;
};

class JsonFileCreator {
   public:
    explicit JsonFileCreator(const QString& filePath = "gps_database.json");

    void createDefaultDatabase();
    QList<GpsDeviceEntry> loadDatabase();

   private:
    QString jsonFilePath;
};

#endif  // DEVICES_GPS_SRC_JSON_FILE_CREATOR_JSON_FILE_CREATOR_H_
