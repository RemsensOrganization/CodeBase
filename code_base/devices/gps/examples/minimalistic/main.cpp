#include <QApplication>
#include <QDebug>
#include <QObject>

#include "QtConcurrent/QtConcurrent"  // IWYU pragma: keep
#include "gps_data.h"
#include "gps_device.h"
#include "gps_parser.h"
#include "gps_receiver.h"
#include "gps_widget.h"
#include "qobjectdefs.h"

void exmpl_pure_objects(QApplication &app) {
    qRegisterMetaType<GpsData>("GpsData");
    GPSReceiver *gps_receiver = new GPSReceiver;
    GPSParser *gps_parser = new GPSParser;
    GpsWidget *gps_widget = new GpsWidget;

    //+ автодетектор при необходимости

    QObject::connect(gps_receiver, &GPSReceiver::gpsStatusChanged, gps_widget,
                     &GpsWidget::showGpsStatus);
    QObject::connect(gps_receiver, &GPSReceiver::gpsDataReceived, gps_parser,
                     &GPSParser::parseLine);
    QObject::connect(gps_parser, &GPSParser::gpsUpdated, gps_widget,
                     &GpsWidget::showGpsData);

    auto future = QtConcurrent::run(gps_receiver, &GPSReceiver::startCOM, 3);
    QObject::connect(&app, &QApplication::aboutToQuit, [&]() {
        gps_receiver->stop();
        future.waitForFinished();
        gps_receiver->deleteLater();
        gps_parser->deleteLater();
    });

    gps_widget->show();
}

// останавливаем прием через  gps->stop() если хотим потом иметь возможность
// продолжить через gps->start(); если не хотим, то просто delete gps; в
// конце программы

void exmpl_without_widget() {
    qRegisterMetaType<GpsData>("GpsData");
    GPSDevice *gps = new GPSDevice;
    gps->start();

    // Подключаем сигналы к логике пользователя данной библиотеки
    QObject::connect(gps, &GPSDevice::gpsDataUpdated, [](const GpsData &data) {
        qDebug() << "----user logic---- широта: " << data.latitude;
    });
    QObject::connect(gps, &GPSDevice::gpsStatusChanged,
                     [](const QString &status) {
                         qDebug() << "----user logic---- Status:" << status;
                     });

    QTimer::singleShot(3000, gps, &GPSDevice::stop);
    QTimer::singleShot(5000, gps, SLOT(start()));
    QTimer::singleShot(7000, [gps]() { delete gps; });
}

void exmpl_with_widget() {
    qRegisterMetaType<GpsData>("GpsData");
    GPSDevice *gps = new GPSDevice;
    auto widget = gps->createWidget();
    widget->show();
    gps->start();

    QTimer::singleShot(3000, gps, &GPSDevice::stop);
    QTimer::singleShot(5000, gps, SLOT(start()));
    QTimer::singleShot(7000, [gps]() { delete gps; });
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // exmpl_pure_objects(app);
    //    exmpl_with_widget();
    exmpl_without_widget();
    return app.exec();
}
