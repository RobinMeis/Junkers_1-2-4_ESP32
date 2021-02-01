#include "time.h"
#include "config.h"

void printLocalTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void ntpSetup() {
  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTPSERVER);
}
