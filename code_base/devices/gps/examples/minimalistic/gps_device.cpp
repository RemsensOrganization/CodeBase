#include "gps_device.h"

GpsWidget *GPSDevice::createWidget(QWidget *parent) {
    GpsWidget *widget = new GpsWidget(parent);
    QObject::connect(this, &GPSDevice::gpsDataUpdated, widget,
                     &GpsWidget::showGpsData);
    QObject::connect(this, &GPSDevice::gpsStatusChanged, widget,
                     &GpsWidget::showGpsStatus);
    return widget;
}

GPSDevice::GPSDevice(QObject *parent) : QObject(parent) {
    gps_receiver = new GPSReceiver;
    gps_parser = new GPSParser;

    connect(gps_receiver, &GPSReceiver::gpsDataReceived, gps_parser,
            &GPSParser::parseLine);

    QObject::connect(gps_receiver, &GPSReceiver::gpsStatusChanged, this,
                     &GPSDevice::gpsStatusChanged);

    QObject::connect(gps_parser, &GPSParser::gpsUpdated, this,
                     &GPSDevice::gpsDataUpdated);
}

GPSDevice::~GPSDevice() {
    stop();
    if (gps_port_autodetector) delete gps_port_autodetector;
    delete gps_receiver;
    delete gps_parser;
    qDebug() << "GPSDevice is deleted";
}

void GPSDevice::start() {
    // пока что логика такая, что старт и стоп вызыюваются только 1 раз.
    // случаи, когда старт-стоп-старт-стоп не обработаны

    gps_port_autodetector = new GpsPortAutoDetector;
    gps_port_autodetector->findPorts();
    QString port = gps_port_autodetector->getGpsPortName();
    future = QtConcurrent::run(gps_receiver, &GPSReceiver::start, port,
                               QSerialPort::Baud9600);
}

void GPSDevice::start(int comPort) {
    future = QtConcurrent::run(gps_receiver, &GPSReceiver::startCOM, comPort);
}

void GPSDevice::start(int comPort, QSerialPort::BaudRate baudRate) {
    future = QtConcurrent::run(gps_receiver, &GPSReceiver::startCOM, comPort,
                               baudRate);
}

void GPSDevice::stop() {
    if (future.isRunning()) {
        gps_receiver->stop();
        future.waitForFinished();
    }
}
