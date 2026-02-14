#include "gps_receiver.h"

#include <gps_port_autodetector.h>

#include <QDebug>
#include <QThread>

constexpr int GPS_READ_TIMEOUT_MS = 2000;
constexpr int TRY_TO_RECONNECT_INTERVAL_MS = 5000;

namespace msgs {

const char kGpsMsgAutoPortSelected[] =
    "GPS module selected COM port automatically";
const char kGpsMsgFailedToOpenPort[] = "GPS module is failed to open COM port";
const char kGpsMsgModuleConnected[] = "GPS module is connected";
const char kGpsMsgIsWaitingForDevice[] = "GPS module is trying to connect";
const char kGpsMsgIsNotAvailabel[] = "Gps device is not available";
const char kGpsMsgIsReconnected[] = "GPS module reconnected to";
const char kGpsMsgLoopFinished[] = "GPS module is terminated";

}  // end namespace msgs

void GPSReceiver::start() { start("", QSerialPort::Baud9600); }

void GPSReceiver::start(const QString &portName,
                        QSerialPort::BaudRate baudRate) {
    if (isRun) return;
    isRun = true;
    readLoop(portName, baudRate);
}

void GPSReceiver::start(const QString &portName) {
    start(portName, QSerialPort::Baud9600);
}

void GPSReceiver::stop() { isRun = false; }

void GPSReceiver::readLoop(const QString &portName, const int baudRate) {
    QSerialPort gps;
    gps.setBaudRate(baudRate);
    gps.setDataBits(QSerialPort::Data8);
    gps.setParity(QSerialPort::NoParity);
    gps.setStopBits(QSerialPort::OneStop);
    gps.setFlowControl(QSerialPort::NoFlowControl);

    QString targetPort = portName;

    if (targetPort.isEmpty()) {
        GpsPortAutoDetector detector;
        detector.findPorts();
        auto gpsPorts = detector.getGpsPorts();
        if (!gpsPorts.isEmpty()) {
            targetPort = gpsPorts.first().portName();
            qDebug() << QString(msgs::kGpsMsgAutoPortSelected) + targetPort;
        } else {
            emit gpsStatusChanged(GpsStatus::OFFLINE, QPrivateSignal{});
            qWarning() << QString(msgs::kGpsMsgIsNotAvailabel);
            return;
        }
    }
    gps.setPortName(targetPort);

    if (!gps.open(QIODevice::ReadOnly)) {
        qWarning() << QString(msgs::kGpsMsgFailedToOpenPort);
    } else {
        qDebug() << QString(msgs::kGpsMsgModuleConnected);
    }

    while (isRun) {
        if (gps.isOpen() && gps.waitForReadyRead(GPS_READ_TIMEOUT_MS)) {
            emit gpsStatusChanged(GpsStatus::IDLE, QPrivateSignal{});
            QByteArray chunk = gps.readAll();
            if (!chunk.isEmpty()) {
                emit gpsStatusChanged(GpsStatus::ACTIVE, QPrivateSignal{});
                emit gpsDataReceived(chunk, QPrivateSignal{});
            }
        } else {
            emit gpsStatusChanged(GpsStatus::OFFLINE, QPrivateSignal{});
            gps.close();

            bool reconnected = false;
            while (isRun && !reconnected) {
                auto detectedPorts = QSerialPortInfo::availablePorts();
                for (const QSerialPortInfo &portInfo :
                     qAsConst(detectedPorts)) {
                    if (portInfo.portName() == targetPort) {
                        gps.setPortName(portInfo.portName());
                        if (gps.open(QIODevice::ReadOnly)) {
                            reconnected = true;
                            break;
                        }
                    }
                }
                if (!reconnected) {
                    qDebug() << QString(msgs::kGpsMsgIsWaitingForDevice);
                    emit gpsStatusChanged(GpsStatus::OFFLINE, QPrivateSignal{});
                    QThread::msleep(TRY_TO_RECONNECT_INTERVAL_MS);
                }
            }
        }
    }

    gps.close();
    emit gpsStatusChanged(GpsStatus::OFFLINE, QPrivateSignal{});
    qDebug() << QString(msgs::kGpsMsgLoopFinished);
}
