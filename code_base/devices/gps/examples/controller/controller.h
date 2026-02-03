#ifndef DEVICES_GPS_EXAMPLES_CONTROLLER_CONTROLLER_H_
#define DEVICES_GPS_EXAMPLES_CONTROLLER_CONTROLLER_H_

#include <QObject>
#include <QSerialPort>

#include "gps_data.h"

class GpsController : public QObject {
    Q_OBJECT
    friend class GpsWidget;

public:
    explicit GpsController(QThread *thread, QObject *parent = nullptr);
    ~GpsController();

    void start(const QString &portName, const int baudRate);
    void start(const QString &portName);
    void start();
    void startCOM(const int portNumber);
    void stop();
    void releaseResources();

signals:
    void gpsStatus(const QString &status);
    void gpsUpdated(const GpsData &data);

private slots:
    void handleParsedData(const GpsData &data);
    void handleCloseEvent();
    void initObjects();

private:
    class GPSReceiver *receiver;
    class GPSParser *parser;
    class GpsPortAutoDetector *detector;
    QThread *controller_thread;
};

#endif  // DEVICES_GPS_EXAMPLES_CONTROLLER_CONTROLLER_H_
