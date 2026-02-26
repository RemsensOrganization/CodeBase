#ifndef DEVICES_GPS_SRC_SOLUTIONS_GPS_DEVICE_GPS_WIDGET_H_
#define DEVICES_GPS_SRC_SOLUTIONS_GPS_DEVICE_GPS_WIDGET_H_

#include <QtWidgets>

#include "gps_view_interface.h"

struct GpsData;

// Одна из возможных реализаций. Можно добавить другие, отнаследовавшись от
// IGpsView
class GpsSimpleWidget : public QWidget, public IGpsView {
    Q_OBJECT
public:
    explicit GpsSimpleWidget(QWidget* parent = nullptr);
    QObject* asQObject() override { return this; };
    void showGpsData(const GpsData& data) override;
    void showGpsStatus(const QString& status) override;

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
