#include "controller.h"

#include <QCloseEvent>
#include <QCoreApplication>
#include <QDebug>
#include <QThread>

#include "gps_parser.h"
#include "gps_port_autodetector.h"
#include "gps_receiver.h"

GpsController::GpsController(QObject *parent) : QObject(parent) {
    receiver = nullptr;
    detector = nullptr;
    parser = nullptr;
}

GpsController::~GpsController() {
    qDebug() << "gps controller destructor...";
    releaseResources();
}

void GpsController::start(const QString &portName, const int baudRate) {
    receiver->startInThread(portName, baudRate);
}

void GpsController::start() { start("", QSerialPort::BaudRate::Baud9600); }

void GpsController::start(const QString &portName) {
    start(portName, QSerialPort::BaudRate::Baud9600);
}

void GpsController::startCOM(const int portNumber) {
    const QString portName = QString("COM%1").arg(portNumber);
    start(portName);
}

void GpsController::stop() {
    receiver->stopInThread();
    qDebug() << "stop function";
}

void GpsController::handleParsedData(const GpsData &data) {
    emit gpsUpdated(data);
}

void GpsController::handleCloseEvent() { releaseResources(); }

void GpsController::releaseResources() {
    qDebug() << "release GpsController resources";
    receiver->stopInThread();
    if (receiver) delete receiver;
    if (detector) delete detector;
    if (parser) delete parser;
}

void GpsController::initObjects() {
    detector = new GpsPortAutoDetector(this);
    receiver = new GPSReceiver();
    parser = new GPSParser();
    connect(receiver, &GPSReceiver::getDataReceived, parser,
            &GPSParser::parseLine);
    connect(parser, &GPSParser::gpsUpdated, this,
            &GpsController::handleParsedData);
    qDebug() << "receiverThread thread ID:" << QThread::currentThreadId();
    emit controller_started();
}
