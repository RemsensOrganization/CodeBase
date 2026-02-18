#ifndef DEVICES_GPS_TESTS_GPS_LOGGER_TESTS_GPS_LOGGER_TESTS_H_
#define DEVICES_GPS_TESTS_GPS_LOGGER_TESTS_GPS_LOGGER_TESTS_H_

#include <QObject>
#include <QtTest>

class gps_logger_UnitTests : public QObject {
    Q_OBJECT

private slots:
    void gps_logger_save_to_file_Test();
};

#endif  // DEVICES_GPS_TESTS_GPS_LOGGER_TESTS_GPS_LOGGER_TESTS_H_
