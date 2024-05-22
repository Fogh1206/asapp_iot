#ifndef SENSOR_H
#define SENSOR_H

#include <OneWire.h>
#include <DallasTemperature.h>

class Sensor {
public:
    Sensor(uint8_t pin);
    void begin();
    float getTemperature();
    bool isConnected();

private:
    OneWire oneWire;
    DallasTemperature sensors;
};

#endif
