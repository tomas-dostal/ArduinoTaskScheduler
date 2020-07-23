/**************************************************************************************
  Description:  ArduinoTaskSheduller Task class
  Type:         N/A
  Purpose:      Defining types of Tasks

  Original copyright: Copyright 2011 Alan Burlison, alan@bleaklow.com.
  All rights reserved. Use is subject to license terms.

  Modified by: Tomas Dostal <t@xn--dostl-0qa.eu> <t@dostál.eu> https://xn--dostl-0qa.eu

**************************************************************************************/

#ifndef Task_h
#define Task_h

#include "RTClib.h"

#include <stdint.h>

// Maximum time into the future - approximately 50 days.
#define MAX_TIME UINT32_MAX

class Task
{

    /*
      Newbie Note: The "= 0;" following each virtual method
      in the Task Class makes it an "abstract" or "pure virtual"
      method in C++. These methods *must* be defined/implemented in the
      derived class. These are *only* Task Class. -KG 3-19-2019
    */

public:
    virtual bool canRun(DateTime dt_now) = 0; //<--ABSTRACT
    virtual bool canUpdate(DateTime dt_now) = 0; //<--ABSTRACT
    virtual void run(DateTime dt_now) = 0; //<--ABSTRACT
    virtual void update(DateTime dt_now) = 0;

    virtual bool canRun(uint32_t now) = 0; //<--ABSTRACT
    virtual bool canUpdate(uint32_t now) = 0; //<--ABSTRACT
    virtual void run(uint32_t now) = 0; //<--ABSTRACT
    virtual void update(uint32_t now) = 0;
    virtual bool isRtcTask()
    {
        return false;
    };

    virtual void setTaskName(String s)
    {
        task_name = s;
    };
    virtual String getTaskName()
    {
        return task_name;
    };

    virtual void setTaskID(int id)
    {
        taskID = id;
    };
    virtual bool hasTaskID()
    {
        if (taskID == -1)
            return false;
        return true;
    };
    virtual int getTaskID()
    {
        return taskID;
    };

protected:
    String task_name = "Unnamed Task";
    int taskID = -1;
};

/*
 * A task that is triggered by an external event.
 */
class TriggeredTask : public Task
{

public:
    /*
     * Can the task currently run?
     * now - current time, in milliseconds.
     */
    virtual bool canRun(uint32_t now);
    virtual bool canRun(DateTime dt_now); //<--ABSTRACT

    // virtual void update(uint32_t now);
    // virtual void update(DateTime dt_now);

    virtual bool canUpdate(uint32_t now)
    {
        return false;
    };
    virtual bool canUpdate(DateTime dt_now)
    {
        return false;
    };

    /*
     * Mark the task as runnable.
     */
    inline void setRunnable()
    {
        runFlag = true;
    }
    inline void setRunnable(bool val)
    {
        runFlag = val;
    }

    /*
     * Mark the task as non-runnable.
     */
    inline void resetRunnable()
    {
        runFlag = false;
    }

    virtual bool isRtcTask()
    {
        return false;
    };


    virtual void setTaskName(String s)
    {
        task_name = s;
    };
    virtual String getTaskName()
    {
        return task_name;
    };

    // void setTaskID(int id){ taskID = id; };
    // int getTaskID(){ return taskID; };


protected:
    bool runFlag; // True if the task is currently runnable.
    String task_name = "Unnamed TriggeredTask";
};

/*
 * A task that is run on a periodic basis.
 */
class TimedTask : public Task
{

public:
    /*
     * Create a periodically executed task.
     * when - the system clock tick when the task should run, in milliseconds.
     */
    inline TimedTask(uint32_t when)
    {
        runTime = when;
    }
    inline TimedTask(DateTime when)
    {
        runTime = 0;
    } // TODO

    /*
     * Can the task currently run?
     * now - current system clock tick, in milliseconds.
     */
    virtual bool canRun(uint32_t now);
    virtual bool canRun(DateTime dt_now);

    virtual bool canUpdate(uint32_t now)
    {
        return false;
    };
    virtual bool canUpdate(DateTime dt_now)
    {
        return false;
    };

    // virtual bool isRtcEnabled();


    /*
     * Set the system clock tick when the task can next run.
     * when - the system clock tick when the task should run, in milliseconds.
     */
    inline void setRunTime(uint32_t when)
    {
        runTime = when;
    }

