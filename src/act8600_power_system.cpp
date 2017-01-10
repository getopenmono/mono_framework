// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "act8600_power_system.h"
#include <mono.h>
#include <mbed_error.h>
#include <project.h>

using namespace mono::power;

ACT8600PowerSystem::ACT8600PowerSystem() :
    i2c(NC, NC),
    powerInterrupt(InterruptPin, PullUp)
{
    enableFaultMask();

    setSystemVoltageThreshold(VSYS_3V4);
    setSystemMonitorInterrupt(true);

    powerInterrupt.fall<ACT8600PowerSystem>(this, &ACT8600PowerSystem::powerInterruptHandler);
}

uint8_t ACT8600PowerSystem::SystemStatus()
{
    uint8_t data = 0x5A;
    if (!readRegister(SYS, &data))
    {
        //debug("Could not get SystemStatus, I2C failed\r\n");
        wait_ms(10);
    }

    return data;
}

IPowerSubSystem::ChargeState ACT8600PowerSystem::ChargeStatus()
{
    uint8_t data = 0x5A;
    if (!readRegister(APCH_4, &data)) {
        //printf("Failed to read APCH 4 register, I2C failed\r\n");
    }

    ACTChargeState state = (ACTChargeState) ((data >> 4) & 0x03);

    switch (state) {
        case PRECONDITION:
            return CHARGE_PRECONDITION;
            break;
        case FAST_CHARGE:
            return CHARGE_FAST;
            break;
        case END_OF_CHARGE:
            return CHARGE_ENDED;
            break;
        case SUSPENDED:
            return CHARGE_SUSPENDED;
            break;
        default:
            return UNKNOWN;
            break;
    }
}

bool ACT8600PowerSystem::PowerFencePeriperals()
{
    uint8_t data = 0x5A;
    if (!readRegister(REG1_EXT, &data))
    {
        //debug("could not read from power chip");
        return false;
    }

    return data & ENABLE ? false : true;
}


void ACT8600PowerSystem::powerOffUnused()
{
    bool success = true;
    uint8_t data = 0x0;

    readRegister(REG2_EXT, &data);
    success &= writeRegister(REG2_EXT, data & ~ENABLE);

    readRegister(REG3_EXT, &data);
    success &= writeRegister(REG3_EXT, data & ~ENABLE);

    readRegister(REG4_EXT, &data);
    success &= writeRegister(REG4_EXT, data & ~ENABLE);

    readRegister(REG5_EXT, &data);
    success &= writeRegister(REG5_EXT, data & ~ENABLE);

    readRegister(REG6_EXT, &data);
    success &= writeRegister(REG6_EXT, data & ~ENABLE);

    readRegister(REG7_EXT, &data);
    success &= writeRegister(REG7_EXT, data & ~ENABLE);

    readRegister(REG8_EXT, &data);
    success &= writeRegister(REG8_EXT, data & ~ENABLE);

    if (!success)
    {
        //debug("Failed to set unused power regs!\r\n");
    }
}

void ACT8600PowerSystem::powerReg8(bool power)
{
    uint8_t data = 0;
    readRegister(REG8_EXT, &data);

    if (power)
        writeRegister(REG8_EXT, data | ENABLE);
    else
        writeRegister(REG8_EXT, data & ~ENABLE);
}

void ACT8600PowerSystem::setPowerFence(bool active)
{
    // To keep the VSYS interrupt working, ensure that at least one REG is enabled
    if (active == true)
        powerReg8();

    setPowerFencePeripherals(active);
}

bool ACT8600PowerSystem::IsPowerFenced()
{
    return PowerFencePeriperals();
}

void ACT8600PowerSystem::setPowerFencePeripherals(bool off)
{
    uint8_t reg1 = 0x5A;
    if (!readRegister(REG1_EXT, &reg1))
    {
        //debug("failed to read REG1");
        return;
    }

    bool success;
    if (off)
    {
        success = writeRegister(REG1_EXT, reg1 & (~ENABLE));
    }
    else
    {
        success = writeRegister(REG1_EXT, reg1 | ENABLE);
    }

    if (!success)
    {
        //debug("failed to set power fence!\r\n");
    }
}

bool ACT8600PowerSystem::setUSBOTGPower(bool on)
{
    writeRegister(OTG, on?ONQ1:0x00);
    uint8_t otg = 0;
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
    uint8_t otg = 0x00;
    readRegister(OTG, &otg);

    return (otg & Q1OK) ? true : false;
}

