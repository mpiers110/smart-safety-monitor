#pragma once
#include <Arduino.h>
#include <Preferences.h>

namespace storage {

class FlashStorage {
public:
    static bool begin();

    static bool writeBlob(const char* key, const uint8_t* data, size_t len);
    static size_t readBlob(const char* key, uint8_t* buffer, size_t maxLen);

    static bool remove(const char* key);

private:
    static Preferences prefs;
};

}