
#ifndef running_average_filter_h
#define running_average_filter_h

#include <string.h>

namespace mono { namespace io {

    template <uint16_t Length>
    class RunningAverageFilter
    {
    protected:
        uint16_t filterData[Length];
        uint16_t index;
        uint32_t filterSum;

    public:

        RunningAverageFilter(uint16_t initialValue = 0)
        {
            memset(filterData, initialValue, Length*sizeof(uint16_t));
            filterSum = 0;
            index = 0;
        }

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

        void clear(uint16_t initialValue = 0)
        {
            filterSum = 0;
            for (int i=0; i<Length; i++) {
                filterData[i] = initialValue;
                filterSum += initialValue;
            }
        }

        uint16_t value() const
        {
            return filterSum / Length;
        }

        uint16_t sum() const
        {
            return filterSum;
        }

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

        uint16_t operator[](uint16_t indx)
        {
            return filterData[indx];
        }
    };
} }

#endif /* running_average_filter_h */
