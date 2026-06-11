#include "ecu_interface.h"

namespace obd {

static HardwareSerial ECU(2);

// -----------------------------
// Circular RX buffer (no heap)
// -----------------------------
static const int RX_BUF_SIZE = 128;
static uint8_t rxBuffer[RX_BUF_SIZE];
static int rxIndex = 0;

// -----------------------------
// Flush UART safely
// -----------------------------
static void flushUART() {
    while (ECU.available()) ECU.read();
}

// -----------------------------
// Read raw bytes with timeout
// -----------------------------
static size_t readResponse(uint8_t* out, size_t maxLen, uint32_t timeoutMs) {
    uint32_t start = millis();
    size_t len = 0;

    while (millis() - start < timeoutMs && len < maxLen) {
        while (ECU.available() && len < maxLen) {
            out[len++] = ECU.read();
        }
    }

    return len;
}

// -----------------------------
// Basic noise filter (CRC-like sanity check)
// -----------------------------
static bool validateFrame(uint8_t* data, size_t len) {
    if (len < 3) return false;

    // reject garbage spikes (common in OBD UART noise)
    uint8_t validCount = 0;

    for (size_t i = 0; i < len; i++) {
        if (data[i] >= 0x30 && data[i] <= 0x7A) validCount++;
    }

    return validCount > (len / 2);
}

// -----------------------------
void begin() {
    ECU.begin(38400);
    delay(1000);

    const char* initCmds[] = {
        "ATZ",
        "ATE0",
        "ATL0",
        "ATS0",
        "ATH0",
        "ATSP0"
    };

    for (auto cmd : initCmds) {
        ECU.print(cmd);
        ECU.print("\r");
        delay(200);
        flushUART();
    }
}

// -----------------------------
// Send PID request (binary-safe)
// -----------------------------
bool sendPID(uint8_t mode, uint8_t pid, uint8_t* out, size_t& len) {
    char cmd[10];
    snprintf(cmd, sizeof(cmd), "%02X %02X", mode, pid);

    return sendRaw(cmd, out, len);
}

// -----------------------------
// Core request engine (NO STRING)
// -----------------------------
bool sendRaw(const char* cmd, uint8_t* out, size_t& len) {
    len = 0;

    flushUART();

    ECU.print(cmd);
    ECU.print("\r");

    uint8_t buffer[RX_BUF_SIZE];
    size_t received = readResponse(buffer, RX_BUF_SIZE, 250);

    if (!validateFrame(buffer, received)) {
        // retry once (important for ECU noise)
        delay(50);
        flushUART();

        ECU.print(cmd);
        ECU.print("\r");

        received = readResponse(buffer, RX_BUF_SIZE, 300);

        if (!validateFrame(buffer, received)) {
            return false;
        }
    }

    // copy clean output
    for (size_t i = 0; i < received && i < RX_BUF_SIZE; i++) {
        out[i] = buffer[i];
    }

    len = received;
    return true;
}

}