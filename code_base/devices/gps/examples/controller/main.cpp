#include <QApplication>
#include <QCloseEvent>
#include <QObject>
#include <QTimer>
#include <QWidget>

#include "QDebug"
#include "QThread"
#include "controller.h"
#include "gps_data.h"
#include "gps_logger.h"

class GpsWidget : public QWidget {
public:
    GpsWidget(GpsController *gpsController) { controller = gpsController; };

protected:
    void closeEvent(QCloseEvent *event) override {
        qDebug() << "GpsWidget closing event";
        event->ignore();
        controller->releaseResources();
        event->accept();
    }

private:
    GpsController *controller;
};

int main(int argc, char *argv[]) {
    qRegisterMetaType<GpsData>("GpsData");

    QApplication app(argc, argv);

    QThread *mainThread = QApplication::instance()->thread();
    qDebug() << "Main thread ID:" << mainThread->currentThreadId();

    QThread *controller_thread = new QThread;
    GpsController *controller = new GpsController;
    controller->moveToThread(controller_thread);

    GpsWidget *gps_widget = new GpsWidget(controller);
    gps_widget->setWindowTitle("GpsWidget");

    QObject::connect(
        controller, &GpsController::gpsUpdated,
        [&](const GpsData &data) { logger::saveGpsDataToLogFile(data); });
    QObject::connect(controller_thread, &QThread::started, controller,
                     &GpsController::initObjects);
    QObject::connect(controller, &GpsController::controller_started,
                     [controller]() { controller->startCOM(2); });

    gps_widget->show();

    controller_thread->start();
    return app.exec();
}
