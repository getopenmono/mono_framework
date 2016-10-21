/*******************************************************************************
* File Name: SPI1_PM.c
* Version 2.50
*
* Description:
*  This file contains the setup, control and status commands to support
*  component operations in low power mode.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPI1_PVT.h"

static SPI1_BACKUP_STRUCT SPI1_backup =
{
    SPI1_DISABLED,
    SPI1_BITCTR_INIT,
};


/*******************************************************************************
* Function Name: SPI1_SaveConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void SPI1_SaveConfig(void) 
{

}


/*******************************************************************************
* Function Name: SPI1_RestoreConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void SPI1_RestoreConfig(void) 
{

}


/*******************************************************************************
* Function Name: SPI1_Sleep
********************************************************************************
*
* Summary:
*  Prepare SPIM Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SPI1_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPI1_Sleep(void) 
{
    /* Save components enable state */
    SPI1_backup.enableState = ((uint8) SPI1_IS_ENABLED);

    SPI1_Stop();
}


/*******************************************************************************
* Function Name: SPI1_Wakeup
********************************************************************************
*
* Summary:
*  Prepare SPIM Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SPI1_backup - used when non-retention registers are restored.
*  SPI1_txBufferWrite - modified every function call - resets to
*  zero.
*  SPI1_txBufferRead - modified every function call - resets to
*  zero.
*  SPI1_rxBufferWrite - modified every function call - resets to
*  zero.
*  SPI1_rxBufferRead - modified every function call - resets to
*  zero.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPI1_Wakeup(void) 
{
    #if(SPI1_RX_SOFTWARE_BUF_ENABLED)
        SPI1_rxBufferFull  = 0u;
        SPI1_rxBufferRead  = 0u;
        SPI1_rxBufferWrite = 0u;
    #endif /* (SPI1_RX_SOFTWARE_BUF_ENABLED) */

    #if(SPI1_TX_SOFTWARE_BUF_ENABLED)
        SPI1_txBufferFull  = 0u;
        SPI1_txBufferRead  = 0u;
        SPI1_txBufferWrite = 0u;
    #endif /* (SPI1_TX_SOFTWARE_BUF_ENABLED) */

    /* Clear any data from the RX and TX FIFO */
    SPI1_ClearFIFO();

    /* Restore components block enable state */
    if(0u != SPI1_backup.enableState)
    {
        SPI1_Enable();
    }
}


/* [] END OF FILE */
