/*
 * Copyright 2011 Alan Burlison, alan@bleaklow.com.  All rights reserved.
 * Use is subject to license terms.
 */

#include <Arduino.h>


// *** Define the Debugger Class as type Task
// ***
#include "Task.h"
#include "Debugger.h"
#include "Display.h"

#include "Fader.h"



/*****************************************************************************************
*  Class:    Fader
* Task Type:  TimedTask (runs when a specific time interval is reached)
* Purpose:  Fades the LED attached to (LED_FADER: Pin 3 ~PWM pin) at a specific rate
*       (RATE_FADER_FADE) whilst using the value INCREMENT_FADER_STEP to increase or decrease
*       the brightness of the LED with analogWrite(). It utilizes the pointer ptrDebugger
*       to print simple debug messages to the Serial Monitor when upper and lower ranges
*       are reached.
*
*       (e.g. "FADER BRIGHTNESS: 0" and "FADER BRIGHTNESS: 255")
*
******************************************************************************************/

// ***
// *** Define the Fader Class as type TimedTask
// ***


// ***
// *** Fader Constructor
// ***
Fader::Fader(uint8_t _pin, uint8_t _increment, uint32_t _rate, Debugger *_ptrDebugger)
  : TimedTask(millis()),
  pin(_pin),
  increment(_increment),
  rate(_rate),
  brightness(0),
  ptrDebugger(_ptrDebugger)
  {
    
    display_connedted = false; 
    pinMode(pin, OUTPUT);     // Set pin for output.
  }

Fader::Fader(uint8_t _pin, uint8_t _increment, uint32_t _rate, Debugger *_ptrDebugger, LCD * _ptr_lcd)
  : TimedTask(millis()),
  pin(_pin),
  increment(_increment),
  rate(_rate),
  brightness(0),
  ptr_lcd(_ptr_lcd),
  ptrDebugger(_ptrDebugger)
  {
    display_connedted = true; 
    pinMode(pin, OUTPUT);     // Set pin for output.
  }

// ***
// *** Fader::run() <--executed by TaskScheduler as a result of canRun() returning true.
// ***
void Fader::run(uint32_t now)
{
  // set the brightness of pin 9:
  analogWrite(pin, brightness);



  // change the brightness for next time through the loop:
  brightness = brightness + increment;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 255) {
    increment = -increment;
  }
  
  // ***
  // *** Is the brightness 0 or 255?
  // *** Write to the Debugger task
  // ***
  
  if(brightness == 0)
  {
    ptrDebugger->debugWrite("FADER LEVEL: 0");
    
  }
  else if(brightness == 255)
  {
    ptrDebugger->debugWrite("FADER LEVEL: 255");
  }
  
  // Run again in the specified number of milliseconds.
  incRunTime(rate);
}
