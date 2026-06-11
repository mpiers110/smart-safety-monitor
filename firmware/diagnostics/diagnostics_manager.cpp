#include "diagnostics_manager.h"

namespace diagnostics {

static uint32_t makeId(const VehicleSnapshot& s, AlertType type) {
    return ((uint32_t)type << 24) ^ s.timestamp;
}

void DiagnosticsManager::begin() {
    healthScore = 100;

    for (uint8_t i = 0; i < 16; i++) {
        alertMeta[i] = {};
    }
}

void DiagnosticsManager::tick(const obd::VehicleSnapshot& snap) {

    latest = {
        snap.timestamp,
        snap.rpm,
        snap.speed,
        snap.coolantTemp,
        snap.voltageMv,
        snap.fuelTrim
    };

    history.push(latest);

    uint32_t now = snap.timestamp;

    processTrends();
    processAnomalies();
    processRules(latest, now);
    updateAlerts(now);
    emitEvents();

    healthScore = HealthScoring::calculate(latest);
}

void DiagnosticsManager::processTrends() {

    VehicleSnapshot ordered[HistoryBuffer::SIZE];
    history.toOrdered(ordered);

    // future expansion point:
    // TrendAnalyzer operates ONLY on ordered data
}

void DiagnosticsManager::processAnomalies() {

    // instantaneous checks (no history dependency)

    if (AnomalyDetector::coolantOverheat(latest.coolantTemp)) {
        // handled in rules layer
    }

    if (AnomalyDetector::rpmOutlier(latest.rpm)) {
        // handled in rules layer
    }
}

void DiagnosticsManager::processRules(
    const VehicleSnapshot& s,
    uint32_t now
) {
    Alert alert;

    bool triggered = false;

    if (RuleEngine::evaluateBattery(s, alert)) triggered = true;
    else if (RuleEngine::evaluateCooling(s, alert)) triggered = true;
    else if (RuleEngine::evaluateEngine(s, alert)) triggered = true;
    else if (RuleEngine::evaluateFuel(s, alert)) triggered = true;

    if (triggered) {
        alert.id = makeId(s, alert.type);
        alert.firstSeen = now;
        alert.lastSeen = now;

        alertManager.raise(alert);
    }
}

void DiagnosticsManager::updateAlerts(uint32_t now) {

    const Alert* alerts = alertManager.getAll();
    uint8_t count = alertManager.count();

    for (uint8_t i = 0; i < count; i++) {

        Alert& a = const_cast<Alert&>(alerts[i]);

        uint8_t metaIndex = (uint8_t)a.type;

        AlertEngine::update(a, alertMeta[metaIndex], now);
    }

    alertManager.updateState(now);
}

void DiagnosticsManager::emitEvents() {

    const Alert* alerts = alertManager.getAll();
    uint8_t count = alertManager.count();

    for (uint8_t i = 0; i < count; i++) {

        eventStream.push(alerts[i]);
    }
}

uint8_t DiagnosticsManager::getHealthScore() const {
    return healthScore;
}

const AlertManager& DiagnosticsManager::getAlertManager() const {
    return alertManager;
}

const AlertEventStream& DiagnosticsManager::getEventStream() const {
    return eventStream;
}

}