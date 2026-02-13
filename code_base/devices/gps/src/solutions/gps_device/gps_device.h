#ifndef DEVICES_GPS_SRC_SOLUTIONS_GPS_DEVICE_GPS_DEVICE_H_
#define DEVICES_GPS_SRC_SOLUTIONS_GPS_DEVICE_GPS_DEVICE_H_

#include <QObject>

#include "QtConcurrent/QtConcurrent"  // IWYU pragma: keep
#include "gps_data.h"
#include "gps_logger.h"
#include "gps_parser.h"
#include "gps_port_autodetector.h"
#include "gps_receiver.h"
#include "gps_widget.h"
#include "qobjectdefs.h"

namespace {
constexpr char kBinFileName[] = "gpsAll.txt";
constexpr char kTextFileName[] = "gpsDataLog.txt";
}  // namespace

class GPSDevice : public QObject {
    Q_OBJECT
public:
    GpsWidget *createWidget(QWidget *parent = nullptr);

    explicit GPSDevice(QObject *parent = nullptr);
    bool isRunning() const { return m_isRunning; };
    ~GPSDevice();

public slots:
    void start();  // автодетект порта (поиск gps приёмников из файла
                   // gps_devices.json)
    void start(const QString &portName);  // default baudRate
    void start(const QString &portName, QSerialPort::BaudRate baudRate);
    void stop();  // вызов есть в  деструкторе класса

    void writeParcedToTextFile(logger::saveFormat format,
                               const QString &fileFullPath = kTextFileName);
    void writeAllToBinFile(const QString &fileFullPath = kBinFileName);

signals:
    void gpsStatusChanged(const QString &msg);
    void gpsDataUpdated(const GpsData &data);

private:
    bool setupBeforeStart();

private:
    bool m_isRunning = false;
    bool m_isSaveGpsDataToFile_connected = false;
    bool m_isSaveGpsLineToFile_connected = false;
    GPSReceiver *m_gps_receiver;
    GPSParser *m_gps_parser;
    QFuture<void> m_future;
};

#endif  // DEVICES_GPS_SRC_SOLUTIONS_GPS_DEVICE_GPS_DEVICE_H_
