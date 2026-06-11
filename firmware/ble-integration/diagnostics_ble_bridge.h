#pragma once

#include <stdint.h>
#include "diagnostics/diagnostics_manager.h"
#include "../esp32-ble/gatt_profile.h"

namespace ssm {
class BLEServerManager;
}

namespace ble {

class DiagnosticsBLEBridge {
public:
    void begin(diagnostics::DiagnosticsManager* dm, ssm::BLEServerManager* server);

    void tick(); // called from main loop or RTOS task

private:
    diagnostics::DiagnosticsManager* diag = nullptr;
    ssm::BLEServerManager* server = nullptr;

    uint32_t lastSnapshotPush = 0;
    uint32_t lastHealthPush = 0;
    uint32_t lastAlertStatePush = 0;

    void pushSnapshot();
    void pushHealth();
    void pushAlerts();
};

}