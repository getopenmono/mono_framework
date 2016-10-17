/*******************************************************************************
* File Name: SPI_RP_INT.c
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

#include "SPI_RP_PVT.h"


/* User code required at start of ISR */
/* `#START SPI_RP_ISR_START_DEF` */

/* `#END` */


/*******************************************************************************
* Function Name: SPI_RP_TX_ISR
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
*  SPI_RP_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer.
*  SPI_RP_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer, modified when exist data to
*  sending and FIFO Not Full.
*  SPI_RP_txBuffer[SPI_RP_TX_BUFFER_SIZE] - used to store
*  data to sending.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(SPI_RP_TX_ISR)
{
    #if(SPI_RP_TX_SOFTWARE_BUF_ENABLED)
        uint8 tmpStatus;
    #endif /* (SPI_RP_TX_SOFTWARE_BUF_ENABLED) */

    #ifdef SPI_RP_TX_ISR_ENTRY_CALLBACK
        SPI_RP_TX_ISR_EntryCallback();
    #endif /* SPI_RP_TX_ISR_ENTRY_CALLBACK */

    /* User code required at start of ISR */
    /* `#START SPI_RP_TX_ISR_START` */

    /* `#END` */
    
    #if(SPI_RP_TX_SOFTWARE_BUF_ENABLED)
        /* Check if TX data buffer is not empty and there is space in TX FIFO */
        while(SPI_RP_txBufferRead != SPI_RP_txBufferWrite)
        {
            tmpStatus = SPI_RP_GET_STATUS_TX(SPI_RP_swStatusTx);
            SPI_RP_swStatusTx = tmpStatus;

            if(0u != (SPI_RP_swStatusTx & SPI_RP_STS_TX_FIFO_NOT_FULL))
            {
                if(0u == SPI_RP_txBufferFull)
                {
                   SPI_RP_txBufferRead++;

                    if(SPI_RP_txBufferRead >= SPI_RP_TX_BUFFER_SIZE)
                    {
                        SPI_RP_txBufferRead = 0u;
                    }
                }
                else
                {
                    SPI_RP_txBufferFull = 0u;
                }

                /* Put data element into the TX FIFO */
                CY_SET_REG8(SPI_RP_TXDATA_PTR, 
                                             SPI_RP_txBuffer[SPI_RP_txBufferRead]);
            }
            else
            {
                break;
            }
        }

        if(SPI_RP_txBufferRead == SPI_RP_txBufferWrite)
        {
            /* TX Buffer is EMPTY: disable interrupt on TX NOT FULL */
            SPI_RP_TX_STATUS_MASK_REG &= ((uint8) ~SPI_RP_STS_TX_FIFO_NOT_FULL);
        }

    #endif /* (SPI_RP_TX_SOFTWARE_BUF_ENABLED) */

    /* User code required at end of ISR (Optional) */
    /* `#START SPI_RP_TX_ISR_END` */

    /* `#END` */
    
    #ifdef SPI_RP_TX_ISR_EXIT_CALLBACK
        SPI_RP_TX_ISR_ExitCallback();
    #endif /* SPI_RP_TX_ISR_EXIT_CALLBACK */
}


/*******************************************************************************
* Function Name: SPI_RP_RX_ISR
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
*  SPI_RP_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer modified when FIFO contains
*  new data.
*  SPI_RP_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified when overflow occurred.
*  SPI_RP_rxBuffer[SPI_RP_RX_BUFFER_SIZE] - used to store
*  received data, modified when FIFO contains new data.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(SPI_RP_RX_ISR)
{
    #if(SPI_RP_RX_SOFTWARE_BUF_ENABLED)
        uint8 tmpStatus;
        uint8 rxData;
    #endif /* (SPI_RP_RX_SOFTWARE_BUF_ENABLED) */

    #ifdef SPI_RP_RX_ISR_ENTRY_CALLBACK
        SPI_RP_RX_ISR_EntryCallback();
    #endif /* SPI_RP_RX_ISR_ENTRY_CALLBACK */

    /* User code required at start of ISR */
    /* `#START SPI_RP_RX_ISR_START` */

    /* `#END` */
    
    #if(SPI_RP_RX_SOFTWARE_BUF_ENABLED)

        tmpStatus = SPI_RP_GET_STATUS_RX(SPI_RP_swStatusRx);
        SPI_RP_swStatusRx = tmpStatus;

        /* Check if RX data FIFO has some data to be moved into the RX Buffer */
        while(0u != (SPI_RP_swStatusRx & SPI_RP_STS_RX_FIFO_NOT_EMPTY))
        {
            rxData = CY_GET_REG8(SPI_RP_RXDATA_PTR);

            /* Set next pointer. */
            SPI_RP_rxBufferWrite++;
            if(SPI_RP_rxBufferWrite >= SPI_RP_RX_BUFFER_SIZE)
            {
                SPI_RP_rxBufferWrite = 0u;
            }

            if(SPI_RP_rxBufferWrite == SPI_RP_rxBufferRead)
            {
                SPI_RP_rxBufferRead++;
                if(SPI_RP_rxBufferRead >= SPI_RP_RX_BUFFER_SIZE)
                {
                    SPI_RP_rxBufferRead = 0u;
                }

                SPI_RP_rxBufferFull = 1u;
            }

            /* Move data from the FIFO to the Buffer */
            SPI_RP_rxBuffer[SPI_RP_rxBufferWrite] = rxData;

            tmpStatus = SPI_RP_GET_STATUS_RX(SPI_RP_swStatusRx);
            SPI_RP_swStatusRx = tmpStatus;
        }

    #endif /* (SPI_RP_RX_SOFTWARE_BUF_ENABLED) */

    /* User code required at end of ISR (Optional) */
    /* `#START SPI_RP_RX_ISR_END` */

    /* `#END` */
    
    #ifdef SPI_RP_RX_ISR_EXIT_CALLBACK
        SPI_RP_RX_ISR_ExitCallback();
    #endif /* SPI_RP_RX_ISR_EXIT_CALLBACK */
}

/* [] END OF FILE */
