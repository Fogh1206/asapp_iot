#include "Measurement.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "SafeStringReader.h"

// Pin 12 on Arduino has the status pin from the HC-05
#define statusPin 12

// Make a standard measurement with starts and ends at 0, thus receiving no reading.
//Measurement measurement(0);

Measurement* measurementPtr = nullptr;

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices such as the Dallas DS18B20.
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

char data = 0;                      // Bluetooth Data
unsigned long previousMillis = 0;   // Stores the last time data was sent
const long interval = 5000;          // Interval between data transmissions (in milliseconds)
String dataString = "";

bool isMeasuring;
float records[120];
long recordsMillis[120];
long offsetMillis = 0;
int recordCount = 0;

createSafeStringReader(sfReader, 5, " ,\r\n");

void setup() {

  isMeasuring = false;

  // Begin serial port on 9600. This is used by Bluetooth.
  Serial.begin(9600);

  // Make status pin from HC-05 as input.
  pinMode(statusPin, INPUT);

  // Delete previous data.
  //measurement.clearRecords();

  int recordCount = 0;

  SafeString::setOutput(Serial); // enable error messages and SafeString.debug() output to be sent to Serial
  sfReader.connect(Serial); // where SafeStringReader will read from
  sfReader.echoOn(); // echo back all input, by default echo is off

}

void loop() {

  unsigned long currentMillis = millis();
  sensors.requestTemperatures(); 
  float temp = sensors.getTempCByIndex(0);

  if(sfReader.read()) {
    if(sfReader == "start"){
      if(measurementPtr == nullptr) {
        measurementPtr = new Measurement(currentMillis);
        offsetMillis = currentMillis;
        isMeasuring = true;
        Serial.println("(Start command received)");
      } else {
        Serial.println("Measurement already started");
      }
    } else if (sfReader == "stop") {
      if (measurementPtr != nullptr) {

        measurementPtr->sendAllRecords();

        delete measurementPtr;
        measurementPtr = nullptr;
        Serial.println("(Stop command received)");
        isMeasuring = false;
      } else {
        Serial.println("There is no instance of measurement");
      }
    } else {
      Serial.println("Unknown comand");
    }
  }

  if(Serial.available() > 0){
  } else {

    if (currentMillis - previousMillis >= interval) {

      previousMillis = currentMillis;

      // Check if the temperature sensor is disconnected from the Arduino.
      if(temp != DEVICE_DISCONNECTED_C) {

        if(isMeasuring){

          /*if (offsetMillis == 0) {
            offsetMillis = currentMillis;
          }*/

          // Check if there is a Bluetooth connection between the HC05 module and the user device.
          if(digitalRead(statusPin)){

            for(int i = recordCount ; i > 0 ; i--){
              sendTemp(records[i], recordsMillis[i]);
              records[i] = 0;
              recordsMillis[i] = 0;
              recordCount = recordCount - 1;
            }

            sendTemp(temp, (currentMillis - offsetMillis));

          // If there is no connection but the temperature sensor is connected, then save recorded measurements.
          } else {
            if(recordCount <= 120){
              records[recordCount] = temp;
              recordsMillis[recordCount] = (currentMillis - offsetMillis);
              recordCount = recordCount + 1;
            } else {
              Serial.println("There is no more space");
            }
          }

          // If it is not measuring but there is Bluetooth connection then send current temperature.
        } else if (!isMeasuring && digitalRead(statusPin)) {
          sendTemp(temp, (currentMillis - offsetMillis));
        }
      }
    }
  }
}

void sendTemp(float temp, long timestamp){
  Serial.print("{");
  Serial.print(temp);
  Serial.print(",");
  Serial.print(timestamp);
  Serial.println("}");
}

