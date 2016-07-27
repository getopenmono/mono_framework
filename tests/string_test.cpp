//
//  string_test.cpp
//  mnStringTest
//
//  Created by Kristoffer Andersen on 13/11/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#include "catch.hpp"
#include "../mn_string.h"

SCENARIO("test the mono string class works")
{
    GIVEN("a string with const chars")
    {
        const char *constChar = "hej, jeg er konstant!";
        mono::String conStr(constChar);
        
        THEN("it should not be malloced")
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
}