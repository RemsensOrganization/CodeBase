#include "gps_device.h"

GPSDevice::GPSDevice(EmitMode mode, QObject *parent) : QObject(parent) {
    qRegisterMetaType<GpsData>("GpsData");
    qRegisterMetaType<GpsStatus>("GpsStatus");
    m_gps_receiver = new GPSReceiver;
    m_gps_parser = new GPSParser(mode);

    connect(m_gps_receiver, &GPSReceiver::gpsDataReceived, m_gps_parser,
            &GPSParser::parseLine);

    QObject::connect(m_gps_receiver, &GPSReceiver::gpsStatusChanged, this,
                     &GPSDevice::gpsStatusUpdated);

    QObject::connect(m_gps_parser, &GPSParser::gpsUpdated, this,
                     &GPSDevice::gpsDataReceived);
}

GPSDevice::~GPSDevice() {
    stop();
    delete m_gps_receiver;
    delete m_gps_parser;
    qDebug() << "GPSDevice is deleted";
}

void GPSDevice::attachView(IGpsView *view) {
    QObject::connect(this, &GPSDevice::gpsDataUpdated, view->asQObject(),
                     [view](const GpsData &data) { view->showGpsData(data); });
    QObject::connect(
        this, &GPSDevice::gpsStatusChanged, view->asQObject(),
        [view](const QString &status) { view->showGpsStatus(status); });
}

void GPSDevice::start() {
    if (m_future.isRunning()) return;
    m_future = QtConcurrent::run(m_gps_receiver, &GPSReceiver::start);
}

void GPSDevice::start(const QString &portName) {
    if (m_future.isRunning()) return;
    m_future = QtConcurrent::run(m_gps_receiver, &GPSReceiver::start, portName);
}

void GPSDevice::start(const QString &portName, QSerialPort::BaudRate baudRate) {
    if (m_future.isRunning()) return;
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
    if (m_formattedFilePath == fileFullPath && m_formattedSaveFormat == format)
        return;  // ничего не изменилось

    m_formattedFilePath = fileFullPath;
    m_formattedSaveFormat = format;

    QObject::disconnect(m_saveFormattedConnection);  // отключаем старый
    m_saveFormattedConnection = QObject::connect(
        m_gps_parser, &GPSParser::gpsUpdated,
        [format, fileFullPath](const GpsData &data) {
            logger::saveGpsDataToFile(data, format, fileFullPath);
        });
}

void GPSDevice::writeOriginGpsDataToFile(const QString &fileFullPath) {
    if (m_originFilePath == fileFullPath) return;  // путь не изменился

    m_originFilePath = fileFullPath;

    QObject::disconnect(m_saveOriginConnection);  // отключаем старый
    m_saveOriginConnection =
        QObject::connect(m_gps_receiver, &GPSReceiver::gpsDataReceived,
                         [fileFullPath](const QByteArray &line) {
                             logger::saveGpsLineToFile(line, fileFullPath);
                         });
}

void GPSDevice::gpsStatusUpdated(GpsStatus status) {
    emit gpsStatusChanged(toString(status), QPrivateSignal{});
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

void GPSDevice::gpsDataReceived(const GpsData data) {
    emit gpsDataUpdated(data, QPrivateSignal{});
    if (!data.errors.empty()) {
        // обработка сценария ошибок
    }
}
