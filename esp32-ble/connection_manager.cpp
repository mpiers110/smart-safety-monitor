#include <Arduino.h>

namespace ssm {

class ConnectionManager {
public:
    ConnectionManager() : connected(false) {}

    void setConnected(bool state) {
        connected = state;
        lastChange = millis();
    }

    bool isConnected() const {
        return connected;
    }

    unsigned long lastSeen() const {
        return lastChange;
    }

private:
    bool connected;
    unsigned long lastChange;
};

}
