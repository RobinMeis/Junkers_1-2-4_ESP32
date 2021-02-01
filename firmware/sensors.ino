#include "Adafruit_BME280.h"
#include "config.h"
#include <Wire.h>

Adafruit_BME280 bme(I2C_SDA, I2C_SCL);

bool sensorsInit()  { //Perform initialization for all sensors. Returns true on success
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    return false;
  }

  return true;
}

float getRoomTemperature() {
 return bme.readTemperature() - 1.7;
}

float getRoomHumidity() {
  return bme.readHumidity();
}

float getRoomPressure() {
  return bme.readPressure();
}
