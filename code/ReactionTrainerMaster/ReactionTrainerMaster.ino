#include "FS.h" 
#include "SPIFFS.h"
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <WiFiAP.h>
#include <ArduinoJson.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include <StreamString.h>


#define AP_SSID_PREFIX "REACTION_TRAINER_"
#define AP_PASSWORD "" // let empty if you don't want to use a password
#define WEB_USERNAME "tormentor"
#define WEB_USERPASS "sportisfun"

#define SETTINGS_FILE "/settings.json"

#define BATTERY_MEASURE_PIN 26
#define BUZZER_PIN 18
#define LED_PIN 16
#define NB_LED 12


WebServer server(80);
String lang = "fr";
WebSocketsServer webSocket = WebSocketsServer(81);

StaticJsonDocument<128> doc;
StaticJsonDocument<256> settings;

uint8_t brightness = 100;
CRGB groupColor = 0xffff00;
CRGB leds[NB_LED];



/****************************************/
/*  server funtions (needed here)       */
/****************************************/
String getContentType(String filename){
  if(server.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path){
  if(!server.authenticate(WEB_USERNAME, WEB_USERPASS)) {
    server.requestAuthentication();
    return false;
  }
  if(path.endsWith("/")) path += "index.html";
  //if(path.endsWith(".html")) path = "/" + lang + path;
  Serial.println("handleFileRead: " + path);
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
    if(SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}



/**************************************************************************/
/*  Arduino main funtions                                                 */
/**************************************************************************/
void setup(){
  Serial.begin(115200);

  ///// LOAD SETTINGS FROM FILE /////
  if(!SPIFFS.begin()){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
  }
  File file = SPIFFS.open(SETTINGS_FILE, FILE_READ);
  DeserializationError error = deserializeJson(settings, file);
  file.close();
  if(error) {
    Serial.println(F("Failed to read file, using default settings"));
    Serial.println(error.c_str());
    settings.createNestedObject("settings");
    settings["settings"]["brightness"] = 100;
    settings["settings"]["group_color"] = "#ffff00";
  }
  else {
    brightness = settings["settings"]["brightness"] | 100;
    groupColor = htmlToCRGB(settings["settings"]["group_color"] | "#ffff00");
  }

  ///// SETUP FASTLED AND DISPLAY BATTERY LEVEL /////
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NB_LED).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(brightness);
  for(int i = 0; i < NB_LED; i++) { leds[i] = groupColor; }
  FastLED.show();
  delay(1000);
  // TODO : Measure battery voltage and display it

  //delay(1000);

  ///// SETUP WIFI ACCESS POINT /////
  WiFi.mode(WIFI_AP);
  //WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  Serial.println("Setting AP (Access Point)…");
  char ssid[sizeof(AP_SSID_PREFIX) + 6];
  strcpy(ssid, AP_SSID_PREFIX);
  uint64_t chipid = ESP.getEfuseMac();
  char chipIdStr[12];
  sprintf(chipIdStr, "%02X%02X%02X",
          (uint8_t)(chipid >> 24),
          (uint8_t)(chipid >> 32),
          (uint8_t)(chipid >> 40));
  strcat(ssid, chipIdStr);
  if(AP_PASSWORD != "")
    WiFi.softAP(ssid, AP_PASSWORD);
  else
    WiFi.softAP(ssid);

  Serial.printf("Access point name is %s, IP is ", ssid);
  Serial.println(WiFi.softAPIP());

  ///// SETUP WEB SERVER AND WEBSOCKET SERVER /////
  //called when the url is not defined
  //use it to load content from SPIFFS
  server.onNotFound([](){
    if(!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}



void loop() {
  webSocket.loop();
  server.handleClient();
  FastLED.show();
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
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
              IPAddress ip = webSocket.remoteIP(num);
              Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
              // send current config
              StreamString buf;
              serializeJson(settings, buf);
              webSocket.sendTXT(num, buf);
            }
            break;
        case WStype_TEXT:
            {
              if(payload[0] == '#') {
                switch(payload[1]) {
                  case 'B': {
                      brightness = (uint8_t)strtol((char*)payload + 2, NULL, 10);
                      settings["settings"]["brightness"] = brightness;
                      FastLED.setBrightness(brightness);
                      //Serial.printf("set brightness to %i\n", brightness);
                      //webSocket.broadcastTXT(payload);
                      FastLED.show();
                    }
                    break;
                  case 'C': {
                      groupColor = htmlToCRGB((char*)payload + 2);
                      settings["settings"]["group_color"] = crgbToHtml(groupColor);
                      for(int i = 0; i < NB_LED; i++) { leds[i] = groupColor; }
                      //Serial.printf("set color to %s\n", crgbToHtml(leds[0]));
                      //webSocket.broadcastTXT(payload);
                      FastLED.show();
                    }
                    break;
                  default:
                    break;
                }
              }
              else {
                Serial.printf("[%u] get Text: %s\n", num, payload);
                DeserializationError error = deserializeJson(doc, payload);
                if (error) {
                  Serial.print(F("deserializeJson() failed: "));
                  Serial.println(error.c_str());
                  return;
                }
                serializeJsonPretty(doc, Serial);
                Serial.println();
                if(!doc["settings"].isNull()) {
                  Serial.println("deserialize");
                  deserializeJson(settings, payload);
                  Serial.println("open file");
                  File file = SPIFFS.open(SETTINGS_FILE, FILE_WRITE);
                  if (!file) {
                    Serial.printf("There was an error opening the file %s for writing\n", SETTINGS_FILE);
                    return;
                  }
                  Serial.println("serialize");
                  if (serializeJson(settings, file) == 0) {
                    Serial.printf("Failed to write to file %s\n", SETTINGS_FILE);
                  }
                  file.close();
                  Serial.println("settings saved in file");
                }
              }
            }
      default:  
        break;
    }
}