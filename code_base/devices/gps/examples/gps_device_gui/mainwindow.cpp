#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      pushButton_start(new QPushButton("start")),
      pushButton_stop(new QPushButton("stop")) {
    ui->setupUi(this);
    ui->statusbar->addPermanentWidget(pushButton_start);
    ui->statusbar->addPermanentWidget(pushButton_stop);
    m_gps_dock = new QDockWidget("GpsWidget", this);

    m_gps_dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    m_gps = new GPSDevice;
    auto *w1 =
        new GpsSimpleWidget(m_gps_dock);  // создали виджет и теперь можно
                                          // использовать его в своем GUI
    w1->setObjectName("GpsWidget");
    m_gps->attachView(w1);
    m_gps_dock->setMaximumSize(QSize(200, 300));

    m_gps_dock->setWidget(w1);
    QPalette pal = w1->palette();
    pal.setColor(QPalette::Window, QColor("#101418"));
    pal.setColor(QPalette::WindowText, QColor("#4FFFE1"));
    w1->setPalette(pal);
    w1->setAutoFillBackground(true);
    addDockWidget(Qt::LeftDockWidgetArea, m_gps_dock);

    m_gps->start();
    m_gps->writeOriginGpsDataToFile();
    m_gps->writeFormattedGpsDataToFile(logger::saveFormat::jsonIndented);
    connect(pushButton_start, &QPushButton::clicked, this,
            &MainWindow::on_pushButton_start_clicked);
    connect(pushButton_stop, &QPushButton::clicked, this,
            &MainWindow::on_pushButton_stop_clicked);
}

MainWindow::~MainWindow() {
    delete ui;
    delete m_gps;
}

void MainWindow::on_pushButton_start_clicked() { m_gps->start(); }

void MainWindow::on_pushButton_stop_clicked() { m_gps->stop(); }
