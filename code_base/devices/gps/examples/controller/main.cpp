#include <QApplication>

#include "controller.h"
#include "file_data_writer.cpp"
#include "gps_data.h"

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
