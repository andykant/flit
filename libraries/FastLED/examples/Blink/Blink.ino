#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 1

  #define DATA_PIN 6
CRGB leds[NUM_LEDS];

void setup() { 
FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS); 	  
}

void loop() { 
  // Turn the LED on, then pause
  leds[0] = CRGB::Red;
  FastLED.show();
  delay(200);
  leds[0] = CRGB::Green;
  FastLED.show();
  delay(200);
  leds[0] = CRGB::Blue;
  FastLED.show();
  delay(200);
}
