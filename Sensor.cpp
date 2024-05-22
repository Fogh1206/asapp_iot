#include "Sensor.h"

Sensor::Sensor(uint8_t pin) : oneWire(pin), sensors(&oneWire) {
    // Constructor
}

void Sensor::begin() {
    sensors.begin();
}

float Sensor::getTemperature() {
    sensors.requestTemperatures();
    return sensors.getTempCByIndex(0);
}

bool Sensor::isConnected() {
    return sensors.getTempCByIndex(0) != DEVICE_DISCONNECTED_C;
}
