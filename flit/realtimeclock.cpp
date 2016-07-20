#ifndef __REALTIMECLOCK_CPP
#define __REALITIMECLOCK_CPP

// Realtime Clock function implementation
// Nate Marx
// 2016
#include <Arduino.h>
#include <RTClib.h>
#include <Sunrise.h>
#include "realtimeclock.h"
#include "power.h"

RTC_DS1307 rtc;
float LAT = 43.079128;
float LONG = -88.158561;
int TZ = -5;

void RtcSetClock()
{
  rtc.adjust(DateTime(2016, 7, 14, 12, 0, 0));
}


void RtcSetup()
{
    if (!rtc.begin())
    {
        Serial.println(F("RTC not found..."));
        rtc_missing = true;
    }

    rotate_time = 0;

    DateTime t = rtc.now();
    if(t.year() == 2000)
      RtcSetClock();

    Serial.print("Clock time: ");
    Serial.println(getASCIIDateTime());
}

void RtcCheck()
{
    DateTime t = rtc.now();

    if(!sunrise)
      sunrise = initSunrise(t);
    if(!sunset)
      sunset = initSunset(t);
/*
    Serial.print("Sunrise: ");
    Serial.println(sunrise);
    Serial.print("Sunset: ");
    Serial.println(sunset);
    Serial.print("Now: ");
    Serial.println(((t.hour() * 60) + t.minute()));
    Serial.print("Hour: ");
    Serial.println(t.hour());
    Serial.print("Min: ");
    Serial.println(t.minute());
    delay(2000);
  */    
    if((((t.hour() * 60) + t.minute()) < sunrise + 60) || ((t.hour() * 60)+t.minute()) > sunset - 60)
    {
        if(digitalRead(SSR_PWR_PIN) == HIGH)
        {
          Serial.println(F("Before dawn or after dusk, turning on LEDs."));
          powerOn();
        }
    }
    else
    {
        if(digitalRead(SSR_PWR_PIN) == LOW)
        {
          Serial.print(F("After dawn and before dusk, turning off LEDs."));
          powerOff();
        }
    }

    
}

int initSunrise(DateTime t)
{
  Sunrise mySunrise(LAT, LONG, TZ);
  return mySunrise.Rise(t.month(), t.day());
}
int initSunset(DateTime t)
{
  Sunrise mySunrise(LAT, LONG, TZ);
  return mySunrise.Set(t.month(), t.day());
}

String getASCIIDateTime()
{
    String s;
    DateTime t = rtc.now();

    s += String(t.year(), DEC);
    s+="_";
    s += String(t.month(), DEC);
    s+="_";
    s += String(t.day(), DEC);
    s+="_";
    s += String(t.hour(), DEC);
    s+="_";
    s += String(t.minute(), DEC);
    s+="_";
    s += String(t.second(), DEC);

    return s;
}


#endif //__REALITIMECLOCK_CPP
