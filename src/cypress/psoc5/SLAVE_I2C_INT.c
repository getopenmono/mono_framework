/*******************************************************************************
* File Name: SLAVE_I2C_INT.c
* Version 3.50
*
* Description:
*  This file provides the source code of Interrupt Service Routine (ISR)
*  for the I2C component.
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SLAVE_I2C_PVT.h"



/*******************************************************************************
*  Place your includes, defines and code here.
********************************************************************************/
/* `#START SLAVE_I2C_ISR_intc` */

/* `#END` */


/*******************************************************************************
* Function Name: SLAVE_I2C_ISR
********************************************************************************
*
* Summary:
*  The handler for the I2C interrupt. The slave and master operations are
*  handled here.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
CY_ISR(SLAVE_I2C_ISR)
{
#if (SLAVE_I2C_MODE_SLAVE_ENABLED)
   uint8  tmp8;
#endif  /* (SLAVE_I2C_MODE_SLAVE_ENABLED) */

    uint8  tmpCsr;
    
#ifdef SLAVE_I2C_ISR_ENTRY_CALLBACK
    SLAVE_I2C_ISR_EntryCallback();
#endif /* SLAVE_I2C_ISR_ENTRY_CALLBACK */
    

#if(SLAVE_I2C_TIMEOUT_FF_ENABLED)
    if(0u != SLAVE_I2C_TimeoutGetStatus())
    {
        SLAVE_I2C_TimeoutReset();
        SLAVE_I2C_state = SLAVE_I2C_SM_EXIT_IDLE;
        /* SLAVE_I2C_CSR_REG should be cleared after reset */
    }
#endif /* (SLAVE_I2C_TIMEOUT_FF_ENABLED) */


    tmpCsr = SLAVE_I2C_CSR_REG;      /* Make copy as interrupts clear */

#if(SLAVE_I2C_MODE_MULTI_MASTER_SLAVE_ENABLED)
    if(SLAVE_I2C_CHECK_START_GEN(SLAVE_I2C_MCSR_REG))
    {
        SLAVE_I2C_CLEAR_START_GEN;

        /* Set transfer complete and error flags */
        SLAVE_I2C_mstrStatus |= (SLAVE_I2C_MSTAT_ERR_XFER |
                                        SLAVE_I2C_GET_MSTAT_CMPLT);

        /* Slave was addressed */
        SLAVE_I2C_state = SLAVE_I2C_SM_SLAVE;
    }
#endif /* (SLAVE_I2C_MODE_MULTI_MASTER_SLAVE_ENABLED) */


#if(SLAVE_I2C_MODE_MULTI_MASTER_ENABLED)
    if(SLAVE_I2C_CHECK_LOST_ARB(tmpCsr))
    {
        /* Set errors */
        SLAVE_I2C_mstrStatus |= (SLAVE_I2C_MSTAT_ERR_XFER     |
                                        SLAVE_I2C_MSTAT_ERR_ARB_LOST |
                                        SLAVE_I2C_GET_MSTAT_CMPLT);

        SLAVE_I2C_DISABLE_INT_ON_STOP; /* Interrupt on Stop is enabled by write */

        #if(SLAVE_I2C_MODE_MULTI_MASTER_SLAVE_ENABLED)
            if(SLAVE_I2C_CHECK_ADDRESS_STS(tmpCsr))
            {
                /* Slave was addressed */
                SLAVE_I2C_state = SLAVE_I2C_SM_SLAVE;
            }
            else
            {
                SLAVE_I2C_BUS_RELEASE;

                SLAVE_I2C_state = SLAVE_I2C_SM_EXIT_IDLE;
            }
        #else
            SLAVE_I2C_BUS_RELEASE;

            SLAVE_I2C_state = SLAVE_I2C_SM_EXIT_IDLE;

        #endif /* (SLAVE_I2C_MODE_MULTI_MASTER_SLAVE_ENABLED) */
    }
