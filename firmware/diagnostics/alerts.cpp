#include "alerts.h"
#include <string.h>

namespace diagnostics {

bool AlertManager::raise(const Alert& alert) {

    // deduplicate by type
    for (uint8_t i = 0; i < size; i++) {
        if (alerts[i].type == alert.type &&
            alerts[i].state != AlertState::CLEARED) {

            alerts[i].lastSeen = alert.lastSeen;
            alerts[i].state = AlertState::ACTIVE;
            return true;
        }
    }

    if (size >= MAX_ALERTS) return false;

    alerts[size] = alert;
    alerts[size].state = AlertState::NEW;

    size++;
    return true;
}

void AlertManager::clear(uint32_t id) {
    for (uint8_t i = 0; i < size; i++) {
        if (alerts[i].id == id) {
            alerts[i].state = AlertState::CLEARED;
        }
    }
}

const Alert* AlertManager::getAll() const {
    return alerts;
}

uint8_t AlertManager::count() const {
    return size;
}

void AlertManager::updateState(uint32_t timestamp) {

    for (uint8_t i = 0; i < size; i++) {

        if (alerts[i].state == AlertState::NEW) {
            alerts[i].state = AlertState::ACTIVE;
        }

        // escalation logic
        if (alerts[i].state == AlertState::ACTIVE) {

            uint32_t age = timestamp - alerts[i].firstSeen;

            if (age > 60000 && alerts[i].severity == AlertSeverity::MEDIUM) {
                alerts[i].state = AlertState::ESCALATED;
            }
        }
    }
}

}