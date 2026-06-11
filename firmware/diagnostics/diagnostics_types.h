#pragma once

#include <Arduino.h>

namespace diagnostics {

constexpr uint8_t MAX_ALERT_MESSAGE_LENGTH = 128;

enum class AlertSeverity : uint8_t {
    INFO = 0,
    LOW,
    MEDIUM,
    HIGH,
    CRITICAL
};

enum class AlertType : uint8_t {
    BATTERY = 0,
    COOLING,
    ENGINE,
    FUEL,
    SENSOR,
    ELECTRICAL
};

enum class AlertState : uint8_t {
    NEW = 0,
    ACTIVE,
    ESCALATED,
    CLEARED
};

struct VehicleSnapshot {
    uint32_t timestamp;

    uint16_t rpm;
    uint16_t speed;

    uint16_t coolantTemp;      // °C

    uint16_t voltageMv;        // millivolts
    int16_t fuelTrim;          // signed %

};

struct Alert {
    uint32_t id;

    AlertType type;
    AlertSeverity severity;
    AlertState state;

    uint32_t firstSeen;
    uint32_t lastSeen;

    char message[MAX_ALERT_MESSAGE_LENGTH];
};

}