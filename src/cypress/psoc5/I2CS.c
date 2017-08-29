/*******************************************************************************
* File Name: I2CS.c
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

#include "I2CS_PVT.h"


/**********************************
*      System variables
**********************************/

uint8 I2CS_initVar = 0u; /* Defines if component was initialized */

volatile uint8 I2CS_state;  /* Current state of I2C FSM */


/*******************************************************************************
* Function Name: I2CS_Init
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
void I2CS_Init(void) 
{
#if (I2CS_FF_IMPLEMENTED)
    /* Configure fixed function block */
    I2CS_CFG_REG  = I2CS_DEFAULT_CFG;
    I2CS_XCFG_REG = I2CS_DEFAULT_XCFG;
    I2CS_ADDR_REG = I2CS_DEFAULT_ADDR;
    I2CS_CLKDIV1_REG = LO8(I2CS_DEFAULT_DIVIDE_FACTOR);
    I2CS_CLKDIV2_REG = HI8(I2CS_DEFAULT_DIVIDE_FACTOR);

#else
    uint8 intState;

    /* Configure control and interrupt sources */
    I2CS_CFG_REG      = I2CS_DEFAULT_CFG;
    I2CS_INT_MASK_REG = I2CS_DEFAULT_INT_MASK;

    /* Enable interrupt generation in status */
    intState = CyEnterCriticalSection();
    I2CS_INT_ENABLE_REG |= I2CS_INTR_ENABLE;
    CyExitCriticalSection(intState);

    /* Configure bit counter */
    #if (I2CS_MODE_SLAVE_ENABLED)
        I2CS_PERIOD_REG = I2CS_DEFAULT_PERIOD;
    #endif  /* (I2CS_MODE_SLAVE_ENABLED) */

    /* Configure clock generator */
    #if (I2CS_MODE_MASTER_ENABLED)
        I2CS_MCLK_PRD_REG = I2CS_DEFAULT_MCLK_PRD;
        I2CS_MCLK_CMP_REG = I2CS_DEFAULT_MCLK_CMP;
    #endif /* (I2CS_MODE_MASTER_ENABLED) */
#endif /* (I2CS_FF_IMPLEMENTED) */

#if (I2CS_TIMEOUT_ENABLED)
    I2CS_TimeoutInit();
#endif /* (I2CS_TIMEOUT_ENABLED) */

    /* Configure internal interrupt */
    CyIntDisable    (I2CS_ISR_NUMBER);
    CyIntSetPriority(I2CS_ISR_NUMBER, I2CS_ISR_PRIORITY);
    #if (I2CS_INTERN_I2C_INTR_HANDLER)
        (void) CyIntSetVector(I2CS_ISR_NUMBER, &I2CS_ISR);
    #endif /* (I2CS_INTERN_I2C_INTR_HANDLER) */

    /* Set FSM to default state */
    I2CS_state = I2CS_SM_IDLE;

#if (I2CS_MODE_SLAVE_ENABLED)
    /* Clear status and buffers index */
    I2CS_slStatus = 0u;
    I2CS_slRdBufIndex = 0u;
    I2CS_slWrBufIndex = 0u;

    /* Configure matched address */
    I2CS_SlaveSetAddress(I2CS_DEFAULT_ADDR);
#endif /* (I2CS_MODE_SLAVE_ENABLED) */

#if (I2CS_MODE_MASTER_ENABLED)
    /* Clear status and buffers index */
    I2CS_mstrStatus = 0u;
    I2CS_mstrRdBufIndex = 0u;
    I2CS_mstrWrBufIndex = 0u;
#endif /* (I2CS_MODE_MASTER_ENABLED) */
}


/*******************************************************************************
* Function Name: I2CS_Enable
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
void I2CS_Enable(void) 
{
#if (I2CS_FF_IMPLEMENTED)
    uint8 intState;

    /* Enable power to block */
    intState = CyEnterCriticalSection();
    I2CS_ACT_PWRMGR_REG  |= I2CS_ACT_PWR_EN;
    I2CS_STBY_PWRMGR_REG |= I2CS_STBY_PWR_EN;
    CyExitCriticalSection(intState);
