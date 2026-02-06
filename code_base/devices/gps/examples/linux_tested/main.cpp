#include <QCoreApplication>
#include <QDebug>
#include <QObject>
#include <csignal>
#include <iostream>

#include "QtConcurrent/QtConcurrent"  // IWYU pragma: keep
#include "gps_data.h"
#include "gps_parser.h"
#include "gps_receiver.h"
#include "qobjectdefs.h"

QFuture<void> future;
GPSReceiver *gps_receiver;

void handleSignal(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        qDebug() << "Signal received, stopping GPSReceiver...";
        if (gps_receiver) gps_receiver->stop();
        QCoreApplication::quit();
    }
}

void watchForQuitKey() {
    std::cout << "Press 'q' to quit..." << std::endl;
    char c;
    while (std::cin.get(c)) {
        if (c == 'q' || c == 'Q') {
            qDebug() << "Key 'q' pressed, quitting application...";
            if (gps_receiver) gps_receiver->stop();
            QCoreApplication::quit();
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    // Устанавливаем обработчики сигналов
    std::signal(SIGINT, handleSignal);
    std::signal(SIGTERM, handleSignal);
    qRegisterMetaType<GpsData>("GpsData");
    gps_receiver = new GPSReceiver;
    GPSParser *gps_parser = new GPSParser;

    QObject::connect(gps_receiver, &GPSReceiver::gpsDataReceived, gps_parser,
                     &GPSParser::parseLine);

    future = QtConcurrent::run(gps_receiver, &GPSReceiver::startCOM, 3);
    QObject::connect(&app, &QCoreApplication::aboutToQuit, &app, [&]() {
        gps_receiver->stop();
        future.waitForFinished();
        gps_receiver->deleteLater();
        gps_parser->deleteLater();
    });

    // Запускаем поток для отслеживания клавиши q
    std::thread quitThread(watchForQuitKey);
    quitThread.detach();

    return app.exec();
}
