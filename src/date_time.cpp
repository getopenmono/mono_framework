// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#include "date_time.h"
#include "regex.h"
#include "application_context_interface.h"
#include <stdio.h>

using namespace mono;

const uint8_t DateTime::DaysPerMonth[13] =
{
    0,
    31,
    28,
    31,
    30,
    31,
    30,
    31, //july
    31, //aug
    30,
    31,
    30,
    31 //dec
};

// MARK: Public Constructors

DateTime::DateTime()
{
    secs = mins = hours = day = month -1;
    year = 1970;
    
    type = UNKNOWN_TIME_ZONE;
}

DateTime::DateTime(uint16_t years, uint8_t months, uint8_t days,
                   uint8_t hours, uint8_t minutes, uint8_t seconds, TimeTypes zone)
{
    secs = trim(seconds, 0, 59);
    mins = trim(minutes, 0, 59);
    this->hours = trim(hours, 0, 23);
    day = trim(days, 1, 31);
    month = trim(months, 1, 12);
    year = years;
    leapYear = isLeapYear(years);
    type = zone;
}

DateTime::DateTime(const DateTime &other)
{
    secs = other.secs;
    mins = other.mins;
    this->hours = other.hours;
    day = other.day;
    month = other.month;
    year = other.year;
    type = other.type;
    leapYear = other.leapYear;
}

DateTime &DateTime::operator=(const DateTime &other)
{
    secs = other.secs;
    mins = other.mins;
    this->hours = other.hours;
    day = other.day;
    month = other.month;
    year = other.year;
    type = other.type;
    leapYear = other.leapYear;

    return *this;
}

// MARK: Public Methods

String DateTime::toString() const
{
    return String::Format("%04d-%02d-%02d %02d:%02d:%02d",year,month,day,hours,mins,secs);
}

String DateTime::toISO8601() const
{
    const char *timeZone;
    switch (type) {
        case UTC_TIME_ZONE:
            timeZone = "Z";
            break;
        case LOCAL_TIME_ZONE:
        case UNKNOWN_TIME_ZONE:
        default:
            timeZone = "";
            break;
    }

    return String::Format("%04d-%02d-%02dT%02d:%02d:%02d%s",year,month,day,hours,mins,secs,timeZone);
}

String DateTime::toTimeString() const
{
    return String::Format("%02d:%02d:%02d",hours,mins,secs);
}

String DateTime::toDateString() const
{
    return String::Format("%04d-%02d-%02d",year,month,day);
}

uint32_t DateTime::toJulianDayNumber() const
{
    if (type == LOCAL_TIME_ZONE) return toUtcTime().toJulianDayNumber();

    // this formula is from https://en.wikipedia.org/wiki/Julian_day
    uint32_t a = (14 - month) / 12;
    uint32_t y = year + 4800 - a;
    uint32_t m = month + 12 * a - 3;
    uint32_t jdn = day + (153 * m + 2) / 5
        + 365 * y + (y / 4) - (y / 100) + (y / 400) - 32045;

    return jdn;
}

uint32_t DateTime::toUnixTime() const
{
    if (type == LOCAL_TIME_ZONE) return toUtcTime().toUnixTime();

    uint32_t jdn = toJulianDayNumber();
    uint32_t hms = hours * 3600 + mins * 60 + secs;
    uint32_t unix_time = (jdn - 2440588) * 86400 + hms;

    return unix_time;
}

bool DateTime::isValid() const
{
    return month > 0;
}

DateTime DateTime::toUtcTime() const
{
    if (type == UTC_TIME_ZONE)
        return *this;
    else if (type == LOCAL_TIME_ZONE)
        return DateTime(year, month, day, hours+LocalTimeZoneHourOffset, mins, secs, UTC_TIME_ZONE);
    else
        return *this;

}

// MARK: Date / Time manipulation

DateTime DateTime::addSeconds(int seconds) const
{
    DateTime other(*this);
    if (seconds > 0)
    {
        for (int s=0; s<seconds; s++) {
            other.incrementSecond();
        }
    }
    else if (seconds < 0) {
        for (int s=0; s< -1*seconds; s++) {
            other.decrementSecond();
        }
    }

    return other;
}

