#include <QApplication>
#include <QCloseEvent>
#include <QObject>
#include <QStatusBar>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include "QDebug"
#include "QThread"
#include "controller.h"
#include "gps_data.h"

class GpsWidget : public QWidget {
public:
    GpsWidget(QObject *parent = nullptr) {
        status = new QStatusBar;
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(status);
        setLayout(layout);
        showGpsStatus("Gps status: ");
    }

public slots:
    void showGpsData(GpsData) { qDebug() << "update gps data in Gui thread"; };
    void showGpsStatus(const QString &msg) {
        if (status) {
            status->showMessage(msg);
        }
    };

private:
    QStatusBar *status;
};

int main(int argc, char *argv[]) {
    qRegisterMetaType<GpsData>("GpsData");

    QApplication app(argc, argv);
    QThread *controller_thread = new QThread;
    GpsController *controller = new GpsController(controller_thread);

    GpsWidget *gps_widget = new GpsWidget;
    gps_widget->setWindowTitle("GpsWidget");

    QObject::connect(controller_thread, &QThread::started, controller,
                     [controller]() { controller->startCOM(2); });
    QObject::connect(controller, &GpsController::gpsUpdated, gps_widget,
                     &GpsWidget::showGpsData);
    QObject::connect(controller, &GpsController::gpsStatus, gps_widget,
                     &GpsWidget::showGpsStatus);
    gps_widget->show();

    controller_thread->start();
    return app.exec();
}
