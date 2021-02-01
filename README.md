# Junkers_1-2-4_ESP32
Ein ESP32 Arduino Sketch für die Steuerung einer Junkers Gastherme an einer 1-2-4 Schnittstelle über MQTT. Mehr dazu hier: https://robin.meis.space/2019/07/03/junkers-therme-mit-wlan/

## Schaltplan
Der Schalplan ist von https://wiki.fhem.de/wiki/Junkers_Therme_Stetigregelung#Vorbereitung_3 abgeleitet und befindet sich in der Datei `schematic.jpg`

![Schematic](schematic.jpg)

## Firmware
Die Firmware befindet sich im Verzeichnis `firmware/`. Vor dem Flashen der Firmware muss die Datei `config.h` angelegt werden. In der Datei `config-example.h` befindet sich eine Vorlage, bei der die eigenen WLAN Zugangsdaten und der MQTT eingetragen werden müssen.

## MQTT
Die folgenden MQTT werden verwendet.

### Subscribe
Auf diesen Topics wartet der ESP32 auf Nachrichten:
* **heater/control/targetTemperature** Zieltemperatur setzen
* **heater/control/standby** Standby aktivieren/deaktivieren (true/false)

### Publish
Auf diesen Topics publiziert der ESP32 Nachrichten:
* **heater/temperature/room** Aktuelle Raumtemperatur
* **heater/temperature/target** Aktuelle Zieltemperatur
* **heater/power** Aktuelle "Leistung". 0% bis 100% je nach eingestellter Spannung auf der Schnittstelle
* **heater/standby** Standby Status (true/false)
* **heater/humidity** Aktuelle Luftfeuchtigkeit
* **heater/pressure** Aktueller Luftdruck
