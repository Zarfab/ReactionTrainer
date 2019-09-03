#include <Wire.h>
#include <VL6180X.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

#define VCC_MEASURE_PIN 34
#define HIGH_VCC 4200
#define LOW_VCC 3500
#define BUZZER_PIN 19
#define LED_PIN 23
#define NB_LEDS 12

VL6180X sensor;
CRGB leds[NB_LEDS];
CRGBPalette16 palette = RainbowColors_p;
uint8_t brightness = 50;
uint8_t startIndex = 0;
int channel = 0;




void setup() 
{
  Serial.begin(115200);
  delay(500);
  Serial.println("\nSetup");

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NB_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(brightness);
  
  Wire.begin();
  
  sensor.init();
  sensor.configureDefault();
  sensor.setTimeout(500);

  ledcSetup(channel, 440, 8);
  ledcAttachPin(BUZZER_PIN, channel);
  int freq = 60;
  for(int i = 0; i < 8; i++)
  {
    Serial.printf("Play tone at %iHz\n", freq);
    ledcWriteTone(channel, freq);
    delay(1000);
    freq *= 1.4;
  }
  ledcWriteTone(channel, 0);

  int batteryLevel = readVcc();
  Serial.printf("VCC = %dmV\n", batteryLevel);
  showBatteryLevel(batteryLevel);

  int fakeLevel = 3400;
  for(int i = 0; i < 10; i++)
  {
    Serial.printf("Show fake level = %dmV\n", fakeLevel);
    showBatteryLevel(fakeLevel);
    fakeLevel += 100;
    delay(2000);
  }
}




void loop() 
{ 
  uint16_t sensorVal = sensor.readRangeSingleMillimeters();
  if (sensor.timeoutOccurred()) {
  {
    Serial.println(" TIMEOUT"); }
  }
  else
  { 
    //Serial.println(sensorVal);
    if(sensorVal < 220) {
      ledcWriteTone(channel, 2 * sensorVal);
    }
    else {
      ledcWriteTone(channel, 0);
    }
  }

  FillLEDsFromPaletteColors(startIndex++);
  FastLED.show();
  Serial.printf("VCC = %dmV\n", readVcc());
}



void FillLEDsFromPaletteColors( uint8_t colorIndex)
{    
    for( int i = 0; i < NB_LEDS; i++) {
        leds[i] = ColorFromPalette( palette, colorIndex, 255, LINEARBLEND);
        colorIndex += 3;
    }
}


void showBatteryLevel(int level) {
  int localLevel = constrain(level, LOW_VCC, HIGH_VCC);
  int pos_led = map(localLevel, LOW_VCC, HIGH_VCC, 1, NB_LEDS-2);
    
  for (int i = NB_LEDS-1; i >= 0; i--){
    if (i < pos_led)
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
  FastLED.delay(1000);
}


int readVcc() {
  int rawVal = analogRead(VCC_MEASURE_PIN);
  return (rawVal * 3300 * 2) / 4096; // * 2 because of resitor divider
}
