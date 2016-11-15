// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef date_time_h
#define date_time_h

#include <stdint.h>
#include "mn_string.h"

namespace mono {

    /**
     * @brief A Date and time representation in the Gregorian calendar
     * 
     * This class represents a point in time, defined in the gregorian calendar.
     * Such a timestamp are given in year, month, day of month, hours since
     * midnight, minutes and seconds. This class also defined if the timestamp
     * is in UTC / GMT or a defined local time zone.
     *
     * The class handles leap years and the varying length of months. You can
     * add seconds, minutes, hours and days to a DateTime object and get the
     * result as a new DateTime object.
     *
     * When you create DateTime objects they are created in the local time zone
     * by default. The local time zone is defined as a offset in hours relative
     * to UTC. There is no notion of IANA Time Zone names of alike - just an
     * offset to the UTC time.
     *
     * There are two convenient method to print DateTime as readable strings.
     * The @ref toString method print a human readable (MySql compatible)
     * timestamp. The other @ref toISO8601 returns a string formatted in the
     * ISO 8601 standard format used in JSON objects.
     *
     * When printing DateTime objects, they are returned in the time zone that
     * they are created in.
     *
     * ### System Wall Clock
     *
     * This class also has a global DateTime object reserved for use by a RTC
     * feature. A subsystem manages the RTC and increments the global system
     * DateTime object.
     *
     * You can get the current DateTime time by using the static method @ref now
     * To set the system clock use the static method @ref setSystemClock
     */
    class DateTime
    {
    public:

        /** DateTime timestamps can be one of three types */
        enum TimeTypes {
            LOCAL_TIME_ZONE,    /**< The DateTime is specified in local time zone */
            UTC_TIME_ZONE,      /**< The DateTime is specified in UTC / GMT time zone */
            UNKNOWN_TIME_ZONE   /**< The DateTime do not have a specified time zone */
        };

        static const uint8_t DaysPerMonth[13];
        
    protected:
        // MARK: Protected Members

        int8_t secs, mins, hours, day, month;
        uint16_t year;
        bool leapYear;
        TimeTypes type;

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
         * @param zone The timezone where this DateTime define its time, default is the local timezone
         */
        DateTime(uint16_t years, uint8_t months, uint8_t days,
                 uint8_t hours = 0, uint8_t minutes = 0, uint8_t seconds = 0,
                 TimeTypes zone = LOCAL_TIME_ZONE);

        DateTime(const DateTime &other);

        DateTime &operator=(const DateTime &other);


        // MARK: Public Methods

        /**
         * @brief Return the DateTime object as a huamn readable string
         * @return A mono string on the format: yyyy-MM-dd hh:mm:ss
         */
        String toString() const;

        /**
         * @brief Return an ISO8601 formatted timestamp as a string
         */
        String toISO8601() const;


        bool isValid() const;

        /** @brief Convert this DateTime to UTC time */
        DateTime toUtcTime() const;

        // MARK: Operator Overloads

        bool operator==(const DateTime &other) const;
        bool operator!=(const mono::DateTime &other) const;

        bool operator>=(const mono::DateTime &other) const;
        bool operator>(const mono::DateTime &other) const;
        bool operator<(const mono::DateTime &other) const;
        bool operator<=(const mono::DateTime &other) const;

        // MARK: Date / Time manipulation
        
        DateTime addSeconds(int seconds) const;
        DateTime addMinutes(int minutes) const;
        DateTime addHours(int hours) const;
        DateTime addDays(int days) const;
//        DateTime addMonths(int months) const;
//        DateTime addYears(int years) const;

        // MARK: Static Methods

        /** Get the maximum possible DateTime value (far in the future) */
        static DateTime maxValue() { return DateTime(9999, 12, 31, 23, 59, 59); }

        /** Get the lowest possible DateTime value (the distant past) */
        static DateTime minValue() { return DateTime(0, 1, 1, 00, 0, 0); }

        /** @brief Check is a year is a leap year */
        static bool isLeapYear(uint16_t year);

        /**
         * @brief Parse a subset of ISO 8601 compatible date time representations
         *
         * This static method takes a ISO 8601 formatted string, and creates a
         * DateTime object from that. This method only parses a subset of the
         * possible date representations allowed in ISO 8601. Specifically
         * it can handle dates in these format:
         *
         * * `yyyy-MM-ddTHH:mm:ssZ`
         * * `yyyy-MM-ddTHH:mm:ss+01:00` or other time zones
         * * `yyyy-MM-dd HH:mm:ssZ`
         * * `yyyy-MM-dd HH:mm:ss`
         *
         * @return The parsed DateTime object, that might be valid or invalid
         */
        static DateTime fromISO8601(String date);

    protected:
        // MARK: Protected internal methods

        void incrementSecond();
        void incrementMinute();
        void incrementHour();
        void incrementDayOfMonth();
        void incrementMonth();
        void incrementYear();

        inline uint8_t trim(uint8_t value, uint8_t min, uint8_t max);

        static DateTime systemDateTimeClock;
        static void incrementSystemClock();

    public:

        static int LocalTimeZoneHourOffset;

        static void setSystemDateTime(DateTime dt);
        static DateTime now();
    };

}

#endif /* date_time_h */
