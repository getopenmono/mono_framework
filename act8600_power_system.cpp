//
//  act8600_power_system.cpp
//  mono
//
//  Created by Kristoffer Andersen on 08/09/15.
//  Copyright (c) 2015 Monolit ApS. All rights reserved.
//

#include "act8600_power_system.h"
#include <mono.h>
#include <mbed_error.h>

using namespace mono::power;

ACT8600PowerSystem::ACT8600PowerSystem() : i2c(NC, NC)
{
    
}

uint8_t ACT8600PowerSystem::SystemStatus()
{
    int8_t data;
    if (!readRegister(SYS, &data))
        error("Could not get SystemStatus, I2C failed\n\r");
    
    return data;
}

ACT8600PowerSystem::ChargeState ACT8600PowerSystem::ChargeStatus()
{
    int8_t data;
    if (!readRegister(APCH_4, &data))
        error("Failed to read APCH 4 register, I2C failed\n\r");
    
    return (ChargeState) ((data >> 4) & 0x03);
}

/// POWER SYSTEM METHODS

void ACT8600PowerSystem::onSystemPowerOnReset()
{
    //Set output voltage on OUT5 to 3.3V - to power MCU input supply the same as VSYS
    //writeRegister(REG5, VOLTAGE_SELECTION & V3_3);
}

void ACT8600PowerSystem::onSystemEnterSleep()
{
    SPI0_Sleep();
    I2C_Sleep();
    //TFT_Sleep();
    USBUART_Suspend();
}

void ACT8600PowerSystem::onSystemWakeFromSleep()
{
    USBUART_Resume();
    I2C_Wakeup();
    //TFT_Wakeup();
    SPI0_Wakeup();
}

/// READ / WRITE REGISTERS

bool ACT8600PowerSystem::readRegister(int8_t regAddr, int8_t *regData)
{
    int status = i2c.write(ACT8600I2CAddress, (const char*)&regAddr, 1);
    if (status != 0)
        return false;
    
    status = i2c.read(ACT8600I2CAddress, (char*)regData, 1);
    if (status != 0)
        return false;
    
    return true;
}

bool ACT8600PowerSystem::writeRegister(int8_t regAddr, int8_t regData)
{
    int8_t data[2] = {regAddr, regData};
    return i2c.write(ACT8600I2CAddress, (const char*)data, 2) == 0 ? true: false;
}