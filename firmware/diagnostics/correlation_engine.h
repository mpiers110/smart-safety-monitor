#pragma once

#include <stdint.h>
#include "diagnostics_types.h"

namespace diagnostics {

enum class RootCause : uint8_t {
    NONE = 0,
    BATTERY_WEAK,
    ALTERNATOR_FAULT,
    COOLING_SYSTEM_FAULT,
    FUEL_SYSTEM_FAULT,
    SENSOR_DEGRADATION,
    IGNITION_INSTABILITY
};

struct CorrelationResult {
    RootCause cause;
    uint8_t confidence;   // 0–100
    uint8_t severityBoost; // how much to escalate alerts
};

class CorrelationEngine {
public:
    static CorrelationResult analyze(
        const VehicleSnapshot& s,
        const VehicleSnapshot* history,
        uint8_t historySize
    );
};

}