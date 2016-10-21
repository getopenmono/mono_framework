//
//  string_test.cpp
//  mnStringTest
//
//  Created by Kristoffer Andersen on 13/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#include "catch.hpp"
#include "../mn_string.h"

using mono::String;

SCENARIO("test the mono string class works")
{
    GIVEN("a string with const chars")
    {
        const char *constChar = "hej, jeg er konstant!";
        mono::String conStr(constChar);
        
        THEN("it should be malloced")
        {
            REQUIRE(conStr.malloced == true);
        }
        
        WHEN("we create a copy of the string")
        {
            mono::String copy = conStr;
            
            THEN("the copy should be a ref to the const char")
            {
                REQUIRE(copy.stringData == conStr.stringData);
            }
        }
    }
    
    GIVEN("a dynamic string")
    {
        mono::String dynStr = mono::String::Format("Hej jeg er en %s streng","dynamisk");
        
        THEN("it should be malloced")
        {
            REQUIRE(dynStr.malloced == true);
        }
        
        WHEN("we make a new copy")
        {
            mono::String copy = dynStr;
            
            THEN("it shoul dref the original")
            {
                REQUIRE(dynStr.stringData == copy.stringData);
            }
        }
    }

    GIVEN("two qeual strings (content equal)")
    {
        String str1("dette er en tekststreng!");
        String str2("dette er en tekststreng!");
        const char *str3 = "dette er en tekststreng!";

        THEN("The the two mono string equal operator should return true")
        {
            REQUIRE(str1 == str2);
            REQUIRE(str2 == str1);
        }

        THEN("The mono string equal operator with const char shuold be true")
        {
            REQUIRE(str1 == str3);
        }

        THEN("The string diff operator should return false")
        {
            REQUIRE((str1 != str2) == false);
            REQUIRE((str2 != str1) == false);
        }

        THEN("The mono string diff. operator with const char shuold be false")
        {
            REQUIRE((str1 != str3) == false);
        }
    }

    GIVEN("two different strings (content different)")
    {
        String str1("dette er en tekststreng!");
        String str2("dette er en anden tekststreng, som er forskellig!");
        const char *str3 = "dette er en helt tredje tekststreng!";

        THEN("The the two mono string equal operator should return false")
        {
            REQUIRE((str1 == str2) == false);
            REQUIRE((str2 == str1) == false);
        }

        THEN("the mono string equal operator with const char shuold be false")
        {
            REQUIRE((str1 == str3) == false);
        }

        THEN("The string diff operator should return true")
        {
            REQUIRE((str1 != str2));
            REQUIRE((str2 != str1));
        }

        THEN("The mono string diff. operator with const char shuold be true")
        {
            REQUIRE((str1 != str3));
        }
    }
}