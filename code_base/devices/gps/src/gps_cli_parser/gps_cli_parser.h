#ifndef GPS_CLI_PARSER_H
#define GPS_CLI_PARSER_H

#include <QCommandLineParser>
#include <QString>

#include "QSerialPort"

class GpsCliParser {
public:
    explicit GpsCliParser();

    void setApplicationDescription(const QString& description);
    void addGpsOptions();
    bool parse(const QCoreApplication& app);

    QString path() const { return m_path; }
    QString comPort() const { return m_comPort; }
    QSerialPort::BaudRate baudRate() const { return m_baudRate; }
    bool saveZeroLevel() const { return m_saveZeroLevel; }
    QString format() const { return m_format; }
    bool isValid() const { return m_valid; }

private:
    bool isValidFormat() const;

    QCommandLineParser m_parser;
    QString m_path{"gps_log.txt"};
    QString m_comPort{"/dev/pts/1"};
    QSerialPort::BaudRate m_baudRate{QSerialPort::BaudRate::Baud9600};
    bool m_saveZeroLevel{false};
    QString m_format{"CSV"};
    bool m_valid{false};
};

#endif
