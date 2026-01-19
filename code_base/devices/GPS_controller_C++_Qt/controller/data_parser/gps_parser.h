#ifndef CODE_BASE_DEVICES_GPS_CONTROLLER_DATA_PARSER_GPS_PARSER_H
#define CODE_BASE_DEVICES_GPS_CONTROLLER_DATA_PARSER_GPS_PARSER_H

#include <QObject>

#include "controller/data/gps_data.h"

class GPSParser : public QObject {
    Q_OBJECT
public:
    explicit GPSParser(QObject *parent = nullptr);

    void parseLine(const QString &line);

signals:
    void gpsUpdated(const GpsData &data);

private:
    double convertCoord(const QString &coord, const QString &dir);
    void parseGpgga(const QString &line);
    void parseGprmc(const QString &line);

    GpsData latest;
    bool gotGGA = false;
    bool gotRMC = false;

};

#endif // CODE_BASE_DEVICES_GPS_CONTROLLER_DATA_PARSER_GPS_PARSER_H
