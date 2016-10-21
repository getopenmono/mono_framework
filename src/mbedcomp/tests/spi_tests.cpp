//
//  spi_tests.cpp
//  tests
//
//  Created by Kristoffer Andersen on 30/10/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#include "catch.hpp"

#include <device.h>
#include <PinNames.h>
#include <SPI.h>

SCENARIO("mbed SPI class sends data correctly", "[spi]")
{
    uint8_t sendData[] = { 0x57,0x29,0x44,0x99 };
    
    GIVEN("a spi object at RP pins")
    {
        mbed::SPI device(RP_SPI_MOSI, RP_SPI_MISO, RP_SPI_CLK);
        
        
        WHEN("when just initialized format should be 8-bit")
        {
            int *val = (int*) sendData;
            uint8_t bytes = device.write(*val);
            
            THEN("only send first 8 bits")
            {
                REQUIRE( bytes == sendData[0] );
            }
        }
        
        WHEN("format is changed to 16-bit")
        {
            device.format(16);
            uint16_t *val = (uint16_t*) sendData;
            uint16_t bytes = device.write(*val);
            
            THEN("it should send only the first 2 bytes")
            {
                REQUIRE( bytes == *val );
            }
        }
        
        WHEN("format is changed to 32-bit")
        {
            device.format(32);
            uint32_t *val = (uint32_t*) sendData;
            uint32_t bytes = device.write(*val);
            
            THEN("the 4 bytes to data should be returned")
            {
                REQUIRE(bytes == *val);
            }
        }
        
        WHEN("two bytes are send in a row")
        {
            uint8_t r1 = device.write(sendData[0]);
            uint8_t r2 = device.write(sendData[2]);
            
            THEN("they should be returned in-order")
            {
                REQUIRE(r1 == sendData[0]);
                REQUIRE(r2 == sendData[2]);
            }
        }
        
        WHEN("more than 8-bit is provided in 8-bit mode")
        {
            int *val = (int*) sendData;
            int bytes = device.write(*val);
            
            THEN("only the LSB byte should be sent (depends on endian!)")
            {
                REQUIRE( bytes == *((uint8_t*)sendData) );
            }
        }
    }
    
    GIVEN("a spi object at display pins")
    {
        mbed::SPI device(TFT_SPI_MOSI, TFT_SPI_MISO, TFT_SPI_CLK);
        
        WHEN("when just initialized format should be 8-bit")
        {
            int *val = (int*) sendData;
            uint8_t bytes = device.write(*val);
            
            THEN("only send first 8 bits")
            {
                REQUIRE( bytes == sendData[0] );
            }
        }
        
        WHEN("format is changed to 16-bit")
        {
            device.format(16);
            uint16_t *val = (uint16_t*) sendData;
            uint16_t bytes = device.write(*val);
            
            THEN("it should send only the first 2 bytes")
            {
                REQUIRE( bytes == *val );
            }
        }
        
        WHEN("format is changed to 32-bit")
        {
            device.format(32);
            uint32_t *val = (uint32_t*) sendData;
            uint32_t bytes = device.write(*val);
            
            THEN("the 4 bytes to data should be returned")
            {
                REQUIRE(bytes == *val);
            }
        }
        
        WHEN("two bytes are send in a row")
        {
            uint8_t r1 = device.write(sendData[0]);
            uint8_t r2 = device.write(sendData[2]);
            
            THEN("they should be returned in-order")
            {
                REQUIRE(r1 == sendData[0]);
                REQUIRE(r2 == sendData[2]);
            }
        }
        
        WHEN("more than 8-bit is provided in 8-bit mode")
        {
            int *val = (int*) sendData;
            int bytes = device.write(*val);
            
            THEN("only the LSB byte should be sent (depends on endian!)")
            {
                REQUIRE( bytes == *((uint8_t*)sendData) );
            }
        }
        
        WHEN("sending more than one byte data types")
        {
            uint16_t pixel = 0x1234;
            uint8_t a = device.write(pixel >> 8);
            uint8_t b = device.write(pixel);
            
            device.format(16);
            uint16_t resp = device.write(pixel);
            
            THEN("byte order must match MSB first")
            {
                REQUIRE(a == resp >> 8);
                REQUIRE(b == (resp & 0xff));
            }
        }
    }
    
}
