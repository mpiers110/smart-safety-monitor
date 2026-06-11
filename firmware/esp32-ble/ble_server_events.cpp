#include "ble_server_events.h"
#include "ble_server_command_handler.h"
#include <cstring>

namespace ssm {

BLEServerEventHandler::BLEServerEventHandler()
    : streamRequested(false), commandHandler(nullptr) {}

void BLEServerEventHandler::onConnect() {
    streamRequested = true;
}

void BLEServerEventHandler::onDisconnect() {
    streamRequested = false;
}

void BLEServerEventHandler::onCommand(const uint8_t* data, size_t len) {
    if (len < 1) return;

    CommandPacket packet;
    packet.type = static_cast<CommandType>(data[0]);
    packet.payload = 0;

    if (len >= 5) {
        memcpy(&packet.payload, data + 1, 4);
    }

    switch (packet.type) {
        case CommandType::START_STREAM:
            streamRequested = true;
            break;

        case CommandType::STOP_STREAM:
            streamRequested = false;
            break;

        case CommandType::READ_DTC:
        case CommandType::CLEAR_DTC:
        case CommandType::GET_STATUS:
            if (commandHandler) {
                commandHandler->handleCommand(packet);
            }
            break;

        default:
            break;
    }
}

bool BLEServerEventHandler::shouldStream() const {
    return streamRequested;
}

void BLEServerEventHandler::clearStreamRequest() {
    streamRequested = false;
}

void BLEServerEventHandler::setCommandHandler(BLEServerCommandHandler* handler) {
    commandHandler = handler;
}

}