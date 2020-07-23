/*********************************************************************************
*  Class:    Button
*  Task Type:  TimedTask (runs when a specific time interval is reached)
*  Purpose:  Controls button(s) and runs specific defined actions 
* + writes debug messages
*       
*       (e.g. "Button: short press" and "Button: long press")
*       
*********************************************************************************/

#include "Task.h"

class Button : public TimedTask {

public:
    // Constructor
    Button(uint8_t _pin, TriggeredTimeTask* _task_to_run, Debugger* _ptrDebugger);

    virtual void run(uint32_t now);
    virtual void run(DateTime dt_now);

    virtual void update(uint32_t now){};
    virtual void update(DateTime dt_now){};

    virtual String getTaskName() { return task_name; };
    virtual void setTaskName(String s) { task_name = s; };

private:
    Debugger* ptrDebugger; // Pointer to debugger
    TriggeredTimeTask* ptrTaskToRun; // pointer to the task you want to run

    bool debug_enabled;
    int buttonPin; // the number of the pushbutton pin

    const int shortPressTime = 500; // 500 milliseconds
    const int longPressTime = 1000; // 1000 milliseconds
    const int debounce = 50; // 50 milliseconds

    // Variables will change:
    int lastState = HIGH; // the previous state from the input pin
    int currentState; // the current reading from the input pin
    unsigned long pressedTime = 0;
    unsigned long releasedTime = 0;
};
