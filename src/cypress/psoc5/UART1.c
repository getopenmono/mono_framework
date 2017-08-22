/*******************************************************************************
* File Name: UART1.c
* Version 2.50
*
* Description:
*  This file provides all API functionality of the UART component
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "UART1.h"
#if (UART1_INTERNAL_CLOCK_USED)
    #include "UART1_IntClock.h"
#endif /* End UART1_INTERNAL_CLOCK_USED */


/***************************************
* Global data allocation
***************************************/

uint8 UART1_initVar = 0u;

#if (UART1_TX_INTERRUPT_ENABLED && UART1_TX_ENABLED)
    volatile uint8 UART1_txBuffer[UART1_TX_BUFFER_SIZE];
    volatile uint8 UART1_txBufferRead = 0u;
    uint8 UART1_txBufferWrite = 0u;
#endif /* (UART1_TX_INTERRUPT_ENABLED && UART1_TX_ENABLED) */

#if (UART1_RX_INTERRUPT_ENABLED && (UART1_RX_ENABLED || UART1_HD_ENABLED))
    uint8 UART1_errorStatus = 0u;
    volatile uint8 UART1_rxBuffer[UART1_RX_BUFFER_SIZE];
    volatile uint8 UART1_rxBufferRead  = 0u;
    volatile uint8 UART1_rxBufferWrite = 0u;
    volatile uint8 UART1_rxBufferLoopDetect = 0u;
    volatile uint8 UART1_rxBufferOverflow   = 0u;
    #if (UART1_RXHW_ADDRESS_ENABLED)
        volatile uint8 UART1_rxAddressMode = UART1_RX_ADDRESS_MODE;
        volatile uint8 UART1_rxAddressDetected = 0u;
    #endif /* (UART1_RXHW_ADDRESS_ENABLED) */
#endif /* (UART1_RX_INTERRUPT_ENABLED && (UART1_RX_ENABLED || UART1_HD_ENABLED)) */


/*******************************************************************************
* Function Name: UART1_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin component operation.
*  UART1_Start() sets the initVar variable, calls the
*  UART1_Init() function, and then calls the
*  UART1_Enable() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The UART1_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time UART1_Start() is called. This
*  allows for component initialization without re-initialization in all
*  subsequent calls to the UART1_Start() routine.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART1_Start(void) 
{
    /* If not initialized then initialize all required hardware and software */
    if(UART1_initVar == 0u)
    {
        UART1_Init();
        UART1_initVar = 1u;
    }

    UART1_Enable();
}


/*******************************************************************************
* Function Name: UART1_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure
*  dialog settings. It is not necessary to call UART1_Init() because
*  the UART1_Start() API calls this function and is the preferred
*  method to begin component operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void UART1_Init(void) 
{
    #if(UART1_RX_ENABLED || UART1_HD_ENABLED)

        #if (UART1_RX_INTERRUPT_ENABLED)
            /* Set RX interrupt vector and priority */
            (void) CyIntSetVector(UART1_RX_VECT_NUM, &UART1_RXISR);
            CyIntSetPriority(UART1_RX_VECT_NUM, UART1_RX_PRIOR_NUM);
            UART1_errorStatus = 0u;
        #endif /* (UART1_RX_INTERRUPT_ENABLED) */

        #if (UART1_RXHW_ADDRESS_ENABLED)
            UART1_SetRxAddressMode(UART1_RX_ADDRESS_MODE);
            UART1_SetRxAddress1(UART1_RX_HW_ADDRESS1);
            UART1_SetRxAddress2(UART1_RX_HW_ADDRESS2);
        #endif /* End UART1_RXHW_ADDRESS_ENABLED */

        /* Init Count7 period */
        UART1_RXBITCTR_PERIOD_REG = UART1_RXBITCTR_INIT;
        /* Configure the Initial RX interrupt mask */
        UART1_RXSTATUS_MASK_REG  = UART1_INIT_RX_INTERRUPTS_MASK;
    #endif /* End UART1_RX_ENABLED || UART1_HD_ENABLED*/

    #if(UART1_TX_ENABLED)
        #if (UART1_TX_INTERRUPT_ENABLED)
            /* Set TX interrupt vector and priority */
            (void) CyIntSetVector(UART1_TX_VECT_NUM, &UART1_TXISR);
            CyIntSetPriority(UART1_TX_VECT_NUM, UART1_TX_PRIOR_NUM);
        #endif /* (UART1_TX_INTERRUPT_ENABLED) */

        /* Write Counter Value for TX Bit Clk Generator*/
        #if (UART1_TXCLKGEN_DP)
            UART1_TXBITCLKGEN_CTR_REG = UART1_BIT_CENTER;
            UART1_TXBITCLKTX_COMPLETE_REG = ((UART1_NUMBER_OF_DATA_BITS +
                        UART1_NUMBER_OF_START_BIT) * UART1_OVER_SAMPLE_COUNT) - 1u;
        #else
            UART1_TXBITCTR_PERIOD_REG = ((UART1_NUMBER_OF_DATA_BITS +
                        UART1_NUMBER_OF_START_BIT) * UART1_OVER_SAMPLE_8) - 1u;
        #endif /* End UART1_TXCLKGEN_DP */

        /* Configure the Initial TX interrupt mask */
        #if (UART1_TX_INTERRUPT_ENABLED)
            UART1_TXSTATUS_MASK_REG = UART1_TX_STS_FIFO_EMPTY;
        #else
            UART1_TXSTATUS_MASK_REG = UART1_INIT_TX_INTERRUPTS_MASK;
        #endif /*End UART1_TX_INTERRUPT_ENABLED*/

    #endif /* End UART1_TX_ENABLED */

    #if(UART1_PARITY_TYPE_SW)  /* Write Parity to Control Register */
        UART1_WriteControlRegister( \
            (UART1_ReadControlRegister() & (uint8)~UART1_CTRL_PARITY_TYPE_MASK) | \
            (uint8)(UART1_PARITY_TYPE << UART1_CTRL_PARITY_TYPE0_SHIFT) );
    #endif /* End UART1_PARITY_TYPE_SW */
}


