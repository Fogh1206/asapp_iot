#ifndef Measurement_h
#define Measurement_h
#include <Arduino.h>

struct Record {
  float value;
  long timestamp;
};

class Measurement {
  public:
          Measurement(long startTime);
          void addRecord(float value, long timestamp);
          void deleteRecord(int index);
          int getRecordCount();
          void sendAllRecords();
          Record getRecordByIndex(int index);
          int getMaxRecordCount();
          long getStartTime();
          void clearRecords();
  private:
          long startTime;
          static const int MAX_RECORDS = 120;
          Record records[MAX_RECORDS];
          int recordCount = 0;
          void sendMessage(float tempValue, long timestamp);
};

#endif