/**************************************************************************************
  Description:  ArduinoTaskSheduller TaskSheduller class
  Type:         N/A
  Purpose:      Running/updating tasks if they are allowed to run/update
                Simple priority-based task sheduller

  Original copyright: Copyright 2011 Alan Burlison, alan@bleaklow.com.
  All rights reserved. Use is subject to license terms.

  Modified by: Tomas Dostal <t@xn--dostl-0qa.eu> <t@dostál.eu> https://xn--dostl-0qa.eu

**************************************************************************************/


#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

// needed for RTClib tasks
#include "RTClib.h"

#include "TaskScheduler.h"

TaskScheduler::TaskScheduler(Task** _tasks, uint8_t _numTasks) : 
    tasks(_tasks),
    numTasks(_numTasks), 
    enableRTC(false)
{
}
TaskScheduler::TaskScheduler(Task** _tasks, uint8_t _numTasks, RTC_DS3231* myrtc) : 
    tasks(_tasks),
    numTasks(_numTasks), 
    rtc(myrtc),
    enableRTC(true)
{
}

void TaskScheduler::runTasks()
{

    while (1)
    {
        uint32_t now = millis();
        DateTime dt_now = (*rtc).now();


        Task** tpp = tasks;


        for (int x = 0; x < numTasks; x++)
        {

            Task* tp = *tpp;

            // set task ID. It might be great to move it outside of the loop 
            // and one day somebody should definitely do that
            
            if (!tp -> hasTaskID())
                tp -> setTaskID(x); 

            if (tp -> isRtcTask()) // RTC task
            {
                if (tp -> canRun(dt_now))
                {
                    tp -> run(dt_now);
                    break;
                }
                else if (tp->canUpdate(dt_now))
                {
                    tp -> update(dt_now);
                    break;
                }
            }
            else // NON-RTC task
            {
                if (tp -> canRun(now))
                {
                    tp -> run(now);
                    break;
                }
                else if (tp -> canUpdate(now))
                {
                    tp -> update(now);
                    break;
                }
            }
            tpp++; // next Task
        }
        yield();
    }
}
