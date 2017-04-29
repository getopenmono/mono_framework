
#ifndef FilteredAnalogIn_h
#define FilteredAnalogIn_h

#include <mbed.h>
#include "running_average_filter.h"

namespace mono { namespace io {

    /**
     * @brief A low pass filtered analog input pin.
     *
     * This is a class built upon the standard 
     * [mbed::AnalogIn](https://developer.mbed.org/handbook/AnalogIn)
     * class, that include running average low pass filtering.
     *
     * ## Filtered analog inputs
     *
     * In almost all cases you should low pass filter an incoming analog signal,
     * to remove high frequency noise. This class is a convenient way to do
     * that. It uses the @ref RunningAverageFilter to filter all incoming samples.
     *
     * Just like the @ref RunningAverageFilter, this class is templated with the
     * length of the low pass filter.
     *
     * ## Example
     *
     * Here we create 32 sample long filter, attached to the 3.5mm jack connector
     * input pin:
     *
     * @code
     * FilteredAnalogIn<32> input(J_RING1);
     * uint16_t filteredSample = input.rea_u16();
     * @endcode
     *
     * @see RunningAverageFilter
     */
    template <uint16_t depth>
    class FilteredAnalogIn : public mbed::AnalogIn, public RunningAverageFilter<depth>
    {

    public:

        /**
         * @brief Create an AnalogIn, connected to the specified pin
         * @param pin AnalogIn pin to connect to
         */
        FilteredAnalogIn(PinName pin) : AnalogIn(pin) {
        }

        /**
         * @brief Read the input voltage, represented as a float in the range [0.0, 1.0]
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
         * @brief Read the input voltage, represented as an unsigned short in the range [0x0, 0xFFFF]
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
