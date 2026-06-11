#include "diagnostics_ble_bridge.h"
#include "../esp32-ble/ble_server.h"

namespace ble {

void DiagnosticsBLEBridge::begin(
    diagnostics::DiagnosticsManager* dm,
    ssm::BLEServerManager* server
) {
    diag = dm;
    this->server = server;
}

void DiagnosticsBLEBridge::tick() {

    if (!diag || !server) return;

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

    // 3. active alert state sync (2Hz)
    if (now - lastAlertStatePush > 500) {
        pushActiveAlerts();
        lastAlertStatePush = now;
    }

    // 4. alerts events (event-driven but polled safely)
    pushAlerts();
}

void DiagnosticsBLEBridge::pushSnapshot() {

    auto score = diag->getHealthScore();
    uint8_t packet[2];
    packet[0] = score;
    packet[1] = score > 75 ? 0 : 1;

    server->sendLiveData(packet, sizeof(packet));
}

void DiagnosticsBLEBridge::pushHealth() {

    uint8_t score = diag->getHealthScore();
    uint8_t payload[2];
    payload[0] = score;
    payload[1] = score > 75 ? 0 : 1;

    server->sendStatus(payload, sizeof(payload));
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

    server->sendDiagnostics(packet, idx);
}

void DiagnosticsBLEBridge::pushActiveAlerts() {
    const auto& alerts = diag->getAlertManager();
    const Alert* alertList = alerts.getAll();
    uint8_t count = alerts.count();

    if (count == 0) return;

    uint8_t packet[32];
    uint8_t idx = 0;

    packet[idx++] = count;

    for (uint8_t i = 0; i < count && idx + 7 <= sizeof(packet); i++) {
        const Alert& alert = alertList[i];
        if (alert.state == AlertState::CLEARED) continue;

        packet[idx++] = static_cast<uint8_t>(alert.type);
        packet[idx++] = static_cast<uint8_t>(alert.severity);
        packet[idx++] = static_cast<uint8_t>(alert.state);
        packet[idx++] = (uint8_t)(alert.id >> 24);
        packet[idx++] = (uint8_t)(alert.id >> 16);
        packet[idx++] = (uint8_t)(alert.id >> 8);
        packet[idx++] = (uint8_t)(alert.id & 0xFF);
    }

    server->sendActiveAlerts(packet, idx);
}

}