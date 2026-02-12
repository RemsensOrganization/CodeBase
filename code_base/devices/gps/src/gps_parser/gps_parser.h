#ifndef DEVICES_GPS_SRC_DATA_PARSER_GPS_PARSER_H_
#define DEVICES_GPS_SRC_DATA_PARSER_GPS_PARSER_H_

#include <gps_data.h>

#include <QObject>

class GPSParser : public QObject {
    Q_OBJECT
    friend class TestsParser;

public:
    void parseLine(const QString &line);

signals:
    void gpsUpdated(const GpsData &data, QPrivateSignal);

private:
    void parseGGA(const QString &line, bool &isValid);
    void parseRMC(const QString &line, bool &isValid);

    GpsData data;
};

#endif  // DEVICES_GPS_SRC_DATA_PARSER_GPS_PARSER_H_
