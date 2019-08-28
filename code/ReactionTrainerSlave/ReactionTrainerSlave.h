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



enum ShotEventOrder
{
  SIMULTANEOUS,
  COLOR_BEFORE,
  SOUND_BEFORE
};

enum ColorAnimation
{
  STILL,
  BLINK,
  TIMER,
  STOPWATCH,
  LOADING,
  WHEEL
};



struct Shot
{
  uint16_t distanceThreshold; // max distance to activate sensor in mm (30 - 250)
  uint32_t timeout; // time to wait before sending failed message (ms)
  ShotEventOrder order;
  
  uint32_t colorDuration; // how long color animation is playing (ms)
  CRGBPalette16 colorPalette; // colors to display during animation
  ColorAnimation coloAnim; // type of animation
  
  uint32_t soundDuration; // how long sound animation is playing (ms)
  int32_t soundFreq;
};



class ReactionTrainerSlave
{
  protected:
    Shot shot;
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

/*CRGB htmlToCRGB(const char* htmlString) {
  return (int) strtol(htmlString + 1, NULL, 16);
}*/

#endif // REACTION_TRAINER_SLAVE_H
