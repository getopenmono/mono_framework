//
//  cy_i2c.c
//  tests
//
//  Created by Kristoffer Lyder Andersen on 31/10/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#include "cy_i2c.h"

#include <string.h>
#include <stdio.h>

uint8_t I2C_started = 0;
const uint8_t I2C_size = 5;
uint8_t I2C_buffer[I2C_size];
uint8_t I2C_WPtr = 0;
uint8_t I2C_RPtr = 0;
uint8_t I2C_slaveAddr = 0x62;

void I2C_Start()
{
    I2C_started = 1;
    I2C_WPtr = I2C_RPtr = 0;
    memset(I2C_buffer, 0, I2C_size);
}
void I2C_Stop()
{
    I2C_started = 0;
}

uint8_t I2C_MasterStatus()
{
    return 0;
}

uint8_t I2C_MasterWriteBuf(uint8_t slaveAddress, uint8_t * wrData, uint8_t cnt, uint8_t mode)
{
    if (!I2C_started)
        return I2C_MSTR_NOT_READY;
    
    if (slaveAddress != I2C_slaveAddr)
        return I2C_MSTR_ERR_LB_NAK;
    
    int ptr = 0;
    
    do {
        
        if ((I2C_WPtr+1) % I2C_size == I2C_RPtr)
        {
            printf("I2C write FIFO full!");
            return I2C_MSTR_BUS_BUSY;
        }
        
        I2C_buffer[I2C_WPtr++] = wrData[ptr];
        
        if (I2C_WPtr % I2C_size == 0)
        {
            I2C_WPtr = 0;
        }
        
    } while (ptr++ < cnt);
    
    return I2C_MSTR_NO_ERROR;
}

uint8_t I2C_dataCnt()
{
    if (I2C_WPtr == I2C_RPtr)
        return 0;
    else if (I2C_RPtr < I2C_WPtr)
        return I2C_WPtr - I2C_RPtr;
    else
        return I2C_RPtr - I2C_WPtr - 1; // buffersize - 1
}

uint8_t I2C_MasterReadBuf(uint8_t slaveAddr, uint8_t *data, uint8_t cnt, uint8_t mode)
{
    if (!I2C_started)
        return I2C_MSTR_NOT_READY;
    
    if (slaveAddr != I2C_slaveAddr)
        return I2C_MSTR_ERR_LB_NAK;
    
    uint8_t ptr = 0;
    
    do {
        if (I2C_dataCnt() == 0)
            return 0;
        
        data[ptr] = I2C_buffer[I2C_RPtr++];
        
        if (I2C_RPtr % I2C_size == 0)
            I2C_RPtr = 0;
        
    } while(ptr++ < cnt);
    
    return I2C_MSTR_NO_ERROR;
}