/*******************************************************************************
* File Name: SLAVE_I2C.c
* Version 3.50
*
* Description:
*  This file provides the source code of APIs for the I2C component.
*  The actual protocol and operation code resides in the interrupt service
*  routine file.
*
*******************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SLAVE_I2C_PVT.h"


/**********************************
*      System variables
**********************************/

uint8 SLAVE_I2C_initVar = 0u; /* Defines if component was initialized */

volatile uint8 SLAVE_I2C_state;  /* Current state of I2C FSM */


/*******************************************************************************
* Function Name: SLAVE_I2C_Init
********************************************************************************
*
* Summary:
*  Initializes I2C registers with initial values provided from customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SLAVE_I2C_Init(void) 
{
#if (SLAVE_I2C_FF_IMPLEMENTED)
    /* Configure fixed function block */
    SLAVE_I2C_CFG_REG  = SLAVE_I2C_DEFAULT_CFG;
    SLAVE_I2C_XCFG_REG = SLAVE_I2C_DEFAULT_XCFG;
    SLAVE_I2C_ADDR_REG = SLAVE_I2C_DEFAULT_ADDR;
    SLAVE_I2C_CLKDIV1_REG = LO8(SLAVE_I2C_DEFAULT_DIVIDE_FACTOR);
    SLAVE_I2C_CLKDIV2_REG = HI8(SLAVE_I2C_DEFAULT_DIVIDE_FACTOR);

#else
    uint8 intState;

    /* Configure control and interrupt sources */
    SLAVE_I2C_CFG_REG      = SLAVE_I2C_DEFAULT_CFG;
    SLAVE_I2C_INT_MASK_REG = SLAVE_I2C_DEFAULT_INT_MASK;

    /* Enable interrupt generation in status */
    intState = CyEnterCriticalSection();
    SLAVE_I2C_INT_ENABLE_REG |= SLAVE_I2C_INTR_ENABLE;
    CyExitCriticalSection(intState);

    /* Configure bit counter */
    #if (SLAVE_I2C_MODE_SLAVE_ENABLED)
        SLAVE_I2C_PERIOD_REG = SLAVE_I2C_DEFAULT_PERIOD;
    #endif  /* (SLAVE_I2C_MODE_SLAVE_ENABLED) */

    /* Configure clock generator */
    #if (SLAVE_I2C_MODE_MASTER_ENABLED)
        SLAVE_I2C_MCLK_PRD_REG = SLAVE_I2C_DEFAULT_MCLK_PRD;
        SLAVE_I2C_MCLK_CMP_REG = SLAVE_I2C_DEFAULT_MCLK_CMP;
    #endif /* (SLAVE_I2C_MODE_MASTER_ENABLED) */
#endif /* (SLAVE_I2C_FF_IMPLEMENTED) */

#if (SLAVE_I2C_TIMEOUT_ENABLED)
    SLAVE_I2C_TimeoutInit();
#endif /* (SLAVE_I2C_TIMEOUT_ENABLED) */

    /* Configure internal interrupt */
    CyIntDisable    (SLAVE_I2C_ISR_NUMBER);
    CyIntSetPriority(SLAVE_I2C_ISR_NUMBER, SLAVE_I2C_ISR_PRIORITY);
    #if (SLAVE_I2C_INTERN_I2C_INTR_HANDLER)
        (void) CyIntSetVector(SLAVE_I2C_ISR_NUMBER, &SLAVE_I2C_ISR);
    #endif /* (SLAVE_I2C_INTERN_I2C_INTR_HANDLER) */

    /* Set FSM to default state */
    SLAVE_I2C_state = SLAVE_I2C_SM_IDLE;

#if (SLAVE_I2C_MODE_SLAVE_ENABLED)
    /* Clear status and buffers index */
    SLAVE_I2C_slStatus = 0u;
    SLAVE_I2C_slRdBufIndex = 0u;
    SLAVE_I2C_slWrBufIndex = 0u;

    /* Configure matched address */
    SLAVE_I2C_SlaveSetAddress(SLAVE_I2C_DEFAULT_ADDR);
#endif /* (SLAVE_I2C_MODE_SLAVE_ENABLED) */

#if (SLAVE_I2C_MODE_MASTER_ENABLED)
    /* Clear status and buffers index */
    SLAVE_I2C_mstrStatus = 0u;
    SLAVE_I2C_mstrRdBufIndex = 0u;
    SLAVE_I2C_mstrWrBufIndex = 0u;
#endif /* (SLAVE_I2C_MODE_MASTER_ENABLED) */
}


