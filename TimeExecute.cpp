/*********************************************************************************
*  Class:    TimeExecute
*  Task Type:  TriggeredTimedTask (runs when a specific time interval is reached or when triggered)
*  Purpose:  Unversal Task class
*       
*  Created by: Tomas Dostal <t@xn--dostl-0qa.eu> <t@dostál.eu> https://xn--dostl-0qa.eu
*  
*********************************************************************************/

#include <Arduino.h>

#include "Task.h"
#include "Debugger.h"
#include "Display.h"

#include "TimeExecute.h"

// this is useful just for debug. Once I'll catch all bugs I'll remove it

void showDate(String txt, const DateTime& dt)
{
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
void showDate(const char* txt, const DateTime& dt)
{
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

// ***
// *** TimeExecute contructor with DateTime
// ***
TimeExecute::TimeExecute(uint8_t _pin, DateTime* _dt_startTime, TimeSpan* _dt_activeTime, Debugger* _ptrDebugger, LCD* _ptr_lcd)
    : TriggeredTimeTask()
    , pin(_pin)
    , ptrDebugger(_ptrDebugger)
    , RtcTask(true)
    ,

    ptr_lcd(_ptr_lcd)
{
    runAt(*_dt_startTime);
    stopAt(*_dt_startTime + *_dt_activeTime);

    pinMode(pin, OUTPUT); // Set pin for output.

    ptrDebugger->debugWrite(getTaskName() + ": Constructor");
    showDate("Start: ", *_dt_startTime);
    showDate("Stop: ", *_dt_startTime + *_dt_activeTime);

    setRunnable(true);
}

// ***
// *** TimeExecute contructor simple (using only integrated clocks), starts imediatelly
// ***
TimeExecute::TimeExecute(uint8_t _pin, uint32_t _active_time, Debugger* _ptrDebugger, LCD* _ptr_lcd)
    : TriggeredTimeTask()
    , pin(_pin)
    , ptrDebugger(_ptrDebugger)
    , RtcTask(false)
    , ptr_lcd(_ptr_lcd)
{

    pinMode(pin, OUTPUT); // Set pin for output.

    runAt(millis()); // now
    stopAt(millis() + _active_time);
}

// ***
// *** TimeExecute contructor simple (using only integrated clocks), starts after _delay
// ***
TimeExecute::TimeExecute(uint8_t _pin, uint32_t _delay, uint32_t _active_time, Debugger* _ptrDebugger, LCD* _ptr_lcd)
    : TriggeredTimeTask()
    , pin(_pin)
    , ptrDebugger(_ptrDebugger)
    , RtcTask(false)
    , ptr_lcd(_ptr_lcd)
{

    pinMode(pin, OUTPUT); // Set pin for output.

    Serial.println("delay:" + String(_delay));
    Serial.println("_active_time:" + String(_active_time));

    runAt(millis() + _delay);
    stopAt(millis() + _delay + _active_time);

    Serial.println("\nTimeExecute constructor (current: " + String(millis()) + " )");
}

void TimeExecute::update(uint32_t now)
{

    if (showProgressBar()) {
        ptr_lcd->drawProgressBar(getTaskID(), now - runTime, 0, stopTime - runTime);
    }
    ptrDebugger->debugWrite(getTaskName() + ": update at " + String(now));
    runUpdateIn(500);
}

void TimeExecute::update(DateTime dt_now)
{

    if (showProgressBar()) {
        ptr_lcd->drawProgressBar(getTaskID(), (dt_now - dt_runTime).totalseconds(), 0, ((dt_runTime - dt_runUpdateTime) - dt_now).totalseconds());
    }
    ptrDebugger->debugWrite(getTaskName() + ": update");

    TimeSpan ts = TimeSpan(0, 0, 0, 1);

    if (dt_now <= ((dt_runTime) + (dt_runUpdateTime)))
        runUpdateIn(ts);
}
void TimeExecute::run(DateTime dt_now)
{

    Serial.println("\n\n\nTimeExecute DateTime RUN (current: " + String(millis()) + " ) ");

    showDate("run", dt_runTime);
    showDate("STOP", dt_stopTime);

    if (on) // If the LED is on, turn it off and break
    {
        digitalWrite(pin, LOW);
        on = false;
        ptrDebugger->debugWrite(getTaskName() + ": OFF");

        if (showProcessName()) {
            ptr_lcd->drawProcessName(getTaskName() + ": OFF");
            ptr_lcd->doneDrawing();
        }
        ptrDebugger->debugWrite(getTaskName() + ": Task finished");
        setRunnable(false);

        if (isPeriodic())
            runAgainAt(dt_stopTime + ts_period);
    }
    else {
        digitalWrite(pin, HIGH);
        on = true;
        //Send output to Serial Monitor via debugger
        ptrDebugger->debugWrite(getTaskName() + ": ON");

        ptr_lcd->clearLine(2);
        ptr_lcd->writeLineFrom(2, 0, getTaskName() + ": ON ");

        ptr_lcd->drawProcessName(getTaskName());

        TimeSpan ts = TimeSpan(0, 0, 0, 1);
        runUpdateIn(ts);
    }
}

void TimeExecute::run(uint32_t now)
{

    Serial.println("\n\n\nTimeExecute run (current: " + String(millis()) + " ) " + "start: " + String(runTime) + " Stop: " + String(stopTime) + " )");
    if (on) // If the LED is off, turn it on and remember the state.
    {

        if (isOutputInverted())
            digitalWrite(pin, HIGH);
        else
            digitalWrite(pin, LOW);

        on = false;
        ptrDebugger->debugWrite(getTaskName() + ": OFF");

        setRunnable(false);
        ptr_lcd->doneDrawing();

        if (isPeriodic())
            runAgainAt(millis() + period); // or stoptime + period
    }

    else {

        if (isOutputInverted())
            digitalWrite(pin, LOW);
        else
            digitalWrite(pin, HIGH);

        on = true;
        //Send output to Serial Monitor via debugger
        ptrDebugger->debugWrite(getTaskName() + ": ON");
        ptr_lcd->drawProcessName(getTaskName());

        runUpdateIn(100);
    }
}
