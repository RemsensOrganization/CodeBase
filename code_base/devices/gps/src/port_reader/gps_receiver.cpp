#include "gps_receiver.h"

#include <Windows.h>
#include <gps_port_autodetector.h>

#include <QDebug>
#include <QThread>

constexpr int GPS_READ_TIMEOUT_MS = 2000;
constexpr int TRY_TO_RECONNECT_INTERVAL_MS = 5000;

namespace msgs {

const char kGpsMsgIsNotAvailabel[] = "GPS device is not available";
const char kGpsMsgIsWaitingForDevice[] = "Waiting for GPS device...";

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
            qDebug() << "Автоматически выбран порт:" << targetPort;
        } else {
            qWarning() << QString(msgs::kGpsMsgIsNotAvailabel);
            return;
        }
    }
    qDebug() << "Target port: " << targetPort;
    gps.setPortName(targetPort);

    if (!gps.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть порт:" << gps.errorString();
    } else {
        qDebug() << "GPS подключен к" << targetPort;
    }

    while (running.load(std::memory_order_relaxed)) {
        if (gps.isOpen() && gps.waitForReadyRead(GPS_READ_TIMEOUT_MS)) {
            QByteArray chunk = gps.readAll();
            if (!chunk.isEmpty()) {
                emit getDataReceived(chunk);
            }
        } else {
            qDebug() << "Нет данных или порт закрыт, пробуем реконнект...";
            gps.close();

            bool reconnected = false;
            while (running.load(std::memory_order_relaxed) && !reconnected) {
                auto detectedPorts = QSerialPortInfo::availablePorts();
                for (const QSerialPortInfo &portInfo : detectedPorts) {
                    if (portInfo.portName() == targetPort) {
                        gps.setPortName(portInfo.portName());
                        if (gps.open(QIODevice::ReadOnly)) {
                            qDebug() << "Реконнект к" << portInfo.portName();
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
    qDebug() << "Цикл завершён";
}
