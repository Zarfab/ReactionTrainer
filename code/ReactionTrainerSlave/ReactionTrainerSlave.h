#ifndef REACTION_TRAINER_SLAVE_H
#define REACTION_TRAINER_SLAVE_H

#include <ArduinoJson.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include <Wire.h>
#include <VL6180X.h>


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
  
  uint32_t colorDuration; // how long color animation is playing (ms)
  CRGBPalette16 colorPalette; // colors to display during animation
  uint8_t colorAnim; // type of animation
  
  uint32_t soundDuration; // how long sound animation is playing (ms)
  int32_t soundFreq;
};



class ReactionTrainerSlave
{
  protected:
    Shot shot;
    Shot defaultShot;
    int64_t shotT0;
    
    uint8_t brightness = 100;
    CRGB groupColor = 0xff0000;
    
    VL6180X sensor;

  public:
    CRGB leds[NB_LED];
        
    ReactionTrainerSlave();
    void init();
    String update();
    void handleJson(const JsonObject& doc);
    void showBatteryLevel();
};

#endif // REACTION_TRAINER_SLAVE_H
