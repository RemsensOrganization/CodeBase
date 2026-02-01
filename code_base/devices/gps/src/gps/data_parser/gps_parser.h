#ifndef DEVICES_GPS_SRC_GPS_DATA_PARSER_GPS_PARSER_H_
#define DEVICES_GPS_SRC_GPS_DATA_PARSER_GPS_PARSER_H_

#include <gps_data.h>

#include <QObject>

class GPSParser : public QObject {
    Q_OBJECT
    friend class TestsParser;

public:
    explicit GPSParser(QObject *parent = nullptr);

    void parseLine(const QString &line);

signals:
    void gpsUpdated(const GpsData &data);

private:
    double convertCoord(const QString &coord, const QString &dir);
    void parseGpgga(const QString &line, bool &isValid);
    void parseGprmc(const QString &line, bool &isValid);

    GpsData data;
};
Q_DECLARE_METATYPE(GpsData)
#endif  // DEVICES_GPS_SRC_GPS_DATA_PARSER_GPS_PARSER_H_
