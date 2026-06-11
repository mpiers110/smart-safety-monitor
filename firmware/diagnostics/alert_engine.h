#pragma once
#include "diagnostics_types.h"

namespace diagnostics {

struct AlertStateMeta {
    uint8_t persistence = 0;     // how long condition exists
    uint8_t severityScore = 0;   // weighted severity accumulation
    uint32_t lastUpdate = 0;
};

class AlertEngine {
public:
    static void update(Alert& alert, AlertStateMeta& meta, uint32_t now);
};

}