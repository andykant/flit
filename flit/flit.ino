#ifndef __FLIT_INO
#define __FLIT_INO
/*
	Microcontroller code for Flit
	Nate Marx
	2016
*/

//must include headers for all packages in ../libraries
//or your cpp files can't find them later.
#include <FastLED.h>
#include <RTClib.h>
#include <Wire.h>

#include "globals.h"
#include "power.h"
#include "buttons.h"
#include "realtimeclock.h"
#include "serial_control.h"
#include "leds.h"

//runs once at start
void setup() {
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println(F("Hello. I am Flit."));
#endif
  powerSetup();
  ButtonSetup();
  RtcSetup();
  serialSetup();
  LEDSetup();

  delay(3000);
}

//loops  forever
void loop() {
  LEDLoop();
  if(autoOnOff && !rtc_missing)
  {
     RtcCheck();
  }
  ButtonCheck();
}

#endif //__FLIT_INO
