#include "mainwindow.h"

#include "gps_simple_widget.h"
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
    m_gps_dock->setMaximumSize(QSize(200, 300));

    m_gps = new GPSDevice;
    // создаем виджет для GPS и теперь можно использовать его в своем GUI.
    // в данном примере виджет дополнительно помещен в QDockWidget
    auto *w1 = new GpsSimpleWidget(m_gps_dock);
    m_gps->attachView(w1);
    m_gps->start("/tmp/gpsCOM");
    m_gps->writeOriginGpsDataToFile();
    m_gps->writeFormattedGpsDataToFile(logger::saveFormat::jsonIndented);

    // настройки доквиджета и палитры виджета
    QPalette pal = w1->palette();
    pal.setColor(QPalette::Window, QColor("#101418"));
    pal.setColor(QPalette::WindowText, QColor("#4FFFE1"));
    w1->setPalette(pal);
    w1->setAutoFillBackground(true);
    m_gps_dock->setWidget(w1);
    addDockWidget(Qt::LeftDockWidgetArea, m_gps_dock);

    // коннекты, в которых остановка и запуск GPS
    connect(pushButton_start, &QPushButton::clicked, this,
            &MainWindow::pushButton_start_clicked);
    connect(pushButton_stop, &QPushButton::clicked, this,
            &MainWindow::pushButton_stop_clicked);
}

MainWindow::~MainWindow() {
    delete ui;
    delete m_gps;
}

void MainWindow::pushButton_start_clicked() { m_gps->start(); }

void MainWindow::pushButton_stop_clicked() { m_gps->stop(); }
