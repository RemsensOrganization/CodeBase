#ifndef CODE_BASE_DEVICES_GPS_CONTROLLER_JSON_FILE_CREATOR_JSONFILECREATOR_H
#define CODE_BASE_DEVICES_GPS_CONTROLLER_JSON_FILE_CREATOR_JSONFILECREATOR_H

#include <QString>
#include <QStringList>
#include <QList>

struct GpsDeviceEntry {
    QString vendor;
    QString vid;
    QString pid;
    QString comment;
    QStringList keywords;
};

class JsonFileCreator
{
public:
    explicit JsonFileCreator(const QString& filePath = "gps_database.json");

    void createDefaultDatabase();
    QList<GpsDeviceEntry> loadDatabase();

private:
    QString jsonFilePath;
};

#endif // CODE_BASE_DEVICES_GPS_CONTROLLER_JSON_FILE_CREATOR_JSONFILECREATOR_H

