#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <StreamString.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include <Wire.h>
#include <VL6180X.h>



#define AP_SSID_PREFIX "REACTION_TRAINER_"
#define AP_PASSWORD "" // let empty if you don't want to use a password


#define BATTERY_MEASURE_PIN 26
#define BUZZER_PIN 18
#define LED_PIN 16
#define NB_LED 12


struct Settings {
  uint8_t threshold;
  uint8_t reactTimeout;
};


WebSocketsClient webSocket;

StaticJsonDocument<256> jsonSettings;
Settings settings;

uint8_t brightness = 100;
CRGB groupColor = 0xff0000;
CRGB leds[NB_LED];
bool readSensor = false;
uint64_t sensorTimer;
VL6180X sensor;



/**************************************************************************/
/*  Arduino main funtions                                                 */
/**************************************************************************/
void setup(){
  Serial.begin(115200);

  ///// SETUP FASTLED AND DISPLAY BATTERY LEVEL /////
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NB_LED).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(brightness);
  for(int i = 0; i < NB_LED; i++) { leds[i] = groupColor; }
  FastLED.show();
  delay(1000);
  // TODO : Measure battery voltage and display it

  //delay(1000);

  ///// SCAN WIFI NETWORKS /////
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  for(int i = 0; i < NB_LED; i++) { leds[i] = CRGB::White; }
  FastLED.show();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
      Serial.println("no networks found");
  } else {
    String ssid = "";
      for (int i = 0; i < n; ++i) {
        String currentSsid = WiFi.SSID(i);
        if(currentSsid.indexOf(AP_SSID_PREFIX) == 0) {
          ssid = currentSsid;
          Serial.printf("Will try to connect to %s, signal force is %i dB\n", WiFi.SSID(i), WiFi.RSSI(i));
          break;
        }
        delay(10);
      }
      if(AP_PASSWORD != "")
        WiFi.begin(ssid.c_str(), AP_PASSWORD);
      else
        WiFi.begin(ssid.c_str());
  }

  webSocket.begin("192.168.4.1", 81, "/");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);

  sensor.init();
  sensor.configureDefault();
  sensor.setTimeout(200);
}



void loop() {
  webSocket.loop();
  FastLED.show();
  if(readSensor) {
    uint8_t val = sensor.readRangeSingle();
    if (val <= settings.threshold) {
      uint64_t reactTime = millis() - sensorTimer;
      Serial.printf("got touched in %d ms\n", reactTime);
      readSensor = false;
    }
    if(millis() - sensorTimer > reactTimeout) {
      Serial.println("Not touched in timeout");
      readSensor = false;
    }
  }
}



/**************************************************************************/
/*  FastLed utils                                                         */
/**************************************************************************/
String crgbToHtml(const CRGB& color) {
  char buf[8];
  sprintf(buf, "#%02x%02x%02x", color.r, color.g, color.b);
  return String(buf);
}

CRGB htmlToCRGB(const char* htmlString) {
  return (int) strtol(htmlString + 1, NULL, 16);
}



/**************************************************************************/
/*  WebSocket callbacks                                                   */
/**************************************************************************/
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[WSc] Disconnected!\n");
            break;
        case WStype_CONNECTED:
              Serial.printf("[WSc] Connected to url: %s\n", payload);
            break;
        case WStype_TEXT:
              Serial.printf("[WSc] get text: %s\n", payload);
              //webSocket.sendTXT("message here");
            break;
      default:  
        break;
    }
}
