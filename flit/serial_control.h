#ifndef __SERIAL_CONTROL_H
#define __SERIAL_CONTROL_H

//for when the test rig isn't a mega

//#include <SoftwareSerial.h>
//extern SoftwareSerial softSerial;
//void mySerialSetup();
//void mySerialEvent();
//
void serial2Setup();
void serialEvent2();
//void serialEvent2();

void processInput(String input);

extern String controllerInput;
extern bool autoOnOff, rotate;
extern uint8_t  flitmode;
/*
extern uint8_t  globalBrightness,
                globalSpeed,
                breatheHue,
                rainbowWidth,
                rippleCount,
                springSize,
                summerShimmer,
                fallSaturation,
                winterSnowfall,
*/
#endif //__SERIAL_CONTROL_H
