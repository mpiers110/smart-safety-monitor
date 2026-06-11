#include "alert_engine.h"

namespace diagnostics {

void AlertEngine::update(Alert& alert, AlertStateMeta& meta, uint32_t now) {

    uint32_t delta = now - meta.lastUpdate;

    if (delta > 0) {
        meta.persistence++;
        meta.severityScore += (uint8_t)alert.severity;
        meta.lastUpdate = now;
    }

    // ESCALATION LOGIC (NOT TIME ONLY)
    if (meta.persistence > 5 && meta.severityScore > 8) {
        alert.state = AlertState::ESCALATED;
    }

    if (meta.persistence > 10 && alert.severity == AlertSeverity::HIGH) {
        alert.state = AlertState::ESCALATED;
    }

    // stabilization fallback
    if (meta.persistence == 0) {
        alert.state = AlertState::ACTIVE;
    }
}

}