#ifndef DEVICES_GPS_SRC_GPS_RECEIVER_GPS_LOGGER_H_
#define DEVICES_GPS_SRC_GPS_RECEIVER_GPS_LOGGER_H_

#include "gps_data.h"

namespace logger {

extern const char kGpsFileLogName[];

void saveGpsDataToFile(const GpsData &data, const QString &filePath);

void saveGpsDataToFile(const GpsData &data);

}  // namespace logger

#endif  // DEVICES_GPS_SRC_GPS_RECEIVER_GPS_LOGGER_H_
