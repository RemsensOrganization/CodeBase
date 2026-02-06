#ifndef GPSDEVICE_H
#define GPSDEVICE_H

#include <QObject>

#include "QtConcurrent/QtConcurrent"  // IWYU pragma: keep
#include "gps_data.h"
#include "gps_parser.h"
#include "gps_port_autodetector.h"
#include "gps_receiver.h"
#include "gps_widget.h"
#include "qobjectdefs.h"

class GPSDevice : public QObject {
    Q_OBJECT
public:
    static std::pair<GPSDevice *, GpsWidget *> createWithWidget(
        QWidget *parent = nullptr);

    explicit GPSDevice(QObject *parent = nullptr);
    ~GPSDevice();

public slots:
    void start();             // autodetect port
    void start(int comPort);  // standart baudRate
    void start(int comPort, QSerialPort::BaudRate baudRate);
    void stop();

signals:
    void gpsStatusChanged(const QString &msg);
    void gpsDataUpdated(const GpsData &data);

private:
    GPSReceiver *gps_receiver;
    GPSParser *gps_parser;
    GpsPortAutoDetector *gps_port_autodetector;
    QFuture<void> future;
};

#endif  // GPSDEVICE_H
