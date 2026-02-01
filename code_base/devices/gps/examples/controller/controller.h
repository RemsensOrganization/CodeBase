#ifndef DEVICES_GPS_EXAMPLES_CONTROLLER_CONTROLLER_H_
#define DEVICES_GPS_EXAMPLES_CONTROLLER_CONTROLLER_H_

#include <QObject>
#include <QThread>

#include "gps_data.h"

class gps_controller : public QObject {
    Q_OBJECT
public:
    explicit gps_controller(QObject *parent = nullptr);
    ~gps_controller();

    void start(const QString &portName, const int baudRate = 9600);
    void stop();

signals:
    void gpsUpdated(const GpsData &data);

private slots:
    void handleParsedData(const GpsData &data);

private:
    class GPSReceiver *receiver;
    class GPSParser *parser;
    class GpsPortAutoDetector *detector;

    QThread receiverThread;
};

#endif  // DEVICES_GPS_EXAMPLES_CONTROLLER_CONTROLLER_H_
