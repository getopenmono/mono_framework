
#ifndef FilteredAnalogIn_h
#define FilteredAnalogIn_h

#include <mbed.h>
#include "running_average_filter.h"

namespace mono { namespace io {

    template <uint16_t depth>
    class FilteredAnalogIn : public mbed::AnalogIn, public RunningAverageFilter<depth>
    {

    public:

        /** Create an AnalogIn, connected to the specified pin
         *
         * @param pin AnalogIn pin to connect to
         */
        FilteredAnalogIn(PinName pin) : AnalogIn(pin) {
        }

        /**
         * Read the input voltage, represented as a float in the range [0.0, 1.0]
         *
         * The value is filtered through a running average filter
         * Complexity: O(1)
         *
         * @returns A floating-point value representing the current input voltage, measured as a percentage
         */
        float read() {
            return read_u16() * 0xFFFF;
        }

        /**
         * Read the input voltage, represented as an unsigned short in the range [0x0, 0xFFFF]
         *
         * The value is filtered through a running average filter
         * Complexity: O(1)
         *
         * @returns
         *   16-bit unsigned short representing the current input voltage, normalised to a 16-bit value
         */
        unsigned short read_u16() {
            
            return this->append(analogin_read_u16(&_adc));
        }

    };
} }

#endif /* FilteredAnalogIn_h */
