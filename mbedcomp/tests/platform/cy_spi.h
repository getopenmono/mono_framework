//
//  spi.h
//  tests
//
//  Created by Kristoffer Andersen on 30/10/2015.
//  Copyright © 2015 Monolit ApS. All rights reserved.
//

#ifndef cy_spi_h
#define cy_spi_h

#include <stdint.h>

enum SpiStatusBits
{
    SPI0_STS_SPI_DONE = 0x01,
    SPI0_STS_TX_FIFO_EMPTY = 0x02,
    SPI0_STS_TX_FIFO_NOT_FULL = 0x04,
    SPI0_STS_BYTE_COMPLETE = 0x08,
    SPI0_STS_SPI_IDLE = 0x10
};

void SPI0_Start();
void SPI0_Stop();

void SPI0_WriteTxData(uint8_t);
uint8_t SPI0_ReadTxStatus();

uint8_t SPI0_GetRxBufferSize();
uint8_t SPI0_ReadRxData();


void SPI1_Start();
void SPI1_Stop();

void SPI1_WriteTxData(uint8_t);
uint8_t SPI1_ReadTxStatus();

uint8_t SPI1_GetRxBufferSize();
uint8_t SPI1_ReadRxData();

#endif /* cy_spi_h */
