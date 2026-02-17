#include <QApplication>
#include <QCommandLineParser>

#include "gps_device.h"

void exmpl_CLI(QApplication &app) {
    QCommandLineParser parser;
    parser.setApplicationDescription("com gps cli module");
    parser.addHelpOption();
    parser.addVersionOption();

    // Опции
    QCommandLineOption pathOption({"p", "path"}, "path to file", "gps_log.txt");
    parser.addOption(pathOption);

    QCommandLineOption comOption({"c", "com"}, "com port name", "/dev/pts/1");
    parser.addOption(comOption);

    QCommandLineOption baudRateOption({"b", "baud"}, "baud rate", "9600");
    parser.addOption(baudRateOption);

    QCommandLineOption logOption({"z", "zerolevel"},
                                 "save zero level data flag", "false");
    parser.addOption(logOption);

    QCommandLineOption formatOption(
        {"f", "format"}, "Format for gps data (CSV or JSON).", "format");
    parser.addOption(formatOption);

    parser.process(app);

    if (parser.isSet("help")) {
        parser.showHelp();
        return;
    }

    QString path = parser.value(pathOption);
    QString comPort = parser.value(comOption);
    QString logFile = parser.value(logOption);
    QString format =
        parser.value(formatOption).toUpper();  // нормализуем в верхний регистр

    qDebug() << "Path:" << path;
    qDebug() << "COM Port:" << comPort;
    qDebug() << "Path to output file:" << logFile;
    qDebug() << "Format to save gps data:" << format;

    // Проверка формата без учёта регистра
    if (format == "CSV") {
        qDebug() << "CSV format";
    } else if (format == "JSON") {
        qDebug() << "JSON format";
    } else {
        format = "CSV";
        qDebug() << "CSV as default";
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationVersion("0.0.1");

    return app.exec();
}
