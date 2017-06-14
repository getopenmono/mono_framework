// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#ifndef date_time_h
#define date_time_h

#include <stdint.h>
#include <time.h>
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
         * @brief Create a DateTime from the a libc simple time
         *
         * This constructor takes a standard lic time variable and create a 
         * calendar DateTime object from that. It uses the systems timezone,
         * and libc systems time stamp to calendar conversion.
         *
         * You can set the optional argument `localTimeZone` to `false` to force
         * the *DateTime* to be created in UTC time, instead of system time.
         *
         * Times is converted to a date in the Gregorian calendar.
         *
         * @param t The libc simple time stamp
         * @param localTimeZone Optional: Use the systems timezone in convertion
         */
        DateTime(const time_t t, bool localTimeZone = true);
        
        /**
         * @brief Create a DateTime from the a libc broken-down time struct
         *
         * This constructor takes a standard libc broken-downtime variable 
         * and creates a calendar DateTime object from that. It uses the 
         * timezone present in libc structure.
         *
         * You can set the optional argument `localTimeZone` to `false` to force
         * the *DateTime* to be created in UTC time, instead of system time.
         *
         * @param brokentime The libc simple broken-down calendar time
         * @param localTimeZone Optional: Use the systems timezone in convertion
         */
        DateTime(const tm *brokentime, bool localTimeZone = true);

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
         * @brief Get the DateTime as an RFC1123 compatible date/time string
         * 
         * This is the standard used by the HTTP standard and therefore used
         * in HTTP headers like *Date* and *Last-Modified*. Use this method to
         * format a DateTime to this representation.
         */
        String toRFC1123() const;

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
         * @brief Return a libc broken-down time/date compnents
         * 
         * This uses the default timezone of the local systems time zone
         */
        struct tm toBrokenDownUnixTime() const;
        
        /**
         * @brief Returns the libc style timestamp (simple time)
         *
         */
        time_t toLibcUnixTime() const;

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
        
        /** @brief Get the hour component, from 0 to 23 */
        uint8_t Hours() const;
        
        /** @brief Get the Minute component, from 0 to 59 */
        uint8_t Minutes() const;
        
        /** @brief Get the Seconds component, from 0 to 59 */
        uint8_t Seconds() const;
        
        /** @brief Get the day of month component, first day of month is 1. */
        uint8_t Days() const;
        
        /** @brief Get the month component, January is 1, December 12 */
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
        
        /**
         * @brief Return a new object with the Unix simple time component added
         *
         * The provided pointer to a unix time is added to the existing DateTime
         * and the result is returned as a new object.
         *
         * @param t The unix simple time to add
         * @return The new DateTime object with the time interval added
         */
        DateTime addTime(const time_t *t) const;

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

#ifndef TEST_CASE
        
        /**
         * @brief Set a new system DateTime
         */
        static void setSystemDateTime(DateTime dt);

