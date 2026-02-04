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
    Q_INVOKABLE void start(
        const QString &portName,
        QSerialPort::BaudRate baudRate = QSerialPort::BaudRate::Baud9600);
    void stop();

signals:
    void getDataReceived(const QByteArray &data);
    void sendStatus(const QString &status);

private:
    void readLoop(const QString &portName, const int baudRate);
    bool isRun = false;
};

#endif  // DEVICES_GPS_SRC_PORT_READER_GPS_RECEIVER_H_
