#include "ble_server.h"

namespace ssm {

BLEServerManager::BLEServerManager()
    : pServer(nullptr),
      service(nullptr),
      liveDataChar(nullptr),
      diagnosticsChar(nullptr),
      commandChar(nullptr),
      statusChar(nullptr),
      deviceConnected(false),
      eventHandler(nullptr) {}

BLEServerManager::~BLEServerManager() {}

void BLEServerManager::begin(const std::string& deviceName) {
    BLEDevice::init(deviceName);

    setupServer();
    setupServices();
    setupCharacteristics();

    startAdvertising();
}

void BLEServerManager::setupServer() {
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(this);
}

void BLEServerManager::setupServices() {
    service = pServer->createService(SSM_SERVICE_UUID);
}

void BLEServerManager::setupCharacteristics() {
    liveDataChar = service->createCharacteristic(
        SSM_CHAR_LIVE_DATA_UUID,
        BLECharacteristic::PROPERTY_NOTIFY
    );
    liveDataChar->addDescriptor(new BLE2902());

    diagnosticsChar = service->createCharacteristic(
        SSM_CHAR_DIAGNOSTICS_UUID,
        BLECharacteristic::PROPERTY_NOTIFY
    );
    diagnosticsChar->addDescriptor(new BLE2902());

    statusChar = service->createCharacteristic(
        SSM_CHAR_STATUS_UUID,
        BLECharacteristic::PROPERTY_NOTIFY
    );
    statusChar->addDescriptor(new BLE2902());

    commandChar = service->createCharacteristic(
        SSM_CHAR_COMMANDS_UUID,
        BLECharacteristic::PROPERTY_WRITE
    );
    commandChar->setCallbacks(this);

    service->start();
}

void BLEServerManager::startAdvertising() {
    BLEAdvertising* advertising = BLEDevice::getAdvertising();
    advertising->addServiceUUID(SSM_SERVICE_UUID);
    advertising->setScanResponse(true);
    advertising->start();
}

void BLEServerManager::loop() {}

void BLEServerManager::sendLiveData(const uint8_t* data, size_t len) {
    if (!deviceConnected) return;
    liveDataChar->setValue(data, len);
    liveDataChar->notify();
}

void BLEServerManager::sendDiagnostics(const uint8_t* data, size_t len) {
    if (!deviceConnected) return;
    diagnosticsChar->setValue(data, len);
    diagnosticsChar->notify();
}

void BLEServerManager::sendStatus(const uint8_t* data, size_t len) {
    if (!deviceConnected) return;
    statusChar->setValue(data, len);
    statusChar->notify();
}

bool BLEServerManager::isConnected() const {
    return deviceConnected;
}

void BLEServerManager::setEventHandler(BLEServerEvents* handler) {
    eventHandler = handler;
}

void BLEServerManager::onConnect(BLEServer* pServer) {
    deviceConnected = true;
    if (eventHandler) eventHandler->onConnect();
}

void BLEServerManager::onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    if (eventHandler) eventHandler->onDisconnect();
    startAdvertising();
}

void BLEServerManager::onWrite(BLECharacteristic* characteristic) {
    std::string value = characteristic->getValue();
    if (eventHandler) {
        eventHandler->onCommand(
            reinterpret_cast<const uint8_t*>(value.data()),
            value.length()
        );
    }
}

} // namespace ssm
