//
//  cy_i2c.h
//  tests
//
//  Created by Kristoffer Lyder Andersen on 31/10/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#ifndef cy_i2c_h
#define cy_i2c_h

#include <stdint.h>

enum I2CModes
{
    I2C_MODE_COMPLETE_XFER,
    I2C_MODE_REPEAT_START,
    I2C_MODE_NO_STOP
};

enum ErrorStatus
{
    I2C_MSTR_NO_ERROR,
    I2C_MSTR_BUS_BUSY,
    I2C_MSTR_NOT_READY,
    I2C_MSTR_ERR_LB_NAK,
    I2C_MSTR_ERR_ARB_LOST,
    I2C_MSTR_ERR_ABORT_START_GEN
};

enum I2CStates
{
    I2C_MSTAT_XFER_INP
};

void I2C_Start();
void I2C_Stop();

uint8_t I2C_MasterStatus();

uint8_t I2C_MasterWriteBuf(uint8_t slaveAddress, uint8_t * wrData, uint8_t cnt, uint8_t mode);
uint8_t I2C_MasterReadBuf(uint8_t slaveAddr, uint8_t *data, uint8_t cnt, uint8_t mode);


#endif /* cy_i2c_h */
