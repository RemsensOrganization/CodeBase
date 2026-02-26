#include "gps_cli_parser.h"

#include <QDebug>
#include <QFileInfo>

#include "qdir.h"

GpsCliParser::GpsCliParser() {
    m_parser.addHelpOption();
    m_parser.addVersionOption();
}

void GpsCliParser::setApplicationDescription(const QString& description) {
    m_parser.setApplicationDescription(description);
}

void GpsCliParser::addGpsOptions() {
    m_parser.addOption(
        QCommandLineOption({"p", "path"}, "path to file", "gps_log.txt"));
    m_parser.addOption(
        QCommandLineOption({"c", "com"}, "com port name", "/dev/pts/1"));
    m_parser.addOption(QCommandLineOption({"b", "baud"}, "baud rate", "9600"));
    m_parser.addOption(QCommandLineOption(
        {"z", "zerolevel"}, "save zero level data flag", "false"));
    m_parser.addOption(QCommandLineOption(
        {"f", "format"}, "Format for gps data (CSV or JSON).", "CSV"));
}

bool GpsCliParser::parse(const QCoreApplication& app) {
    QStringList arguments = app.arguments();
    m_parser.process(arguments);

    if (!m_parser.errorText().isEmpty()) {
        qWarning() << "Parse error:" << m_parser.errorText();
        m_valid = false;
        return false;
    }

    m_path = m_parser.value("path").isEmpty() ? "gps_log.txt"
                                              : m_parser.value("path");
    m_comPort =
        m_parser.value("com").isEmpty() ? "/dev/pts/1" : m_parser.value("com");

    QFileInfo fileInfo(m_path);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        qWarning() << "Directory does not exist:" << dir.absolutePath();
        qWarning() << "Creating directory...";
        if (!dir.mkpath(".")) {
            qWarning() << "Failed to create directory" << dir.absolutePath();
            qWarning() << "Using current directory";
            m_path = "gps_log.txt";
        }
    }

    if (!fileInfo.exists()) {
        qDebug() << "Output file" << m_path << "will be created";
    }

    bool baudOk = false;
    m_baudRate = (QSerialPort::BaudRate)m_parser.value("baud").toInt(&baudOk);
    if (!baudOk || m_baudRate <= 0 || m_baudRate > QSerialPort::Baud115200) {
        qWarning().nospace() << "Invalid baud rate '" << m_parser.value("baud")
                             << "' - using 9600";
        m_baudRate = QSerialPort::Baud9600;
    }

    QString zeroFlag = m_parser.value("zerolevel").toLower();
    m_saveZeroLevel = zeroFlag == "true" || zeroFlag == "1" ||
                      zeroFlag == "yes" || zeroFlag == "y";

    m_format = m_parser.value("format").toUpper();
    if (!isValidFormat()) {
        qWarning().nospace() << "Invalid format '" << m_parser.value("format")
                             << "' - using CSV";
        m_format = "CSV";
    }

    m_valid = true;

    qDebug() << "GPS CLI parameters parsed successfully:";
    qDebug() << "  Path:" << m_path;
    qDebug() << "  COM:" << m_comPort;
    qDebug() << "  Baud:" << m_baudRate;
    qDebug() << "  ZeroLevel:" << (m_saveZeroLevel ? "ON" : "OFF");
    qDebug() << "  Format:" << m_format;

    return true;
}

bool GpsCliParser::isValidFormat() const {
    return m_format == "CSV" || m_format == "JSON";
}
