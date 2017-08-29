/*******************************************************************************
* File Name: I2CM.c
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

#include "I2CM_PVT.h"


/**********************************
*      System variables
**********************************/

uint8 I2CM_initVar = 0u; /* Defines if component was initialized */

volatile uint8 I2CM_state;  /* Current state of I2C FSM */


/*******************************************************************************
* Function Name: I2CM_Init
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
void I2CM_Init(void) 
{
#if (I2CM_FF_IMPLEMENTED)
    /* Configure fixed function block */
    I2CM_CFG_REG  = I2CM_DEFAULT_CFG;
    I2CM_XCFG_REG = I2CM_DEFAULT_XCFG;
    I2CM_ADDR_REG = I2CM_DEFAULT_ADDR;
    I2CM_CLKDIV1_REG = LO8(I2CM_DEFAULT_DIVIDE_FACTOR);
    I2CM_CLKDIV2_REG = HI8(I2CM_DEFAULT_DIVIDE_FACTOR);

#else
    uint8 intState;

    /* Configure control and interrupt sources */
    I2CM_CFG_REG      = I2CM_DEFAULT_CFG;
    I2CM_INT_MASK_REG = I2CM_DEFAULT_INT_MASK;

    /* Enable interrupt generation in status */
    intState = CyEnterCriticalSection();
    I2CM_INT_ENABLE_REG |= I2CM_INTR_ENABLE;
    CyExitCriticalSection(intState);

    /* Configure bit counter */
    #if (I2CM_MODE_SLAVE_ENABLED)
        I2CM_PERIOD_REG = I2CM_DEFAULT_PERIOD;
    #endif  /* (I2CM_MODE_SLAVE_ENABLED) */

    /* Configure clock generator */
    #if (I2CM_MODE_MASTER_ENABLED)
        I2CM_MCLK_PRD_REG = I2CM_DEFAULT_MCLK_PRD;
        I2CM_MCLK_CMP_REG = I2CM_DEFAULT_MCLK_CMP;
    #endif /* (I2CM_MODE_MASTER_ENABLED) */
#endif /* (I2CM_FF_IMPLEMENTED) */

#if (I2CM_TIMEOUT_ENABLED)
    I2CM_TimeoutInit();
#endif /* (I2CM_TIMEOUT_ENABLED) */

    /* Configure internal interrupt */
    CyIntDisable    (I2CM_ISR_NUMBER);
    CyIntSetPriority(I2CM_ISR_NUMBER, I2CM_ISR_PRIORITY);
    #if (I2CM_INTERN_I2C_INTR_HANDLER)
        (void) CyIntSetVector(I2CM_ISR_NUMBER, &I2CM_ISR);
    #endif /* (I2CM_INTERN_I2C_INTR_HANDLER) */

    /* Set FSM to default state */
    I2CM_state = I2CM_SM_IDLE;

#if (I2CM_MODE_SLAVE_ENABLED)
    /* Clear status and buffers index */
    I2CM_slStatus = 0u;
    I2CM_slRdBufIndex = 0u;
    I2CM_slWrBufIndex = 0u;

    /* Configure matched address */
    I2CM_SlaveSetAddress(I2CM_DEFAULT_ADDR);
#endif /* (I2CM_MODE_SLAVE_ENABLED) */

#if (I2CM_MODE_MASTER_ENABLED)
    /* Clear status and buffers index */
    I2CM_mstrStatus = 0u;
    I2CM_mstrRdBufIndex = 0u;
    I2CM_mstrWrBufIndex = 0u;
#endif /* (I2CM_MODE_MASTER_ENABLED) */
}


/*******************************************************************************
* Function Name: I2CM_Enable
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
void I2CM_Enable(void) 
{
#if (I2CM_FF_IMPLEMENTED)
    uint8 intState;

    /* Enable power to block */
    intState = CyEnterCriticalSection();
    I2CM_ACT_PWRMGR_REG  |= I2CM_ACT_PWR_EN;
    I2CM_STBY_PWRMGR_REG |= I2CM_STBY_PWR_EN;
    CyExitCriticalSection(intState);
