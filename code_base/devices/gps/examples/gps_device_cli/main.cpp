#include <QCommandLineParser>
#include <QCoreApplication>
#include <csignal>

#include "gps_cli_parser.h"
#include "gps_device.h"

void signalHandler(int sig) {
    QCoreApplication::quit();
    exit(0);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    QCoreApplication app(argc, argv);
    app.setApplicationVersion("0.0.1");

    GpsCliParser parser;
    parser.setApplicationDescription("com gps cli module");
    parser.addGpsOptions();

    if (!parser.parse(app)) {
        return EXIT_SUCCESS;
    }

    // const QString path = parser.path();
    const QString comPortName = parser.comPort();
    QSerialPort::BaudRate baudRate = parser.baudRate();
    const bool isSaveZeroLevel = parser.saveZeroLevel();
    // const QString format = parser.format();

    GPSDevice gps_device;
    gps_device.start(comPortName, baudRate);
    gps_device.writeFormattedGpsDataToFile(logger::saveFormat::csv);
    if (isSaveZeroLevel) {
        gps_device.writeOriginGpsDataToFile();
    }
    return app.exec();
}
