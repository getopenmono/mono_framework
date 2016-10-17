/*******************************************************************************
* File Name: .h
* Version 2.50
*
* Description:
*  This private header file contains internal definitions for the SPIM
*  component. Do not use these definitions directly in your application.
*
* Note:
*
********************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SPIM_PVT_SPI1_H)
#define CY_SPIM_PVT_SPI1_H

#include "SPI1.h"


/**********************************
*   Functions with external linkage
**********************************/


/**********************************
*   Variables with external linkage
**********************************/

extern volatile uint8 SPI1_swStatusTx;
extern volatile uint8 SPI1_swStatusRx;

#if(SPI1_TX_SOFTWARE_BUF_ENABLED)
    extern volatile uint16 SPI1_txBuffer[SPI1_TX_BUFFER_SIZE];
    extern volatile uint8 SPI1_txBufferRead;
    extern volatile uint8 SPI1_txBufferWrite;
    extern volatile uint8 SPI1_txBufferFull;
#endif /* (SPI1_TX_SOFTWARE_BUF_ENABLED) */

#if(SPI1_RX_SOFTWARE_BUF_ENABLED)
    extern volatile uint16 SPI1_rxBuffer[SPI1_RX_BUFFER_SIZE];
    extern volatile uint8 SPI1_rxBufferRead;
    extern volatile uint8 SPI1_rxBufferWrite;
    extern volatile uint8 SPI1_rxBufferFull;
#endif /* (SPI1_RX_SOFTWARE_BUF_ENABLED) */

#endif /* CY_SPIM_PVT_SPI1_H */


/* [] END OF FILE */
