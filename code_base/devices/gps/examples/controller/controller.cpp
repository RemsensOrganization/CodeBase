#include "controller.h"

#include <QCoreApplication>
#include <QDebug>
#include <QThread>

#include "gps_parser.h"
#include "gps_port_autodetector.h"
#include "gps_receiver.h"

gps_controller::gps_controller(QObject *parent) : QObject(parent) {
    detector = new GpsPortAutoDetector(this);
    receiver = new GPSReceiver();
    parser = new GPSParser();

    receiver->moveToThread(&receiverThread);
    parser->moveToThread(&receiverThread);

    connect(&receiverThread, &QThread::finished, receiver,
            &QObject::deleteLater);

    connect(&receiverThread, &QThread::finished, parser, &QObject::deleteLater);

    connect(receiver, &GPSReceiver::getDataReceived, parser,
            &GPSParser::parseLine);

    connect(parser, &GPSParser::gpsUpdated, this,
            &gps_controller::handleParsedData);

    receiverThread.start();
}

gps_controller::~gps_controller() {
    receiver->stopInThread();
    receiverThread.quit();
    receiverThread.wait();
}

void gps_controller::start(const QString &portName, const int baudRate) {
    receiver->startInThread(portName, baudRate);
}

void gps_controller::stop() { receiver->stopInThread(); }

void gps_controller::handleParsedData(const GpsData &data) {
    emit gpsUpdated(data);
}
