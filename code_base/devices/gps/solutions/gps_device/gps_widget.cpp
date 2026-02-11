#include "gps_widget.h"

#include "QVBoxLayout"
#include "gps_data.h"

namespace {
constexpr char NA[] = "NA";
}

GpsWidget::GpsWidget(QWidget *parent) : QWidget(parent) {
    layout = new QFormLayout(this);
    latitudeLabel = new QLabel(NA);
    longitudeLabel = new QLabel(NA);
    altitudeLabel = new QLabel(NA);
    speedLabel = new QLabel(NA);
    courseLabel = new QLabel(NA);
    dateLabel = new QLabel(NA);
    timeUtcLabel = new QLabel(NA);
    satellitesLabel = new QLabel(NA);
    gpsStatusLabel = new QLabel(NA);

    layout->addRow("Latitude:", latitudeLabel);
    layout->addRow("Longitude:", longitudeLabel);
    layout->addRow("Altitude (m):", altitudeLabel);
    layout->addRow("Speed (km/h):", speedLabel);
    layout->addRow("Course:", courseLabel);
    layout->addRow("Date:", dateLabel);
    layout->addRow("Time UTC:", timeUtcLabel);
    layout->addRow("Satellites:", satellitesLabel);
    layout->addRow("Status:", gpsStatusLabel);
}

void GpsWidget::showGpsData(const GpsData &data) {
    latitudeLabel->setText(QString::number(data.latitude, 'f', 6));
    longitudeLabel->setText(QString::number(data.longitude, 'f', 6));
    altitudeLabel->setText(QString::number(data.altitude, 'f', 1));
    speedLabel->setText(QString::number(data.speedKmh, 'f', 1));
    courseLabel->setText(QString::number(data.course, 'f', 1));
    dateLabel->setText(data.date);
    timeUtcLabel->setText(data.timeUtc);
    satellitesLabel->setText(QString::number(data.satellites));
};
void GpsWidget::showGpsStatus(const QString &status) {
    gpsStatusLabel->setText(status);
};

QPlainTextEdit *status;
