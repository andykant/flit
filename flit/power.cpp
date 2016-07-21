#ifndef __POWER_CPP
#define __POWER_CPP

/*
	Power relay support
	Microcontroller code for Flit
	Nate Marx
	2016
*/

#include <Arduino.h>
#include "power.h"

void powerSetup()
{
    pinMode(SSR_PWR_PIN, OUTPUT);
    pinMode(LED_PWR_PIN, OUTPUT);
    digitalWrite(SSR_PWR_PIN, HIGH);
}

void powerOn()
{
    digitalWrite(SSR_PWR_PIN, LOW);
    digitalWrite(LED_PWR_PIN, HIGH);
}

void powerOff()
{
    digitalWrite(SSR_PWR_PIN, HIGH);
    digitalWrite(LED_PWR_PIN, LOW);
}
void togglePower()
{
    if (digitalRead(SSR_PWR_PIN) == LOW)
    {
#ifdef DEBUG
        Serial.println(F("Power button :Powering down LEDs."));
#endif
        powerOff();
    }
    else
    {
#ifdef DEBUG
        Serial.println(F("Power Button: Powering up LEDs."));
#endif
        powerOn();
    }
}

void powerSelfTest()
{
  Serial.println(F("Power section self-test:"));
  Serial.println(F("powerSetup()"));
  powerSetup();
  delay(1000);
  Serial.println(F("powerOn()"));
  powerOn();
  delay(1000);
  Serial.println(F("powerOff()"));
  powerOff();
  delay(1000);
  Serial.println(F("togglePower()"));
  togglePower();
  delay(1000);
  Serial.println(F("togglePower()"));
  togglePower();
  delay(1000);
  Serial.println(F("Test complete."));
  delay(5000);
}

#endif //__POWER_CPP
