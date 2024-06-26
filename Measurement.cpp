#include "HardwareSerial.h"
#include "Arduino.h"
#include "Measurement.h"

Measurement::Measurement(long startTimestamp) {
  startTime = startTimestamp;
  recordCount = 0;
}

void Measurement::addRecord(float value, long timestamp) {
  if (recordCount < MAX_RECORDS) {
    records[recordCount].value = value;
    records[recordCount].timestamp = timestamp;
    recordCount++;
  } else {
  }
}

void Measurement::deleteRecord(int index) {
  if (index >= 0 && index < MAX_RECORDS) {
    for (int i = index; i < recordCount - 1; i++) {
      records[i] = records[i + 1];
    }
    recordCount--;
  }
}

void Measurement::sendMessage(float tempValue, long timestamp) {
  Serial.print("{");
  Serial.print(tempValue);
  Serial.print(",");
  Serial.print(timestamp);
  Serial.println("}");
}

void Measurement::sendAllRecords() {
  for (int i = 0; i < recordCount; i++) {
    sendMessage(records[i].value, records[i].timestamp);
  }
}

Record Measurement::getRecordByIndex(int index) {
  if (index >= 0 && index < recordCount) {
    return records[index];
  }
  return {0, 0};  // Return an empty record in case of invalid index
}

int Measurement::getMaxRecordCount() {
  return MAX_RECORDS;
}

int Measurement::getRecordCount() {
  return recordCount;
}

long Measurement::getStartTime() {
  return startTime;
}


void Measurement::clearRecords() {
  recordCount = 0;
}

