#include <QApplication>
#include <QCloseEvent>
#include <QObject>
#include <QPlainTextEdit>
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
    GpsWidget(QWidget *parent = nullptr) : QWidget(parent) {
        status = new QPlainTextEdit(this);
        status->setStyleSheet(
            "QPlainTextEdit {"
            " background-color: black;"
            " color: lime;"
            " font-family: 'Courier New';"
            " font-size: 12pt;"
            "}");

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(status);
        setLayout(layout);
    }

public slots:
    void showGpsData(GpsData data) {
        status->appendPlainText(data.toArseniyString());
    };
    void showGpsStatus(const QString &msg) {
        if (status) {
            status->appendPlainText(msg);
        }
    };

private:
    QPlainTextEdit *status;
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
    QObject::connect(controller, &GpsController::updateGpsData, gps_widget,
                     &GpsWidget::showGpsData);
    QObject::connect(controller, &GpsController::updateStatus, gps_widget,
                     &GpsWidget::showGpsStatus);

    QObject::connect(&app, &QApplication::aboutToQuit, [&]() {
        controller->stop();
        controller_thread->quit();
        controller_thread->wait();
        controller->deleteLater();
    });

    gps_widget->show();
    // mock gps data
    GpsData data;
    data.valid_gps_flag = true;
    data.latitude = 53.6;
    data.longitude = 27.3;
    // emulate gps data recieved
    emit controller->updateGpsData(data);

    controller_thread->start();
    return app.exec();
}
