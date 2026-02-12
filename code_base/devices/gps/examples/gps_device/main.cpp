#include <QApplication>
#include <QDebug>
#include <QObject>

#include "QtConcurrent/QtConcurrent"  // IWYU pragma: keep
#include "gps_data.h"
#include "gps_device.h"
#include "gps_parser.h"
#include "gps_receiver.h"
#include "qobjectdefs.h"

void exmpl_without_widget() {
    qRegisterMetaType<GpsData>("GpsData");
    GPSDevice *gps = new GPSDevice;  // 1. создаем объект GPSDevice

    // 2. Подключаем сигналы к логике пользователя данной библиотеки
    QObject::connect(gps, &GPSDevice::gpsDataUpdated, [](const GpsData &data) {
        qDebug() << "----user logic---- широта: " << data.latitude;
    });
    QObject::connect(gps, &GPSDevice::gpsStatusChanged,
                     [](const QString &status) {
                         qDebug() << "----user logic---- Status:" << status;
                     });

    //  GPSDevice можно останавливать и заново запускать при необходиомсти
    gps->start();  // 3. Запускаем с автодетектом порта на котором висит GPS или
                   // вручную указываем номер
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

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    int example_num = 1;  // выбери номер примера

    switch (example_num) {
        case 1:
            exmpl_with_widget();
            break;
        case 2:
            exmpl_without_widget();
        default:
            break;
    }

    return app.exec();
}
