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

bool ACT8600PowerSystem::PowerFencePeriperals()
{
    int8_t data = 0;
    if (!readRegister(REG1_EXT, &data))
    {
        debug("could not read from power chip");
        return false;
    }
    
    debug("REG1: 0x%x\n\r",data);
    return data & ENABLE ? false : true;
}

void ACT8600PowerSystem::setPowerFencePeripherals(bool off)
{
    if (off)
        writeRegister(REG1_EXT, 0x00 & (~ENABLE));
    else
        writeRegister(REG1_EXT, 0x00 | ENABLE);
}

bool ACT8600PowerSystem::setUSBOTGPower(bool on)
{
    writeRegister(OTG, on?ONQ1:0x00);
    int8_t otg = 0;
    bool ok = readRegister(OTG, &otg);
    
    if (!on && ok)
        return true;
    else if (!on)
        return false;
    
    if (ok && otg & Q1OK)
    {
        return true;
    }
    else
        return false;
}

bool ACT8600PowerSystem::USBOTGPower()
{
    int8 otg = 0x00;
    readRegister(OTG, &otg);
    
    return (otg & Q1OK) ? true : false;
}

uint8_t ACT8600PowerSystem::USBOTG()
{
    uint8_t otg = 0x00;
    readRegister(OTG, (int8_t*)(&otg));
    return otg;
}

/// POWER SYSTEM METHODS

void ACT8600PowerSystem::onSystemPowerOnReset()
{
    //Set output voltage on OUT5 to 3.3V - to power MCU input supply the same as VSYS
    //writeRegister(REG5, VOLTAGE_SELECTION & V3_3);
}

void ACT8600PowerSystem::onSystemEnterSleep()
{
    
}

void ACT8600PowerSystem::onSystemWakeFromSleep()
{
    
}

/// READ / WRITE REGISTERS

bool ACT8600PowerSystem::readRegister(int8_t regAddr, int8_t *regData)
{
    
    act8600_i2c_en_Write(1);
    int status = i2c.write(ACT8600I2CAddress, (const char*)&regAddr, 1);
    if (status != 0)
    {
        act8600_i2c_en_Write(0);
        return false;
    }
    
    
    status = i2c.read(ACT8600I2CAddress, (char*)regData, 1);
    if (status != 0)
    {
        act8600_i2c_en_Write(0);
        return false;
    }
    
    act8600_i2c_en_Write(0);
    return true;
}

bool ACT8600PowerSystem::writeRegister(int8_t regAddr, int8_t regData)
{
    act8600_i2c_en_Write(1);
    int8_t data[2] = {regAddr, regData};
    bool success = i2c.write(ACT8600I2CAddress, (const char*)data, 2) == 0 ? true: false;
    act8600_i2c_en_Write(0);
    
    return success;
}