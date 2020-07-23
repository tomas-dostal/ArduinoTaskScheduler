/* This code is a modification of modification of ....

   To be exact it is modification based on Kevin Gagnon's modification of Alan Burlison's Task Scheduler Library.
   I'll include here all the credit and licence stuff and hopefully everything will be okay and nobody gets hurt.

   CREDIT:

    Alan Burlison's Copyright Alan Burlison, 2011
    Original Source Code: http://bleaklow.com/files/2010/Task.tar.gz
    Original Reference:   http://bleaklow.com/2010/07/20/a_very_simple_arduino_task_manager.html

    Kevin Gagnon's (@GadgetsToGrow  https://www.hackster.io/GadgetsToGrow) modification
    https://github.com/gadgetstogrow/TaskScheduler
    from date: 17.03.2019
    Copyright 2019, Kevin Gagnon


   CREDIT: other libraries

    Adafruit library (DHT sensor)  https://github.com/adafruit/DHT-sensor-library
    Licence: MIT

    Adafruit library (RTClib)  https://github.com/adafruit/RTClib
    Licence: MIT

    Adafruit Unified Sensor (for RTClib)  https://github.com/adafruit/Adafruit_Sensor
    Licence: Apache 2

    LiquidCrystal_I2C https://www.arduinolibraries.info/libraries/liquid-crystal-i2-c
    Licence: Unknown

    Display:DrawProgressBar inspired by https://forum.arduino.cc/index.php?topic=180678.0
    Licence: Unknown


    Modified by: Tomas Dostal <t@xn--dostl-0qa.eu> <t@dostál.eu> https://xn--dostl-0qa.eu
    ArtuinoTaskScheduler https://github.com/tomas-dostal/ArduinoTaskScheduler
    
     * **************************************************************************************

*/

#include <Arduino.h>

// ***
// *** Include all Classes you want to use
// *** Necessary ones: "Task.h", "TaskScheduler.h", ("RTClib.h")
// ***

#include "Task.h"
#include "TaskScheduler.h"
#include "Debugger.h"
#include "Display.h"
#include "Fader.h"
#include "Blinker.h"
#include "AdvancedBlinker.h"
#include "Temperature.h"
#include "PrintTime.h"
#include "TimeExecute.h"
#include "Button.h"


// setup RTC:
#include "RTClib.h"
RTC_DS3231 myrtc;


/**************************************************************************************
	Description:	void setup()
	Purpose:		Setup for RTC module
  
**************************************************************************************/
void setup()
{
   Wire.begin(D4,D5); //  Wire.begin(SDA,SCL) I had to move SDA, SCL pins. Feel free to comment this line
   
  Serial.begin(115200);

  while (! myrtc.begin()) {
    Serial.println("Couldn't find RTC");
    yield();  // needed for ESP8266, otherwise creshes
  }

  if (myrtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // If the RTC have lost power it will sets the RTC to the date & time this sketch was compiled in the following line
    myrtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  // If you need to set the time of the uncomment line 34 or 37
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}



// Main Loop
void loop()
{
  /***************************************************************************************
  	Description:	Instantiate the tasks and schedule task priority.
  	Purpose:		This is the heart of the program.  All of the needed code has been
  					encapsulated in the above Class definitions. The code below will
  					create the task objects based on these classes and fill a task
  					array for the TaskScheduler to manage.


  	Note:			Although this is located in the loop() routine, this will only
  					run once. TaskScheduler::run() never returns control to loop().
  ***************************************************************************************/

  // ***
  // *** Instantiate the task objects for use by the TaskScheduler
  // ***

  // debugger instance 
  Debugger			debugger;
  debugger.setTaskName("Debugger controller"); 

  // LCd instance - if you have different screen that 20x4, modify Display.cpp
  LCD           mylcd;
  mylcd.setTaskName("LCD controller"); 


  // *************** START HERE *******************

  // simple Blinker class. blinker(PIN, periodInMilliseconds, pointerToDebugger); 
  Blinker				blinker(LED_BUILTIN, 400, &debugger);
  blinker.setTaskName("Blinker 1"); 

  // Temperature ( sensorPin, refreshInMS, debuggerPoiner, lcdPointer); 
  Temperature   temperature(D8, 4000, &debugger, &mylcd);
  temperature.setTaskName("Temperature"); 

  
  //  TimeExecute  timeexecute(LED_PIN, DELAY_FROM_START, ACTIVE_TIME, &debugger, &mylcd);
  TimeExecute  t(D8, 500, 6000 , &debugger, &mylcd);
  t.setRunnable(true);          // you have to set the task runnable to be executed 
  t.setPeriod(5000);            // if you want the task to repeat, set repeat period. It is a time after the task ends. 
  t.setTaskName("Watering..."); // You might want to set name to the task. It 
  t.showProcessName(true); 
  t.showProgressBar(true); 
  //t.invertOutput();            // In case you need to invert output (e.g. rellay runs on logical LOW), uncomment this line 

  // now add button to controll TimeExecute t. 
  // Button (buttonPin, pointerToInstanceToBeControlled, pointerToDebugger)
  Button btn1 (D6, &t, &debugger); 
  btn1.setRunTime(100); // run 100ms after start 
  

  // PrintTime (refreshInMS, debuggerPoiner, lcdPointer); 
  PrintTime printtime( 500, &debugger, &mylcd);
  printtime.setTaskName("PrintTime"); 

   
  DateTime startTime (2020, 7, 21, 22, 18 , 30);
  TimeSpan activeTime ( 0, 0, 0, 20); // ( days, hours, minutes, seconds)
  
  TimeExecute  te2(D1, &startTime , &activeTime, &debugger, &mylcd);
  te2.setRunnable(true);
  te2.setTaskName("TimeExecute2");



  TimeExecute  timeexecute3(LED_BUILTIN, 1000 , &debugger, &mylcd);
  


  /*


    AdvancedBlinker
                  advancedBlinker(LED_BLINKER,
                  5*RATE_BLINKER_BLINK,
                  10*RATE_BLINKER_BLINK,
                  &debugger, &mylcd);



  	Fader				fader(LED_BUILTIN,
  							  INCREMENT_FADER_STEP,
  							  RATE_FADER_FADE,
  							  &debugger);


  	LightLevelAlarm		lightLevelAlarm(LED_LIGHTLEVEL_OK,
  										LED_LIGHTLEVEL_ALARM);

  */

  // ***
  // *** Create an array of pointers (eek!) to the task objects we just instantiated.
  // ***
  // *** The order matters here.  When the TaskScheduler is running it finds the first task it can
  // *** run--canRun(), runs the task--run(), then returns to the top of the list and starts the
  // *** process again. I've experimented with different orders, but couldn't find any astonishing
  // *** differences; and considering this isn't a "real" application, this'll do.  One other note:
  // *** of all the tasks, communicating with the Serial Monitor via the Debugger object is the by
  // *** far the biggest tax on the MCU, but can be removed fairly from your final release. Have fun.
  // ***
  
  // ********************** Add/modify instances you want to run here *****************
  Task *tasks[] = {

    &debugger,
    &blinker,
    &t,
    &te2,
    &printtime,
    &temperature,
    &btn1,
    &mylcd // this should be last one
    //&advancedBlinker,
    //&fader,
    //&lightLevelAlarm,
  };

  // ***
  // *** Instantiate the TaskScheduler and fill it with tasks.
  // ***


  TaskScheduler scheduler(tasks, sizeof(tasks)/sizeof(tasks[0]) /*NUM_TASKS(tasks)*/, &myrtc);
  
  // GO! Run the scheduler - it never returns.

  scheduler.runTasks();

}
