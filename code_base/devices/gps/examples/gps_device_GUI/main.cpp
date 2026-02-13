#include <QApplication>
#include <QCommandLineParser>

#include "gps_device.h"
#include "mainwindow.h"

void exmpl_with_widget() {
    qRegisterMetaType<GpsData>("GpsData");
    GPSDevice *gps = new GPSDevice;
    GpsWidget *widget = gps->createWidget();  // создали виджет и теперь можно
                                              // использовать его в своем GUI
    widget->show();
    gps->start();
    gps->writeAllToBinFile();
    gps->writeParcedToTextFile(logger::saveFormat::jsonIndented);

    widget->setAttribute(Qt::WA_DeleteOnClose,
                         true);  // Авто delete после close

    // QObject::connect(widget, &QObject::destroyed, [&]() { gps->stop(); });
    QTimer::singleShot(7000, [gps]() { gps->stop(); });
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow *window = new MainWindow;
    window->setAttribute(Qt::WA_DeleteOnClose,
                         true);  // Авто delete после close

    window->show();

    return app.exec();
}
