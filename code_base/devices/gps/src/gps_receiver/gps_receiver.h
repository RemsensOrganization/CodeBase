#ifndef DEVICES_GPS_SRC_PORT_READER_GPS_RECEIVER_H_
#define DEVICES_GPS_SRC_PORT_READER_GPS_RECEIVER_H_

#include <QObject>
#include <QSerialPort>

namespace msgs {

extern const char kGpsAutoPortSelected[];
extern const char kGpsMsgFailedToOpenPort[];
extern const char kGpsMsgIsNotAvailabel[];
extern const char kGpsMsgIsWaitingForDevice[];
extern const char kGpsMsgNoDataOrPortClosed[];
extern const char kGpsMsgIsReconnected[];
extern const char kGpsMsgLoopFinished[];

}  // end namespace msgs

class GPSReceiver : public QObject {
    Q_OBJECT

public:
    void start(const QString &portName, QSerialPort::BaudRate baudRate);
    void startInAutoMode() { start("", QSerialPort::Baud9600); }
    void startCOM(const int COM) {
        start((QString("COM%1").arg(COM)), QSerialPort::Baud9600);
    }
    void startCOM(const int COM, QSerialPort::BaudRate baudRate) {
        start((QString("COM%1").arg(COM)), baudRate);
    }
    void stop();

signals:
    void gpsDataReceived(const QByteArray &data, QPrivateSignal);
    void gpsStatusChanged(const QString &status, QPrivateSignal);

private:
    void readLoop(const QString &portName, const int baudRate);
    bool isRun = false;
};

#endif  // DEVICES_GPS_SRC_PORT_READER_GPS_RECEIVER_H_
