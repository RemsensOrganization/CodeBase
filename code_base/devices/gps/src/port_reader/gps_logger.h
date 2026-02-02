#ifndef DEVICES_GPS_SRC_PORT_READER_GPS_LOGGER_H_
#define DEVICES_GPS_SRC_PORT_READER_GPS_LOGGER_H_

class QString;

struct GpsData;

namespace logger{

extern const char kGpsFileLogName[];

void saveGpsDataToFile(const GpsData &data, const QString &filePath);

void saveGpsDataToLogFile(const GpsData &data);

}  // end namespace msgs


#endif  // DEVICES_GPS_SRC_PORT_READER_GPS_LOGGER_H_
