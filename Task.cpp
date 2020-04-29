/*
 * Copyright 2011 Alan Burlison, alan@bleaklow.com.  All rights reserved.
 * Use is subject to license terms.
 */
#include <Arduino.h>
#include "Task.h"

// Virtual.
bool TriggeredTask::canRun(uint32_t now) {
    return runFlag;
}

// Virtual.
bool TimedTask::canRun(uint32_t now) {
    return now >= runTime;
}

bool TriggeredTimeTask::canRun(uint32_t now) {
    return (continueFlag && now >= runTime) || (runFlag && now >= runTime);
}
bool TriggeredTimeTask::canUpdate(uint32_t now) {
    return continueFlag && now >= runUpdateTime;
}


void TriggeredTimeTask::setRunnable() { 
  runFlag = true; 
  continueFlag = false; 
}
void TriggeredTimeTask::runAgainIn(uint32_t t) { 
  runFlag = false; 
  continueFlag = true;  
  runTime = millis() + t;  
}
void TriggeredTimeTask::runUpdateIn(uint32_t t) { 
  runFlag = 0; 
  continueFlag = true;  
  runUpdateTime = millis() + t;  
}
