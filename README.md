# KeypadInterrupt
## Arduino Keypad library using interrupts

KeypadInt v1.1, 17/03/2022, Jesus Macias
--------------------------------------------------------------------------------

This is an Arduino library that enables you to read a Keypad using interrupts.
The difference with Keypad.zip (from Mark Stanley and Alexander Brevig) is that 
may pass any time between successive call to getKey() without losing any key. In 
Keypad.zip lib the function getKey() must be called continuously with a minimum 
delay between consecutive calls, otherwise keys can be lost. With the KeypadInt
library the function getKey() does not need to be called continuously.
Keypad lib as of version 3.0 supports multiple key presses while KeypadInt lib
only supports a single key.

KeypadInt lib uses a buffer of up to 10 keys that are stored in case the CPU is
occupied in other tasks.

KeypadInt example is based on example of Keypad.zip library (from Mark Stanley
and Alexander Brevig) but the implementation of the library is different as
is based in interrupts instead of polling.

KeypadInt lib uses overflow interrupt of timer2 (2.048mS) but does not modify 
the configuration of the timer2, so does not affect the AnalogWrite() function
(pins ~3 and ~11 on Arduino Uno) that maintains a PWM with a frequency of
488,28Hz.

KeypadInt lib spends 160us every 51mS (0.3% of CPU) and does not interfere with
Timer0 interrupt that controls the functions millis(), micros() and delay().

As an example of a typical Keypad tested, the time of a momentary key pressed 
varies between a minimum of about 190ms and a maximum of 380ms and the
key pressed has no rebounds. Obviously the maximum time can be increased if 
the key is pressed continously.

To obtain the most recent version of the code or to report issues (or,
better, 
provide fixes), please visit the Github pages at

<https://github.com/toranlo/KeypadInterrupt>.


Inquiries via the Github issue tracker, please.  This way, others may benefit

from the answers.

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
