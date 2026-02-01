#ifndef DEVICES_GPS_SRC_PORT_READER_GPS_RECEIVER_H_
#define DEVICES_GPS_SRC_PORT_READER_GPS_RECEIVER_H_

#include <QObject>
#include <QSerialPort>

class GPSReceiver : public QObject {
    Q_OBJECT

public:
    explicit GPSReceiver(QObject *parent = nullptr);

    void startInThread(const QString &portName, int baudRate = 9600);
    void stopInThread();

signals:
    void getDataReceived(const QByteArray &data);

private:
    void readLoop(const QString &portName, int baudRate);

    bool running = false;
};

#endif  // DEVICES_GPS_SRC_PORT_READER_GPS_RECEIVER_H_
