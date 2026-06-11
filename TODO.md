Try the following on obd reader

## 1. Adaptive PID Scheduler

* reduces ECU load
* rotates PID polling intelligently
* prioritizes anomalies

## 2. Interrupt-driven UART RX ring buffer

* removes polling entirely

## 3. BLE protocol hardening (recommended)

* packet versioning
* CRC validation
* fragmentation system
* reconnect sync logic

## 4. Full mobile sync contract

* defines exact app state model
* makes frontend trivial to build


## 5. Active alerts characteristic not implemented yet

We only stream events, not full state sync
