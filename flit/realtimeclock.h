#ifndef __REALTIMECLOCK_H
#define __REALTIMECLOCK_H
/*
	Realtime Clock support
	Microcontroller code for Flit
	Nate Marx
	2016
*/ 

extern bool rtc_missing;
extern long rotate_time;
extern int sunrise;
extern int sunset; 

void rtcSetClock();
void rtcSetup();
void rtcCheck();
int initSunrise(DateTime t);
int initSunset(DateTime t);
String getASCIIDateTime();
void rtcSelfTest();


#endif //__REALTIMECLOCK_H
