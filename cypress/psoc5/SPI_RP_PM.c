/*******************************************************************************
* File Name: SPI_RP_PM.c
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

#include "SPI_RP_PVT.h"

static SPI_RP_BACKUP_STRUCT SPI_RP_backup =
{
    SPI_RP_DISABLED,
    SPI_RP_BITCTR_INIT,
};


/*******************************************************************************
* Function Name: SPI_RP_SaveConfig
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
void SPI_RP_SaveConfig(void) 
{

}


/*******************************************************************************
* Function Name: SPI_RP_RestoreConfig
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
void SPI_RP_RestoreConfig(void) 
{

}


/*******************************************************************************
* Function Name: SPI_RP_Sleep
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
*  SPI_RP_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPI_RP_Sleep(void) 
{
    /* Save components enable state */
    SPI_RP_backup.enableState = ((uint8) SPI_RP_IS_ENABLED);

    SPI_RP_Stop();
}


/*******************************************************************************
* Function Name: SPI_RP_Wakeup
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
*  SPI_RP_backup - used when non-retention registers are restored.
*  SPI_RP_txBufferWrite - modified every function call - resets to
*  zero.
*  SPI_RP_txBufferRead - modified every function call - resets to
*  zero.
*  SPI_RP_rxBufferWrite - modified every function call - resets to
*  zero.
*  SPI_RP_rxBufferRead - modified every function call - resets to
*  zero.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPI_RP_Wakeup(void) 
{
    #if(SPI_RP_RX_SOFTWARE_BUF_ENABLED)
        SPI_RP_rxBufferFull  = 0u;
        SPI_RP_rxBufferRead  = 0u;
        SPI_RP_rxBufferWrite = 0u;
    #endif /* (SPI_RP_RX_SOFTWARE_BUF_ENABLED) */

    #if(SPI_RP_TX_SOFTWARE_BUF_ENABLED)
        SPI_RP_txBufferFull  = 0u;
        SPI_RP_txBufferRead  = 0u;
        SPI_RP_txBufferWrite = 0u;
    #endif /* (SPI_RP_TX_SOFTWARE_BUF_ENABLED) */

    /* Clear any data from the RX and TX FIFO */
    SPI_RP_ClearFIFO();

    /* Restore components block enable state */
    if(0u != SPI_RP_backup.enableState)
    {
        SPI_RP_Enable();
    }
}


/* [] END OF FILE */
