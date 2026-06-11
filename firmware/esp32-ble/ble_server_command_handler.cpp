#include "ble_server_command_handler.h"
#include "ble_server.h"
#include <Arduino.h>

namespace ssm {

BLEServerCommandHandler::BLEServerCommandHandler(
    diagnostics::DiagnosticsManager* diagnostics,
    BLEServerManager* server
) : diagnostics(diagnostics), server(server) {}

void BLEServerCommandHandler::handleCommand(const CommandPacket& packet) {
    switch (packet.type) {
        case CommandType::READ_DTC: {
            uint16_t dtcs[8];
            size_t count = obd::readDTC(dtcs, 8);
            uint8_t payload[16] = {0};
            size_t len = count * 2;
            for (size_t i = 0; i < count && i < 8; i++) {
                payload[i * 2] = (uint8_t)(dtcs[i] >> 8);
                payload[i * 2 + 1] = (uint8_t)(dtcs[i] & 0xFF);
            }
            if (server && len > 0) {
                server->sendDiagnostics(payload, len);
            }
            break;
        }

        case CommandType::CLEAR_DTC:
            obd::clearDTC();
            break;

        case CommandType::GET_STATUS: {
            uint8_t status[2];
            status[0] = diagnostics ? diagnostics->getHealthScore() : 0;
            status[1] = status[0] > 75 ? 0 : 1;
            if (server) {
                server->sendStatus(status, sizeof(status));
            }
            break;
        }

        case CommandType::START_STREAM:
        case CommandType::STOP_STREAM:
            break;
    }
}

}