/*******************************************************************************
* Function Name: SLAVE_I2C_Enable
********************************************************************************
*
* Summary:
*  Enables I2C operations.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  None.
*
*******************************************************************************/
void SLAVE_I2C_Enable(void) 
{
#if (SLAVE_I2C_FF_IMPLEMENTED)
    uint8 intState;

    /* Enable power to block */
    intState = CyEnterCriticalSection();
    SLAVE_I2C_ACT_PWRMGR_REG  |= SLAVE_I2C_ACT_PWR_EN;
    SLAVE_I2C_STBY_PWRMGR_REG |= SLAVE_I2C_STBY_PWR_EN;
    CyExitCriticalSection(intState);
#else
    #if (SLAVE_I2C_MODE_SLAVE_ENABLED)
        /* Enable bit counter */
        uint8 intState = CyEnterCriticalSection();
        SLAVE_I2C_COUNTER_AUX_CTL_REG |= SLAVE_I2C_CNT7_ENABLE;
        CyExitCriticalSection(intState);
    #endif /* (SLAVE_I2C_MODE_SLAVE_ENABLED) */

    /* Enable slave or master bits */
    SLAVE_I2C_CFG_REG |= SLAVE_I2C_ENABLE_MS;
#endif /* (SLAVE_I2C_FF_IMPLEMENTED) */

#if (SLAVE_I2C_TIMEOUT_ENABLED)
    SLAVE_I2C_TimeoutEnable();
#endif /* (SLAVE_I2C_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: SLAVE_I2C_Start
********************************************************************************
*
* Summary:
*  Starts the I2C hardware. Enables Active mode power template bits or clock
*  gating as appropriate. It is required to be executed before I2C bus
*  operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  This component automatically enables its interrupt.  If I2C is enabled !
*  without the interrupt enabled, it can lock up the I2C bus.
*
* Global variables:
*  SLAVE_I2C_initVar - This variable is used to check the initial
*                             configuration, modified on the first
*                             function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SLAVE_I2C_Start(void) 
{
    if (0u == SLAVE_I2C_initVar)
    {
        SLAVE_I2C_Init();
        SLAVE_I2C_initVar = 1u; /* Component initialized */
    }

    SLAVE_I2C_Enable();
    SLAVE_I2C_EnableInt();
}


/*******************************************************************************
* Function Name: SLAVE_I2C_Stop
********************************************************************************
*
* Summary:
*  Disables I2C hardware and disables I2C interrupt. Disables Active mode power
*  template bits or clock gating as appropriate.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void SLAVE_I2C_Stop(void) 
{
    SLAVE_I2C_DisableInt();

#if (SLAVE_I2C_TIMEOUT_ENABLED)
    SLAVE_I2C_TimeoutStop();
#endif  /* End (SLAVE_I2C_TIMEOUT_ENABLED) */

#if (SLAVE_I2C_FF_IMPLEMENTED)
    {
        uint8 intState;
        uint16 blockResetCycles;

        /* Store registers effected by block disable */
        SLAVE_I2C_backup.addr    = SLAVE_I2C_ADDR_REG;
        SLAVE_I2C_backup.clkDiv1 = SLAVE_I2C_CLKDIV1_REG;
        SLAVE_I2C_backup.clkDiv2 = SLAVE_I2C_CLKDIV2_REG;

        /* Calculate number of cycles to reset block */
        blockResetCycles = ((uint16) ((uint16) SLAVE_I2C_CLKDIV2_REG << 8u) | SLAVE_I2C_CLKDIV1_REG) + 1u;

        /* Disable block */
        SLAVE_I2C_CFG_REG &= (uint8) ~SLAVE_I2C_CFG_EN_SLAVE;
        /* Wait for block reset before disable power */
        CyDelayCycles((uint32) blockResetCycles);

        /* Disable power to block */
        intState = CyEnterCriticalSection();
        SLAVE_I2C_ACT_PWRMGR_REG  &= (uint8) ~SLAVE_I2C_ACT_PWR_EN;
        SLAVE_I2C_STBY_PWRMGR_REG &= (uint8) ~SLAVE_I2C_STBY_PWR_EN;
        CyExitCriticalSection(intState);

        /* Enable block */
        SLAVE_I2C_CFG_REG |= (uint8) SLAVE_I2C_ENABLE_MS;

        /* Restore registers effected by block disable. Ticket ID#198004 */
        SLAVE_I2C_ADDR_REG    = SLAVE_I2C_backup.addr;
        SLAVE_I2C_ADDR_REG    = SLAVE_I2C_backup.addr;
        SLAVE_I2C_CLKDIV1_REG = SLAVE_I2C_backup.clkDiv1;
        SLAVE_I2C_CLKDIV2_REG = SLAVE_I2C_backup.clkDiv2;
    }
#else

    /* Disable slave or master bits */
    SLAVE_I2C_CFG_REG &= (uint8) ~SLAVE_I2C_ENABLE_MS;

#if (SLAVE_I2C_MODE_SLAVE_ENABLED)
    {
        /* Disable bit counter */
        uint8 intState = CyEnterCriticalSection();
        SLAVE_I2C_COUNTER_AUX_CTL_REG &= (uint8) ~SLAVE_I2C_CNT7_ENABLE;
        CyExitCriticalSection(intState);
    }
#endif /* (SLAVE_I2C_MODE_SLAVE_ENABLED) */

    /* Clear interrupt source register */
    (void) SLAVE_I2C_CSR_REG;
#endif /* (SLAVE_I2C_FF_IMPLEMENTED) */

    /* Disable interrupt on stop (enabled by write transaction) */
    SLAVE_I2C_DISABLE_INT_ON_STOP;
    SLAVE_I2C_ClearPendingInt();

    /* Reset FSM to default state */
    SLAVE_I2C_state = SLAVE_I2C_SM_IDLE;

    /* Clear busy statuses */
#if (SLAVE_I2C_MODE_SLAVE_ENABLED)
    SLAVE_I2C_slStatus &= (uint8) ~(SLAVE_I2C_SSTAT_RD_BUSY | SLAVE_I2C_SSTAT_WR_BUSY);
#endif /* (SLAVE_I2C_MODE_SLAVE_ENABLED) */
}


/* [] END OF FILE */
