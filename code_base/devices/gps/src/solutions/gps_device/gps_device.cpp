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
    qRegisterMetaType<GpsStatus>("GpsStatus");
    m_gps_receiver = new GPSReceiver;
    m_gps_parser = new GPSParser();

    connect(m_gps_receiver, &GPSReceiver::gpsDataReceived, m_gps_parser,
            &GPSParser::parseLine);

    QObject::connect(m_gps_receiver, &GPSReceiver::gpsStatusChanged, this,
                     &GPSDevice::gpsStatusUpdated);

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
    if (!setupBeforeStart()) return;
    m_future = QtConcurrent::run(m_gps_receiver, &GPSReceiver::start);
}

void GPSDevice::start(const QString &portName) {
    if (!setupBeforeStart()) return;
    m_future = QtConcurrent::run(m_gps_receiver, &GPSReceiver::start, portName);
}

void GPSDevice::start(const QString &portName, QSerialPort::BaudRate baudRate) {
    if (!setupBeforeStart()) return;
    m_future = QtConcurrent::run(m_gps_receiver, &GPSReceiver::start, portName,
                                 baudRate);
}

void GPSDevice::stop() {
    if (!m_future.isRunning()) return;
    qDebug() << "GPS device receved request to be stopped";
    m_gps_receiver->stop();
    m_future.waitForFinished();
}

void GPSDevice::writeFormattedGpsDataToFile(logger::saveFormat format,
                                            const QString &fileFullPath) {
    m_isSaveGpsDataToFile_connected = true;
    QObject::connect(m_gps_parser, &GPSParser::gpsUpdated,
                     [format, fileFullPath](const GpsData &data) {
                         logger::saveGpsDataToFile(data, format, fileFullPath);
                     });
}

void GPSDevice::writeOriginGpsDataToFile(const QString &fileFullPath) {
    m_isSaveGpsLineToFile_connected = true;
    QObject::connect(m_gps_receiver, &GPSReceiver::gpsDataReceived,
                     [fileFullPath](const QByteArray &data) {
                         logger::saveGpsLineToFile(data, fileFullPath);
                     });
}

bool GPSDevice::setupBeforeStart() {
    if (m_future.isRunning()) {
        qDebug() << "GPS device already running. Doing nothing";
        return false;
    }
    qDebug() << "GPS device receved request to be started";
    return true;
}

void GPSDevice::gpsStatusUpdated(GpsStatus status) {
    emit gpsStatusChanged(toString(status));
    switch (status) {
        case GpsStatus::OFFLINE:  // нет связи с COM-портом
            break;
        case GpsStatus::IDLE:  // порт открыт, но данных нет
            break;
        case GpsStatus::ACTIVE:  // поток NMEA идёт
            break;
        case GpsStatus::SEARCHING:  // данные есть, но фиксации нет
            break;
        case GpsStatus::DATA_ERROR:  // ошибка данных
            break;
    }
}