#endif /* (SLAVE_I2C_MODE_MULTI_MASTER_ENABLED) */

    /* Check for master operation mode */
    if(SLAVE_I2C_CHECK_SM_MASTER)
    {
    #if(SLAVE_I2C_MODE_MASTER_ENABLED)
        if(SLAVE_I2C_CHECK_BYTE_COMPLETE(tmpCsr))
        {
            switch (SLAVE_I2C_state)
            {
            case SLAVE_I2C_SM_MSTR_WR_ADDR:  /* After address is sent, write data */
            case SLAVE_I2C_SM_MSTR_RD_ADDR:  /* After address is sent, read data */

                tmpCsr &= ((uint8) ~SLAVE_I2C_CSR_STOP_STATUS); /* Clear Stop bit history on address phase */

                if(SLAVE_I2C_CHECK_ADDR_ACK(tmpCsr))
                {
                    /* Setup for transmit or receive of data */
                    if(SLAVE_I2C_state == SLAVE_I2C_SM_MSTR_WR_ADDR)   /* TRANSMIT data */
                    {
                        /* Check if at least one byte to transfer */
                        if(SLAVE_I2C_mstrWrBufSize > 0u)
                        {
                            /* Load the 1st data byte */
                            SLAVE_I2C_DATA_REG = SLAVE_I2C_mstrWrBufPtr[0u];
                            SLAVE_I2C_TRANSMIT_DATA;
                            SLAVE_I2C_mstrWrBufIndex = 1u;   /* Set index to 2nd element */

                            /* Set transmit state until done */
                            SLAVE_I2C_state = SLAVE_I2C_SM_MSTR_WR_DATA;
                        }
                        /* End of buffer: complete writing */
                        else if(SLAVE_I2C_CHECK_NO_STOP(SLAVE_I2C_mstrControl))
                        {
                            /* Set write complete and master halted */
                            SLAVE_I2C_mstrStatus |= (SLAVE_I2C_MSTAT_XFER_HALT |
                                                            SLAVE_I2C_MSTAT_WR_CMPLT);

                            SLAVE_I2C_state = SLAVE_I2C_SM_MSTR_HALT; /* Expect ReStart */
                            SLAVE_I2C_DisableInt();
                        }
                        else
                        {
                            SLAVE_I2C_ENABLE_INT_ON_STOP; /* Enable interrupt on Stop, to catch it */
                            SLAVE_I2C_GENERATE_STOP;
                        }
                    }
                    else  /* Master receive data */
                    {
                        SLAVE_I2C_READY_TO_READ; /* Release bus to read data */

                        SLAVE_I2C_state  = SLAVE_I2C_SM_MSTR_RD_DATA;
                    }
                }
                /* Address is NACKed */
                else if(SLAVE_I2C_CHECK_ADDR_NAK(tmpCsr))
                {
                    /* Set Address NAK error */
                    SLAVE_I2C_mstrStatus |= (SLAVE_I2C_MSTAT_ERR_XFER |
                                                    SLAVE_I2C_MSTAT_ERR_ADDR_NAK);

                    if(SLAVE_I2C_CHECK_NO_STOP(SLAVE_I2C_mstrControl))
                    {
                        SLAVE_I2C_mstrStatus |= (SLAVE_I2C_MSTAT_XFER_HALT |
                                                        SLAVE_I2C_GET_MSTAT_CMPLT);

                        SLAVE_I2C_state = SLAVE_I2C_SM_MSTR_HALT; /* Expect RESTART */
                        SLAVE_I2C_DisableInt();
                    }
                    else  /* Do normal Stop */
                    {
                        SLAVE_I2C_ENABLE_INT_ON_STOP; /* Enable interrupt on Stop, to catch it */
                        SLAVE_I2C_GENERATE_STOP;
                    }
                }
                else
                {
                    /* Address phase is not set for some reason: error */
                    #if(SLAVE_I2C_TIMEOUT_ENABLED)
                        /* Exit interrupt to take chance for timeout timer to handle this case */
                        SLAVE_I2C_DisableInt();
                        SLAVE_I2C_ClearPendingInt();
                    #else
                        /* Block execution flow: unexpected condition */
                        CYASSERT(0u != 0u);
                    #endif /* (SLAVE_I2C_TIMEOUT_ENABLED) */
                }
                break;

            case SLAVE_I2C_SM_MSTR_WR_DATA:

                if(SLAVE_I2C_CHECK_DATA_ACK(tmpCsr))
                {
                    /* Check if end of buffer */
                    if(SLAVE_I2C_mstrWrBufIndex  < SLAVE_I2C_mstrWrBufSize)
                    {
                        SLAVE_I2C_DATA_REG =
                                                 SLAVE_I2C_mstrWrBufPtr[SLAVE_I2C_mstrWrBufIndex];
                        SLAVE_I2C_TRANSMIT_DATA;
                        SLAVE_I2C_mstrWrBufIndex++;
                    }
                    /* End of buffer: complete writing */
                    else if(SLAVE_I2C_CHECK_NO_STOP(SLAVE_I2C_mstrControl))
                    {
                        /* Set write complete and master halted */
                        SLAVE_I2C_mstrStatus |= (SLAVE_I2C_MSTAT_XFER_HALT |
                                                        SLAVE_I2C_MSTAT_WR_CMPLT);

                        SLAVE_I2C_state = SLAVE_I2C_SM_MSTR_HALT;    /* Expect restart */
                        SLAVE_I2C_DisableInt();
                    }
                    else  /* Do normal Stop */
                    {
                        SLAVE_I2C_ENABLE_INT_ON_STOP;    /* Enable interrupt on Stop, to catch it */
                        SLAVE_I2C_GENERATE_STOP;
                    }
                }
                /* Last byte NAKed: end writing */
                else if(SLAVE_I2C_CHECK_NO_STOP(SLAVE_I2C_mstrControl))
                {
                    /* Set write complete, short transfer and master halted */
                    SLAVE_I2C_mstrStatus |= (SLAVE_I2C_MSTAT_ERR_XFER       |
                                                    SLAVE_I2C_MSTAT_ERR_SHORT_XFER |
                                                    SLAVE_I2C_MSTAT_XFER_HALT      |
                                                    SLAVE_I2C_MSTAT_WR_CMPLT);

                    SLAVE_I2C_state = SLAVE_I2C_SM_MSTR_HALT;    /* Expect ReStart */
                    SLAVE_I2C_DisableInt();
                }
                else  /* Do normal Stop */
                {
                    SLAVE_I2C_ENABLE_INT_ON_STOP;    /* Enable interrupt on Stop, to catch it */
                    SLAVE_I2C_GENERATE_STOP;

                    /* Set short transfer and error flag */
                    SLAVE_I2C_mstrStatus |= (SLAVE_I2C_MSTAT_ERR_SHORT_XFER |
                                                    SLAVE_I2C_MSTAT_ERR_XFER);
                }

                break;

            case SLAVE_I2C_SM_MSTR_RD_DATA:

                SLAVE_I2C_mstrRdBufPtr[SLAVE_I2C_mstrRdBufIndex] = SLAVE_I2C_DATA_REG;
                SLAVE_I2C_mstrRdBufIndex++;

                /* Check if end of buffer */
                if(SLAVE_I2C_mstrRdBufIndex < SLAVE_I2C_mstrRdBufSize)
                {
                    SLAVE_I2C_ACK_AND_RECEIVE;       /* ACK and receive byte */
                }
                /* End of buffer: complete reading */
                else if(SLAVE_I2C_CHECK_NO_STOP(SLAVE_I2C_mstrControl))
                {
                    /* Set read complete and master halted */
                    SLAVE_I2C_mstrStatus |= (SLAVE_I2C_MSTAT_XFER_HALT |
                                                    SLAVE_I2C_MSTAT_RD_CMPLT);

                    SLAVE_I2C_state = SLAVE_I2C_SM_MSTR_HALT;    /* Expect ReStart */
                    SLAVE_I2C_DisableInt();
                }
                else
                {
                    SLAVE_I2C_ENABLE_INT_ON_STOP;
                    SLAVE_I2C_NAK_AND_RECEIVE;       /* NACK and TRY to generate Stop */
                }
                break;

            default: /* This is an invalid state and should not occur */

            #if(SLAVE_I2C_TIMEOUT_ENABLED)
                /* Exit interrupt to take chance for timeout timer to handles this case */
                SLAVE_I2C_DisableInt();
                SLAVE_I2C_ClearPendingInt();
            #else
                /* Block execution flow: unexpected condition */
                CYASSERT(0u != 0u);
            #endif /* (SLAVE_I2C_TIMEOUT_ENABLED) */

                break;
            }
        }

        /* Catches Stop: end of transaction */
        if(SLAVE_I2C_CHECK_STOP_STS(tmpCsr))
        {
            SLAVE_I2C_mstrStatus |= SLAVE_I2C_GET_MSTAT_CMPLT;

            SLAVE_I2C_DISABLE_INT_ON_STOP;
            SLAVE_I2C_state = SLAVE_I2C_SM_IDLE;
        }
    #endif /* (SLAVE_I2C_MODE_MASTER_ENABLED) */
    }
    else if(SLAVE_I2C_CHECK_SM_SLAVE)
    {
    #if(SLAVE_I2C_MODE_SLAVE_ENABLED)

        if((SLAVE_I2C_CHECK_STOP_STS(tmpCsr)) || /* Stop || Restart */
           (SLAVE_I2C_CHECK_BYTE_COMPLETE(tmpCsr) && SLAVE_I2C_CHECK_ADDRESS_STS(tmpCsr)))
        {
            /* Catch end of master write transaction: use interrupt on Stop */
            /* The Stop bit history on address phase does not have correct state */
            if(SLAVE_I2C_SM_SL_WR_DATA == SLAVE_I2C_state)
            {
                SLAVE_I2C_DISABLE_INT_ON_STOP;

                SLAVE_I2C_slStatus &= ((uint8) ~SLAVE_I2C_SSTAT_WR_BUSY);
                SLAVE_I2C_slStatus |= ((uint8)  SLAVE_I2C_SSTAT_WR_CMPLT);

                SLAVE_I2C_state = SLAVE_I2C_SM_IDLE;
            }
        }

        if(SLAVE_I2C_CHECK_BYTE_COMPLETE(tmpCsr))
        {
            /* The address only issued after Start or ReStart: so check the address
               to catch these events:
                FF : sets an address phase with a byte_complete interrupt trigger.
                UDB: sets an address phase immediately after Start or ReStart. */
            if(SLAVE_I2C_CHECK_ADDRESS_STS(tmpCsr))
            {
            /* Check for software address detection */
            #if(SLAVE_I2C_SW_ADRR_DECODE)
                tmp8 = SLAVE_I2C_GET_SLAVE_ADDR(SLAVE_I2C_DATA_REG);

                if(tmp8 == SLAVE_I2C_slAddress)   /* Check for address match */
                {
                    if(0u != (SLAVE_I2C_DATA_REG & SLAVE_I2C_READ_FLAG))
                    {
                        /* Place code to prepare read buffer here                  */
                        /* `#START SLAVE_I2C_SW_PREPARE_READ_BUF_interrupt` */

                        /* `#END` */

                    #ifdef SLAVE_I2C_SW_PREPARE_READ_BUF_CALLBACK
                        SLAVE_I2C_SwPrepareReadBuf_Callback();
                    #endif /* SLAVE_I2C_SW_PREPARE_READ_BUF_CALLBACK */
                        
                        /* Prepare next operation to read, get data and place in data register */
                        if(SLAVE_I2C_slRdBufIndex < SLAVE_I2C_slRdBufSize)
                        {
                            /* Load first data byte from array */
                            SLAVE_I2C_DATA_REG = SLAVE_I2C_slRdBufPtr[SLAVE_I2C_slRdBufIndex];
                            SLAVE_I2C_ACK_AND_TRANSMIT;
                            SLAVE_I2C_slRdBufIndex++;

                            SLAVE_I2C_slStatus |= SLAVE_I2C_SSTAT_RD_BUSY;
                        }
                        else    /* Overflow: provide 0xFF on bus */
                        {
                            SLAVE_I2C_DATA_REG = SLAVE_I2C_OVERFLOW_RETURN;
                            SLAVE_I2C_ACK_AND_TRANSMIT;

                            SLAVE_I2C_slStatus  |= (SLAVE_I2C_SSTAT_RD_BUSY |
                                                           SLAVE_I2C_SSTAT_RD_ERR_OVFL);
                        }

                        SLAVE_I2C_state = SLAVE_I2C_SM_SL_RD_DATA;
                    }
                    else  /* Write transaction: receive 1st byte */
                    {
                        SLAVE_I2C_ACK_AND_RECEIVE;
                        SLAVE_I2C_state = SLAVE_I2C_SM_SL_WR_DATA;

                        SLAVE_I2C_slStatus |= SLAVE_I2C_SSTAT_WR_BUSY;
                        SLAVE_I2C_ENABLE_INT_ON_STOP;
                    }
                }
                else
                {
                    /*     Place code to compare for additional address here    */
                    /* `#START SLAVE_I2C_SW_ADDR_COMPARE_interruptStart` */

                    /* `#END` */

                #ifdef SLAVE_I2C_SW_ADDR_COMPARE_ENTRY_CALLBACK
                    SLAVE_I2C_SwAddrCompare_EntryCallback();
                #endif /* SLAVE_I2C_SW_ADDR_COMPARE_ENTRY_CALLBACK */
                    
                    SLAVE_I2C_NAK_AND_RECEIVE;   /* NACK address */

                    /* Place code to end of condition for NACK generation here */
                    /* `#START SLAVE_I2C_SW_ADDR_COMPARE_interruptEnd`  */

                    /* `#END` */

                #ifdef SLAVE_I2C_SW_ADDR_COMPARE_EXIT_CALLBACK
                    SLAVE_I2C_SwAddrCompare_ExitCallback();
                #endif /* SLAVE_I2C_SW_ADDR_COMPARE_EXIT_CALLBACK */
                }

            #else /* (SLAVE_I2C_HW_ADRR_DECODE) */

                if(0u != (SLAVE_I2C_DATA_REG & SLAVE_I2C_READ_FLAG))
                {
                    /* Place code to prepare read buffer here                  */
                    /* `#START SLAVE_I2C_HW_PREPARE_READ_BUF_interrupt` */

                    /* `#END` */
                    
                #ifdef SLAVE_I2C_HW_PREPARE_READ_BUF_CALLBACK
                    SLAVE_I2C_HwPrepareReadBuf_Callback();
                #endif /* SLAVE_I2C_HW_PREPARE_READ_BUF_CALLBACK */

                    /* Prepare next operation to read, get data and place in data register */
                    if(SLAVE_I2C_slRdBufIndex < SLAVE_I2C_slRdBufSize)
                    {
                        /* Load first data byte from array */
                        SLAVE_I2C_DATA_REG = SLAVE_I2C_slRdBufPtr[SLAVE_I2C_slRdBufIndex];
                        SLAVE_I2C_ACK_AND_TRANSMIT;
                        SLAVE_I2C_slRdBufIndex++;

                        SLAVE_I2C_slStatus |= SLAVE_I2C_SSTAT_RD_BUSY;
                    }
                    else    /* Overflow: provide 0xFF on bus */
                    {
                        SLAVE_I2C_DATA_REG = SLAVE_I2C_OVERFLOW_RETURN;
                        SLAVE_I2C_ACK_AND_TRANSMIT;

                        SLAVE_I2C_slStatus  |= (SLAVE_I2C_SSTAT_RD_BUSY |
                                                       SLAVE_I2C_SSTAT_RD_ERR_OVFL);
                    }

                    SLAVE_I2C_state = SLAVE_I2C_SM_SL_RD_DATA;
                }
                else  /* Write transaction: receive 1st byte */
                {
                    SLAVE_I2C_ACK_AND_RECEIVE;
                    SLAVE_I2C_state = SLAVE_I2C_SM_SL_WR_DATA;

                    SLAVE_I2C_slStatus |= SLAVE_I2C_SSTAT_WR_BUSY;
                    SLAVE_I2C_ENABLE_INT_ON_STOP;
                }

            #endif /* (SLAVE_I2C_SW_ADRR_DECODE) */
            }
            /* Data states */
            /* Data master writes into slave */
            else if(SLAVE_I2C_state == SLAVE_I2C_SM_SL_WR_DATA)
            {
                if(SLAVE_I2C_slWrBufIndex < SLAVE_I2C_slWrBufSize)
                {
                    tmp8 = SLAVE_I2C_DATA_REG;
                    SLAVE_I2C_ACK_AND_RECEIVE;
                    SLAVE_I2C_slWrBufPtr[SLAVE_I2C_slWrBufIndex] = tmp8;
                    SLAVE_I2C_slWrBufIndex++;
                }
                else  /* of array: complete write, send NACK */
                {
                    SLAVE_I2C_NAK_AND_RECEIVE;

                    SLAVE_I2C_slStatus |= SLAVE_I2C_SSTAT_WR_ERR_OVFL;
                }
            }
            /* Data master reads from slave */
            else if(SLAVE_I2C_state == SLAVE_I2C_SM_SL_RD_DATA)
            {
                if(SLAVE_I2C_CHECK_DATA_ACK(tmpCsr))
                {
                    if(SLAVE_I2C_slRdBufIndex < SLAVE_I2C_slRdBufSize)
                    {
                         /* Get data from array */
                        SLAVE_I2C_DATA_REG = SLAVE_I2C_slRdBufPtr[SLAVE_I2C_slRdBufIndex];
                        SLAVE_I2C_TRANSMIT_DATA;
                        SLAVE_I2C_slRdBufIndex++;
                    }
                    else   /* Overflow: provide 0xFF on bus */
                    {
                        SLAVE_I2C_DATA_REG = SLAVE_I2C_OVERFLOW_RETURN;
                        SLAVE_I2C_TRANSMIT_DATA;

                        SLAVE_I2C_slStatus |= SLAVE_I2C_SSTAT_RD_ERR_OVFL;
                    }
                }
                else  /* Last byte was NACKed: read complete */
                {
                    /* Only NACK appears on bus */
                    SLAVE_I2C_DATA_REG = SLAVE_I2C_OVERFLOW_RETURN;
                    SLAVE_I2C_NAK_AND_TRANSMIT;

                    SLAVE_I2C_slStatus &= ((uint8) ~SLAVE_I2C_SSTAT_RD_BUSY);
                    SLAVE_I2C_slStatus |= ((uint8)  SLAVE_I2C_SSTAT_RD_CMPLT);

                    SLAVE_I2C_state = SLAVE_I2C_SM_IDLE;
                }
            }
            else
            {
            #if(SLAVE_I2C_TIMEOUT_ENABLED)
                /* Exit interrupt to take chance for timeout timer to handle this case */
                SLAVE_I2C_DisableInt();
                SLAVE_I2C_ClearPendingInt();
            #else
                /* Block execution flow: unexpected condition */
                CYASSERT(0u != 0u);
            #endif /* (SLAVE_I2C_TIMEOUT_ENABLED) */
            }
        }
    #endif /* (SLAVE_I2C_MODE_SLAVE_ENABLED) */
    }
    else
    {
        /* The FSM skips master and slave processing: return to IDLE */
        SLAVE_I2C_state = SLAVE_I2C_SM_IDLE;
    }