#else
    #if (I2CS_MODE_SLAVE_ENABLED)
        /* Enable bit counter */
        uint8 intState = CyEnterCriticalSection();
        I2CS_COUNTER_AUX_CTL_REG |= I2CS_CNT7_ENABLE;
        CyExitCriticalSection(intState);
    #endif /* (I2CS_MODE_SLAVE_ENABLED) */

    /* Enable slave or master bits */
    I2CS_CFG_REG |= I2CS_ENABLE_MS;
#endif /* (I2CS_FF_IMPLEMENTED) */

#if (I2CS_TIMEOUT_ENABLED)
    I2CS_TimeoutEnable();
#endif /* (I2CS_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: I2CS_Start
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
*  I2CS_initVar - This variable is used to check the initial
*                             configuration, modified on the first
*                             function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void I2CS_Start(void) 
{
    if (0u == I2CS_initVar)
    {
        I2CS_Init();
        I2CS_initVar = 1u; /* Component initialized */
    }

    I2CS_Enable();
    I2CS_EnableInt();
}


/*******************************************************************************
* Function Name: I2CS_Stop
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
void I2CS_Stop(void) 
{
    I2CS_DisableInt();

#if (I2CS_TIMEOUT_ENABLED)
    I2CS_TimeoutStop();
#endif  /* End (I2CS_TIMEOUT_ENABLED) */

#if (I2CS_FF_IMPLEMENTED)
    {
        uint8 intState;
        uint16 blockResetCycles;

        /* Store registers effected by block disable */
        I2CS_backup.addr    = I2CS_ADDR_REG;
        I2CS_backup.clkDiv1 = I2CS_CLKDIV1_REG;
        I2CS_backup.clkDiv2 = I2CS_CLKDIV2_REG;

        /* Calculate number of cycles to reset block */
        blockResetCycles = ((uint16) ((uint16) I2CS_CLKDIV2_REG << 8u) | I2CS_CLKDIV1_REG) + 1u;

        /* Disable block */
        I2CS_CFG_REG &= (uint8) ~I2CS_CFG_EN_SLAVE;
        /* Wait for block reset before disable power */
        CyDelayCycles((uint32) blockResetCycles);

        /* Disable power to block */
        intState = CyEnterCriticalSection();
        I2CS_ACT_PWRMGR_REG  &= (uint8) ~I2CS_ACT_PWR_EN;
        I2CS_STBY_PWRMGR_REG &= (uint8) ~I2CS_STBY_PWR_EN;
        CyExitCriticalSection(intState);

        /* Enable block */
        I2CS_CFG_REG |= (uint8) I2CS_ENABLE_MS;

        /* Restore registers effected by block disable. Ticket ID#198004 */
        I2CS_ADDR_REG    = I2CS_backup.addr;
        I2CS_ADDR_REG    = I2CS_backup.addr;
        I2CS_CLKDIV1_REG = I2CS_backup.clkDiv1;
        I2CS_CLKDIV2_REG = I2CS_backup.clkDiv2;
    }
#else

    /* Disable slave or master bits */
    I2CS_CFG_REG &= (uint8) ~I2CS_ENABLE_MS;

#if (I2CS_MODE_SLAVE_ENABLED)
    {
        /* Disable bit counter */
        uint8 intState = CyEnterCriticalSection();
        I2CS_COUNTER_AUX_CTL_REG &= (uint8) ~I2CS_CNT7_ENABLE;
        CyExitCriticalSection(intState);
    }
#endif /* (I2CS_MODE_SLAVE_ENABLED) */

    /* Clear interrupt source register */
    (void) I2CS_CSR_REG;
#endif /* (I2CS_FF_IMPLEMENTED) */

    /* Disable interrupt on stop (enabled by write transaction) */
    I2CS_DISABLE_INT_ON_STOP;
    I2CS_ClearPendingInt();

    /* Reset FSM to default state */
    I2CS_state = I2CS_SM_IDLE;

    /* Clear busy statuses */
#if (I2CS_MODE_SLAVE_ENABLED)
    I2CS_slStatus &= (uint8) ~(I2CS_SSTAT_RD_BUSY | I2CS_SSTAT_WR_BUSY);
#endif /* (I2CS_MODE_SLAVE_ENABLED) */
}


/* [] END OF FILE */
