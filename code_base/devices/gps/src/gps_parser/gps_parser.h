#ifndef DEVICES_GPS_SRC_GPS_PARSER_GPS_PARSER_H_
#define DEVICES_GPS_SRC_GPS_PARSER_GPS_PARSER_H_

#include <gps_data.h>

#include <QObject>

enum class EmitMode {
    BothValid,  // только если GPRMC и GPGGA валидные и время совпадает
    AnyValid    // если хотя бы одно валидное
};

class GPSParser : public QObject {
    Q_OBJECT
    friend class TestsParser;

public:
    explicit GPSParser(QObject *parent = nullptr,
                       EmitMode mode = EmitMode::BothValid);

public:
    void parseLine(const QString line);

signals:
    void gpsUpdated(const GpsData data, QPrivateSignal);

private:
    void parseGGA(const QString &line, bool &isValid);
    void parseRMC(const QString &line, QString &rmcTime, bool &isValid);
    bool isSameMoment(const QString &rmcTime, const QString &ggaTime);

    EmitMode emitMode;
    GpsData data;
};

#endif  // DEVICES_GPS_SRC_GPS_PARSER_GPS_PARSER_H_
