#include <WiFi.h>
#include <WebSocketsServer.h>
#include <WiFiAP.h>
#include <ArduinoJson.h>

#define AP_SSID_PREFIX "REACTION_TRAINER_"
#define AP_PASSWORD "PainIsComing"


WebSocketsServer webSocket = WebSocketsServer(8025, "/rt");
DynamicJsonDocument doc(1024);


/**************************************************************************/
/*  Arduino main funtions                                                 */
/**************************************************************************/
void setup(){
  Serial.begin(115200);

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
  WiFi.softAP(ssid, AP_PASSWORD);

  Serial.printf("Access point name is %s, IP is ", ssid);
  Serial.println(WiFi.softAPIP());

  ///// SETUP WEB SERVER AND WEBSOCKET SERVER /////
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}



void loop() {
  webSocket.loop();
  if(Serial.available()) {
    DeserializationError err = deserializeJson(doc, Serial);
    if(err) {
      Serial.printf("deserializeJson() failed with code %s\n", err.c_str());
      return;
    }
    uint8_t target = doc["target"] | 0;
    char buf[1024];
    serializeJson(doc, buf);
    if(target == 255)
      webSocket.broadcastTXT(buf);
    else
      webSocket.sendTXT(target, buf);
  }
}


/**************************************************************************/
/*  WebSocket callbacks                                                   */
/**************************************************************************/
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
      case WStype_DISCONNECTED:
      {
        Serial.printf("[%u] Disconnected!\n", num);
      }
        break;
      case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
        break;
      case WStype_TEXT:
      {
        Serial.printf("WS message from %u:\n", num);
        Serial.printf("%s\n", payload);
      }
        break;
      default:  
        break;
    }
}
