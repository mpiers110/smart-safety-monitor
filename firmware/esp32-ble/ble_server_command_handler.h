#pragma once

#include "ble_server.h"
#include "../obd-reader/dtc_reader.h"
#include "../diagnostics/diagnostics_manager.h"

namespace ssm {

class BLEServerManager;

class BLEServerCommandHandler {
public:
    BLEServerCommandHandler(
        diagnostics::DiagnosticsManager* diagnostics,
        BLEServerManager* server
    );

    void handleCommand(const CommandPacket& packet);

private:
    diagnostics::DiagnosticsManager* diagnostics;
    BLEServerManager* server;
};

}