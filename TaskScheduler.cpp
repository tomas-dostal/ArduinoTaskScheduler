/*
 * Copyright 2011 Alan Burlison, alan@bleaklow.com.  All rights reserved.
 * Use is subject to license terms.
 */

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include "RTClib.h"
RTC_DS3231 t; 



#include "TaskScheduler.h"

TaskScheduler::TaskScheduler(Task **_tasks, uint8_t _numTasks) :
  tasks(_tasks),
  numTasks(_numTasks), 
  enableRTC(false)
{
}
/*TaskScheduler::TaskScheduler(Task **_tasks, uint8_t _numTasks) :
  tasks(_tasks),
  numTasks(_numTasks),
  
{
}
*/
void TaskScheduler::runTasks() 
{
    while (1) 
    {
      //Serial.println("runTasks()"); 
        uint32_t now = millis();
        DateTime dt_now = t.now(); 
        
        Task **tpp = tasks;
        for (int x = 0; x < numTasks; x++) 
        {
            Task *tp = *tpp;
            if(tp->isRtcTask())  // RTC task
            {
               if (tp->canRun(dt_now))
                {
                    tp->run(dt_now);
                    break;
                }
                else if(tp->canUpdate(dt_now))
                {
                  tp->update(dt_now);
                  break;
                } 
            }
            else // NON-RTC task
            {
                if (tp->canRun(now))
                {
                    tp->run(now);
                    break;
                }
                else if(tp->canUpdate(now))
                {
                  tp->update(now);
                  break;
                }
            }
            tpp++; // next Task
        }
        yield; 
    }
}
