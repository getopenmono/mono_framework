// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "pct2075_temperature.h"

#include "application_context_interface.h"

extern "C" {
#include <project.h>
}

using namespace mono::sensor;

PCT2075Temperature::PCT2075Temperature()
{
    currentPointerRegister = 0xFF;
}

int PCT2075Temperature::Read()
{
    EnablePower();

    uint8_t temp[2] = {0,0};
    if (!readRegister(TEMP_REG, (uint16_t*)&temp))
        return 0xFFFF;

    return ((int)(temp[0]<<8 | temp[1])) >> 8; // 5 for LSB, and 3 for division by 8
}

// MARK: Power functions

void PCT2075Temperature::EnablePower()
{
    if (IApplicationContext::Instance->PowerManager->PowerSystem->IsPowerFenced())
    {
        IApplicationContext::Instance->PowerManager->PowerSystem->setPowerFence(false);
    }
}

// MARK: Read/Write Registers

bool PCT2075Temperature::writeRegister(uint8_t regAddress, uint8_t data)
{
    uint8_t bytes[2];
    bytes[0] = regAddress;
    bytes[1] = data;

    currentPointerRegister = regAddress;

    int ack = I2C_MasterWriteBuf(deviceAddress, bytes, 2, I2C_MODE_COMPLETE_XFER);

    if (ack != I2C_MSTR_NO_ERROR)
    {
        debug("data write err: %i\r\n",ack);
        I2C_MasterSendStop();
        return false;
    }

    while ((I2C_MasterStatus() & (I2C_MSTAT_WR_CMPLT | I2C_MSTAT_ERR_XFER)) == 0);

    return true;
}

bool PCT2075Temperature::readRegister(uint8_t regAddress, uint16_t *data)
{
    if (regAddress != currentPointerRegister)
    {
        int ack = I2C_MasterWriteBuf(deviceAddress, &regAddress, 1, I2C_MODE_NO_STOP);

        if (ack != I2C_MSTR_NO_ERROR)
        {
            debug("write err: %i\r\n",ack);
            I2C_MasterSendStop();
            return false;
        }

        while ((I2C_MasterStatus() & (I2C_MSTAT_WR_CMPLT | I2C_MSTAT_ERR_XFER)) == 0);

        if (I2C_MasterStatus() & I2C_MSTAT_ERR_XFER)
        {
            debug("xfer error: 0x%x\r\n",I2C_MasterClearStatus());
            I2C_MasterSendStop();
            return false;
        }
    }

    I2C_MasterReadBuf(deviceAddress, (uint8_t*)data, 2, I2C_MODE_REPEAT_START);

    while ((I2C_MasterStatus() & (I2C_MSTAT_RD_CMPLT | I2C_MSTAT_ERR_XFER)) == 0);

    I2C_MasterSendStop();

    return true;
}


// MARK: - Power Aware Interface

void PCT2075Temperature::onSystemPowerOnReset()
{

}

void PCT2075Temperature::onSystemEnterSleep()
{

}

void PCT2075Temperature::onSystemWakeFromSleep()
{

}

void PCT2075Temperature::OnSystemBatteryLow()
{

}