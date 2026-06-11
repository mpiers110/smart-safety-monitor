#include "trend_analyzer.h"

namespace diagnostics {

float TrendAnalyzer::slope(const uint16_t* values, size_t count) {
    if (count < 2) return 0.0f;

    float xMean = (count - 1) * 0.5f;
    float yMean = 0;

    for (size_t i = 0; i < count; i++) {
        yMean += values[i];
    }
    yMean /= count;

    float num = 0;
    float den = 0;

    for (size_t i = 0; i < count; i++) {
        float dx = (float)i - xMean;
        float dy = values[i] - yMean;

        num += dx * dy;
        den += dx * dx;
    }

    return (den == 0.0f) ? 0.0f : (num / den);
}

bool TrendAnalyzer::voltageDeclining(const uint16_t* mv, size_t count) {
    return slope(mv, count) < -5.0f; // mV per sample threshold
}

bool TrendAnalyzer::coolantRising(const uint16_t* temp, size_t count) {
    return slope(temp, count) > 1.0f;
}

bool TrendAnalyzer::fuelTrimDrifting(const int16_t* trim, size_t count) {
    if (count < 3) return false;

    int32_t sum = 0;
    for (size_t i = 0; i < count; i++) {
        sum += trim[i];
    }

    int16_t avg = sum / (int32_t)count;

    return (avg > 8 || avg < -8);
}

}