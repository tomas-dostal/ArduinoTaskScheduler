/**************************************************************************************
  Description:  ArduinoTaskSheduller Task class
  Type:         N/A
  Purpose:      Defining types of Tasks

  Original copyright: Copyright 2011 Alan Burlison, alan@bleaklow.com.
  All rights reserved. Use is subject to license terms.

  Modified by: Tomas Dostal <t@xn--dostl-0qa.eu> <t@dostál.eu> https://xn--dostl-0qa.eu

**************************************************************************************/
#include <Arduino.h>
#include "Task.h"
#include "RTClib.h"

bool TriggeredTask::canRun(uint32_t now)
{
    return runFlag;
}

bool TimedTask::canRun(uint32_t now)
{
    return now >= runTime;
}

bool TimedTask::canRun(DateTime dt_now)
{
    return dt_now.unixtime() >= dt_runTime->unixtime();
}

bool close_to(uint32_t t)
{
    return (millis() - t) <= 1000;
}

bool close_to(DateTime dt_now, DateTime t)
{
    TimeSpan delta = TimeSpan(0, 0, 0, 1);
    return (dt_now - t).totalseconds() >= delta.totalseconds();
}

// ####################### TriggeredTimeTask  ###################

bool TriggeredTimeTask::canRun(uint32_t now)
{

    /*
    // Some debugging stuff
    Serial.println("TriggeredTimeTask Task runs in " + String((runTime - now) / 1000) + "secs");
    Serial.println("stops in " + String((stopTime - now) / 1000) + "secs");

    Serial.println("isRunnable " + String(isRunnable()));

    Serial.println("isRunning " + String(isRunning()));
    Serial.println("close_to " + String(close_to(runTime)));
    */


    return (isRunnable() && !isRunning() && close_to(runTime)) || // not running yet
        (!isRunnable() && isRunning() && close_to(stopTime)); // already running, need to stop
}


bool TriggeredTimeTask::canRun(DateTime dt_now)
{

    /*
    // Some debugging stuff
    Serial.println("TriggeredTimeTask Task runs in "+ String( ( dt_runTime -
    dt_now).totalseconds()) +"secs");
    Serial.println("isRunnable " + String( isRunnable()));
    Serial.println("isRunning " + String( isRunning()));
    Serial.println("close_to " + String( close_to(dt_now, dt_runTime)));
     */

    return (isRunnable() && (!isRunning() && close_to(dt_now, dt_runTime))) || // not running yet
        (!isRunnable()
               && (isRunning() && close_to(dt_now, dt_stopTime))); // already running, need to stop
}

bool TriggeredTimeTask::canUpdate(uint32_t now)
{
    return isRunning() && isContinueFlag() && (now >= runNextUpdateTime);
}

bool TriggeredTimeTask::canUpdate(DateTime dt_now)
{

    return isRunning() && isContinueFlag()
        && close_to(dt_now, dt_runTime + dt_runUpdateTime); // dt_runUpdateTime is TimeSpan = need
    // to add to the start time
}

void TriggeredTimeTask::setRunnable(bool value)
{
    Runnable = value;
    Running = false;
    continueFlag = false;
}
void TriggeredTimeTask::setRunnable()
{
    Runnable = true;
    Running = false;
    continueFlag = false;
}
void TriggeredTimeTask::runAt(uint32_t t)
{
    Runnable = true;
    Running = false;
    continueFlag = false;
    runTime = t;
}

void TriggeredTimeTask::stopAt(uint32_t t)
{
    Runnable = true;
    Running = false;
    continueFlag = false;
    stopTime = t;
}
void TriggeredTimeTask::runUpdateIn(uint32_t t)
{
    Runnable = false;
    Running = true;
    continueFlag = true;
    runNextUpdateTime = millis() + t;
    // Serial.println("Next update " + String(runNextUpdateTime));
}

void TriggeredTimeTask::runAgainAt(uint32_t t)
{

    uint32_t delta = stopTime - runTime; // for how long was the last one executed?
    runAt(t);
    stopAt(t + delta);
}


void TriggeredTimeTask::runAt(DateTime dt_start)
{
    Runnable = true;
    Running = false;
    continueFlag = false;
    dt_runTime = (dt_start);
}


void TriggeredTimeTask::stopAt(DateTime dt_stop)
{
    Runnable = true;
    Running = false;
    continueFlag = true;
    dt_stopTime = (dt_stop);
}


void TriggeredTimeTask::runAgainAt(DateTime dt_next)
{

    TimeSpan delta = dt_stopTime - dt_runTime; // how long was the process running

    runAt(dt_next);
    stopAt(dt_next + delta);
}

void TriggeredTimeTask::runUpdateIn(TimeSpan ts)
{
    Runnable = false;
    Running = true;
    continueFlag = true;
    dt_runUpdateTime = (ts);
}
