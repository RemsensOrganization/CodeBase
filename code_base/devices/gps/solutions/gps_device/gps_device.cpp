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
    m_gps_receiver = new GPSReceiver;
    m_gps_parser = new GPSParser;

    connect(m_gps_receiver, &GPSReceiver::gpsDataReceived, m_gps_parser,
            &GPSParser::parseLine);

    QObject::connect(m_gps_receiver, &GPSReceiver::gpsStatusChanged, this,
                     &GPSDevice::gpsStatusChanged);

    QObject::connect(m_gps_parser, &GPSParser::gpsUpdated, this,
                     &GPSDevice::gpsDataUpdated);
}

GPSDevice::~GPSDevice() {
    stop();
    delete m_gps_receiver;
    delete m_gps_parser;
    qDebug() << "GPSDevice is deleted";
}

void GPSDevice::start() {
    setupBeforeStart();
    m_future = QtConcurrent::run(m_gps_receiver, &GPSReceiver::start);
}

void GPSDevice::start(const QString &portName) {
    setupBeforeStart();
    m_future = QtConcurrent::run(m_gps_receiver, &GPSReceiver::start, portName);
}

void GPSDevice::start(const QString &portName, QSerialPort::BaudRate baudRate) {
    setupBeforeStart();
    m_future = QtConcurrent::run(m_gps_receiver, &GPSReceiver::start, portName,
                                 baudRate);
}

void GPSDevice::stop() {
    if (!m_isRunning && !m_future.isRunning()) return;
    qDebug() << "GPS device receved request to be stopped";
    if (m_future.isRunning()) {
        m_gps_receiver->stop();
        m_future.waitForFinished();
    }
    m_isRunning = false;
}

void GPSDevice::setupBeforeStart() {
    if (m_isRunning) {
        qDebug() << "GPS device already running. Doing nothing";
        return;
    }
    if (m_future.isRunning()) {
        stop();
    }
    m_isRunning = true;
    qDebug() << "GPS device receved request to be started";
}
