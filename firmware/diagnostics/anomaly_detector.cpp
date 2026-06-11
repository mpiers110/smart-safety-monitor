#include "anomaly_detector.h"

namespace diagnostics {

bool AnomalyDetector::voltageSpike(uint16_t currentMv, uint16_t baselineMv) {
    if (baselineMv == 0) return false;

    int diff = (int)baselineMv - (int)currentMv;

    return diff > 800 || diff < -800; // 0.8V deviation
}

bool AnomalyDetector::coolantOverheat(uint16_t temp) {
    return temp >= 110;
}

bool AnomalyDetector::rpmOutlier(uint16_t rpm) {
    return rpm < 400 || rpm > 5000;
}

bool AnomalyDetector::fuelTrimAnomaly(int16_t trim) {
    return trim > 20 || trim < -20;
}

}