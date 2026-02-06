#ifndef DEVICES_GPS_SRC_GPS_DATA_BASE_GPS_DATA_BASE_H_
#define DEVICES_GPS_SRC_GPS_DATA_BASE_GPS_DATA_BASE_H_

#include <QList>
#include <QString>
#include <QStringList>

namespace msgs {

extern const char kJsonMsgFailedToCreateJson[];
extern const char kJsonMsgFileCreated[];
extern const char kJsonMsgFailedToOpen[];
extern const char kJsonMsgInvalidFormat[];
extern const char kJsonMsgDevicesLoaded[];

}  // namespace msgs

struct GpsDeviceEntry {
    QString vendor;
    QString vid;
    QString pid;
    QString comment;
    QStringList keywords;
};

class GpsDataBase {
public:
    explicit GpsDataBase(const QString& filePath = "gps_database.json");

    void createDefaultDatabaseIfNotExist();
    QList<GpsDeviceEntry> loadDatabase();

private:
    QString jsonFilePath;
};

#endif  // DEVICES_GPS_SRC_GPS_DATA_BASE_GPS_DATA_BASE_H_
