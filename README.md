# Smart Safety Monitor
Your car's personal health advisor. It learns how your specific car drives, spots problems weeks before they happen, explains them in plain language, and tells you how urgent they are.

**Privacy-first by design.**

All vehicle data stays on your device. No cloud uploads, no tracking, no subscriptions, and no backend infrastructure required.

Instead of generic diagnostic codes, the system learns:

* How *your* engine behaves
* How *your* transmission behaves
* Typical fuel consumption
* Battery health trends
* Cooling system behavior
* Sensor performance over time

Then it answers:

* What's wrong?
* How serious is it?
* How soon should I act?
* What will likely fail next?


# System Architecture

```text
┌─────────────────────┐
│ Vehicle OBD-II Port │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│ ESP32 Custom Device │
│ BLE Server          │
│ OBD Reader          │
│ Local Storage       │
└──────────┬──────────┘
           │ BLE
           ▼
┌─────────────────────┐
│ Mobile App          │
│ React Native        │
│ Data Sync           │
│ Local Database      │
│ Vehicle Profiling   │
│ Dashboard           │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│ ML Engine           │
│ Vehicle Profiling   │
│ Anomaly Detection   │
│ Risk Scoring        │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
| On-Device LLM       │
│ Human Readable      │
│ Recommendations     │
└─────────────────────┘
```

# Monorepo Structure

```text
smart-safety-monitor/

├── firmware/
│   ├── esp32-ble
│   ├── obd-reader
│   ├── storage
│   └── diagnostics
│
├── mobile/
│   ├── react-native
│   ├── bluetooth
│   ├── local-db
│   ├── vehicle-profile
│   ├── dashboards
│   └── notifications
│
├── ml/
│   ├── feature-engineering
│   ├── anomaly-detection
│   ├── predictive-maintenance
│   └── model-training
│
├── ai/
│   ├── local-llm
│   ├── prompt-engineering
│   ├── diagnosis-engine
│   └── recommendations
│
├── docs/
│   ├── architecture
│   ├── hardware
│   ├── api
│   ├── privacy
│   └── protocols
│
└── firmware-tools/
    ├── flashing
    ├── diagnostics
    └── testing
```

# Hardware Stack

## MCU

ESP32-S3

Why:

* BLE built-in
* WiFi available for future updates
* Enough RAM
* Cheap
* Large ecosystem

## OBD Interface

STN1110 or ELM327 compatible chip

Responsibilities:

* OBD protocol translation
* CAN communication
* Vehicle compatibility

## Storage

16MB Flash minimum

Store:

* Historical driving data
* Last diagnostic snapshots
* Offline buffering

## Power

Vehicle 12V → Buck converter → 3.3V

Must include:

* Reverse polarity protection
* Surge protection
* Sleep mode

# Firmware Deliverables

## Module 1

Vehicle Communication

```cpp
readPID()
readDTC()
clearDTC()
```

Supported PIDs:

```text
RPM
Speed
Coolant Temp
Fuel Trim
MAF
Throttle Position
Battery Voltage
Engine Load
Fuel Pressure
```

## Module 2

BLE Service

Custom GATT profile:

```text
Vehicle Status
Live Telemetry
Diagnostic Codes
Alerts
Device Settings
```

## Module 3

Local Intelligence

Basic edge calculations:

```text
Moving averages
Sensor drift
Voltage drops
Temperature trends
```

No LLM on device.

The ESP32 only preprocesses.

## Current Firmware Status

The firmware is partially implemented and now includes:

* ESP32 runtime entrypoint with `setup()` / `loop()`
* BLE GATT service and notification characteristics for live data, status, diagnostics, and alerts
* Active alert state sync over BLE
* OBD-II data capture path with PID polling, DTC read/clear support, and OBD interface initialization
* Correlation engine integrated into diagnostics for root-cause inference
* PlatformIO build configuration at `platformio.ini`

Remaining work:

* full BLE command response payload validation and mobile app contract
* expanded diagnostics rule coverage and trend analysis
* firmware testing on real ESP32 hardware

# Mobile App Deliverables

## Dashboard

Shows:

```text
Vehicle Health Score
Active Issues
Battery Health
Engine Health
Fuel Efficiency
```

## Live Screen

```text
RPM
Speed
Engine Temp
Voltage
Fuel Rate
```

## Alert Screen

Examples:

```text
Battery voltage has been declining for 14 days.

Severity: Medium

Estimated failure window:
2-4 weeks.
```

# Machine Learning Deliverables

Phase 1:

Rule-based intelligence.

```text
Voltage dropping
+
Cold start issues
=
Battery warning
```

Phase 2:

Anomaly Detection

Models:

```text
Isolation Forest
One-Class SVM
Autoencoders
```

Learn:

* Normal idle RPM
* Normal temperature curves
* Normal fuel usage

Detect deviations.

Phase 3:

Predictive Maintenance

Predict:

```text
Battery failure
Alternator issues
Cooling system problems
Sensor degradation
Misfires
```

Weeks before DTCs appear.

# AI Layer

The LLM never diagnoses directly. Instead:

```text
Telemetry
+
ML Findings
+
Vehicle Data
=
Structured Report
```

Then LLM translates:

```text
Technical:
Battery voltage dropped
13.9V → 12.7V

Human:
Your battery is weakening.
You should test or replace it within
the next 30 days.
```

# Risk Scoring Engine

Simple 0-100 score.

```text
0-25  Critical
26-50 High
51-75 Medium
76-100 Healthy
```

Factors:

* DTC codes
* Anomalies
* Trend deterioration
* Mileage

# The moat

Most OBD scanners tell users:

> "P0420 Catalytic Converter Efficiency Below Threshold"

This system tells them:

> "Over the last 21 days your fuel efficiency has dropped by 11% and oxygen sensor readings have become unstable. The catalytic converter may be degrading. Schedule an inspection within the next month."

That's the difference between a scanner and a vehicle health advisor.

