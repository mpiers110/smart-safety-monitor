#include "obd_manager.h"
#include "pid_reader.h"

namespace obd {

void OBDManager::begin() {}

void OBDManager::update() {
    snapshot.rpm         = readRPM();
    snapshot.speed       = readSpeed();
    snapshot.coolantTemp = readCoolantTemp();
    snapshot.voltage     = readVoltage();
    snapshot.fuelTrim    = readFuelTrim();
}

VehicleSnapshot OBDManager::getSnapshot() const {
    return snapshot;
}

}