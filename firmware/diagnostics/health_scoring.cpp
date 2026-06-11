#include "health_scoring.h"

namespace diagnostics {

uint8_t HealthScoring::calculate(const VehicleSnapshot& s) {
    int score = 100;

    // voltage health
    if (s.voltageMv < 12000) score -= 20;
    if (s.voltageMv < 11000) score -= 30;

    // coolant stress
    if (s.coolantTemp > 95) score -= 10;
    if (s.coolantTemp > 110) score -= 25;

    // fuel system instability
    if (s.fuelTrim > 15 || s.fuelTrim < -15) score -= 15;

    // engine stress
    if (s.rpm < 500 || s.rpm > 4500) score -= 10;

    if (score < 0) score = 0;
    return (uint8_t)score;
}

const char* HealthScoring::category(uint8_t score) {
    if (score <= 25) return "CRITICAL";
    if (score <= 50) return "HIGH_RISK";
    if (score <= 75) return "WARNING";
    return "HEALTHY";
}

}