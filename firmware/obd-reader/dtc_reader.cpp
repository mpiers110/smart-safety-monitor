#include "ecu_interface.h"

namespace obd {

size_t readDTC(uint16_t* out, size_t maxCount) {
    uint8_t raw[128];
    size_t len = 0;

    if (!sendRaw("03", raw, len)) {
        return 0;
    }

    size_t count = 0;

    for (size_t i = 0; i + 4 < len && count < maxCount; i += 4) {
        uint16_t code = (raw[i] << 8) | raw[i + 1];

        if (code == 0) break;

        out[count++] = code;
    }

    return count;
}

void clearDTC() {
    uint8_t dummy[16];
    size_t len;
    sendRaw("04", dummy, len);
}

}