uint8_t ACT8600PowerSystem::USBOTG()
{
    uint8_t otg = 0x00;
    readRegister(OTG, &otg);
    return otg;
}

bool ACT8600PowerSystem::IsPowerOk()
{
    // Enable at leat one regulator before checking VSYS OK state
    // For some reason ACT8600 needs at least one REG enabled before VSYS
    // can be OK at all!
    uint8_t data = 0x0;
    readRegister(REG8_EXT, &data);
    if ((data & ENABLE) == 0)
    {
        writeRegister(REG8_EXT, data | ENABLE);
    }

    uint8_t sysreg = 0x00;
    if (readRegister(SYS, &sysreg))
    {
        // Restore Regulator 8 state
        writeRegister(REG8_EXT, data & ~ENABLE);

        //printf("ACT8600 VSYS reg: 0x%x\t\n",sysreg);
        return ((sysreg & VSYSDAT) == 0) ? true : false;
    }

    // Restore Regulator 8 state
    writeRegister(REG8_EXT, data & ~ENABLE);

    //printf("ACT8600: failed to read sys reg!");
    return false;
}

void ACT8600PowerSystem::setSystemMonitorInterrupt(bool enable)
{
    uint8_t data = 0;
    if (!readRegister(SYS, &data))
    {
        //debug("ACT8600: Could not read SYS register\t\n");
        return;
    }
    //printf("SYS: 0x%x\t\n",data);
    if (enable)
        writeRegister(SYS, (data | SYSLEVMSKn));
    else
        writeRegister(SYS, (data & ~SYSLEVMSKn));
}

void ACT8600PowerSystem::setSystemVoltageThreshold(ACT8600PowerSystem::SystemVoltageLevels level)
{
    uint8_t data = 0;
    if (!readRegister(SYS, &data))
    {
        //debug("ACT8600: Could not read SYS register\t\n");
        return;
    }
    //printf("SYS: 0x%x\t\n",data);

    data &= ~SYSLEV; //clear all threshold bits
    data |= level; // set new level bits
    //printf("new SYS reg: 0x%x\t\n",data);
    writeRegister(SYS, data);
}

ACT8600PowerSystem::SystemVoltageLevels ACT8600PowerSystem::SystemVoltageThreshold()
{
    uint8_t data = 0;
    if (!readRegister(SYS, &data))
    {
        //debug("ACT8600: Could not read SYS register\t\n");
        return VSYS_DISABLED;
    }

    return (SystemVoltageLevels) (data & SYSLEV);
}

/// MARK: POWER AWARE SYSTEM METHODS

void ACT8600PowerSystem::onSystemPowerOnReset()
{
    //Set output voltage on OUT5 to 3.3V - to power MCU input supply the same as VSYS
    //writeRegister(REG5, VOLTAGE_SELECTION & V3_3);
}

void ACT8600PowerSystem::onSystemEnterSleep()
{
    setSystemMonitorInterrupt(false);
    powerInterrupt.setInterruptsSleep(false);
}

extern char *debugData;

void ACT8600PowerSystem::onSystemWakeFromSleep()
{
    powerInterrupt.setInterruptsSleep(true);

    // Enable at leat one regulator before checking VSYS OK state
    // For some reason ACT8600 needs at least one REG enabled before VSYS
    // can be OK at all!
    uint8_t data = 0x0;
    readRegister(REG8_EXT, &data);
    if ((data & ENABLE) == 0)
    {
        writeRegister(REG8_EXT, data | ENABLE);
    }

    uint8_t sysreg = 0;
    if (!readRegister(SYS, &sysreg))
        return;

    // Restore Regulator 8 state
    writeRegister(REG8_EXT, data & ~ENABLE);


    //SystemVoltageLevels threshold = (SystemVoltageLevels) (sysreg & SYSLEV);
    bool powerOk = !(sysreg & VSYSDAT);

    if (!powerOk)
    {
        PWM_WriteCompare2(64);
        wait_ms(30);
        PWM_WriteCompare2(0);

        BatteryEmptyHandler.call();
    }

    setSystemMonitorInterrupt(true);
}

/// MARK: Protected Methods


