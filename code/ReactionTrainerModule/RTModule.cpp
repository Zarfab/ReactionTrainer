#include "RTModule.h"

RTModule::RTModule() :
  brightness(32), groupColor(0xff0000), presentSelf(false), shotT0(-1), buzzerOn(false)
{
  defaultShot.distanceThreshold = 200;
  defaultShot.timeout = 4000;
  defaultShot.order = SIMULTANEOUS;
  
  defaultShot.colorDuration = 1500;
  fill_solid(defaultShot.colorPalette, 16, CRGB::Blue);
  defaultShot.colorAnim = COLOR_ANIM_NONE;
  
  defaultShot.soundDuration = 0;
  defaultShot.soundFreq = -1;
}



void RTModule::init()
{
  ///// SETUP FASTLED AND DISPLAY BATTERY LEVEL /////
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NB_LED).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(brightness);
  showBatteryLevel();
  FastLED.delay(1000);
  // setup VL6180X sensor
  Wire.begin();
  sensor.init();
  sensor.configureDefault();
  sensor.setTimeout(200);
  // setup buzzer
  ledcSetup(buzzerChannel, 440, 8);
  ledcAttachPin(BUZZER_PIN, buzzerChannel);
}



String RTModule::update()
{
  String str = "";
  if(presentSelf) {
    StaticJsonDocument<64> doc;
    doc["type"] = "RTModule";
    doc["version"] = RT_VERSION;
    char buf[64];
    serializeJson(doc, buf);
    str += buf;
    presentSelf = false;
    return str;
  }
  if(shotT0 >= 0) {
    int64_t reactTime = millis() - shotT0;
    // play sound
    if(!buzzerOn && shot.soundFreq > 20 && reactTime >= shot.soundT0) {
      ledcWriteTone(buzzerChannel, shot.soundFreq);
      buzzerOn = true;
    }
    // stop sound;
    if(buzzerOn && reactTime >= shot.soundTEnd) {
      ledcWriteTone(buzzerChannel, 0);
      buzzerOn = false;
      shot.soundFreq = -1; // prevent from playing again
    }
    // play color animation
    if(reactTime >= shot.colorT0 && reactTime < shot.colorTEnd) {
      int64_t animTime = reactTime - shot.colorT0;
      float animProgress = animTime / (float)(shot.colorDuration);
      int paletteIndex = round(255.0 * animProgress);
      CRGB currentColor = ColorFromPalette(shot.colorPalette, paletteIndex);
      switch(shot.colorAnim) {
        case COLOR_ANIM_NONE:
          fill_solid(leds, NB_LED, shot.colorPalette[0]);
          break;
        case COLOR_ANIM_STILL:
          fill_solid(leds, NB_LED, currentColor);
          break;
        case COLOR_ANIM_BLINK:
        {
          bool isBlack = animTime % 200 >= 100;
          fill_solid(leds, NB_LED, isBlack? CRGB::Black : currentColor);
        }
          break;
        case COLOR_ANIM_TIMER:
        {
          for(int i = 0; i < NB_LED; i++) {
            if(i <= floor(animProgress * NB_LED)) {
              leds[i] = currentColor;
            }
            else {
              leds[i] = CRGB::Black;
            }
          }
        }
          break;
        case COLOR_ANIM_STOPWATCH:
        {
          for(int i = 0; i < NB_LED; i++) {
            if(i <= floor((1.0 - animProgress) * NB_LED)) {
              leds[i] = currentColor;
            }
            else {
              leds[i] = CRGB::Black;
            }
          }
        }
          break;
        case COLOR_ANIM_LOADING:
        {
          fill_solid(leds, NB_LED, CRGB::Black);
          int firstLed = (NB_LED * animTime / 492) % NB_LED;
          for(int i = firstLed; i < firstLed + NB_LED / 2; i++) {
            leds[i % NB_LED] = currentColor;
          }
        }
          break;
        case COLOR_ANIM_WHEEL:
        {
          fill_solid(leds, NB_LED, CRGB::Black);
          int firstLed = (NB_LED * animTime / 2000) % 4;
          for(int i = firstLed; i < NB_LED; i += 4) {
              leds[i] = currentColor;
          }
          break;
        }
        default:
          break;
      }
    }
    else {
      fill_solid(leds, NB_LED, CRGB::Black);
    }
    
    // read sensor value
    uint16_t val = sensor.readRangeSingle();
    if (val <= shot.distanceThreshold || reactTime > shot.timeout) {
      StaticJsonDocument<64> doc;
      doc["result"] = val <= shot.distanceThreshold? 1 : 0;
      doc["reactionTime"] = reactTime;
      char buf[64];
      serializeJson(doc, buf);
      str += buf;
      shotT0 = -1;
      fill_solid(leds, NB_LED, CRGB::Black);
      ledcWriteTone(buzzerChannel, 0);
      buzzerOn = false; 
    }
  }
  return str;
}



