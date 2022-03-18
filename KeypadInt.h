/*
 KeypadInt v1.1, 17/03/2022, Jesus Macias
--------------------------------------------------------------------------------
Reads a Keypad using interruptions (Timer 2, 2.048ms), so may pass any time 
between calls to getKey() function. Up to 10 keys are buffered.
Does not affect AnalogWrite() on Timer 2 (pines ~3 and ~11 on Arduino Uno) that 
maintains the PWM function at a frequency of 488,28Hz.
In Keypad.zip library the function getkey() uses polling and must be called 
continuously with minimum delay between consecutive calls, otherwise keys can be 
lost. With the KeypadInt library the function getKey() does not need to be 
called continuously.

(C) 2022 Jesus Macias

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/

#ifndef KeypadInt_h
#define KeypadInt_h

#include <Arduino.h>

//#define DEBUG //Uncomment to measure interrupt times with an oscilloscope

#define NUMKEYS 10


class KeypadInt
{
  public:
    KeypadInt(char * keys, byte *row, byte *col, byte numRows, byte numCols);
    char getKey();
    static void scanKeypad(); //Called by ISR(TIMER2_OVF_vect)
    
  private:
    void KeypadStartInt();
    void Timer2StartInt();
    static void putKey(char key);
    
    static byte *rowPins, *colPins;
    static byte ROWS, COLS;
    static char *hexaKeys;
    static byte bufr,bufw;
    static volatile byte numk;
    static char bufk[NUMKEYS];
  
};
#endif
