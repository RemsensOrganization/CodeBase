#ifndef DEVICES_GPS_EXAMPLES_CONTROLLER_CONTROLLER_H_
#define DEVICES_GPS_EXAMPLES_CONTROLLER_CONTROLLER_H_

#include <QObject>
#include <QSerialPort>

#include "gps_data.h"

class GpsController : public QObject {
    Q_OBJECT

public:
    explicit GpsController(QThread *thread, QObject *parent = nullptr);
    ~GpsController();

    void start(const QString &portName, QSerialPort::BaudRate baudRate);
    void start(const QString &portName);
    void start();
    void startCOM(const int portNumber);
    void stop();

signals:
    void updateStatus(const QString &status);
    void updateGpsData(const GpsData &data);

private slots:
    void handleParsedData(const GpsData &data);
    void initObjects();

private:
    class GPSReceiver *receiver;
    class GPSParser *parser;
    class GpsPortAutoDetector *detector;
    QThread *controller_thread;
    void releaseResources();
};

#endif  // DEVICES_GPS_EXAMPLES_CONTROLLER_CONTROLLER_H_