#endif
        
        /**
         * @brief Get the current DateTime from the system RTC clock
         */
        static DateTime now();

        /**
         * @brief Internal method used by the RTC system to increment the system
         * DateTime. You should not call this manually.
         */
        static void incrementSystemClock();

        // MARK: Time Format Method

        /**
         * @brief Format the DateTime to a string, using *libc* `strftime` function
         *
         * Use this method to get a custom representation of the date/time as
         * text. You control the format of the output using the string at format.
         * It can contain two kinds of specifications: text to be copied
         * literally into the formatted string, and time conversion specifications.
         * Time conversion specifications are two- and three-character sequences
         * beginning with ‘%’ (use ‘%%’ to include a percent sign in the output).
         * Each defined conversion specification selects only the specified
         * field(s) of calendar time data from *timp, and converts it to a
         * string in one of the following ways:
         *
         * %a
         * The abbreviated weekday name according to the current locale. [tm_wday]
         *
         * %A
         * The full weekday name according to the current locale. In the default 
         * "C" locale, one of ‘Sunday’, ‘Monday’, ‘Tuesday’, ‘Wednesday’, 
         * ‘Thursday’, ‘Friday’, ‘Saturday’. [tm_wday]
         *
         * %b
         * The abbreviated month name according to the current locale. [tm_mon]
         *
         * %B
         * The full month name according to the current locale. In the default 
         * "C" locale, one of ‘January’, ‘February’, ‘March’, ‘April’, ‘May’, 
         * ‘June’, ‘July’, ‘August’, ‘September’, ‘October’, ‘November’, 
         * ‘December’. [tm_mon]
         *
         * %c
         * The preferred date and time representation for the current locale. 
         * [tm_sec, tm_min, tm_hour, tm_mday, tm_mon, tm_year, tm_wday]
         *
         * %C
         * The century, that is, the year divided by 100 then truncated. For 
         * 4-digit years, the result is zero-padded and exactly two characters; 
         * but for other years, there may a negative sign or more digits. In 
         * this way, ‘%C%y’ is equivalent to ‘%Y’. [tm_year]
         *
         * %d
         * The day of the month, formatted with two digits (from ‘01’ to ‘31’). 
         * [tm_mday]
         *
         * %D
         * A string representing the date, in the form ‘"%m/%d/%y"’. [tm_mday, 
         * tm_mon, tm_year]
         *
         * %e
         * The day of the month, formatted with leading space if single digit 
         * (from ‘1’ to ‘31’). [tm_mday]
         *
         * %Ex
         * In some locales, the E modifier selects alternative representations 
         * of certain modifiers x. In newlib, it is ignored, and treated as %x.
         *
         * %F
         * A string representing the ISO 8601:2000 date format, in the form 
         * ‘"%Y-%m-%d"’. [tm_mday, tm_mon, tm_year]
         *
         * %g
         * The last two digits of the week-based year, see specifier %G (from 
         * ‘00’ to ‘99’). [tm_year, tm_wday, tm_yday]
         *
         * %G
         * The week-based year. In the ISO 8601:2000 calendar, week 1 of the 
         * year includes January 4th, and begin on Mondays. Therefore, if 
         * January 1st, 2nd, or 3rd falls on a Sunday, that day and earlier 
         * belong to the last week of the previous year; and if December 29th, 
         * 30th, or 31st falls on Monday, that day and later belong to week 1 of 
         * the next year. For consistency with %Y, it always has at least four 
         * characters. Example: "%G" for Saturday 2nd January 1999 gives "1998",
         * and for Tuesday 30th December 1997 gives "1998". [tm_year, tm_wday, 
         * tm_yday]
         *
         * %h
         * Synonym for "%b". [tm_mon]
         *
         * %H
         * The hour (on a 24-hour clock), formatted with two digits 
         * (from ‘00’ to ‘23’). [tm_hour]
         *
         * %I
         * The hour (on a 12-hour clock), formatted with two digits 
         * (from ‘01’ to ‘12’). [tm_hour]
         *
         * %j
         * The count of days in the year, formatted with three digits 
         * (from ‘001’ to ‘366’). [tm_yday]
         *
         * %k
         * The hour (on a 24-hour clock), formatted with leading space if single 
         * digit (from ‘0’ to ‘23’). Non-POSIX extension (c.p. %I). [tm_hour]
         *
         * %l
         * The hour (on a 12-hour clock), formatted with leading space if single 
         * digit (from ‘1’ to ‘12’). Non-POSIX extension (c.p. %H). [tm_hour]
         *
         * %m
         * The month number, formatted with two digits (from ‘01’ to ‘12’).
         * [tm_mon]
         *
         * %M
         * The minute, formatted with two digits (from ‘00’ to ‘59’). [tm_min]
         *
         * %n
         * A newline character (‘\n’).
         *
         * %Ox
         * In some locales, the O modifier selects alternative digit characters
         * for certain modifiers x. In newlib, it is ignored, and treated as %x.
         *
         * %p
         * Either ‘AM’ or ‘PM’ as appropriate, or the corresponding strings for 
         * the current locale. [tm_hour]
         *
         * %P
         * Same as ’%p’, but in lowercase. This is a GNU extension. [tm_hour]
         *
         * %r
         * Replaced by the time in a.m. and p.m. notation. In the "C" locale 
         * this is equivalent to "%I:%M:%S %p". In locales which don’t define 
         * a.m./p.m. notations, the result is an empty string. [tm_sec, tm_min, 
         * tm_hour]
         *
         * %R
         * The 24-hour time, to the minute. Equivalent to "%H:%M". [tm_min, 
         * tm_hour]
         *
         * %S
         * The second, formatted with two digits (from ‘00’ to ‘60’). The value 
         * 60 accounts for the occasional leap second. [tm_sec]
         *
         * %t
         * A tab character (‘\t’).
         *
         * %T
         * The 24-hour time, to the second. Equivalent to "%H:%M:%S". 
         * [tm_sec, tm_min, tm_hour]
         *
         * %u
         * The weekday as a number, 1-based from Monday (from ‘1’ to ‘7’). 
         * [tm_wday]
         *
         * %U
         * The week number, where weeks start on Sunday, week 1 contains the first
         * Sunday in a year, and earlier days are in week 0. Formatted with two 
         * digits (from ‘00’ to ‘53’). See also %W. [tm_wday, tm_yday]
         *
         * %V
         * The week number, where weeks start on Monday, week 1 contains January
         * 4th, and earlier days are in the previous year. Formatted with two 
         * digits (from ‘01’ to ‘53’). See also %G. [tm_year, tm_wday, tm_yday]
         *
         * %w
         * The weekday as a number, 0-based from Sunday (from ‘0’ to ‘6’). 
         * [tm_wday]
         *
         * %W
         * The week number, where weeks start on Monday, week 1 contains the 
         * first Monday in a year, and earlier days are in week 0. Formatted 
         * with two digits (from ‘00’ to ‘53’). [tm_wday, tm_yday]
         *
         * %x
         * Replaced by the preferred date representation in the current locale. 
         * In the "C" locale this is equivalent to "%m/%d/%y". 
         * [tm_mon, tm_mday, tm_year]
         *
         * %X
         * Replaced by the preferred time representation in the current locale. 
         * In the "C" locale this is equivalent to "%H:%M:%S". 
         * [tm_sec, tm_min, tm_hour]
         *
         * %y
         * The last two digits of the year (from ‘00’ to ‘99’). [tm_year]
         * (Implementation interpretation: always positive, even for negative 
         * years.)
         *
         * %Y
         * The full year, equivalent to %C%y. It will always have at least four
         * characters, but may have more. The year is accurate even when tm_year
         * added to the offset of 1900 overflows an int. [tm_year]
         *
         * %z
         * The offset from UTC. The format consists of a sign (negative is west
         * of Greewich), two characters for hour, then two characters for 
         * minutes (-hhmm or +hhmm). If tm_isdst is negative, the offset is 
         * unknown and no output is generated; if it is zero, the offset is the 
         * standard offset for the current time zone; and if it is positive, the 
         * offset is the daylight savings offset for the current timezone. The
         * offset is determined from the TZ environment variable, as if by 
         * calling tzset(). [tm_isdst]
         *
         * %Z
         * The time zone name. If tm_isdst is negative, no output is generated. 
         * Otherwise, the time zone name is based on the TZ environment variable,
         * as if by calling tzset(). [tm_isdst]
         *
         * %%
         * A single character, ‘%’.
         *
         */
        String toString(const char *format) const;

    };

}

#endif /* date_time_h */
