//
//  i2c_tests.cpp
//  tests
//
//  Created by Kristoffer Lyder Andersen on 31/10/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#include "catch.hpp"
#include <I2C.h>

SCENARIO("mbed I2C master read/write must work","[i2c]")
{
    uint8_t sendData[6] = { 0x15, 0xA7, 0x11, 0x90, 0xD4, 0x0F };
    
    GIVEN("an i2c object at default pins")
    {
        mbed::I2C i2c(HARD_WIRED, HARD_WIRED);
        char slaveAddr = 0x62;
        
        WHEN("sending 1 byte to echo slave at 0x62")
        {
            int nack = i2c.write(slaveAddr, (char*)sendData, 1);
            
            THEN("bus should respond with ACK")
            {
                REQUIRE(nack == 0);
            }
            
            WHEN("we read the echo from 0x62")
            {
                uint8_t resp[1];
                int nack = i2c.read(slaveAddr, (char*)resp, 1);
                
                THEN("bus should respond ACK")
                {
                    REQUIRE(nack == 0);
                }
                
                THEN("response must be echo")
                {
                    REQUIRE(resp[0] == sendData[0]);
                }
            }
        }
        
        
        WHEN("sending 3 bytes in one go")
        {
            int nack = i2c.write(slaveAddr, (char*)sendData, 3);
            
            THEN("bus must ACK")
            {
                REQUIRE(nack == 0);
            }
            
            WHEN("reading the echoed response")
            {
                uint8_t resp[3];
                int nack = i2c.read(slaveAddr, (char*)resp, 3);
                
                THEN("bus must ACK")
                {
                    REQUIRE(nack == 0);
                }
                
                THEN("response should match send data")
                {
                    uint8_t a = resp[0];
                    uint8_t b = resp[1];
                    uint8_t c = resp[2];
                    REQUIRE(a == sendData[0]);
                    REQUIRE(b == sendData[1]);
                    REQUIRE(c == sendData[2]);
                }
            }
        }
        
        WHEN("send and read 2 + 2 + 2 bytes")
        {
            int nack = i2c.write(slaveAddr, (char*)sendData, 2);
            
            THEN("bus must ACK")
            {
                REQUIRE(nack == 0);
            }
            
            WHEN("reading response")
            {
                uint8_t resp[2];
                int nack = i2c.read(slaveAddr, (char*)resp, 2);
                
                THEN("bus must ACK")
                {
                    REQUIRE(nack == 0);
                }
                
                THEN("response must match send data")
                {
                    REQUIRE(resp[0] == sendData[0]);
                    REQUIRE(resp[1] == sendData[1]);
                }
                
                WHEN("sending next 2 bytes")
                {
                    nack = i2c.write(slaveAddr, (char*)sendData+2, 2);
                    
                    THEN("bus must ACK")
                    {
                        REQUIRE(nack == 0);
                    }
                    
                    WHEN("read response for next 2 bytes")
                    {
                        uint8_t resp[2];
                        nack = i2c.read(slaveAddr, (char*)resp, 2);
                        
                        THEN("bus must ACK")
                        {
                            REQUIRE(nack == 0);
                        }
                        
                        THEN("response must match")
                        {
                            REQUIRE(resp[0] == sendData[2]);
                            REQUIRE(resp[1] == sendData[3]);
                        }
                        
                        WHEN("sending last 2 bytes")
                        {
                            nack = i2c.write(slaveAddr, (char*)sendData+4, 2);
                            
                            THEN("bus must ACK")
                            {
                                REQUIRE(nack == 0);
                            }
                            
                            WHEN("reading last 2 bytes")
                            {
                                uint8_t resp[2];
                                nack = i2c.read(slaveAddr, (char*)resp, 2);
                                
                                THEN("bus must ACK")
                                {
                                    REQUIRE(nack == 0);
                                }
                                
                                THEN("last response must match")
                                {
                                    REQUIRE(resp[0] == sendData[4]);
                                    REQUIRE(resp[1] == sendData[5]);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    GIVEN("an slave addresss that does not exist")
    {
        mbed::I2C i2c(HARD_WIRED, HARD_WIRED);
        char slaveAddr = 0x00;
        
        WHEN("writing to non existing slave")
        {
            uint8_t nack = i2c.write(slaveAddr, (char*)sendData, 2);
            
            THEN("bus should not ACK")
            {
                REQUIRE(nack != 0);
            }
        }
    }
}
