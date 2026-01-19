#include <QDebug>
#include <QThread>
#include <QApplication>

#include "controller/gps_controller.h"
#include "controller/port_reader/gps_receiver.h"
#include "controller/data_parser/gps_parser.h"
#include "controller/devices_ports_explorer/gps_port_autodetector.h"

gps_controller::gps_controller(QObject *parent)
    : QObject(parent)
{
    qDebug() << "gps_controller thread:" << QThread::currentThread();
    qDebug() << "main thread:" << qApp->thread();
    Q_ASSERT(QThread::currentThread() == qApp->thread());

    detector = new GpsPortAutoDetector(this);
    receiver = new GPSReceiver();             // без parent, будет жить в потоке
    parser   = new GPSParser();

    // переносим ресивер в отдельный поток
    receiver->moveToThread(&receiverThread);
    parser->moveToThread(&receiverThread);
    Q_ASSERT(receiver->thread() == &receiverThread);
    Q_ASSERT(parser->thread() == &receiverThread);

    connect(&receiverThread, &QThread::finished,
            receiver, &QObject::deleteLater);
    connect(&receiverThread, &QThread::finished,
            parser,   &QObject::deleteLater);

    // связи контроллер ↔ ресивер
    connect(this, &gps_controller::startReceiver,
            receiver, &GPSReceiver::startInThread);
    connect(this, &gps_controller::stopReceiver,
            receiver, &GPSReceiver::stopInThread);

    // ресивер → парсер (остаются как были, но оба QObject – ок)
    connect(receiver, &GPSReceiver::getDataReceived,
            parser, &GPSParser::parseLine);

    // парсер → контроллер
    connect(parser, &GPSParser::gpsUpdated,
            this, &gps_controller::handleParsedData);

    receiverThread.start();
}

gps_controller::~gps_controller()
{
    emit stopReceiver();
    receiverThread.quit();
    receiverThread.wait();
}

void gps_controller::start(const QString &portName, int baudRate)
{
    detector->FindPorts();    // это лёгко, можно в GUI
    emit startReceiver(portName, baudRate);
}

void gps_controller::stop()
{
    emit stopReceiver();
}

void gps_controller::handleParsedData(const GpsData &data)
{
    // qDebug() << "[gps_tracker]" << data.toString();
    emit gpsUpdated(data);
}












