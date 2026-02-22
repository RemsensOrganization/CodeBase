#include <QApplication>
#include <QCommandLineParser>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow *window = new MainWindow;
    window->setAttribute(Qt::WA_DeleteOnClose, true);
    window->show();
    return app.exec();
}
