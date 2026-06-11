#include <Arduino.h>

namespace ssm {

struct CommandPacket {
    uint8_t type;
    uint32_t payload;
};

size_t decodeCommand(const uint8_t* in, size_t len, CommandPacket& out) {
    if (len < 5) return 0;

    out.type = in[0];
    memcpy(&out.payload, in + 1, sizeof(out.payload));

    return 5;
}

}
