#ifndef __SERIAL_CONTROL_H
#define __SERIAL_CONTROL_H

//for when the test rig isn't a mega

//#include <SoftwareSerial.h>
//extern SoftwareSerial softSerial;
//void mySerialSetup();
//void mySerialEvent();
//
void serialSetup();
void serialEvent();
//void serialEvent2();


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

bool Contains( String s, String search);

#endif //__SERIAL_CONTROL_H
