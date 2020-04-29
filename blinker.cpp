/*
 * Copyright 2011 Alan Burlison, alan@bleaklow.com.  All rights reserved.
 * Use is subject to license terms.
 */

#include <Arduino.h>


// *** Define the Debugger Class as type Task
// ***
#include "Task.h"
#include "debugger.h"
#include "blinker.h"





// ***
// *** Blinker Constructor
// ***
Blinker::Blinker(uint8_t _pin, uint32_t _rate, Debugger *_ptrDebugger)
  : TimedTask(millis()),
  pin(_pin),
  rate(_rate),
  on(false),
  ptrDebugger(_ptrDebugger)
  {
    pinMode(pin, OUTPUT);     // Set pin for output.
  }

void Blinker::update(uint32_t now)
{}; 

// ***
// *** Blinker::run() <--executed by TaskScheduler as a result of canRun() returning true.
// ***
void Blinker::run(uint32_t now)
{
  // If the LED is on, turn it off and remember the state.
  if (on) {
    digitalWrite(pin, LOW);
    on = false;
    ptrDebugger->debugWrite("BLINKER: OFF");
    // If the LED is off, turn it on and remember the state.
  } else {
    digitalWrite(pin, HIGH);
    on = true;
    //Send output to Serial Monitor via debugger
    ptrDebugger->debugWrite("BLINKER: ON");
  }
  // Run again in the specified number of milliseconds.
  incRunTime(rate);
}
