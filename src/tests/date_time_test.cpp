//
//  main.cpp
//  dtTest
//
//  Created by Kristoffer Lyder Andersen on 14/11/2016.
//  Copyright © 2016 Kristoffer Lyder Andersen. All rights reserved.
//

#define SLRE_DEBUG 1
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../date_time.h"

using namespace mono;

SCENARIO("DateTime and calendar works")
{
    GIVEN("A specific date")
    {
        DateTime today(2016, 11, 14);
        //printf("Today %s\n", today.toString()());

        WHEN("equal comparison must work")
        {
            DateTime other(2016, 11, 14);
            REQUIRE(today == other);
            REQUIRE(today >= other);
            REQUIRE(today <= other);
        }

        WHEN("not equal must work")
        {
            DateTime other(2016, 10, 31);
            //printf("other: %s\n",other.toUtcTime().toISO8601()());
            REQUIRE(today != other);
            REQUIRE(today > other);
            REQUIRE(today >= other);
            REQUIRE(other < today);
            REQUIRE(other <= today);
        }

        WHEN("1 day is added")
        {
            DateTime tomorrow = today.addDays(1);
            //printf("tomorrow: %s\n", tomorrow.toString()());

            REQUIRE(today < tomorrow);
        }

        REQUIRE(DateTime::isLeapYear(1970) == false);
        REQUIRE(DateTime::isLeapYear(1972) == true);

        DateTime epoch(1970,1,1,0,0,0,DateTime::UTC_TIME_ZONE);
        
        WHEN("Calendar epoch is compared to libc time_t epoch")
        {
            time_t e = 0;
            DateTime unixEpoch(e, false);
            
            THEN("Unix time_t to DateTime convertion must work")
            {
                //printf("unix: %s == org: %s\n", unixEpoch.toString()(), epoch.toString()());
                REQUIRE(unixEpoch == epoch);
            }
        }
        
        WHEN("timezone is changed from UTC+01:00 to UTC")
        {
            tzset();
            int tmz = -timezone/(60*60) + daylight;
            //printf("System timezone: %s, tz: %li, tmz: %i\n", tzname[daylight], timezone, tmz);
            DateTime::LocalTimeZoneHourOffset = tmz;
            time_t t;
            struct tm bt;
            time(&t);
            DateTime now(t);
            //printf("local now: %s\n", now.toString()());
            
            t += timezone + daylight*60*60;
            gmtime_r(&t, &bt);
            DateTime utcNow(&bt, false);
            
            
            THEN("change from local to UTC must work")
            {
                //printf("utcConv: %s == utcNow: %s\n", now.toUtcTime().toString()(), utcNow.toString()());
                REQUIRE(now.toUtcTime() == utcNow);
            }
        }
        
        WHEN("Converting back to unix time")
        {
            time_t now;
            time(&now);
            DateTime dt(now);
            time_t now2 = dt.toUnixTime();
            
            THEN("The org unix time must mast converted from DateTime")
            {
                REQUIRE(now == now2);
            }
        }
        
        WHEN("Using addDays function, and added to EPOCH")
        {
            THEN("31 days added must match unix time")
            {
                DateTime later = epoch.addDays(31);
                DateTime ul = DateTime(31*24*60*60, false);
                REQUIRE(later == ul);
            }
            
            THEN("61 days added must match unix time func")
            {
                DateTime later = epoch.addDays(61);
                DateTime ul = DateTime(61*24*60*60, false);
                REQUIRE(later == ul);
            }

            THEN("365 days added must match unix time func")
            {
                DateTime later = epoch.addDays(365);
                DateTime ul = DateTime(365*24*60*60, false);
                REQUIRE(later == ul);
            }
            
            THEN("730 days added must match unix time func")
            {
                DateTime later = epoch.addDays(365*2);
                DateTime ul = DateTime(2*365*24*60*60, false);
                REQUIRE(later == ul);
            }

            THEN("1095 days added must match unix time func")
            {
                DateTime later = epoch.addDays(365*3);
                DateTime ul = DateTime(3*365*24*60*60, false);
                REQUIRE(later == ul);
            }
        }

        WHEN("using the addTime function")
        {
            THEN("adding 123456 should reveal the same as libc method")
            {
                time_t now;
                time(&now);
                time_t interval = 123456;
                time_t dest = now + interval;
                DateTime dtNow(now);
                DateTime dtDest(dest);
                DateTime dtConv = dtNow.addTime(&interval);
                //printf("dst: %s == conv: %s\n", dtDest.toString()(), dtConv.toString()());
                REQUIRE(dtDest == dtConv);
            }
        }
        
        WHEN("we parse a ISO date")
        {
            String dateStr("2016-11-14T23:56:12Z");
            String dateStr2("2016-11-14T23:56:12+01:00");
            String dateStr3("2016-11-14 23:56:12.000+01:30");

            DateTime d1 = DateTime::fromISO8601(dateStr);
            //printf("D1: %s\n", d1.toString()());
            DateTime d2 = DateTime::fromISO8601(dateStr2);
            //printf("D2: %s\n", d2.toString()());
            DateTime d3 = DateTime::fromISO8601(dateStr3);
            //printf("D3: %s\n", d3.toString()());
        }

        WHEN("we format datetime with libc")
        {
            time_t now;
            struct tm comps;
            time(&now);
            localtime_r(&now, &comps);
            DateTime dtNow(now);
            char buffer[100];

            strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", &comps);
            String other = dtNow.toString("%a, %d %b %Y %H:%M:%S GMT");

            printf("libc: %s == dt: %s\n", buffer, other());
            REQUIRE(strcmp(buffer, other()) == 0);
        }

        WHEN("we get a RFC 1123 formatted date and time")
        {
            time_t now;
            struct tm comps;
            time(&now);
            time_t utcNow = now + timezone - (daylight != 0 ? 60*60 : 0);
            localtime_r(&utcNow, &comps);
            DateTime dtNow(now, false);

            char buffer[80];
            strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", &comps);
            String rfcTime = dtNow.toRFC1123();
            printf("libc: %s == dt: %s\n", buffer, rfcTime());
            REQUIRE(strcmp(buffer, rfcTime()) == 0);
        }
    }
}
