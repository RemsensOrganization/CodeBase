#ifndef GPSPORTAUTODETECTOR_H
#define GPSPORTAUTODETECTOR_H

#include <QObject>
#include <QSerialPortInfo>

#include <controller/json_file_creator/json_file_creator.h>

class GpsPortAutoDetector : public QObject
{
    Q_OBJECT
public:
    explicit GpsPortAutoDetector(QObject *parent = nullptr);
    void FindPorts();
    QList<QSerialPortInfo> getGpsPorts() const;
    QString getGpsPortName() const {return gpsPortName;}

private:
    bool isCOMPortGPS(const QSerialPortInfo& portInfo) const;
    void createDefaultGpsJson(const QString& filePath);
    void loadGpsDatabase(const QString& filePath);

    QList<QSerialPortInfo> detectedPorts;
    QList<GpsDeviceEntry> gpsDatabase;
    QString gpsPortName;
    QList<QSerialPortInfo> gpsPorts;
};

#endif // GPSPORTAUTODETECTOR_H
