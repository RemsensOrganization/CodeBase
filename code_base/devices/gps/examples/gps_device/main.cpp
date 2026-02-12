#include <QApplication>
#include <QCommandLineParser>

#include "csignal"
#include "gps_device.h"
#include "iostream"

enum class GPS_MODULE_VARIANTS {
    GPS_MODULE_CLI,
    GPS_MODULE_WITHOUT_GUI,
    GPS_MODULE_WIDGET
};

void exmpl_without_widget() {
    // 1. создаем объект GPSDevice
    GPSDevice *gps = new GPSDevice;

    // 2. Подключаем сигналы к логике пользователя данной библиотеки
    QObject::connect(gps, &GPSDevice::gpsDataUpdated, [](const GpsData &data) {
        qDebug() << "----user logic---- широта: " << data.latitude;
    });
    QObject::connect(gps, &GPSDevice::gpsStatusChanged,
                     [](const QString &status) {
                         qDebug() << "----user logic---- Status:" << status;
                     });

    // 3. Запускаем с автодетектом порта на котором висит GPS или вручную
    // указываем имя порта: gps->start("COM5");
    gps->start();

    //  GPSDevice можно останавливать и заново запускать при необходиомсти
    gps->stop();
    gps->start();
    gps->stop();
    gps->start();
    gps->stop();

    // запускаем разные  функции gps по таймеру для проверки
    QTimer::singleShot(3000, gps, &GPSDevice::stop);
    QTimer::singleShot(5000, gps, SLOT(start()));
    QTimer::singleShot(7000, [gps]() { delete gps; });
}

void exmpl_with_widget() {
    qRegisterMetaType<GpsData>("GpsData");
    GPSDevice *gps = new GPSDevice;
    GpsWidget *widget = gps->createWidget();  // создали виджет и теперь можно
                                              // использовать его в своем GUI
    widget->show();

    QTimer::singleShot(3000, gps, &GPSDevice::stop);
    QTimer::singleShot(5000, gps, SLOT(start()));
    QTimer::singleShot(7000, [gps]() { delete gps; });
}

void exmpl_CLI(QApplication &app) {
    QCommandLineParser parser;
    parser.setApplicationDescription("com gps cli module");
    parser.addHelpOption();
    parser.addVersionOption();

    // Опции
    QCommandLineOption pathOption({"p", "path"}, "working directory", "path");
    parser.addOption(pathOption);

    QCommandLineOption comOption({"c", "com"}, "com port name", "/dev/pts/1");
    parser.addOption(comOption);

    QCommandLineOption baudRateOption({"b", "baud"}, "9600", "logfile");
    parser.addOption(baudRateOption);

    QCommandLineOption logOption({"l", "log"}, "path to gps.log.", "logfile");
    parser.addOption(logOption);

    QCommandLineOption formatOption(
        {"f", "format"}, "Format for gps data (CSV or JSON).", "format");
    parser.addOption(formatOption);

    parser.process(app);

    QString path = parser.value(pathOption);
    QString comPort = parser.value(comOption);
    QString logFile = parser.value(logOption);
    QString format =
        parser.value(formatOption).toUpper();  // нормализуем в верхний регистр

    qDebug() << "Path:" << path;
    qDebug() << "COM Port:" << comPort;
    qDebug() << "Log File:" << logFile;
    qDebug() << "Format:" << format;

    // Проверка формата без учёта регистра
    if (format == "CSV") {
        qDebug() << "CSV format";
    } else if (format == "JSON") {
        qDebug() << "JSON format";
    } else {
        format = "CSV";
        qDebug() << "CSV as default";
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    GPS_MODULE_VARIANTS example_num = GPS_MODULE_VARIANTS::GPS_MODULE_WIDGET;

    switch (example_num) {
        case GPS_MODULE_VARIANTS::GPS_MODULE_CLI:
            exmpl_CLI(app);
            break;
        case GPS_MODULE_VARIANTS::GPS_MODULE_WITHOUT_GUI:
            exmpl_without_widget();
            break;
        case GPS_MODULE_VARIANTS::GPS_MODULE_WIDGET:
            exmpl_with_widget();
            break;
    }

    return app.exec();
}
