#ifndef DEVICES_GPS_EXAMPLES_GPS_DEVICE_GUI_MAINWINDOW_H_
#define DEVICES_GPS_EXAMPLES_GPS_DEVICE_GUI_MAINWINDOW_H_

#include <QMainWindow>

#include "gps_device.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void pushButton_start_clicked();
    void pushButton_stop_clicked();

private:
    Ui::MainWindow *ui;
    GPSDevice *m_gps;
    QDockWidget *m_gps_dock;
    QPushButton *pushButton_start;
    QPushButton *pushButton_stop;
};

#endif  // DEVICES_GPS_EXAMPLES_GPS_DEVICE_GUI_MAINWINDOW_H_
