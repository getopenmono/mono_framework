
#ifndef running_average_filter_h
#define running_average_filter_h

#include <string.h>

namespace mono { namespace io {

    /**
     * @brief A basic running average low pass filter
     *
     * This is a standard moving window / moving average low pass filter, to
     * remove noise from analog input signals. You append raw samples to the 
     * filter, and read the filtered output vy @ref value method.
     *
     * Filter sample values are akways unsigned 16-bit integers, since this is
     * the return type of 
     * [`mbed::AnalogIn`](https://developer.mbed.org/handbook/AnalogIn)
     *
     * ## Moving Average Low pass filtering
     *
     * This digital filter type is quite common, and works well in most cases.
     * You control the filter amount by varying the length og the filter. Longer
     * filters does more aggressive low pass filtering. A filter with the length
     * of 1, does no filtering at all.
     *
     * The length of a filter object is determined by C++ templating and are 
     * fixes for the life-time of the filter.
     *
     * ## Appending the first sample
     *
     * If you create a filter with the length of 100 samples, then if filter will
     * need an initialization period of 100 samples before its output is correctly
     * filtered. To account for this issue you can add the first sample using
     * the @ref clear method, providing it the value of the first value.
     *
     * This will in most cases help you to no see filter values rise slowly 
     * until it stabilizes after 100 samples.
     *
     * ## Complexity
     *
     * Filter operations are normally O(1), that is constant time. You can append
     * new samples and read filter output in O(1). However, clearing the filter
     * or calculating the variance of the samples is O(n).
     *
     * To get even faster output calculations use a filter length that is a
     * multi of 2, like 8, 16, 32, 64 etc. This reduce the CPU's integer 
     * division to simple shift operations.
     *
     * ## Example
     *
     * @code
     * med::AnalogIn adc(J_RING1);
     * RunningAverageFilter<128> filter(adc.read_u16());
     *
     * filter.append(adc.read_u16());
     * uint16_t lpValue = filter.value();
     * @endcode
     *
     * @see FilteredAnalogIn
     */
    template <uint16_t Length>
    class RunningAverageFilter
    {
    protected:
        uint16_t filterData[Length];
        uint16_t index;
        uint32_t filterSum;

    public:

        /**
         * @brief Construct a filter with an optional initial value
         *
         * @param initialValue Provide an initial filter value, for quick stabilization
         */
        RunningAverageFilter(uint16_t initialValue = 0)
        {
            memset(filterData, initialValue, Length*sizeof(uint16_t));
            filterSum = 0;
            index = 0;
        }

        /**
         * @brief Add a sample to the filter
         */
        uint16_t append(uint16_t val)
        {
            filterSum -= filterData[index];
            filterData[index] =  val;
            filterSum += filterData[index];

            if (++index >= Length)
            {
                index = 0;
            }

            return filterSum / Length;
        }

        /**
         * @brief Reset the filter setting all samples to a new value
         *
         * Use this method to reset the filter, perhaps after a period of 
         * inactivity in the sampling of your signal source.
         */
        void clear(uint16_t initialValue = 0)
        {
            filterSum = 0;
            for (int i=0; i<Length; i++) {
                filterData[i] = initialValue;
                filterSum += initialValue;
            }
        }

        /**
         * @brief Gets the current low pass filtered value
         */
        uint16_t value() const
        {
            return filterSum / Length;
        }

        /**
         * @brief Get the sum of all samples in the filter
         */
        uint16_t sum() const
        {
            return filterSum;
        }

        /**
         * @brief Get the variance of the filter samples
         */
        uint16_t variance() const
        {
            int sum = 0;
            int mean = value();
            for (int i =0; i<Length; i++)
            {
                sum += (filterData[i] - mean)*(filterData[i] - mean);
            }

            return sum / Length;
        }

        /**
         * @brief Get a specific sample from the filters array of samples
         */
        uint16_t operator[](uint16_t indx) const
        {
            return filterData[indx];
        }

        /**
         * @brief Get the length of the filter
         */
        uint16_t length() const
        {
            return Length;
        }
    };
} }

#endif /* running_average_filter_h */
