#pragma once
#include <Arduino.h>

namespace obd {

void begin();
bool sendPID(uint8_t mode, uint8_t pid, uint8_t* out, size_t& len);
bool sendRaw(const char* cmd, uint8_t* out, size_t& len);

}