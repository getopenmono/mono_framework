/*******************************************************************************
* File Name: I2CS_PM.c
* Version 3.50
*
* Description:
*  This file provides low power mode APIs for the I2C component.
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "I2CS_PVT.h"

I2CS_BACKUP_STRUCT I2CS_backup =
{
    I2CS_DISABLE,

#if (I2CS_FF_IMPLEMENTED)
    I2CS_DEFAULT_XCFG,
    I2CS_DEFAULT_CFG,
    I2CS_DEFAULT_ADDR,
    LO8(I2CS_DEFAULT_DIVIDE_FACTOR),
    HI8(I2CS_DEFAULT_DIVIDE_FACTOR),
#else  /* (I2CS_UDB_IMPLEMENTED) */
    I2CS_DEFAULT_CFG,
#endif /* (I2CS_FF_IMPLEMENTED) */

#if (I2CS_TIMEOUT_ENABLED)
    I2CS_DEFAULT_TMOUT_PERIOD,
    I2CS_DEFAULT_TMOUT_INTR_MASK,
#endif /* (I2CS_TIMEOUT_ENABLED) */
};

#if ((I2CS_FF_IMPLEMENTED) && (I2CS_WAKEUP_ENABLED))
    volatile uint8 I2CS_wakeupSource;
#endif /* ((I2CS_FF_IMPLEMENTED) && (I2CS_WAKEUP_ENABLED)) */


/*******************************************************************************
* Function Name: I2CS_SaveConfig
********************************************************************************
*
* Summary:
*  The Enable wakeup from Sleep Mode selection influences this function
*  implementation:
*   Unchecked: Stores the component non-retention configuration registers.
*   Checked:   Disables the master, if it was enabled before, and enables
*              backup regulator of the I2C hardware. If a transaction intended
*              for component executes during this function call, it waits until
*              the current transaction is completed and I2C hardware is ready
*              to enter sleep mode. All subsequent I2C traffic is NAKed until
*              the device is put into sleep mode.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  I2CS_backup - The global variable used to save the component
*                            configuration and non-retention registers before
*                            entering the sleep mode.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void I2CS_SaveConfig(void) 
{
#if (I2CS_FF_IMPLEMENTED)
    #if (I2CS_WAKEUP_ENABLED)
        uint8 intState;
    #endif /* (I2CS_WAKEUP_ENABLED) */

    /* Store registers before enter low power mode */
    I2CS_backup.cfg     = I2CS_CFG_REG;
    I2CS_backup.xcfg    = I2CS_XCFG_REG;
    I2CS_backup.addr    = I2CS_ADDR_REG;
    I2CS_backup.clkDiv1 = I2CS_CLKDIV1_REG;
    I2CS_backup.clkDiv2 = I2CS_CLKDIV2_REG;

#if (I2CS_WAKEUP_ENABLED)
    /* Disable master */
    I2CS_CFG_REG &= (uint8) ~I2CS_ENABLE_MASTER;

    /* Enable backup regulator to keep block powered in low power mode */
    intState = CyEnterCriticalSection();
    I2CS_PWRSYS_CR1_REG |= I2CS_PWRSYS_CR1_I2C_REG_BACKUP;
    CyExitCriticalSection(intState);

    /* 1) Set force NACK to ignore I2C transactions;
    *  2) Wait unti I2C is ready go to Sleep; !!
    *  3) These bits are cleared on wake up.
    */
    /* Wait when block is ready for sleep */
    I2CS_XCFG_REG |= I2CS_XCFG_FORCE_NACK;
    while (0u == (I2CS_XCFG_REG & I2CS_XCFG_RDY_TO_SLEEP))
    {
    }

    /* Setup wakeup interrupt */
    I2CS_DisableInt();
    (void) CyIntSetVector(I2CS_ISR_NUMBER, &I2CS_WAKEUP_ISR);
    I2CS_wakeupSource = 0u;
    I2CS_EnableInt();
#endif /* (I2CS_WAKEUP_ENABLED) */

#else
    /* Store only address match bit */
    I2CS_backup.control = (I2CS_CFG_REG & I2CS_CTRL_ANY_ADDRESS_MASK);
#endif /* (I2CS_FF_IMPLEMENTED) */

#if (I2CS_TIMEOUT_ENABLED)
    I2CS_TimeoutSaveConfig();
#endif /* (I2CS_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: I2CS_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred method to prepare the component before device enters
*  sleep mode. The Enable wakeup from Sleep Mode selection influences this
*  function implementation:
*   Unchecked: Checks current I2C component state, saves it, and disables the
*              component by calling I2C_Stop() if it is currently enabled.
*              I2C_SaveConfig() is then called to save the component
*              non-retention configuration registers.
*   Checked:   If a transaction intended for component executes during this
*              function call, it waits until the current transaction is
*              completed. All subsequent I2C traffic intended for component
*              is NAKed until the device is put to sleep mode. The address
*              match event wakes up the device.
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
void I2CS_Sleep(void) 
{
#if (I2CS_WAKEUP_ENABLED)
    /* Do not enable block after exit low power mode if it is wakeup source */
    I2CS_backup.enableState = I2CS_DISABLE;

    #if (I2CS_TIMEOUT_ENABLED)
        I2CS_TimeoutStop();
    #endif /* (I2CS_TIMEOUT_ENABLED) */

#else
    /* Store enable state */
    I2CS_backup.enableState = (uint8) I2CS_IS_ENABLED;

    if (0u != I2CS_backup.enableState)
    {
        I2CS_Stop();
    }
#endif /* (I2CS_WAKEUP_ENABLED) */

    I2CS_SaveConfig();
}


