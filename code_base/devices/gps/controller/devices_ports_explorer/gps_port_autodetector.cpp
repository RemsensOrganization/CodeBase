#include "controller/devices_ports_explorer/gps_port_autodetector.h"

#include <QDebug>

#include "controller/json_file_creator/json_file_creator.h"

GpsPortAutoDetector::GpsPortAutoDetector(QObject* parent) : QObject(parent) {
    JsonFileCreator db("gps_database.json");
    db.createDefaultDatabase();
    gpsDatabase = db.loadDatabase();
}

// функция определение GPS-порта по id из известной базы данных
bool GpsPortAutoDetector::isCOMPortGPS(const QSerialPortInfo& portInfo) const {
    if (!portInfo.hasVendorIdentifier() || !portInfo.hasProductIdentifier())
        return false;

    QString vid = QString::number(portInfo.vendorIdentifier(), 16)
                      .toUpper()
                      .rightJustified(4, '0');
    QString pid = QString::number(portInfo.productIdentifier(), 16)
                      .toUpper()
                      .rightJustified(4, '0');

    for (const GpsDeviceEntry& entry : gpsDatabase) {
        if (entry.vid == vid && entry.pid == pid) {
            qDebug() << "GPS найден:" << entry.vendor << entry.comment;
            return true;
        }
    }
    return false;
}

// функция определения портов и проверки GPS
void GpsPortAutoDetector::FindPorts() {
    qDebug() << "Загружено устройств:" << gpsDatabase.size();
    detectedPorts = QSerialPortInfo::availablePorts();
    gpsPorts.clear();

    for (const QSerialPortInfo& portInfo : detectedPorts) {
        qDebug() << "\nPort:" << portInfo.portName()
                 << "\nLocation:" << portInfo.systemLocation()
                 << "\nDescription:" << portInfo.description()
                 << "\nManufacturer:" << portInfo.manufacturer()
                 << "\nSerial number:" << portInfo.serialNumber()
                 << "\nVendor Identifier:"
                 << (portInfo.hasVendorIdentifier()
                         ? QByteArray::number(portInfo.vendorIdentifier(), 16)
                         : QByteArray())
                 << "\nProduct Identifier:"
                 << (portInfo.hasProductIdentifier()
                         ? QByteArray::number(portInfo.productIdentifier(), 16)
                         : QByteArray());

        if (isCOMPortGPS(portInfo)) {
            gpsPorts.append(portInfo);
            gpsPortName = portInfo.portName();
            qDebug() << "Это GPS-порт!";
        } else {
            qDebug() << "Не GPS.";
        }
    }
}
QList<QSerialPortInfo> GpsPortAutoDetector::getGpsPorts() const {
    return gpsPorts;
}
