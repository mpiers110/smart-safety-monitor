#include "ecu_interface.h"
#include "pid_definitions.h"

namespace obd {

// -----------------------------
// Fast HEX parser (no String)
// -----------------------------
static uint8_t hex2byte(uint8_t c1, uint8_t c2) {
    auto v = [](uint8_t c) -> uint8_t {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        return 0;
    };

    return (v(c1) << 4) | v(c2);
}

// -----------------------------
// Extract PID response safely
// -----------------------------
static bool findPID(uint8_t* data, size_t len, uint8_t pid, uint8_t& A, uint8_t& B) {
    for (size_t i = 0; i + 4 < len; i++) {
        if (data[i] == '4' && data[i + 1] == '1') {
            if (hex2byte(data[i + 3], data[i + 4]) == pid) {
                A = hex2byte(data[i + 6], data[i + 7]);
                B = hex2byte(data[i + 9], data[i + 10]);
                return true;
            }
        }
    }
    return false;
}

// -----------------------------
uint16_t readPID(uint8_t pid) {
    uint8_t raw[128];
    size_t len = 0;

    if (!sendPID(0x01, pid, raw, len)) {
        return 0;
    }

    uint8_t A = 0, B = 0;

    if (!findPID(raw, len, pid, A, B)) {
        return 0;
    }

    switch (pid) {
        case RPM:
            return ((A * 256) + B) / 4;

        case SPEED:
            return A;

        case COOLANT_TEMP:
            return A - 40;

        case VOLTAGE:
            return (A * 256 + B) / 1000;

        case FUEL_TRIM:
            return A - 128;

        default:
            return A;
    }
}

uint16_t readRPM()         { return readPID(RPM); }
uint16_t readSpeed()       { return readPID(SPEED); }
uint16_t readCoolantTemp() { return readPID(COOLANT_TEMP); }
uint16_t readVoltage()     { return readPID(VOLTAGE); }
uint16_t readFuelTrim()    { return readPID(FUEL_TRIM); }

}