/*******************************************************************************
* Function Name: I2CS_RestoreConfig
********************************************************************************
*
* Summary:
*  The Enable wakeup from Sleep Mode selection influences this function
*  implementation:
*   Unchecked: Restores the component non-retention configuration registers
*              to the state they were in before I2C_Sleep() or I2C_SaveConfig()
*              was called.
*   Checked:   Disables the backup regulator of the I2C hardware. Sets up the
*              regular component interrupt handler and generates the component
*              interrupt if it was wake up source to release the bus and
*              continue in-coming I2C transaction.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  I2CS_backup - The global variable used to save the component
*                            configuration and non-retention registers before
*                            exiting the sleep mode.
*
* Reentrant:
*  No.
*
* Side Effects:
*  Calling this function before I2CS_SaveConfig() or
*  I2CS_Sleep() will lead to unpredictable results.
*
*******************************************************************************/
void I2CS_RestoreConfig(void) 
{
#if (I2CS_FF_IMPLEMENTED)
    uint8 intState;

    if (I2CS_CHECK_PWRSYS_I2C_BACKUP)
    /* Low power mode was Sleep - backup regulator is enabled */
    {
        /* Enable backup regulator in active mode */
        intState = CyEnterCriticalSection();
        I2CS_PWRSYS_CR1_REG &= (uint8) ~I2CS_PWRSYS_CR1_I2C_REG_BACKUP;
        CyExitCriticalSection(intState);

        /* Restore master */
        I2CS_CFG_REG = I2CS_backup.cfg;
    }
    else
    /* Low power mode was Hibernate - backup regulator is disabled. All registers are cleared */
    {
    #if (I2CS_WAKEUP_ENABLED)
        /* Disable power to block before register restore */
        intState = CyEnterCriticalSection();
        I2CS_ACT_PWRMGR_REG  &= (uint8) ~I2CS_ACT_PWR_EN;
        I2CS_STBY_PWRMGR_REG &= (uint8) ~I2CS_STBY_PWR_EN;
        CyExitCriticalSection(intState);

        /* Enable component in I2C_Wakeup() after register restore */
        I2CS_backup.enableState = I2CS_ENABLE;
    #endif /* (I2CS_WAKEUP_ENABLED) */

        /* Restore component registers after Hibernate */
        I2CS_XCFG_REG    = I2CS_backup.xcfg;
        I2CS_CFG_REG     = I2CS_backup.cfg;
        I2CS_ADDR_REG    = I2CS_backup.addr;
        I2CS_CLKDIV1_REG = I2CS_backup.clkDiv1;
        I2CS_CLKDIV2_REG = I2CS_backup.clkDiv2;
    }

#if (I2CS_WAKEUP_ENABLED)
    I2CS_DisableInt();
    (void) CyIntSetVector(I2CS_ISR_NUMBER, &I2CS_ISR);
    if (0u != I2CS_wakeupSource)
    {
        /* Generate interrupt to process incoming transaction */
        I2CS_SetPendingInt();
    }
    I2CS_EnableInt();
#endif /* (I2CS_WAKEUP_ENABLED) */

#else
    I2CS_CFG_REG = I2CS_backup.control;
#endif /* (I2CS_FF_IMPLEMENTED) */

#if (I2CS_TIMEOUT_ENABLED)
    I2CS_TimeoutRestoreConfig();
#endif /* (I2CS_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: I2CS_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred method to prepare the component for active mode
*  operation (when device exits sleep mode). The Enable wakeup from Sleep Mode
*  selection influences this function implementation:
*   Unchecked: Restores the component non-retention configuration registers
*              by calling I2C_RestoreConfig(). If the component was enabled
*              before the I2C_Sleep() function was called, I2C_Wakeup()
*              re-enables it.
*   Checked:   Enables  master functionality if it was enabled before sleep,
*              and disables the backup regulator of the I2C hardware.
*              The incoming transaction continues as soon as the regular
*              I2C interrupt handler is set up (global interrupts has to be
*              enabled to service I2C component interrupt).
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
* Side Effects:
*  Calling this function before I2CS_SaveConfig() or
*  I2CS_Sleep() will lead to unpredictable results.
*
*******************************************************************************/
void I2CS_Wakeup(void) 
{
    I2CS_RestoreConfig();

    /* Restore component enable state */
    if (0u != I2CS_backup.enableState)
    {
        I2CS_Enable();
        I2CS_EnableInt();
    }
    else
    {
    #if (I2CS_TIMEOUT_ENABLED)
        I2CS_TimeoutEnable();
    #endif /* (I2CS_TIMEOUT_ENABLED) */
    }
}


/* [] END OF FILE */
