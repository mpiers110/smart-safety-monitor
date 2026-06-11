#include "correlation_engine.h"

namespace diagnostics {

static uint16_t avgVoltage(
    const VehicleSnapshot* h,
    uint8_t size
) {
    if (size == 0) return 0;

    uint32_t sum = 0;

    for (uint8_t i = 0; i < size; i++) {
        sum += h[i].voltageMv;
    }

    return sum / size;
}

CorrelationResult CorrelationEngine::analyze(
    const VehicleSnapshot& s,
    const VehicleSnapshot* history,
    uint8_t historySize
) {
    CorrelationResult result{};
    result.cause = RootCause::NONE;
    result.confidence = 0;
    result.severityBoost = 0;

    uint16_t avgV = avgVoltage(history, historySize);

    bool voltageDropping =
        (avgV > 0 && s.voltageMv + 500 < avgV);

    bool rpmInstability =
        (s.rpm < 500 || s.rpm > 4500);

    bool coolantStress =
        (s.coolantTemp > 105);

    bool fuelInstability =
        (s.fuelTrim > 15 || s.fuelTrim < -15);

    // =========================
    // ALTERNATOR FAILURE PATTERN
    // =========================
    if (voltageDropping && rpmInstability) {
        result.cause = RootCause::ALTERNATOR_FAULT;
        result.confidence = 80;
        result.severityBoost = 25;
        return result;
    }

    // =========================
    // BATTERY WEAK PATTERN
    // =========================
    if (voltageDropping && !rpmInstability) {
        result.cause = RootCause::BATTERY_WEAK;
        result.confidence = 75;
        result.severityBoost = 20;
        return result;
    }

    // =========================
    // COOLING SYSTEM FAILURE
    // =========================
    if (coolantStress) {
        result.cause = RootCause::COOLING_SYSTEM_FAULT;
        result.confidence = 70;
        result.severityBoost = 15;
        return result;
    }

    // =========================
    // FUEL SYSTEM ISSUE
    // =========================
    if (fuelInstability) {
        result.cause = RootCause::FUEL_SYSTEM_FAULT;
        result.confidence = 65;
        result.severityBoost = 10;
        return result;
    }

    return result;
}

}