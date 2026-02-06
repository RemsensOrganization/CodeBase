#include "gps_port_autodetector.h"

#include <QDebug>

namespace msgs {

const char kGpsMsgIsDevicesLoaded[] = "GPS devices loaded:";
const char kGpsMsgIsFound[] = "GPS found:";
const char kGpsMsgIsGpsPort[] = "This is a GPS port!";
const char kGpsMsgIsNotGpsPort[] = "Not a GPS port.";

}  // end namespace msgs

GpsPortAutoDetector::GpsPortAutoDetector(QObject* parent) : QObject(parent) {
    GpsDataBase db("gps_database.json");
    db.createDefaultDatabaseIfNotExist();
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
            qDebug() << QString(msgs::kGpsMsgIsFound) << entry.vendor
                     << entry.comment;
            return true;
        }
    }
    return false;
}

// функция определения портов и проверки GPS
void GpsPortAutoDetector::findPorts() {
    qDebug() << QString(msgs::kGpsMsgIsDevicesLoaded) << gpsDatabase.size();
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
            qDebug() << QString(msgs::kGpsMsgIsGpsPort);
        } else {
            qDebug() << QString(msgs::kGpsMsgIsNotGpsPort);
        }
    }
}
QList<QSerialPortInfo> GpsPortAutoDetector::getGpsPorts() const {
    return gpsPorts;
}
