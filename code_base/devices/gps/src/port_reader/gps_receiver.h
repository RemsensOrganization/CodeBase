#ifndef DEVICES_GPS_SRC_PORT_READER_GPS_RECEIVER_H_
#define DEVICES_GPS_SRC_PORT_READER_GPS_RECEIVER_H_

#include <QObject>
#include <QSerialPort>

#include "atomic"

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
    void startInThread(const QString &portName, int baudRate = 9600);
    void stopInThread();

signals:
    void getDataReceived(const QByteArray &data);

private:
    void readLoop(const QString &portName, int baudRate);

    std::atomic<bool> running{false};
};

#endif  // DEVICES_GPS_SRC_PORT_READER_GPS_RECEIVER_H_
