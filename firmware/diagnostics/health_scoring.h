#pragma once
#include <stdint.h>
#include "diagnostics_types.h"

namespace diagnostics {

class HealthScoring {
public:
    static uint8_t calculate(const VehicleSnapshot& s);

    static const char* category(uint8_t score);
};

}