#include <Arduino.h>
#include "snapshot_store.h"

namespace storage {

class PersistenceManager {
public:
    void begin() {
        // nothing required for NVS-based system
    }

    void store(const VehicleSnapshot& snapshot) {
        SnapshotStore::save(snapshot);
    }

    size_t retrieve(VehicleSnapshot* out, size_t maxCount) {
        return SnapshotStore::load(out, maxCount);
    }

    void clear() {
        SnapshotStore::clear();
    }
};

}