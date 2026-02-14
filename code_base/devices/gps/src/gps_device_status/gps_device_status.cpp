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
        case GpsStatus::FIX2D:
            return "Fix2D";
        case GpsStatus::FIX3D:
            return "Fix3D";
        case GpsStatus::DGPS:
            return "DGPS";
        case GpsStatus::RTK:
            return "RTK";
        case GpsStatus::ERROR:
            return "Error";
        default:
            return "Unknown";
    }
}
