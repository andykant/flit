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


void RtcSetup();
void RtcCheck();
String getASCIIDateTime();


#endif //__REALTIMECLOCK_H
