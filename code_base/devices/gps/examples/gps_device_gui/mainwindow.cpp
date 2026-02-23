#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    gps = new GPSDevice;

    auto *w1 = new GpsSimpleWidget(this);  // создали виджет и теперь можно
                                           // использовать его в своем GUI
    gps->attachView(w1);
    ui->verticalLayout->addWidget(w1);
    gps->start();
    gps->writeOriginGpsDataToFile();
    gps->writeFormattedGpsDataToFile(logger::saveFormat::jsonIndented);
}

MainWindow::~MainWindow() {
    delete ui;
    delete gps;
}

void MainWindow::on_pushButton_start_clicked() { gps->start(); }

void MainWindow::on_pushButton_stop_clicked() { gps->stop(); }
