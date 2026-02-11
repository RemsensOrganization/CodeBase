#ifndef DEVICES_GPS_SOLUTIONS_GPS_WIDGET_GPS_WIDGET_H_

#include <QObject>
#include <QtWidgets>

#include "gps_parser.h"
#include "gps_receiver.h"

class GpsWidget : public QWidget {
    Q_OBJECT
public:
    explicit GpsWidget(
        const QString& comPortName,
        QSerialPort::BaudRate baudRate = QSerialPort::BaudRate::Baud9600,
        QWidget* parent = nullptr);

    ~GpsWidget();

private:
    GPSReceiver* m_receiver;
    GPSParser* m_parser;
    QString m_com_port;
    QSerialPort::BaudRate m_baudrate;
    QFuture<void> future;

    QLabel* latitudeLabel;
    QLabel* longitudeLabel;
    QLabel* altitudeLabel;
    QLabel* speedLabel;
    QLabel* courseLabel;
    QLabel* dateLabel;
    QLabel* timeUtcLabel;
    QLabel* satellitesLabel;
    QLabel* gpsStatusLabel;
    QFormLayout* layout;

private slots:
    void showGpsData(const GpsData& data);
    void showStatus(const QString& status);
};

#define DEVICES_GPS_SOLUTIONS_GPS_WIDGET_GPS_WIDGET_H_
#endif  // DEVICES_GPS_SOLUTIONS_GPS_WIDGET_GPS_WIDGET_H_
