#pragma once

#include <Arduino.h>

namespace storage {

struct VehicleSnapshot {
    uint32_t timestamp;

    uint16_t rpm;
    uint16_t speed;

    uint16_t coolantTemp;

    uint16_t voltageMv;
    int16_t fuelTrim;
};

class SnapshotStore {
public:
    static bool save(const VehicleSnapshot& snapshot);

    static size_t load(
        VehicleSnapshot* out,
        size_t maxCount
    );

    static void clear();
};

}