#include <PubSubClient.h>
#include <stdlib.h>

PubSubClient client(espClient);
long nextReconnect = 0;
int mqttRetryCount = 0;

bool mqttConnect() {
  client.setServer(
    MQTT_SERVER, //Server
    MQTT_PORT //Port
  );
  client.connect(
    "heater",
    MQTT_USER, //Username
    MQTT_PASSWORD, //Username
    "heater/status", //Will Topic
    2, //Will QoS
    1, //Will retain
    "Offline" //Will message
  );

  client.publish("heater/status", "Online", true); //Report as online
  subscribe();
  client.setCallback(callback);
}

void subscribe() {
  client.subscribe("heater/control/#");
}

bool mqttConnected() {
  return client.connected();
}

bool mqttReconnect() { //Returns true on success, otherwise false
  if (millis() > nextReconnect) {
    Serial.println("MQTT connection failed");
    Serial.print("Attempting MQTT connection...");
    client.disconnect();
    if (client.connect("heater")) {
      Serial.println("connected");
      mqttRetryCount = 0;
      subscribe();
      return true;
    } else {
      Serial.println("Reconnect failed.");
      mqttRetryCount += 1;
      Serial.print("MQTT retry count: ");
      Serial.println(mqttRetryCount);
      if (mqttRetryCount > MAX_MQTT_RETRY_COUNT) {
        Serial.println("MQTT failed too often. Resetting...");
        delay(1000);
        ESP.restart();
      }
      nextReconnect = millis() + MQTT_RETRY_INTERVAL;
      return false;
    }
  } else {
    return false;
  }
}

void mqttLoop() {
  client.loop();
}

bool publishRoomTemperature(float temperature) {
  char tempString[8];
  dtostrf(temperature, 1, 2, tempString);
  return client.publish("heater/temperature/room", tempString);
}

bool publishTargetTemperature() {
  char tempString[8];
  dtostrf(getTargetTemperature(), 1, 2, tempString);
  return client.publish("heater/temperature/target", tempString);
}

bool publishHumidity(float humidity) {
  char humString[8];
  dtostrf(humidity, 1, 2, humString);
  return client.publish("heater/humidity", humString);
}

bool publishPressure(float pressure) {
  char pressString[15];
  dtostrf(pressure, 1, 2, pressString);
  return client.publish("heater/pressure", pressString);
}

bool publishPower() {
  char powerString[4];
  itoa(getPower(), powerString, 10);
  return client.publish("heater/power", powerString);
}

bool publishStandby() {
  if (isStandy()) return client.publish("heater/standby", "true");
  else return client.publish("heater/standby", "false");
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "heater/control/targetTemperature") { //Set target temperature
    Serial.print("Change target temperature: ");
    Serial.println(messageTemp);
    float temperature = atof (messageTemp.c_str());
    setTargetTemperature(temperature);
    setNextSet(0); //Reset change blocker
  } else if (String(topic) == "heater/control/standby") { //Set standby / power on
    if (messageTemp == "true") {
      Serial.println("Standby: On");
      setStandby();
      setNextSet(0); //Reset change blocker
    } else if(messageTemp == "false") {
      Serial.println("Standby: Off");
      setOn();
      setNextSet(0); //Reset change blocker
    } else {
      Serial.println("Standby: Invalid command");
    }
  }
}
