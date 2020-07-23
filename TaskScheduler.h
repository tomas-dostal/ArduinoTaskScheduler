/**************************************************************************************
  Description:  ArduinoTaskSheduller TaskSheduller class
  Type:         N/A
  Purpose:      Running/updating tasks if they are allowed to run/update
                Simple (kinda) priority-based task sheduller

  Original copyright: Copyright 2011 Alan Burlison, alan@bleaklow.com.
  All rights reserved. Use is subject to license terms.

  Modified by: Tomas Dostal <t@xn--dostl-0qa.eu> <t@dostál.eu> https://xn--dostl-0qa.eu

**************************************************************************************/

#ifndef TaskScheduler_h
#define TaskScheduler_h

#include "Task.h"

class TaskScheduler 
{

/*
 * Create a new task scheduler.  Tasks are scheduled in priority order,
 * where the highest priority task is first in the array, and the lowest
 * priority task is the last.
 * 
 * Well... "Highest priority" for the first run. Later the cycle goes on again and again
 * by the order, no matter the priority. But maybe I just don't understand the code enough
 */
     
public:

    TaskScheduler(Task **task, uint8_t numTasks);
    TaskScheduler(Task **task, uint8_t numTasks, RTC_DS3231 *rtc);

    void runTasks();

private:

    Task **tasks;   // Array of task pointers.
    int numTasks;   // Number of tasks in the array.
    bool enableRTC; 
    RTC_DS3231 * rtc; 

};

#endif
