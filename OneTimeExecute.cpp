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

#include "OneTimeExecute.h"



void showDate(const char* txt, const DateTime& dt) {
    Serial.begin(115200); 
    Serial.print(txt);
    Serial.print(' ');
    Serial.print(dt.year(), DEC);
    Serial.print('/');
    Serial.print(dt.month(), DEC);
    Serial.print('/');
    Serial.print(dt.day(), DEC);
    Serial.print(' ');
    Serial.print(dt.hour(), DEC);
    Serial.print(':');
    Serial.print(dt.minute(), DEC);
    Serial.print(':');
    Serial.print(dt.second(), DEC);

    Serial.print(" = ");
    Serial.print(dt.unixtime());
    Serial.print("s / ");
    Serial.print(dt.unixtime() / 86400L);
    Serial.print("d since 1970");

    Serial.println();
}


OneTimeExecute::OneTimeExecute(String _taskname, uint8_t _pin, DateTime *_dt_startTime, TimeSpan* _dt_activeTime, Debugger *_ptrDebugger, LCD * _ptr_lcd)
  : TriggeredTimeTask(),
  taskname(_taskname), 
  pin(_pin),
  ptrDebugger(_ptrDebugger),
  ptr_lcd(_ptr_lcd)
  {
  dt_runTime = _dt_startTime; 
  dt_runUpdateTime = _dt_activeTime; 
  RtcTask = true; 

    delay(100); 
    lcd_printout = true; 
    pinMode(pin, OUTPUT);     // Set pin for output.
    active_time = 0; 
    ptrDebugger->debugWrite(taskname + ": INIT");
        ptrDebugger->debugWrite(taskname + ": INIT");

    ptrDebugger->debugWrite(taskname + ": INIT");

    ptrDebugger->debugWrite(taskname + ": INIT");

    ptrDebugger->debugWrite(taskname + ": INIT");

    showDate("OneTimeExecute start ", *dt_runTime);
    showDate("OneTimeExecute end ", (*dt_runTime) + (*dt_runUpdateTime) );


   }

// ***
// *** OneTimeExecute contructor
// ***
OneTimeExecute::OneTimeExecute(String _taskname, uint8_t _pin, uint32_t _active_time, Debugger *_ptrDebugger, LCD * _ptr_lcd)
  : TriggeredTimeTask(),
  taskname(_taskname), 
  pin(_pin),
  active_time(_active_time),
  ptrDebugger(_ptrDebugger),
  RtcTask(false),
  ptr_lcd(_ptr_lcd)
  {
    lcd_printout = true; 
    pinMode(pin, OUTPUT);     // Set pin for output.

  }

// ***
// *** OneTimeExecute alternative  contructor
/***
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
  RtcTask(false),
  ptrDebugger(_ptrDebugger)
  {
    lcd_printout = false; 
    pinMode(pin, OUTPUT);     // Set pin for output.
  }
  

*/

// ***
// *** OneTimeExecute::run() <--executed by TaskScheduler as a result of canRun() returning true.
// ***

void OneTimeExecute::update(uint32_t now)
{
  if(lcd_printout) ptr_lcd->drawProgressBar(2, now - started, 0, active_time);
ptrDebugger->debugWrite(taskname + ": update");


  if(now - started <= active_time)
    runUpdateIn(100); 


}
void OneTimeExecute::update(DateTime  dt_now)
{
    update(millis()); 
}
void OneTimeExecute::run(DateTime dt_now)
{
    if (on) // If the LED is off, turn it on and remember the state.
    {
      digitalWrite(pin, LOW);
      on = false;
      ptrDebugger->debugWrite(taskname + ": OFF");
      ptr_lcd->clearLine(2); 
      if(lcd_printout) ptr_lcd->writeLineFrom(2, 0, taskname + ": OFF");
      ptrDebugger->debugWrite(taskname + ": RESET RUNNABLE");
      setRunnable(false); 
    } 
    else
    {
      digitalWrite(pin, HIGH);
      on = true;
      //Send output to Serial Monitor via debugger
      ptrDebugger->debugWrite(taskname + ": ON");

      ptr_lcd->clearLine(2); 
      if(lcd_printout) ptr_lcd->writeLineFrom(2, 0, taskname + ": ON ");

      started = millis(); 
      runUpdateIn(50); 

      runAgainIn(active_time); // turn off 
    }
}

void OneTimeExecute::run(uint32_t now)
{
    if (on) // If the LED is off, turn it on and remember the state.
    {
      digitalWrite(pin, LOW);
      on = false;
      ptrDebugger->debugWrite(taskname + ": OFF");
      ptr_lcd->clearLine(2); 
      if(lcd_printout) ptr_lcd->writeLineFrom(2, 0, taskname + ": OFF");
      ptrDebugger->debugWrite(taskname + ": OBYC RESET RUNNABLE");
     
      setRunnable(false);

 
    } 
    else
    {
      digitalWrite(pin, HIGH);
      on = true;
      //Send output to Serial Monitor via debugger
      ptrDebugger->debugWrite(taskname + ": ON");

      ptr_lcd->clearLine(2); 
      if(lcd_printout) ptr_lcd->writeLineFrom(2, 0, taskname + ": ON ");

      started = millis(); 
      runUpdateIn(50); 

      runAgainIn(active_time); // turn off 
    }
}
