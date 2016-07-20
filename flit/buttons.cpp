#ifndef __BUTTONS_CPP
#define __BUTTONS_CPP
/*
	Button board support
	Microcontroller code for Flit
	Nate Marx
	2016
*/
#include <Arduino.h>
#include "leds.h"
#include "power.h"
#include "buttons.h"

void trig_pwr_btn()
{
    togglePower();
    autoOnOff = false;
    rotate = false;
    digitalWrite(LED_LCK_PIN, HIGH);
}

void trig_nxt_btn()
{
    nextAnimation();
}

void trig_lck_btn()
{
    if (autoOnOff)
    {
#ifdef DEBUG
        Serial.println(F("Disable: autoonff animation cycles."));
#endif
        autoOnOff = false;
        rotate = false;
        digitalWrite(LED_LCK_PIN, HIGH);
    }
    else
    {
#ifdef DEBUG
        Serial.println(F("Enable: autoOnOff and animation cycles."));
#endif
        autoOnOff = true;
        rotate = true;
        digitalWrite(LED_LCK_PIN, LOW);
    }
}

void buttonSetup()
{
    pinMode(BTN_PWR_PIN, INPUT);
    pinMode(BTN_NXT_PIN, INPUT);
    pinMode(BTN_LCK_PIN, INPUT);
    pinMode(LED_PWR_PIN, OUTPUT);
    digitalWrite(LED_PWR_PIN, LOW);
    pinMode(LED_LCK_PIN, OUTPUT);
    digitalWrite(LED_LCK_PIN, LOW);
    cooldown_state = false;
    btn_pwr_press = false;
    btn_nxt_press = false;
    btn_lck_press = false;
    last_btn_mils = millis();
    autoOnOff = true;
    rotate=true;
}

//basic button press with debounce and 1 sec cooldown
void buttonCheck()
{
    bool reading = false;
    if (!cooldown_state)
    {
        reading = (digitalRead(BTN_PWR_PIN) == LOW);
//        reading = (digitalRead(BTN_PWR_PIN) == HIGH);
        if (reading && btn_pwr_press && (millis() > (DEB_MILS + last_btn_mils)))
        {
            Serial.println(F("Power button pressed."));
            last_btn_mils = 0;
            cooldown_state = true;
            cooldown_mils = millis();
            trig_pwr_btn();
            btn_pwr_press = false;
        }
        else
        {
            btn_pwr_press = reading;
            if (!last_btn_mils)
            {
                last_btn_mils = millis();
            }
        }

        reading = (digitalRead(BTN_NXT_PIN) == LOW);
//        reading = (digitalRead(BTN_NXT_PIN) == HIGH);
        if (reading && btn_nxt_press && (millis() > (DEB_MILS + last_btn_mils)))
        {
            Serial.println(F("Next button pressed."));
            last_btn_mils = 0;
            cooldown_state = true;
            cooldown_mils = millis();
            trig_nxt_btn();
            btn_nxt_press = false;
        }
        else
        {
            btn_nxt_press = reading;
            if (!last_btn_mils)
            {
                last_btn_mils = millis();
            }
        }

//        reading = (digitalRead(BTN_LCK_PIN) == HIGH);
        reading = (digitalRead(BTN_LCK_PIN) == LOW);
        if (reading && btn_lck_press && (millis() > (DEB_MILS + last_btn_mils)))
        {
            Serial.println(F("Hold button pressed."));
            last_btn_mils = 0;
            cooldown_state = true;
            cooldown_mils = millis();
            trig_lck_btn();
            btn_lck_press = false;
        }
        else
        {
            btn_lck_press = reading;
            if (!last_btn_mils)
            {
                last_btn_mils = millis();
            }
        }
    }
    else
    {
        if (millis() > (cooldown_mils + COOLDOWN_MILS))
        {
            cooldown_state = false;
        }
    }
}

void buttonSelfTest()
{
  Serial.println("Button module self-test:");
  Serial.println("buttonSetup()");
   buttonSetup();
  delay(1000);
  Serial.println("buttonCheck()");
   buttonCheck();
  delay(1000);
  Serial.println("trig_pwr_btn()");
  trig_pwr_btn();
  delay(1000);
  Serial.println("trig_nxt_btn()");
  trig_nxt_btn();
  delay(1000);
  Serial.println("trig_lck_btn()");
  trig_lck_btn();
  delay(1000);
  Serial.println("Button Self-Test Complete.");
  delay(5000);
}


#endif //__BUTTONS_CPP
