//
//  spi.c
//  tests
//
//  Created by Kristoffer Andersen on 30/10/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#include "cy_spi.h"
#include <string.h>
#include <stdio.h>

uint8_t SPI0_started = 0;
uint8_t SPI0_bufPointer = 0;
uint8_t SPI0_bufPointerRd = 0;
uint8_t SPI0_statusReg = 0x17; //done, idle, empty, not full
uint8_t SPI0_asyncDelay = 0; // no delay
uint8_t SPI0_asyncDelayPtr = 0;
const int SPI0_size = 5;
uint8_t SPI0_buffer[SPI0_size] = {0,0,0,0,0};

void SPI0_Start()
{
    memset(SPI0_buffer,0,SPI0_size);
    SPI0_bufPointer = 0;
    SPI0_bufPointerRd = 0;
    SPI0_started = 1;
}

void SPI0_Stop()
{
    SPI0_started = 0;
}

void SPI0_WriteTxData(uint8_t data)
{
    if (!SPI0_started)
        return;
    
    if ((SPI0_bufPointer+1) % SPI0_size == SPI0_bufPointerRd)
    {
        printf("SPI0 FIFO full!\n");
        return;
    }
    
    SPI0_buffer[SPI0_bufPointer++] = data;
    
    SPI0_statusReg &= ~(SPI0_STS_TX_FIFO_EMPTY | SPI0_STS_SPI_IDLE); // set not empty AND not idle
    
    if (SPI0_bufPointer % SPI0_size == 0)
    {
        SPI0_bufPointer = 0;
    }
    
    if ((SPI0_bufPointer+1) % SPI0_size == SPI0_bufPointerRd)
        SPI0_statusReg &= ~SPI0_STS_TX_FIFO_NOT_FULL; // set full
    
    SPI0_statusReg |= SPI0_STS_BYTE_COMPLETE;
}

uint8_t SPI0_ReadTxStatus()
{
    return SPI0_statusReg;
}

uint8_t SPI0_GetRxBufferSize()
{
    if (SPI0_bufPointer == SPI0_bufPointerRd)
        return 0;
    else if (SPI0_bufPointerRd < SPI0_bufPointer)
    {
        return SPI0_bufPointer - SPI0_bufPointerRd;
    }
    else
    {
        return SPI0_bufPointerRd - SPI0_bufPointer -1; // buffersize - 1
    }
}

uint8_t SPI0_ReadRxData()
{
    if (SPI0_GetRxBufferSize() > 0)
    {
        uint8_t d = SPI0_buffer[SPI0_bufPointerRd++];
        
        if (SPI0_bufPointerRd % SPI0_size == 0)
            SPI0_bufPointerRd = 0;
        
        if (SPI0_GetRxBufferSize() == 0)
            SPI0_statusReg |= SPI0_STS_TX_FIFO_EMPTY | SPI0_STS_TX_FIFO_NOT_FULL;
        
        return d;
    }
    else
    {
        printf("SPI0 buffer empty!\n");
        return 0; // no data avail
    }
}




uint8_t SPI1_started = 0;
uint8_t SPI1_bufPointer = 0;
uint8_t SPI1_bufPointerRd = 0;
uint8_t SPI1_statusReg = 0x17; //done, idle, empty, not full
uint8_t SPI1_asyncDelay = 0; // no delay
uint8_t SPI1_asyncDelayPtr = 0;
const int SPI1_size = 5;
uint8_t SPI1_buffer[SPI0_size] = {0,0,0,0,0};


void SPI1_Start()
{
    memset(SPI1_buffer,0,SPI0_size);
    SPI1_bufPointer = 0;
    SPI1_bufPointerRd = 0;
    SPI1_started = 1;
}

void SPI1_Stop()
{
    SPI1_started = 0;
}

void SPI1_WriteTxData(uint8_t data)
{
    if (!SPI1_started)
        return;
    
    if ((SPI1_bufPointer+1) % SPI1_size == SPI1_bufPointerRd)
    {
        printf("SPI1 FIFO full!\n");
        return;
    }
    
    SPI1_buffer[SPI1_bufPointer++] = data;
    
    SPI1_statusReg &= ~(SPI0_STS_TX_FIFO_EMPTY | SPI0_STS_SPI_IDLE); // set not empty AND not idle
    
    if (SPI1_bufPointer % SPI1_size == 0)
    {
        SPI1_bufPointer = 0;
    }
    
    if ((SPI1_bufPointer+1) % SPI1_size == SPI1_bufPointerRd)
        SPI1_statusReg &= ~SPI0_STS_TX_FIFO_NOT_FULL; // set full
    
    SPI1_statusReg |= SPI0_STS_BYTE_COMPLETE;
}

uint8_t SPI1_ReadTxStatus()
{
    return SPI1_statusReg;
}

uint8_t SPI1_GetRxBufferSize()
{
    if (SPI1_bufPointer == SPI1_bufPointerRd)
        return 0;
    else if (SPI1_bufPointerRd < SPI1_bufPointer)
    {
        return SPI1_bufPointer - SPI1_bufPointerRd;
    }
    else
    {
        return SPI1_bufPointerRd - SPI1_bufPointer -1; // buffersize - 1
    }
}

uint8_t SPI1_ReadRxData()
{
    if (SPI1_GetRxBufferSize() > 0)
    {
        uint8_t d = SPI1_buffer[SPI1_bufPointerRd++];
        
        if (SPI1_bufPointerRd % SPI1_size == 0)
            SPI1_bufPointerRd = 0;
        
        if (SPI1_GetRxBufferSize() == 0)
            SPI1_statusReg |= SPI0_STS_TX_FIFO_EMPTY | SPI0_STS_TX_FIFO_NOT_FULL;
        
        return d;
    }
    else
    {
        printf("SPI1 buffer empty!\n");
        return 0; // no data avail
    }
}