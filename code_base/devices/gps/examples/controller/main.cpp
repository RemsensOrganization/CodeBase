#include <QApplication>

#include "controller.h"

extern void saveGpsDataToFile(const GpsData &data, const QString &filePath);

int main(int argc, char *argv[]) {
    qRegisterMetaType<GpsData>("GpsData");

    QApplication app(argc, argv);
    gps_controller controller;
    QString logFilePath = QApplication::applicationDirPath() + "/gps_log.txt";

    QObject::connect(&controller, &gps_controller::gpsUpdated,
                     [&](const GpsData &data) {
                         if (!logFilePath.isEmpty()) {
                             saveGpsDataToFile(data, logFilePath);
                         }
                     });

    controller.start("COM2", 9600);

    return app.exec();
}
