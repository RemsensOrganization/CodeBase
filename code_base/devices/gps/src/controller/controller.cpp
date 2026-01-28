#include "controller.h"

#include <QCoreApplication>
#include <QDebug>
#include <QThread>

#include "data_parser/gps_parser.h"
#include "devices_ports_explorer/gps_port_autodetector.h"
#include "port_reader/gps_receiver.h"

gps_controller::gps_controller(QObject *parent) : QObject(parent) {
    detector = new GpsPortAutoDetector(this);
    receiver = new GPSReceiver();
    parser = new GPSParser();

    receiver->moveToThread(&receiverThread);
    parser->moveToThread(&receiverThread);

    connect(&receiverThread, &QThread::finished, receiver,
            &QObject::deleteLater);
    connect(&receiverThread, &QThread::finished, parser, &QObject::deleteLater);

    connect(this, &gps_controller::startReceiver, receiver,
            &GPSReceiver::startInThread);
    connect(this, &gps_controller::stopReceiver, receiver,
            &GPSReceiver::stopInThread);

    connect(receiver, &GPSReceiver::getDataReceived, parser,
            &GPSParser::parseLine);

    connect(parser, &GPSParser::gpsUpdated, this,
            &gps_controller::handleParsedData);

    receiverThread.start();
}

gps_controller::~gps_controller() {
    emit stopReceiver();
    receiverThread.quit();
    receiverThread.wait();
}

void gps_controller::start(const QString &portName, int baudRate = 9600) {
    detector->FindPorts();
    emit startReceiver(portName, baudRate);
}

void gps_controller::stop() { emit stopReceiver(); }

void gps_controller::handleParsedData(const GpsData &data) {
    emit gpsUpdated(data);
}
