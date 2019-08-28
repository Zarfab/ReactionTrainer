#include "ReactionTrainerSlave.h"

ReactionTrainerSlave::ReactionTrainerSlave() :
  brightness(100), groupColor(0xff0000), shotT0(-1)
{
  
}



void ReactionTrainerSlave::init()
{
  ///// SETUP FASTLED AND DISPLAY BATTERY LEVEL /////
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NB_LED).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(brightness);
  showBatteryLevel();
  delay(1000);
  // setul VL6180X sensor
  sensor.init();
  sensor.configureDefault();
  sensor.setTimeout(100);
}



String ReactionTrainerSlave::update()
{
  String str = "";
  if(shotT0 >= 0) {
    uint16_t val = sensor.readRangeSingle();
    if (val <= shot.distanceThreshold) {
      uint32_t reactTime = millis() - shotT0;
      Serial.printf("got touched in %d ms\n", reactTime);
      shotT0 = -1;
    }
    if(millis() - shotT0 > shot.timeout) {
      Serial.println("Not touched in timeout");
      shotT0 = -1;
    }
  }
  return str;
}



void ReactionTrainerSlave::handleJson(const JsonObject& doc)
{
  
}



void ReactionTrainerSlave::showBatteryLevel()
{
  // read VCC
  int rawVal = analogRead(VCC_MEASURE_PIN);
  int mvVal = round((rawVal * 3300.0 * 2) / 4096.0); // * 2 because of resitor divider
  Serial.printf("VCC = %dmV\n", batteryLevel);
  
  int localLevel = constrain(mvVal, LOW_VCC, HIGH_VCC);
  int pos = map(localLevel, LOW_VCC, HIGH_VCC, 1, NB_LEDS-2);
    
  for (int i = NB_LEDS-1; i >= 0; i--){
    if (i < pos)
    {
      if (i < 3)
      {
        leds[i]=CRGB::Red;
      }
      else if (i < 6)
      {
        leds[i]=CRGB::Orange;
      }
      else 
      {
        leds[i]=CRGB::Green;
      }
    }
    else 
    {
      leds[i]=CRGB::Black;
    }
  }
  FastLED.show();  
}
