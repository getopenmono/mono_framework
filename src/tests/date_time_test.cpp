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
        printf("Today %s\n", today.toString()());

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
            printf("other: %s\n",other.toUtcTime().toISO8601()());
            REQUIRE(today != other);
            REQUIRE(today > other);
            REQUIRE(today >= other);
            REQUIRE(other < today);
            REQUIRE(other <= today);
        }

        WHEN("1 day is added")
        {
            DateTime tomorrow = today.addDays(1);
            printf("tomorrow: %s\n", tomorrow.toString()());

            REQUIRE(today < tomorrow);
        }

        REQUIRE(DateTime::isLeapYear(1970) == false);
        REQUIRE(DateTime::isLeapYear(1972) == true);

        WHEN("1000 days is added to EPOCH")
        {
            DateTime epoch(1970,1,1,0,0,0,DateTime::UTC_TIME_ZONE);
            printf("Epoch: %s\n",epoch.toString()());

            DateTime later = epoch.addDays(31);
            printf("31 days later: %s\n", later.toString()());

            later = epoch.addDays(61);
            printf("61 days later: %s\n", later.toString()());

            later = epoch.addDays(365);
            printf("365 days later: %s\n", later.toString()());

            later = epoch.addDays(365*2);
            printf("730 days later: %s\n", later.toString()());

            later = epoch.addDays(365*3);
            printf("1095 days later: %s\n", later.toString()());
        }

        WHEN("we parse a ISO date")
        {
            String dateStr("2016-11-14T23:56:12Z");
            String dateStr2("2016-11-14T23:56:12+01:00");
            String dateStr3("2016-11-14 23:56:12.000+01:30");

            DateTime d1 = DateTime::fromISO8601(dateStr);
            printf("D1: %s\n", d1.toString()());
            DateTime d2 = DateTime::fromISO8601(dateStr2);
            printf("D2: %s\n", d2.toString()());
            DateTime d3 = DateTime::fromISO8601(dateStr3);
            printf("D3: %s\n", d3.toString()());
        }
    }
}
