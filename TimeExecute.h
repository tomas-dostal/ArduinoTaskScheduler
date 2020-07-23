/*********************************************************************************
*  Class:    TimeExecute
*  Task Type:  TriggeredTimedTask (runs when a specific time interval is reached or when triggered)
*  Purpose:  Unversal Task class
*       
*  Created by: Tomas Dostal <t@xn--dostl-0qa.eu> <t@dostál.eu> https://xn--dostl-0qa.eu
*  
*********************************************************************************/
#include "Task.h"

class TimeExecute : public TriggeredTimeTask {
public:

    TimeExecute(uint8_t _pin, uint32_t _active_time, Debugger* _ptrDebugger, LCD* _ptr_lcd);
    TimeExecute(uint8_t _pin, uint32_t _delay, uint32_t _active_time, Debugger* _ptrDebugger, LCD* _ptr_lcd);
    TimeExecute(uint8_t _pin, DateTime* _dt_startTime, TimeSpan* _dt_activeTime, Debugger* _ptrDebugger, LCD* _ptr_lcd);

    virtual void run(uint32_t now);
    virtual void update(uint32_t now);

    virtual void run(DateTime dt_now);
    virtual void update(DateTime dt_now);

    virtual bool isRtcTask() { return RtcTask; };

private:

    bool on = false; // Current state of the LED.
    uint8_t pin; // LED pin.

    bool RtcTask; // Current state of the LED.

    Debugger* ptrDebugger; // Pointer to debugger
    LCD* ptr_lcd; // Pointer to debugger
};
