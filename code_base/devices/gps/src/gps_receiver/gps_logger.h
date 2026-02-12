#ifndef DEVICES_GPS_SRC_GPS_RECEIVER_GPS_LOGGER_H_
#define DEVICES_GPS_SRC_GPS_RECEIVER_GPS_LOGGER_H_

#include "gps_data.h"

namespace logger {

extern const char kGpsFileLogName[];
enum class saveFormat { styled, csv, jsonCompact, jsonIndented };

void saveGpsDataToFile(const GpsData &data, saveFormat format,
                       const QString &filePath);

void saveGpsDataToFile(const GpsData &data, saveFormat format);  // рядом с exe

}  // namespace logger

#endif  // DEVICES_GPS_SRC_GPS_RECEIVER_GPS_LOGGER_H_
