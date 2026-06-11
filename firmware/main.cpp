#include <Arduino.h>
#include "esp32-ble/ble_server.h"
#include "esp32-ble/ble_server_events.h"
#include "esp32-ble/ble_server_command_handler.h"
#include "ble-integration/diagnostics_ble_bridge.h"
#include "obd-reader/obd_manager.h"
#include "diagnostics/diagnostics_manager.h"

static obd::OBDManager obdManager;
static diagnostics::DiagnosticsManager diagnosticsManager;
static ssm::BLEServerManager bleServer;
static ssm::BLEServerEventHandler bleEventHandler;
static ssm::BLEServerCommandHandler bleCommandHandler(&diagnosticsManager, &bleServer);
static ble::DiagnosticsBLEBridge bleBridge;

void setup() {
    Serial.begin(115200);
    delay(1000);

    obdManager.begin();
    diagnosticsManager.begin();
    bleServer.begin("SmartSafetyMonitor");
    bleEventHandler.setCommandHandler(&bleCommandHandler);
    bleServer.setEventHandler(&bleEventHandler);
    bleBridge.begin(&diagnosticsManager, &bleServer);
}

void loop() {
    obdManager.update();
    auto snapshot = obdManager.getSnapshot();
    diagnosticsManager.tick(snapshot);

    if (bleEventHandler.shouldStream()) {
        bleBridge.tick();
    }

    bleServer.loop();
    delay(200);
}
