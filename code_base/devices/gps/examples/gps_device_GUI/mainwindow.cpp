#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    gps = new GPSDevice;
    GpsWidget *widget =
        gps->createWidget(this);  // создали виджет и теперь можно
                                  // использовать его в своем GUI
    ui->verticalLayout->addWidget(widget);
    gps->start();
    gps->writeAllToBinFile();
    gps->writeParcedToTextFile(logger::saveFormat::jsonIndented);
}

MainWindow::~MainWindow() {
    delete ui;
    delete gps;
}

void MainWindow::on_pushButton_start_clicked() { gps->start(); }

void MainWindow::on_pushButton_stop_clicked() { gps->stop(); }
