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

    // Устанавливаем обработчики сигналов
    std::signal(SIGINT, handleSignal);
    std::signal(SIGTERM, handleSignal);

    gps_receiver = new GPSReceiver;
    QObject::connect(gps_receiver, &GPSParser::gpsDataUpdated,
                     [&](const GpsData data) {
                         qDebug() << gps::toCsvString(data);
                         logger::saveGpsDataToLogFile(data);
                     });

    // Проверяем аргументы командной строки
    QString portPath;
    if (argc > 1) {
        portPath = QString(argv[1]);
    } else {
        qWarning() << "default COM port will be used.....";
        portPath = QString("/dev/pts/1");
    }

    future = QtConcurrent::run(gps_receiver, &GPSReceiver::start, portPath);

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
