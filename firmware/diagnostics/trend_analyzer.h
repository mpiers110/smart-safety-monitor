#pragma once
#include <stdint.h>
#include "diagnostics_types.h"

namespace diagnostics {

class TrendAnalyzer {
public:
    static float slope(const uint16_t* values, size_t count);

    static bool voltageDeclining(const uint16_t* mv, size_t count);
    static bool coolantRising(const uint16_t* temp, size_t count);
    static bool fuelTrimDrifting(const int16_t* trim, size_t count);
};

}