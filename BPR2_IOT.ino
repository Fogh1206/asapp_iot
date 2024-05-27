#include "Sensor.h"
#include "Measurement.h"

// Define pins

// Pin 12 on Arduino has the status pin from the HC-05.
#define STATUS_PIN 12
// Pin 2 on Arduino has the One Wire Bus for the temperature sensor.
#define ONE_WIRE_BUS 2  


// Instantiate objects
Measurement* measurementPtr = nullptr; 
Sensor temperatureSensor(ONE_WIRE_BUS);

// Instantiate variables
unsigned long previousMillis = 0;               // Stores the last time data was sent
const long interval = 5000;                     // Interval between data transmissions (in milliseconds)

void setup() {
    // Begin serial port on 9600. This is used by Bluetooth.
    Serial.begin(9600);

    // Make status pin from HC-05 as input.
    pinMode(STATUS_PIN, INPUT);

    temperatureSensor.begin();
}

void loop() {

  unsigned long currentMillis = millis();
  float temp = temperatureSensor.getTemperature();

  handleBluetoothCommands(currentMillis);

  manageMeasurements(currentMillis, temp);

}

void handleBluetoothCommands(long currentMillis) {
  while (Serial.available() > 0) {
      char data = Serial.read();
      switch (data) {
        case 's':
          startMeasurement(currentMillis);
          break;
        case 'p':
          stopMeasurement();
          break;
        default:
          break;
      }        
      return;
  }
}


void startMeasurement(long currentMillis) {

    if (measurementPtr == nullptr) {
        measurementPtr = new Measurement(currentMillis);
        Serial.println("(Start command received)");
    } else {
        Serial.println("Measurement already started");
    }
}

void stopMeasurement() {
    if (measurementPtr != nullptr) {
        measurementPtr->sendAllRecords();
        delete measurementPtr;
        measurementPtr = nullptr;
        Serial.println("(Stop command received)");
    } else {
        Serial.println("There is no instance of measurement");
    }
}

void sendAndClearRecords() {
    if (measurementPtr->getRecordCount() > 0) {
        for (int i = measurementPtr->getRecordCount() - 1; i >= 0; i--) {
            sendTemp(
                measurementPtr->getRecordByIndex(i).value,
                measurementPtr->getRecordByIndex(i).timestamp
            );
            measurementPtr->deleteRecord(i);
        }
    }
}


void manageMeasurements(unsigned long currentMillis, float temp) {
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        if (temperatureSensor.isConnected()) {
            if (measurementPtr != nullptr) {
                if (digitalRead(STATUS_PIN)) {
                    sendAndClearRecords();
                    sendTemp(temp, (currentMillis - measurementPtr->getStartTime()));
                } else {
                    measurementPtr->addRecord(temp, (currentMillis - 
                    measurementPtr->getStartTime()
                    ));
                }
            } else if (digitalRead(STATUS_PIN)) {
                sendTemp(temp, currentMillis);
            }
        }
    }
}

void sendTemp(float temp, long timestamp) {
    Serial.print("{");
    Serial.print(temp);
    Serial.print(",");
    Serial.print(timestamp);
    Serial.println("}");
}

