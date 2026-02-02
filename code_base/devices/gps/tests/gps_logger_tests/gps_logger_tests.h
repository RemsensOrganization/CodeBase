#ifndef UNITTESTS_H
#define UNITTESTS_H

#include <QObject>
#include <QtTest>

class gps_logger_UnitTests : public QObject {
    Q_OBJECT

private slots:
    void gps_logger_save_to_file_Test();
};

#endif  // UNITTESTS_H
