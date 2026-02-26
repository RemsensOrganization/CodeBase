#ifndef DEVICES_GPS_SRC_SOLUTIONS_GPS_DEVICE_GPS_DEVICE_H_
#define DEVICES_GPS_SRC_SOLUTIONS_GPS_DEVICE_GPS_DEVICE_H_

#include <QObject>

#include "QtConcurrent/QtConcurrent"  // IWYU pragma: keep
#include "gps_data.h"
#include "gps_device_status.h"
#include "gps_logger.h"
#include "gps_parser.h"
#include "gps_receiver.h"
#include "gps_view_interface.h"
#include "qobjectdefs.h"

namespace {
constexpr char kOriginDataFileName[] = "gpsDataOrigin.txt";
constexpr char kFormattedDataFileName[] = "gpsDataFormatted.txt";
}  // namespace

class GPSDevice : public QObject {
    Q_OBJECT
public:
    explicit GPSDevice(EmitMode mode = EmitMode::BothValid,
                       QObject *parent = nullptr);
    bool isRunning() const { return m_future.isRunning(); };
    void attachView(IGpsView *view);
    ~GPSDevice();

public slots:
    //! запуск приема данных с устройства
    void start();  // автодетект порта (поиск gps приёмников из файла
                   // gps_devices.json)
    void start(const QString &portName);  // default baudRate
    void start(const QString &portName, QSerialPort::BaudRate baudRate);

    //! остановка приема данных с устройства
    void stop();  // этот вызов есть также в  деструкторе класса

    //! сохранение форматированных данных на диск.
    //! можно менять путь для сохранения на лету при помощи повторного вызова
    void writeFormattedGpsDataToFile(
        logger::saveFormat format,
        const QString &fileFullPath = kFormattedDataFileName);

    //! сохранение сырых данных на диск.
    //! можно менять путь для сохранения на лету при помощи повторного вызова
    void writeOriginGpsDataToFile(
        const QString &fileFullPath = kOriginDataFileName);

signals:
    //! сигнал со статусом gps
    void gpsStatusChanged(const QString &msg, QPrivateSignal);

    //! сигнал с данными от gps
    void gpsDataUpdated(const GpsData &data, QPrivateSignal);

private:
    QMetaObject::Connection m_saveFormattedConnection;
    QMetaObject::Connection m_saveOriginConnection;
    logger::saveFormat m_formattedSaveFormat{};
    QString m_originFilePath;
    QString m_formattedFilePath;
    GPSReceiver *m_gps_receiver;
    GPSParser *m_gps_parser;
    QFuture<void> m_future;

private slots:
    void gpsStatusUpdated(GpsStatus status);
    void gpsDataReceived(const GpsData data);
};

#endif  // DEVICES_GPS_SRC_SOLUTIONS_GPS_DEVICE_GPS_DEVICE_H_
