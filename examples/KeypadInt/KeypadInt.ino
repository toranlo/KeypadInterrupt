/*
 KeypadInt v1.2, 19/03/2022, Jesus Macias
--------------------------------------------------------------------------------
Example of reading a Keypad using interruptions (Timer 2, 2.048ms), so may pass 
any time between calls to the getKey() function. Up to 10 keys are buffered.
Does not affect AnalogWrite() on Timer 2 (pines ~3 and ~11 on Arduino Uno) that 
maintains the PWM function at a frequency of 488,28Hz.
In the Keypad.zip library the function getkey() uses polling and must be called 
continuously with minimum delay betwen consecutive calls, otherwise keys can be 
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

#include "KeypadInt.h"

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
                                   //Rows are the input_pullup pins.
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
                                   //Columns are the output pins.

//initialize an instance of class KeypadInt
//KEY_PRESSED: Key is generated on press
//KEY_RELEASED: Key is generated on release (default for compatibility with v1.1)
KeypadInt Keypad( (char *)hexaKeys, rowPins, colPins, ROWS, COLS, KEY_PRESSED);
//KeypadInt Keypad( (char *)hexaKeys, rowPins, colPins, ROWS, COLS); // For v1.1

void setup(){
  Serial.begin(9600);
}
  
void loop(){
  char Key = Keypad.getKey();
  
  if (Key!=0) {
    Serial.println(Key);
  }
  // Do any process required by the program,
  // e.g. can be simulated with an delay.
  delay(1000); // 1Sec process, can be any value as keys are buffered as they 
               // are pressed.
}

