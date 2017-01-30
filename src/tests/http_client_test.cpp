//
//  http_client_test.cpp
//  httpClientTest
//
//  Created by Kristoffer Andersen on 29/08/2016.
//  Copyright © 2016 Monolit ApS. All rights reserved.
//

#define SLRE_DEBUG 1

#include "catch.hpp"
#include "../mn_string.h"
#include "../regex.h"

using mono::String;
using mono::Regex;

SCENARIO("Check that the HTTPClient can parse URLs")
{
    GIVEN("A new a IP addr regex")
    {
        mono::Regex regex("(http://)(\\d+\\.\\d+\\.\\d+\\.\\d+):?(\\d*)(/?[^\\s'\\\"<>]*)");

        WHEN("the an IPv4 is given (without port num.): http://10.0.41.190")
        {

            String input("http://10.0.41.190");
            Regex::Capture capture[4];
            bool match = regex.Match(input, capture, 4);

            THEN("Ip should match regex!")
            {
                REQUIRE(match == true);
            }

            THEN("The params should be correct")
            {

                REQUIRE(regex.Value(capture[0]) == "http://");
                REQUIRE(regex.Value(capture[1]) == "10.0.41.190");
                REQUIRE(regex.Value(capture[2]).Length() == 0);
                REQUIRE(regex.Value(capture[3]) == "");
            }
        }

        WHEN("we use a IP addr with path")
        {
            String input("http://10.0.41.190/hej/med/dig?options=sort&test=false&pageSize=56");
            Regex::Capture capture[4];
            bool match = regex.Match(input, capture, 4);

            THEN("Ip should match regex!")
            {
                REQUIRE(match == true);
            }

            THEN("The params should be correct")
            {
                REQUIRE(regex.Value(capture[0]) == "http://");
                REQUIRE(regex.Value(capture[1]) == "10.0.41.190");
                REQUIRE(regex.Value(capture[2]).Length() == 0);
                REQUIRE(regex.Value(capture[3]) == "/hej/med/dig?options=sort&test=false&pageSize=56");
            }
        }

        WHEN("we use a IP addr with port num")
        {
            String input("http://10.0.41.190:8080");
            Regex::Capture capture[4];
            bool match = regex.Match(input, capture, 4);

            THEN("Ip should match regex!")
            {
                REQUIRE(match == true);
            }

            THEN("The params should be correct")
            {
                REQUIRE(regex.Value(capture[0]) == "http://");
                REQUIRE(regex.Value(capture[1]) == "10.0.41.190");
                REQUIRE(regex.Value(capture[2]) == "8080");
                REQUIRE(regex.Value(capture[3]) == "");
            }
        }

        WHEN("we use a IP addr with port num and path")
        {
            String input("http://10.0.41.190:8080/hej/med/dig?options=sort&test=false&pageSize=56");
            Regex::Capture capture[4];
            bool match = regex.Match(input, capture, 4);

            THEN("Ip should match regex!")
            {
                REQUIRE(match == true);
            }

            THEN("The params should be correct")
            {
                REQUIRE(regex.Value(capture[0]) == "http://");
                REQUIRE(regex.Value(capture[1]) == "10.0.41.190");
                REQUIRE(regex.Value(capture[2]) == "8080");
                REQUIRE(regex.Value(capture[3]) == "/hej/med/dig?options=sort&test=false&pageSize=56");
            }
        }
    }

    GIVEN("A new a domain based address regex")
    {
        mono::Regex regex("(http://)([^\\s/'\\\"<>\\?:,_;\\*\\^\\!<>]+):?(\\d*)(/?[^\\s'\\\"<>]*)");

        WHEN("a domain is given (without port num.): http://www.google.com")
        {

            String input("http://www.google.com");
            Regex::Capture capture[4];
            bool match = regex.Match(input, capture, 4);

            THEN("Ip should match regex!")
            {
                REQUIRE(match == true);
            }

            THEN("The params should be correct")
            {

                REQUIRE(regex.Value(capture[0]) == "http://");
                REQUIRE(regex.Value(capture[1]) == "www.google.com");
                REQUIRE(regex.Value(capture[2]).Length() == 0);
                REQUIRE(regex.Value(capture[3]).Length() == 0);
            }
        }

        WHEN("we use a domain with path")
        {
            String input("http://www.google.com/hej/med/dig?options=sort&test=false&pageSize=56");
            Regex::Capture capture[4];
            bool match = regex.Match(input, capture, 4);

            THEN("Ip should match regex!")
            {
                REQUIRE(match == true);
            }

            THEN("The params should be correct")
            {
                REQUIRE(regex.Value(capture[0]) == "http://");
                REQUIRE(regex.Value(capture[1]) == "www.google.com");
                REQUIRE(regex.Value(capture[2]).Length() == 0);
                REQUIRE(regex.Value(capture[3]) == "/hej/med/dig?options=sort&test=false&pageSize=56");
            }
        }

        WHEN("we use a domain with port num")
        {
            String input("http://www.google.com:8080");
            Regex::Capture capture[4];
            bool match = regex.Match(input, capture, 4);

            THEN("Ip should match regex!")
            {
                REQUIRE(match == true);
            }

            THEN("The params should be correct")
            {
                REQUIRE(regex.Value(capture[0]) == "http://");
                REQUIRE(regex.Value(capture[1]) == "www.google.com");
                REQUIRE(regex.Value(capture[2]) == "8080");
                REQUIRE(regex.Value(capture[3]).Length() == 0);
            }
        }

        WHEN("we use a domain with port num and path")
        {
            String input("http://www.google.com:8080/hej/med/dig?options=sort&test=false&pageSize=56");
            Regex::Capture capture[4];
            bool match = regex.Match(input, capture, 4);

            THEN("Ip should match regex!")
            {
                REQUIRE(match == true);
            }

            THEN("The params should be correct")
            {
                REQUIRE(regex.Value(capture[0]) == "http://");
                REQUIRE(regex.Value(capture[1]) == "www.google.com");
                REQUIRE(regex.Value(capture[2]) == "8080");
                REQUIRE(regex.Value(capture[3]) == "/hej/med/dig?options=sort&test=false&pageSize=56");
            }
        }
    }
}