DateTime DateTime::addMinutes(int minutes) const
{
    DateTime other(*this);
    if (minutes > 0)
    {
        for (int m=0; m<minutes; m++) {
            other.incrementMinute();
        }
    }
    else if (minutes < 0) {
        for (int m=0; m< -1*minutes; m++) {
            other.decrementMinute();
        }
    }

    return other;
}

DateTime DateTime::addHours(int hrs) const
{
    DateTime other(*this);
    if (hrs > 0)
    {
        for (int h=0; h<hrs; h++) {
            other.incrementHour();
        }
    }
    else if (hrs < 0) {
        for (int h=0; h< -1*hrs; h++) {
            other.decrementHour();
        }
    }

    return other;
}

DateTime DateTime::addDays(int days) const
{
    DateTime other(*this);
    if (days > 0)
    {
        for (int d=0; d<days; d++) {
            other.incrementDayOfMonth();
        }
    }
    else if (days < 0) {
        for (int d=0; d< -1*days; d++) {
            other.decrementDayOfMonth();
        }
    }

    return other;
}

//DateTime DateTime::addMonths(int months) const
//{
//    DateTime other(*this);
//    if (months > 0)
//    {
//        for (int M=0; M<months; M++) {
//            other.incrementMonth();
//        }
//    }
//
//    return other;
//}

//DateTime DateTime::addYears(int years) const
//{
//    DateTime other(*this);
//    if (years > 0)
//    {
//        for (int y=0; y<years; y++) {
//            other.incrementYear();
//        }
//    }
//
//    return other;
//}

// MARK: Operator overloads

bool DateTime::operator==(const DateTime &other) const
{
    DateTime utcSelf = this->toUtcTime();
    DateTime utcOther = other.toUtcTime();

    return  utcSelf.secs == utcOther.secs &&
            utcSelf.mins == utcOther.mins &&
            utcSelf.hours == utcOther.hours &&
            utcSelf.day == utcOther.day &&
            utcSelf.month == utcOther.month &&
            utcSelf.year == utcOther.year &&
            utcSelf.leapYear == utcOther.leapYear;
}

bool DateTime::operator!=(const mono::DateTime &other) const
{
    return !(this->operator==(other));
}

bool DateTime::operator>(const mono::DateTime &other) const
{
    DateTime utcSelf = this->toUtcTime();
    DateTime utcOther = other.toUtcTime();

    if (utcSelf.year > utcOther.year)
        return true;
    else if (utcSelf.year < utcOther.year)
        return false;
    
    if (utcSelf.month > utcOther.month)
        return true;
    else if (utcSelf.month < utcOther.month)
        return false;
    
    if (utcSelf.day > utcOther.day)
        return true;
    else if (utcSelf.day < utcOther.day)
        return false;
    
    if (utcSelf.hours > utcOther.hours)
        return true;
    else if (utcSelf.hours < utcOther.hours)
        return false;
    
    if (utcSelf.mins > utcOther.mins)
        return true;
    else if (utcSelf.mins < utcOther.mins)
        return false;
    
    if (utcSelf.secs > utcOther.secs)
        return true;
    else
        return false; // secs must en less than other
}

bool DateTime::operator<(const mono::DateTime &other) const
{
    DateTime utcSelf = this->toUtcTime();
    DateTime utcOther = other.toUtcTime();

    if (utcSelf.year < utcOther.year)
        return true;
    else if (utcSelf.year > utcOther.year)
        return false;
    
    if (utcSelf.month < utcOther.month)
        return true;
    else if (utcSelf.month > utcOther.month)
        return false;
    
    if (utcSelf.day < utcOther.day)
        return true;
    else if (utcSelf.day > utcOther.day)
        return false;
    
    if (utcSelf.hours < utcOther.hours)
        return true;
    else if (utcSelf.hours > utcOther.hours)
        return false;
    
    if (utcSelf.mins < utcOther.mins)
        return true;
    else if (utcSelf.mins > utcOther.mins)
        return false;
    
    if (utcSelf.secs < utcOther.secs)
        return true;
    else
        return false; // secs must en less than other
}

bool DateTime::operator>=(const mono::DateTime &other) const
{
    if (this->operator==(other) || this->operator>(other))
        return true;
    else
        return false;
}

bool DateTime::operator<=(const mono::DateTime &other) const
{
    if (this->operator==(other) || this->operator<(other))
        return true;
    else
        return false;
}

// MARK: Static Public Methods

bool DateTime::isLeapYear(uint16_t year)
{
    return year % 400 == 0 || (year % 4 == 0  && year % 100 != 0);
}

