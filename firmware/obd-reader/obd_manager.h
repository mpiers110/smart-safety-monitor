#pragma once

#include <stdint.h>

namespace obd {

struct VehicleSnapshot {
    uint32_t timestamp;

    uint16_t rpm;
    uint16_t speed;

    uint16_t coolantTemp;

    uint16_t voltageMv;
    int16_t fuelTrim;
};

class OBDManager {
public:
    void begin();
    void update();

    VehicleSnapshot getSnapshot() const;

private:
    VehicleSnapshot snapshot{};
};

}