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

const uint8_t kMatrixWidth  = 2;
const uint8_t kMatrixHeight = 31;
const bool    kMatrixSerpentineLayout = true;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
#define MAX_DIMENSION ((kMatrixWidth>kMatrixHeight) ? kMatrixWidth : kMatrixHeight)

// The 16 bit version of our coordinates
extern uint16_t x;
extern uint16_t y;
extern uint16_t z;


void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}


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


void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;
  
  currentPalette = CRGBPalette16( 
    green,  green,  black,  black,
    purple, purple, black,  black,
    green,  green,  black,  black,
    purple, purple, black,  black );
}


#define HOLD_PALETTES_X_TIMES_AS_LONG 10
void ChangePaletteAndSettingsPeriodically()
{
  uint8_t secondHand = ((millis() / 1000) / HOLD_PALETTES_X_TIMES_AS_LONG) % 60;
  static uint8_t lastSecond = 99;
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    if( secondHand ==  0)  { currentPalette = RainbowColors_p;         noise_speed = 20; noise_scale = 30; colorLoop = 1; }
    if( secondHand ==  5)  { SetupPurpleAndGreenPalette();             noise_speed = 10; noise_scale = 50; colorLoop = 1; }
    if( secondHand == 10)  { SetupBlackAndWhiteStripedPalette();       noise_speed = 20; noise_scale = 30; colorLoop = 1; }
    if( secondHand == 15)  { currentPalette = ForestColors_p;          noise_speed =  8; noise_scale =120; colorLoop = 0; }
    if( secondHand == 20)  { currentPalette = CloudColors_p;           noise_speed =  4; noise_scale = 30; colorLoop = 0; }
    if( secondHand == 25)  { currentPalette = LavaColors_p;            noise_speed =  8; noise_scale = 50; colorLoop = 0; }
    if( secondHand == 30)  { currentPalette = OceanColors_p;           noise_speed = 20; noise_scale = 90; colorLoop = 0; }
    if( secondHand == 35)  { currentPalette = PartyColors_p;           noise_speed = 20; noise_scale = 30; colorLoop = 1; }
    if( secondHand == 40)  { SetupRandomPalette();                     noise_speed = 20; noise_scale = 20; colorLoop = 1; }
    if( secondHand == 45)  { SetupRandomPalette();                     noise_speed = 50; noise_scale = 50; colorLoop = 1; }
    if( secondHand == 50)  { SetupRandomPalette();                     noise_speed = 90; noise_scale = 90; colorLoop = 1; }
    if( secondHand == 55)  { currentPalette = RainbowStripeColors_p;   noise_speed = 30; noise_scale = 20; colorLoop = 1; }
  }
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

      if( colorLoop) { 
        index += ihue;
      }

      if( bri > 127 ) {
        bri = 255;
      } else {
        bri = dim8_raw( bri * 2 );
      }

      CRGB color = ColorFromPalette( currentPalette, index, bri);
      leds_left[XY(i,j)] = color;
      leds_right[XY(i,j)] = color;
    }
  }
  
  ihue+=1;
}


uint8_t breatheHue = 0;

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
        //FastLED.setBrightness(globalBrightness);
        FastLED.setBrightness(200);
        modeinit = true;
    }
    uint8_t sat = 255;
    if (breatheHue == 0)
        sat = 0;
    float breath = (exp(sin(millis() / 4000.0 * PI)) - 0.36787944) * 108.0;
    fill_solid(leds_left, NUM_LEDS, CHSV(breatheHue, sat, 255));
    fill_solid(leds_right, NUM_LEDS, CHSV(breatheHue, sat, 255));

    //Serial.println(breath);
    //Serial.println(globalBrightness);
//    FastLED.setBrightness((breath/255)*globalBrightness);
    FastLED.setBrightness(breath);
    FastLED.show();
}

uint8_t rainbowWidth = 255;
void rainbow_beat() {
    if (!modeinit)
    {
        Serial.println(F("Init: RAINBOWBEAT"));
        FastLED.setBrightness(200);
        modeinit = true;
        rainbowWidth = 255;
    }
//    float wid = rainbowWidth/255;
//    int bpmA = int(4 * wid);
    uint8_t beatA = beatsin8(4, 0, 255);
    uint8_t beatB = beatsin8(13, 4.1803, 4.1803);
    fill_rainbow(leds_left, NUM_LEDS, beatA, beatB);
    fill_rainbow(leds_right, NUM_LEDS, beatA, beatB);
//	  FastLED.setBrightness(globalBrightness);
    FastLED.show();
} // rainbow_beat()


