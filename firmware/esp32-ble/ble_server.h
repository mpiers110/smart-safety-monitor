#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

namespace ssm { // Smart Safety Monitor

// ==============================
// GATT UUIDs (keep stable)
// ==============================

#define SSM_SERVICE_UUID              "a12f0001-4b8f-4c3d-9a12-7f1a2b3c4d5e"

#define SSM_CHAR_LIVE_DATA_UUID       "a12f0002-4b8f-4c3d-9a12-7f1a2b3c4d5e"
#define SSM_CHAR_DIAGNOSTICS_UUID     "a12f0003-4b8f-4c3d-9a12-7f1a2b3c4d5e"
#define SSM_CHAR_COMMANDS_UUID        "a12f0004-4b8f-4c3d-9a12-7f1a2b3c4d5e"
#define SSM_CHAR_STATUS_UUID         "a12f0005-4b8f-4c3d-9a12-7f1a2b3c4d5e"

// ==============================
// Packet Types
// ==============================

enum class CommandType : uint8_t {
    START_STREAM = 0x01,
    STOP_STREAM  = 0x02,
    READ_DTC     = 0x03,
    CLEAR_DTC    = 0x04,
    GET_STATUS   = 0x05
};

struct CommandPacket {
    CommandType type;
    uint32_t payload;
};

// ==============================
// Callback Interface
// ==============================

class BLEServerEvents {
public:
    virtual void onConnect() {}
    virtual void onDisconnect() {}

    virtual void onCommand(const uint8_t* data, size_t len) {}
};

// ==============================
// BLE Server Class
// ==============================

class BLEServerManager : public BLEServerCallbacks, public BLECharacteristicCallbacks {
public:
    BLEServerManager();
    ~BLEServerManager();

    // lifecycle
    void begin(const std::string& deviceName);
    void loop();

    // streaming APIs
    void sendLiveData(const uint8_t* data, size_t len);
    void sendDiagnostics(const uint8_t* data, size_t len);
    void sendStatus(const uint8_t* data, size_t len);
    void sendActiveAlerts(const uint8_t* data, size_t len);

    bool isConnected() const;

    // event handler injection
    void setEventHandler(BLEServerEvents* handler);

protected:
    // BLE callbacks
    void onConnect(BLEServer* pServer) override;
    void onDisconnect(BLEServer* pServer) override;

    void onWrite(BLECharacteristic* pCharacteristic) override;

private:
    void setupServer();
    void setupServices();
    void setupCharacteristics();

    void startAdvertising();

private:
    BLEServer* pServer;

    BLEService* service;

    BLECharacteristic* liveDataChar;
    BLECharacteristic* diagnosticsChar;
    BLECharacteristic* commandChar;
    BLECharacteristic* statusChar;
    BLECharacteristic* alertsChar;

    bool deviceConnected;
    BLEServerEvents* eventHandler;
};

} // namespace ssm
