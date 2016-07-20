#ifndef __LEDS_H
#define __LEDS_H
/*
	LED animations
	Microcontroller code for Flit
	Nate Marx
	2016
*/

#include <FastLED.h>

#define LED_PIN_L 7
#define LED_PIN_R 8
#define NUM_LEDS 61
#define COLOR_ORDER RGB
#define LED_TYPE WS2812B
#define maxsteps 16

extern uint8_t   globalBrightness,
                 globalSpeed,
                 breatheHue,
                 rainbowWidth,
                 rippleCount,
                 springSize,
                 summerShimmer,
                 fallSaturation,
                 winterSnowfall,
                 bgcol,
                 colour,
                 flitmode;

extern long modeTimer;
extern bool modeinit, rotate;

extern int thisdelay;
extern int center;
extern int step;

extern CRGBPalette16 currentPalette;
extern uint16_t x,y,z;
extern uint16_t noise_speed;
extern uint16_t noise_scale;
extern uint8_t noise[2][31];
extern uint8_t colorLoop;




void breathe();
void rainbow_beat();
void ripple() ;
void spring();
void show_automaton(int automaton[]);
void summer();
void fall();
void winter();
void nextAnimation();
void reset_mode();
void setFlitMode(uint8_t newmode);
void LEDSetup();
void LEDLoop();

#endif //__LEDS_H
