#ifndef CODE_BASE_DEVICES_GPS_CONTROLLER_PORT_READER_GPSRECEIVER_H
#define CODE_BASE_DEVICES_GPS_CONTROLLER_PORT_READER_GPSRECEIVER_H

#include <QObject>
#include <QSerialPort>

class GPSReceiver : public QObject
{
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


#endif // CODE_BASE_DEVICES_GPS_CONTROLLER_PORT_READER_GPSRECEIVER_H



