#pragma once

#include <stdint.h>
#include "diagnostics_types.h"
#include "diagnostics_history.h"
#include "health_scoring.h"
#include "rule_engine.h"
#include "anomaly_detector.h"
#include "trend_analyzer.h"
#include "correlation_engine.h"
#include "alert_engine.h"
#include "alerts.h"
#include "alert_event_stream.h"

#include "../obd-reader/obd_manager.h"

namespace diagnostics {

class DiagnosticsManager {
public:
    void begin();

    // called at fixed interval (e.g. 1Hz or 2Hz)
    void tick(const obd::VehicleSnapshot& snap);

    uint8_t getHealthScore() const;

    const AlertManager& getAlertManager() const;
    const AlertEventStream& getEventStream() const;

private:
    VehicleSnapshot latest{};
    HistoryBuffer history;

    AlertManager alertManager;
    AlertEventStream eventStream;

    AlertStateMeta alertMeta[16]; // per-alert tracking (no heap)

    uint8_t healthScore = 100;

    void processRules(const VehicleSnapshot& s, uint32_t now);
    void processTrends();
    void processAnomalies();
    void processCorrelation(const VehicleSnapshot& s, uint32_t now);
    void updateAlerts(uint32_t now);
    void emitEvents();
};

}