#else
    #if (I2CM_MODE_SLAVE_ENABLED)
        /* Enable bit counter */
        uint8 intState = CyEnterCriticalSection();
        I2CM_COUNTER_AUX_CTL_REG |= I2CM_CNT7_ENABLE;
        CyExitCriticalSection(intState);
    #endif /* (I2CM_MODE_SLAVE_ENABLED) */

    /* Enable slave or master bits */
    I2CM_CFG_REG |= I2CM_ENABLE_MS;
#endif /* (I2CM_FF_IMPLEMENTED) */

#if (I2CM_TIMEOUT_ENABLED)
    I2CM_TimeoutEnable();
#endif /* (I2CM_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: I2CM_Start
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
*  I2CM_initVar - This variable is used to check the initial
*                             configuration, modified on the first
*                             function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void I2CM_Start(void) 
{
    if (0u == I2CM_initVar)
    {
        I2CM_Init();
        I2CM_initVar = 1u; /* Component initialized */
    }

    I2CM_Enable();
    I2CM_EnableInt();
}


/*******************************************************************************
* Function Name: I2CM_Stop
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
void I2CM_Stop(void) 
{
    I2CM_DisableInt();

#if (I2CM_TIMEOUT_ENABLED)
    I2CM_TimeoutStop();
#endif  /* End (I2CM_TIMEOUT_ENABLED) */

#if (I2CM_FF_IMPLEMENTED)
    {
        uint8 intState;
        uint16 blockResetCycles;

        /* Store registers effected by block disable */
        I2CM_backup.addr    = I2CM_ADDR_REG;
        I2CM_backup.clkDiv1 = I2CM_CLKDIV1_REG;
        I2CM_backup.clkDiv2 = I2CM_CLKDIV2_REG;

        /* Calculate number of cycles to reset block */
        blockResetCycles = ((uint16) ((uint16) I2CM_CLKDIV2_REG << 8u) | I2CM_CLKDIV1_REG) + 1u;

        /* Disable block */
        I2CM_CFG_REG &= (uint8) ~I2CM_CFG_EN_SLAVE;
        /* Wait for block reset before disable power */
        CyDelayCycles((uint32) blockResetCycles);

        /* Disable power to block */
        intState = CyEnterCriticalSection();
        I2CM_ACT_PWRMGR_REG  &= (uint8) ~I2CM_ACT_PWR_EN;
        I2CM_STBY_PWRMGR_REG &= (uint8) ~I2CM_STBY_PWR_EN;
        CyExitCriticalSection(intState);

        /* Enable block */
        I2CM_CFG_REG |= (uint8) I2CM_ENABLE_MS;

        /* Restore registers effected by block disable. Ticket ID#198004 */
        I2CM_ADDR_REG    = I2CM_backup.addr;
        I2CM_ADDR_REG    = I2CM_backup.addr;
        I2CM_CLKDIV1_REG = I2CM_backup.clkDiv1;
        I2CM_CLKDIV2_REG = I2CM_backup.clkDiv2;
    }
#else

    /* Disable slave or master bits */
    I2CM_CFG_REG &= (uint8) ~I2CM_ENABLE_MS;

#if (I2CM_MODE_SLAVE_ENABLED)
    {
        /* Disable bit counter */
        uint8 intState = CyEnterCriticalSection();
        I2CM_COUNTER_AUX_CTL_REG &= (uint8) ~I2CM_CNT7_ENABLE;
        CyExitCriticalSection(intState);
    }
#endif /* (I2CM_MODE_SLAVE_ENABLED) */

    /* Clear interrupt source register */
    (void) I2CM_CSR_REG;
#endif /* (I2CM_FF_IMPLEMENTED) */

    /* Disable interrupt on stop (enabled by write transaction) */
    I2CM_DISABLE_INT_ON_STOP;
    I2CM_ClearPendingInt();

    /* Reset FSM to default state */
    I2CM_state = I2CM_SM_IDLE;

    /* Clear busy statuses */
#if (I2CM_MODE_SLAVE_ENABLED)
    I2CM_slStatus &= (uint8) ~(I2CM_SSTAT_RD_BUSY | I2CM_SSTAT_WR_BUSY);
#endif /* (I2CM_MODE_SLAVE_ENABLED) */
}


/* [] END OF FILE */
