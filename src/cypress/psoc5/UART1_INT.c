/*******************************************************************************
* File Name: UART1INT.c
* Version 2.50
*
* Description:
*  This file provides all Interrupt Service functionality of the UART component
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "UART1.h"



/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (UART1_RX_INTERRUPT_ENABLED && (UART1_RX_ENABLED || UART1_HD_ENABLED))
    /*******************************************************************************
    * Function Name: UART1_RXISR
    ********************************************************************************
    *
    * Summary:
    *  Interrupt Service Routine for RX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART1_rxBuffer - RAM buffer pointer for save received data.
    *  UART1_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  UART1_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  UART1_rxBufferOverflow - software overflow flag. Set to one
    *     when UART1_rxBufferWrite index overtakes
    *     UART1_rxBufferRead index.
    *  UART1_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when UART1_rxBufferWrite is equal to
    *    UART1_rxBufferRead
    *  UART1_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  UART1_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(UART1_RXISR)
    {
        uint8 readData;
        uint8 readStatus;
        uint8 increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef UART1_RXISR_ENTRY_CALLBACK
        UART1_RXISR_EntryCallback();
    #endif /* UART1_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START UART1_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = UART1_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support 
            *  of the user code in UART1_RXISR_ERROR` section. 
            */
            readData = readStatus;

            if((readStatus & (UART1_RX_STS_BREAK | 
                            UART1_RX_STS_PAR_ERROR |
                            UART1_RX_STS_STOP_ERROR | 
                            UART1_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                UART1_errorStatus |= readStatus & ( UART1_RX_STS_BREAK | 
                                                            UART1_RX_STS_PAR_ERROR | 
                                                            UART1_RX_STS_STOP_ERROR | 
                                                            UART1_RX_STS_OVERRUN);
                /* `#START UART1_RXISR_ERROR` */

                /* `#END` */
                
            #ifdef UART1_RXISR_ERROR_CALLBACK
                UART1_RXISR_ERROR_Callback();
            #endif /* UART1_RXISR_ERROR_CALLBACK */
            }
            
            if((readStatus & UART1_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = UART1_RXDATA_REG;
            #if (UART1_RXHW_ADDRESS_ENABLED)
                if(UART1_rxAddressMode == (uint8)UART1__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & UART1_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & UART1_RX_STS_ADDR_MATCH) != 0u)
                        {
                            UART1_rxAddressDetected = 1u;
                        }
                        else
                        {
                            UART1_rxAddressDetected = 0u;
                        }
                    }
                    if(UART1_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        UART1_rxBuffer[UART1_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    UART1_rxBuffer[UART1_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                UART1_rxBuffer[UART1_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (UART1_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(UART1_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        UART1_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    UART1_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(UART1_rxBufferWrite >= UART1_RX_BUFFER_SIZE)
                    {
                        UART1_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(UART1_rxBufferWrite == UART1_rxBufferRead)
                    {
                        UART1_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (UART1_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            UART1_RXSTATUS_MASK_REG  &= (uint8)~UART1_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(UART1_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (UART1_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & UART1_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START UART1_RXISR_END` */

        /* `#END` */

    #ifdef UART1_RXISR_EXIT_CALLBACK
        UART1_RXISR_ExitCallback();
    #endif /* UART1_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }
    
#endif /* (UART1_RX_INTERRUPT_ENABLED && (UART1_RX_ENABLED || UART1_HD_ENABLED)) */


#if (UART1_TX_INTERRUPT_ENABLED && UART1_TX_ENABLED)
    /*******************************************************************************
    * Function Name: UART1_TXISR
    ********************************************************************************
    *
    * Summary:
    * Interrupt Service Routine for the TX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART1_txBuffer - RAM buffer pointer for transmit data from.
    *  UART1_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  UART1_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(UART1_TXISR)
    {
    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef UART1_TXISR_ENTRY_CALLBACK
        UART1_TXISR_EntryCallback();
    #endif /* UART1_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START UART1_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((UART1_txBufferRead != UART1_txBufferWrite) &&
             ((UART1_TXSTATUS_REG & UART1_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(UART1_txBufferRead >= UART1_TX_BUFFER_SIZE)
            {
                UART1_txBufferRead = 0u;
            }

            UART1_TXDATA_REG = UART1_txBuffer[UART1_txBufferRead];

            /* Set next pointer */
            UART1_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START UART1_TXISR_END` */

        /* `#END` */

    #ifdef UART1_TXISR_EXIT_CALLBACK
        UART1_TXISR_ExitCallback();
    #endif /* UART1_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (UART1_TX_INTERRUPT_ENABLED && UART1_TX_ENABLED) */


/* [] END OF FILE */
