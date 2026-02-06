#include "gps_device.h"

GPSDevice::GPSDevice(QObject *parent) : QObject(parent) {
    gps_receiver = new GPSReceiver;
    gps_parser = new GPSParser;
    gps_port_autodetector = new GpsPortAutoDetector;

    connect(gps_receiver, &GPSReceiver::gpsDataReceived, gps_parser,
            &GPSParser::parseLine);

    QObject::connect(gps_receiver, &GPSReceiver::gpsStatusChanged, this,
                     &GPSDevice::gpsStatusChanged);

    QObject::connect(gps_parser, &GPSParser::gpsUpdated, this,
                     &GPSDevice::gpsDataUpdated);
}

void GPSDevice::start() {
    future = QtConcurrent::run(gps_receiver, &GPSReceiver::startInAutoMode);
}

void GPSDevice::start(int comPort) {
    future = QtConcurrent::run(gps_receiver, &GPSReceiver::startCOM, comPort);
}

void GPSDevice::start(int comPort, QSerialPort::BaudRate baudRate) {
    future = QtConcurrent::run(gps_receiver, &GPSReceiver::startCOM, comPort,
                               baudRate);
}

void GPSDevice::stop() {
    gps_receiver->stop();
    future.waitForFinished();
    gps_receiver->deleteLater();
    gps_parser->deleteLater();
}
