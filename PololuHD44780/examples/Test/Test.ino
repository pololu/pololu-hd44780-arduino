#include <Wire.h>
#include <FastGPIO.h>
#include <USBPause.h>  // TODO: change to UsbPause.h after that library is released
#include <PololuHD44780.h>
#include <util/delay.h>

class MyLCD : public PololuHD44780Base
{
    static const uint8_t rs = 7;
    static const uint8_t e = 8;
    static const uint8_t db4 = 14;
    static const uint8_t db5 = 17;
    static const uint8_t db6 = 13;
    static const uint8_t db7 = IO_D5;

public:

    virtual void initPins()
    {
        FastGPIO::Pin<e>::setOutputLow();
    }

    virtual void send(uint8_t data, bool rsValue, bool only4bits)
    {
        init();

        FastGPIO::Pin<rs>::setOutput(rsValue);

        USBPause usbPause;
        FastGPIO::PinLoan<db4> loanDB4;
        FastGPIO::PinLoan<db5> loanDB5;
        FastGPIO::PinLoan<db6> loanDB6;
        FastGPIO::PinLoan<db7> loanDB7;

        if (!only4bits)
        {
            sendNibble(data >> 4);
        }
        sendNibble(data & 0x0F);

        // Every data transfer or command takes at least 37 us to
        // complete, and most of them only take that long according
        // to the HD44780 datasheet.
        _delay_us(37);
    }

private:
    void sendNibble(uint8_t data)
    {
        FastGPIO::Pin<db4>::setOutput(data >> 0 & 1);
        FastGPIO::Pin<db5>::setOutput(data >> 1 & 1);
        FastGPIO::Pin<db6>::setOutput(data >> 2 & 1);
        FastGPIO::Pin<db7>::setOutput(data >> 3 & 1);

        // Send a pulse on the Enable line.  According to the "Bus
        // Timing Characteristics" section of the HD44780 datasheet,
        // 450 ns is the minimum enable pulse width (PW_EH) and the
        // minimum enable cycle time is 1000 ns.
        FastGPIO::Pin<e>::setOutputHigh();
        _delay_us(1);   // At least 450 ns.
        FastGPIO::Pin<e>::setOutputLow();
        _delay_us(1);   // At least 550 ns.
    }
};

MyLCD lcd;

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

    // Make sure that old code that uses 'char' for the pictures still works
    // because the OrangutanLCD function took char* instead of uint8_t*.
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
  loadCustomCharacters();
}

void wait()
{
    delay(1000);
}

void loop()
{
  // Test both overloads of write
  // Expected screen: "Hello   "
  //                  "        "
  lcd.clear();
  lcd.write('H');
  lcd.write("ello");
  wait();

  // Test custom characters
  // Expected screen:  "0:[smile] 7:[arrow] "
  //                   "1:[diamond]        "
  lcd.clear();
  lcd.write("0:\x00 7:\x07", 7);
  lcd.gotoLine(1);
  lcd.write("1:\x01");
  wait();

  // Test print and goto
  // Expected screen: "Hi there"
  //                  "12345678"
  lcd.clear();
  lcd.print("Hi");
  lcd.gotoXY(3, 0);
  lcd.print(F("there")); // print from program space
  lcd.gotoXY(0, 1);
  lcd.print(123456789);
  lcd.print("abcdef");
  wait();

  // Test noDisplay
  // Expected screen: empty
  lcd.noDisplay();
  wait();

  // Test display()
  // Expected screen: "Hi there"
  //                  "12345678"
  lcd.display();
  wait();

  // Test cursor().  The space should have a cursor on it now.
  lcd.cursor();
  lcd.gotoXY(0, 2);

  // Test noCursor().  The cursor should be gone.
  lcd.noCursor();
  wait();

  // Test cursorSolid().  The cursor should be solid.
  lcd.cursorSolid();
  wait();

  // Test cursorBlinking().  The cursor should be blinking.
  lcd.cursorBlinking();
  wait();

  // Test noBlink().  The cursor should be solid.
  lcd.noBlink();
  wait();

  // Test blink().  The cursor should be blinking.
  lcd.blink();
  wait();

  // Test scrollDisplayLeft.
  // Expected screen: "i there "
  //                  "23456789"
  lcd.scrollDisplayLeft();
  wait();

  // Test scrollDisplayLeft.
  // Expected screen: " there  "
  //                  "3456789a"
  lcd.scrollDisplayLeft();
  wait();

  // Test scrollDisplayRight.
  // Expected screen: "i there "
  //                  "23456789"
  lcd.scrollDisplayRight();
  wait();

  // Test home.
  // Expected screen: "Hi there"
  //                  "12345678"
  lcd.home();
  wait();

  // Test clear().  The screen should be empty.
  lcd.clear();
  wait();

  // Test leftToRight and rightToLeft
  // Expected screen: "abcdef  "
  //                  "        "
  lcd.print("abXX");
  lcd.rightToLeft();
  lcd.print("dc");
  lcd.leftToRight();
  lcd.gotoXY(4, 0);
  lcd.print("ef");
  wait();

  // Test autoscrolling
  // TODO: figure out what we expect to see here
  lcd.autoscroll();
  lcd.print("hijk");
  lcd.noAutoscroll();
  lcd.print("lmno");
  wait();
}
