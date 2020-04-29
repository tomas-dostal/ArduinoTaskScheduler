/*
 * Copyright 2011 Alan Burlison, alan@bleaklow.com.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 * Simple task classes.
 */
#include "Task.h"
#include <LiquidCrystal_I2C.h>

class LCD : public Task
{
public:
  LCD();
  void writeString(String text); //Used for simple debugging of other tasks
  void writeLine(int index, String text);
  void writeLineFrom(int row, int index, String text);

  void drawProgressBar(int row, uint32_t var, uint32_t minVal, uint32_t maxVal);

  void writeCharTemperature(int row, int index);
  void writeCharHumidity(int row, int index);

  void clearAll(); //Used for simple debugging of other tasks
  void clearLine(int line); //Used for simple debugging of other tasks

  virtual void run(uint32_t now);   //Override the run() method
  virtual bool canRun(uint32_t now);  //Override the canRun() method
  
  virtual void update(uint32_t now);   //Override the run() method
  virtual bool canUpdate(uint32_t now){return false;};  //Override the canRun() method
  
private:
  LiquidCrystal_I2C* p_lcd; //    lcd(0x3F, 20, 4);
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
