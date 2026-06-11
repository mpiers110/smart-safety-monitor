#include "flash_storage.h"

namespace storage {

Preferences FlashStorage::prefs;

bool FlashStorage::begin() {
    return prefs.begin("ssm_storage", false);
}

bool FlashStorage::writeBlob(const char* key, const uint8_t* data, size_t len) {
    size_t written = prefs.putBytes(key, data, len);
    return written == len;
}

size_t FlashStorage::readBlob(const char* key, uint8_t* buffer, size_t maxLen) {
    return prefs.getBytes(key, buffer, maxLen);
}

bool FlashStorage::remove(const char* key) {
    return prefs.remove(key);
}

}