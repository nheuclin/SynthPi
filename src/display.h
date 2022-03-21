#ifndef SYNTHPI_DISPLAY
#define SYNTHPI_DISPLAY
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h> 
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "Thread.h"

namespace SYNTHPI{


class MAX7219 {

    public:
        MAX7219();

        // ~MAX7219();

        std::vector<unsigned char> digitBuffer;

        int spi_transfer(int fd, unsigned char* tx, int n);

        void command(unsigned char reg, unsigned char data);

        uint32_t speed = 500000;
        uint16_t delay = 0;
        int fd;
        uint8_t bpw   = 8;

        unsigned char decodeMode = 0x0;
        unsigned char intensity = 0x7;
        unsigned char scanLimit = 0x7;
        unsigned char shutdown = 0x1;
        unsigned char displayTest = 0x0;
        unsigned int numDigits = 8;

        //SETTERS //

       /**
         * Sets a value in the \ref digitBuffer.
         *
         * @param digit Byte address of digit.
         * @param value Byte value to assign.
         * @param redraw If true, updates all digits at the end of method call.
         */
        void setDigit(unsigned int digit, unsigned char value, bool redraw);

          /**
         * Sets the \ref decodeMode.
         * @param value Decode mode byte.
         */
        void setDecodeMode(unsigned char value);

        /**
         * Sets the \ref intensity.
         * @param value Intensity value (1-15).
         */
        void setIntensity(unsigned char value);

        /**
         * Sets the \ref scanLimit.
         * @param value Scan limit (0-7).
         */
        void setScanLimit(unsigned char value);

        /**
         * Sets the \ref shutdown mode.
         * @param value Shutdown mode (1/0).
         */
        void setShutdown(unsigned char value);

        /**
         * Sets the \ref displayTest mode.
         * @param value Display test mode (1/0).
         */
        void setDisplayTest(unsigned char value);

         // GETTERS //

        /**
         * Gets a value of a digit in the \ref digitBuffer.
         * @param digit Byte address of digit.
         * @returns Current byte value of digit.
         */
        unsigned char getDigit(unsigned char digit);

        /**
         * Gets the current \ref decodeMode.
         * @returns Current \ref decodeMode.
         */
        unsigned char getDecodeMode();

        /**
         * Gets the current \ref intensity.
         * @returns Current \ref intensity.
         */
        unsigned char getIntensity();

        /**
         * Gets the current \ref scanLimit.
         * @returns Current \ref scanLimit.
         */
        unsigned char getScanLimit();

        /**
         * Gets the current \ref shutdown mode.
         * @returns Current \ref shutdown mode.
         */
        unsigned char getShutdown();

        /**
         * Gets the current \ref displayTest mode.
         * @returns Current \ref displayTest mode.
         */
        unsigned char getDisplayTest();

        /**
         * Gets the number of digits in the display.
         * @returns Number of display digits.
         */
        unsigned int getNumDigits();
       
        // HIGH LEVEL METHODS //

        /**
            * Writes all \ref digitBuffer values to display via SPI bus.
            */
        void flush();

        /**
            * Resets all digits in the \ref digitBuffer to 0.
            * @param redraw If true, calls \ref flush to update display.
            */
        void clear(bool redraw);

};


class Display: public MAX7219 {

    private:

        /** Vector to contain corresponding hex values
         * for decimal digit representation.
         *
         * Prevents the need to toggle Code B decode
         * for every numerical display.
         */
        const std::vector<unsigned char> decHexVals = {
            0x7E, // '0'
            0x30, // '1'
            0x6D, // '2'
            0x79, // '3'
            0x33, // '4'
            0x5B, // '5'
            0x5F, // '6'
            0x70, // '7'
            0x7F, // '8'
            0x7B, // '9'
        };

        const std::vector<std::vector<unsigned char>> params = {
            {0x3E,0x1D,0x0E,0x00}, // VOL
            {0x77,0x0F,0x77,0x4E}, // BLND
            {0x77,0x0F,0x77,0x4E}, // ATAC
            {0x3D,0x4E,0x77,0x3B}, // DCAY
            {0x5B,0x1C,0x5B,0x0F}, // SUST
            {0x05,0x4F,0x0E,0x00}, // REL
            {0x1F,0x77,0x15,0x37}, // BANK
            
        };

        unsigned int currentParam = 0;

    
        /** Sets a 3 digit decimal value.
         * @param value 3 digit value to set.
         */
        void setThreeDigit(unsigned int value);

        /** Sets a 2 digit decimal value.
         * @param value 2 digit value to set.
         */
        void setTwoDigit(unsigned int value);

        /** Sets a 1 digit decimal value.
         * @param value 1 digit value to set.
         */
        void setOneDigit(unsigned int value);



    public:

        Display(); ///Constructor.


        /**
         * Sets values in \ref digitBuffer
         * to display a number up to 999.
         *
         * @param value Numerical value to display.
         * @param redraw If true, updates display.
         */
        void setVal(unsigned int value, bool redraw);

        /**
         * Sets values in \ref digitBuffer
         * to display which parameter is being changed by the MIDI keyboard.
         *
         * @param param Numerical representation of which MIDI parameter is being changed.
         * @param redraw If true, updates display.
         */
        void setParam(unsigned int param, bool redraw);

        /**
        *  Checks incoming CC values and sends them for processing to display
        * 
        *  @param address Numerical representation of which MIDI parameter is being changed.
        *  @param value Value of MIDI parameter that is being changed.
        */
        void ccToDisplay(unsigned int address, unsigned int value);

       // ~Display(); ///Destructor.


};

class DisplayThread: public Thread{

    protected:
    
        Display *display;
        bool isPLaying;

    public:

        unsigned int currentParam;

        DisplayThread(Display *display); // Constructor

        ~DisplayThread(); //Destructor

        virtual void run();

        void ccToDisplay(unsigned int address, unsigned int value);

        
        

    


};

} //close namespace

 #endif
