#ifndef REACTION_TRAINER_SLAVE_H
#define REACTION_TRAINER_SLAVE_H

#include <ArduinoJson.h>
#include <StreamString.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include <Wire.h>
#include <VL6180X.h>


#define RT_VERSION "0.0.1"

#define VCC_MEASURE_PIN 34
#define HIGH_VCC 4200
#define LOW_VCC 3500
#define BUZZER_PIN 19
#define LED_PIN 23
#define NB_LED 12



#define SIMULTANEOUS 0
#define COLOR_BEFORE 1
#define SOUND_BEFORE 2

#define COLOR_ANIM_NONE 0
#define COLOR_ANIM_STILL 1
#define COLOR_ANIM_BLINK 2
#define COLOR_ANIM_TIMER 3
#define COLOR_ANIM_STOPWATCH 4
#define COLOR_ANIM_LOADING 5
#define COLOR_ANIM_WHEEL 6



struct Shot
{
  uint16_t distanceThreshold; // max distance to activate sensor in mm (30 - 250)
  uint32_t timeout; // time to wait before sending failed message (ms)
  uint8_t order;
  uint32_t colorT0;
  uint32_t colorTEnd;
  uint32_t soundT0;
  uint32_t soundTEnd;
  
  uint32_t colorDuration; // how long color animation is playing (ms)
  CRGBPalette16 colorPalette; // colors to display during animation
  uint8_t colorAnim; // type of animation
  
  uint32_t soundDuration; // how long sound animation is playing (ms)
  int32_t soundFreq;
};



class RTModule
{
  protected:
    Shot shot;
    Shot defaultShot;
    int64_t shotT0;
    bool buzzerOn;
    int buzzerChannel = 0;
    
    uint8_t brightness;
    CRGB groupColor;
    bool presentSelf;
    
    VL6180X sensor;

  public:
    CRGB leds[NB_LED];
        
    RTModule();
    void init();
    String update();
    void handleJson(const JsonObject& doc);
    void showBatteryLevel();
};

#endif // REACTION_TRAINER_SLAVE_H
