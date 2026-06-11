#pragma once

#include "ble_server.h"

namespace ssm {
class BLEServerCommandHandler;

class BLEServerEventHandler : public BLEServerEvents {
public:
    BLEServerEventHandler();

    void onConnect() override;
    void onDisconnect() override;
    void onCommand(const uint8_t* data, size_t len) override;

    bool shouldStream() const;
    void clearStreamRequest();
    void setCommandHandler(BLEServerCommandHandler* handler);

private:
    bool streamRequested = false;
    BLEServerCommandHandler* commandHandler = nullptr;
};

}