// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "at30ts74_temperature.h"

#include <mbed_debug.h>
#include <project.h>
#include <wait_api.h>
#include <consoles.h>

using namespace mono::sensor;

//static ITemperature *Temperature = 0;

AT30TS74Temperature::AT30TS74Temperature()
{
    currentPointerRegister = 0x00;
}

int AT30TS74Temperature::Read()
{
    uint8_t temp[2] = {0,0};
    if (!readRegister(TEMPERATURE_REG, (uint16_t*)&temp))
        return 0xFFFF;

    int rawTemp = (temp[0]<<8 | temp[1]) >> 4; // right shoft 12-bit number
    int celcius = rawTemp >> 4; // divide by 16 to get celcius

    return celcius;
}

int AT30TS74Temperature::ReadMilliCelcius()
{
    uint8_t temp[2] = {0,0};
    if (!readRegister(TEMPERATURE_REG, (uint16_t*)&temp))
        return 0xFFFF;

    int rawTemp = (temp[0]<<8 | temp[1]) >> 4; // right shoft 12-bit number
    int millicelcius = (rawTemp*1000) >> 4; // divide by 16 to get milli-celcius

    return millicelcius;
}

// MARK: Read/Write Registers

bool AT30TS74Temperature::writeRegister(uint8_t regAddress, uint8_t data)
{
    uint8_t bytes[2];
    bytes[0] = regAddress;
    bytes[1] = data;
    
    currentPointerRegister = regAddress;
    
    int ack = I2C_MasterWriteBuf(deviceAddress, bytes, 2, I2C_MODE_COMPLETE_XFER);
    
    if (ack != I2C_MSTR_NO_ERROR)
    {
        debug("data write err: %i\n\r",ack);
        I2C_MasterSendStop();
        return false;
    }
    
    while ((I2C_MasterStatus() & (I2C_MSTAT_WR_CMPLT | I2C_MSTAT_ERR_XFER)) == 0);
    
    return true;
}

bool AT30TS74Temperature::readRegister(uint8_t regAddress, uint16_t *data)
{
    //act8600_i2c_en_Write(0);
    if (regAddress != currentPointerRegister)
    {
        int ack = I2C_MasterWriteBuf(deviceAddress, &regAddress, 1, I2C_MODE_COMPLETE_XFER);
        
        if (ack != I2C_MSTR_NO_ERROR)
        {
            debug("read-reg: addr write err: %i\n\r",ack);
            I2C_MasterSendStop();
            return false;
        }
        
        while ((I2C_MasterStatus() & (I2C_MSTAT_WR_CMPLT | I2C_MSTAT_ERR_XFER)) == 0);
        
        if (I2C_MasterStatus() & I2C_MSTAT_ERR_XFER)
        {
            debug("read reg: value xfer error: 0x%x\n\r",I2C_MasterClearStatus());
            I2C_MasterSendStop();
            return false;
        }
    }
    
    I2C_MasterReadBuf(deviceAddress, (uint8_t*)data, 2, I2C_MODE_COMPLETE_XFER);
    int timeout = 500;
    while ((I2C_MasterStatus() & (I2C_MSTAT_RD_CMPLT | I2C_MSTAT_ERR_XFER)) == 0 && timeout > 0)
    {
        wait_us(1);
        timeout--;
    }

    if (I2C_MasterStatus() & I2C_MSTAT_ERR_XFER)
    {
        debug("read xfer error: 0x%x\n\r",I2C_MasterClearStatus());
        return false;
    }
    
    if (timeout == 0)
    {
        debug("Failed to receive temp data!\n\r");
        return false;
    }
    
    return true;
}