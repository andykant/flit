#ifndef __SERIAL_CONTROL_CPP
#define __SERIAL_CONTROL_CPP
/*
	Serial controller support
	Microcontroller code for Flit
	Nate Marx
	2016
*/

#include <Arduino.h>
#include <FastLED.h>
#include "serial_control.h"
#include "leds.h"
#include "power.h"


void serialSetup()
{
    Serial2.begin(9600);
    Serial.println("Serial2 up!");
    controllerInput.reserve(200);
}

//we must manually call thsi for SoftSerial
//void mySerialEvent()

void serialEvent2()
{
    while (Serial2.available()) {

        char inChar = (char)Serial2.read();

        controllerInput += inChar;


        // the big if else. no other way to do this, really.

        if (Contains(controllerInput ,F("POWR")))
        {
            Serial.println("POWR");
            togglePower();
            controllerInput ="";
        }
        else if (Contains(controllerInput ,F("ROTA")))
        {
            Serial.println("ROTA");
            rotate = (controllerInput.substring(4,4) == "true");
            controllerInput ="";
        }
        else if (Contains(controllerInput ,F("AUTO")))
        {
            autoOnOff = (controllerInput.substring(4,4) == "true");
            controllerInput ="";
        }
        else if (Contains(controllerInput ,F("ANIM")))
        {
            Serial.println("ANIM");
            if (Contains(controllerInput, F("Breathe")))
            {
                flitmode = 0;
                controllerInput ="";
            }
            else if (Contains(controllerInput, F("Ripple")))
            {
                flitmode = 1;
                controllerInput ="";
            }
            else if (Contains(controllerInput, F("Rainbow")))
            {
                flitmode = 2;
                controllerInput ="";
            }
            else if (Contains(controllerInput, F("Spring")))
            {
                flitmode = 3;
                controllerInput ="";
            }
            else if (Contains(controllerInput, F("Summer")))
            {
                flitmode = 4;
                controllerInput ="";
            }
            else if (Contains(controllerInput, F("Fall")))
            {
                flitmode = 5;
                controllerInput ="";
            }
            else if (Contains(controllerInput, F("Winter")))
            {
                flitmode = 6;
                controllerInput ="";
            }
        }
        else if (Contains(controllerInput ,F("BRIG")))
        {
            globalBrightness  = int(255 * atof(controllerInput.substring(5).c_str()));
            controllerInput	  = "";
        }
        else if (Contains(controllerInput ,F("SPEE")))
        {
            globalSpeed  = int(255 * atof(controllerInput.substring(5).c_str()));
            controllerInput	  = "";
        }
        else if (Contains(controllerInput ,F("BREA")))
        {
            breatheHue  = int(255 * atof(controllerInput.substring(5).c_str()));
            controllerInput	  = "";
        }

        else if (Contains(controllerInput ,F("RAIN")))
        {
            rainbowWidth  = int(255 * atof(controllerInput.substring(5).c_str()));
            controllerInput	  = "";
        }
        else if (Contains(controllerInput ,F("RIPP")))
        {
            rippleCount  = int(255 * atof(controllerInput.substring(5).c_str()));
            controllerInput	  = "";
        }
        else if (Contains(controllerInput ,F("SPRI")))
        {
            springSize  = int(255 * atof(controllerInput.substring(5).c_str()));
            controllerInput	  = "";
        }
    }
}

bool Contains( String s, String search) {
    int max = s.length() - search.length();
    for (int i=0; i<= max; i++)
    {
        if (s.substring(i) == search) return true;
    }
    return false;}



#endif //__SERIAL_CONTROL_CPP
