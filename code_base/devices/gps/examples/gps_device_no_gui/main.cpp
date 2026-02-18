#include <QApplication>

#include "gps_device.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationVersion("0.0.1");

    // 1. создаем объект GPSDevice
    // EmitMode отвечает за механизм формирования данных
    GPSDevice *gps = new GPSDevice(EmitMode::AnyValid);

    // 2. Подключаем сигналы к логике пользователя данной библиотеки
    QObject::connect(gps, &GPSDevice::gpsDataUpdated, [](const GpsData &data) {
        qDebug() << "----user logic---- широта: " << data.latitude;
    });
    QObject::connect(gps, &GPSDevice::gpsStatusChanged,
                     [](const QString &status) {
                         // qDebug() << "----user logic---- Status:" << status;
                     });

    // 3. Запускаем с автодетектом порта на котором висит GPS или вручную
    // указываем имя порта: gps->start("COM5");
    gps->start();

    // 4. При необходимости можно сохранять данные на диск в виде текстового
    // файла.
    gps->writeOriginGpsDataToFile();
    gps->writeFormattedGpsDataToFile(logger::saveFormat::jsonIndented);

    // удаляем и одновременно останавливаем через деструктор.
    // если нужно просто остановить, то gps->stop();
    int secondsBeforeEnd = 10;
    QTimer::singleShot(secondsBeforeEnd * 1000, [gps]() { delete gps; });

    return app.exec();
}
