#include "diagnostics_manager.h"
#include "correlation_engine.h"
#include <cstdio>

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
    processCorrelation(latest, now);
    updateAlerts(now);
    emitEvents();

    healthScore = HealthScoring::calculate(latest);
}

void DiagnosticsManager::processTrends() {

    VehicleSnapshot ordered[HistoryBuffer::SIZE];
    history.toOrdered(ordered);

    // future expansion point:
    // TrendAnalyzer operates ONLY on ordered data
    TrendAnalyzer::analyze(ordered, history.size());
}

void DiagnosticsManager::processAnomalies() {

    if (AnomalyDetector::coolantOverheat(latest.coolantTemp)) {
        Alert alert;
        if (RuleEngine::evaluateCooling(latest, alert)) {
            alert.id = makeId(latest, alert.type);
            alert.firstSeen = latest.timestamp;
            alert.lastSeen = latest.timestamp;
            alertManager.raise(alert);
        }
    }

    if (AnomalyDetector::rpmOutlier(latest.rpm)) {
        Alert alert;
        if (RuleEngine::evaluateEngine(latest, alert)) {
            alert.id = makeId(latest, alert.type);
            alert.firstSeen = latest.timestamp;
            alert.lastSeen = latest.timestamp;
            alertManager.raise(alert);
        }
    }
}

void DiagnosticsManager::processCorrelation(
    const VehicleSnapshot& s,
    uint32_t now
) {
    VehicleSnapshot ordered[HistoryBuffer::SIZE];
    history.toOrdered(ordered);

    auto result = CorrelationEngine::analyze(s, ordered, history.size());

    if (result.cause != RootCause::NONE) {
        Alert alert;
        alert.type = AlertType::ENGINE;
        alert.severity = AlertSeverity::HIGH;
        alert.state = AlertState::NEW;
        alert.firstSeen = now;
        alert.lastSeen = now;

        snprintf(alert.message, MAX_ALERT_MESSAGE_LENGTH,
            "Correlated %u with confidence %u",
            (uint8_t)result.cause, result.confidence);

        if (result.severityBoost >= 20) {
            alert.severity = AlertSeverity::CRITICAL;
        }

        alert.id = makeId(s, alert.type);
        alertManager.raise(alert);
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