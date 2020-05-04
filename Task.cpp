/*
 * Copyright 2011 Alan Burlison, alan@bleaklow.com.  All rights reserved.
 * Use is subject to license terms.
 */
#include <Arduino.h>
#include "Task.h"
#include "RTClib.h"

// Virtual.
bool TriggeredTask::canRun(uint32_t now) {
    return runFlag;
}

// Virtual.
bool TimedTask::canRun(uint32_t now) {
    return now >= runTime;
}

// Virtual.
bool TimedTask::canRun(DateTime dt_now) {
    return dt_now.unixtime() >= dt_runTime->unixtime();
}

// ####################### TriggeredTimeTask  ###################
/*     virtual bool isRtcTask(){return rtcTask;};
    
    virtual bool isRunnable() { return Runnable; }; 
    virtual bool isRunning() { return Running; }; 
    virtual bool isUpdating() { return Updateing; }; 
    */  
bool TriggeredTimeTask::canRun(uint32_t now) {
    return (isRunnable() && now >= runTime) || ( isRunning() && now >= runTime);
}

bool TriggeredTimeTask::canRun(DateTime dt_now) {
  Serial.print("now  "); 
  Serial.print(dt_now.unixtime()); 
  Serial.print (" run at : "); 
  if(dt_runTime != NULL)
    Serial.print(dt_runTime->unixtime()); 

  if(!dt_runTime) 
  {
    Serial.println("dt_runTime is null"); 
    return false; 
  }

  //Serial.println(String("DateTime::TIMESTAMP_FULL:\t")+dt_runTime->timestamp(DateTime::TIMESTAMP_FULL));

  // can run if
  //  - is runnable and it right time
  return (isRunnable() && dt_now.unixtime() >= dt_runTime->unixtime()) || (isContinueFlag() && dt_now.unixtime() >= dt_runTime->unixtime());
}

bool TriggeredTimeTask::canUpdate(uint32_t now) 
{
    return( isRunning() && now >= runNextUpdateTime) || (isContinueFlag() && now >= runNextUpdateTime);
}

bool TriggeredTimeTask::canUpdate(DateTime dt_now) {
    return isRunning(); // && dt_now.unixtime() >= ((*dt_runTime) + (*dt_runUpdateTime)).unixtime();
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
void TriggeredTimeTask::runAgainIn(uint32_t t) 
{  
  Runnable = true; 
  Running = false; 
  continueFlag = true;  
  runTime = millis() + t;  
}

void TriggeredTimeTask::runUpdateIn(uint32_t t) { 
  Runnable = false; 
  Running = true; 
  continueFlag = true;  
  runNextUpdateTime = millis() + t;  
}
