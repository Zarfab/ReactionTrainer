#include "ReactionTrainerSlave.h"

ReactionTrainerSlave::ReactionTrainerSlave() :
  brightness(100), groupColor(0xff0000), shotT0(-1)
{
  defaultShot.distanceThreshold = 200;
  defaultShot.timeout = 4000;
  defaultShot.order = SIMULTANEOUS;
  
  defaultShot.colorDuration = 1500;
  defaultShot.colorPalette[0] = CRGB::Blue;
  defaultShot.colorAnim = COLOR_ANIM_NONE;
  
  defaultShot.soundDuration = 0;
  defaultShot.soundFreq = -1;
}



void ReactionTrainerSlave::init()
{
  ///// SETUP FASTLED AND DISPLAY BATTERY LEVEL /////
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NB_LED).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(brightness);
  showBatteryLevel();
  FastLED.delay(1000);
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
  JsonVariant brightnessJson = doc["brightness"];
  JsonVariant groupColorJson = doc["groupColor"];
  if(!brightnessJson.isNull() && !groupColorJson.isNull()) {
    brightness = brightnessJson.as<uint8_t>();
    groupColor = groupColorJson.as<int>();
    FastLED.setBrightness(brightness);
    fill_solid(leds, NB_LED, groupColor);
    FastLED.show();
    FastLED.delay(1000);
  }
  else
  {
    shot.distanceThreshold = doc["distanceThreshold"] | defaultShot.distanceThreshold;
    shot.timeout = doc["timeout"] | defaultShot.timeout;
    shot.order = doc["order"].as<uint8_t>() | defaultShot.order;
  
    shot.colorDuration = doc["colorDuration"] | defaultShot.colorDuration;
    shot.colorAnim = doc["colorAnim"].as<uint8_t>() | defaultShot.colorAnim;
    JsonArray palette = doc["colorPalette"];
    if(palette.isNull())
    {
      shot.colorPalette =  defaultShot.colorPalette;
    }
    else
    {
      int i = 0;
      for(JsonVariant v : palette) 
      {
        shot.colorPalette[i] = v.as<int>();
        i++;
        if(i == 16) break;
      }
      for(i; i < 16; i++) {
         shot.colorPalette[i] = CRGB::Black;
      }
    }
  
    shot.soundDuration = doc["soundDuration"] | defaultShot.soundDuration;
    shot.soundFreq = doc["soundFreq"] | defaultShot.soundFreq;
  }
}



void ReactionTrainerSlave::showBatteryLevel()
{
  // read VCC
  int rawVal = analogRead(VCC_MEASURE_PIN);
  int mvVal = round((rawVal * 3300.0 * 2) / 4096.0); // * 2 because of resitor divider
  Serial.printf("VCC = %dmV\n", mvVal);
  
  int localLevel = constrain(mvVal, LOW_VCC, HIGH_VCC);
  int pos = map(localLevel, LOW_VCC, HIGH_VCC, 1, NB_LED-2);
    
  for (int i = NB_LED-1; i >= 0; i--){
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