    /*
     * Increment the system clock tick when the task can next run.
     * inc - system clock increment, in milliseconds.
     */
    inline void incRunTime(uint32_t inc)
    {
        runTime += inc;
    }

    /*
     * Get the system clock tick when the task can next run.
     * return - system clock tick when the task is next due to run.
     */
    inline uint32_t getRunTime()
    {
        return runTime;
    }

    virtual bool isRtcTask()
    {
        return false;
    };


    virtual void setTaskName(String s)
    {
        task_name = s;
    };
    virtual String getTaskName()
    {
        return task_name;
    };

protected:
    uint32_t runTime; // The  system clock tick when the task can next run.
    DateTime* dt_runTime;
    String task_name = "Unnamed TimedTask";
    // int taskID = -1;
};


class TriggeredTimeTask : public Task
{

public:
    // Check if task is allowed to run now (checked by TaskScheduler)
    bool canRun(uint32_t now);

    // Alternative for RTClib
    bool canRun(DateTime dt_now);

    // kinda new feature - you can use it in case u need to write updated to the screen during
    // process run
    bool canUpdate(uint32_t now);
    // same here, but for Tasks where RTClib constructor was used
    bool canUpdate(DateTime dt_now);


    uint32_t started()
    {
        return runTime;
    };

    // Yoou have to set this type of task runnable to be executed
    void setRunnable(bool value);
    void setRunnable();


    inline void resetRunnable()
    {
        Runnable = false;
        continueFlag = false;
    };

    void runAgainAt(DateTime dt_next); // use AFTER process runned to repeat it at dt_next
    void runAgainAt(uint32_t t); // use AFTER process runned to repeat it at "t"

    // used if you want to periodically update something e.g. on display during the process
    void runUpdateIn(uint32_t t);
    // -||- but minimum update period is 1 second because of TimeSpan limitations
    void runUpdateIn(TimeSpan ts);

    // You can set the taks's name by
    void setTaskName(String s)
    {
        task_name = s;
    };
    String getTaskName()
    {
        return task_name;
    };

    void runAt(uint32_t start);
    void stopAt(uint32_t stop);

    void runAt(DateTime dt_start);
    void stopAt(DateTime dt_stop);

    void disablePeriod()
    {
        periodic = false;
    };
    void enablePeriod()
    {
        periodic = true;
    };

    void setPeriod(uint32_t p)
    {
        period = p;
        periodic = true;
    };
    void setPeriod(TimeSpan ts)
    {
        ts_period = ts;
        periodic = true;
    };

    // virtual void restartTask(uint32_t now);
    // virtual void restartTask(DateTime dt_now);

    void showProcessName(bool v)
    {
        showPName = v;
    };
    void showProgressBar(bool v)
    {
        showPBar = v;
    };

    // Maybe this could be renamed in the future.
    // It just returns the flags whether you wish to display
    // process name and/or progress bar on screen.

    // However it all depends on Display class if all of this info
    // appears or not, since there is only one process at time allowed to
    // write on screen (controlled by process ID)

    bool showProcessName()
    {
        return showPName;
    };
    bool showProgressBar()
    {
        return showPBar;
    };

    // Just in case you need to invert output because of
    // rellay running on LOW intead if HIGH or whatever

    virtual void invertOutput()
    {
        inverted = !inverted;
    };


protected:
    virtual bool isRtcTask()
    {
        return rtcTask;
    };
    virtual bool isRunnable()
    {
        return Runnable;
    };
    virtual bool isRunning()
    {
        return Running;
    };
    virtual bool isContinueFlag()
    {
        return continueFlag;
    };

    virtual bool isPeriodic()
    {
        return periodic;
    };
    virtual bool isOutputInverted()
    {
        return inverted;
    };

    uint32_t runTime; // The  system clock tick when the task can run.
    uint32_t stopTime; // The  system clock tick when the task stops.
    uint32_t runNextUpdateTime;

    DateTime dt_runTime;
    DateTime dt_stopTime;
    TimeSpan dt_runUpdateTime;

    bool periodic = false;
    uint32_t period;
    TimeSpan ts_period;

    bool showPName = false;
    bool showPBar = false;
    bool inverted = false;
    bool rtcTask;
    bool Runnable;
    bool Running;
    bool continueFlag; // True if the task is currently runnable.

    String task_name = "Unnamed TriggeredTimeTask";
};

#endif
