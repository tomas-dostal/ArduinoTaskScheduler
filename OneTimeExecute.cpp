/*
 * Copyright 2011 Alan Burlison, alan@bleaklow.com.  All rights reserved.
 * Use is subject to license terms.
 */

#include <Arduino.h>


// *** Define the Debugger Class as type Task
// ***
#include "Task.h"
#include "debugger.h"
#include "display.h"

#include "OneTimeExecute.h"





// ***
// *** OneTimeExecute contructor
// ***
OneTimeExecute::OneTimeExecute(String _taskname, uint8_t _pin, uint32_t _active_time, Debugger *_ptrDebugger, LCD * _ptr_lcd)
  : TriggeredTimeTask(),
  taskname(_taskname), 
  pin(_pin),
  active_time(_active_time),
  ptrDebugger(_ptrDebugger),
  ptr_lcd(_ptr_lcd)
  {
    lcd_printout = true; 
    pinMode(pin, OUTPUT);     // Set pin for output.

  }
// ***
// *** OneTimeExecute alternative  contructor
// ***
  OneTimeExecute::OneTimeExecute(String _taskname, uint8_t _pin, Debugger *_ptrDebugger) : TriggeredTimeTask(),
  taskname(_taskname),
  pin(_pin),
  ptrDebugger(_ptrDebugger)
  {
    lcd_printout = false; 
    active_time = 0; 

    pinMode(pin, OUTPUT);     // Set pin for output.
  }

  
OneTimeExecute::OneTimeExecute(String _taskname,  uint8_t _pin, uint32_t _active_time, Debugger *_ptrDebugger) : TriggeredTimeTask(),
  taskname(_taskname), 
  pin(_pin),
  active_time(_active_time),
  ptrDebugger(_ptrDebugger)
  {
    lcd_printout = false; 
    pinMode(pin, OUTPUT);     // Set pin for output.
  }
  



// ***
// *** OneTimeExecute::run() <--executed by TaskScheduler as a result of canRun() returning true.
// ***

void OneTimeExecute::update(uint32_t now)
{
  if(lcd_printout) ptr_lcd->drawProgressBar(print_to_line, now - started, 0, active_time);
ptrDebugger->debugWrite(taskname + ": update");


  if(now - started <= active_time)
    runUpdateIn(100); 


}

void OneTimeExecute::run(uint32_t now)
{
    if (on) // If the LED is off, turn it on and remember the state.
    {
      digitalWrite(pin, LOW);
      on = false;
      ptrDebugger->debugWrite(taskname + ": OFF");
      ptr_lcd->clearLine(print_to_line); 
      if(lcd_printout) ptr_lcd->writeLineFrom(2, 0, taskname + ": OFF");
      ptrDebugger->debugWrite(taskname + ": RESET RUNNABLE");
      resetRunnable(); 
    } 
    else
    {
      digitalWrite(pin, HIGH);
      on = true;
      //Send output to Serial Monitor via debugger
      ptrDebugger->debugWrite(taskname + ": ON");

      ptr_lcd->clearLine(print_to_line); 
      if(lcd_printout) ptr_lcd->writeLineFrom(2, 0, taskname + ": ON ");

      started = millis(); 
      runUpdateIn(100); 

      runAgainIn(active_time); 
    }

}
