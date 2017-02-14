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
         *
         * This returned string is on the format:
         * yyyy-MM-ddTHH:mm:ss+tt:00 if not UTC or yyyy-MM-ddTHH:mm:ssZ
         */
        String toISO8601() const;

        /**
         * @brief Return only a time string from the DateTime
         *
         * The format is: HH:mm:ss
         */
        String toTimeString() const;

        /**
         * @brief Return only a date string from the DateTime
         *
         * The format is: yyyy-MM-dd
         */
        String toDateString() const;

        /**
         * @brief Return Julian day number of the DateTime
         */
        uint32_t toJulianDayNumber() const;

        /**
         * @brief Return Unix time of the DateTime
         */
        uint32_t toUnixTime() const;

        /**
         * @brief Return `true` if the DateTime is valid
         * 
         * Invalid date object is contructed by the default constructor
         */
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

        // MARK: Accessors
        
        /** @brief Get the hour component */
        uint8_t Hours() const;
        
        /** @brief Get the Minute component */
        uint8_t Minutes() const;
        
        /** @brief Get the Seconds component */
        uint8_t Seconds() const;
        
        /** @brief Get the day of month component */
        uint8_t Days() const;
        
        /** @brief Get the month component */
        uint8_t Month() const;
        
        /** @brief Get the year component */
        uint16_t Year() const;
        
        // MARK: Date / Time manipulation

        /**
         * @brief Return a new object with a number of seconds added
         * 
         * This method increments the timestamp for the given second interval
         * @param seconds The seconds to add
         * @return The new DateTime object with seconds added
         */
        DateTime addSeconds(int seconds) const;

        /**
         * @brief Return a new object with a number of minutes added
         *
         * This method increments the timestamp for the given minute interval
         * @param minutes The minutes to add
         * @return The new DateTime object with minutes added
         */
        DateTime addMinutes(int minutes) const;

        /**
         * @brief Return a new object with a number of hours added
         *
         * This method increments the timestamp for the given hour interval
         * @param hours The hours to add
         * @return The new DateTime object with hours added
         */
        DateTime addHours(int hours) const;

        /**
         * @brief Return a new object with a number of days added
         *
         * This method increments the timestamp for the given day interval
         * @param days The days to add
         * @return The new DateTime object with days added
         */
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
        
        void decrementSecond();
        void decrementMinute();
        void decrementHour();
        void decrementDayOfMonth();
        void decrementMonth();
        void decrementYear();

        inline uint8_t trim(uint8_t value, uint8_t min, uint8_t max);

        static DateTime systemDateTimeClock;

    public:

        /**
         * @brief The systems current TimeZone setting
         *
         * The timezone is just an hour-offset from the UTC / GMT time
         */
        static int LocalTimeZoneHourOffset;

        /**
         * @brief Set a new system DateTime
         */
        static void setSystemDateTime(DateTime dt);

        /**
         * @brief Get the current DateTime from the system RTC clock
         */
        static DateTime now();

        /**
         * @brief Internal method used by the RTC system to increment the system
         * DateTime. You should not call this manually.
         */
        static void incrementSystemClock();
    };

}

#endif /* date_time_h */
