#ifndef __LEDS_CPP
#define __LEDS_CPP
/*
	LED animations
	Microcontroller code for Flit
	Nate Marx
	2016
*/

#include <Arduino.h>
#include <FastLED.h>
#include "leds.h"

struct CRGB leds_left[NUM_LEDS];
struct CRGB leds_right[NUM_LEDS];

// Available Palette List:
// RainbowColors_p,
// RainbowStripeColors_p,
// OceanColors_p,
// CloudColors_p,
// LavaColors_p,
// ForestColors_p,
//PartyColors_p

void SetupPalette()
{
   currentPalette = CloudColors_p;
   noise_speed =  4;
   noise_scale = 30;
   colorLoop = 0;  
}

void SetupRandomPalette()
{
  currentPalette = CRGBPalette16( 
                      CHSV( random8(), 255, 32), 
                      CHSV( random8(), 255, 255), 
                      CHSV( random8(), 128, 255), 
                      CHSV( random8(), 255, 255)); 
}



//xymapper - courtesy mark kreigman
uint16_t XY( uint8_t x, uint8_t y)
{
  bool kMatrixSerpentineLayout = true;
  uint16_t i;
  if( kMatrixSerpentineLayout == false) {
    i = (y * 2) + x;
  }
  if( kMatrixSerpentineLayout == true) {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (2 - 1) - x;
      i = (y * 2) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * 2) + x;
    }
  }
  return i;
}

void fillnoise8() {
  uint8_t dataSmoothing = 0;
  if( noise_speed < 50) {
    dataSmoothing = 200 - (noise_speed * 4);
  }
  
  for(int i = 0; i < 31; i++) {
    int ioffset = noise_scale * i;
    for(int j = 0; j < 31; j++) {
      int joffset = noise_scale * j;
      
      uint8_t data = inoise8(x + ioffset,y + joffset,z);

      // The range of the inoise8 function is roughly 16-238.
      // These two operations expand those values out to roughly 0..255
      // You can comment them out if you want the raw noise data.
      data = qsub8(data,16);
      data = qadd8(data,scale8(data,39));

      if( dataSmoothing ) {
        uint8_t olddata = noise[i][j];
        uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
        data = newdata;
      }
      
      noise[i][j] = data;
    }
  }
  
  z += noise_speed;
  
  // apply slow drift to X and Y, just for visual variation.
  x += noise_speed / 8;
  y -= noise_speed / 16;
}

void mapNoiseToLEDsUsingPalette()
{
  static uint8_t ihue=0;
  
  for(int i = 0; i < 2; i++) {
    for(int j = 0; j < 31; j++) {
 
      uint8_t index = noise[j][i];
      uint8_t bri =   noise[i][j];

      // if this palette is a 'loop', add a slowly-changing base value
      if( colorLoop) { 
        index += ihue;
      }

      // brighten up, as the color palette itself often contains the 
      // light/dark dynamic range desired
      if( bri > 127 ) {
        bri = 255;
      } else {
        bri = dim8_raw( bri * 2);
      }

      CRGB color = ColorFromPalette( currentPalette, index, bri);
      leds_left[XY(i,j)] = color;
      leds_right[XY(i,j)] = color;
    }
  }
  
  ihue+=1;
}

void breathe()
{
    if (!modeinit)
    {
#ifdef DEBUG
        Serial.println(F("Init: BREATHE"));
#endif
        FastLED.clear();
        fill_solid(leds_left, NUM_LEDS, CHSV(0, 0, 255));
        fill_solid(leds_right, NUM_LEDS, CHSV(0, 0, 255));
        FastLED.setBrightness(globalBrightness);
        modeinit = true;
    }
    uint8_t sat = 255;
    if (breatheHue == 0)
        sat = 0;
    float breath = (exp(sin(millis() / 4000.0 * PI)) - 0.36787944) * 108.0;
    fill_solid(leds_left, NUM_LEDS, CHSV(breatheHue, sat, 255));
    fill_solid(leds_right, NUM_LEDS, CHSV(breatheHue, sat, 255));

    FastLED.setBrightness((breath/255)*globalBrightness);
    FastLED.show();
}

