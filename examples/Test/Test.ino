/** Test program for the PololuHD44780 library which tests all of
 * the functions it defines.  This program is mainly meant as a
 * test suite for library developers who want to make sure that
 * their changes did not break any previously-working features of
 * the library. */

#include <PololuHD44780.h>

PololuHD44780 lcd(7, 6, 5, 4, 3, 2);

void loadCustomCharacters()
{
    //static const uint8_t smiley[] PROGMEM = {
    static const char smiley[] PROGMEM = {
        0b01010,
        0b01010,
        0b01010,
        0b01010,
        0b01010,
        0b00000,
        0b10001,
        0b01110,
    };
    lcd.loadCustomCharacter(smiley, 0);

    // Make sure that old code that uses 'char' for the pictures
    // still works because the OrangutanLCD function took char*
    // instead of uint8_t*.
    static const char back_arrow[] PROGMEM = {
	0b00000,
	0b00010,
	0b00001,
	0b00101,
	0b01001,
	0b11110,
	0b01000,
	0b00100,
    };
    lcd.loadCustomCharacter(back_arrow, 7);

    // Make sure createChar and loadCustomCharacterFromRam work.
    byte diamond[8] = {
        B00000,
        B00100,
        B01010,
        B10001,
        B10001,
        B01010,
        B00100,
    };
    lcd.createChar(1, diamond);
}

void setup()
{
}

// Prints the ID of the screen we are currently showing and waits
// for the user to send input on the serial monitor before
// continuing.
void wait(uint16_t id)
{
  SERIAL_PORT_MONITOR.println(id);
  SERIAL_PORT_MONITOR.flush();
  while(SERIAL_PORT_MONITOR.read() == -1);
}

void loop()
{
  lcd.reinitialize();
  loadCustomCharacters();

  // Test both overloads of write
  // Expected screen: "Hello   "
  //                  "        "
  lcd.clear();
  lcd.write('H');
  lcd.write("ello");
  wait(0);

  // Test custom characters
  // Expected screen:  "0:[smile] 7:[arrow] "
  //                   "1:[diamond]        "
  lcd.clear();
  lcd.write("0:\x00 7:\x07", 7);
  lcd.gotoXY(0, 1);
  lcd.write("1:\x01");
  wait(10);

  // Test print and gotoXY
  // Expected screen: "Hi there"
  //                  "12345678"
  lcd.clear();
  lcd.print("Hi");
  lcd.gotoXY(3, 0);
  lcd.print(F("there")); // print from program space
  lcd.gotoXY(0, 1);
  lcd.print(123456789);
  lcd.print("abcdef");
  wait(20);

  // Test noDisplay
  // Expected screen: empty
  lcd.noDisplay();
  wait(30);

  // Test display()
  // Expected screen: "Hi there"
  //                  "12345678"
  lcd.display();
  wait(40);

  // Test cursor().  The space should have a solid cursor.
  lcd.cursor();
  lcd.gotoXY(2, 0);
  wait(45);

  // Test blink().  There should be a solid cursor AND a blinking one.
  lcd.blink();
  wait(55);

  // Test noCursor().  There should just be a blinking cursor.
  lcd.noCursor();
  wait(50);

  // Test noBlink().  No cursor.
  lcd.noBlink();
  wait(60);

  // Test cursorSolid().  The cursor should be solid.
  lcd.cursorSolid();
  wait(70);

  // Test hideCursor().  The cursor should be gone.
  lcd.hideCursor();
  wait(80);

  // Test cursorBlinking().  The cursor should be blinking.
  lcd.cursorBlinking();
  wait(90);

  // Test scrollDisplayLeft.
  // Expected screen: "i there "
  //                  "23456789"
  // (with a blinking cursor on the second column, top row)
  lcd.scrollDisplayLeft();
  wait(100);

  // Test scrollDisplayLeft.
  // Expected screen: " there  "
  //                  "3456789a"
  // (with a blinking cursor on the first column, top row)
  lcd.scrollDisplayLeft();
  wait(110);

  // Test scrollDisplayRight.
  // Expected screen: "i there "
  //                  "23456789"
  // (with a blinking cursor on the second column, top row)
  lcd.scrollDisplayRight();
  wait(120);

  // Test home.
  // Expected screen: "Hi there"
  //                  "12345678"
  // (with a blinking cursor on the H)
  lcd.home();
  wait(130);

  // Test clear().  The screen should be empty except for a
  // blinking cursor in the upper left.
  lcd.clear();
  wait(140);

  // Test leftToRight and rightToLeft
  // Expected screen: "abcdef  "
  //                  "        "
  // (with a blinking cursor)
  lcd.print("abXX");
  lcd.rightToLeft();
  lcd.print("Xdc");
  lcd.leftToRight();
  lcd.gotoXY(4, 0);
  lcd.print("ef");
  wait(150);

  // Test autoscrolling
  // Expected screen: "efghijkl"
  //                  "        "
  lcd.autoscroll();
  lcd.print("ghij");
  lcd.noAutoscroll();
  lcd.print("klmn");
  wait(160);

  // Test that the screen can really hold 40 characters on each
  // line, and that it can display the last 4 columns with the
  // first 4 columns.
  // Expected screen: "!@#$abcd"
  //                  "%^&*ABCD"
  // (with a blinking cursor over the a)
  lcd.clear();
  lcd.print("abcdefghijklmnopqrstuvwxyz0123456789!@#$");
  lcd.gotoXY(0, 1);
  lcd.print("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789%^&*");
  lcd.scrollDisplayRight();
  lcd.scrollDisplayRight();
  lcd.scrollDisplayRight();
  lcd.scrollDisplayRight();
  wait(170);
}
