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



   
    Author of modification of modification of... eeh: 
  
    Tomas Dostal admin@dostál.eu (https://dostál.eu)
    ArtuinoTaskScheduler https://github.com/tomas-dostal/ArduinoTaskScheduler
    
     * **************************************************************************************

*/

#include <Arduino.h>

// ***
// *** Include Task Scheduler library
// *** (should be located in same folder as this .ino file)
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
#include "OneTimeExecute.h"


// setup RTC:
#include "RTClib.h"
RTC_DS3231 myrtc;



// ***
// *** Pinout for Arduino Uno
// ***
#define PHOTOCELL_PIN			1
#define LED_BLINKER				LED_BUILTIN
#define LED_FADER				LED_BUILTIN
#define LED_LIGHTLEVEL_ALARM	4
#define LED_LIGHTLEVEL_OK		5


#define TEMP_REALOAD   5000
#define TEMP_PIN    D8


// ***
// *** Timed Task intervals
// ***
#define RATE_PHOTOCELL_READING	3000	//Read Photocell	- Timed Task
#define RATE_BLINKER_BLINK	200		//Blink LED_BLINKER	- Timed Task
#define RATE_FADER_FADE			50		//Fade LED_FADER	- Timed Task

// ***
// *** Utility defines
// ***
#define INCREMENT_FADER_STEP			5	//Value used to increment/decrement LED_FADER
#define LIGHT_LEVEL_LOWER_THRESHOLD		300	//Value used to determine if LightLevelAlarm is triggered



/**************************************************************************************
	Description:	void setup()
	Purpose:		Setup for RTC module
  
**************************************************************************************/
void setup()
{
  Serial.begin(115200);
  while (! myrtc.begin()) {
    Serial.println("Couldn't find RTC");
    yield;
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
  Serial.println("TESt");
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
  Debugger			debugger;

  LCD           mylcd;


  Blinker				blinker(LED_BLINKER,
                        RATE_BLINKER_BLINK);

  Temperature   temperature(TEMP_PIN,
                            TEMP_REALOAD,
                            &debugger, &mylcd);
  // OneTimeExecute::OneTimeExecute(String _taskname, uint8_t _pin, DateTime _dt_startTime, TimeSpan _dt_activeTime, Debugger *_ptrDebugger, LCD * _ptr_lcd)

  OneTimeExecute  onetimeexecute("OneTimeExecute", D6, 10000 , &debugger, &mylcd);
  DateTime d (2020, 5, 4, 21, 04, 45);

  TimeSpan ts (0, 0, 1, 12);
  OneTimeExecute  onetimeexecute2("Test@", D6, &d , &ts, &debugger, &mylcd);
  onetimeexecute2.setRunnable();

  onetimeexecute.setRunnable();

  PrintTime     printtime( 1000,
                           &debugger, &mylcd);

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
  Serial.println("TEST");
  Task *tasks[] = {

    &debugger,

    &blinker,
    &onetimeexecute,
    &onetimeexecute2,

    &printtime,
    &temperature,
    &mylcd // here might be an error

    /*
      &advancedBlinker,
      &fader,
      &lightLevelAlarm,
    */
  };

  // ***
  // *** Instantiate the TaskScheduler and fill it with tasks.
  // ***
  TaskScheduler scheduler(tasks, NUM_TASKS(tasks));
  
  // GO! Run the scheduler - it never returns.

  scheduler.runTasks();

}
