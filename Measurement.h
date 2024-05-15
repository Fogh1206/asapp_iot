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
          bool isWithingTimeFrame(long currentTime);
          void sendAllRecords();
          Record getRecordByIndex(int index);
          int getRecordsSize();
          long getStartTime();
          void clearRecords();
  private:
          long startTime;
          static const int MAX_RECORDS = 100;
          Record records[MAX_RECORDS];
          int recordCount = 0;
};

#endif