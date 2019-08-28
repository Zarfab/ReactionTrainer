#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <StreamString.h>
#include "ReactionTrainerSlave.h"

#define AP_SSID_PREFIX "REACT_T"
#define AP_PASSWORD "PainIsComing"

#define BOOT_COLOR          CRGB::White
#define WIFI_NOT_FOUND      CRGB::Orange
#define WIFI_CONNECTING     CRGB::Yellow
#define WIFI_NOT_CONNECTED  CRGB::Red
#define WIFI_CONNECTED      CRGB::Chartreuse


DynamicJsonDocument doc(1024);
WebSocketsClient webSocket;
ReactionTrainerSlave reactionTrainer;

    
/**************************************************************************/
/*  Arduino main funtions                                                 */
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  reactionTrainer.init();

  ///// SCAN WIFI NETWORKS /////
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  fill_solid(reactionTrainer.leds, NB_LED, BOOT_COLOR);
  FastLED.show();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) 
  {
      Serial.println("no network found");
      fill_solid(reactionTrainer.leds, NB_LED, WIFI_NOT_FOUND);
      FastLED.show();
      delay(5000);
      ESP.restart();
  } 
  
  String ssid = "";
  for (int i = 0; i < n; ++i) 
  {
    String currentSsid = WiFi.SSID(i);
    if(currentSsid.indexOf(AP_SSID_PREFIX) == 0) 
    {
      ssid = currentSsid;
      Serial.printf("Will try to connect to %s, signal force is %i dB\n", WiFi.SSID(i), WiFi.RSSI(i));
      fill_solid(reactionTrainer.leds, NB_LED, WIFI_CONNECTING);
      FastLED.show();
      break;
    }
    delay(10);
  }
  if(ssid == "") {
    Serial.printf("no network with %s as prefix\n", AP_SSID_PREFIX);
    fill_solid(reactionTrainer.leds, NB_LED, WIFI_NOT_FOUND);
    FastLED.show();
    delay(5000);
    ESP.restart();
  }
  
  WiFi.begin(ssid.c_str(), AP_PASSWORD);
  int nbTries = 0;
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if(nbTries % 4 == 3) {
      WiFi.begin(ssid.c_str(), AP_PASSWORD);
    }
    if(nbTries >= 20) {
      Serial.println("impossible to connect to " + ssid + ", check if password is set correctly");
      fill_solid(reactionTrainer.leds, NB_LED, WIFI_NOT_CONNECTED);
      FastLED.show();
      delay(5000);
      ESP.restart();
    }
    nbTries++;
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  fill_solid(reactionTrainer.leds, NB_LED, WIFI_CONNECTED);
  FastLED.show();
  delay(100);

  webSocket.begin("192.168.4.1", 8025, "/rt");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}



void loop()
{
  webSocket.loop();
  String answer = reactionTrainer.update();
  if(answer != "")
  {
    webSocket.sendTXT(answer);
  }
  FastLED.show();
}



/**************************************************************************/
/*  WebSocket callbacks                                                   */
/**************************************************************************/
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length)
{
    switch(type)
    {
        case WStype_DISCONNECTED:
          Serial.printf("[WSc] Disconnected!\n");
          break;
        case WStype_CONNECTED:
          Serial.printf("[WSc] Connected to url: %s\n", payload);
          break;
        case WStype_TEXT:
        {
          Serial.printf("[WSc] get text: %s\n", payload);
          DeserializationError err = deserializeJson(doc, payload);
          if(err == DeserializationError::Ok)
            reactionTrainer.handleJson(doc.as<JsonObject>());
          else 
            Serial.printf("deserializeJson() failed with code %s\n", err.c_str());
        }
        break;
      default:  
        break;
    }
}