void rainbow_beat() {
    if (!modeinit)
    {
        Serial.println(F("Init: RAINBOWBEAT"));
        FastLED.setBrightness(globalBrightness);
        modeinit = true;
    }
    uint8_t beatA = beatsin8(4, 0, 255);
    uint8_t beatB = beatsin8(13, 4.1803, 4.1803);
    fill_rainbow(leds_left, NUM_LEDS, beatA, beatB);
    fill_rainbow(leds_right, NUM_LEDS, beatA, beatB);
    FastLED.show();
} // rainbow_beat()

void ripple() {
    if (!modeinit)
    {
#ifdef DEBUG
        Serial.println(F("Init: Ripple"));
#endif
        FastLED.setBrightness(globalBrightness);
        modeinit = true;
    }

    for (int i = 0; i < NUM_LEDS; i++) leds_left[i] = CHSV(bgcol++, 255, 15);
    for (int i = 0; i < NUM_LEDS; i++) leds_right[i] = CHSV(bgcol++, 255, 15);

    switch (step) {

    case -1:
        center = random(NUM_LEDS);
        colour = random8();
        step = 0;
        break;

    case 0:
        leds_left[center] = CHSV(colour, 255, 255);
        leds_right[center] = CHSV(colour, 255, 255);
        step ++;
        break;

    case maxsteps:
        step = -1;
        break;

    default:
        leds_left[(center + step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, 255 / step * 2);
        leds_left[(center - step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, 255 / step * 2);
        leds_right[(center + step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, 255 / step * 2);
        leds_right[(center - step + NUM_LEDS) % NUM_LEDS] += CHSV(colour, 255, 255 / step * 2);
        step ++;
        break;
    }
} // ripple()

void spring()
{

//spring
    int rule = 60;
    int automaton[NUM_LEDS];

    if (!modeinit)
    {
#ifdef DEBUG
        Serial.println(F("Init: SPRING"));
#endif
        FastLED.clear();
        FastLED.setBrightness(globalBrightness);
        fill_solid(leds_left, NUM_LEDS, CHSV(128, 255, 15));
        fill_solid(leds_right, NUM_LEDS, CHSV(128, 255, 15));
        modeinit = true;
    }
    int next_generation[NUM_LEDS];
    for (int i = 0; i < NUM_LEDS; i++) {
        next_generation[i] = automaton[i] / 4;

        // Orient ourselves
        int i_left = i - 1;
        if (i_left < 0) {
            i_left = NUM_LEDS;
        }
        int i_right = i + 1;
        if (i_right > NUM_LEDS) {
            i_right = 0;
        }

        // Calculate the next generation
        if (automaton[i_left] <= 32 && automaton[i] <= 32 && automaton[i_right] <= 32) {
            if (rule & (1 << 0)) {
                next_generation[i] = 64;
            }
        }
        if (automaton[i_left] <= 32 && automaton[i] <= 32 && automaton[i_right] > 32) {
            if (rule & (1 << 1)) {
                next_generation[i] = 64;
            }
        }
        if (automaton[i_left] <= 32 && automaton[i] > 32 && automaton[i_right] <= 32) {
            if (rule & (1 << 2)) {
                next_generation[i] = 64;
            }
        }
        if (automaton[i_left] <= 32 && automaton[i] > 32 && automaton[i_right] > 32) {
            if (rule & (1 << 3)) {
                next_generation[i] = 64;
            }
        }
        if (automaton[i_left] > 32 && automaton[i] <= 32 && automaton[i_right] <= 32) {
            if (rule & (1 << 4)) {
                next_generation[i] = 64;
            }
        }
        if (automaton[i_left] > 32 && automaton[i] <= 32 && automaton[i_right] > 32) {
            if (rule & (1 << 5)) {
                next_generation[i] = 64;
            }
        }
        if (automaton[i_left] > 32 && automaton[i] > 32 && automaton[i_right] <= 32) {
            if (rule & (1 << 6)) {
                next_generation[i] = 64;
            }
        }
        if (automaton[i_left] > 32 && automaton[i] > 32 && automaton[i_right] > 32) {
            if (rule & (1 << 7)) {
                next_generation[i] = 64;
            }
        }

    }

    // Re-seed
    for (int i = 0; i < NUM_LEDS; i++) {
        automaton[i] = next_generation[i];
    }

    // Paint this generation
    show_automaton(automaton);
}

void show_automaton(int automaton[]) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds_left[i].r = automaton[i]/10;
        leds_right[i].r = automaton[i]/10;
        leds_left[i].g = automaton[i];
        leds_right[i].g = automaton[i];
        leds_left[i].b = automaton[i]/10;
        leds_right[i].b = automaton[i]/10;
    }

    fadeToBlackBy(leds_left, NUM_LEDS, 64);
    fadeToBlackBy(leds_right, NUM_LEDS, 64);
    FastLED.show();
} //spring()


void summer()
{
    if (!modeinit)
    {
        FastLED.setBrightness(globalBrightness);
      
#ifdef DEBUG
        Serial.println(F("Init: SUMMER"));
#endif
        x = random16();
        y = random16();
        z = random16();
        modeinit = true;
    }
    
  SetupPalette();
  fillnoise8();
  mapNoiseToLEDsUsingPalette();
  LEDS.show();
  // delay(10);
}

void fall()
{
    if (!modeinit)
    {
#ifdef DEBUG
        Serial.println(F("Init: FALL"));
#endif
        FastLED.setBrightness(globalBrightness);
        modeinit = true;
    }

}


void winter()
{
    if (!modeinit)
    {
#ifdef DEBUG
        Serial.println(F("Init: WINTER"));
#endif
        FastLED.setBrightness(globalBrightness);
        modeinit = true;
    }

    int gr[9] = {50, 100, 150, 200, 255, 200, 150, 100, 50}; //highlights
    int i, j, k;
    fill_solid(leds_left, NUM_LEDS, CHSV(240, 20, 128));
    fill_solid(leds_right, NUM_LEDS, CHSV(240, 20, 128));
    for (i = 0 ; i < NUM_LEDS; i++ ) { //fill with blue
        leds_left[i].b = 192;
        leds_right[i].b = 192;
    }

    for (i=0; i<NUM_LEDS; i+=20) { //add the green
        for (j=0; j<9; j++) {
            leds_left[i+j].g =gr[j];
            leds_right[i+j].g =gr[j];
            leds_left[i+j].r =gr[j];
            leds_right[i+j].r =gr[j];
        }
    }
    for (j=0; j<NUM_LEDS; j++) { //rotate display
        leds_left[NUM_LEDS]=leds_left[0];
        leds_right[NUM_LEDS]=leds_right[0];
        for ( i=0; i<NUM_LEDS; i++) {
            leds_left[i]=leds_right[i+1];
            leds_left[i]=leds_right[i+1];
        }
    }
}



void nextAnimation()
{
    reset_mode();
    if (flitmode >= 6)
        flitmode = 0;
    else
        ++flitmode;
#ifdef DEBUG
    Serial.print(F("mode: "));
    Serial.println(flitmode);
#endif
}

void reset_mode()
{
    FastLED.clear();
    FastLED.setBrightness(0);
    modeinit = false;
    FastLED.show();
}

void LEDLoop()
{
    if (rotate && (millis() > (modeTimer + 3600000)))
    {
#ifdef DEBUG
        -     Serial.println(flitmode);
#endif
        nextAnimation();
#ifdef DEBUG
        Serial.println(flitmode);
#endif
        modeTimer = millis();
    }

    switch (flitmode)
    {
    case 0:
        breathe();
        break;
    case 1:
        EVERY_N_MILLIS(thisdelay)
        {            ripple();        }
        break;
    case 2:
        rainbow_beat();
        break;
    case 3:
        spring();
        break;
    case 4:
        summer();
        break;
    case 5:
        fall();
        break;
    case 6:
        EVERY_N_MILLIS(thisdelay)
        {            winter();        }
        break;
    }
    FastLED.show();
}

void setFlitMode(uint8_t newmode)
{
	if (newmode < 7)
	{
		reset_mode();
		modeinit = false;
		flitmode = newmode;
	}
}

void LEDSetup()
{
    LEDS.addLeds<LED_TYPE, LED_PIN_L, COLOR_ORDER>(leds_left, NUM_LEDS);
    LEDS.addLeds<LED_TYPE, LED_PIN_R, COLOR_ORDER>(leds_right, NUM_LEDS);
    reset_mode();
    fill_solid(leds_left, NUM_LEDS, CRGB::Black);
    fill_solid(leds_right, NUM_LEDS, CRGB::Black);
    FastLED.show();
    modeinit = false;
    flitmode = 0;
    modeTimer = millis();
}

#endif //__LEDS_CPP
