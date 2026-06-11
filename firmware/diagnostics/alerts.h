#pragma once
#include <stdint.h>
#include "diagnostics_types.h"

namespace diagnostics {

class AlertManager {
public:
    bool raise(const Alert& alert);

    void clear(uint32_t id);

    const Alert* getAll() const;
    uint8_t count() const;

    void updateState(uint32_t timestamp);

private:
    static const uint8_t MAX_ALERTS = 16;
    Alert alerts[MAX_ALERTS];
    uint8_t size = 0;
};

}