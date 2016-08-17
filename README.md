# PololuHD44780 LCD library for Arduino

Version: 2.0.0<br/>
Release date: 2016-08-17<br/>
[![Build Status](https://travis-ci.org/pololu/pololu-hd44780-arduino.svg?branch=master)](https://travis-ci.org/pololu/pololu-hd44780-arduino)<br>
[www.pololu.com](https://www.pololu.com/)

## Summary

This is a C++ library for the Arduino IDE that allows you to control an LCD that uses the Hitachi HD44780 protocol.  This library is very similar to the [LiquidCrystal library](https://arduino.cc/en/Reference/LiquidCrystal), but it provides more separation between the low-level hardware access functions and the high-level functions so that the low-level functions can be replaced if necessary.

This library assumes that you have the RS, E, DB4, DB5. DB6, and DB7 pins of the LCD connected to your microcontroller, and that the RW pin is pulled low.  With this configuration, it is not possible to poll the LCD's busy flag, so blocking delays are used after each command in order to give it time to complete.

## Supported platforms

This library was designed to be used with the Arduino IDE versions 1.0.x or 1.5.x and will probably not work with earlier versions.

This library should work with any board that can be programmed by those versions of the Arduino IDE.

## Getting Started

### Software

If you are using version 1.6.2 or later of the
[Arduino software (IDE)](https://www.arduino.cc/en/Main/Software), you can use
the Library Manager to install this library:

1. In the Arduino IDE, open the "Sketch" menu, select "Include Library", then
   "Manage Libraries...".
2. Search for "PololuHD44780".
3. Click the PololuHD44780 entry in the list.
4. Click "Install".

If this does not work, you can manually install the library:

1. Download the
   [latest release archive from GitHub](https://github.com/pololu/pololu-hd44780-arduino/releases)
   and decompress it.
2. Rename the folder "pololu-hd44780-arduino-xxxx" to "PololuHD44780".
3. Drag the "PololuHD44780" folder into the "libraries" directory inside your
   Arduino sketchbook directory. You can view your sketchbook location by
   opening the "File" menu and selecting "Preferences" in the Arduino IDE. If
   there is not already a "libraries" folder in that location, you should make
   the folder yourself.
4. After installing the library, restart the Arduino IDE.

## Creating a PololuHD44780 object

Unless you are using a library that creates an LCD object for you in some way, you will want to create a PololuHD44780 object that can be used to access your LCD.  To do so, add these lines near the top of your sketch:

~~~{.cpp}
#include <PololuHD44780.h>
PololuHD44780 lcd(7, 6, 5, 4, 3, 2);
~~~

The numbers listed above are the pin numbers for the pins that are controlling the LCD.  The pins are specified in this order: RS, E, DB4, DB5, DB6, DB7.

## Basic usage

To clear your LCD's screen, use `clear`:

~~~{.cpp}
lcd.clear();
~~~

To print strings and numbers to it, you can use `print`:

~~~{.cpp}
lcd.print("hi");
lcd.print(13);
lcd.print(14, HEX);
~~~

The `print` function is provided by Arduino's `Print` class, so it behaves just like the `print` function on the `Serial` object and can print a lot of different things.  See the [Arduino print() documentation](https://arduino.cc/en/Serial/Print) for more information, or look at the overloads of print that are defined in [Print.h in the Arduino IDE source code](https://github.com/arduino/Arduino/blob/master/hardware/arduino/cores/arduino/Print.h).

To control where data is printed on the LCD, you can use `gotoXY()`.  The following code positions the cursor on the first column (column 0) of the second line (line 1):

~~~{.cpp}
lcd.gotoXY(0, 1)
~~~

If you call these functions too often in a tight loop, your LCD might flicker and be hard to read.  Here is some code that waits at least 100 milliseconds between writes to the LCD:

~~~{.cpp}
void loop()
{
    static uint16_t lastDisplayTime;
    if ((uint16_t)(millis() - lastDisplayTime) >= 100)
    {
      lcd.clear();
      lcd.print("hi");
      lastDisplayTime = millis();
    }
}
~~~

## Documentation

For complete documentation of this library, including many features that were not covered here, see the [PololuHD44780.h file documentation](https://pololu.github.io/pololu-hd44780-arduino/_pololu_h_d44780_8h.html) from https://pololu.github.io/pololu-hd44780-arduino.

## Version history

* 2.0.0 (2016-08-17): Updated libray to work with the Arduino Library Manager.
* 1.0.0 (2014-12-11): Original release.
