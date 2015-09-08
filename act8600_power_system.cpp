//
//  act8600_power_system.cpp
//  mono
//
//  Created by Kristoffer Andersen on 08/09/15.
//  Copyright (c) 2015 Monolit ApS. All rights reserved.
//

#include "act8600_power_system.h"
#include <mono.h>

using namespace mono::power;

ACT8600PowerSystem::ACT8600PowerSystem() : i2c(NC, NC)
{
    
}

/// POWER SYSTEM METHODS

void ACT8600PowerSystem::onSystemPowerOnReset()
{
    //Set output voltage on OUT5 to 3.3V - to power MCU input supply the same as VSYS
    writeRegister(REG5, VOLTAGE_SELECTION & V3_3);
    
//    *((reg8*)CYREG_PRT0_DM0) = 0;
//    *((reg8*)CYREG_PRT0_DM1) = 1;
//    *((reg8*)CYREG_PRT0_DM2) = 1;
//    *((reg8*)CYREG_PRT0_DR) = 0;
//    
//    *((reg8*)CYREG_PRT1_DM0) = 0;
//    *((reg8*)CYREG_PRT1_DM1) = 1;
//    *((reg8*)CYREG_PRT1_DM2) = 1;
//    *((reg8*)CYREG_PRT1_DR) = 0;
//    
//    *((reg8*)CYREG_PRT2_DM0) = 0;
//    *((reg8*)CYREG_PRT2_DM1) = 1;
//    *((reg8*)CYREG_PRT2_DM2) = 1;
//    *((reg8*)CYREG_PRT2_DR) = 0;
//    
//    *((reg8*)CYREG_PRT3_DM0) = 0;
//    *((reg8*)CYREG_PRT3_DM1) = 1;
//    *((reg8*)CYREG_PRT3_DM2) = 1;
//    *((reg8*)CYREG_PRT3_DR) = 0;
//    
//    *((reg8*)CYREG_PRT4_DM0) = 0;
//    *((reg8*)CYREG_PRT4_DM1) = 1;
//    *((reg8*)CYREG_PRT4_DM2) = 1;
//    *((reg8*)CYREG_PRT4_DR) = 0;
//    
//    *((reg8*)CYREG_PRT5_DM0) = 0;
//    *((reg8*)CYREG_PRT5_DM1) = 1;
//    *((reg8*)CYREG_PRT5_DM2) = 1;
//    *((reg8*)CYREG_PRT5_DR) = 0;
//    
//    *((reg8*)CYREG_PRT6_DM0) = 0;
//    *((reg8*)CYREG_PRT6_DM1) = 1;
//    *((reg8*)CYREG_PRT6_DM2) = 1;
//    *((reg8*)CYREG_PRT6_DR) = 0;
//    
//    *((reg8*)CYREG_PRT12_DM0) = 0;
//    *((reg8*)CYREG_PRT12_DM1) = 1;
//    *((reg8*)CYREG_PRT12_DM2) = 1;
//    *((reg8*)CYREG_PRT12_DR) = 0;
//    
//    *((reg8*)CYREG_PRT15_DM0) = 0;
//    *((reg8*)CYREG_PRT15_DM1) = 1;
//    *((reg8*)CYREG_PRT15_DM2) = 1;
//    *((reg8*)CYREG_PRT15_DR) = 0;
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