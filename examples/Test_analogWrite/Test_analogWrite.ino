/*
 Test_analogWrite v1.2, 19/03/2022, toranlo
--------------------------------------------------------------------------------
Test to verify that the function AnalogWrite() still works on pins ~3 and ~11 on
an Arduino Uno (Pines affected by the Timer2 used by the KeypadInt library).
Like pin ~3 is used by the Keypad, only pin ~11 will be tested.

Connect a LED diode (catode to GND) in series with a 220 Ohm resistor between pin
~11 and GND, and press any key in the Keypad:
  '1': Start
  '2': Stop
  '3': Up
  'A': Down
  '4': Fast
  '5': Slow

(C) 2022 toranlo

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

const byte TIMER2_OC2A_PIN = 11; //Pin ~11 of the arduino Uno used by
                                 //the Timer2 for the analogWrite() 
                                 //function.
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
  static byte intensity=1, speed=10, start=1; 
  static int direcction=1;
  char Key;
  
  Key=Keypad.getKey();
  if (Key!=0) {
    Serial.print(Key);Serial.print(" ");
    switch (Key)
    {
    case '1':
      start=1;
      Serial.println("START");
      break;
    case '2':
      start=0;
      Serial.println("STOP");
      break;
    case '3':
      start=1;
      direcction=1;
      Serial.println("UP");
      break;
    case 'A':
      start=1;
      direcction=-1;
      Serial.println("DOWN");
      break;
    case '4':
      start=1;
      speed=10;
      Serial.println("FAST");
      break;
    case '5':
      start=1;
      speed=30;
      Serial.println("SLOW");
      break;
    }
  }

  if (start)
  {
    analogWrite(TIMER2_OC2A_PIN,intensity);
    if (direcction==1) intensity+=1;
    else intensity-=1;
    delay(speed); //ms
  }
}

