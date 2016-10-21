/*******************************************************************************
* File Name: TFT_MISO.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "TFT_MISO.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 TFT_MISO__PORT == 15 && ((TFT_MISO__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: TFT_MISO_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None
*  
*******************************************************************************/
void TFT_MISO_Write(uint8 value) 
{
    uint8 staticBits = (TFT_MISO_DR & (uint8)(~TFT_MISO_MASK));
    TFT_MISO_DR = staticBits | ((uint8)(value << TFT_MISO_SHIFT) & TFT_MISO_MASK);
}


/*******************************************************************************
* Function Name: TFT_MISO_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  TFT_MISO_DM_STRONG     Strong Drive 
*  TFT_MISO_DM_OD_HI      Open Drain, Drives High 
*  TFT_MISO_DM_OD_LO      Open Drain, Drives Low 
*  TFT_MISO_DM_RES_UP     Resistive Pull Up 
*  TFT_MISO_DM_RES_DWN    Resistive Pull Down 
*  TFT_MISO_DM_RES_UPDWN  Resistive Pull Up/Down 
*  TFT_MISO_DM_DIG_HIZ    High Impedance Digital 
*  TFT_MISO_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void TFT_MISO_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(TFT_MISO_0, mode);
}


/*******************************************************************************
* Function Name: TFT_MISO_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro TFT_MISO_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 TFT_MISO_Read(void) 
{
    return (TFT_MISO_PS & TFT_MISO_MASK) >> TFT_MISO_SHIFT;
}


/*******************************************************************************
* Function Name: TFT_MISO_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 TFT_MISO_ReadDataReg(void) 
{
    return (TFT_MISO_DR & TFT_MISO_MASK) >> TFT_MISO_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(TFT_MISO_INTSTAT) 

    /*******************************************************************************
    * Function Name: TFT_MISO_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 TFT_MISO_ClearInterrupt(void) 
    {
        return (TFT_MISO_INTSTAT & TFT_MISO_MASK) >> TFT_MISO_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
