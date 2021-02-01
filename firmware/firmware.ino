#include <PubSubClient.h>
#include <WiFi.h>
#include "config.h"

WiFiClient espClient;

long nextMsg = 0;
long nextWifiRetry = 0;
int wifiRetryCount = 0;

void setup() {
  pinMode(LED_ERROR, OUTPUT); //Prepare LED_ERROR
  digitalWrite(LED_ERROR, LOW);

  Serial.begin(115200);
  Serial.println("Welcome to SmartHeating System");
  delay(100);
  if (!sensorsInit()) { //Catch sensor fail
    while(true) {
      digitalWrite(LED_ERROR, !digitalRead(LED_ERROR));
      delay(100);
    }
  }

  wifiSetup();
  ntpSetup();
  printLocalTime();
  mqttConnect();
  setTargetTemperature(18); //Set start mode
  setStandby();
}

void wifiSetup() { //Configures wifi and waits for connection
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);
  
  WiFi.setHostname(HOSTNAME);
  if (wifiReconnect()) { //Show IP on success
    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else { //Show failed state using LED
    digitalWrite(LED_ERROR, HIGH);
  }
}

bool wifiReconnect() { //Checks connection and reconnects if neccessary. Returns true on good connection, otherwise false
  if (WiFi.status() != WL_CONNECTED) { //Try to reconnect...
    if (millis() > nextWifiRetry) { //...if interval has been expired...
      Serial.println("WiFi not connected (yet)!");
      WiFi.begin(SSID, PSK);

      nextWifiRetry = millis() + WIFI_CONNECT_TIMEOUT;
      while (WiFi.status() != WL_CONNECTED && millis() <= nextWifiRetry) { //Wait at most specified seconds for connection
        delay(500);
        Serial.print(".");
      }

      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Success");
        wifiRetryCount = 0;
        return true;
      } else {
        Serial.println("Failed");
        wifiRetryCount += 1;
        Serial.print("WiFi retry count: ");
        Serial.println(wifiRetryCount);
        
        if (wifiRetryCount > MAX_WIFI_RETRY_COUNT) {
          Serial.println("WiFi failed too often. Reset...");
          ESP.restart();
        }
        nextWifiRetry = millis() + WIFI_RETRY_INTERVAL; //Next retry in 5 seconds
        return false;
      }
    } else { //...otherwise just return false
      return false;
    }
  } else { //Connection is ok
    return true;
  }
}

void loop() {
  if (wifiReconnect()) { //Enable error LED on WiFi failure
    digitalWrite(LED_ERROR, LOW);
  } else {
    digitalWrite(LED_ERROR, HIGH);
  }

  if (!mqttConnected()) {
    if (!mqttReconnect()) { //Enable error LED on MQTT failure
      digitalWrite(LED_ERROR, LOW);
    } else {
      digitalWrite(LED_ERROR, HIGH);
    }
  }

  mqttLoop();

  if (nextMsg < millis()) { //Send values every X seconds
    Serial.println("transmitting_mqtt");
    nextMsg = millis() + MESSAGE_INTERVAL;

    float temperature = getRoomTemperature(); //Masure Temperature, Humidity and Pressure
    float humidity = getRoomHumidity();
    float pressure = getRoomPressure();
    Serial.println(temperature); //Print current values
    Serial.println(humidity);
    Serial.println(pressure);

    if (getToSet()) {
      setCurrentTemperature(temperature); //Inform controller about new temperature
      setVoltage(getPower()); //Set output
      setNextSet(CONTROLLER_INTERVAL);
    }

    publishRoomTemperature(temperature); //Publish data using MQTT
    publishHumidity(humidity);
    publishPressure(pressure);
    publishTargetTemperature();
    publishPower();
    publishStandby();
  }
}
