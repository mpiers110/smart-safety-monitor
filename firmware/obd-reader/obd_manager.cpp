#include "obd_manager.h"
#include "pid_reader.h"

namespace obd {

void OBDManager::begin() {}

void OBDManager::update() {
    snapshot.timestamp = millis();
    snapshot.rpm = readRPM();
    snapshot.speed = readSpeed();
    snapshot.coolantTemp = readCoolantTemp();
    snapshot.voltageMv = readVoltage();
    snapshot.fuelTrim = readFuelTrim();
    
}

VehicleSnapshot OBDManager::getSnapshot() const {
    return snapshot;
}

}