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
    qRegisterMetaType<GpsData>("GpsData");
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
    bool canBeStarted = setupBeforeStart();
    if (canBeStarted) {
        m_future = QtConcurrent::run(m_gps_receiver, &GPSReceiver::start);
    }
}

void GPSDevice::start(const QString &portName) {
    bool canBeStarted = setupBeforeStart();
    if (canBeStarted) {
        m_future =
            QtConcurrent::run(m_gps_receiver, &GPSReceiver::start, portName);
    }
}

void GPSDevice::start(const QString &portName, QSerialPort::BaudRate baudRate) {
    bool canBeStarted = setupBeforeStart();
    if (canBeStarted) {
        m_future = QtConcurrent::run(m_gps_receiver, &GPSReceiver::start,
                                     portName, baudRate);
    }
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

void GPSDevice::writeParcedToTextFile(logger::saveFormat format,
                                      const QString &fileFullPath) {
    // TODO отправить в другой поток
    m_isSaveGpsDataToFile_connected = true;
    QObject::connect(m_gps_parser, &GPSParser::gpsUpdated,
                     [format, fileFullPath](const GpsData &data) {
                         logger::saveGpsDataToFile(data, format, fileFullPath);
                     });
}

void GPSDevice::writeAllToBinFile(const QString &fileFullPath) {
    m_isSaveGpsLineToFile_connected = true;
    QObject::connect(m_gps_receiver, &GPSReceiver::gpsDataReceived,
                     [fileFullPath](const QByteArray &data) {
                         logger::saveGpsLineToFile(data, fileFullPath);
                     });
}

bool GPSDevice::setupBeforeStart() {
    if (m_isRunning) {
        qDebug() << "GPS device already running. Doing nothing";
        return false;
    }
    if (m_future.isRunning()) {
        stop();
    }
    m_isRunning = true;
    qDebug() << "GPS device receved request to be started";
    return true;
}
