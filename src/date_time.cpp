// This software is part of OpenMono, see http://developer.openmono.com
// Released under the MIT license, see LICENSE.txt

#include "date_time.h"

using namespace mono;

// MARK: Public Constructors

DateTime::DateTime()
{
    secs = mins = hours = day = month -1;
    year = 1970;
}

DateTime::DateTime(uint16_t years, uint8_t months, uint8_t days,
                   uint8_t hours, uint8_t minutes, uint8_t seconds)
{
    secs = trim(seconds, 0, 59);
    mins = trim(minutes, 0, 59);
    this->hours = trim(hours, 0, 23);
    day = trim(days, 1, 31);
    month = trim(months, 1, 12);
}

// MARK: Public Methods

String DateTime::ToString() const
{
    return String::Format("%u-%i-%i %i:%i:%i",year,month,day,hours,mins,secs);
}


// MARK: Protected internal methods

uint8_t DateTime::trim(uint8_t value, uint8_t min, uint8_t max)
{
    if (value < min)
        return min;

    if (value > max)
        return max;

    return value;
}