DateTime DateTime::fromISO8601(mono::String date)
{
    Regex isoReg("(\\d\\d\\d\\d)-(\\d\\d)-(\\d\\d)[T ](\\d\\d):(\\d\\d):(\\d\\d)(Z)?");
    Regex::Capture cap[8];

    if (!isoReg.Match(date, cap, 8))
        return DateTime();

    TimeTypes zone = UNKNOWN_TIME_ZONE;

    int year, month, day, hour, minute, second;
    sscanf(isoReg.Value(cap[0])(), "%d", &year);
    sscanf(isoReg.Value(cap[1])(), "%d", &month);
    sscanf(isoReg.Value(cap[2])(), "%d", &day);

    sscanf(isoReg.Value(cap[3])(), "%d", &hour);
    sscanf(isoReg.Value(cap[4])(), "%d", &minute);
    sscanf(isoReg.Value(cap[5])(), "%d", &second);

    if (isoReg.Value(cap[6]) == String("Z"))
        zone = UTC_TIME_ZONE;
//    else if (isoReg.Value(cap[8]).Length() > 0)
//    {
//        int offset = 0;
//        sscanf(isoReg.Value(cap[8])(), "%d", &offset);
//        DateTime utcTime(year, month, day, hour, minute, second, UTC_TIME_ZONE);
//        return utcTime.addHours(-offset);
//    }

    return DateTime(year, month, day, hour, minute, second, zone);
}

// MARK: Protected internal methods

void DateTime::incrementSecond()
{
    secs++;
    if (secs > 59)
    {
        secs = 0;
        incrementMinute();
    }
}

void DateTime::incrementMinute()
{
    mins++;
    if (mins > 59)
    {
        mins = 0;
        incrementHour();
    }
}

void DateTime::incrementHour()
{
    hours++;
    if (hours > 23)
    {
        hours = 0;
        incrementDayOfMonth();
    }
}

void DateTime::incrementDayOfMonth()
{
    day++;
    if (leapYear && month == 2)
    {
        if (day > 29)
        {
            day = 1;
            incrementMonth();
        }
    }
    else if (day > DaysPerMonth[month])
    {
        day = 1;
        incrementMonth();
    }
}

void DateTime::incrementMonth()
{
    month++;
    if (month > 12)
    {
        month = 1;
        incrementYear();
    }
}

void DateTime::incrementYear()
{
    year++;
    leapYear = isLeapYear(year);
}

void DateTime::decrementSecond()
{
    if (secs == 0) {
        secs = 59;
        decrementMinute();
    }
    else
        secs--;
        
}

void DateTime::decrementMinute()
{
    if (mins == 0)
    {
        mins = 59;
        decrementHour();
    }
    else
        mins--;
}

void DateTime::decrementHour()
{
    if (hours == 0) {
        hours = 23;
        decrementDayOfMonth();
    }
    else
        hours--;
}

void DateTime::decrementDayOfMonth()
{
    if (day == 1) {
        decrementMonth();
        if (leapYear && month == 2)
            day = 29;
        else
            day = DaysPerMonth[month];
    }
    else
        day--;
}

void DateTime::decrementMonth()
{
    if (month == 1) {
        month = 12;
        decrementYear();
    }
    else
        month--;
}

void DateTime::decrementYear()
{
    year--;
    leapYear = isLeapYear(year);
}

uint8_t DateTime::trim(uint8_t value, uint8_t min, uint8_t max)
{
    if (value < min)
        return min;

    if (value > max)
        return max;

    return value;
}

/// MARK: STATIC SYSTEM DATETIME

int DateTime::LocalTimeZoneHourOffset = 0;
DateTime DateTime::systemDateTimeClock = DateTime(1970, 1, 1, 0, 0, 0, DateTime::UTC_TIME_ZONE);

void DateTime::incrementSystemClock()
{
    systemDateTimeClock.incrementSecond();
}

DateTime DateTime::now()
{
    return systemDateTimeClock;
}

void DateTime::setSystemDateTime(mono::DateTime dt)
{
    if (IApplicationContext::Instance->RTC)
        IApplicationContext::Instance->RTC->stopRtc();

    systemDateTimeClock = dt;

    if (IApplicationContext::Instance->RTC)
        IApplicationContext::Instance->RTC->startRtc();
}
