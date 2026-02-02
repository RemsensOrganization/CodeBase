#include "gps_receiver.h"

#include <Windows.h>
#include <gps_port_autodetector.h>

#include <QDebug>
#include <QThread>

constexpr int GPS_READ_TIMEOUT_MS = 2000;
constexpr int TRY_TO_RECONNECT_INTERVAL_MS = 5000;

namespace msgs {

const char kGpsMsgAutoPortSelected[] = "Port selected automatically";
const char kGpsMsgFailedToOpenPort[] = "Failed to open port";
const char kGpsMsgIsConnected[] = "GPS connected to";
const char kGpsMsgIsNotAvailabel[] = "GPS device is not available";
const char kGpsMsgIsWaitingForDevice[] = "Waiting for GPS device...";
const char kGpsMsgNoDataOrPortClosed[] =
    "No data or port closed, trying to reconnect...";
const char kGpsMsgIsReconnected[] = "Reconnected to";
const char kGpsMsgLoopFinished[] = "Read loop finished";

}  // end namespace msgs

void GPSReceiver::startInThread(const QString &portName, int baudRate) {
    if (running.load(std::memory_order_relaxed)) return;
    running.store(true, std::memory_order_relaxed);
    readLoop(portName, baudRate);
}

void GPSReceiver::stopInThread() {
    running.store(false, std::memory_order_relaxed);
}

void GPSReceiver::readLoop(const QString &portName, int baudRate) {
    QSerialPort gps;
    gps.setBaudRate(baudRate);
    gps.setDataBits(QSerialPort::Data8);
    gps.setParity(QSerialPort::NoParity);
    gps.setStopBits(QSerialPort::OneStop);
    gps.setFlowControl(QSerialPort::NoFlowControl);

    QString targetPort = portName;

    if (targetPort.isEmpty()) {
        GpsPortAutoDetector detector;
        detector.FindPorts();
        auto gpsPorts = detector.getGpsPorts();
        if (!gpsPorts.isEmpty()) {
            targetPort = gpsPorts.first().portName();
            qDebug() << QString(msgs::kGpsMsgAutoPortSelected) << targetPort;
        } else {
            qWarning() << QString(msgs::kGpsMsgIsNotAvailabel);
            return;
        }
    }
    qDebug() << "Target port: " << targetPort;
    gps.setPortName(targetPort);

    if (!gps.open(QIODevice::ReadOnly)) {
        qWarning() << QString(msgs::kGpsMsgFailedToOpenPort);
    } else {
        qDebug() << QString(msgs::kGpsMsgIsConnected) << targetPort;
    }

    while (running.load(std::memory_order_relaxed)) {
        if (gps.isOpen() && gps.waitForReadyRead(GPS_READ_TIMEOUT_MS)) {
            QByteArray chunk = gps.readAll();
            if (!chunk.isEmpty()) {
                emit getDataReceived(chunk);
            }
        } else {
            qDebug() << QString(msgs::kGpsMsgNoDataOrPortClosed);
            gps.close();

            bool reconnected = false;
            while (running.load(std::memory_order_relaxed) && !reconnected) {
                auto detectedPorts = QSerialPortInfo::availablePorts();
                for (const QSerialPortInfo &portInfo : detectedPorts) {
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
                    qDebug() << QString(msgs::kGpsMsgIsWaitingForDevice);
                    Sleep(TRY_TO_RECONNECT_INTERVAL_MS);
                }
            }
        }
    }

    gps.close();
    qDebug() << QString(msgs::kGpsMsgLoopFinished);
}
