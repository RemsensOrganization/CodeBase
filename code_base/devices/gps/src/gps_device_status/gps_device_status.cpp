#include "gps_device_status.h"

QString toString(GpsStatus status) {
    switch (status) {
        case GpsStatus::OFFLINE:
            return "Offline";
        case GpsStatus::IDLE:
            return "Idle";
        case GpsStatus::ACTIVE:
            return "Active";
        case GpsStatus::SEARCHING:
            return "Searching";
        case GpsStatus::DATA_ERROR:
            return "Error";
        default:
            return "Unknown";
    }
}
