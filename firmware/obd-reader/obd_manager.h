#pragma once

namespace obd {

struct VehicleSnapshot {
    uint16_t rpm;
    uint16_t speed;
    uint16_t coolantTemp;
    uint16_t voltage;
    uint16_t fuelTrim;
};

class OBDManager {
public:
    void begin();
    void update();

    VehicleSnapshot getSnapshot() const;

private:
    VehicleSnapshot snapshot;
};

}