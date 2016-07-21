#ifndef __FLIT_INO
#define __FLIT_INO
/*
	Microcontroller code for Flit
	Nate Marx
	2016
*/
#include <Arduino.h>
#include <FastLED.h>
#include <RTClib.h>
#include <Sunrise.h>
#include <Wire.h>

#include "globals.h"
#include "power.h"
#include "serial_control.h"
#include "buttons.h"
#include "realtimeclock.h"
#include "leds.h"

//runs once at start
void setup() {
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println(F("Hello. I am Flit."));
#endif
  powerSetup();
  buttonSetup();
  rtcSetup();
  serial2Setup();
  LEDSetup();

  delay(3000);
  //rtcSelfTest();
  //while(1){};
}

//loops  forever
void loop() {
  serialEvent2();
    LEDLoop();
  if(autoOnOff && !rtc_missing)
  {
     rtcCheck();
  }
  buttonCheck();
}

#endif //__FLIT_INO
