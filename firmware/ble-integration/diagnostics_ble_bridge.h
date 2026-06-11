#pragma once

#include <stdint.h>
#include "diagnostics/diagnostics_manager.h"
#include "../esp32-ble/gatt_profile.h"

namespace ble {

class DiagnosticsBLEBridge {
public:
    void begin(diagnostics::DiagnosticsManager* dm);

    void tick(); // called from main loop or RTOS task

private:
    diagnostics::DiagnosticsManager* diag = nullptr;

    uint32_t lastSnapshotPush = 0;
    uint32_t lastHealthPush = 0;

    void pushSnapshot();
    void pushHealth();
    void pushAlerts();
};

}