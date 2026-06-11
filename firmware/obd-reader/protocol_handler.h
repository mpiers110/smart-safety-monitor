#pragma once
#include <Arduino.h>

namespace obd {

enum class Protocol : uint8_t {
    UNKNOWN,
    CAN_11BIT,
    CAN_29BIT,
    ISO9141,
    KWP2000,
    KLINE
};

class ProtocolHandler {
public:
    void begin();
    Protocol detect();

    const char* toATCommand(Protocol p);

private:
    Protocol current = Protocol::UNKNOWN;
};

}