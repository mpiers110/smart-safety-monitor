#pragma once
#include <Arduino.h>

namespace obd {

uint16_t readRPM();
uint16_t readSpeed();
uint16_t readCoolantTemp();
uint16_t readVoltage();
uint16_t readFuelTrim();

}