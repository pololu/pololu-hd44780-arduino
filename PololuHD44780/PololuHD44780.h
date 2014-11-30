#pragma once
#include <Arduino.h>

class PololuHD44780Base : public Print
{
private:
    bool initialized;

    void init2();

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


    /*! Sends data to the LCD.
     *
     * This function, along with initPins, comprise the hardware abstraction
     * layer for the LCD, and must be defined in a subclass of
     * PololuHD44780Base.  All other functions use these two functions to
     * communicate with the LCD.
     */
    // data: the data to send to the LCD
    // rsValue: true to drive rs high, false to drive rs low
    // only4bits: if true, and the LCD is using a 4-bit interface, only sends
    //   the lower 4 bits of the data.
    virtual void send(uint8_t data, bool rsValue, bool only4bits) = 0;

    void sendCommand(uint8_t cmd)
    {
        send(cmd, false, false);
    }

    void sendCommand4Bit(uint8_t cmd)
    {
        send(cmd, false, true);
    }

    void sendData(uint8_t data)
    {
        send(data, true, false);
    }

    // Clear the contents of the LCD.
    void clear();

    // Defines a custom character.
    // picture should be a pointer to the character dot pattern, in program space.
    // number should be a number between 0 and 7.
    void loadCustomCharacter(const uint8_t * picture, uint8_t number);

    // For compatibility with OrangutanLCD; a lot of Orangutan code
    // defines an array of chars for custom character pictures.
    void loadCustomCharacter(const char * picture, uint8_t number)
    {
        loadCustomCharacter((const uint8_t *)picture, number);
    }

    // Useful methods from OrangutanLCD:
    void gotoXY(uint8_t x, uint8_t y);
    void showCursor(uint8_t cursorType);
    void hideCursor();
    void moveCursor(uint8_t direction, uint8_t num);
    void scroll(uint8_t direction, uint8_t num, uint16_t delay_time);
    void initPrintf();
    void initPrintf(uint8_t lcdWidth, uint8_t lcdHeight);

    // Useful methods from Arduino's LiquidCrystal:
    void home();
    void noDisplay();
    void display();
    void noBlink();
    void blink();
    void noCursor();
    void cursor();
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

};
