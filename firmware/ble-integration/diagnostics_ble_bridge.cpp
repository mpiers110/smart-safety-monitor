#include "diagnostics_ble_bridge.h"
#include "../esp32-ble/ble_server.h"

namespace ble {

void DiagnosticsBLEBridge::begin(diagnostics::DiagnosticsManager* dm) {
    diag = dm;
}

void DiagnosticsBLEBridge::tick() {

    if (!diag) return;

    uint32_t now = millis();

    // 1. snapshot stream (5Hz max)
    if (now - lastSnapshotPush > 200) {
        pushSnapshot();
        lastSnapshotPush = now;
    }

    // 2. health score stream (1Hz)
    if (now - lastHealthPush > 1000) {
        pushHealth();
        lastHealthPush = now;
    }

    // 3. alerts (event-driven but polled safely)
    pushAlerts();
}

void DiagnosticsBLEBridge::pushSnapshot() {

    auto alerts = diag->getAlertManager();

    // We assume BLE profile exists in esp32-ble stack:
    // Vehicle Status characteristic

    auto score = diag->getHealthScore();

    // compact payload (NO JSON, embedded-safe)
    uint8_t packet[12];

    // [0-3] health score + padding
    packet[0] = score;

    // BLE send via existing server
    BLEServer_sendCharacteristic(
        GATT_CHAR_VEHICLE_STATUS,
        packet,
        sizeof(packet)
    );
}

void DiagnosticsBLEBridge::pushHealth() {

    uint8_t score = diag->getHealthScore();

    uint8_t payload[2];
    payload[0] = score;
    payload[1] = score > 75 ? 0 : 1;

    BLEServer_sendCharacteristic(
        GATT_CHAR_HEALTH_SCORE,
        payload,
        sizeof(payload)
    );
}

void DiagnosticsBLEBridge::pushAlerts() {

    const auto& stream = diag->getEventStream();

    AlertEvent events[8];
    uint8_t count = stream.getEvents(events, 8);

    if (count == 0) return;

    uint8_t packet[32];
    uint8_t idx = 0;

    for (uint8_t i = 0; i < count && idx + 2 < sizeof(packet); i++) {
        packet[idx++] = (uint8_t)(events[i].id & 0xFF);
        packet[idx++] = (uint8_t)events[i].state;
    }

    BLEServer_sendCharacteristic(
        GATT_CHAR_ALERT_EVENTS,
        packet,
        idx
    );
}

}