void ACT8600PowerSystem::enableFaultMask(bool enable)
{
    uint8_t data = 0;
    if (!readRegister(REG1_EXT, &data))
    {
        //debug("set could not set fault mask");
        return;
    }

    uint8_t bitmask = enable ? (data | FAULT_MASK_CONTROL) : (data & (~FAULT_MASK_CONTROL));

    if (!writeRegister(REG1_EXT, bitmask))
    {
        //debug("set could not set fault mask 2");
        return;
    }

}

void ACT8600PowerSystem::powerInterruptHandler()
{
    uint32_t diff = us_ticker_read() - powerInterrupt.FallTimeStamp();
    uint8_t data = 0x00;

    if (data == REG1_EXT)
        reg1Interrupts(diff);
    else if (data == SYS)
    {
        systemInterrupts(diff);
    }
    else
    {

    }
}

void ACT8600PowerSystem::reg1Interrupts(uint32_t diff)
{
    uint8_t data = 0;
    if (!readRegister(REG1_EXT, &data))
    {
        //debug("FATAL: could not read from power chip");
        setPowerFence(true);
        return;
    }

    if (diff < FaultTolerenceUs)
    {
        mono::Timer::callOnce<ACT8600PowerSystem>(1, this, &ACT8600PowerSystem::reg1InterruptFollowup);
        return;
    }

    if ((data & ENABLE) != 0 && (data & POWER_OK_STATUS) == 0)
    {
        //there is a fault on 3V3 power rail - turn it off!
        setPowerFence(true);

        // sad weeping beep
        uint8_t orgPeriod = PWM_ReadPeriod();

        for (int i=1; i<256; i++) {
            PWM_WritePeriod(i);
            PWM_WriteCompare2(i/2);
            wait_ms(2+i/16);
        }

        PWM_WriteCompare2(0);
        PWM_WritePeriod(orgPeriod);

        // goto sleep
        IApplicationContext::EnterSleepMode();
    }
}

void ACT8600PowerSystem::reg1InterruptFollowup()
{
    uint32_t diff = us_ticker_read() - powerInterrupt.FallTimeStamp();
    reg1Interrupts(diff);
}

void ACT8600PowerSystem::systemInterrupts(uint32_t diff)
{

    // Enable at leat one regulator before checking VSYS OK state
    // For some reason ACT8600 needs at least one REG enabled before VSYS
    // can be OK at all!
//    uint8_t data = 0x0;
//    readRegister(REG8_EXT, &data);
//    if ((data & ENABLE) == 0)
//    {
//        writeRegister(REG8_EXT, data | ENABLE);
//    }

    uint8_t sysreg = 0;
    if (!readRegister(SYS, &sysreg))
    {
        return;
    }


    // Restore Regulator 8 state
//    writeRegister(REG8_EXT, data & ~ENABLE);

    SystemVoltageLevels threshold = (SystemVoltageLevels) (sysreg & SYSLEV);
    bool powerOk = !(sysreg & VSYSDAT);
    bool interrupted = (sysreg & SYSSTATn) != 0;

    if (!interrupted)
    {
        return;
    }

    // if level is 3.4 or lower!
    if (!powerOk && threshold == VSYS_3V4)
    {
        PWM_WriteCompare2(64);
        wait_ms(30);
        PWM_WriteCompare2(0);

        BatteryEmptyHandler.call();
    }

    //if level is below 3.6
//    else if (!powerOk && threshold == VSYS_3V6)
//    {
//        debug("level is 3.6 or lower!\t\n");
//        BatteryLowHandler.call();
//        setSystemVoltageThreshold(VSYS_3V4);
//    }

}

/// MARK: READ / WRITE REGISTERS

bool ACT8600PowerSystem::readRegister(int8_t regAddr, uint8_t *regData)
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
        //debug("failed to read from act register (addr write)");
        return false;
    }


    status = i2c.read(ACT8600I2CAddress, (char*)regData, 1);
    if (status != 0)
    {
        act8600_i2c_en_Write(0);
        //debug("failed to read value act register");
        return false;
    }

    act8600_i2c_en_Write(0);
    return true;
}

bool ACT8600PowerSystem::writeRegister(int8_t regAddr, uint8_t regData)
{
    act8600_i2c_en_Write(1);
    CyDelayUs(10);
    uint8_t data[2] = {(uint8_t)regAddr, regData};
    bool success = i2c.write(ACT8600I2CAddress, (const char*)data, 2) == 0 ? true : false;
    act8600_i2c_en_Write(0);

    return success;
}
