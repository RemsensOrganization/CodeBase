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
const char kGpsMsgIsConnected[] = "GPS module connected to %1 successfully.";
const char kGpsMsgIsNotAvailabel[] = "Gps device is not available";
const char kGpsMsgIsWaitingForDevice[] =
    "GPS module is waiting for gps device...";
const char kGpsMsgNoDataOrPortClosed[] =
    "GPS module has recieved empty data or COM port is closed."
    "Reconnecting...";
const char kGpsMsgIsReconnected[] = "GPS module reconnected to";
const char kGpsMsgLoopFinished[] = "GPS module is terminated";

}  // end namespace msgs

void GPSReceiver::start(const QString &portName,
                        QSerialPort::BaudRate baudRate) {
    if (isRun) return;
    isRun = true;
    readLoop(portName, baudRate);
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
            emit gpsStatusChanged(
                QString(msgs::kGpsMsgAutoPortSelected) + targetPort,
                QPrivateSignal{});
        } else {
            emit gpsStatusChanged(QString(msgs::kGpsMsgIsNotAvailabel),
                                  QPrivateSignal{});
            return;
        }
    }
    emit gpsStatusChanged("Target port: " + targetPort, QPrivateSignal{});
    gps.setPortName(targetPort);

    if (!gps.open(QIODevice::ReadOnly)) {
        qWarning() << QString(msgs::kGpsMsgFailedToOpenPort);
    } else {
        emit gpsStatusChanged(QString(msgs::kGpsMsgIsConnected).arg(targetPort),
                              QPrivateSignal{});
    }

    while (isRun) {
        if (gps.isOpen() && gps.waitForReadyRead(GPS_READ_TIMEOUT_MS)) {
            QByteArray chunk = gps.readAll();
            if (!chunk.isEmpty()) {
                emit gpsDataReceived(chunk, QPrivateSignal{});
            }
        } else {
            emit gpsStatusChanged(QString(msgs::kGpsMsgNoDataOrPortClosed),
                                  QPrivateSignal{});
            gps.close();

            bool reconnected = false;
            while (isRun && !reconnected) {
                auto detectedPorts = QSerialPortInfo::availablePorts();
                for (const QSerialPortInfo &portInfo :
                     qAsConst(detectedPorts)) {
                    if (portInfo.portName() == targetPort) {
                        gps.setPortName(portInfo.portName());
                        if (gps.open(QIODevice::ReadOnly)) {
                            qDebug() << QString(msgs::kGpsMsgIsReconnected)
                                     << portInfo.portName();
                            reconnected = true;
                            break;
                        }
                    }
                }
                if (!reconnected) {
                    emit gpsStatusChanged(
                        QString(msgs::kGpsMsgIsWaitingForDevice),
                        QPrivateSignal{});
                    QThread::msleep(TRY_TO_RECONNECT_INTERVAL_MS);
                }
            }
        }
    }

    gps.close();
    emit gpsStatusChanged(QString(msgs::kGpsMsgLoopFinished), QPrivateSignal{});
    qDebug() << QString(msgs::kGpsMsgLoopFinished);
}
