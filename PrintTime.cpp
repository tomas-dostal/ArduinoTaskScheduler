/*
 * Copyright 2011 Alan Burlison, alan@bleaklow.com.  All rights reserved.
 * Use is subject to license terms.
 */

#include <Arduino.h>
// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// *** Define the Debugger Class as type Task
// ***
#include "Task.h"
#include "Debugger.h"
#include "Display.h"
#include "PrintTime.h"



PrintTime::PrintTime(int _refresh_rate, Debugger *_ptrDebugger, LCD * _ptr_lcd)  : TimedTask(millis()),
  refresh_rate(_refresh_rate),
  ptr_lcd(_ptr_lcd),
  ptrDebugger(_ptrDebugger)
  {
     if (rtc.lostPower()) 
     {
      Serial.println("RTC lost power, lets set the time!");
      // If the RTC have lost power it will sets the RTC to the date & time this sketch was compiled in the following line
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      // This line sets the RTC with an explicit date & time, for example to set
      // January 21, 2014 at 3am you would call:
      // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }
  }


// ***
// *** PrintTime::run() <--executed by TaskScheduler as a result of canRun() returning true.
// ***

void PrintTime::run(DateTime dt_now)
{
  this->run(millis()); 
}
void PrintTime::run(uint32_t now)
{
    int line = 0;
    DateTime t = rtc.now();
    Serial.println("RTC MODULE");

    Serial.print(t.year(), DEC);
    Serial.print('/');
    Serial.print(t.month(), DEC);
    Serial.print('/');
    Serial.print(t.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[t.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(t.hour(), DEC);
    Serial.print(':');
    Serial.print(t.minute(), DEC);
    Serial.print(':');
    Serial.print(t.second(), DEC);
    Serial.println();

    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(t.unixtime());
    Serial.print("s = ");
    Serial.print(t.unixtime() / 86400L);
    Serial.println("d");
    if(t.day() < 10)
    {
      ptr_lcd->writeLineFrom(line, 0, "0"); 
      ptr_lcd->writeLineFrom(line, 1, String(t.day())); 

    }
    else ptr_lcd->writeLineFrom(line, 0, String(t.day()));

     
    ptr_lcd->writeLineFrom(line, 2, "."); 

    if(t.month() < 10)
    {
      ptr_lcd->writeLineFrom(line, 3, "0"); 
      ptr_lcd->writeLineFrom(line, 4, String(t.month())); 

    }
    else ptr_lcd->writeLineFrom(line, 3, String(t.month()));

    ptr_lcd->writeLineFrom(line, 5, "."); 
    ptr_lcd->writeLineFrom(line, 6, String(t.year()-2000)); 

    if(t.hour() < 10)
    {
      ptr_lcd->writeLineFrom(line, 9, "0"); 
      ptr_lcd->writeLineFrom(line, 10, String(t.hour())); 

    }
    else ptr_lcd->writeLineFrom(line, 9, String(t.hour()));

    ptr_lcd->writeLineFrom(line, 11, ":"); 

    if(t.minute() < 10)
    {
      ptr_lcd->writeLineFrom(line, 12, "0"); 
      ptr_lcd->writeLineFrom(line, 13, String(t.minute())); 

    }
    else ptr_lcd->writeLineFrom(line, 12, String(t.minute()));
    ptr_lcd->writeLineFrom(line, 14, ":"); 

    if(t.second() < 10)
    {
      ptr_lcd->writeLineFrom(line, 15, "0"); 
      ptr_lcd->writeLineFrom(line, 16, String(t.second())); 

    }
    else ptr_lcd->writeLineFrom(line, 15, String(t.second()));


    incRunTime(refresh_rate);
  
  // Run again in the specified number of milliseconds.
}
