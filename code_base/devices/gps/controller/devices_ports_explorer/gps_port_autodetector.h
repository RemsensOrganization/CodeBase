#ifndef GPS_PORT_AUTODETECTOR_H
#define GPS_PORT_AUTODETECTOR_H

#include <controller/json_file_creator/json_file_creator.h>

#include <QObject>
#include <QSerialPortInfo>

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

#endif  // GPS_PORT_AUTODETECTOR_H
