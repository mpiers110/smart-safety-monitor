#include <Arduino.h>

namespace ssm {

struct TelemetryPacket {
    uint32_t timestamp;
    uint16_t rpm;
    uint16_t speed;
    uint16_t coolant_temp;
    uint16_t voltage;
    uint16_t fuel_trim;
};

size_t encodeTelemetry(const TelemetryPacket& in, uint8_t* out) {
    memcpy(out, &in.timestamp, sizeof(in.timestamp));
    memcpy(out + 4, &in.rpm, sizeof(in.rpm));
    memcpy(out + 6, &in.speed, sizeof(in.speed));
    memcpy(out + 8, &in.coolant_temp, sizeof(in.coolant_temp));
    memcpy(out + 10, &in.voltage, sizeof(in.voltage));
    memcpy(out + 12, &in.fuel_trim, sizeof(in.fuel_trim));

    return 14;
}

}
