#ifndef DEVICES_GPS_SRC_SOLUTIONS_GPS_DEVICE_GPS_VIEW_INTERFACE_H_
#define DEVICES_GPS_SRC_SOLUTIONS_GPS_DEVICE_GPS_VIEW_INTERFACE_H_
#include <QString>

class QObject;

struct GpsData;

class IGpsView {
public:
    virtual ~IGpsView() = default;
    virtual QObject* asQObject() = 0;  //  указатель на себя как на QObject
    virtual void showGpsData(const GpsData& data) = 0;
    virtual void showGpsStatus(const QString& status) = 0;
};

#endif  // DEVICES_GPS_SRC_SOLUTIONS_GPS_DEVICE_GPS_VIEW_INTERFACE_H_
