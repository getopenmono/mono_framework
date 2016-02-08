// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "act8600_power_system.h"
#include <mono.h>
#include <mbed_error.h>
#include <project.h>

using namespace mono::power;

ACT8600PowerSystem::ACT8600PowerSystem() : i2c(NC, NC)
{
}

uint8_t ACT8600PowerSystem::SystemStatus()
{
    int8_t data = 0x00;
    if (!readRegister(SYS, &data))
        error("Could not get SystemStatus, I2C failed\n\r");
    
    return data;
}

ACT8600PowerSystem::ChargeState ACT8600PowerSystem::ChargeStatus()
{
    int8_t data = 0x00;
    if (!readRegister(APCH_4, &data))
        error("Failed to read APCH 4 register, I2C failed\n\r");
    
    return (ChargeState) ((data >> 4) & 0x03);
}

bool ACT8600PowerSystem::PowerFencePeriperals()
{
    int8_t data = 0x00;
    if (!readRegister(REG1_EXT, &data))
    {
        debug("could not read from power chip");
        return false;
    }
    
    debug("REG1: 0x%x\n\r",data);
    return data & ENABLE ? false : true;
}


void ACT8600PowerSystem::powerOffUnused()
{
    bool success = true;
    success &= writeRegister(REG2_EXT, (uint8_t)(0x00 | (~ENABLE)));
    int8_t reg = 0;
    readRegister(REG2_EXT, &reg);
    debug("reg2: 0x%x\n\r",reg);
    
    success &= writeRegister(REG3_EXT, (uint8_t)(0x00 | (~ENABLE)));
    readRegister(REG3_EXT, &reg);
    debug("reg3: 0x%x\n\r",reg);
    
    success &= writeRegister(REG4_EXT, (uint8_t)(0x00 | (~ENABLE)));
    readRegister(REG4_EXT, &reg);
    debug("reg4: 0x%x\n\r",reg);
    
    success &= writeRegister(REG5_EXT, (uint8_t)(0x00 | (~ENABLE)));
    readRegister(REG5_EXT, &reg);
    debug("reg5: 0x%x\n\r",reg);
    
    success &= writeRegister(REG6_EXT, (uint8_t)(0x00 | (~ENABLE)));
    readRegister(REG6_EXT, &reg);
    debug("reg6: 0x%x\n\r",reg);
    
    success &= writeRegister(REG7_EXT, (uint8_t)(0x00 | (~ENABLE)));
    readRegister(REG7_EXT, &reg);
    debug("reg7: 0x%x\n\r",reg);
    
    success &= writeRegister(REG8_EXT, (uint8_t)(0x00 | (~ENABLE)));
    readRegister(REG8_EXT, &reg);
    debug("reg8: 0x%x\n\r",reg);
    
    readRegister(REG910, &reg);
    debug("reg9: 0x%x\n\r",reg);
    
    readRegister(REG1_EXT, &reg);
    debug("reg1: 0x%x\n\r",reg);
    
    if (!success)
    {
        debug("Failed to set unused power regs!\n\r");
    }
}

void ACT8600PowerSystem::setPowerFencePeripherals(bool off)
{
    bool success;
    if (off)
        success = writeRegister(REG1_EXT, 0x00 & (~ENABLE));
    else
        success = writeRegister(REG1_EXT, 0x00 | ENABLE);
    
    if (!success)
    {
        debug("failed to set power fence!\n\r");
    }
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
    
    // TODO: Avoid this delay by impl digital logic with specific truth table
    // The I2C demux drives the line low, when its not selected. this means the
    // pull up resistor needs time to pull up the line, when the demux selects it
    CyDelayUs(10);
    
    int status = i2c.write(ACT8600I2CAddress, (const char*)&regAddr, 1);
    if (status != 0)
    {
        act8600_i2c_en_Write(0);
        debug("failed to read from act register (addr write)");
        return false;
    }
    
    
    status = i2c.read(ACT8600I2CAddress, (char*)regData, 1);
    if (status != 0)
    {
        act8600_i2c_en_Write(0);
        debug("failed to read value act register");
        return false;
    }
    
    act8600_i2c_en_Write(0);
    return true;
}

bool ACT8600PowerSystem::writeRegister(int8_t regAddr, int8_t regData)
{
    act8600_i2c_en_Write(1);
    CyDelayUs(10);
    int8_t data[2] = {regAddr, regData};
    bool success = i2c.write(ACT8600I2CAddress, (const char*)data, 2) == 0 ? true: false;
    act8600_i2c_en_Write(0);
    
    return success;
}