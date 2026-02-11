#include <QApplication>

#include "gps_widget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    GpsWidget gpsWidget("COM1");
    gpsWidget.show();
    app.exec();
}
