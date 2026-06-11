#pragma once
#include "diagnostics_types.h"

namespace diagnostics {

class RuleEngine {
public:
    static bool evaluateBattery(const VehicleSnapshot& s, Alert& out);
    static bool evaluateCooling(const VehicleSnapshot& s, Alert& out);
    static bool evaluateEngine(const VehicleSnapshot& s, Alert& out);
    static bool evaluateFuel(const VehicleSnapshot& s, Alert& out);
};

}