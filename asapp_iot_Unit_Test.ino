#include <Arduino.h>
#include <ArduinoUnit.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Sensor.h"
#include "Measurement.h"

/*
SENSOR TESTS
*/

test(sensor_initialization) {
    Sensor sensor(2);
    sensor.begin();
    assertTrue(sensor.isConnected());
}

test(sensor_temperature_read) {
    Sensor sensor(2);
    sensor.begin();
    float temp = sensor.getTemperature();
    assertFalse(temp == DEVICE_DISCONNECTED_C);
    assertMore(temp, -55.0);  // Temperature should be more than -55C
    assertLess(temp, 125.0);   // Temperature should be less than 125C
}

/*
MEASUREMENT TESTS
*/


test(measurement_initialization) {
    Measurement measurement(1000);
    assertEqual(measurement.getStartTime(), 1000);
    assertEqual(measurement.getRecordCount(), 0);
}

test(add_zero_records) {
    Measurement measurement(1000);
    assertEqual(measurement.getRecordCount(), 0);
}

test(add_one_record) {
    Measurement measurement(1000);
    measurement.addRecord(25.0, 1100);
    assertEqual(measurement.getRecordCount(), 1);
    Record record = measurement.getRecordByIndex(0);
    assertEqual(record.value, 25.0);
    assertEqual(record.timestamp, 1100);
}

test(add_more_records) {
    Measurement measurement(1000);
    measurement.addRecord(25.0, 1100);
    measurement.addRecord(30.0, 1200);
    measurement.addRecord(35.0, 1300);
    assertEqual(measurement.getRecordCount(), 3);
    Record record = measurement.getRecordByIndex(2);
    assertEqual(record.value, 35.0);
    assertEqual(record.timestamp, 1300);
}

test(add_boundary_records) {
    Measurement measurement(1000);
    for(int i = 0; i < (measurement.getMaxRecordCount()); i++){
      measurement.addRecord(25, (1000*i));
    }
    assertEqual(measurement.getRecordCount(), measurement.getMaxRecordCount());
}



test(delete_zero_records) {
    Measurement measurement(1000);
    assertEqual(measurement.getRecordCount(), 0);
}

test(delete_one_record) {
    Measurement measurement(1000);
    measurement.addRecord(25.0, 1100);
    measurement.deleteRecord(0);
    assertEqual(measurement.getRecordCount(), 0);
}

test(delete_more_records) {
    Measurement measurement(1000);

    for(int i = 0 ; i < 3 ; i++){
      measurement.addRecord(25.0, i * 1000);
    }
    for(int i = 0 ; i < 3 ; i++){
      measurement.deleteRecord(i);
    }
    assertEqual(measurement.getRecordCount(), 0);
}

test(delete_boundary_records) {
  Measurement measurement(1000);

  for(int i = 0 ; i < measurement.getMaxRecordCount() ; i++){
    measurement.addRecord(25.0, i * 1000);
  }

  for(int i = 0 ; i < measurement.getMaxRecordCount() ; i++){
    measurement.deleteRecord(i);
  }
  assertEqual(measurement.getRecordCount(), 0);
}

test(clear_records_zero) {
  Measurement measurement(1000);
  measurement.clearRecords();
  assertEqual(measurement.getRecordCount(), 0);
}

test(clear_records_one) {
    Measurement measurement(1000);
    measurement.addRecord(26.0, 1200);
    measurement.clearRecords();
    assertEqual(measurement.getRecordCount(), 0);
}

test(clear_records_more) {
  Measurement measurement(1000);

  for(int i = 0 ; i < 3 ; i++){
    measurement.addRecord(25.0, i * 1000);
  }

  measurement.clearRecords();
  assertEqual(measurement.getRecordCount(), 0);

}

test(clear_records_boundary) {
  Measurement measurement(1000);

  for(int i = 0 ; i < measurement.getMaxRecordCount() ; i++){
    measurement.addRecord(25.0, i * 1000);
  }

  measurement.clearRecords();
  assertEqual(measurement.getRecordCount(), 0);
}

// Test the getRecordByIndex method
test(get_record_by_index_valid) {
    Measurement measurement(1000);
    measurement.addRecord(25.0, 1100);
    measurement.addRecord(26.0, 1200);
    
    Record record1 = measurement.getRecordByIndex(0);
    assertEqual(record1.value, 25.0);
    assertEqual(record1.timestamp, 1100);
    
    Record record2 = measurement.getRecordByIndex(1);
    assertEqual(record2.value, 26.0);
    assertEqual(record2.timestamp, 1200);
    
}

test(get_record_by_index_invalid) {
    Measurement measurement(1000);
    measurement.addRecord(25.0, 1100);
    
    // Test out-of-bounds index
    Record record = measurement.getRecordByIndex(1);
    assertEqual(record.value, 0.0);  // Assuming default value for invalid index
    assertEqual(record.timestamp, 0);
    
    // Test negative index
    record = measurement.getRecordByIndex(-1);
    assertEqual(record.value, 0.0);  // Assuming default value for invalid index
    assertEqual(record.timestamp, 0);
}

test(isConnectedToBluetooth) {
  assertTrue(digitalRead(12));
}
