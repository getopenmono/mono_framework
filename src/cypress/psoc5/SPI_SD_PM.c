/*******************************************************************************
* File Name: SPI_SD_PM.c
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

#include "SPI_SD_PVT.h"

static SPI_SD_BACKUP_STRUCT SPI_SD_backup =
{
    SPI_SD_DISABLED,
    SPI_SD_BITCTR_INIT,
};


/*******************************************************************************
* Function Name: SPI_SD_SaveConfig
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
void SPI_SD_SaveConfig(void) 
{

}


/*******************************************************************************
* Function Name: SPI_SD_RestoreConfig
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
void SPI_SD_RestoreConfig(void) 
{

}


/*******************************************************************************
* Function Name: SPI_SD_Sleep
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
*  SPI_SD_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPI_SD_Sleep(void) 
{
    /* Save components enable state */
    SPI_SD_backup.enableState = ((uint8) SPI_SD_IS_ENABLED);

    SPI_SD_Stop();
}


/*******************************************************************************
* Function Name: SPI_SD_Wakeup
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
*  SPI_SD_backup - used when non-retention registers are restored.
*  SPI_SD_txBufferWrite - modified every function call - resets to
*  zero.
*  SPI_SD_txBufferRead - modified every function call - resets to
*  zero.
*  SPI_SD_rxBufferWrite - modified every function call - resets to
*  zero.
*  SPI_SD_rxBufferRead - modified every function call - resets to
*  zero.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPI_SD_Wakeup(void) 
{
    #if(SPI_SD_RX_SOFTWARE_BUF_ENABLED)
        SPI_SD_rxBufferFull  = 0u;
        SPI_SD_rxBufferRead  = 0u;
        SPI_SD_rxBufferWrite = 0u;
    #endif /* (SPI_SD_RX_SOFTWARE_BUF_ENABLED) */

    #if(SPI_SD_TX_SOFTWARE_BUF_ENABLED)
        SPI_SD_txBufferFull  = 0u;
        SPI_SD_txBufferRead  = 0u;
        SPI_SD_txBufferWrite = 0u;
    #endif /* (SPI_SD_TX_SOFTWARE_BUF_ENABLED) */

    /* Clear any data from the RX and TX FIFO */
    SPI_SD_ClearFIFO();

    /* Restore components block enable state */
    if(0u != SPI_SD_backup.enableState)
    {
        SPI_SD_Enable();
    }
}


/* [] END OF FILE */
