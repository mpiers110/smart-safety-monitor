#pragma once

namespace obd {

enum PID : uint8_t {
    RPM            = 0x0C,
    SPEED          = 0x0D,
    COOLANT_TEMP   = 0x05,
    FUEL_TRIM      = 0x06,
    MAF            = 0x10,
    THROTTLE       = 0x11,
    VOLTAGE        = 0x42
};

}