#pragma once
#include <stdint.h>

namespace diagnostics {

class AnomalyDetector {
public:
    static bool voltageSpike(uint16_t currentMv, uint16_t baselineMv);
    static bool coolantOverheat(uint16_t temp);
    static bool rpmOutlier(uint16_t rpm);
    static bool fuelTrimAnomaly(int16_t trim);
};

}