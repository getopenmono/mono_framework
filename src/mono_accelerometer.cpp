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


void MMAAccelerometer::start()
{
    uint8_t state;
    if (!readRegister(CTRL_REG1, &state))
        debug("accel start: could not read state\r\n");

    writeRegister(CTRL_REG1, (state|0x01));
}

void MMAAccelerometer::stop()
{
    uint8_t state;
    if (!readRegister(CTRL_REG1, &state))
        debug("accel start: could not read state\r\n");

    writeRegister(CTRL_REG1, (state & ~0x01));
}


bool MMAAccelerometer::IsActive()
{
    uint8_t active;
    if (!readRegister(CTRL_REG1, &active))
    {
        debug("Accel: Could not read from register\r\n");
        return false;
    }

    debug("active is: %i\r\n",active);

    return (active & 0x01) == 0x01;
}

int16_t MMAAccelerometer::rawXAxis(bool monoOrientation)
{
    uint8_t dataReady = 0xFF;
    readRegister(STATUS, &dataReady);

    if ((dataReady & 0x08) == 0)
        debug("data not ready!\r\n");

    uint8_t data[2];
    readRegister(OUT_X_MSB, (uint16_t*) data);

    int16_t x = ((data[0]<<8) | data[1]);

    return x>>4;
}

int16_t MMAAccelerometer::rawYAxis(bool monoOrientation)
{
    uint8_t dataReady = 0xFF;
    readRegister(STATUS, &dataReady);

    if ((dataReady & 0x08) == 0)
        debug("data not ready!\r\n");

    uint8_t data[2];
    readRegister(OUT_Y_MSB, (uint16_t*) data);

    int16_t x = ((data[0]<<8) | data[1]);

    return x>>4;
}

int16_t MMAAccelerometer::rawZAxis(bool monoOrientation)
{
    uint8_t dataReady = 0xFF;
    readRegister(STATUS, &dataReady);

    if ((dataReady & 0x08) == 0)
        debug("data not ready!\r\n");

    uint8_t data[2];
    readRegister(OUT_Z_MSB, (uint16_t*) data);

    int16_t x = ((data[0]<<8) | data[1]);

    return x>>4;
}



bool MMAAccelerometer::readRegister(uint8_t regAddr, uint8_t *data)
{
    int ack = I2C_MasterWriteBuf(accelAddress, &regAddr, 1, I2C_MODE_NO_STOP);

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
        debug("data write err: %i\r\n",ack);
        I2C_MasterSendStop();
        return false;
    }

    while ((I2C_MasterStatus() & (I2C_MSTAT_WR_CMPLT | I2C_MSTAT_ERR_XFER)) == 0);

    return true;
}