void wow()
{
	if(!modeinit)
	{
	  // Initialize our coordinates to some random values
	  x = random16();
	  y = random16();
	  z = random16();
	  modeinit = true;
    FastLED.setBrightness(200);
	}
	ChangePaletteAndSettingsPeriodically();
	fillnoise8();
	mapNoiseToLEDsUsingPalette();
	LEDS.show();
}

/*
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
void spring_()
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
        if (automaton[i_left] <= 61 && automaton[i] <= 61 && automaton[i_right] <= 61) {
            if (rule & (1 << 0)) {
                next_generation[i] = 64;
            }
        }
        if (automaton[i_left] <= 61 && automaton[i] <= 61 && automaton[i_right] > 61) {
            if (rule & (1 << 1)) {
                next_generation[i] = 64;
            }
        }
        if (automaton[i_left] <= 61 && automaton[i] > 61 && automaton[i_right] <= 61) {
            if (rule & (1 << 2)) {
                next_generation[i] = 64;
            }
        }
        if (automaton[i_left] <= 61 && automaton[i] > 61 && automaton[i_right] > 61) {
            if (rule & (1 << 3)) {
                next_generation[i] = 64;
            }
        }
        if (automaton[i_left] > 61 && automaton[i] <= 61 && automaton[i_right] <= 61) {
            if (rule & (1 << 4)) {
                next_generation[i] = 64;
            }
        }
        if (automaton[i_left] > 61 && automaton[i] <= 61 && automaton[i_right] > 61) {
            if (rule & (1 << 5)) {
                next_generation[i] = 64;
            }
        }
        if (automaton[i_left] > 61 && automaton[i] > 61 && automaton[i_right] <= 61) {
            if (rule & (1 << 6)) {
                next_generation[i] = 64;
            }
        }
        if (automaton[i_left] > 61 && automaton[i] > 61 && automaton[i_right] > 61) {
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
    delay(20);
} //spring()

*/

long d;
long e;


void spring()
{
    if (!modeinit)
    {
#ifdef DEBUG
        Serial.println(F("Init: SPRING"));
#endif
        FastLED.setBrightness(200);
        modeinit = true;
    }

    fadeToBlackBy( leds_left, NUM_LEDS, 10);
    fadeToBlackBy( leds_right, NUM_LEDS, 10);
    if(( millis() - d) > 5)
    { 
      leds_left[ random16( NUM_LEDS ) ] += CHSV( random8(90,120), random8(220,255), 255);
      leds_right[ random16( NUM_LEDS ) ] += CHSV( random8(90,120), random8(220,255), 255);
      d = millis();
    }
    if(( millis() - e) > 20)
    { 
      leds_left[ random16( NUM_LEDS ) ] += CHSV( random8(0,60), random8(200,255), 255);
      leds_right[ random16( NUM_LEDS ) ] += CHSV( random8(0,60), random8(200,255), 255);
      e = millis();
    }
    FastLED.show();
    FastLED.delay(5);
}

void summer()
{
    if (!modeinit)
    {
        FastLED.setBrightness(200);
      
#ifdef DEBUG
        Serial.println(F("Init: SUMMER"));
#endif
        x = random16(0, 255);
        y = random16(0, 255);
        z = random16(0,255);
        modeinit = true;
        SetupPalette();
	}

  noise_speed = 4;// * globalSpeed/255;

  fillnoise8();
  mapNoiseToLEDsUsingPalette();
  FastLED.show();
  // delay(10);
}

#define COOLING  55
#define SPARKING 120

void Fire2012()
{
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( false ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds_left[pixelnumber] = color;
      leds_right[pixelnumber] = color;
    }
}

#define FIRE_FRAMES_PER_SECOND 60
void fall()
{
    if (!modeinit)
    {
#ifdef DEBUG
        Serial.println(F("Init: FALL"));
#endif
//        FastLED.setBrightness(globalBrightness);
        FastLED.setBrightness(200);
        modeinit = true;
    }
 Fire2012();
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FIRE_FRAMES_PER_SECOND);
}


#define FRAMES_PER_SECOND  60
#define COOLING            2.9    
#define TWINKLING          400    
#define FLICKER            50     

static int beatInterval =  8912;  
long nextBeat =            0;
long nextTwinkle =         3000;  
unsigned int seeds =       0;
long loops =               0;
long deltaTimeTwinkle =    0;
long deltaTimeSparkle =    0;
boolean beatStarted =      false;

static byte heat[NUM_LEDS];

CHSV HeatColor_( uint8_t temperature)
{
  CHSV heatcolor;
  heatcolor.hue = 180; 
  heatcolor.saturation = random8(150,200); 
  heatcolor.value = temperature; 
  return heatcolor;
}