/*******************************************************************************
* Function Name: UART1_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call UART1_Enable() because the UART1_Start() API
*  calls this function, which is the preferred method to begin component
*  operation.

* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART1_rxAddressDetected - set to initial state (0).
*
*******************************************************************************/
void UART1_Enable(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    #if (UART1_RX_ENABLED || UART1_HD_ENABLED)
        /* RX Counter (Count7) Enable */
        UART1_RXBITCTR_CONTROL_REG |= UART1_CNTR_ENABLE;

        /* Enable the RX Interrupt */
        UART1_RXSTATUS_ACTL_REG  |= UART1_INT_ENABLE;

        #if (UART1_RX_INTERRUPT_ENABLED)
            UART1_EnableRxInt();

            #if (UART1_RXHW_ADDRESS_ENABLED)
                UART1_rxAddressDetected = 0u;
            #endif /* (UART1_RXHW_ADDRESS_ENABLED) */
        #endif /* (UART1_RX_INTERRUPT_ENABLED) */
    #endif /* (UART1_RX_ENABLED || UART1_HD_ENABLED) */

    #if(UART1_TX_ENABLED)
        /* TX Counter (DP/Count7) Enable */
        #if(!UART1_TXCLKGEN_DP)
            UART1_TXBITCTR_CONTROL_REG |= UART1_CNTR_ENABLE;
        #endif /* End UART1_TXCLKGEN_DP */

        /* Enable the TX Interrupt */
        UART1_TXSTATUS_ACTL_REG |= UART1_INT_ENABLE;
        #if (UART1_TX_INTERRUPT_ENABLED)
            UART1_ClearPendingTxInt(); /* Clear history of TX_NOT_EMPTY */
            UART1_EnableTxInt();
        #endif /* (UART1_TX_INTERRUPT_ENABLED) */
     #endif /* (UART1_TX_INTERRUPT_ENABLED) */

    #if (UART1_INTERNAL_CLOCK_USED)
        UART1_IntClock_Start();  /* Enable the clock */
    #endif /* (UART1_INTERNAL_CLOCK_USED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: UART1_Stop
********************************************************************************
*
* Summary:
*  Disables the UART operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void UART1_Stop(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /* Write Bit Counter Disable */
    #if (UART1_RX_ENABLED || UART1_HD_ENABLED)
        UART1_RXBITCTR_CONTROL_REG &= (uint8) ~UART1_CNTR_ENABLE;
    #endif /* (UART1_RX_ENABLED || UART1_HD_ENABLED) */

    #if (UART1_TX_ENABLED)
        #if(!UART1_TXCLKGEN_DP)
            UART1_TXBITCTR_CONTROL_REG &= (uint8) ~UART1_CNTR_ENABLE;
        #endif /* (!UART1_TXCLKGEN_DP) */
    #endif /* (UART1_TX_ENABLED) */

    #if (UART1_INTERNAL_CLOCK_USED)
        UART1_IntClock_Stop();   /* Disable the clock */
    #endif /* (UART1_INTERNAL_CLOCK_USED) */

    /* Disable internal interrupt component */
    #if (UART1_RX_ENABLED || UART1_HD_ENABLED)
        UART1_RXSTATUS_ACTL_REG  &= (uint8) ~UART1_INT_ENABLE;

        #if (UART1_RX_INTERRUPT_ENABLED)
            UART1_DisableRxInt();
        #endif /* (UART1_RX_INTERRUPT_ENABLED) */
    #endif /* (UART1_RX_ENABLED || UART1_HD_ENABLED) */

    #if (UART1_TX_ENABLED)
        UART1_TXSTATUS_ACTL_REG &= (uint8) ~UART1_INT_ENABLE;

        #if (UART1_TX_INTERRUPT_ENABLED)
            UART1_DisableTxInt();
        #endif /* (UART1_TX_INTERRUPT_ENABLED) */
    #endif /* (UART1_TX_ENABLED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: UART1_ReadControlRegister
********************************************************************************
*
* Summary:
*  Returns the current value of the control register.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the control register.
*
*******************************************************************************/
uint8 UART1_ReadControlRegister(void) 
{
    #if (UART1_CONTROL_REG_REMOVED)
        return(0u);
    #else
        return(UART1_CONTROL_REG);
    #endif /* (UART1_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: UART1_WriteControlRegister
********************************************************************************
*
* Summary:
*  Writes an 8-bit value into the control register
*
* Parameters:
*  control:  control register value
*
* Return:
*  None.
*
*******************************************************************************/
void  UART1_WriteControlRegister(uint8 control) 
{
    #if (UART1_CONTROL_REG_REMOVED)
        if(0u != control)
        {
            /* Suppress compiler warning */
        }
    #else
       UART1_CONTROL_REG = control;
    #endif /* (UART1_CONTROL_REG_REMOVED) */
}


#if(UART1_RX_ENABLED || UART1_HD_ENABLED)
    /*******************************************************************************
    * Function Name: UART1_SetRxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the RX interrupt sources enabled.
    *
    * Parameters:
    *  IntSrc:  Bit field containing the RX interrupts to enable. Based on the 
    *  bit-field arrangement of the status register. This value must be a 
    *  combination of status register bit-masks shown below:
    *      UART1_RX_STS_FIFO_NOTEMPTY    Interrupt on byte received.
    *      UART1_RX_STS_PAR_ERROR        Interrupt on parity error.
    *      UART1_RX_STS_STOP_ERROR       Interrupt on stop error.
    *      UART1_RX_STS_BREAK            Interrupt on break.
    *      UART1_RX_STS_OVERRUN          Interrupt on overrun error.
    *      UART1_RX_STS_ADDR_MATCH       Interrupt on address match.
    *      UART1_RX_STS_MRKSPC           Interrupt on address detect.
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void UART1_SetRxInterruptMode(uint8 intSrc) 
    {
        UART1_RXSTATUS_MASK_REG  = intSrc;
    }


    /*******************************************************************************
    * Function Name: UART1_ReadRxData
    ********************************************************************************
    *
    * Summary:
    *  Returns the next byte of received data. This function returns data without
    *  checking the status. You must check the status separately.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Received data from RX register
    *
    * Global Variables:
    *  UART1_rxBuffer - RAM buffer pointer for save received data.
    *  UART1_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  UART1_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  UART1_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 UART1_ReadRxData(void) 
    {
        uint8 rxData;

    #if (UART1_RX_INTERRUPT_ENABLED)

        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        UART1_DisableRxInt();

        locRxBufferRead  = UART1_rxBufferRead;
        locRxBufferWrite = UART1_rxBufferWrite;

        if( (UART1_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = UART1_rxBuffer[locRxBufferRead];
            locRxBufferRead++;

            if(locRxBufferRead >= UART1_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            UART1_rxBufferRead = locRxBufferRead;

            if(UART1_rxBufferLoopDetect != 0u)
            {
                UART1_rxBufferLoopDetect = 0u;
                #if ((UART1_RX_INTERRUPT_ENABLED) && (UART1_FLOW_CONTROL != 0u))
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( UART1_HD_ENABLED )
                        if((UART1_CONTROL_REG & UART1_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only in RX
                            *  configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            UART1_RXSTATUS_MASK_REG  |= UART1_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        UART1_RXSTATUS_MASK_REG  |= UART1_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end UART1_HD_ENABLED */
                #endif /* ((UART1_RX_INTERRUPT_ENABLED) && (UART1_FLOW_CONTROL != 0u)) */
            }
        }
        else
        {   /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
            rxData = UART1_RXDATA_REG;
        }

        UART1_EnableRxInt();

    #else

        /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
        rxData = UART1_RXDATA_REG;

    #endif /* (UART1_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: UART1_ReadRxStatus
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the receiver status register and the software
    *  buffer overflow status.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Current state of the status register.
    *
    * Side Effect:
    *  All status register bits are clear-on-read except
    *  UART1_RX_STS_FIFO_NOTEMPTY.
    *  UART1_RX_STS_FIFO_NOTEMPTY clears immediately after RX data
    *  register read.
    *
    * Global Variables:
    *  UART1_rxBufferOverflow - used to indicate overload condition.
    *   It set to one in RX interrupt when there isn't free space in
    *   UART1_rxBufferRead to write new data. This condition returned
    *   and cleared to zero by this API as an
    *   UART1_RX_STS_SOFT_BUFF_OVER bit along with RX Status register
    *   bits.
    *
    *******************************************************************************/
    uint8 UART1_ReadRxStatus(void) 
    {
        uint8 status;

        status = UART1_RXSTATUS_REG & UART1_RX_HW_MASK;

    #if (UART1_RX_INTERRUPT_ENABLED)
        if(UART1_rxBufferOverflow != 0u)
        {
            status |= UART1_RX_STS_SOFT_BUFF_OVER;
            UART1_rxBufferOverflow = 0u;
        }
    #endif /* (UART1_RX_INTERRUPT_ENABLED) */

        return(status);
    }


    /*******************************************************************************
    * Function Name: UART1_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Returns the last received byte of data. UART1_GetChar() is
    *  designed for ASCII characters and returns a uint8 where 1 to 255 are values
    *  for valid characters and 0 indicates an error occurred or no data is present.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Character read from UART RX buffer. ASCII characters from 1 to 255 are valid.
    *  A returned zero signifies an error condition or no data available.
    *
    * Global Variables:
    *  UART1_rxBuffer - RAM buffer pointer for save received data.
    *  UART1_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  UART1_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  UART1_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 UART1_GetChar(void) 
    {
        uint8 rxData = 0u;
        uint8 rxStatus;

    #if (UART1_RX_INTERRUPT_ENABLED)
        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        UART1_DisableRxInt();

        locRxBufferRead  = UART1_rxBufferRead;
        locRxBufferWrite = UART1_rxBufferWrite;

        if( (UART1_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = UART1_rxBuffer[locRxBufferRead];
            locRxBufferRead++;
            if(locRxBufferRead >= UART1_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            UART1_rxBufferRead = locRxBufferRead;

            if(UART1_rxBufferLoopDetect != 0u)
            {
                UART1_rxBufferLoopDetect = 0u;
                #if( (UART1_RX_INTERRUPT_ENABLED) && (UART1_FLOW_CONTROL != 0u) )
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( UART1_HD_ENABLED )
                        if((UART1_CONTROL_REG & UART1_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only if
                            *  RX configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            UART1_RXSTATUS_MASK_REG |= UART1_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        UART1_RXSTATUS_MASK_REG |= UART1_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end UART1_HD_ENABLED */
                #endif /* UART1_RX_INTERRUPT_ENABLED and Hardware flow control*/
            }

        }
        else
        {   rxStatus = UART1_RXSTATUS_REG;
            if((rxStatus & UART1_RX_STS_FIFO_NOTEMPTY) != 0u)
            {   /* Read received data from FIFO */
                rxData = UART1_RXDATA_REG;
                /*Check status on error*/
                if((rxStatus & (UART1_RX_STS_BREAK | UART1_RX_STS_PAR_ERROR |
                                UART1_RX_STS_STOP_ERROR | UART1_RX_STS_OVERRUN)) != 0u)
                {
                    rxData = 0u;
                }
            }
        }

        UART1_EnableRxInt();

    #else

        rxStatus =UART1_RXSTATUS_REG;
        if((rxStatus & UART1_RX_STS_FIFO_NOTEMPTY) != 0u)
        {
            /* Read received data from FIFO */
            rxData = UART1_RXDATA_REG;

            /*Check status on error*/
            if((rxStatus & (UART1_RX_STS_BREAK | UART1_RX_STS_PAR_ERROR |
                            UART1_RX_STS_STOP_ERROR | UART1_RX_STS_OVERRUN)) != 0u)
            {
                rxData = 0u;
            }
        }
    #endif /* (UART1_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: UART1_GetByte
    ********************************************************************************
    *
    * Summary:
    *  Reads UART RX buffer immediately, returns received character and error
    *  condition.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  MSB contains status and LSB contains UART RX data. If the MSB is nonzero,
    *  an error has occurred.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint16 UART1_GetByte(void) 
    {
        
    #if (UART1_RX_INTERRUPT_ENABLED)
        uint16 locErrorStatus;
        /* Protect variables that could change on interrupt */
        UART1_DisableRxInt();
        locErrorStatus = (uint16)UART1_errorStatus;
        UART1_errorStatus = 0u;
        UART1_EnableRxInt();
        return ( (uint16)(locErrorStatus << 8u) | UART1_ReadRxData() );
    #else
        return ( ((uint16)UART1_ReadRxStatus() << 8u) | UART1_ReadRxData() );
    #endif /* UART1_RX_INTERRUPT_ENABLED */
        
    }


    /*******************************************************************************
    * Function Name: UART1_GetRxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of received bytes available in the RX buffer.
    *  * RX software buffer is disabled (RX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty RX FIFO or 1 for not empty RX FIFO.
    *  * RX software buffer is enabled: returns the number of bytes available in 
    *    the RX software buffer. Bytes available in the RX FIFO do not take to 
    *    account.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  uint8: Number of bytes in the RX buffer. 
    *    Return value type depends on RX Buffer Size parameter.
    *
    * Global Variables:
    *  UART1_rxBufferWrite - used to calculate left bytes.
    *  UART1_rxBufferRead - used to calculate left bytes.
    *  UART1_rxBufferLoopDetect - checked to decide left bytes amount.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the RX Buffer is.
    *
    *******************************************************************************/
    uint8 UART1_GetRxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (UART1_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt */
        UART1_DisableRxInt();

        if(UART1_rxBufferRead == UART1_rxBufferWrite)
        {
            if(UART1_rxBufferLoopDetect != 0u)
            {
                size = UART1_RX_BUFFER_SIZE;
            }
            else
            {
                size = 0u;
            }
        }
        else if(UART1_rxBufferRead < UART1_rxBufferWrite)
        {
            size = (UART1_rxBufferWrite - UART1_rxBufferRead);
        }
        else
        {
            size = (UART1_RX_BUFFER_SIZE - UART1_rxBufferRead) + UART1_rxBufferWrite;
        }

        UART1_EnableRxInt();

    #else

        /* We can only know if there is data in the fifo. */
        size = ((UART1_RXSTATUS_REG & UART1_RX_STS_FIFO_NOTEMPTY) != 0u) ? 1u : 0u;

    #endif /* (UART1_RX_INTERRUPT_ENABLED) */

        return(size);
    }


    /*******************************************************************************
    * Function Name: UART1_ClearRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears the receiver memory buffer and hardware RX FIFO of all received data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART1_rxBufferWrite - cleared to zero.
    *  UART1_rxBufferRead - cleared to zero.
    *  UART1_rxBufferLoopDetect - cleared to zero.
    *  UART1_rxBufferOverflow - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may
    *  have remained in the RAM.
    *
    * Side Effects:
    *  Any received data not read from the RAM or FIFO buffer will be lost.
    *
    *******************************************************************************/
    void UART1_ClearRxBuffer(void) 
    {
        uint8 enableInterrupts;

        /* Clear the HW FIFO */
        enableInterrupts = CyEnterCriticalSection();
        UART1_RXDATA_AUX_CTL_REG |= (uint8)  UART1_RX_FIFO_CLR;
        UART1_RXDATA_AUX_CTL_REG &= (uint8) ~UART1_RX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (UART1_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        UART1_DisableRxInt();

        UART1_rxBufferRead = 0u;
        UART1_rxBufferWrite = 0u;
        UART1_rxBufferLoopDetect = 0u;
        UART1_rxBufferOverflow = 0u;

        UART1_EnableRxInt();

    #endif /* (UART1_RX_INTERRUPT_ENABLED) */

    }


    /*******************************************************************************
    * Function Name: UART1_SetRxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the software controlled Addressing mode used by the RX portion of the
    *  UART.
    *
    * Parameters:
    *  addressMode: Enumerated value indicating the mode of RX addressing
    *  UART1__B_UART__AM_SW_BYTE_BYTE -  Software Byte-by-Byte address
    *                                               detection
    *  UART1__B_UART__AM_SW_DETECT_TO_BUFFER - Software Detect to Buffer
    *                                               address detection
    *  UART1__B_UART__AM_HW_BYTE_BY_BYTE - Hardware Byte-by-Byte address
    *                                               detection
    *  UART1__B_UART__AM_HW_DETECT_TO_BUFFER - Hardware Detect to Buffer
    *                                               address detection
    *  UART1__B_UART__AM_NONE - No address detection
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART1_rxAddressMode - the parameter stored in this variable for
    *   the farther usage in RX ISR.
    *  UART1_rxAddressDetected - set to initial state (0).
    *
    *******************************************************************************/
    void UART1_SetRxAddressMode(uint8 addressMode)
                                                        
    {
        #if(UART1_RXHW_ADDRESS_ENABLED)
            #if(UART1_CONTROL_REG_REMOVED)
                if(0u != addressMode)
                {
                    /* Suppress compiler warning */
                }
            #else /* UART1_CONTROL_REG_REMOVED */
                uint8 tmpCtrl;
                tmpCtrl = UART1_CONTROL_REG & (uint8)~UART1_CTRL_RXADDR_MODE_MASK;
                tmpCtrl |= (uint8)(addressMode << UART1_CTRL_RXADDR_MODE0_SHIFT);
                UART1_CONTROL_REG = tmpCtrl;

                #if(UART1_RX_INTERRUPT_ENABLED && \
                   (UART1_RXBUFFERSIZE > UART1_FIFO_LENGTH) )
                    UART1_rxAddressMode = addressMode;
                    UART1_rxAddressDetected = 0u;
                #endif /* End UART1_RXBUFFERSIZE > UART1_FIFO_LENGTH*/
            #endif /* End UART1_CONTROL_REG_REMOVED */
        #else /* UART1_RXHW_ADDRESS_ENABLED */
            if(0u != addressMode)
            {
                /* Suppress compiler warning */
            }
        #endif /* End UART1_RXHW_ADDRESS_ENABLED */
    }


    /*******************************************************************************
    * Function Name: UART1_SetRxAddress1
    ********************************************************************************
    *
    * Summary:
    *  Sets the first of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #1 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void UART1_SetRxAddress1(uint8 address) 
    {
        UART1_RXADDRESS1_REG = address;
    }


    /*******************************************************************************
    * Function Name: UART1_SetRxAddress2
    ********************************************************************************
    *
    * Summary:
    *  Sets the second of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #2 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void UART1_SetRxAddress2(uint8 address) 
    {
        UART1_RXADDRESS2_REG = address;
    }

#endif  /* UART1_RX_ENABLED || UART1_HD_ENABLED*/


