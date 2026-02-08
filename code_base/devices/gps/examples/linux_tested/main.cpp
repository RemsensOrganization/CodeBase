#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QObject>
#include <csignal>
#include <iostream>

#include "QtConcurrent/QtConcurrent"  // IWYU pragma: keep
#include "gps_data.h"
#include "gps_logger.h"
#include "gps_parser.h"
#include "gps_receiver.h"
#include "qobjectdefs.h"

QFuture<void> future;
GPSReceiver *gps_receiver;

void stopGpsReciever() {
    if (gps_receiver) {
        gps_receiver->stop();
        future.waitForFinished();
    }
    QCoreApplication::quit();
}

void handleSignal(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        qDebug() << "Signal received, stopping GPSReceiver...";
        stopGpsReciever();
    }
}

void watchForQuitKey() {
    std::cout << "Press 'q' to quit..." << std::endl;
    char c;
    while (std::cin.get(c)) {
        if (c == 'q' || c == 'Q') {
            qDebug() << "Key 'q' pressed, quitting application...";
            stopGpsReciever();
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("gps-cli");
    QCoreApplication::setApplicationVersion("1.0");

    // Устанавливаем обработчики сигналов
    std::signal(SIGINT, handleSignal);
    std::signal(SIGTERM, handleSignal);

    QCommandLineParser parser;
    parser.setApplicationDescription("com gps cli module");
    parser.addHelpOption();
    parser.addVersionOption();

    // Опции
    QCommandLineOption pathOption({"p", "path"}, "working directory", "path");
    parser.addOption(pathOption);

    QCommandLineOption comOption({"c", "com"}, "com port name", "/dev/pts/1");
    parser.addOption(comOption);

    QCommandLineOption baudRateOption({"b", "baud"}, "9600", "logfile");
    parser.addOption(baudRateOption);

    QCommandLineOption logOption({"l", "log"}, "path to gps.log.", "logfile");
    parser.addOption(logOption);

    QCommandLineOption formatOption(
        {"f", "format"}, "Format for gps data (CSV or JSON).", "format");
    parser.addOption(formatOption);

    parser.process(app);

    QString path = parser.value(pathOption);
    QString comPort = parser.value(comOption);
    QString logFile = parser.value(logOption);
    QString format =
        parser.value(formatOption).toUpper();  // нормализуем в верхний регистр

    qDebug() << "Path:" << path;
    qDebug() << "COM Port:" << comPort;
    qDebug() << "Log File:" << logFile;
    qDebug() << "Format:" << format;

    // Проверка формата без учёта регистра
    if (format == "CSV") {
        qDebug() << "CSV format";
    } else if (format == "JSON") {
        qDebug() << "JSON format";
    } else {
        format = "CSV";
        qDebug() << "CSV as default";
    }

    gps_receiver = new GPSReceiver;
    QObject::connect(
        gps_receiver, &GPSParser::gpsDataUpdated, [&](const GpsData data) {
            logger::saveGpsDataToLogFile(data);
            auto s = gps::toCsvString(data);
            std::cout << s.remove('\n').toStdString() << "\r" << std::flush;
        });

    future = QtConcurrent::run(gps_receiver, &GPSReceiver::start, comPort);

    QObject::connect(&app, &QCoreApplication::aboutToQuit, &app, [&]() {
        gps_receiver->stop();
        future.waitForFinished();
        gps_receiver->deleteLater();
    });

    // Запускаем поток для отслеживания клавиши q
    std::thread quitThread(watchForQuitKey);
    quitThread.detach();

    return app.exec();
}
