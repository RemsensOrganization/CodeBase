#ifndef DEVICES_GPS_SRC_DEVICES_PORTS_EXPLORER_GPS_PORT_AUTODETECTOR_H_
#define DEVICES_GPS_SRC_DEVICES_PORTS_EXPLORER_GPS_PORT_AUTODETECTOR_H_

#include <json_file_creator.h>

#include <QObject>
#include <QSerialPortInfo>

namespace msgs {

extern const char kGpsMsgIsDevicesLoaded[];
extern const char kGpsMsgIsFound[];
extern const char kGpsMsgIsGpsPort[];
extern const char kGpsMsgIsNotGpsPort[];

}  // end namespace msgs

class GpsPortAutoDetector : public QObject {
    Q_OBJECT
public:
    explicit GpsPortAutoDetector(QObject* parent = nullptr);
    void FindPorts();
    QList<QSerialPortInfo> getGpsPorts() const;
    QString getGpsPortName() const { return gpsPortName; }

private:
    bool isCOMPortGPS(const QSerialPortInfo& portInfo) const;
    void createDefaultGpsJson(const QString& filePath);
    void loadGpsDatabase(const QString& filePath);

    QList<QSerialPortInfo> detectedPorts;
    QList<GpsDeviceEntry> gpsDatabase;
    QString gpsPortName;
    QList<QSerialPortInfo> gpsPorts;
};

#endif  // DEVICES_GPS_SRC_DEVICES_PORTS_EXPLORER_GPS_PORT_AUTODETECTOR_H_
