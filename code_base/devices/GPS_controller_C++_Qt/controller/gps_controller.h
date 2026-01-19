#ifndef CODE_BASE_DEVICES_GPS_CONTROLLER_GPS_CONTROLLER_H
#define CODE_BASE_DEVICES_GPS_CONTROLLER_GPS_CONTROLLER_H

#include <QObject>
#include <QThread>

#include "controller/data/gps_data.h"

class gps_controller : public QObject {
    Q_OBJECT
public:
    explicit gps_controller(QObject *parent = nullptr);
    ~gps_controller();

    void start(const QString &portName, int baudRate);
    void stop();

signals:
    void gpsUpdated(const GpsData &data);

    void startReceiver(const QString &portName, int baudRate);
    void stopReceiver();

private slots:
    void handleParsedData(const GpsData &data);

private:
    class GPSReceiver *receiver;
    class GPSParser *parser;
    class GpsPortAutoDetector *detector;

    QThread receiverThread;
};

#endif  // CODE_BASE_DEVICES_GPS_CONTROLLER_GPS_CONTROLLER_H
