/**************************************************************************************
  Description:  ArduinoTaskSheduller Debugger class
  Type:         Simple Task
  Purpose:      Universal interface for debugging

  Based on: Kevin Gagnon https://github.com/gadgetstogrow/TaskScheduler

  Original copyright: Copyright 2011 Alan Burlison, alan@bleaklow.com.
  All rights reserved. Use is subject to license terms.

  Modified by: Tomas Dostal <t@xn--dostl-0qa.eu> <t@dostál.eu> https://xn--dostl-0qa.eu

**************************************************************************************/

#include "Task.h"

// Define the Debugger Class as type Task
class Debugger : public Task
{
public:
    Debugger(); // empty contructor
    Debugger(int costumBaudRate); // contructor

    void debugWrite(String debugMsg); // Used for simple debugging of other tasks
    virtual void run(uint32_t now); // Override the run() method
    virtual void run(DateTime dt_now); // Override the run() method

    virtual void update(uint32_t now); // Override the run() method
    virtual void update(DateTime dt_now);

    virtual bool canRun(uint32_t now); // Override the canRun() method
    virtual bool canRun(DateTime dt_now); // Override the canRun() method

    virtual bool canUpdate(uint32_t now)
    {
        return false;
    }; // Override the canRun() method
    virtual bool canUpdate(DateTime dt_now)
    {
        return false;
    }; // Override the canRun() method
};
