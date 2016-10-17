/*******************************************************************************
* File Name: SPI1_INT.c
* Version 2.50
*
* Description:
*  This file provides all Interrupt Service Routine (ISR) for the SPI Master
*  component.
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPI1_PVT.h"


/* User code required at start of ISR */
/* `#START SPI1_ISR_START_DEF` */

/* `#END` */


/*******************************************************************************
* Function Name: SPI1_TX_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for TX portion of the SPI Master.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  SPI1_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer.
*  SPI1_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer, modified when exist data to
*  sending and FIFO Not Full.
*  SPI1_txBuffer[SPI1_TX_BUFFER_SIZE] - used to store
*  data to sending.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(SPI1_TX_ISR)
{
    #if(SPI1_TX_SOFTWARE_BUF_ENABLED)
        uint8 tmpStatus;
    #endif /* (SPI1_TX_SOFTWARE_BUF_ENABLED) */

    #ifdef SPI1_TX_ISR_ENTRY_CALLBACK
        SPI1_TX_ISR_EntryCallback();
    #endif /* SPI1_TX_ISR_ENTRY_CALLBACK */

    /* User code required at start of ISR */
    /* `#START SPI1_TX_ISR_START` */

    /* `#END` */
    
    #if(SPI1_TX_SOFTWARE_BUF_ENABLED)
        /* Check if TX data buffer is not empty and there is space in TX FIFO */
        while(SPI1_txBufferRead != SPI1_txBufferWrite)
        {
            tmpStatus = SPI1_GET_STATUS_TX(SPI1_swStatusTx);
            SPI1_swStatusTx = tmpStatus;

            if(0u != (SPI1_swStatusTx & SPI1_STS_TX_FIFO_NOT_FULL))
            {
                if(0u == SPI1_txBufferFull)
                {
                   SPI1_txBufferRead++;

                    if(SPI1_txBufferRead >= SPI1_TX_BUFFER_SIZE)
                    {
                        SPI1_txBufferRead = 0u;
                    }
                }
                else
                {
                    SPI1_txBufferFull = 0u;
                }

                /* Put data element into the TX FIFO */
                CY_SET_REG16(SPI1_TXDATA_PTR, 
                                             SPI1_txBuffer[SPI1_txBufferRead]);
            }
            else
            {
                break;
            }
        }

        if(SPI1_txBufferRead == SPI1_txBufferWrite)
        {
            /* TX Buffer is EMPTY: disable interrupt on TX NOT FULL */
            SPI1_TX_STATUS_MASK_REG &= ((uint8) ~SPI1_STS_TX_FIFO_NOT_FULL);
        }

    #endif /* (SPI1_TX_SOFTWARE_BUF_ENABLED) */

    /* User code required at end of ISR (Optional) */
    /* `#START SPI1_TX_ISR_END` */

    /* `#END` */
    
    #ifdef SPI1_TX_ISR_EXIT_CALLBACK
        SPI1_TX_ISR_ExitCallback();
    #endif /* SPI1_TX_ISR_EXIT_CALLBACK */
}


/*******************************************************************************
* Function Name: SPI1_RX_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for RX portion of the SPI Master.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  SPI1_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer modified when FIFO contains
*  new data.
*  SPI1_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified when overflow occurred.
*  SPI1_rxBuffer[SPI1_RX_BUFFER_SIZE] - used to store
*  received data, modified when FIFO contains new data.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(SPI1_RX_ISR)
{
    #if(SPI1_RX_SOFTWARE_BUF_ENABLED)
        uint8 tmpStatus;
        uint16 rxData;
    #endif /* (SPI1_RX_SOFTWARE_BUF_ENABLED) */

    #ifdef SPI1_RX_ISR_ENTRY_CALLBACK
        SPI1_RX_ISR_EntryCallback();
    #endif /* SPI1_RX_ISR_ENTRY_CALLBACK */

    /* User code required at start of ISR */
    /* `#START SPI1_RX_ISR_START` */

    /* `#END` */
    
    #if(SPI1_RX_SOFTWARE_BUF_ENABLED)

        tmpStatus = SPI1_GET_STATUS_RX(SPI1_swStatusRx);
        SPI1_swStatusRx = tmpStatus;

        /* Check if RX data FIFO has some data to be moved into the RX Buffer */
        while(0u != (SPI1_swStatusRx & SPI1_STS_RX_FIFO_NOT_EMPTY))
        {
            rxData = CY_GET_REG16(SPI1_RXDATA_PTR);

            /* Set next pointer. */
            SPI1_rxBufferWrite++;
            if(SPI1_rxBufferWrite >= SPI1_RX_BUFFER_SIZE)
            {
                SPI1_rxBufferWrite = 0u;
            }

            if(SPI1_rxBufferWrite == SPI1_rxBufferRead)
            {
                SPI1_rxBufferRead++;
                if(SPI1_rxBufferRead >= SPI1_RX_BUFFER_SIZE)
                {
                    SPI1_rxBufferRead = 0u;
                }

                SPI1_rxBufferFull = 1u;
            }

            /* Move data from the FIFO to the Buffer */
            SPI1_rxBuffer[SPI1_rxBufferWrite] = rxData;

            tmpStatus = SPI1_GET_STATUS_RX(SPI1_swStatusRx);
            SPI1_swStatusRx = tmpStatus;
        }

    #endif /* (SPI1_RX_SOFTWARE_BUF_ENABLED) */

    /* User code required at end of ISR (Optional) */
    /* `#START SPI1_RX_ISR_END` */

    /* `#END` */
    
    #ifdef SPI1_RX_ISR_EXIT_CALLBACK
        SPI1_RX_ISR_ExitCallback();
    #endif /* SPI1_RX_ISR_EXIT_CALLBACK */
}

/* [] END OF FILE */
