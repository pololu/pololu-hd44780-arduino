/* This program can be used to test your LCD and demonstrates
basic use of the PololuHD44780 class.  The word "hi" is displayed
in the upper left corner and the letter "i" should be blinking.

If this doesn't work for you, be sure to check all of your
connections to the LCD, including GND, VCC, VO, RS, RW, E, DB4,
DB5, DB6, and DB7.  Make sure RW is tied to GND or pulled low
with a strong pull-down resistor (at most 1.5 kilOhms).  Try
adjusting the contrast of your LCD, especially if all the pixels
are black or all the pixels are clear.  Also try power cycling
the system in case the LCD is in a bad state. */

#include <PololuHD44780.h>

/* The pins specified below will need to be connected to your
LCD.  The pins are specified in this order:
RS, E, DB4, DB5, DB6, DB7. */
PololuHD44780 lcd(7, 6, 5, 4, 3, 2);

void setup()
{
}

void loop()
{
  lcd.clear();
  lcd.print('h');
  delay(500);
  lcd.print('i');
  delay(500);
}