void Twinkle() 
{
  // Step 1. Create a random number of seeds
  random16_add_entropy( random()); //random8() isn't very random, so this mixes things up a bit
  seeds = random8(10,NUM_LEDS-10);

  // Step 2. "Cool" down every location on the strip a little
  for( int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i], COOLING);
  }

  // Step 3. Make the seeds into heat on the string
  for ( int j = 0 ; j < seeds ; j++) {
    if (random16() < TWINKLING) {
      //again, we have to mix things up so the same locations don't always light up
      random16_add_entropy( random()); 
      heat[random8(NUM_LEDS)] = random8(100,255);
    }
  }

  // Step 4. Add some "flicker" to LEDs that are already lit
  //         Note: this is most visible in dim LEDs
  for ( int k = 0 ; k < NUM_LEDS ; k++ ) {
    if (heat[k] > 0 && random8() < FLICKER) {
      heat[k] = qadd8(heat[k] , 10);
    }
  }

  // Step 5. Map from heat cells to LED colors
  for( int j = 0; j < NUM_LEDS; j++) 
  {
    leds_left[j] = HeatColor_( heat[j] );
    leds_right[j] = HeatColor_( heat[j] );
  }
  nextTwinkle += 1000 / FRAMES_PER_SECOND ; // assign the next time Twinkle() should happen
}

void Sparkle() {
  // Step 1. Make a random numnber of seeds
  seeds = random8(NUM_LEDS - 20 ,NUM_LEDS);

  // Step 2. Increase the heat at those locations
  for ( int i = 0 ; i < seeds ; i++) {
    {
      int pos = random8(NUM_LEDS);
      random16_add_entropy( random());
      heat[pos] = random8(150,255);
    }
  }
  nextBeat += beatInterval; // assign the next time Twinkle() should happen
  loops++ ;
}

void winter()
{
  if(!modeinit)
  {
    FastLED.setBrightness(200);
    modeinit = true;
  }
  if (loops == 0 ) {
    nextBeat = millis();
  }
  else {
    if (loops == 0 && beatStarted == false) {
      nextBeat = millis();
      beatStarted == true;
      Sparkle();
    }
    else {
      long deltaTimeSparkle = millis() - nextBeat;
      if ( deltaTimeSparkle > 0 ) Sparkle(); // if more time than 
    }
  }

  deltaTimeTwinkle = millis() - nextTwinkle;
  if ( deltaTimeTwinkle > 0 ) {
    Twinkle();
  }
  FastLED.show();
}

/*
void winter()
{
    if (!modeinit)
    {
#ifdef DEBUG
        Serial.println(F("Init: WINTER"));
#endif
        FastLED.setBrightness(200);
        modeinit = true;
    }

    int gr[9] = {50, 100, 150, 200, 255, 200, 150, 100, 50}; //highlights
    int i, j, k;
    fill_solid(leds_left, NUM_LEDS, CHSV(240, 20, 128));
    fill_solid(leds_right, NUM_LEDS, CHSV(240, 20, 128));
    for (i = 0 ; i < NUM_LEDS; i++ ) { //fill with blue
        leds_left[i].b = CHSV(250;
        leds_left[i].g =
        leds_right[i].b = 250;
    }

    for (i=0; i<(NUM_LEDS-9); i+=4) { //add the green
        for (j=0; j<9; j++) {
            leds_left[i+j].g =gr[j];
            leds_right[i+j].g =gr[j];
            //leds_left[i+j].r =gr[j];
            //leds_right[i+j].r =gr[j];
        }
    }
    for (j=0; j<NUM_LEDS; j++) { //rotate display
        leds_left[NUM_LEDS]=leds_left[0];
        leds_right[NUM_LEDS]=leds_right[0];
        for ( i=0; i<NUM_LEDS-1; i++) {
            leds_left[i]=leds_left[i+1];
            leds_right[i]=leds_right[i+1];
        }
    }
	//FastLED.setBrightness(globalBrightness);
	FastLED.show();
}

*/

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

    //thisdelay =   globalSpeed;

    switch (flitmode)
    {
    case 0:
        breathe();
        break;
    case 1:
        rainbow_beat();
        break;
    case 2:
        wow();
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
        winter();
        break;
    }
    FastLED.show();
}

void setFlitMode(uint8_t newmode)
{
	if (newmode < 7)
	{
/*
    Serial.print(F("Switching flitmode: "));
    Serial.print(flitmode);
    Serial.print(F(" to "));
    Serial.println(newmode);
*/
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
    thisdelay = 10;
    modeTimer = millis();
}

#endif //__LEDS_CPP
