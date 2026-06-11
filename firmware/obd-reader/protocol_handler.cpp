#include "protocol_handler.h"

namespace obd {

void ProtocolHandler::begin() {
    current = detect();
}

Protocol ProtocolHandler::detect() {
    // In real ELM/STN this is automatic via ATSP0
    // We still keep abstraction for future direct CAN interface

    return Protocol::CAN_11BIT;
}

const char* ProtocolHandler::toATCommand(Protocol p) {
    switch (p) {
        case Protocol::CAN_11BIT: return "ATSP6";
        case Protocol::CAN_29BIT: return "ATSP7";
        case Protocol::ISO9141:   return "ATSP3";
        case Protocol::KWP2000:   return "ATSP4";
        case Protocol::KLINE:     return "ATSP5";
        default:                  return "ATSP0";
    }
}

}