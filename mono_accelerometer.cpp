// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "mono_accelerometer.h"

#include <I2C.h>
#include <mbed_debug.h>
#include "consoles.h"

using namespace mono::sensor;


MMAAccelerometer::MMAAccelerometer()
{
    I2C_Start();
}


void MMAAccelerometer::Start()
{
    uint8_t state;
    if (!readRegister(CTRL_REG1, &state))
        debug("accel start: could not read state\n\r");
    
    writeRegister(CTRL_REG1, (state|0x01));
}

void MMAAccelerometer::Stop()
{
    uint8_t state;
    if (!readRegister(CTRL_REG1, &state))
        debug("accel start: could not read state\n\r");
    
    writeRegister(CTRL_REG1, (state & ~0x01));
}


bool MMAAccelerometer::IsActive()
{
    uint8_t active;
    if (!readRegister(CTRL_REG1, &active))
    {
        debug("Accel: Could not read from register\n\r");
        return false;
    }
    
    debug("active is: %i\n\r",active);
    
    return (active & 0x01) == 0x01;
}

int16_t MMAAccelerometer::rawXAxis()
{
    uint8_t dataReady = 0xFF;
    readRegister(STATUS, &dataReady);
    
    if ((dataReady & 0x08) == 0)
        debug("data not ready!\n\r");
    
    uint8_t data[2];
    readRegister(OUT_X_MSB, (uint16_t*) data);
    
    int16_t x = ((data[0]<<8) | data[1]);
    
    return x>>4;
}



bool MMAAccelerometer::readRegister(uint8_t regAddr, uint8_t *data)
{
    int ack = I2C_MasterWriteBuf(accelAddress, &regAddr, 1, I2C_MODE_NO_STOP);
    
    if (ack != I2C_MSTR_NO_ERROR)
    {
        debug("write err: %i\n\r",ack);
        I2C_MasterSendStop();
        return false;
    }
    
    while ((I2C_MasterStatus() & (I2C_MSTAT_WR_CMPLT | I2C_MSTAT_ERR_XFER)) == 0);
    
    if (I2C_MasterStatus() & I2C_MSTAT_ERR_XFER)
    {
        debug("xfer error: 0x%x\n\r",I2C_MasterClearStatus());
        I2C_MasterSendStop();
        return false;
    }
    
    ack = I2C_MasterReadBuf(accelAddress, data, 1, I2C_MODE_REPEAT_START);
    
    while ((I2C_MasterStatus() & (I2C_MSTAT_RD_CMPLT | I2C_MSTAT_ERR_XFER)) == 0);
    
    I2C_MasterSendStop();
    
    return true;
}

bool MMAAccelerometer::readRegister(uint8_t regAddr, uint16_t *data)
{
    int ack = I2C_MasterWriteBuf(accelAddress, &regAddr, 1, I2C_MODE_NO_STOP);
    
    if (ack != I2C_MSTR_NO_ERROR)
    {
        debug("write err: %i\n\r",ack);
        I2C_MasterSendStop();
        return false;
    }
    
    while ((I2C_MasterStatus() & (I2C_MSTAT_WR_CMPLT | I2C_MSTAT_ERR_XFER)) == 0);
    
    if (I2C_MasterStatus() & I2C_MSTAT_ERR_XFER)
    {
        debug("xfer error: 0x%x\n\r",I2C_MasterClearStatus());
        I2C_MasterSendStop();
        return false;
    }
    
    ack = I2C_MasterReadBuf(accelAddress, (uint8_t*)data, 2, I2C_MODE_REPEAT_START);
    
    while ((I2C_MasterStatus() & (I2C_MSTAT_RD_CMPLT | I2C_MSTAT_ERR_XFER)) == 0);
    
    I2C_MasterSendStop();
    
    return true;
}

bool MMAAccelerometer::writeRegister(uint8_t regAddr, uint8_t data)
{
    uint8_t bytes[2];
    bytes[0] = regAddr;
    bytes[1] = data;
    
    int ack = I2C_MasterWriteBuf(accelAddress, bytes, 2, I2C_MODE_COMPLETE_XFER);
    
    if (ack != I2C_MSTR_NO_ERROR)
    {
        debug("data write err: %i\n\r",ack);
        I2C_MasterSendStop();
        return false;
    }
    
    while ((I2C_MasterStatus() & (I2C_MSTAT_WR_CMPLT | I2C_MSTAT_ERR_XFER)) == 0);
    
    return true;
}