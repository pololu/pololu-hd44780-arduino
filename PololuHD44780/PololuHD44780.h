#pragma once
#include <Arduino.h>
#include <util/delay.h>

/*! \brief General class for handling the HD44780 protocol.
 *
 * This is an abstract class that knows about the HD44780 LCD commands but
 * does not directly read or write from the actual LCD.  To make a usable class,
 * you need to define a subclass of PololuHD44780Base and implement the
 * initPins() and send() functions.  The subclass you create will inherit all
 * the functions from PololuHD44780 base which are documented here, and all of
 * the functions from the Arduino's Print class, which are listed in
 * [Print.h in the Arduino IDE source code](https://github.com/arduino/Arduino/blob/master/hardware/arduino/cores/arduino/Print.h).
 *
 * Most users of this library will not need to know about this class and should
 * just use PololuHD44780 instead.  This class is intended for advanced users
 * who need to write their own LCD hardware access functions for whatever
 * reason. */
class PololuHD44780Base : public Print
{
public:

    PololuHD44780Base();

    /*! Initializes the pins so that the send() function can be called
     * successfully.  This is the first step of initializing the LCD. */
    virtual void initPins() = 0;

    /*! Initialize the LCD if it has not already been initialized. */
    void init()
    {
        if (!initialized)
        {
            initialized = true;
            init2();
        }
    }

    /*! Reinitialize the LCD.  This performs the same initialization that is
     *  done automatically the first time any function is called that writes to
     *  the LCD.  This is useful if you want to get it back to a totally clean
     *  state. */
    void reinitialize()
    {
        initialized = true;
        init2();
    }

    /*! Sends data or commands to the LCD.
     *
     * The initPins() function will always be called before the first time this
     * function is called.  This function does not need to worry about the
     * delays necessary to make sure the previous command has finished; that is
     * taken care of by PololuHD44780Base.
     *
     * This function, along with initPins(), comprise the hardware abstraction
     * layer for the LCD, and must be defined in a subclass of
     * PololuHD44780Base.  All other functions use these two functions to
     * communicate with the LCD.
     *
     * @param data The data to send to the LCD.
     * @param rsValue True to drive the RS pin high, false to drive it low.
     * @param only4bits: If true, and the LCD is using a 4-bit interface, only sends
     *   the lower 4 bits of the data. */
    virtual void send(uint8_t data, bool rsValue, bool only4bits) = 0;

private:

    void sendAndDelay(uint8_t data, bool rsValue, bool only4bit);

    /*! Sends an 8-bit command to the LCD. */
    void sendCommand(uint8_t cmd)
    {
        sendAndDelay(cmd, false, false);
    }

    /*! Sends a 4-bit command to the LCD. */
    void sendCommand4Bit(uint8_t cmd)
    {
        sendAndDelay(cmd, false, true);
    }

    /*! Sends 8 bits of a data to the LCD. */
    void sendData(uint8_t data)
    {
        sendAndDelay(data, true, false);
    }

public:

    /*! Clear the contents of the LCDs, resets the cursor position to the upper
     *  left, and resets the scroll position. */
    void clear();

    /*! Defines a custom character.
     * @param picture A pointer to the character dot pattern, in program space.
     * @param number A number between 0 and 7. */
    void loadCustomCharacter(const uint8_t * picture, uint8_t number);

    /*! Defines a custom character from RAM.
     * @param picture A pointer toe the character dot pattern, in RAM.
     * @param number A number between 0 and 7. */
    void loadCustomCharacterFromRam(const uint8_t * picture, uint8_t number);

    /*! This overload of loadCustomCharacter is only provided for compatibility
     * with OrangutanLCD; a lot of Orangutan code defines an array of chars for
     * custom character pictures. */
    void loadCustomCharacter(const char * picture, uint8_t number)
    {
        loadCustomCharacter((const uint8_t *)picture, number);
    }

    /*! Defines a custom character.
     * This is provided for compatibility with the LiquidCrystal library. */
    void createChar(uint8_t number, uint8_t picture[])
    {
        loadCustomCharacterFromRam(picture, number);
    }

