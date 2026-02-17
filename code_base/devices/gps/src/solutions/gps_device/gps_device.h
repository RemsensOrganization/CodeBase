#ifndef DEVICES_GPS_SRC_SOLUTIONS_GPS_DEVICE_GPS_DEVICE_H_
#define DEVICES_GPS_SRC_SOLUTIONS_GPS_DEVICE_GPS_DEVICE_H_

#include <QObject>

#include "QtConcurrent/QtConcurrent"  // IWYU pragma: keep
#include "gps_data.h"
#include "gps_device_status.h"
#include "gps_logger.h"
#include "gps_parser.h"
#include "gps_receiver.h"
#include "gps_widget.h"
#include "qobjectdefs.h"

namespace {
constexpr char kOriginDataFileName[] = "gpsDataOrigin.txt";
constexpr char kFormattedDataFileName[] = "gpsDataFormatted.txt";
}  // namespace

class GPSDevice : public QObject {
    Q_OBJECT
public:
    GpsWidget *createWidget(QWidget *parent = nullptr);

    explicit GPSDevice(QObject *parent = nullptr);
    bool isRunning() const { return m_future.isRunning(); };
    ~GPSDevice();

public slots:
    void start();  // автодетект порта (поиск gps приёмников из файла
                   // gps_devices.json)
    void start(const QString &portName);  // default baudRate
    void start(const QString &portName, QSerialPort::BaudRate baudRate);
    void stop();  // вызов есть в  деструкторе класса

    void writeFormattedGpsDataToFile(
        logger::saveFormat format,
        const QString &fileFullPath = kFormattedDataFileName);
    void writeOriginGpsDataToFile(
        const QString &fileFullPath = kOriginDataFileName);

signals:
    void gpsStatusChanged(const QString &msg);
    void gpsDataUpdated(const GpsData &data);

private:
    bool setupBeforeStart();
    bool m_isSaveGpsDataToFile_connected = false;
    bool m_isSaveGpsLineToFile_connected = false;
    GPSReceiver *m_gps_receiver;
    GPSParser *m_gps_parser;
    QFuture<void> m_future;

private slots:
    void gpsStatusUpdated(GpsStatus status);
};

#endif  // DEVICES_GPS_SRC_SOLUTIONS_GPS_DEVICE_GPS_DEVICE_H_
