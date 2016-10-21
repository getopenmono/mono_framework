//
//  digital_out_test.cpp
//  tests
//
//  Created by Kristoffer Lyder Andersen on 30/10/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#include "catch.hpp"

#include <DigitalOut.h>

SCENARIO("mbed class DigitalOut perform correctly","[digital_out]")
{
    GIVEN("A DigitalOut on a pin")
    {
        mbed::DigitalOut output(TFT_RESET);
        
        WHEN("Just initialized")
        {
            THEN("pin mode should be strong")
            {
                REQUIRE(CyPins_GetPinDriveMode(TFT_RESET) == CY_PINS_DM_STRONG);
            }
            
            THEN("initial value must be 0 (low): .read()")
            {
                REQUIRE(output.read() == 0);
            }
            
            THEN("initial value must be 0: operator=")
            {
                uint8_t val = output;
                REQUIRE(val == 0);
            }
        }
        
        WHEN("value is set high: .write()")
        {
            output.write(1);
            
            THEN("value should be high: operator=")
            {
                uint8_t val = output;
                REQUIRE(val == 1);
            }
            
        }
        
        WHEN("value is set high: opretator=")
        {
            output = 1;
            
            THEN("value must be high .read()")
            {
                REQUIRE(output.read() == 1);
            }
        }
    }
}
