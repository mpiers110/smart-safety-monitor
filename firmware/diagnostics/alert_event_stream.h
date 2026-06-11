#pragma once
#include "diagnostics_types.h"

namespace diagnostics {

struct AlertEvent {
    uint32_t id;
    AlertState state;
};

class AlertEventStream {
public:
    void push(const Alert& alert);

    uint8_t getEvents(AlertEvent* out, uint8_t max);

    void clear();

private:
    static const uint8_t MAX_EVENTS = 16;

    AlertEvent events[MAX_EVENTS];
    uint8_t size = 0;

    bool isDuplicate(const AlertEvent& e);
};

}