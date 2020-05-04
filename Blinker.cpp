/*
 * Copyright 2011 Alan Burlison, alan@bleaklow.com.  All rights reserved.
 * Use is subject to license terms.
 */

#include <Arduino.h>


// *** Define the Debugger Class as type Task
// ***
#include "Task.h"
#include "Debugger.h"
#include "Blinker.h"





// ***
// *** Blinker Constructor
// ***
Blinker::Blinker(uint8_t _pin, uint32_t _rate, Debugger *_ptrDebugger)
  : TimedTask(millis()),
  pin(_pin),
  rate(_rate),
  on(false),
  debug_enabled(true),
  ptrDebugger(_ptrDebugger)
  {
    pinMode(pin, OUTPUT);     // Set pin for output.
  }

  
Blinker::Blinker(uint8_t _pin, uint32_t _rate)
  : TimedTask(millis()),
  pin(_pin),
  rate(_rate),
  on(false),
  debug_enabled(false)
  {
    pinMode(pin, OUTPUT);     // Set pin for output.
  }


// ***
// *** Blinker::run() <--executed by TaskScheduler as a result of canRun() returning true.
// ***
void Blinker::run(uint32_t now)
{
  Serial.println("Blinker"); 
  // If the LED is on, turn it off and remember the state.
  if (on) {
    digitalWrite(pin, LOW);
    on = false;
    if(debug_enabled) ptrDebugger->debugWrite("BLINKER: OFF");
    // If the LED is off, turn it on and remember the state.
  } else {
    digitalWrite(pin, HIGH);
    on = true;
    //Send output to Serial Monitor via debugger
    if(debug_enabled) ptrDebugger->debugWrite("BLINKER: ON");
  }
  // Run again in the specified number of milliseconds.
  incRunTime(rate);
}

void Blinker::run(DateTime  dt_now) 
{
  run(millis());   
}
