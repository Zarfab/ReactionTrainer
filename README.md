# ReactionTrainer
These modules are developed with the aim of creating exercises to improve athletes' reflexes.

## How to use

## Make your own

### Bill of material
(For each module)

|Part|Reference|cost|Other information|
|----|----|----|----|
|Micro-controller|ESP32 D1 Mini|3,80€|
|Distance sensor|VL6180X|2.40€|[datasheet](https://www.st.com/resource/en/datasheet/vl6180x.pdf)
|RGB LED ring|WS2812B ring|3.30€|12 LED, outer diameter 52mm|
|Buzzer|12085 16R|0.20€|Often sold in packs of 5 or 10|
|Battery|18650|3.00€|
|Battery holder|For 1x 18650 cell|0.55€|
|On / Off switch|TS-13|1.00€|
|Case|3D printed||

Total cost for a single module is around 15€ without the case.

### Software requirement
- [Arduino IDE 1.8.6](https://www.arduino.cc/en/main/software) or newer with [Arduino Core for ESP32 installed](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md)
- [FastLED](https://github.com/FastLED/FastLED) (install latest from Arduino IDE Librairies Manager)
- [arduinoWebSockets](https://github.com/Links2004/arduinoWebSockets) (install latest - at least v 2.x.x - from Arduino IDE Librairies Manager)
- [arduinoJson](https://github.com/bblanchon/ArduinoJson) (install latest - at least v 6.x.x - from Arduino IDE Librairies Manager)
- [VL6180X (Polulu)](https://github.com/pololu/vl6180x-arduino) (install latest from Arduino IDE Librairies Manager)

