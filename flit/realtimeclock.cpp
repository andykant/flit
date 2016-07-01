#ifndef __REALTIMECLOCK_CPP
#define __REALITIMECLOCK_CPP

// Realtime Clock function implementation
// Nate Marx
// 2016
#include <Arduino.h>
#include <RTClib.h>
#include "power.h"
#include "realtimeclock.h"

RTC_DS1307 rtc;

void RtcSetup()
{
    if (!rtc.begin())
    {
#ifdef DEBUG
        Serial.println(F("Couldn't find RTC"));
#endif
        delay(50);
#ifdef DEBUG
        Serial.println(F("RTC not found..."));
#endif
        rtc_missing = true;
    }

    if (!rtc.isrunning())
    {
#ifdef DEBUG
        Serial.println(F("Waiting for RTC.."));
#endif
    }
    rotate_time = 0;


    Serial.print("Clock time: ");

    Serial.println(getASCIIDateTime());
}

void RtcCheck()
{
    DateTime t = rtc.now();
  
    long time_on = 0; //(time of day to turn on in secs
    long time_off = 0;

    switch (t.month()-1)
    {
    case 0:
        time_off = 29700;
        time_on = 56700;
        break;
    case 1:
        time_off = 27900;
        time_on = 59400;
        break;
    case 2:
        time_off = 32400;
        time_on = 68400;
        break;
    case 3:
        time_off = 28800;
        time_on = 70200;
        break;
    case 4:
        time_off = 27000;
        time_on = 72000;
        break;
    case 5:
        time_off = 22860;
        time_on = 73500;
        break;
    case 6:
        time_off = 27000;
        time_on = 73200;
        break;
    case 7:
        time_off = 28800;
        time_on = 71400;
        break;
    case 8:
        time_off = 30600;
        time_on = 64800;
        break;
    case 9:
        time_off = 34800;
        time_on = 64800;
        break;
    case 10:
        time_off = 27600;
        time_on = 55800;
        break;
    case 11:
        time_off = 29700;
        time_on = 54900;
        break;
    default:
        //donothing
        break;
    };


    long time_now = (t.hour() * 3600) + (t.minute() * 60) + t.second();
    if (rotate_time == 0)
        rotate_time = time_now;

#ifdef DEBUG
//  Serial.println(time_now);
//  Serial.println(time_off);
//  Serial.println(time_on);
#endif

    // if OFF and before turn off time or after turn on time (math doesn't know about midnight)
    if ((digitalRead(SSR_PWR_PIN) == HIGH) && ((time_now < time_off) || (time_now > time_on)))
    {
#ifdef DEBUG
        Serial.println(F("OFF within ON hours: toggling power."));
#endif
        powerOn();
    }
    // if ON and after turn off time AND before turn on time
    if ((digitalRead(SSR_PWR_PIN) == LOW) && ((time_now > time_off) && (time_now < time_on)))
    {
#ifdef DEBUG
        Serial.println(F("ON within OFF hours: toggling power."));
#endif
        powerOff();
    }
    //else it's already doing what it should
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
