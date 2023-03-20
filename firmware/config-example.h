#ifndef __CONFIG_H__
#define __CONFIG_H__

//Wireless Networks Section
#define SSID     "Your IoT WiFi"
#define PSK      "SomeSecurePassword"
#define HOSTNAME "junkers"

//MQTT Section
#define MQTT_SERVER "your.mqtt.server"
#define MQTT_PORT "1883"
#define MQTT_USER "your.mqtt.username"
#define MQTT_PASSWORD "your.mqtt.password"

//Initialization (Heating setpoint and mode after a controller reset)
#define INIT_TEMPERATURE 18;
#define INIT_STANDBY true;

//Retries
#define MAX_MQTT_RETRY_COUNT 20 //sets the maximum amount of MQTT retries before reboot
#define MAX_WIFI_RETRY_COUNT 20 //sets the maximum amount of WiFi retries before reboot

//Intervals
#define CONTROLLER_INTERVAL     180000 //Interval between level changes
#define MESSAGE_INTERVAL        60000 //Interval between MQTT Messages

#define WIFI_CONNECT_TIMEOUT    10000 //WiFi will abort connection attempt after this amount of time
#define WIFI_RETRY_INTERVAL     5000 //Interval between the wireless network will retry the connection on failed state
#define MQTT_RETRY_INTERVAL     5000 //Interval between the MQTT connection will try to recover on failed state

//Sensors Section
#define SENSOR_TYPE BME280
#define I2C_SDA     22
#define I2C_SCL     23

//NTP Section
#define NTPSERVER           "pool.ntp.org"
#define GMT_OFFSET_SEC      3600
#define DAYLIGHT_OFFSET_SEC 7200

//DAC Section
#define DAC_U_MAX  2.6
#define OPV_U_MAX  22.9
#define DAC_MAX    165

//LEDs
#define LED_ERROR  2 //LED to enable on failed state
#endif
