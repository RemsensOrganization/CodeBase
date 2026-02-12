#include "gps_logger_tests.h"

#include <QDebug>

#include "gps_data.h"
#include "gps_logger.cpp"

void gps_logger_UnitTests ::gps_logger_save_to_file_Test() {
    GpsData data;
    data.latitude = 53.4;
    data.longitude = 27.6;
    data.isGpsDataValid = true;
    for (int i = 0; i < 5; ++i) {
        logger::saveGpsDataToFile(data);
    }
}

QTEST_MAIN(gps_logger_UnitTests)
