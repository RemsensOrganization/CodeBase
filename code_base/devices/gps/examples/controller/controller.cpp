#include "controller.h"

#include <QCloseEvent>
#include <QCoreApplication>
#include <QDebug>
#include <QThread>

#include "gps_logger.h"
#include "gps_parser.h"
#include "gps_port_autodetector.h"
#include "gps_receiver.h"

GpsController::GpsController(QThread *thread, QObject *parent)
    : QObject(parent), controller_thread(thread) {
    this->moveToThread(controller_thread);
    receiver = nullptr;
    detector = nullptr;
    parser = nullptr;
    QObject::connect(controller_thread, &QThread::started, this,
                     &GpsController::initObjects);
    QObject::connect(controller_thread, &QThread::finished, this,
                     &GpsController::releaseResources);
}

GpsController::~GpsController() {
    qDebug() << "gps controller destructor...";
    releaseResources();
}

void GpsController::start(const QString &portName,
                          QSerialPort::BaudRate baudRate) {
    receiver->start(portName, baudRate);
}

void GpsController::start() { start("", QSerialPort::BaudRate::Baud9600); }

void GpsController::start(const QString &portName) {
    start(portName, QSerialPort::BaudRate::Baud9600);
}

void GpsController::startCOM(const int portNumber) {
    const QString portName = QString("COM%1").arg(portNumber);
    start(portName);
}

void GpsController::stop() { receiver->stop(); }

void GpsController::handleParsedData(const GpsData &data) {
    emit updateGpsData(data);
}

void GpsController::releaseResources() {
    receiver->stop();
    if (receiver) delete receiver;
    if (detector) delete detector;
    if (parser) delete parser;
}

void GpsController::initObjects() {
    detector = new GpsPortAutoDetector(this);
    receiver = new GPSReceiver();
    parser = new GPSParser();
    connect(receiver, &GPSReceiver::gpsDataReceived, parser,
            &GPSParser::parseLine);
    connect(receiver, &GPSReceiver::gpsStatusChanged, this,
            &GpsController::updateStatus);
    connect(parser, &GPSParser::gpsUpdated, this,
            &GpsController::handleParsedData);
    QObject::connect(
        this, &GpsController::updateGpsData,
        [&](const GpsData &data) { logger::saveGpsDataToLogFile(data); });
    qDebug() << "receiverThread thread ID:" << QThread::currentThreadId();
}
