#include <controller/devices_ports_explorer/gps_port_autodetector.h>

#include <QApplication>
#include <QDebug>

#include "controller/gps_controller.h"

extern void saveGpsDataToFile(const GpsData &data, const QString &filePath);

int main(int argc, char *argv[]) {
    qRegisterMetaType<GpsData>("GpsData");

    QCoreApplication app(argc, argv);
    gps_controller controller;
    QString logFilePath;
    // logFilePath =
    // "D:/programming/Qt/build-TrackerGPS-Desktop_Qt_5_12_5_MSVC2015_64bit-Debug/gps_log.txt";

    QObject::connect(&controller, &gps_controller::gpsUpdated,
                     [&](const GpsData &data) {
                         qDebug() << "[main]" << data.toString();
                         if (!logFilePath.isEmpty()) {
                             saveGpsDataToFile(data, logFilePath);
                         }
                     });

    controller.start("COM2", 9600);

    return app.exec();
}
