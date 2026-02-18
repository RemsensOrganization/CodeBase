#include <QApplication>
#include <QDebug>
#include <QObject>

#include "QtConcurrent/QtConcurrent"  // IWYU pragma: keep
#include "gps_data.h"
#include "gps_parser.h"
#include "gps_receiver.h"
#include "gps_widget.h"
#include "qobjectdefs.h"

int main(int argc, char *argv[]) {
    qRegisterMetaType<GpsData>("GpsData");

    QApplication app(argc, argv);

    GPSReceiver *gps_receiver = new GPSReceiver;
    GPSParser *gps_parser = new GPSParser;
    GpsWidget *gps_widget = new GpsWidget;

    QObject::connect(gps_receiver, &GPSReceiver::gpsStatusChanged, gps_widget,
                     &GpsWidget::showGpsStatus);
    QObject::connect(gps_receiver, &GPSReceiver::gpsDataReceived, gps_parser,
                     &GPSParser::parseLine);
    QObject::connect(gps_parser, &GPSParser::gpsUpdated, gps_widget,
                     &GpsWidget::showGpsData);

    auto future = QtConcurrent::run(gps_receiver, &GPSReceiver::startCOM, 2);
    QObject::connect(&app, &QApplication::aboutToQuit, [&]() {
        gps_receiver->stop();
        future.waitForFinished();
        gps_receiver->deleteLater();
        gps_parser->deleteLater();
    });

    gps_widget->show();

    return app.exec();
}
