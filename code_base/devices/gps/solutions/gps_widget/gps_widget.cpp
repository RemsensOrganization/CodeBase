#include "gps_widget.h"

#include <QtConcurrent/QtConcurrent>

constexpr char NA[] = "NA";
constexpr QSize gpsWidgetSize(200, 220);

GpsWidget::GpsWidget(const QString& comPortName, QSerialPort::BaudRate baudRate,
                     QWidget* parent) {
    if (parent) setParent(parent);
    setMinimumSize(gpsWidgetSize);
    setMaximumSize(gpsWidgetSize);
    m_receiver = new GPSReceiver;
    m_parser = new GPSParser;
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

    setLayout(layout);

    qRegisterMetaType<GpsData>();
    QObject::connect(m_receiver, &GPSReceiver::gpsDataReceived, m_parser,
                     &GPSParser::parseLine);
    QObject::connect(m_parser, &GPSParser::gpsUpdated, this,
                     &GpsWidget::showGpsData);
    QObject::connect(m_receiver, &GPSReceiver::gpsStatusChanged, this,
                     &GpsWidget::showStatus);

    future = QtConcurrent::run(m_receiver, &GPSReceiver::start, comPortName,
                               baudRate);
}

GpsWidget::~GpsWidget() {
    m_receiver->stop();
    future.waitForFinished();
    delete m_receiver;
    delete m_parser;
    delete layout;
}

void GpsWidget::showGpsData(const GpsData& data) {
    latitudeLabel->setText(QString::number(data.latitude, 'f', 6));
    longitudeLabel->setText(QString::number(data.longitude, 'f', 6));
    altitudeLabel->setText(QString::number(data.altitude, 'f', 1));
    speedLabel->setText(QString::number(data.speedKmh, 'f', 1));
    courseLabel->setText(QString::number(data.course, 'f', 1));
    dateLabel->setText(data.date);
    timeUtcLabel->setText(data.timeUtc);
    satellitesLabel->setText(QString::number(data.satellites));
}

void GpsWidget::showStatus(const QString& status) {
    gpsStatusLabel->setText(status);
}