    /*! Change the location of the cursor.  The cursor (whether visible or invisible),
     *  is the place where the next character will be displayed when.
     * @param x The number of the column to go to, with 0 being the leftmost column.
     * @param y The number of the row to go to, with 0 being the top row. */
    void gotoXY(uint8_t x, uint8_t y);

    /*! Changes the location of the cursor.  This is just a wrapper around
     * gotoXY provided for compaitibility with the LiquidCrystal library. */
    void setCursor(uint8_t col, uint8_t row)
    {
        gotoXY(col, row);
    }

    /*! Change the cursor to the beginning of the specified line.
     *
     * Calling gotoLine(y) is equivalent to calling gotoXY(0, y). */
    void gotoLine(uint8_t y)
    {
        gotoXY(0, y);
    }

    /*! Turns off the display while preserving its state.
     *
     * You can turn the display on again by calling display(). */
    void noDisplay();

    /*! Turns the display on.  This should only be needed if noDisplay() was
     *  previously called. */
    void display();

    /*! Hides the cursor.  The cursor is hidden by default, so this is only
     *  needed if it has been turned on.
     *
     * This function clears the LCD's "C" configuration bit without changing the
     * other bits.
     */
    void noCursor();

    /*! Shows the cursor.
     *
     * This function sets the LCD's "C" configuration bit without changing the
     * other bits.
     *
     * The cursor will normally be solid, but it could be blinking if previous
     * commands have enabled a blinking cursor.  For this reason, it is usually
     * better to call cursorSolid() or cursorBlinking() instead.  This function
     * is only provided for compatibility with the LiquidCrystal library,
     *
     * Note that the cursor will not be shown if the display is currently off
     * (due to a call to noDisplay()), or if the cursor position is not within
     * the bounds of the screen. */
    void cursor();

    /*! Makes the cursor stop blinking.
     *
     * This functions clears the LCD's "B" configuration bit without changing
     * the other bits.
     *
     * Calling this function does not actually enable the cursor so it is
     * usually better to call cursorSolid() or cursorBlinking() instead.  This
     * function is only provided for compatibilty with the LiquidCrystal
     * library. */
    void noBlink();

    /*! Makes the cursor start blinking.
     *
     * This function sets the LCD's "B" configuration bit without changing the other
     * bits.
     *
     * Calling this function does not actually enable the cursor, so it is
     * usually better to call cursorSolid() or cursorBlinking() instead.  This
     * function is only provided for compatibilty with the LiquidCrystal
     * library. */
    void blink();

    /*! Turns on the cursor and disables its blinking.
     *
     * This sets the LCD's "C" configuration bit and clears its "B" bit.
     *
     * Note that the cursor will not be shown if the display is currently off
     * (due to a call to noDisplay()), or if the cursor position is not within
     * the bounds of the screen. */
    void cursorSolid();

    /*! Turns on the cursor and makes it blink.
     *
     * This sets the LCD's "C" and "B" configuration bits.
     *
     * Note that the cursor will not be shown if the display is currently off
     * (due to a call to noDisplay()), or if the cursor position is not within
     * the bounds of the screen. */
    void cursorBlinking();

    /*! Scrolls everything on the screen one position to the left.
     *
     * This command takes about 37 microseconds. */
    void scrollDisplayLeft();

    /*! Scrolls everything on the screen one position to the right.
     *
     * This command takes about 37 microseconds. */
    // TODO: add a better explanation of scrolling in the class reference
    void scrollDisplayRight();

    /*! Resets the screen scrolling position back to the default and moves the
     *  cursor to the upper left corner of the screen.
     *
     * This command takes about 1600 microseconds, so it would be faster to
     * instead call scrollDisplayLeft() or scrollDisplayRight() the appropriate
     * number of times and then call gotoXY(0, 0). */
    void home();

    /*! Puts the LCD into left-to-right mode: the cursor will shift to the right
     *  after any character is written.  This is the default behavior. */
    void leftToRight();

    /*! Puts the LCD into right-to-left mode: the cursor will shift to the left
     *  after any character is written. */
    void rightToLeft();

