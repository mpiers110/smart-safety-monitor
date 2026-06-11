#include "rule_engine.h"
#include <string.h>

namespace diagnostics {

static void fill(
    Alert& a,
    AlertType t,
    AlertSeverity s,
    const char* msg
) {
    a.type = t;
    a.severity = s;
    a.state = AlertState::NEW;
    strncpy(a.message, msg, MAX_ALERT_MESSAGE_LENGTH - 1);
    a.message[MAX_ALERT_MESSAGE_LENGTH - 1] = '\0';
}

bool RuleEngine::evaluateBattery(const VehicleSnapshot& s, Alert& out) {
    if (s.voltageMv < 11500) {
        fill(out, AlertType::BATTERY, AlertSeverity::HIGH,
             "Battery voltage low");
        return true;
    }
    return false;
}

bool RuleEngine::evaluateCooling(const VehicleSnapshot& s, Alert& out) {
    if (s.coolantTemp > 110) {
        fill(out, AlertType::COOLING, AlertSeverity::CRITICAL,
             "Engine overheating");
        return true;
    }
    return false;
}

bool RuleEngine::evaluateEngine(const VehicleSnapshot& s, Alert& out) {
    if (s.rpm < 400 || s.rpm > 5000) {
        fill(out, AlertType::ENGINE, AlertSeverity::MEDIUM,
             "Engine RPM abnormal");
        return true;
    }
    return false;
}

bool RuleEngine::evaluateFuel(const VehicleSnapshot& s, Alert& out) {
    if (s.fuelTrim > 20 || s.fuelTrim < -20) {
        fill(out, AlertType::FUEL, AlertSeverity::MEDIUM,
             "Fuel trim unstable");
        return true;
    }
    return false;
}

}