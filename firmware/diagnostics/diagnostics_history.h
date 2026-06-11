#pragma once
#include <stdint.h>
#include "diagnostics_types.h"

namespace diagnostics {

struct HistoryBuffer {
    static const uint8_t SIZE = 16;

    VehicleSnapshot data[SIZE];
    uint32_t timestamps[SIZE];

    uint8_t head = 0;
    uint8_t count = 0;

    void push(const VehicleSnapshot& s) {
        data[head] = s;
        timestamps[head] = s.timestamp;

        head = (head + 1) % SIZE;

        if (count < SIZE) count++;
    }

    uint8_t size() const { return count; }

    // ALWAYS chronological output (FIXES WRAP BUG)
    void toOrdered(VehicleSnapshot* out) const {
        for (uint8_t i = 0; i < count; i++) {
            uint8_t idx = (head + SIZE - count + i) % SIZE;
            out[i] = data[idx];
        }
    }
};

}