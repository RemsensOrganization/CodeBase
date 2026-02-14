#ifndef DEVICES_GPS_SRC_GPS_DEVICE_STATUS_GPS_DEVICE_STATUS_H_
#define DEVICES_GPS_SRC_GPS_DEVICE_STATUS_GPS_DEVICE_STATUS_H_

#include <QMetaType>  // IWYU pragma: keep
#include <QString>

// Перечисление возможных состояний GPS устройства
enum class GpsStatus {
    OFFLINE,    // нет связи с COM-портом
    IDLE,       // порт открыт, но данных нет
    ACTIVE,     // поток NMEA идёт
    SEARCHING,  // данные есть, но фиксации нет
    FIX2D,      // 2D фиксация
    FIX3D,      // 3D фиксация
    DGPS,       // дифференциальная фиксация
    RTK,        // RTK фиксация
    ERROR       // ошибка данных
};

Q_DECLARE_METATYPE(GpsStatus)

// Функция для получения текстового представления статуса
QString toString(GpsStatus status);

#endif  // DEVICES_GPS_SRC_GPS_DEVICE_STATUS_GPS_DEVICE_STATUS_H_