    /*! Turns on auto-scrolling. */
    // TODO: better explanation of this
    void autoscroll();

    /*! Turns off auto-scrolling.  Auto-scrolling is off by default. */
    void noAutoscroll();

    //void initPrintf();
    //void initPrintf(uint8_t lcdWidth, uint8_t lcdHeight);

    /*! Send an arbitrary command to the LCD.  This is here for compatibility
     * with the LiquidCrystal library. */
    void command(uint8_t cmd)
    {
        sendCommand(cmd);
    }

    // These are required in order to inherit from Print:
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t * buffer, size_t size);

    // This allows us to easily call overrides of write that are
    // defined in Print.
    using Print::write;

private:
    bool initialized;

    /* The lower three bits of this store the arguments to the
     * last "Display on/off control" HD44780 command that we sent.
     * bit 2: D: Whether the display is on.
     * bit 1: C: Whether the cursor is shown.
     * bit 0: B: Whether the cursor is blinking. */
    uint8_t displayControl;

    /* The lower two bits of this variable store the arguments to the
     * last "Entry mode set" HD44780 command that we sent.
     * bit 1: I/D: 0 for moving the cursor to the left after data is written,
     *        1 for moving the cursor to the right.
     * bit 0: 1 for autoscrolling. */
    uint8_t entryMode;

    void setEntryMode(uint8_t entryMode);
    void setDisplayControl(uint8_t displayControl);

    void init2();
};

/*! Main class for interfacing with the HD44780 LCDs.
 *
 * This class is suitable for controlling an HD44780 LCD assuming that the LCD's
 * RS, E, DB4, DB5, DB6, and DB7 pins are each connected to a pin on the
 * microcontroller, each of those six microcontroller pins is supported by the
 * Arduino's `pinMode` and `digitalWrite` functions, and those pins are not
 * being used for any other purpose that conflicts with the LCD.
 *
 * This class sets the E pin to be an output driving low the first time you use
 * the LCD, and it assumes that no other code will change that pin.  You cannot
 * use E for any other purposes because if the LCD sees a pulse on the E pin
 * then it might consider that to be a command or data, and the LCD state will
 * become corrupted.
 *
 * For the other pins (RS, DB4, DB5, and DB6), this library reconfigures them
 * each time they are used, so it is OK if you have other code that uses those
 * pins for other purposes.  Before writing to the LCD, you just need to disable
 * any peripherals (such as UARTs) that override the output values of those
 * pins.
 *
 * If you cannot meet these conditions, you might be able to control your LCD
 * using a custom subclass of PololuHD44780Base.  You can use this class as a
 * reference for how to do that. */
class PololuHD44780 : public PololuHD44780Base
{
public:
    PololuHD44780(uint8_t rs, uint8_t e, uint8_t db4, uint8_t db5, uint8_t db6, uint8_t db7)
    {
        this->rs = rs;
        this->e = e;
        this->db4 = db4;
        this->db5 = db5;
        this->db6 = db6;
        this->db7 = db7;
    }

    virtual void initPins()
    {
        digitalWrite(e, LOW);
        pinMode(e, OUTPUT);
    }

    virtual void send(uint8_t data, bool rsValue, bool only4bits)
    {
        digitalWrite(rs, rsValue);

        pinMode(rs, OUTPUT);
        pinMode(db4, OUTPUT);
        pinMode(db5, OUTPUT);
        pinMode(db6, OUTPUT);
        pinMode(db7, OUTPUT);

        if (!only4bits) { sendNibble(data >> 4); }
        sendNibble(data & 0x0F);
    }

private:

    void sendNibble(uint8_t data)
    {
        digitalWrite(db4, data >> 0 & 1);
        digitalWrite(db5, data >> 1 & 1);
        digitalWrite(db6, data >> 2 & 1);
        digitalWrite(db7, data >> 3 & 1);

        digitalWrite(e, HIGH);
        _delay_us(1);  // Must be at least 450 ns.
        digitalWrite(e, LOW);
        _delay_us(1);  // Must be at least 550 ns.
    }

    uint8_t rs, e, db4, db5, db6, db7;
};