#ifdef SLAVE_I2C_ISR_EXIT_CALLBACK
    SLAVE_I2C_ISR_ExitCallback();
#endif /* SLAVE_I2C_ISR_EXIT_CALLBACK */    
}


#if ((SLAVE_I2C_FF_IMPLEMENTED) && (SLAVE_I2C_WAKEUP_ENABLED))
    /*******************************************************************************
    * Function Name: SLAVE_I2C_WAKEUP_ISR
    ********************************************************************************
    *
    * Summary:
    *  The interrupt handler to trigger after a wakeup.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    CY_ISR(SLAVE_I2C_WAKEUP_ISR)
    {
    #ifdef SLAVE_I2C_WAKEUP_ISR_ENTRY_CALLBACK
        SLAVE_I2C_WAKEUP_ISR_EntryCallback();
    #endif /* SLAVE_I2C_WAKEUP_ISR_ENTRY_CALLBACK */
         
        /* Set flag to notify that matched address is received */
        SLAVE_I2C_wakeupSource = 1u;

        /* SCL is stretched until the I2C_Wake() is called */

    #ifdef SLAVE_I2C_WAKEUP_ISR_EXIT_CALLBACK
        SLAVE_I2C_WAKEUP_ISR_ExitCallback();
    #endif /* SLAVE_I2C_WAKEUP_ISR_EXIT_CALLBACK */
    }
#endif /* ((SLAVE_I2C_FF_IMPLEMENTED) && (SLAVE_I2C_WAKEUP_ENABLED)) */


/* [] END OF FILE */
