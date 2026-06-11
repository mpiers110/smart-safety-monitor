#pragma once
#include <stdint.h>
#include <stddef.h>

namespace obd {

size_t readDTC(uint16_t* out, size_t maxCount);
void clearDTC();

}