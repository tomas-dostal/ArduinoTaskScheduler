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

#include "AdvancedBlinker.h"




// ***
// *** Blinker Constructor
// ***
/*
AdvancedBlinker::AdvancedBlinker(uint8_t _pin, uint32_t _time_on, uint32_t _time_off, Debugger *_ptrDebugger)  : TimedTask(millis()),
  pin(_pin),
  time_on(_time_on),
  time_off(_time_off),

  on(false),
  ptrDebugger(_ptrDebugger)
  {
    pinMode(pin, OUTPUT);     // Set pin for output.
  }
  */

AdvancedBlinker::AdvancedBlinker(uint8_t _pin, uint32_t _time_on, uint32_t _time_off, Debugger *_ptrDebugger, LCD * _ptr_lcd)  : TimedTask(millis()),
  pin(_pin),
  time_on(_time_on),
  time_off(_time_off),
  ptr_lcd(_ptr_lcd),
  on(false),
  ptrDebugger(_ptrDebugger)
  {
    pinMode(pin, OUTPUT);     // Set pin for output.
  }

// ***
// *** Blinker::run() <--executed by TaskScheduler as a result of canRun() returning true.
// ***
void AdvancedBlinker::run(uint32_t now)
{
  // If the LED is on, turn it off and remember the state.
  if (on) {
    digitalWrite(pin, LOW);
    on = false;
    ptrDebugger->debugWrite("BLINKER: OFF");
    
    //ptr_lcd->writeLine(0, "BLINKER: OFF");

    incRunTime(time_off);

    // If the LED is off, turn it on and remember the state.
  } else {
    digitalWrite(pin, HIGH);
    on = true;
    //Send output to Serial Monitor via debugger
    ptrDebugger->debugWrite("BLINKER: ON");

    //ptr_lcd->writeLine(0, "BLINKER: ON ");
    incRunTime(time_on);
  }
  // Run again in the specified number of milliseconds.
}
