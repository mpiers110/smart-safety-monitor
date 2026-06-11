#include "snapshot_store.h"
#include "flash_storage.h"

namespace storage {

static const char* KEY = "snapshots";

bool SnapshotStore::save(const VehicleSnapshot& snapshot) {
    // read existing
    VehicleSnapshot buffer[50];
    size_t count = FlashStorage::readBlob(KEY, (uint8_t*)buffer, sizeof(buffer)) / sizeof(VehicleSnapshot);

    if (count >= 50) {
        // shift left (keep latest 50)
        for (int i = 1; i < 50; i++) {
            buffer[i - 1] = buffer[i];
        }
        buffer[49] = snapshot;
        count = 50;
    } else {
        buffer[count++] = snapshot;
    }

    return FlashStorage::writeBlob(KEY, (uint8_t*)buffer, count * sizeof(VehicleSnapshot));
}

size_t SnapshotStore::load(VehicleSnapshot* out, size_t maxCount) {
    size_t bytes = FlashStorage::readBlob(KEY, (uint8_t*)out, maxCount * sizeof(VehicleSnapshot));
    return bytes / sizeof(VehicleSnapshot);
}

void SnapshotStore::clear() {
    FlashStorage::remove(KEY);
}

}