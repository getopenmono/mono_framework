// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef mn_output_out_h
#define mn_output_out_h

#include <DigitalOut.h>

namespace mono { namespace io {
    
    /**
     * @brief Mono variant of mbed::DigitalOut class, adding setMode() method
     * 
     * This class is an extension of *mbed's* `DigitalOut` class, adding an
     * additional contructor and public method.
     *
     * The additional function is the possibility to change the pin drive mode, such
     * that it does not need to be strong drive (CMOS). This class allows you to
     * set the mode to `PullUp` / `PullDown` / `OpenDrain`.
     *
     * To see how you use the `DigitalOut` class, please refer to the [mbed
     * documentation page](https://developer.mbed.org/handbook/DigitalOut)
     */
    class DigitalOut : public mbed::DigitalOut
    {
    public:
        
        
        DigitalOut(PinName pin);
        
        DigitalOut(PinName pin, int value);
        
        /**
         * Construct a output pin with a specific output mode, other than the
         * default strong drive (CMOS).
         *
         * **WARNING**: The mode is set *AFTER* the pin value. You should account
         * for this possible hazard, by setting a sound initial value.
         * 
         * @param pin The pin identifier
         * @param value the initial value of the pin.
         * @param mode the initial drive mode of the pin
         */
        DigitalOut(PinName pin, int value, PinMode mode);
        
        /**
         * @brief Set the pins drive mode
         * 
         * Use this method to enable resistive pull up/down or open drain modes.
         *
         * @param mode The pin drive mode to use
         */
        void setMode(PinMode mode);
        
#ifdef MBED_OPERATORS
        /** A shorthand for write()
         */
        DigitalOut& operator= (int value) {
            write(value);
            return *this;
        }
        
        DigitalOut& operator= (DigitalOut& rhs) {
            write(rhs.read());
            return *this;
        }
        
        /** A shorthand for read()
         */
        operator int() {
            return read();
        }
#endif
    };
    
} }



#endif /* mn_output_out.h */
