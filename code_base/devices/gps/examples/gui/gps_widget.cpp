#include "gps_widget.h"

#include "QVBoxLayout"
#include "gps_data.h"

GpsWidget::GpsWidget(QWidget *parent) : QWidget(parent) {
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

void GpsWidget::showGpsData(GpsData data) {
    status->appendPlainText(data.toArseniyString());
};
void GpsWidget::showGpsStatus(const QString &msg) {
    if (status) {
        status->appendPlainText(msg);
    }
};

QPlainTextEdit *status;
