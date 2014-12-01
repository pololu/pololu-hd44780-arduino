#pragma once
#include <Arduino.h>

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

    /*! Sends data or commands to the LCD.
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

    /*! Sends an 8-bit command to the LCD. */
    void sendCommand(uint8_t cmd)
    {
        send(cmd, false, false);
    }

    /*! Sends a 4-bit command to the LCD. */
    void sendCommand4Bit(uint8_t cmd)
    {
        send(cmd, false, true);
    }

    /*! Sends 8 bits of a data to the LCD. */
    void sendData(uint8_t data)
    {
        send(data, true, false);
    }

    /*! Clear the contents of the LCD and reset the cursor position to the upper
     *  left. */
    void clear();

    /*! Defines a custom character.
     * @param picture A pointer to the character dot pattern, in program space.
     * @param number A number between 0 and 7. */
    void loadCustomCharacter(const uint8_t * picture, uint8_t number);

    /*! This overload of loadCustomCharacter is only provided for compatibility
     * with OrangutanLCD; a lot of Orangutan code defines an array of chars for
     * custom character pictures. */
    void loadCustomCharacter(const char * picture, uint8_t number)
    {
        loadCustomCharacter((const uint8_t *)picture, number);
    }

    /*! Change the location of the cursor.  The cursor (whether visible or invisible),
     *  is the place where the next character will be displayed when.
     * @param x The number of the column to go to, with 0 being the leftmost column.
     * @param y The number of the row to go to, with 0 being the top row. */
    void gotoXY(uint8_t x, uint8_t y);

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

    void moveCursor(uint8_t direction, uint8_t num);
    void scroll(uint8_t direction, uint8_t num, uint16_t delay_time);
    void initPrintf();
    void initPrintf(uint8_t lcdWidth, uint8_t lcdHeight);

    // Useful methods from Arduino's LiquidCrystal:
    void home();

    void scrollDisplayLeft();
    void scrollDisplayRight();
    void leftToRight();
    void rightToLeft();
    void autoscroll();
    void noAutoscroll();

    void createChar(uint8_t, uint8_t[]);
    void setCursor(uint8_t, uint8_t);

    // For compatibility with Arduino's LiquidCrystal.
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
     * "Display on/off control" HD44780 command that we last sent.
     * bit 2: D: Whether the display is on.
     * bit 1: C: Whether the cursor is shown.
     * bit 0: B: Whether the cursor is blinking. */
    uint8_t displayControl;

    void setDisplayControl(uint8_t displayControl);

    void init2();


};
