/*******************************************************************************
* File Name: UART1_PM.c
* Version 2.50
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
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


/***************************************
* Local data allocation
***************************************/

static UART1_BACKUP_STRUCT  UART1_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: UART1_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the UART1_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART1_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART1_SaveConfig(void)
{
    #if(UART1_CONTROL_REG_REMOVED == 0u)
        UART1_backup.cr = UART1_CONTROL_REG;
    #endif /* End UART1_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: UART1_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the nonretention control register except FIFO.
*  Does not restore the FIFO which is a set of nonretention registers.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART1_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling UART1_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void UART1_RestoreConfig(void)
{
    #if(UART1_CONTROL_REG_REMOVED == 0u)
        UART1_CONTROL_REG = UART1_backup.cr;
    #endif /* End UART1_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: UART1_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The UART1_Sleep() API saves the current component state. Then it
*  calls the UART1_Stop() function and calls 
*  UART1_SaveConfig() to save the hardware configuration.
*  Call the UART1_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART1_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART1_Sleep(void)
{
    #if(UART1_RX_ENABLED || UART1_HD_ENABLED)
        if((UART1_RXSTATUS_ACTL_REG  & UART1_INT_ENABLE) != 0u)
        {
            UART1_backup.enableState = 1u;
        }
        else
        {
            UART1_backup.enableState = 0u;
        }
    #else
        if((UART1_TXSTATUS_ACTL_REG  & UART1_INT_ENABLE) !=0u)
        {
            UART1_backup.enableState = 1u;
        }
        else
        {
            UART1_backup.enableState = 0u;
        }
    #endif /* End UART1_RX_ENABLED || UART1_HD_ENABLED*/

    UART1_Stop();
    UART1_SaveConfig();
}


/*******************************************************************************
* Function Name: UART1_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  UART1_Sleep() was called. The UART1_Wakeup() function
*  calls the UART1_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  UART1_Sleep() function was called, the UART1_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART1_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART1_Wakeup(void)
{
    UART1_RestoreConfig();
    #if( (UART1_RX_ENABLED) || (UART1_HD_ENABLED) )
        UART1_ClearRxBuffer();
    #endif /* End (UART1_RX_ENABLED) || (UART1_HD_ENABLED) */
    #if(UART1_TX_ENABLED || UART1_HD_ENABLED)
        UART1_ClearTxBuffer();
    #endif /* End UART1_TX_ENABLED || UART1_HD_ENABLED */

    if(UART1_backup.enableState != 0u)
    {
        UART1_Enable();
    }
}


/* [] END OF FILE */
