#include "HardwareSerial.h"
#include "Arduino.h"
#include "Measurement.h"

Measurement::Measurement(long startTimestamp){
  startTime = startTimestamp;
  recordCount = 0;
}

void Measurement::addRecord(float value, long timestamp){
  if (recordCount < MAX_RECORDS) {
    
    records[recordCount].value = value;
    records[recordCount].timestamp = timestamp - startTime;
    Serial.print("Record of ");
    Serial.print(records[recordCount].value);
    Serial.print("C added, ");
    recordCount++;
    Serial.print(MAX_RECORDS - recordCount);
    Serial.print(" left out of ");
    Serial.println(MAX_RECORDS);
  } else {
    Serial.println("No more space for records!");
  }
}

bool Measurement::isWithingTimeFrame(long currentTimestamp){
  if( currentTimestamp > startTime){
    return true;
  } else {
    return false;
  }
}

void Measurement::sendAllRecords(){

  for(int i = 0 ; i < recordCount ; i++){
    Serial.print("{");
    Serial.print(records[i].value);
    Serial.print(",");
    Serial.print(records[i].timestamp);
    Serial.println("}"); 
  }
}

Record Measurement::getRecordByIndex(int index){
  return records[index];
}

int Measurement::getRecordsSize(){
  return recordCount;
}

long Measurement::getStartTime(){
  return startTime;
}

void Measurement::clearRecords(){
  delete[] records;
}