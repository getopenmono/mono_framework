// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef date_time_h
#define date_time_h

#include <stdint.h>
#include "mn_string.h"

namespace mono {

    /**
     * @brief A DateTime representation
     * 
     *
     */
    class DateTime
    {
    protected:
        // MARK: Protected Members

        int8_t secs, mins, hours, day, month;
        uint16_t year;
    public:
        // MARK: Public Contructors

        /**
         * @brief Construct an empty / invalid DateTime object
         * 
         */
        DateTime();

        /**
         * @brief Construct a DateTime object with a given date and time
         * 
         * @param years The Year component of the date, for example 2016
         * @param months The month component of the date from 1 to 12, May is 5
         * @param days The day component of the date, 1-indexed, from 1 to 31
         * @param hours Optional: The hour component of the timestamp, range is 0 to 23
         * @param minutes The minute component of the timestamp, range is 0 to 59
         * @param seconds The seconds component of the timestamp, range is 0 to 59
         */
        DateTime(uint16_t years, uint8_t months, uint8_t days,
                 uint8_t hours = 0, uint8_t minutes = 0, uint8_t seconds = 0);


        // MARK: Public Methods

        /**
         * @brief Return the DateTime object as a huamn readable string
         * @return A mono string on the format: yyyy-MM-dd hh:mm:ss
         */
        String ToString() const;


    protected:
        // MARK: Protected internal methods

        inline uint8_t trim(uint8_t value, uint8_t min, uint8_t max);
    };

}

#endif /* date_time_h */