void RTModule::handleJson(const JsonObject& doc)
{
  JsonVariant brightnessJson = doc["brightness"];
  JsonVariant groupColorJson = doc["groupColor"];
  if(!brightnessJson.isNull() && !groupColorJson.isNull()) {
    brightness = brightnessJson.as<uint8_t>();
    groupColor = groupColorJson.as<int>();
    presentSelf = strcmp(doc["answerExpected"], "true") == 0;
    FastLED.setBrightness(brightness);
    fill_solid(leds, NB_LED, groupColor);
    FastLED.show();
    FastLED.delay(1000);
    fill_solid(leds, NB_LED, CRGB::Black);
    FastLED.show();
  }
  else
  {
    // configure and measure reaction time
    shot.distanceThreshold = doc["distanceThreshold"] | defaultShot.distanceThreshold;
    shot.timeout = doc["timeout"] | defaultShot.timeout;
    shot.order = doc["order"].as<uint8_t>() | defaultShot.order;
  
    shot.colorDuration = doc["colorDuration"] | defaultShot.colorDuration;
    shot.colorAnim = doc["colorAnim"].as<uint8_t>() | defaultShot.colorAnim;
    JsonArray palette = doc["colorPalette"];
    if(palette.isNull())
    {
      CRGB colorBeg = doc["colorBegin"].as<int>();
      if(colorBeg) {
        CRGB colorEnd = doc["colorEnd"].as<int>();
        if(colorEnd) {
          fill_gradient_RGB(shot.colorPalette, 16, colorBeg, colorEnd);
        }
        else {
          fill_solid(shot.colorPalette, 16, colorBeg);
        }
      }
      else {
        shot.colorPalette =  defaultShot.colorPalette;
      }
    }
    else
    {
      fill_solid(shot.colorPalette, 16, CRGB::Black);
      int i = 0;
      for(JsonVariant v : palette) 
      {
        shot.colorPalette[i] = v.as<int>();
        i++;
        if(i == 16) break;
      }
    }
  
    shot.soundDuration = doc["soundDuration"] | defaultShot.soundDuration;
    shot.soundFreq = doc["soundFreq"] | defaultShot.soundFreq;

    switch(shot.order) {
      case COLOR_BEFORE:
        shot.colorT0 = 0;
        shot.colorTEnd = shot.colorDuration;
        shot.soundT0 = shot.colorDuration;
        shot.soundTEnd = shot.colorDuration + shot.soundDuration;
        break;
      case SOUND_BEFORE:
        shot.colorT0 = shot.soundDuration;
        shot.colorTEnd = shot.soundDuration + shot.colorDuration;
        shot.soundT0 = 0;
        shot.soundTEnd = shot.soundDuration;
        break;
      default:
        shot.colorT0 = 0;
        shot.colorTEnd = shot.colorDuration;
        shot.soundT0 = 0;
        shot.soundTEnd = shot.soundDuration;
        break;
    }
    shotT0 = millis();
  }
}



void RTModule::showBatteryLevel()
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
