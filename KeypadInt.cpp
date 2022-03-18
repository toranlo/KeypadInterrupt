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

#include "KeypadInt.h"

//TIMER2 has higher priority than TIMER0
ISR(TIMER2_OVF_vect) //This interrupt enters every 2.048ms
{
static byte numloop=0;

#ifdef DEBUG
  digitalWrite(11,HIGH);//Debug for measuring interrupt time with an scope
#endif
#ifndef DEBUG
  sei(); //Enable to enter other interrupts, particulary Timer0 interrupt that
         //maintains millis(), micros() and delay() functions.
#endif
  numloop++;
  if (numloop==25) // 51.2ms, tick to scan for keys
  {
#ifdef DEBUG
    digitalWrite(13,digitalRead(13)^1); //Scan ticks=51ms
    digitalWrite(12,HIGH); //Scan takes 139us, 23uS when key pressed
#endif    
    numloop=0;
    KeypadInt::scanKeypad();
#ifdef DEBUG
    digitalWrite(12,LOW);
#endif
  }
#ifdef DEBUG
  digitalWrite(11,LOW);
#endif

}
//Measuring pin 11, the interruption lasts the following times:
//24 of 25 times there is no key scan and the interrupt lasts about 6.4uS.
//1 of 25 times there is scan key and the interrupts lasts about 157us. If a key
//is pressed this time is reduced to about 42us


KeypadInt::KeypadInt(char * keys, byte *row, byte *col, byte numRows, byte numCols)
{
#ifdef DEBUG
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT); // LED on Arudino Uno
#endif
  rowPins=row;
  colPins=col;
  ROWS=numRows;
  COLS=numCols;
  hexaKeys=keys;
  KeypadStartInt();
}

void KeypadInt::scanKeypad()
{
byte a;
static byte row, col;
static byte numt=0; //Number of ticks a key is pressed
  if (numt==0) //Scan for a key
  {
    for (byte i=0;i<COLS;i++)
    {
      digitalWrite(colPins[i],0);
      for (byte j=0;j<ROWS;j++)
      {
        a=digitalRead(rowPins[j]);
        if (a==0)
        {
            numt++;
            row=j;col=i;
            break; //Stop on first key found  
        }
      }
      digitalWrite(colPins[i],1);
      if (numt) break; // Stop on first key found
    }
  } else
  {
    //Check that the key is still pressed after rebound time
    numt++;
    if (numt >=3) //2 ticks=100ms rebound
    {
      // Check that the key is still pressed
      digitalWrite(colPins[col],0);
      a=digitalRead(rowPins[row]);
      digitalWrite(colPins[col],1);
      if (a==0) numt=3; //To avoid overflow 
      else 
      {
        putKey(hexaKeys[row*ROWS+col]);
        numt=0;
      }
    }
  }
}

void KeypadInt::KeypadStartInt()
{
  for (byte i=0;i<ROWS;i++)
  {
    pinMode(rowPins[i],INPUT_PULLUP);
  }
  for (byte i=0;i<COLS;i++)
  {
    pinMode(colPins[i],OUTPUT);
    digitalWrite(colPins[i],1);
  }
  bufr=bufw=numk=0;
  Timer2StartInt();
}  

void KeypadInt::Timer2StartInt()
{
  // Timer2 runs at startup at Phase Correct PWM Mode (double ramp)
  // with a 64 pre-scaler-> In an Arduino Uno, 16MHz/64/256/2=488.28Hz -> 
  // tick=2.048ms.
  
  // Start TIMER2 overflow interrupt
  TIMSK2|=_BV(TOIE2);
}  


char KeypadInt::getKey()
{
char key;
  if (numk==0) return(0);

  key=bufk[bufr];
  bufr++;
  if (bufr==NUMKEYS) bufr=0;
  byte oldSREG=SREG;
  cli();
  numk--;
  SREG=oldSREG;

  return(key);
}

void KeypadInt::putKey(char key)
{
  if (numk==NUMKEYS) return; // No space for more keys, the key is lost.
  bufk[bufw]=key;
  bufw++;
  if (bufw==NUMKEYS) bufw=0;
  byte oldSREG=SREG;
  cli();
  numk++;
  SREG=oldSREG;
}

//Allocate space for static variables
byte *KeypadInt::rowPins, *KeypadInt::colPins;
byte KeypadInt::ROWS, KeypadInt::COLS;
char *KeypadInt::hexaKeys;
byte KeypadInt::bufr, KeypadInt::bufw;
volatile byte KeypadInt::numk;
char KeypadInt::bufk[NUMKEYS];
