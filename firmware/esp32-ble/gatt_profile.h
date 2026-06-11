#pragma once

namespace ssm {

// Service
#define SSM_SERVICE_UUID              "a12f0001-4b8f-4c3d-9a12-7f1a2b3c4d5e"

// Characteristics
#define SSM_CHAR_LIVE_DATA_UUID       "a12f0002-4b8f-4c3d-9a12-7f1a2b3c4d5e"
#define SSM_CHAR_DIAGNOSTICS_UUID     "a12f0003-4b8f-4c3d-9a12-7f1a2b3c4d5e"
#define SSM_CHAR_COMMANDS_UUID        "a12f0004-4b8f-4c3d-9a12-7f1a2b3c4d5e"
#define SSM_CHAR_STATUS_UUID         "a12f0005-4b8f-4c3d-9a12-7f1a2b3c4d5e"
#define SSM_CHAR_ALERTS_UUID         "a12f0006-4b8f-4c3d-9a12-7f1a2b3c4d5e"

#define GATT_CHAR_VEHICLE_STATUS   0x01
#define GATT_CHAR_HEALTH_SCORE     0x02
#define GATT_CHAR_ALERT_EVENTS     0x03
#define GATT_CHAR_ACTIVE_ALERTS    0x04

// Payload definitions
// Vehicle status payload: [healthScore(uint8_t), statusFlag(uint8_t)]
// Health score payload: [healthScore(uint8_t), riskLevel(uint8_t)]
// Diagnostics payload: arbitrary binary diagnostics data
// Alert events payload: [eventIdLow(uint8_t), state(uint8_t)]... up to 16 events
// Active alerts payload: [count(uint8_t), type(uint8_t), severity(uint8_t), state(uint8_t), id[4 bytes]]...

}