#ifndef __POWER_H
#define __POWER_H

#define SSR_PWR_PIN 12
#define LED_PWR_PIN 22
//#define LED_PWR_PIN 3


void powerSetup();
void powerOn();
void powerOff();
void togglePower();
void powerSelfTest();

#endif //__POWER_H