#if( (UART1_TX_ENABLED) || (UART1_HD_ENABLED) )
    /*******************************************************************************
    * Function Name: UART1_SetTxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the TX interrupt sources to be enabled, but does not enable the
    *  interrupt.
    *
    * Parameters:
    *  intSrc: Bit field containing the TX interrupt sources to enable
    *   UART1_TX_STS_COMPLETE        Interrupt on TX byte complete
    *   UART1_TX_STS_FIFO_EMPTY      Interrupt when TX FIFO is empty
    *   UART1_TX_STS_FIFO_FULL       Interrupt when TX FIFO is full
    *   UART1_TX_STS_FIFO_NOT_FULL   Interrupt when TX FIFO is not full
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void UART1_SetTxInterruptMode(uint8 intSrc) 
    {
        UART1_TXSTATUS_MASK_REG = intSrc;
    }


    /*******************************************************************************
    * Function Name: UART1_WriteTxData
    ********************************************************************************
    *
    * Summary:
    *  Places a byte of data into the transmit buffer to be sent when the bus is
    *  available without checking the TX status register. You must check status
    *  separately.
    *
    * Parameters:
    *  txDataByte: data byte
    *
    * Return:
    * None.
    *
    * Global Variables:
    *  UART1_txBuffer - RAM buffer pointer for save data for transmission
    *  UART1_txBufferWrite - cyclic index for write to txBuffer,
    *    incremented after each byte saved to buffer.
    *  UART1_txBufferRead - cyclic index for read from txBuffer,
    *    checked to identify the condition to write to FIFO directly or to TX buffer
    *  UART1_initVar - checked to identify that the component has been
    *    initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void UART1_WriteTxData(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function*/
        if(UART1_initVar != 0u)
        {
        #if (UART1_TX_INTERRUPT_ENABLED)

            /* Protect variables that could change on interrupt. */
            UART1_DisableTxInt();

            if( (UART1_txBufferRead == UART1_txBufferWrite) &&
                ((UART1_TXSTATUS_REG & UART1_TX_STS_FIFO_FULL) == 0u) )
            {
                /* Add directly to the FIFO. */
                UART1_TXDATA_REG = txDataByte;
            }
            else
            {
                if(UART1_txBufferWrite >= UART1_TX_BUFFER_SIZE)
                {
                    UART1_txBufferWrite = 0u;
                }

                UART1_txBuffer[UART1_txBufferWrite] = txDataByte;

                /* Add to the software buffer. */
                UART1_txBufferWrite++;
            }

            UART1_EnableTxInt();

        #else

            /* Add directly to the FIFO. */
            UART1_TXDATA_REG = txDataByte;

        #endif /*(UART1_TX_INTERRUPT_ENABLED) */
        }
    }


    /*******************************************************************************
    * Function Name: UART1_ReadTxStatus
    ********************************************************************************
    *
    * Summary:
    *  Reads the status register for the TX portion of the UART.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Contents of the status register
    *
    * Theory:
    *  This function reads the TX status register, which is cleared on read.
    *  It is up to the user to handle all bits in this return value accordingly,
    *  even if the bit was not enabled as an interrupt source the event happened
    *  and must be handled accordingly.
    *
    *******************************************************************************/
    uint8 UART1_ReadTxStatus(void) 
    {
        return(UART1_TXSTATUS_REG);
    }


    /*******************************************************************************
    * Function Name: UART1_PutChar
    ********************************************************************************
    *
    * Summary:
    *  Puts a byte of data into the transmit buffer to be sent when the bus is
    *  available. This is a blocking API that waits until the TX buffer has room to
    *  hold the data.
    *
    * Parameters:
    *  txDataByte: Byte containing the data to transmit
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART1_txBuffer - RAM buffer pointer for save data for transmission
    *  UART1_txBufferWrite - cyclic index for write to txBuffer,
    *     checked to identify free space in txBuffer and incremented after each byte
    *     saved to buffer.
    *  UART1_txBufferRead - cyclic index for read from txBuffer,
    *     checked to identify free space in txBuffer.
    *  UART1_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to transmit any byte of data in a single transfer
    *
    *******************************************************************************/
    void UART1_PutChar(uint8 txDataByte) 
    {
    #if (UART1_TX_INTERRUPT_ENABLED)
        /* The temporary output pointer is used since it takes two instructions
        *  to increment with a wrap, and we can't risk doing that with the real
        *  pointer and getting an interrupt in between instructions.
        */
        uint8 locTxBufferWrite;
        uint8 locTxBufferRead;

        do
        { /* Block if software buffer is full, so we don't overwrite. */

        #if ((UART1_TX_BUFFER_SIZE > UART1_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Disable TX interrupt to protect variables from modification */
            UART1_DisableTxInt();
        #endif /* (UART1_TX_BUFFER_SIZE > UART1_MAX_BYTE_VALUE) && (CY_PSOC3) */

            locTxBufferWrite = UART1_txBufferWrite;
            locTxBufferRead  = UART1_txBufferRead;

        #if ((UART1_TX_BUFFER_SIZE > UART1_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Enable interrupt to continue transmission */
            UART1_EnableTxInt();
        #endif /* (UART1_TX_BUFFER_SIZE > UART1_MAX_BYTE_VALUE) && (CY_PSOC3) */
        }
        while( (locTxBufferWrite < locTxBufferRead) ? (locTxBufferWrite == (locTxBufferRead - 1u)) :
                                ((locTxBufferWrite - locTxBufferRead) ==
                                (uint8)(UART1_TX_BUFFER_SIZE - 1u)) );

        if( (locTxBufferRead == locTxBufferWrite) &&
            ((UART1_TXSTATUS_REG & UART1_TX_STS_FIFO_FULL) == 0u) )
        {
            /* Add directly to the FIFO */
            UART1_TXDATA_REG = txDataByte;
        }
        else
        {
            if(locTxBufferWrite >= UART1_TX_BUFFER_SIZE)
            {
                locTxBufferWrite = 0u;
            }
            /* Add to the software buffer. */
            UART1_txBuffer[locTxBufferWrite] = txDataByte;
            locTxBufferWrite++;

            /* Finally, update the real output pointer */
        #if ((UART1_TX_BUFFER_SIZE > UART1_MAX_BYTE_VALUE) && (CY_PSOC3))
            UART1_DisableTxInt();
        #endif /* (UART1_TX_BUFFER_SIZE > UART1_MAX_BYTE_VALUE) && (CY_PSOC3) */

            UART1_txBufferWrite = locTxBufferWrite;

        #if ((UART1_TX_BUFFER_SIZE > UART1_MAX_BYTE_VALUE) && (CY_PSOC3))
            UART1_EnableTxInt();
        #endif /* (UART1_TX_BUFFER_SIZE > UART1_MAX_BYTE_VALUE) && (CY_PSOC3) */

            if(0u != (UART1_TXSTATUS_REG & UART1_TX_STS_FIFO_EMPTY))
            {
                /* Trigger TX interrupt to send software buffer */
                UART1_SetPendingTxInt();
            }
        }

    #else

        while((UART1_TXSTATUS_REG & UART1_TX_STS_FIFO_FULL) != 0u)
        {
            /* Wait for room in the FIFO */
        }

        /* Add directly to the FIFO */
        UART1_TXDATA_REG = txDataByte;

    #endif /* UART1_TX_INTERRUPT_ENABLED */
    }


    /*******************************************************************************
    * Function Name: UART1_PutString
    ********************************************************************************
    *
    * Summary:
    *  Sends a NULL terminated string to the TX buffer for transmission.
    *
    * Parameters:
    *  string[]: Pointer to the null terminated string array residing in RAM or ROM
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART1_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void UART1_PutString(const char8 string[]) 
    {
        uint16 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(UART1_initVar != 0u)
        {
            /* This is a blocking function, it will not exit until all data is sent */
            while(string[bufIndex] != (char8) 0)
            {
                UART1_PutChar((uint8)string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: UART1_PutArray
    ********************************************************************************
    *
    * Summary:
    *  Places N bytes of data from a memory array into the TX buffer for
    *  transmission.
    *
    * Parameters:
    *  string[]: Address of the memory array residing in RAM or ROM.
    *  byteCount: Number of bytes to be transmitted. The type depends on TX Buffer
    *             Size parameter.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART1_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void UART1_PutArray(const uint8 string[], uint8 byteCount)
                                                                    
    {
        uint8 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(UART1_initVar != 0u)
        {
            while(bufIndex < byteCount)
            {
                UART1_PutChar(string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: UART1_PutCRLF
    ********************************************************************************
    *
    * Summary:
    *  Writes a byte of data followed by a carriage return (0x0D) and line feed
    *  (0x0A) to the transmit buffer.
    *
    * Parameters:
    *  txDataByte: Data byte to transmit before the carriage return and line feed.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART1_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void UART1_PutCRLF(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function */
        if(UART1_initVar != 0u)
        {
            UART1_PutChar(txDataByte);
            UART1_PutChar(0x0Du);
            UART1_PutChar(0x0Au);
        }
    }


    /*******************************************************************************
    * Function Name: UART1_GetTxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of bytes in the TX buffer which are waiting to be 
    *  transmitted.
    *  * TX software buffer is disabled (TX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty TX FIFO, 1 for not full TX FIFO or 4 for full TX FIFO.
    *  * TX software buffer is enabled: returns the number of bytes in the TX 
    *    software buffer which are waiting to be transmitted. Bytes available in the
    *    TX FIFO do not count.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Number of bytes used in the TX buffer. Return value type depends on the TX 
    *  Buffer Size parameter.
    *
    * Global Variables:
    *  UART1_txBufferWrite - used to calculate left space.
    *  UART1_txBufferRead - used to calculate left space.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the TX Buffer is.
    *
    *******************************************************************************/
    uint8 UART1_GetTxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (UART1_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        UART1_DisableTxInt();

        if(UART1_txBufferRead == UART1_txBufferWrite)
        {
            size = 0u;
        }
        else if(UART1_txBufferRead < UART1_txBufferWrite)
        {
            size = (UART1_txBufferWrite - UART1_txBufferRead);
        }
        else
        {
            size = (UART1_TX_BUFFER_SIZE - UART1_txBufferRead) +
                    UART1_txBufferWrite;
        }

        UART1_EnableTxInt();

    #else

        size = UART1_TXSTATUS_REG;

        /* Is the fifo is full. */
        if((size & UART1_TX_STS_FIFO_FULL) != 0u)
        {
            size = UART1_FIFO_LENGTH;
        }
        else if((size & UART1_TX_STS_FIFO_EMPTY) != 0u)
        {
            size = 0u;
        }
        else
        {
            /* We only know there is data in the fifo. */
            size = 1u;
        }

    #endif /* (UART1_TX_INTERRUPT_ENABLED) */

    return(size);
    }


    /*******************************************************************************
    * Function Name: UART1_ClearTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears all data from the TX buffer and hardware TX FIFO.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART1_txBufferWrite - cleared to zero.
    *  UART1_txBufferRead - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may have
    *  remained in the RAM.
    *
    * Side Effects:
    *  Data waiting in the transmit buffer is not sent; a byte that is currently
    *  transmitting finishes transmitting.
    *
    *******************************************************************************/
    void UART1_ClearTxBuffer(void) 
    {
        uint8 enableInterrupts;

        enableInterrupts = CyEnterCriticalSection();
        /* Clear the HW FIFO */
        UART1_TXDATA_AUX_CTL_REG |= (uint8)  UART1_TX_FIFO_CLR;
        UART1_TXDATA_AUX_CTL_REG &= (uint8) ~UART1_TX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (UART1_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        UART1_DisableTxInt();

        UART1_txBufferRead = 0u;
        UART1_txBufferWrite = 0u;

        /* Enable Tx interrupt. */
        UART1_EnableTxInt();

    #endif /* (UART1_TX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: UART1_SendBreak
    ********************************************************************************
    *
    * Summary:
    *  Transmits a break signal on the bus.
    *
    * Parameters:
    *  uint8 retMode:  Send Break return mode. See the following table for options.
    *   UART1_SEND_BREAK - Initialize registers for break, send the Break
    *       signal and return immediately.
    *   UART1_WAIT_FOR_COMPLETE_REINIT - Wait until break transmission is
    *       complete, reinitialize registers to normal transmission mode then return
    *   UART1_REINIT - Reinitialize registers to normal transmission mode
    *       then return.
    *   UART1_SEND_WAIT_REINIT - Performs both options: 
    *      UART1_SEND_BREAK and UART1_WAIT_FOR_COMPLETE_REINIT.
    *      This option is recommended for most cases.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART1_initVar - checked to identify that the component has been
    *     initialized.
    *  txPeriod - static variable, used for keeping TX period configuration.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  SendBreak function initializes registers to send 13-bit break signal. It is
    *  important to return the registers configuration to normal for continue 8-bit
    *  operation.
    *  There are 3 variants for this API usage:
    *  1) SendBreak(3) - function will send the Break signal and take care on the
    *     configuration returning. Function will block CPU until transmission
    *     complete.
    *  2) User may want to use blocking time if UART configured to the low speed
    *     operation
    *     Example for this case:
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     SendBreak(1);     - complete Break operation
    *  3) Same to 2) but user may want to initialize and use the interrupt to
    *     complete break operation.
    *     Example for this case:
    *     Initialize TX interrupt with "TX - On TX Complete" parameter
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     When interrupt appear with UART1_TX_STS_COMPLETE status:
    *     SendBreak(2);     - complete Break operation
    *
    * Side Effects:
    *  The UART1_SendBreak() function initializes registers to send a
    *  break signal.
    *  Break signal length depends on the break signal bits configuration.
    *  The register configuration should be reinitialized before normal 8-bit
    *  communication can continue.
    *
    *******************************************************************************/
    void UART1_SendBreak(uint8 retMode) 
    {

        /* If not Initialized then skip this function*/
        if(UART1_initVar != 0u)
        {
            /* Set the Counter to 13-bits and transmit a 00 byte */
            /* When that is done then reset the counter value back */
            uint8 tmpStat;

        #if(UART1_HD_ENABLED) /* Half Duplex mode*/

            if( (retMode == UART1_SEND_BREAK) ||
                (retMode == UART1_SEND_WAIT_REINIT ) )
            {
                /* CTRL_HD_SEND_BREAK - sends break bits in HD mode */
                UART1_WriteControlRegister(UART1_ReadControlRegister() |
                                                      UART1_CTRL_HD_SEND_BREAK);
                /* Send zeros */
                UART1_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = UART1_TXSTATUS_REG;
                }
                while((tmpStat & UART1_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == UART1_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == UART1_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = UART1_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & UART1_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == UART1_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == UART1_REINIT) ||
                (retMode == UART1_SEND_WAIT_REINIT) )
            {
                UART1_WriteControlRegister(UART1_ReadControlRegister() &
                                              (uint8)~UART1_CTRL_HD_SEND_BREAK);
            }

        #else /* UART1_HD_ENABLED Full Duplex mode */

            static uint8 txPeriod;

            if( (retMode == UART1_SEND_BREAK) ||
                (retMode == UART1_SEND_WAIT_REINIT) )
            {
                /* CTRL_HD_SEND_BREAK - skip to send parity bit at Break signal in Full Duplex mode */
                #if( (UART1_PARITY_TYPE != UART1__B_UART__NONE_REVB) || \
                                    (UART1_PARITY_TYPE_SW != 0u) )
                    UART1_WriteControlRegister(UART1_ReadControlRegister() |
                                                          UART1_CTRL_HD_SEND_BREAK);
                #endif /* End UART1_PARITY_TYPE != UART1__B_UART__NONE_REVB  */

                #if(UART1_TXCLKGEN_DP)
                    txPeriod = UART1_TXBITCLKTX_COMPLETE_REG;
                    UART1_TXBITCLKTX_COMPLETE_REG = UART1_TXBITCTR_BREAKBITS;
                #else
                    txPeriod = UART1_TXBITCTR_PERIOD_REG;
                    UART1_TXBITCTR_PERIOD_REG = UART1_TXBITCTR_BREAKBITS8X;
                #endif /* End UART1_TXCLKGEN_DP */

                /* Send zeros */
                UART1_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = UART1_TXSTATUS_REG;
                }
                while((tmpStat & UART1_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == UART1_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == UART1_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = UART1_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & UART1_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == UART1_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == UART1_REINIT) ||
                (retMode == UART1_SEND_WAIT_REINIT) )
            {

            #if(UART1_TXCLKGEN_DP)
                UART1_TXBITCLKTX_COMPLETE_REG = txPeriod;
            #else
                UART1_TXBITCTR_PERIOD_REG = txPeriod;
            #endif /* End UART1_TXCLKGEN_DP */

            #if( (UART1_PARITY_TYPE != UART1__B_UART__NONE_REVB) || \
                 (UART1_PARITY_TYPE_SW != 0u) )
                UART1_WriteControlRegister(UART1_ReadControlRegister() &
                                                      (uint8) ~UART1_CTRL_HD_SEND_BREAK);
            #endif /* End UART1_PARITY_TYPE != NONE */
            }
        #endif    /* End UART1_HD_ENABLED */
        }
    }


    /*******************************************************************************
    * Function Name: UART1_SetTxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the transmitter to signal the next bytes is address or data.
    *
    * Parameters:
    *  addressMode: 
    *       UART1_SET_SPACE - Configure the transmitter to send the next
    *                                    byte as a data.
    *       UART1_SET_MARK  - Configure the transmitter to send the next
    *                                    byte as an address.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  This function sets and clears UART1_CTRL_MARK bit in the Control
    *  register.
    *
    *******************************************************************************/
    void UART1_SetTxAddressMode(uint8 addressMode) 
    {
        /* Mark/Space sending enable */
        if(addressMode != 0u)
        {
        #if( UART1_CONTROL_REG_REMOVED == 0u )
            UART1_WriteControlRegister(UART1_ReadControlRegister() |
                                                  UART1_CTRL_MARK);
        #endif /* End UART1_CONTROL_REG_REMOVED == 0u */
        }
        else
        {
        #if( UART1_CONTROL_REG_REMOVED == 0u )
            UART1_WriteControlRegister(UART1_ReadControlRegister() &
                                                  (uint8) ~UART1_CTRL_MARK);
        #endif /* End UART1_CONTROL_REG_REMOVED == 0u */
        }
    }

#endif  /* EndUART1_TX_ENABLED */

#if(UART1_HD_ENABLED)


    /*******************************************************************************
    * Function Name: UART1_LoadRxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the receiver configuration in half duplex mode. After calling this
    *  function, the UART is ready to receive data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the transmitter
    *  configuration.
    *
    *******************************************************************************/
    void UART1_LoadRxConfig(void) 
    {
        UART1_WriteControlRegister(UART1_ReadControlRegister() &
                                                (uint8)~UART1_CTRL_HD_SEND);
        UART1_RXBITCTR_PERIOD_REG = UART1_HD_RXBITCTR_INIT;

    #if (UART1_RX_INTERRUPT_ENABLED)
        /* Enable RX interrupt after set RX configuration */
        UART1_SetRxInterruptMode(UART1_INIT_RX_INTERRUPTS_MASK);
    #endif /* (UART1_RX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: UART1_LoadTxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the transmitter configuration in half duplex mode. After calling this
    *  function, the UART is ready to transmit data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the receiver configuration.
    *
    *******************************************************************************/
    void UART1_LoadTxConfig(void) 
    {
    #if (UART1_RX_INTERRUPT_ENABLED)
        /* Disable RX interrupts before set TX configuration */
        UART1_SetRxInterruptMode(0u);
    #endif /* (UART1_RX_INTERRUPT_ENABLED) */

        UART1_WriteControlRegister(UART1_ReadControlRegister() | UART1_CTRL_HD_SEND);
        UART1_RXBITCTR_PERIOD_REG = UART1_HD_TXBITCTR_INIT;
    }

#endif  /* UART1_HD_ENABLED */


/* [] END OF FILE */
