#ifndef DEVICES_GPS_SRC_SOLUTIONS_GPS_DEVICE_GPS_WIDGET_H_
#define DEVICES_GPS_SRC_SOLUTIONS_GPS_DEVICE_GPS_WIDGET_H_

#include <QPlainTextEdit>
#include <QtWidgets>
// Самый новый
struct GpsData;

class GpsWidget : public QWidget {
public:
    GpsWidget(QWidget* parent = nullptr);

public slots:
    void showGpsData(const GpsData& data);
    void showGpsStatus(const QString& status);

private:
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
};

#endif  // DEVICES_GPS_SRC_SOLUTIONS_GPS_DEVICE_GPS_WIDGET_H_
