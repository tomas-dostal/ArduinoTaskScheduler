/**************************************************************************************
  Description:  ArduinoTaskSheduller Display class
  Purpose:      Something like universal interface modified to work properly with 
                20x4 I2C display.

                The idea is, that this class would be the only one who controlls where 
                the text is displayed to prevent overwriting

                You can see several ideas how the screen could look like here: 

//  |----------------|
//  |# HH:MM:SS #100%| #... battery status
//  |#35,3°C #35,3°C | #... temperature char
//  |----------------|

//  |--------------------|
//  |DD:MM:SS DD:MM:RR   |
//  |#35,3°C #100%       |
//  |####Progressbar#####|
//  |  CURRENT_STATUS ##1| status (0,15) connected clients (16,19) [ ##1],
//  OTHERWISE status (0,19)
//  |--------------------|


  Wrote-modified by: Tomas Dostal <t@xn--dostl-0qa.eu> <t@dostál.eu>

 
**************************************************************************************/

#include "Task.h"
#include <LiquidCrystal_I2C.h>

class LCD : public Task
{
public:
  LCD();
  void writeString(String text); //Used for simple debugging of other tasks
  void writeLine(int index, String text);
  void writeLineFrom(int row, int index, String text);

  void drawProgressBar(int task_id, uint32_t var, uint32_t minVal, uint32_t maxVal);
  void drawProcessName(String processName);


  void writeTemperatureChar(int row, int index);
  void writeHumidityChar(int row, int index);
  void writeUserChar();
  void writeWifiChar();
  void writeLockChar(); 

  void clearAll(); //Used for simple debugging of other tasks
  void clearLine(int line); //Used for simple debugging of other tasks

  virtual void run(uint32_t now);   //Override the run() method
  virtual void run(DateTime dt_now);   //Override the run() method

  virtual bool canRun(uint32_t now){return true;};  //Override the canRun() method
  virtual bool canRun(DateTime dt_now){return true;};  

  virtual void update(uint32_t now){};   //Override the run() method
  virtual void update(DateTime dt_now){};   //Override the run() method

  virtual bool canUpdate(uint32_t now){return false;};  //Override the canRun() method
  virtual bool canUpdate(DateTime dt_now){return false;};  //Override the canRun() method

  
  bool isAvailable() {return availability;};
  void setOwner(int task_id) {availability = false; task_using = task_id;  clearLine(3); };
  void doneDrawing() {availability = true; task_using = -1; clearLine(2); clearLine(3); };


protected: 
  bool availability = true; 
  int task_using = -1;
  String task_name = ""; 
  
private:
  LiquidCrystal_I2C* p_lcd; //    lcd(0x3F, 20, 4);

  
  byte bar1[8] = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 };
  byte bar2[8] = { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 };
  byte bar3[8] = { 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C };
  byte bar4[8] = { 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E };
  byte bar5[8] = { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F };



  byte humidity[8] =    {0b00000,  0b00100,  0b00100,  0b01010,  0b10001,  0b10011,  0b10011,  0b01110};
  byte lock[8] =        {0b00000,  0b01110,  0b01010,  0b01010,  0b11111,  0b11011,  0b11011,  0b11111};
  byte temperature[8] = {0b00100,  0b01010,  0b01010,  0b01110,  0b01110,  0b11111,  0b11111,  0b01110};
  byte humidity_2[8] =  {0b00100,  0b00100,  0b01110,  0b01010,  0b10001,  0b10011,  0b10111,  0b01110};
  byte wifi_part1[8] =  {0b00111,  0b01100,  0b01000,  0b00011,  0b00100,  0b00000,  0b00011,  0b00011};
  byte wifi_part2[8] =  {0b10000,  0b01000,  0b00000,  0b00000,  0b10000,  0b00000,  0b00000,  0b00000};
  byte lock_part1[8] =  {0b00001,  0b00010,  0b00010,  0b00111,  0b00100,  0b00100,  0b00100,  0b00111};
  byte lock_part2[8] =  {0b10000,  0b01000,  0b01000,  0b11100,  0b00100,  0b00100,  0b00100,  0b11100};
  byte user[8]       =  {0b01100,  0b01100,  0b11110,  0b11110,  0b11110,  0b01100,  0b01100,  0b01100};


  


};
