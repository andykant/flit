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
//#include <SoftwareSerial.h>


#include "serial_control.h"
#include "leds.h"
#include "power.h"
bool done;
//extern SoftwareSerial mySerial;

void serial2Setup()
{
    Serial2.begin(9600);
    Serial.println("Serial Controller init!");
    controllerInput.reserve(64);
    done = false;
}

void serialEvent2()
{
   if(Serial2.available())
   {
         char inChar = (char)Serial2.read();
         Serial.print("inChar: ");
         Serial.println(inChar);
         controllerInput += inChar;
         if(inChar == '\n')
         {
            Serial.print("processing: ");
            Serial.println(controllerInput);
            processInput(controllerInput);
            controllerInput = "";
         }
   }
}

void processInput(String input)
{
  String controllerInput = input;
  controllerInput.trim();
  if(controllerInput.substring(0,2) == "##")
  {
      //it's a comment, discard it
      return;
  }
  else if (controllerInput.substring(0,4) == "POWR")
  {
     Serial.println("POWR");
     togglePower();
  }
  else if (controllerInput.substring(0,4) == "ROTA")
  {
     Serial.println("ROTA");
     rotate = (controllerInput.substring(4,4) == "true");
  }
  else if (controllerInput.substring(0,4) == "AUTO")
  {
     Serial.println("AUTO");
     autoOnOff = (controllerInput.substring(4,4) == "true");
  }
  else if (controllerInput.substring(0,4) == "ANIM")
  {
     if (controllerInput =="ANIMBreathe")
     {
         Serial.println(F("ANIMBreathe"));
         setFlitMode(0);
     }
     else if (controllerInput == "ANIMWow")
     {
         Serial.println(F("ANIMWow"));
         setFlitMode(2);
     }
     else if (controllerInput == "ANIMRainbow Beat")
     {
         Serial.println(F("ANIMRainbow"));
         setFlitMode(1);
     }
     else if (controllerInput == "ANIMSpring")
     {
         Serial.println(F("ANIMSpring"));
         setFlitMode(3);
     }
     else if (controllerInput == "ANIMSummer")
     {
         setFlitMode(4);
         Serial.println(F("ANIMSummer"));
     }
     else if (controllerInput == "ANIMFall")
     {
         Serial.println(F("ANIMFall"));
         setFlitMode(5);
     }
     else if (controllerInput == "ANIMWinter")
     {
         Serial.println(F("ANIMWinter"));
         setFlitMode(6);
     }
  }
  /*
  else if (controllerInput.substring(0,4) == "BRIG")
  {
     Serial.print("BRIG: ");
     globalBrightness  = controllerInput.substring(4).toInt();
     Serial.println(globalBrightness);
  }
  else if (controllerInput.substring(0,4) == "SPEE")
  {
     Serial.print("SPEE: ");
     globalSpeed  = controllerInput.substring(4).toInt();
     Serial.println(globalSpeed);
     
  }
  else if (controllerInput.substring(0,4) == "BREA")
  {
     Serial.print("BREA: ");
     breatheHue  = controllerInput.substring(4).toInt();
     Serial.println(breatheHue);
  }
  else if (controllerInput.substring(0,4) == "RAIN")
  {
     Serial.print("RAIN: ");
     rainbowWidth  = controllerInput.substring(4).toInt();
     Serial.println(rainbowWidth);
  }
  else if (controllerInput.substring(0,4) == "RIPP")
  {
     Serial.print("RIPP: ");
     rippleCount  = controllerInput.substring(4).toInt();
     Serial.println(rippleCount);
     thisdelay = 40 * (rippleCount/255);
  }
  else if (controllerInput.substring(0,4) == "SPRI")
  {
     Serial.print("SPRI: ");
     springSize  = controllerInput.substring(4).toInt();
     Serial.println(springSize);
  }
  else if (controllerInput.substring(0,4) == "SUMM")
  {
     Serial.print("SUMM: ");
     summerShimmer  = controllerInput.substring(4).toInt();
     Serial.println(summerShimmer);
  }
  else if (controllerInput.substring(0,4) == "Fall")
  {
     Serial.print("FALL: ");
     fallSaturation  = controllerInput.substring(4).toInt();
     Serial.println(fallSaturation);
  }
  else if (controllerInput.substring(0,4) == "WINT")
  {
     Serial.print("WINT: ");
     winterSnowfall  = controllerInput.substring(4).toInt();
     Serial.println(winterSnowfall);
  }
  */
  else
  {
     Serial.println("COMMAND NOT RECOGNIZED!: ");
     Serial.print("###");
     Serial.print(controllerInput);
     Serial.println("###");
     
  }
}

#endif //__SERIAL_CONTROL_CPP
