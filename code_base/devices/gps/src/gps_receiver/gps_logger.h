<<<<<<< simple_use
#ifndef DEVICES_GPS_SRC_GPS_RECEIVER_GPS_LOGGER_H_
#define DEVICES_GPS_SRC_GPS_RECEIVER_GPS_LOGGER_H_
=======
#ifndef DEVICES_GPS_SRC_PORT_READER_GPS_LOGGER_H_
#define DEVICES_GPS_SRC_PORT_READER_GPS_LOGGER_H_
>>>>>>> main

#include "gps_data.h"

namespace logger {

extern const char kGpsFileLogName[];
<<<<<<< simple_use
enum class saveFormat { styled, csv, jsonCompact, jsonIndented };

// сохранение структуры с GPS данными
void saveGpsDataToFile(const GpsData &data, saveFormat format,
                       const QString &filePath);

void saveGpsDataToFile(const GpsData &data, saveFormat format);  // рядом с exe

// сохранение сырых GPS данных
void saveGpsLineToFile(const QString &line, const QString &filePath);
void saveGpsLineToFile(const QString &line);  // рядом с exe

}  // namespace logger

#endif  // DEVICES_GPS_SRC_GPS_RECEIVER_GPS_LOGGER_H_
=======

void saveGpsDataToFile(const GpsData &data, const QString &filePath);

void saveGpsDataToLogFile(const GpsData &data);

}  // namespace logger

#endif  // DEVICES_GPS_SRC_PORT_READER_GPS_LOGGER_H_
>>>>>>> main
