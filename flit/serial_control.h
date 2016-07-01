#ifndef __SERIAL_CONTROL_H
#define __SERIAL_CONTROL_H

//for when the test rig isn't a mega
//#include <SoftwareSerial.h>
//SoftwareSerial Serial2(4,5);

//we must manually call thsi for SoftSerial
//void mySerialEvent() 


extern String controllerInput;
extern bool autoOnOff, rotate;
extern uint8_t  globalBrightness,
                globalSpeed,
                breatheHue,
                rainbowWidth,
                rippleCount,
                springSize,
                summerShimmer,
                fallSaturation,
                winterSnowfall,
                flitmode;

void serialSetup();
void serialEvent2();
bool Contains( String s, String search);

#endif //__SERIAL_CONTROL_H
