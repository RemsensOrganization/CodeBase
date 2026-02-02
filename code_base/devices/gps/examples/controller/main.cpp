#include <QCoreApplication>

#include "controller.h"
#include "gps_data.h"
#include "gps_logger.h"

int main(int argc, char *argv[]) {
    qRegisterMetaType<GpsData>("GpsData");

    QCoreApplication app(argc, argv);
    gps_controller controller;

    QObject::connect(
        &controller, &gps_controller::gpsUpdated,
        [&](const GpsData &data) { logger::saveGpsDataToLogFile(data); });

    controller.start("COM2", 9600);

    return app.exec();
}
